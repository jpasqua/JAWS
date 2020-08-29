#ifndef HWConfig_h
#define HWConfig_h

#define CustomConfig        1
#define EmbeddedOLEDConfig  2
#define D1MiniConfig        3

#define HWConfig "Please select a Hardware Condiguration in HWConfig.h"

/*------------------------------------------------------------------------------
 *
 * Config Info for D1Mini with 1.4" SH1106 display
 *
 *----------------------------------------------------------------------------*/
#if (HWConfig == D1MiniConfig)
  // ----- Display Type
  #define DISPLAY_OBJ   SH1106Wire
  // ----- I2C Settings
  const int SDA_PIN = D2;
  const int SCL_PIN = D5;
  const int I2C_DISPLAY_ADDRESS = 0x3c;
  // ----- Button
  const int BUTTON_LOW_PIN = D8;  // "Spare" Ground since there isn't an extra ground pin handy
  const int BUTTON_PIN = D6;


/*------------------------------------------------------------------------------
 *
 * Config Info for Wemos board with embedded 0.96" OLED
 *
 *----------------------------------------------------------------------------*/
#elif (HWConfig == EmbeddedOLEDConfig)
  // ----- Display Type
  #define DISPLAY_OBJ   SSD1306Wire
  // ----- I2C Settings
  const int SDA_PIN = D1;
  const int SCL_PIN = D2;
  const int I2C_DISPLAY_ADDRESS = 0x3c;
  // ----- Button
  const int BUTTON_LOW_PIN = -1;  // Using a regular ground pin - this isn't needed
  const int BUTTON_PIN = D3;


/*------------------------------------------------------------------------------
 *
 * Custom Config - put your settings here
 *
 *----------------------------------------------------------------------------*/
#elif (HWConfig ==  CustomConfig)
  // ----- Display Type
  #define DISPLAY_OBJ   SH1106Wire
  // #define DISPLAY_OBJ   SSD1306Wire
  // ----- I2C Settings
  const int SDA_PIN = D2;
  const int SCL_PIN = D5;
  //       --- OR ---
  // const int SDA_PIN = D1;
  // const int SCL_PIN = D2;
  const int I2C_DISPLAY_ADDRESS = 0x3c; // I2C Address of your Display (usually 0x3c or 0x3d)
  // ----- Button
  const int BUTTON_LOW_PIN = D8;      // "Spare" Ground. Use -1 when using the actual ground pin
  const int BUTTON_PIN = D6;          // Button to accept input
  //       --- OR ---
  // const int BUTTON_LOW_PIN = -1;  // "Spare" Ground. Use -1 when using the actual ground pin
  // const int BUTTON_PIN = D3;      // Button to accept input
#endif


/*------------------------------------------------------------------------------
 *
 * Sensor Configuration
 *
 *----------------------------------------------------------------------------*/

// There may be a separate DS18B20 attached to provide more accurate and stable
// temperature readings. If so, provide the pin to which it is attached. If not,
// set the pin to -1.
const int DS18B20_PIN = -1;           // -1 to indicate no DS18B20 (which is typical)
//const int DS18B20_PIN = D3;         // Specify a pin to enable DS18B20 support


#endif  // HWConfig_h
