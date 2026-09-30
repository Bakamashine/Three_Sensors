#include <Arduino.h>
#include "temperature.h"
#include "helper.h"
#include "avr/pgmspace.h"
#include "settings.h"
#include "constants/constants.h"
#include "constants/ntcPoint.h"

#define MIN_T -10
#define MAX_T 110

#define ATTEMPTS 5

#define MAX_ACP 1023
#define VCC 5
#define RESISTOR_FROM_SENSOR 2000 // 2kOm

#define FILTER_ALPHA 0.15F // EMA coefficient (0..1], smaller = smoother
#define GET_RES(value) (RESISTOR_FROM_SENSOR * static_cast<float>(value) / (MAX_ACP - value))
#define TEMP_INTERVAL (2 * 1000) // ms between samples


#define MIN_TEMP_BORDER -10

int16_t Temperature::ntcTempAt(size_t i)
{
  return static_cast<int16_t>(pgm_read_word(&ntcTable[i].temp_c));
}
int32_t Temperature::ntcResAt(size_t i)
{
  return static_cast<int32_t>(pgm_read_dword(&ntcTable[i].resistance));
}

int Temperature::getAcp() { return _acp; }

void Temperature::sort(int16_t *array, size_t size)
{
  size_t a, b;
  int16_t t = 0;
  for (a = 1; a < size; a++)
  {
    for (b = size - 1; b >= a; b--)
    {
      if (array[b] < array[b - 1])
      {
        t = array[b - 1];
        array[b - 1] = array[b];
        array[b] = t;
      }
    }
  }
}

int16_t *Temperature::removeMinMax(int16_t *array, size_t size)
{
  sort(array, size);
  int16_t *newArr = static_cast<int16_t *>(malloc(sizeof(int16_t) * (size - 2)));
  for (size_t i = 1; i < size - 1; i++)
  {
    newArr[i - 1] = array[i];
  }
  return newArr;
}

int16_t Temperature::getTemperature()
{
  uint32_t now = millis();
  if (now - _lastSampleMs < TEMP_INTERVAL)
    return _lastTemp;

  _lastSampleMs = now;

  int rawAdc = analogRead(SENSOR_PIN);
  if (_adcFilter < 0.0F)
    _adcFilter = static_cast<float>(rawAdc);
  else
    // EMA: alpha * new + (1 - alpha) * old
    _adcFilter = FILTER_ALPHA * rawAdc + (1.0F - FILTER_ALPHA) * _adcFilter;

  _samples[_sampleIdx] = getTempFromTable(static_cast<int>(_adcFilter + 0.5F)) + Settings::getCorrectInt();
  _sampleIdx++;

  if (_sampleIdx >= ATTEMPTS)
  {
    int16_t *newArr = removeMinMax(_samples, ATTEMPTS);
    _lastTemp = getAvarageValue(newArr, ATTEMPTS - 2);
    free(newArr);
    _sampleIdx = 0;
  }

  return _lastTemp;
}

int Temperature::getMaxT()
{
  return MAX_T;
}

int Temperature::getMinT()
{
  return MIN_T;
}

Temperature &Temperature::setAcp(int acp)
{
  _acp = acp;
  return *this;
}

Temperature &Temperature::setRes(int acp)
{
  if (acp == 0)
  {
    _resist = 0;
    return *this;
  }
  _resist = GET_RES(acp);
  return *this;
}

int16_t Temperature::getTempFromTable(int rawAcp)
{
  _acp = rawAcp;
  if (rawAcp >= MAX_ACP)
    rawAcp = MAX_ACP - 1;
  if (rawAcp <= 0)
    rawAcp = 1;

  _resist = GET_RES(rawAcp);

  // get max or min value
  if (_resist >= ntcResAt(0))
    return ntcTempAt(0);
  if (_resist <= ntcResAt(NTC_TABLE_SIZE - 1))
    return ntcTempAt(NTC_TABLE_SIZE - 1);

  for (size_t i = 0; i + 1 < NTC_TABLE_SIZE; i++)
  {
    int16_t temp = ntcTempAt(i);
    int32_t res = ntcResAt(i);

    if (_resist > res)
      continue;
    if (_resist < ntcResAt(i + 1))
      continue;

    //  returning the round number
    float fraction = static_cast<float>(res - _resist) /
                     (res - ntcResAt(i + 1));
    return temp +
           static_cast<int>(fraction * (ntcTempAt(i + 1) - temp) + 0.5F);
  }
  return 0;
}

ld Temperature::getRes() { return _resist; }