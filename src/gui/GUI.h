/*
 * GUI:
 *    Provide a local interface for weather readings
 *
 */

#ifndef GUI_h
#define GUI_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
//                                  Local Includes
#include "../../JAWS.h"
#include "Screen.h"
//--------------- End:    Includes ---------------------------------------------

namespace GUI {
  static const uint16_t Width = 128;
  static const uint16_t Height = 64;
  static const uint16_t XCenter = Width/2;
  static const uint16_t YCenter = Height/2;

  typedef enum {WiFi, Config, Splash, Time, Temp, Humi, Baro, All, Graph, Info, Off, N_Screens, NoScreen} ScreenName;

  void showScreen(ScreenName which);
  void init(bool flipDisplay = false);
  void setOrientation(bool flip);


  void loop();
  void noteNewSample(float val);

  /**
   * Dump the pixel values from the display to the specified stream in the form
   * of a BMP image. This can be used to take "screen shots".
   * @param s     The Stream to which the screen shot in BMP format should be written
   */
  void streamScreenShotAsBMP(Stream &s);
  uint32_t getSizeOfScreenShotAsBMP();
} // ----- END: GUI namespace

#endif  // GUI_h