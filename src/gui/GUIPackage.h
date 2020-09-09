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
#include <OLEDDisplay.h>
//                                  Local Includes
#include "../../HWConfig.h"
//--------------- End:    Includes ---------------------------------------------


namespace GUIPackage {
  //
  // Constants
  //
  static const uint8_t FmtBufSize = 32;  // Size of the temp graph data

  //
  // Fonts
  //
  extern const uint8_t DSEG7_Classic_Bold_32[];

  extern OLEDDisplay *oled;
  extern char fmtBuf[FmtBufSize];

  //
  // Functions
  //
  void resetDisplay();

} // ----- END: GUIPackage namespace

#endif  // GUIPackage_h