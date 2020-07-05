#ifndef JAWS_h
#define JAWS_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <WebThing.h>
//                                  Local Includes
#include "BMESensor.h"
#include "JAWSSettings.h"
//--------------- End:    Includes ---------------------------------------------


namespace JAWS {
  extern BMESensor bme;
  extern JAWSSettings settings;

  void    processReadings();
  float   outputTemp(float temp);
  float   tempSpread(float spread);
  float   outputBaro(float baro);
  String  tempUnits();
  String  baroUnits();
  char    *formattedTime(time_t theTime);
  char    *bmeTimestamp();
}

#endif  // JAWS_h