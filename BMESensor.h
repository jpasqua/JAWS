#ifndef BMESensor_h
#define BMESensor_h

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoLog.h>


class BMESensor {
public:
  BMESensor(float tempCorrection=0.0, float humiCorrection=0.0, int elevation=0);
  void begin(int addr=0x76);  // Either 0x76 or 0x77
  void takeReadings();
  void setAttributes(float tempCorrection, float humiCorrection, int elevation);

  // ----- Readings
  // Measured
  float      measuredTemp;
  float      measuredHumi;
  float      measuredBaro;
  // Derived
  float      heatIndex;
  int        relPressure;
  float      dewPointSpread;
  double     dewPointTemp;


private:
  Adafruit_BME280 bme;
  int   elevation;        // Units: meters
  float tempCorrection;   // Units: °C
  float humiCorrection;   // Units: %
  bool  mock;
};

#endif  // BMESensor_h
