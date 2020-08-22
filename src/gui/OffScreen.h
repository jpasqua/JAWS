//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class OffScreen : public Screen {
public:
  OffScreen() { }

  void display(bool activating = false) {
    (void)activating; // We don't use this parameter - avoid a warning...

    alreadyOff = false;
    GUIPackage::resetDisplay();
    oled->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    oled->setFont(ArialMT_Plain_24);
    oled->drawString(GUI::XCenter, GUI::YCenter, "Off...");
    oled->display();

    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {
    if (!alreadyOff && (millis() > timeOfLastDisplay + OffTimeDelay)) {
      oled->displayOff();
      alreadyOff = true;
    }
  }

private:
  static const uint32_t OffTimeDelay = 2000L;
  bool alreadyOff;
};