#include <Arduino.h>
#include "sensor.h"
#include "helper.h"
#include "avr/pgmspace.h"
#include "settings.h"
#include "constants/constants.h"
#include "constants/ntcPoint.h"

#define MIN_T -10
#define MAX_T 110

#define MAX_ACP 1023
#define VCC 5
#define RESISTOR_FROM_SENSOR 2000 // 2kOm

#define FILTER_ALPHA 0.15F // EMA coefficient (0..1], smaller = smoother
#define GET_RES(value) (RESISTOR_FROM_SENSOR * static_cast<float>(value) / (MAX_ACP - value))
#define TEMP_INTERVAL (2 * 1000) // ms between samples

Sensor::Sensor(uint8_t pin) : _pin(pin)
{
}

bool *Sensor::checkSensor(Sensor **sensors, size_t size)
{
  return NULL;
  // int16_t temp[size];
  // for (size_t i = 0; i < size; i++)
  // {
  //   temp[i] = sensors[i]->getTemp();
  // }
  // sort(temp,size);
  // int16_t min = temp[0];
  // bool t_statuses[size];
  // for (size_t i = 0; i < size; i++)
  // {
  //   t_statuses[i]  = !(temp[i]+4 >= min || temp[i]-4 <= min);
  // }

  // bool final_result[size];
  // for (size_t i = 0; i < size; i++)
  // {
  //   final_result[i]
  // }

  // return t_statuses;
}

uint8_t Sensor::getPin()
{
  return _pin;
}

int16_t Sensor::ntcTempAt(size_t i)
{
  return static_cast<int16_t>(pgm_read_word(&ntcTable[i].temp_c));
}
int32_t Sensor::ntcResAt(size_t i)
{
  return static_cast<int32_t>(pgm_read_dword(&ntcTable[i].resistance));
}

int Sensor::getAcp() { return _acp; }

void Sensor::sort(int16_t *array, size_t size)
{
  if (size < 2)
    return;

  for (size_t a = 1; a < size; a++)
  {
    for (size_t b = size - 1; b >= a; b--)
    {
      if (array[b] < array[b - 1])
      {
        int16_t t = array[b - 1];
        array[b - 1] = array[b];
        array[b] = t;
      }
    }
  }
}

int16_t Sensor::getTemp()
{
  uint32_t now = millis();
  if (now - _lastSampleMs < TEMP_INTERVAL)
    return _lastTemp + _correctInt;

  _lastSampleMs = now;

  int rawAdc = analogRead(_pin);
  if (_adcFilter < 0.0F)
    _adcFilter = static_cast<float>(rawAdc);
  else
    // EMA: alpha * new + (1 - alpha) * old
    _adcFilter = FILTER_ALPHA * rawAdc + (1.0F - FILTER_ALPHA) * _adcFilter;

  _samples[_sampleIdx] = getTempFromTable(static_cast<int>(_adcFilter + 0.5F));
  _sampleIdx++;

  if (_sampleIdx >= ATTEMPTS)
  {
    sort(_samples, ATTEMPTS);
    _lastTemp = getAvarageValue(&_samples[1], ATTEMPTS - 2);
    _sampleIdx = 0;
  }

  return _lastTemp + _correctInt;
}

int Sensor::getMaxT()
{
  return MAX_T;
}

int Sensor::getMinT()
{
  return MIN_T;
}

Sensor &Sensor::setAcp(int acp)
{
  _acp = acp;
  return *this;
}

Sensor &Sensor::setRes(int rawAdc)
{
  if (rawAdc == 0)
  {
    _resist = 0;
    return *this;
  }
  _resist = GET_RES(rawAdc);
  return *this;
}

int16_t Sensor::getTempFromTable(int rawAcp)
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

float Sensor::getRes() { return _resist; }

Sensor &Sensor::setCorrectInt(int v)
{
  _correctInt = v;
  return *this;
}

int Sensor::getCorrectInt()
{
  return _correctInt;
}