#ifndef DS18B20_h
#define DS18B20_h

#include <OneWire.h>
#include <DallasTemperature.h>

class DS18B20 {
public:
  static constexpr float ReadingError = -1000.0;

  void begin(int pin, float tempCorrection = 0.0) {
    _tempCorrection = tempCorrection;
    ow = new OneWire(pin);
    dt = new DallasTemperature(ow);
    dt->begin();
  }

  float getTempC() {
    dt->requestTemperatures();            // Send the command to get temperatures
    float tempC = dt->getTempCByIndex(0); // Read the temperature back
    if (tempC == DEVICE_DISCONNECTED_C) return ReadingError;
    return tempC + _tempCorrection;
  }

  void setAttributes(float tempCorrection) { _tempCorrection = tempCorrection; }

private:
  OneWire* ow;
  DallasTemperature* dt;
  float _tempCorrection;   // Units: °C
};

#endif  // DS18B20_h
