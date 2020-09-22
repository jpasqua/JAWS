#ifndef JAWS_h
#define JAWS_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <CircularBuffer.h>
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
  void    updateCorrections();

  class TempReading {
  public:
    TempReading() { }
    TempReading(float t, uint32_t ts) : temp(t), timestamp(ts) { }
    float temp;
    uint32_t timestamp;
  };
  const  uint8_t MaxSamples = 128;
  extern CircularBuffer<TempReading, MaxSamples> history;
  void emitHistoryAsJSON(Stream& s);
}

#endif  // JAWS_h