#include "../../HWConfig.h"
#if (HAS_GUI == 0)

#include "GUI.h"

namespace GUI {
  void showScreen(ScreenName which) { (void)which; }
  void init(bool flipDisplay) { (void)flipDisplay; }
  void setOrientation(bool flip) { (void)flip; }
  void hasPressureData(bool hpd) { (void)hpd; }
  void loop() {}
  void streamScreenShotAsBMP(Stream &s) { (void)s;}
  uint32_t getSizeOfScreenShotAsBMP() { return 0; }
}
#endif