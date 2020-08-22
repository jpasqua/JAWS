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

  typedef enum {WiFi, Config, Splash, Time, Temp, Humi, Baro, All, Graph, Off, N_Screens, NoScreen} ScreenName;

  void showScreen(ScreenName which);
  void init(bool flipDisplay = false);
  void setOrientation(bool flip);

  void loop();
  void noteNewSample(float val);
} // ----- END: GUI namespace

#endif  // GUI_h