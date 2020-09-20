/*
 * JAWS - Just Another Weather Station
 *    Monitor weather sensors and upload data to the Blynk Service
 *                    
 * NOTES:
 * o To force the device to ignore 'useLowPowerMode' and always stay on,
 *   place a jumper between D7 and ground.
 *
 */


//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <ESP8266mDNS.h>
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WebUI.h>
//                                  Local Includes
#include "JAWS.h"
#include "HWConfig.h"
#include "JawsWebUI.h"
#include "JAWSBlynk.h"
#include "src/gui/GUI.h"
#include "src/clients/BMESensor.h"
#include "src/clients/DS18B20.h"
//--------------- End:    Includes ---------------------------------------------


namespace JAWS {
  /*------------------------------------------------------------------------------
   *
   * Global State
   *
   *----------------------------------------------------------------------------*/
  BMESensor bme;
  DS18B20*  tempSensor;
  Readings  readings;

  JAWSSettings settings;
  String SSID = "";
  const String Version = "0.3.0";

  namespace History {
    const     uint8_t MaxSamples = 128;
    float     samples[MaxSamples];  // Circular buffer of samples
    uint32_t  times[MaxSamples];    // Circular buffer of timestamps
    uint8_t   nSamples = 0;         // Sample Data: Number of samples stored so far
    int       sampleHead = 0;       // Sample Data: Location to store the next sample

    float getSample(uint8_t index) {
      if (index >= nSamples) return 0.0f; // Index out of range
      if (nSamples == MaxSamples) index = (sampleHead + index) % MaxSamples;
      return samples[index];
    }

    void getSample(uint8_t index, float& sample, uint32_t& timestamp){
      if (index >= nSamples) { sample = 0.0; timestamp = 0; return; } // Index out of range
      if (nSamples == MaxSamples) index = (sampleHead + index) % MaxSamples;
      sample = samples[index];
      timestamp = times[index];
    }

    void appendSample(float value) {
      samples[sampleHead] = value;
      times[sampleHead] = now();
      sampleHead = (sampleHead + 1) % MaxSamples;
      if (nSamples != MaxSamples) nSamples++;
    }
  }

  namespace Internal {
    static const String   SettingsFileName = "/settings.json";

    void ensureWebThingSettings() {
      // JAWS has a fixed hardware configuration so some of the settings need to have
      // specific values. Rather than make the user configure them, set them to the
      // right values here.
      WebThing::settings.sleepOverridePin = 13;     // D7
      // WebThing::settings.hasVoltageSensing = true;
      if (WebThing::settings.hostname.isEmpty()) WebThing::settings.hostname = ("jaws" + String(ESP.getChipId(), HEX));
    }

    void flushBeforeSleep() { JAWSBlynk::disconnect(); }

    void prepIO() {
      Wire.begin(SDA_PIN, SCL_PIN);
    }

    void prepSensors() {
      bme.setAttributes(                // Set up sensor attributes and initialize
        settings.tempCorrection,
        settings.humiCorrection,
        WebThing::settings.elevation);
      bme.begin(BME_I2C_ADDR);  
      if (DS18B20_PIN != -1) {  // Optional temperature sensor
        tempSensor = new DS18B20();
        tempSensor->begin(DS18B20_PIN, settings.tempCorrection);
      } else tempSensor = NULL;
    }

    void prepWebUI() {
      WebThing::setup();
      WebThing::notifyBeforeDeepSleep(flushBeforeSleep);
      JAWSWebUI::init();
    }

    void baseConfigChange() { WebUI::setTitle(settings.description+" ("+WebThing::settings.hostname+")"); }

    void configModeCallback(String &ssid, String &ip) {
      (void)ip; // We don't use this parameter - avoid a warning
      SSID = ssid;
      GUI::showScreen(GUI::ScreenName::Config);
    }
  } // ----- END: JAWS::Internal namespace

  /*------------------------------------------------------------------------------
   *
   * Exported Functions
   *
   *----------------------------------------------------------------------------*/
  float outputTemp(float temp) { return (settings.useMetric) ? temp : (temp*1.8)+32; }
  float tempSpread(float spread) { return (settings.useMetric) ? spread : (spread*1.8); }
  float outputBaro(float baro) { return (settings.useMetric) ? baro : (baro/33.863887); }
  String tempUnits() { return (settings.useMetric) ? "°C" : "°F"; }
  String baroUnits() { return (settings.useMetric) ? "hPa" : "inHg"; }
  char *formattedTime(time_t theTime) {
    static char dateTime[19];
    sprintf(dateTime, "%d/%02d/%02d %02d:%02d:%02d", 
      year(theTime), month(theTime), day(theTime), hour(theTime), minute(theTime), second(theTime));
    return dateTime;
  }
  char *timeOfLastReading() {
    return formattedTime(now() - (millis() - readings.timestamp)/1000L);
  }

  void updateCorrections(float t, float h) {
      bme.setAttributes(                // Set up sensor attributes and initialize
        settings.tempCorrection,
        settings.humiCorrection,
        WebThing::settings.elevation);
      if (tempSensor) { tempSensor->setAttributes(settings.tempCorrection); }
    }

  void processReadings() {
    bme.takeReadings(readings);
    if (tempSensor != NULL) {
      float temp = tempSensor->getTempC();
      if (temp == DS18B20::ReadingError) {
        Log.warning("Error reading from DS18B20");
      } else {
        Log.trace("Overriding reading from BME (%F) with DS (%F)", readings.temp, temp);
        readings.temp = temp;
      }
    }
    readings.timestamp = millis();
    JAWSBlynk::update();
    History::appendSample(readings.temp);
  }

} // ----- END: JAWS namespace


/*------------------------------------------------------------------------------
 *
 * GLOBAL SCOPE: setup() and loop()
 *
 * The setup() and loop() functions need to be in the global scope, but are logically
 * part of the JAWS namespace. Use a "using" directive for JUST THESE 2 functions
 * to treat them as if they were in that namespace.
 *
 *----------------------------------------------------------------------------*/

using namespace JAWS;

void setup() {
  WebThing::preSetup();             // Setup the WebThing - Must be first
  WebThing::setDisplayedVersion(Version);

  settings.init(Internal::SettingsFileName);  
  settings.read();
  Internal::prepIO();               // Prepare any I/O pins used locally

  // Only start the GUI if we have a display attached and are *not* in power saver mode
  GUI::init(true);

  Internal::ensureWebThingSettings();         // Override any pertinent settings in WebThing
  WebThing::notifyConfigChange(Internal::baseConfigChange);
  WebThing::notifyOnConfigMode(Internal::configModeCallback);

  GUI::showScreen(GUI::ScreenName::WiFi);
  Internal::prepWebUI();            // Setup the WebUI, network, etc.
  GUI::showScreen(GUI::ScreenName::Splash);

  Internal::prepSensors();

  JAWSBlynk::init();                // Setup the Blynk Client

  processReadings();                // Get our first set of readings!
  GUI::showScreen(GUI::ScreenName::Time);

  WebThing::postSetup();            // Finalize setup of the WebThing - Must be last
}

void loop() {
  static uint32_t lastSampleTime = 0;

  WebThing::loop();
  JAWSBlynk::run();
  GUI::loop();

  uint32_t curMillis = millis();
  if (curMillis - lastSampleTime > (WebThing::settings.processingInterval * 60 * 1000L)) {
    processReadings();
    lastSampleTime = curMillis;
  }
}

