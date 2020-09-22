//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class AllReadingsScreen : public Screen {
public:
  AllReadingsScreen() { }

  void display(bool activating = false) {
    (void)activating; // We don't use this parameter - avoid a warning...

    GUIPackage::resetDisplay();
    oled->setFont(ArialMT_Plain_16);
    oled->setTextAlignment(TEXT_ALIGN_LEFT);
    snprintf(
        GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Temp: %.1f%s",
        JAWS::outputTemp(JAWS::readings.temp), JAWS::tempUnits().c_str());
    oled->drawString(0, 5, GUIPackage::fmtBuf);
    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Humi: %.0f%%", JAWS::readings.humidity);
    oled->drawString(0, 24, GUIPackage::fmtBuf);
    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Baro: %.1f%s",
        JAWS::outputBaro(JAWS::readings.pressure), JAWS::baroUnits().c_str());
    oled->drawString(0, 43, GUIPackage::fmtBuf);
    oled->display();

    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {
    if (JAWS::readings.timestamp > timeOfLastDisplay) display();
  }

private:
};