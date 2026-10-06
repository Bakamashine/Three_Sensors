#pragma once
#include <Arduino.h>

#define ATTEMPTS 5

class Sensor
{
private:
  float _resist = 0;
  uint8_t _pin;
  int _acp = 0;
  float _adcFilter = -1.0F;
  uint32_t _lastSampleMs = 0;
  int _sampleIdx = 0;
  float _lastTemp = 0;
  float _samples[ATTEMPTS] = {};
  int _correctInt = 0;
  float getTempFromTable (int rawAcp);
  static void sort (float *array, size_t size);

public:
  float getTemp ();
  Sensor (uint8_t pin);
  Sensor &setCorrectInt (int);
  int getCorrectInt ();
  int getAcp ();
  static int16_t ntcTempAt (size_t i);
  static int32_t ntcResAt (size_t i);
};
