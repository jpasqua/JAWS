/*
 * JAWSSettings
 *    Handle reading and writing settings information to the file system
 *    in JSON format.
 *
 * NOTES:
 *
 */

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
#include <FS.h>
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <ArduinoJson.h>
//                                  Local Includes
#include "JAWSSettings.h"
//--------------- End:    Includes ---------------------------------------------

const uint32_t  JAWSSettings::CurrentVersion = 0x0003;

JAWSSettings::JAWSSettings() {
  version = JAWSSettings::CurrentVersion;
  maxFileSize = 1024;
}

void JAWSSettings::fromJSON(JsonDocument &doc) {
  description = doc["description"].as<String>();
  useMetric = doc["useMetric"];
  showDevMenu = doc["showDevMenu"];
  blynkAPIKey = doc["blynkAPIKey"].as<String>();
  tempCorrection = doc["tempCorrection"];
  humiCorrection = doc["humiCorrection"];
  chartColors.temp = String(doc["chartColors"]["temp"]|"#4e7a27");
  chartColors.avg = String(doc["chartColors"]["avg"]|"#ff00ff");

  logSettings();
}

void JAWSSettings::toJSON(JsonDocument &doc) {
  doc["description"] = description;
  doc["useMetric"] = useMetric;
  doc["showDevMenu"] = showDevMenu;
  doc["blynkAPIKey"] = blynkAPIKey;
  doc["tempCorrection"] = tempCorrection;
  doc["humiCorrection"] = humiCorrection;
  doc["chartColors"]["temp"] = chartColors.temp;
  doc["chartColors"]["avg"] = chartColors.avg;
}

void JAWSSettings::logSettings() {
  Log.verbose("JAWS Settings");
  Log.verbose("  description = %s", description.c_str());
  Log.verbose("  useMetric = %T", useMetric);
  Log.verbose("  showDevMenu = %T", showDevMenu);
  Log.verbose("  blynkAPIKey = %s", blynkAPIKey.c_str());
  Log.verbose("  tempCorrection = %F", tempCorrection);
  Log.verbose("  humiCorrection = %F", humiCorrection);
  Log.verbose("  tempColor = %s", chartColors.temp.c_str());
  Log.verbose("  avgColor  = %s", chartColors.avg.c_str());
}

