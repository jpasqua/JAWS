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
    oled->setFont(ArialMT_Plain_10);
    oled->drawString(GUI::XCenter, 0, "Connect to WiFi");
    oled->setFont(ArialMT_Plain_16);
    oled->drawString(GUI::XCenter, 23, JAWS::SSID);
    oled->setFont(ArialMT_Plain_10);
    oled->setTextAlignment(TEXT_ALIGN_LEFT);
    oled->drawString(GUI::XCenter, 42, "Follow on-screen");
    oled->setTextAlignment(TEXT_ALIGN_RIGHT);
    oled->drawString(GUI::XCenter, 42, "Instructions");
    oled->display();

    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {  }

private:
};