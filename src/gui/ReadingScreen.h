//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class ReadingScreen : public Screen {
public:
  ReadingScreen() { }

  virtual void display(bool activating = false) = 0;

  void display(bool activating, const char* heading, const char* fmt, float val, const char* units) {
    (void)activating; // We don't use this parameter - avoid a warning...

    GUIPackage::resetDisplay();
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    oled->setFont(ArialMT_Plain_10);
    oled->drawString(GUI::XCenter, 0, heading);

    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, fmt, val, units);
    oled->setFont(GUIPackage::DSEG7_Classic_Bold_32);
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    oled->drawString(GUI::XCenter, 20, GUIPackage::fmtBuf);
    uint16_t w = oled->getStringWidth(GUIPackage::fmtBuf);
    oled->setFont(ArialMT_Plain_16);
    oled->drawString(GUI::XCenter+w/2+6, 40, units);
    oled->display();

    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {
    if (JAWS::readings.timestamp > timeOfLastDisplay) display();
  }
};

class HumidityScreen : public ReadingScreen {
public:
  void display(bool activating = false) { 
    ReadingScreen::display(activating, "Humidity", "%.0f", JAWS::readings.humidity, "%");
  }
};

class TempScreen : public ReadingScreen {
public:
  void display(bool activating = false) {
    ReadingScreen::display(activating, "Temperature", "%.0f", JAWS::outputTemp(JAWS::readings.temp), JAWS::tempUnits().c_str());
  }
};

class BaroScreen : public ReadingScreen {
public:
  void display(bool activating = false) {
    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Barometer (%s)", JAWS::baroUnits().c_str());
    ReadingScreen::display(activating, GUIPackage::fmtBuf, "%.1f", JAWS::outputBaro(JAWS::readings.pressure), "");
  }
};