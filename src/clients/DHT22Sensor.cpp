#include <ArduinoLog.h>
#include "DHT22Sensor.h"


DHT22Sensor::DHT22Sensor(uint8_t pin, float tempCorrection, float humiCorrection, int elevation) {
  _dht = new DHT(pin, DHT22);
  setAttributes(tempCorrection, humiCorrection, elevation);
}

void DHT22Sensor::setAttributes(float tempCorrection, float humiCorrection, int elevation) {
  this->tempCorrection = tempCorrection;
  this->humiCorrection = humiCorrection;
  this->elevation = elevation;
 }

void DHT22Sensor::begin() {
  _dht->begin();
}

void DHT22Sensor::takeReadings(Readings& readings) {
  readings.timestamp = millis();

  readings.pressure = -1;     // Indicates pressure is unknown
  readings.relPressure = -1;  // Indicates relative pressure is unknown

  // Get temperature
  readings.temp = _dht->readTemperature();
  readings.temp += tempCorrection;
  Log.verbose("Temp: %F°C", readings.temp);

  // Get humidity
  readings.humidity = _dht->readHumidity();
  readings.humidity += humiCorrection;
  if (readings.humidity > 100) readings.humidity = 100;    // the humiCorrection might lead in a value higher than 100%
  Log.verbose("Humidity: %F%%", readings.humidity);

  // Calculate derived values
  // Dewpoint:
  double a = 17.271;
  double b = 237.7;
  double tempcalc = (a * readings.temp) / (b + readings.temp) + log(readings.humidity*0.01);
  readings.dewPointTemp = (b * tempcalc) / (a - tempcalc);
  Log.verbose("Dewpoint: %F°C", readings.dewPointTemp);

  // DewPoint Spread (difference between actual temp and dewpoint -> the smaller the number: rain or fog
  readings.dewPointSpread = readings.temp - readings.dewPointTemp;
  Log.verbose("Dewpoint Spread: %F°C", readings.dewPointSpread);

  // Heat Index (readings.heatIndex in °C) --> HI starts working above 26.7 °C
  if (readings.temp > 26.7) {
    double c1 = -8.784, c2 = 1.611, c3 = 2.338, c4 = -0.146, c5= -1.230e-2, c6=-1.642e-2, c7=2.211e-3, c8=7.254e-4, c9=-2.582e-6  ;
    double T = readings.temp;
    double R = readings.humidity;
    
    double A = (( c5 * T) + c2) * T + c1;
    double B = ((c7 * T) + c4) * T + c3;
    double C = ((c9 * T) + c8) * T + c6;
    readings.heatIndex = (C * R + B) * R + A; 
  } else {
    readings.heatIndex = readings.temp;
    Log.verbose("Not warm enough (less than 26.7 °C) for heat index");
  }
  Log.verbose("Heat Index: %F°C", readings.heatIndex);
}