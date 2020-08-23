//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class ConfigScreen : public Screen {
public:
  ConfigScreen() { }

  void display(bool activating = false) {
    (void)activating; // We don't use this parameter - avoid a warning...

    oled->clear();
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    oled->setFont(ArialMT_Plain_16);
    oled->drawString(64, 0, "Connect to WIFi:");
    oled->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    oled->setFont(ArialMT_Plain_24);
    // TEXT_ALIGN_CENTER_BOTH centers on the overall font height. For this, I want to
    // center on the baseline, hence I use 36 as the center instead of 32 (=64/2+4)
    oled->drawString(64, 36, JAWS::SSID);
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    oled->setFont(ArialMT_Plain_10);
    oled->drawString(64, 52, "then choose your network");
    oled->display();

    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {  }

private:
};