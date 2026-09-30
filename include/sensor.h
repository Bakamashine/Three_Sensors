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
  int16_t _lastTemp = 0;
  int16_t _samples[ATTEMPTS] = {};
  int _correctInt = 0;
  int16_t getTempFromTable(int rawAcp);
  static void sort(int16_t *array, size_t size);

public:
  int16_t getTemp();
  static int getMaxT();
  static int getMinT();
  Sensor(uint8_t pin);
  Sensor &setRes(int rawAdc);
  Sensor &setAcp(int acp);
  Sensor &setCorrectInt(int);
  int getCorrectInt();
  int getAcp();
  float getRes();
  static int16_t ntcTempAt(size_t i);
  static int32_t ntcResAt(size_t i);
};
