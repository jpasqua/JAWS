#ifndef HWConfig_h
#define HWConfig_h

#include <Arduino.h>

// ----- BEGIN: Constants used in the rest of the file. Do not change these
// An enumeration of allowable values for HWConfig
#define Config_Custom        1
#define Config_EmbeddedOLED  2
#define Config_D1Mini        3

// An enumeration of allowable values for DISPLAY_DRIVER
#define SH1106  1
#define SSD1306 2
// ----- END: Constants used in the rest of the file

// Pick a configuration. Use Config_Custom to define your own
#define HWConfig Config_EmbeddedOLED

// If there is an attached display for a GUI, set to 1, otherwise 0
#define HAS_GUI   1


#if (HWConfig == Config_D1Mini)
  /*------------------------------------------------------------------------------
   *
   * Config Info for D1Mini with 1.4" SH1106 display
   *
   *----------------------------------------------------------------------------*/
  // ----- I2C Settings
  static const int SDA_PIN = D2;
  static const int SCL_PIN = D5;

  // ----- Sensors
  static const int BME_I2C_ADDR = 0x76;
  static const int DS18B20_PIN = -1;      // -1 to indicate no DS18B20 (which is typical)

  #if (HAS_GUI == 1)
    // ----- Display Type
    #define DISPLAY_DRIVER   SH1106
    static const int DISPLAY_I2C_ADDRESS = 0x3c;
    // ----- Button
    static const int BUTTON_PIN = D6;
    static const int BUTTON_LOW_PIN = D8; // "Spare" Ground since there isn't an extra ground pin handy
  #endif

#elif (HWConfig == Config_EmbeddedOLED)
  /*------------------------------------------------------------------------------
   *
   * Config Info for Wemos board with embedded 0.96" OLED
   *
   *----------------------------------------------------------------------------*/
  // ----- I2C Settings
  static const int SDA_PIN = D1;
  static const int SCL_PIN = D2;

  // ----- Sensors
  static const int BME_I2C_ADDR = 0x76;
  static const int DS18B20_PIN = -1;

  #if (HAS_GUI == 1)
    // ----- Display Type
    #define DISPLAY_DRIVER   SSD1306
    static const int DISPLAY_I2C_ADDRESS = 0x3c;
    // ----- Button
    static const int BUTTON_PIN = D3;
    static const int BUTTON_LOW_PIN = -1;  // Using a regular ground pin - this isn't needed
  #endif

#elif (HWConfig ==  Config_Custom)
  /*------------------------------------------------------------------------------
   *
   * Custom Config - put your settings here
   *
   *----------------------------------------------------------------------------*/
  // ----- I2C Settings
  static const int SDA_PIN = D1;  // Substitute the correct Pin for your config
  static const int SCL_PIN = D2;  // Substitute the correct Pin for your config

  // ----- Sensors
  static const int BME_I2C_ADDR = 0x76;
  static const int DS18B20_PIN = -1;  // -1 -> No DS18B20, which is typical

  #if (HAS_GUI == 1)
    // ----- Display Type
    #define DISPLAY_DRIVER   SSD1306
    // #define DISPLAY_DRIVER   SSD1306
    static const int DISPLAY_I2C_ADDRESS = 0x3c;
    // ----- Button
    static const int BUTTON_PIN = D3;
    static const int BUTTON_LOW_PIN = -1;  // Using a regular ground pin - this isn't needed
  #endif
#else
    #error "Please set HWConfig"
#endif

#endif  // HWConfig_h
