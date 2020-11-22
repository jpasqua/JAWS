#ifndef Readings_h
#define Readings_h

class Readings {
public:
  float     temp;             // A value < -500 implies no data is available
  float     humidity;         // A value < 0 implies no data is available
  float     pressure;         // A value < 0 implies no data is available
  float     heatIndex;        // A value < -500 implies no data is available
  int       relPressure;      // A value < 0 implies no data is available
  float     dewPointSpread;   // A value < -500 implies no data is available
  double    dewPointTemp;     // A value < -500 implies no data is available
  uint32_t  timestamp;
};

#endif // Readings.h