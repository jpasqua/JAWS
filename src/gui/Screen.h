#ifndef Screen_h
#define Screen_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
//                                  Local Includes
#include "GUIPackage.h"
//--------------- End:    Includes ---------------------------------------------

class Screen {
public:
  // ----- Functions that must be implemented by subclasses
  virtual void display(bool force = false) = 0;
  virtual void processPeriodicActivity() = 0;

  // ----- Functions that may be overriden by subclasses
  virtual void activate() { display(true); }

protected:
  uint32_t timeOfLastDisplay = 0;
};

#endif // Screen_h