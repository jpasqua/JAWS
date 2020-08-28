//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

using GUIPackage::oled;

class GraphScreen : public Screen {
public:
  GraphScreen() { }

  void display(bool activating = false) {
    (void)activating; // We don't use this parameter - avoid a warning...

    // Determine the range of values in the sample data
    float graphMin =  1000.0;
    float graphMax = -1000.0;
    for (int i = 0; i < JAWS::History::nSamples; i++) {
      float cur = JAWS::outputTemp(JAWS::History::getSample(i));
      if (cur < graphMin) { graphMin = cur; }
      if (cur > graphMax) { graphMax = cur; }
    }

    float actualRange = graphMax - graphMin;
    float displayedRange = std::max(actualRange, ((float)MaxYForGraph)/4.0F);
    float rangePadding = (displayedRange - actualRange)/2;
    float rangeLo = graphMin - rangePadding;

    oled->clear();
    for (int i = 0; i < JAWS::History::nSamples; i++) {
      float cur = JAWS::outputTemp(JAWS::History::getSample(i));
      
      float percentIntoRange = (cur - rangeLo)/displayedRange;
      percentIntoRange = 1.0 - percentIntoRange; // Hi values are at the top which are lower pixel values
      oled->setPixel(i, (int)(MaxYForGraph*percentIntoRange));
    }

    // Display the Legend
    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Lo: %0.1f", graphMin);
    oled->setFont(ArialMT_Plain_10);
    oled->setTextAlignment(TEXT_ALIGN_LEFT);
    oled->drawString(0, MaxYForGraph + 5, GUIPackage::fmtBuf);

    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "%0.1f", JAWS::outputTemp(JAWS::readings.temp));
    oled->setTextAlignment(TEXT_ALIGN_CENTER);
    oled->drawString(GUI::XCenter, MaxYForGraph + 5, GUIPackage::fmtBuf);

    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Hi: %0.1f", graphMax);
    oled->setTextAlignment(TEXT_ALIGN_RIGHT);
    oled->drawString(GUI::Width, MaxYForGraph + 5, GUIPackage::fmtBuf);
    
    oled->display();
    timeOfLastDisplay = millis();
  }

  virtual void processPeriodicActivity() {
    if (JAWS::readings.timestamp > timeOfLastDisplay) { display(); }
  }

private:
  static const uint8_t MaxYForGraph = 48;  // Reserve space for the legend
};