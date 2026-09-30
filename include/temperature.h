#pragma once
#include <Arduino.h>
typedef long double ld;

#define ATTEMPTS 5

class Temperature
{
private:
  // float _volt = 0.0F;
  ld _resist = 0;
  uint8_t _pin;
  int _acp = 0;
  float _adcFilter = -1.0F;
  uint32_t _lastSampleMs = 0;
  int _sampleIdx = 0;
  int16_t _lastTemp = 0;
  int16_t _samples[ATTEMPTS];
  int16_t getTempFromTable(int rawAcp = 0);
  void sort(int16_t *array, size_t size);
  int16_t* removeMinMax(int16_t *array, size_t size);

public:
  int16_t getTemperature();
  static int getMaxT();
  static int getMinT();
  Temperature(uint8_t pin);
  Temperature &setRes(int);
  Temperature &setAcp(int);
  int getAcp();
  ld getRes();
  static inline int16_t ntcTempAt(size_t i);
  static inline int32_t ntcResAt(size_t i);
};
