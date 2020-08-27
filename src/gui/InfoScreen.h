//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <FS.h>
#include <ESP8266WiFi.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class InfoScreen : public Screen {
public:
  InfoScreen() { }

  void display(bool activating = false) {
    (void)activating; // We don't use this parameter - avoid a warning...

    GUIPackage::resetDisplay();
    oled->setFont(ArialMT_Plain_16);
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    snprintf(
        GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "JAWS v%s", JAWS::Version.c_str());
    oled->drawString(GUI::XCenter, 0, GUIPackage::fmtBuf);

    oled->setFont(ArialMT_Plain_10);
    snprintf(
        GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "%s", WebThing::settings.hostname.c_str());
    oled->drawString(GUI::XCenter, 24, GUIPackage::fmtBuf);

    snprintf(
        GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "%s", WiFi.localIP().toString().c_str());
    oled->drawString(GUI::XCenter, 37, GUIPackage::fmtBuf);

    // snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Something else: %s", someString);
    // oled->drawString(0, 50, GUIPackage::fmtBuf);
    
    drawRssi();

    oled->display();

    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() { }

private:
  void drawRssi() {
    int8_t quality = WebThing::wifiQualityAsPct();
    for (int8_t i = 0; i < 4; i++) {
      for (int8_t j = 0; j < 3 * (i + 2); j++) {
        if (quality > i * 25 || j == 0) {
          oled->setPixel(114 + 4 * i, 63 - j);
        }
      }
    }
  }  
};