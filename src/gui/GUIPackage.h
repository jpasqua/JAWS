/*
 * GUIPackage:
 *    Types, Constants, State and methods for internal use by the GUI package
 *
 */

#ifndef GUIPackage_h
#define GUIPackage_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Wire.h>
#include <SPI.h>                    // Expected by AdaFruit_GFX, not used here
//                                  Third Party Libraries
#include <SH1106Wire.h>
#include <SSD1306Wire.h>
//                                  Local Includes
#include "../../HWConfig.h"
//--------------- End:    Includes ---------------------------------------------


namespace GUIPackage {
  //
  // Types
  //
  typedef enum {Wifi, Config, Splash, Temp, Humi, Real, All, Graph, Time, Off, N_Screens, NoScreen} ScreenName;

  //
  // Constants
  //
  static const int     MaxSamples = 128; // Size of the temp graph data
  static const uint8_t FmtBufSize = 32;  // Size of the temp graph data

  //
  // Fonts
  //
  extern const uint8_t DSEG7_Classic_Bold_32[];

  extern DISPLAY_OBJ *oled;
  extern char fmtBuf[FmtBufSize];

  //
  // Functions
  //
  void resetDisplay();

} // ----- END: GUIPackage namespace

#endif  // GUIPackage_h