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
    int i = (GUIPackage::nSamples < GUIPackage::MaxSamples) ? 0 : GUIPackage::sampleHead;
    for (int count = 0; count < GUIPackage::nSamples; count++) {
      float cur = GUIPackage::samples[i];
      if (cur < graphMin) { graphMin = cur; }
      if (cur > graphMax) { graphMax = cur; }
      i = (i + 1) % GUIPackage::MaxSamples;
    }

    float actualRange = graphMax - graphMin;
    float displayedRange = std::max(actualRange, ((float)MaxYForGraph)/4.0F);
    float rangePadding = (displayedRange - actualRange)/2;
    float rangeLo = graphMin - rangePadding;

    oled->clear();
    i = (GUIPackage::nSamples < GUIPackage::MaxSamples) ? 0 : GUIPackage::sampleHead;
    for (int count = 0; count < GUIPackage::nSamples; count++) {
      float cur = GUIPackage::samples[i];
      
      float percentIntoRange = (cur - rangeLo)/displayedRange;
      percentIntoRange = 1.0 - percentIntoRange; // Hi values are at the top which are lower pixel values
      oled->setPixel(count, (int)(MaxYForGraph*percentIntoRange));
      i = (i + 1) % GUIPackage::MaxSamples;
    }

    // Display the Legend
    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Lo: %0.1f", graphMin);
    oled->setFont(ArialMT_Plain_10);
    oled->setTextAlignment(TEXT_ALIGN_LEFT);
    oled->drawString(0, MaxYForGraph + 5, GUIPackage::fmtBuf);
    snprintf(GUIPackage::fmtBuf, GUIPackage::FmtBufSize, "Hi: %0.1f", graphMax);
    oled->setTextAlignment(TEXT_ALIGN_RIGHT);
    oled->drawString(GUI::Width, MaxYForGraph + 5, GUIPackage::fmtBuf);
    oled->display();

    lastSampleHead = GUIPackage::sampleHead;
  }

  virtual void processPeriodicActivity() {
    if (lastSampleHead != GUIPackage::sampleHead) display();
  }

private:
  static const uint8_t MaxYForGraph = 48;  // Reserve space for the legend
  int lastSampleHead = -1;
};