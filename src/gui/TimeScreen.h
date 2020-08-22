//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
#include <TimeLib.h>
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class TimeScreen : public Screen {
public:
  TimeScreen() { }

  void display(bool activating = false) {
    (void)activating; // We don't use this parameter - avoid a warning...

    GUIPackage::resetDisplay();
    bool am = true;
    int  m = minute();
    int  h = hour();

    if (h > 12) { h -= 12; am = false; }
    else if (h == 0) { h = 12;}
    else if (h == 12) { am = false; }

    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "%d:%02d", h, m);
    oled->setFont(GUIPackage::DSEG7_Classic_Bold_32);
    oled->setTextAlignment(TEXT_ALIGN_RIGHT);
    oled->drawString(GUI::Width-24, 4, GUIPackage::fmtBuf);
    oled->setTextAlignment(TEXT_ALIGN_LEFT);
    oled->setFont(ArialMT_Plain_10);
    oled->drawString(GUI::Width-20, 4, am ? "AM" : "PM");

    // Might as well display some stats while we are here...
    oled->setFont(ArialMT_Plain_16);
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    snprintf(
        GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "%.0f%s   %.0f%%",
        JAWS::outputTemp(JAWS::bme.measuredTemp), JAWS::tempUnits().c_str(),
        JAWS::bme.measuredHumi);
    oled->drawString(GUI::XCenter, 44, GUIPackage::fmtBuf);

    oled->display();

    compositeTime = h*100 + m;
    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {
    if (compositeTime != hour()*100+minute() ||
        GUIPackage::timeOfLastReading > timeOfLastDisplay) display();
  }

private:
  uint16_t compositeTime = 0;
};