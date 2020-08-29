#ifndef JAWS_h
#define JAWS_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <WebThing.h>
//                                  Local Includes
#include "JAWSSettings.h"
#include "src/clients/Readings.h"
//--------------- End:    Includes ---------------------------------------------


namespace JAWS {
  extern JAWSSettings settings;
  extern Readings readings;
  

  extern const String Version;
  extern String SSID;

  void    processReadings();
  float   outputTemp(float temp);
  float   tempSpread(float spread);
  float   outputBaro(float baro);
  String  tempUnits();
  String  baroUnits();
  char    *formattedTime(time_t theTime);
  char    *timeOfLastReading();

  namespace History {
    extern const uint8_t MaxSamples;
    extern uint8_t nSamples;
    float getSample(uint8_t index);
    void appendSample(float value);
  }
}

#endif  // JAWS_h