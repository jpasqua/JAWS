/*
 * GUI.cpp:
 *    Provide a local interface for weather readings
 *
 */


//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <IoAbstraction.h>
//                                  Local Includes
#include "GUI.h"
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
  DISPLAY_OBJ  *oled;           // The dipslay

  float  samples[MaxSamples];   // Sample Data: Circular buffer
  int    nSamples;              // Sample Data: Number of samples stored so far
  int    sampleHead;            // Sample Data: Location to store the next sample
  uint32_t timeOfLastReading;
  
  char fmtBuf[FmtBufSize];

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

  //
  // Functions
  //
  void setOrientation(bool flipDisplay) {
    flipped = flipDisplay;
    GUIPackage::oled->resetOrientation();
    if (flipDisplay) GUIPackage::oled->flipScreenVertically();
  }

  void loop() {
    taskManager.runLoop();  
    if (curScreen != ScreenName::NoScreen) screens[curScreen]->processPeriodicActivity();
  }

  void showScreen(ScreenName which) {
    screens[(curScreen = which)]->activate();
  }

  void nextScreen() {
    uint8_t next = curScreen+1;
    if (next > lastCycledScreen) next = firstCycledScreen;
    showScreen(static_cast<ScreenName>(next));
  }

  void advanceButtonClicked(pinid_t pin, bool heldDown) {
    (void)pin;
    (void)heldDown;
    nextScreen();
  }

  void init(bool flipDisplay) {
    GUIPackage::oled = new DISPLAY_OBJ(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);
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

    switches.initialise(ioUsingArduino(), true);
    switches.addSwitch(BUTTON_PIN, advanceButtonClicked);
  }

  void noteNewSample(float val) {
    GUIPackage::samples[GUIPackage::sampleHead] = val;
    GUIPackage::sampleHead = (GUIPackage::sampleHead + 1) % GUIPackage::MaxSamples;
    GUIPackage::nSamples = std::min(GUIPackage::nSamples+1, GUIPackage::MaxSamples);
    GUIPackage::timeOfLastReading = millis();
  }
} // ----- END: GUI namespace