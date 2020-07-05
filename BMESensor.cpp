#include "BMESensor.h"


BMESensor::BMESensor(float tempCorrection, float humiCorrection, int elevation) {
  setAttributes(tempCorrection, humiCorrection, elevation);
}

void BMESensor::setAttributes(float tempCorrection, float humiCorrection, int elevation) {
  this->tempCorrection = tempCorrection;
  this->humiCorrection = humiCorrection;
  this->elevation = elevation;
 }

void BMESensor::begin(int addr) {
  bool status;
  status = bme.begin(addr);  
  if (status == 0) {
    Log.error("Could not find a valid BME280 sensor, check wiring!");
    return;
  }

  Log.verbose("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling, filter off");
  bme.setSampling(
    Adafruit_BME280::MODE_FORCED,
    Adafruit_BME280::SAMPLING_X1, // temperature
    Adafruit_BME280::SAMPLING_X1, // pressure
    Adafruit_BME280::SAMPLING_X1, // humidity
    Adafruit_BME280::FILTER_OFF   );

}

void BMESensor::takeReadings() {    
  // Measure: absolute Pressure, Temperature, Humidity, voltage
  // Calculate: Relative Pressure, Dewpoint, Dewpoint Spread, Heat Index
  bme.takeForcedMeasurement();

  // Get temperature
  measuredTemp = bme.readTemperature();
  measuredTemp += tempCorrection;
  Log.verbose("Temp: %F°C", measuredTemp);

  // Get humidity
  measuredHumi = bme.readHumidity();
  measuredHumi += humiCorrection;
  if (measuredHumi > 100) measuredHumi = 100;    // the humiCorrection might lead in a value higher than 100%
  Log.verbose("Humidity: %F%%", measuredHumi);

  // Get pressure
  measuredBaro = bme.readPressure() / 100.0F;
  Log.verbose("Pressure: %F hPa", measuredBaro);

  // Calculate and print relative pressure
  float SLpressure_hPa = (((measuredBaro * 100.0)/pow((1-((float)(elevation))/44330), 5.255))/100.0);
  relPressure=(int)(SLpressure_hPa+.5);
  Log.verbose("Relative Pressure: %d hPa", relPressure);

  // Calculate dewpoint
  double a = 17.271;
  double b = 237.7;
  double tempcalc = (a * measuredTemp) / (b + measuredTemp) + log(measuredHumi*0.01);
  dewPointTemp = (b * tempcalc) / (a - tempcalc);
  Log.verbose("Dewpoint: %F°C", dewPointTemp);

  // Calculate dewpoint spread (difference between actual temp and dewpoint -> the smaller the number: rain or fog
  dewPointSpread = measuredTemp - dewPointTemp;
  Log.verbose("Dewpoint Spread: %F°C", dewPointSpread);

  // Calculate HI (heatIndex in °C) --> HI starts working above 26.7 °C
  if (measuredTemp > 26.7) {
    double c1 = -8.784, c2 = 1.611, c3 = 2.338, c4 = -0.146, c5= -1.230e-2, c6=-1.642e-2, c7=2.211e-3, c8=7.254e-4, c9=-2.582e-6  ;
    double T = measuredTemp;
    double R = measuredHumi;
    
    double A = (( c5 * T) + c2) * T + c1;
    double B = ((c7 * T) + c4) * T + c3;
    double C = ((c9 * T) + c8) * T + c6;
    heatIndex = (C * R + B) * R + A; 
  } else {
    heatIndex = measuredTemp;
    Log.verbose("Not warm enough (less than 26.7 °C) for heat index");
  }
  Log.verbose("Heat Index: %F°C", heatIndex);
}