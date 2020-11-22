#ifndef DHT22Sensor_h
#define DHT22Sensor_h

#include <DHT.h>
#include "Readings.h"

class DHT22Sensor {
public:
  DHT22Sensor(uint8_t pin, float tempCorrection=0.0, float humiCorrection=0.0, int elevation=0);
  void begin();
  void takeReadings(Readings& readings);
  void setAttributes(float tempCorrection, float humiCorrection, int elevation);

private:
  DHT*  _dht;
  int   elevation;        // Units: meters
  float tempCorrection;   // Units: °C
  float humiCorrection;   // Units: %
};

#endif  // DHT22Sensor_h
