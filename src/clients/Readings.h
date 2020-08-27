#ifndef Readings_h
#define Readings_h

class Readings {
public:
  float     temp;
  float     humidity;
  float     pressure;
  float     heatIndex;
  int       relPressure;
  float     dewPointSpread;
  double    dewPointTemp;
  uint32_t  timestamp;
};

#endif // Readings.h