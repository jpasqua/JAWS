/*
 * GUI.cpp:
 *    Provide a local interface for weather readings
 *
 */

#include "../../HWConfig.h"
#if (HAS_GUI == 1)


//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <SH1106Wire.h>
#include <SSD1306Wire.h>
//                                  Local Includes
#include "GUI.h"
#include "ButtonMgr.h"
#include "AllReadingsScreen.h"
#include "BitmapScreen.h"
#include "ConfigScreen.h"
#include "GraphScreen.h"
#include "InfoScreen.h"
#include "OffScreen.h"
#include "ReadingScreen.h"
#include "TimeScreen.h"
//--------------- End:    Includes ---------------------------------------------


namespace GUIPackage {
  //
  // Fonts
  //
  #include "DSEG7_Classic_Bold_32.h"

  //
  // State
  //
  OLEDDisplay*  oled;            // The dipslay
  char          fmtBuf[FmtBufSize];

  //
  // Exported Functions
  //
  void resetDisplay() {
    oled->displayOn();
    oled->clear();
  }
}

namespace GUI {
  //
  // Constants
  //
  static const uint8_t firstCycledScreen = ScreenName::Time;
  static const uint8_t lastCycledScreen = ScreenName::Off;

  //
  // State
  //
  bool    flipped;
  Screen* screens[ScreenName::N_Screens];
  uint8_t curScreen = ScreenName::NoScreen;
  ButtonMgr* buttonMgr;
  Button*    modeButton;


  //
  // Exported Functions
  //

  void showScreen(ScreenName which) {
    if (which == ScreenName::Baro && JAWS::readings.pressure < 0) { return; }
    screens[(curScreen = which)]->activate(); 
  }

  void advanceButtonClicked(Button* b) {
    if (b->_state == 0) return;  // Only do something when the button is released

    uint8_t next = curScreen+1;
    if (next == ScreenName::Baro && JAWS::readings.pressure < 0) next++;
    if (next > lastCycledScreen) next = firstCycledScreen;
    showScreen(static_cast<ScreenName>(next));    
  }

  void init(bool flipDisplay) {
    #if (DISPLAY_DRIVER == SH1106)
      GUIPackage::oled = new SH1106Wire(DISPLAY_I2C_ADDRESS, SDA_PIN, SCL_PIN);
    #elif (DISPLAY_DRIVER == SSD1306)
      GUIPackage::oled = new SSD1306Wire(DISPLAY_I2C_ADDRESS, SDA_PIN, SCL_PIN);
    #else
      #error "DISPLAY_DRIVER not set to known value"
    #endif
    GUIPackage::oled->init();
    setOrientation(flipDisplay);

    screens[ScreenName::WiFi] = new WiFiScreen();
    screens[ScreenName::Config] = new ConfigScreen();
    screens[ScreenName::Splash] = new SplashScreen();
    screens[ScreenName::Time] = new TimeScreen();
    screens[ScreenName::Temp] = new TempScreen();
    screens[ScreenName::Humi] = new HumidityScreen();
    screens[ScreenName::Baro] = new BaroScreen();
    screens[ScreenName::All] = new AllReadingsScreen();
    screens[ScreenName::Graph] = new GraphScreen();
    screens[ScreenName::Info] = new InfoScreen();
    screens[ScreenName::Off] = new OffScreen();

    if (BUTTON_LOW_PIN != -1) pinMode(BUTTON_LOW_PIN, LOW);
    modeButton = new Button(BUTTON_PIN, advanceButtonClicked);
    buttonMgr = new ButtonMgr(modeButton, 1);
  }

  void setOrientation(bool flipDisplay) {
    flipped = flipDisplay;
    GUIPackage::oled->resetOrientation();
    if (flipDisplay) GUIPackage::oled->flipScreenVertically();
  }

  void loop() {
    buttonMgr->process();
    if (curScreen != ScreenName::NoScreen) screens[curScreen]->processPeriodicActivity();
  }

  uint32_t getSizeOfScreenShotAsBMP() {
    return (2ul * oled->getWidth() * oled->getHeight() + 54); // pix data + 54 byte hdr
  }

  void streamScreenShotAsBMP(Stream &s) {
    // Adapted from https://forum.arduino.cc/index.php?topic=406416.0
    byte hiByte, loByte;

    uint8_t bmFlHdr[14] = {
      'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0
    };
    // 54 = std total "old" Windows BMP file header size = 14 + 40
    
    uint8_t bmInHdr[40] = {
      40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 16, 0
    };   
    // 40 = info header size
    //  1 = num of color planes
    // 16 = bits per pixel
    // all other header info = 0, including RI_RGB (no compr), DPI resolution

    uint32_t w = oled->getWidth();
    uint32_t h = oled->getHeight();
    unsigned long bmpSize = 2ul * h * w + 54; // pix data + 54 byte hdr
    
    bmFlHdr[ 2] = (uint8_t)(bmpSize      ); // all ints stored little-endian
    bmFlHdr[ 3] = (uint8_t)(bmpSize >>  8); // i.e., LSB first
    bmFlHdr[ 4] = (uint8_t)(bmpSize >> 16);
    bmFlHdr[ 5] = (uint8_t)(bmpSize >> 24);

    bmInHdr[ 4] = (uint8_t)(w      );
    bmInHdr[ 5] = (uint8_t)(w >>  8);
    bmInHdr[ 6] = (uint8_t)(w >> 16);
    bmInHdr[ 7] = (uint8_t)(w >> 24);
    bmInHdr[ 8] = (uint8_t)(h      );
    bmInHdr[ 9] = (uint8_t)(h >>  8);
    bmInHdr[10] = (uint8_t)(h >> 16);
    bmInHdr[11] = (uint8_t)(h >> 24);

    s.write(bmFlHdr, sizeof(bmFlHdr));
    s.write(bmInHdr, sizeof(bmInHdr));

    for (uint16_t y = h; y > 0; y--) {
      byte buf[w*2];
      byte *ptr = &buf[0];
      for (uint16_t x = 0; x < w; x++) {
        uint8_t theByte = oled->buffer[x + (y / 8) * w];
        uint8_t thePixel = theByte & (1 << (y & 7));
        if (thePixel) { loByte = hiByte = 0xff; }
        else {loByte = hiByte = 0x00; }
        
        *ptr++ = loByte;
        *ptr++ = hiByte;
      }
      s.write(buf, w*2);
    }
  }

} // ----- END: GUI namespace

#endif