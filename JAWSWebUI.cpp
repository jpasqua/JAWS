/*
 * WebUI:
 *    Implements a simple WebUI for a WebThing. It contains the basic
 *    structure that can be extended to incorporate "thing-specific" UI
 *                    
 * NOTES:
 *   
 * TO DO:
 * o Implement /weather endpoint for the homebridge-http-temperature-humidity
 *   plugin. May require changes to WebThing::WebUI if not all required functionality
 *   is exposed (e.g. header handling).
 *
 */


//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <ESP8266WebServer.h>
//                                  Third Party Libraries
#include <ArduinoLog.h>
//                                  Local Includes
#include "JAWS.h"
#include "JAWSWebUI.h"
#include "src/gui/GUI.h"
//--------------- End:    Includes ---------------------------------------------



// ----- BEGIN: WebUI namespacea
namespace JAWSWebUI {
  static const String   checkedOrNot[2] = {"", "checked='checked'"};
  ESPTemplateProcessor  *templateHandler;

  // ----- BEGIN: JAWSWebUI::Internal
  namespace Internal {
    String Actions =
      "<a class='w3-bar-item w3-button' href='/displayJAWSConfig'>"
      "<i class='fa fa-cog'></i> Configure JAWS</a>"
      "<a class='w3-bar-item w3-button' href='/takeReadings'>"
      "<i class='fa fa-thermometer-three-quarters'></i> Take readings</a>";
    String DEV_ACTION =
      "<a class='w3-bar-item w3-button' href='/dev'>"
      "<i class='fa fa-gears'></i> Dev Settings</a>";  }
  // ----- END: JAWSWebUI::Internal


  // ----- BEGIN: JAWSWebUI::Endpoints
  namespace Pages {

    // Displays the home page which shows the last set of weather readings.
    //
    // Form:
    //    GET  /displayHomePage
    //
    void displayHomePage() {
      Log.trace("Web Request: Display Home Page");
      if (!WebUI::authenticationOK()) { return; }

      auto mapper =[](String &key) -> String {
        if (key == "GMAPS_KEY") return WebThing::settings.googleMapsKey;
        if (key == "LAT")  return WebThing::settings.latAsString();
        if (key == "LNG")  return WebThing::settings.lngAsString();
        if (key == "TEMP") return (String(JAWS::outputTemp(JAWS::readings.temp), 1) + JAWS::tempUnits());
        if (key == "HUMI") return (String(JAWS::readings.humidity, 1) + "%");
        if (key == "BARO") return (String(JAWS::outputBaro(JAWS::readings.pressure), 1) + JAWS::baroUnits());
        if (key == "RELP") return (String(JAWS::outputBaro(JAWS::readings.relPressure), 1) + JAWS::baroUnits());
        if (key == "HTIN") return (String(JAWS::outputTemp(JAWS::readings.heatIndex), 1) + JAWS::tempUnits());
        if (key == "DWPT") return (String(JAWS::outputTemp(JAWS::readings.dewPointTemp), 1) + JAWS::tempUnits());
        if (key == "DPSP") return (String(JAWS::tempSpread(JAWS::readings.dewPointSpread), 1) + JAWS::tempUnits());
        if (key == "VLTG") {
          float voltage = WebThing::measureVoltage();
          if (voltage == -1) return "N/A";
          return (String(voltage, 2) + "V");
        }
        if (key == "TMST") return String(JAWS::timeOfLastReading());
        return "";
      };

      WebUI::startPage();
      templateHandler->send("/HomePage.html", mapper);
      WebUI::finishPage();
    }

    // Displays a form allowing the user to update the JAWS settings.
    //
    // Form:
    //    GET  /displayJAWSConfig
    //
    void displayJAWSConfig() {
      Log.trace("Web Request: Display Config");
      if (!WebUI::authenticationOK()) { return; }

      auto mapper =[](String &key) -> String {
        if (key == "DESC") return WebThing::encodeAttr(JAWS::settings.description);
        if (key == "USE_METRIC")  return checkedOrNot[JAWS::settings.useMetric];
        if (key == "BLYNK_KEY")  return JAWS::settings.blynkAPIKey;
        if (key == "TEMP_CORRECT") return String(JAWS::settings.tempCorrection);
        if (key == "HUMI_CORRECT") return String(JAWS::settings.humiCorrection);
        return "";
      };

      WebUI::startPage();
      templateHandler->send("/ConfigForm.html", mapper);
      WebUI::finishPage();
    }
  }   // ----- END: JAWSWebUI::Pages


  namespace Endpoints {
    // Handler for the "/takeReadings" endpoint. This is used to force JAWS to
    // take a new set of readings regardless of the specified interval at which
    // readings are normally taken.
    //
    // Form:
    //    GET  /takeReadings
    //
    void updateReadings() {
      if (!WebUI::authenticationOK()) { return; }
      JAWS::processReadings();
      WebUI::redirectHome();
    }

    // Handler for the "/updateJAWSConfig" endpoint. This is invoked as the target
    // of the form presented by "/displayJAWSConfig". It updates the values of the
    // corresponding settings and writes the settings to EEPROM.
    //
    // TO DO: Call a function to let the main app know that settings may have changed
    //        so that it can take any appropriate actions
    //
    // Form:
    //    GET /updateJAWSConfig?description=DESC&useMetric=BOOL&...
    //
    void updateJAWSConfig() {
      if (!WebUI::authenticationOK()) { return; }
      Log.trace("JAWSWebUI: Handle Update Config");

      JAWS::settings.description = WebUI::arg("description");
      JAWS::settings.useMetric = WebUI::hasArg("useMetric");
      JAWS::settings.blynkAPIKey = WebUI::arg("blynkAPIKey");
      JAWS::settings.tempCorrection = WebUI::arg("tempCorrection").toFloat();
      JAWS::settings.humiCorrection = WebUI::arg("humiCorrection").toFloat();
      JAWS::settings.write();

      // The description MAY have changed. Update the title just in case
      WebUI::setTitle(JAWS::settings.description+" ("+WebThing::settings.hostname+")");

      WebUI::redirectHome();
    }

    // Handler for the "/weather" endpoint. This is used by the HomeBridge
    // plugin homebridge-http-temperature-humidity to get temp and humidity
    // data. Temp is always in celsius. It returns a JSON object of the form:
    //    { "temperature": 25.8, "humidity": 38 }
    //
    // Form:
    //    GET  /weather
    //
    void weather() {
      ESP8266WebServer *server = WebUI::getUnderlyingServer();

      String json = "{";
      json += "\"temperature\": ";
      json += JAWS::readings.temp;
      json += ", ";
      json += "\"humidity\": ";
      json += JAWS::readings.humidity;
      json += "}";
      server->sendHeader("Cache-Control", "private, no-store");
      server->send(200, "application/json", json);
    }
  }   // ----- END: JAWSWebUI::Endpoints


  namespace Dev {
    void dumpArgs() {
      ESP8266WebServer* server = WebUI::getUnderlyingServer();
      int nArgs = server->args();
      Log.verbose("“Number of args received: %d", nArgs);
      for (int i = 0; i < nArgs; i++) {
        Log.verbose("arg(%d): %s = %s", i, server->argName(i).c_str(), server->arg(i).c_str());
      } 
    }

    void updateSettings() {
      if (!WebUI::authenticationOK()) { return; }
      Log.trace("Web Request: /dev/updateSettings");

      JAWS::settings.showDevMenu = WebUI::hasArg("showDevMenu");
      JAWS::settings.write();

      WebUI::redirectHome();
    }

    void yieldScreenShot() {
      Log.trace(F("Web Request: /dev/screenShot"));
      if (!WebUI::authenticationOK()) { return; }

      WebUI::sendArbitraryContent("image/bmp", GUI::getSizeOfScreenShotAsBMP(), GUI::streamScreenShotAsBMP);
    }

    void displayDevPage() {
      Log.trace(F("Web Request: /dev/displayDevPage"));
      if (!WebUI::authenticationOK()) { return; }

      auto mapper =[](String &key) -> String {
        if (key == "SHOW_DEV_MENU") return checkedOrNot[JAWS::settings.showDevMenu];
        return "";
      };

      WebUI::startPage();
      templateHandler->send("/DevPage.html", mapper);
      WebUI::finishPage();
    }

    void reboot() {
      if (!WebUI::authenticationOK()) { return; }
      ESP.restart();
    }

    void yieldSettings() {
      Log.trace(F("Web Request: /dev/settings"));
      if (!WebUI::authenticationOK()) { return; }

      DynamicJsonDocument *doc = (WebUI::hasArg("wt")) ? WebThing::settings.asJSON() :
                                                         JAWS::settings.asJSON();
      WebUI::sendJSONContent(doc);
      doc->clear();
      delete doc;
    }

    void forceScreen() {
      Log.trace(F("Web Request: /dev/forceScreen"));
      if (!WebUI::authenticationOK()) { return; }
      String screen = WebUI::arg(F("screen"));
      if (screen.isEmpty()) return;

      if (screen == F("wifi")) GUI::showScreen(GUI::ScreenName::WiFi);
      else if (screen == F("splash")) GUI::showScreen(GUI::ScreenName::Splash);
      else if (screen == F("config")) {
        JAWS::SSID = "jaws60750b";
        GUI::showScreen(GUI::ScreenName::Config);
      }
      WebUI::redirectHome();
    }
  }   // ----- END: JAWSWebUI::Dev


  void init() {
    WebUI::setTitle(JAWS::settings.description+" ("+WebThing::settings.hostname+")");

    String actions = Internal::Actions;
    if (JAWS::settings.showDevMenu) {
      actions += Internal::DEV_ACTION;
    }
    WebUI::addMenuItems(actions);

    WebUI::registerHandler("/",                   Pages::displayHomePage);
    WebUI::registerHandler("/displayJAWSConfig",  Pages::displayJAWSConfig);

    WebUI::registerHandler("/updateJAWSConfig",   Endpoints::updateJAWSConfig);
    WebUI::registerHandler("/takeReadings",       Endpoints::updateReadings);
    WebUI::registerHandler("/weather",            Endpoints::weather);

    WebUI::registerHandler("/dev",                Dev::displayDevPage);
    WebUI::registerHandler("/dev/reboot",         Dev::reboot);
    WebUI::registerHandler("/dev/settings",       Dev::yieldSettings);
    WebUI::registerHandler("/dev/screenShot",     Dev::yieldScreenShot);
    WebUI::registerHandler("/dev/forceScreen",    Dev::forceScreen);
    WebUI::registerHandler("/dev/updateSettings", Dev::updateSettings);

    templateHandler = WebUI::getTemplateHandler();
  }

}
// ----- END: JAWSWebUI
