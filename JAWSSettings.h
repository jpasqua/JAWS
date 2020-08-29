/*
 * Settings.h
 *    Defines the values that can be set through the web UI and sets their initial values
 *
 */

#ifndef JAWSSettings_h
#define JAWSSettings_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
#include <BaseSettings.h>
//                                  Local Includes
//--------------- End:    Includes ---------------------------------------------


class JAWSSettings: public BaseSettings {
public:
  // ----- Constructors and methods
  JAWSSettings();
  void fromJSON(JsonDocument &doc) override;
  void toJSON(JsonDocument &doc);
  void logSettings();

  // ----- Settings
  String  description = "Weather Station";  // User's description of their weather station
  bool    useMetric = false;                // Imperial or Metric
  bool    hasGUI = false;                   // Is there a display attached to present a GUI
  String  blynkAPIKey = "";                 // Your API Key from Blynk
  float   tempCorrection = 0.0;             // Correction to the temp reading - always in Celsius
  float   humiCorrection = 0.0;             // Correction to the humidty sensor reading
  bool    showDevMenu = false;

private:
  // ----- Constants -----
  static const uint32_t CurrentVersion;
};
#endif // JAWSSettings_h