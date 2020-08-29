#ifndef BlynkClient_h
#define BlynkClient_h

#include "JAWS.h"


namespace JAWSBlynk {
  // ----- Constants
  static const int BlynkTempPin         = 0;
  static const int BlynkHumidityPin     = 1;
  static const int BlynkPressurePin     = 2;
  static const int BlynkRelPressurePin  = 3;
  static const int BlynkDewPointPin     = 4;
  static const int BlynkDewSpreadPin    = 5;
  static const int BlynkHeatIndexPin    = 6;
  static const int BlynkVoltagePin      = 7;
  static const int BlynkTimestampPin    = 8;

  // ----- State
  bool connectedToBlynk = false;

  // ----- Methods
  void init() {
    Log.trace("Connecting to Blynk");
    if (JAWS::settings.blynkAPIKey.length() != 0) {
      Blynk.config(JAWS::settings.blynkAPIKey.c_str());
      connectedToBlynk = Blynk.connect(30*1000L);
      if (connectedToBlynk) { Log.trace("Successfully connected to Blynk"); }
      else { Log.warning("Unable to connect to Blynk"); }
    } else {
      Log.notice("No Blynk API key was specified.");
    }
  }

  void run() {
    if (connectedToBlynk) Blynk.run();
  }

  void update() {
    if (!connectedToBlynk) return;
    Log.trace("About to update Blynk...");
    float voltage = WebThing::measureVoltage();
    if (voltage == -1) voltage = 5.0;

    Blynk.virtualWrite(BlynkTempPin,        JAWS::outputTemp(JAWS::readings.temp));
    Blynk.virtualWrite(BlynkHumidityPin,    JAWS::readings.humidity);
    Blynk.virtualWrite(BlynkPressurePin,    JAWS::outputBaro(JAWS::readings.pressure));
    Blynk.virtualWrite(BlynkRelPressurePin, JAWS::outputBaro(JAWS::readings.relPressure));
    Blynk.virtualWrite(BlynkDewPointPin,    JAWS::outputTemp(JAWS::readings.dewPointTemp));
    Blynk.virtualWrite(BlynkDewSpreadPin,   JAWS::tempSpread(JAWS::readings.dewPointSpread));
    Blynk.virtualWrite(BlynkHeatIndexPin,   JAWS::outputTemp(JAWS::readings.heatIndex));
    Blynk.virtualWrite(BlynkVoltagePin,     voltage);
    if (timeStatus() == timeSet) {
      char *dateTime = JAWS::timeOfLastReading();
      Log.verbose("Timestamp sent to Blynk: %s", dateTime);
      Blynk.virtualWrite(BlynkTimestampPin, dateTime);
    }
    Log.trace("Blynk update complete");
  }

  void disconnect() {
    if (connectedToBlynk) {
      Blynk.run();
      Blynk.disconnect();
    }
  }

}

#endif // BlynkClient_h
