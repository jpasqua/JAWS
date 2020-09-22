//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
#include "SplashBitmap.h"
#include "WiFiBitmap.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class BitmapScreen : public Screen {
public:
  BitmapScreen() { }

  void display(bool activating, const uint8_t *xbm) {
    (void)activating; // We don't use this parameter - avoid a warning...

    oled->clear();
    oled->drawXbm(0, 0, GUI::Width, GUI::Height, xbm);
    oled->display();
  }

  virtual void processPeriodicActivity() { }
};

class SplashScreen : public BitmapScreen {
public:
  void display(bool activating = false) { 
    BitmapScreen::display(activating, SplashBitmap);
  }
};

class WiFiScreen : public BitmapScreen {
public:
  void display(bool activating = false) { 
    BitmapScreen::display(activating, WiFiBitmap);
  }
};