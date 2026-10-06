#include "sensor.h"
#include "avr/pgmspace.h"
#include "constants/constants.h"
#include "constants/ntcPoint.h"
#include "helper.h"
#include "settings.h"
#include <Arduino.h>

// correction is an offset in degrees, keep it a sane displayable value
#define MIN_CORRECT_INT (-50)
#define MAX_CORRECT_INT 50

#define MAX_ACP 1023
#define RESISTOR_FROM_SENSOR 2000 // 2kOm

#define FILTER_ALPHA 0.15F // EMA coefficient (0..1], smaller = smoother
#define GET_RES(value)                                                        \
  (RESISTOR_FROM_SENSOR * static_cast<float> (value) / (MAX_ACP - value))
#define TEMP_INTERVAL (1000) // ms between samples

Sensor::Sensor (uint8_t pin) : _pin (pin) {}

int16_t
Sensor::ntcTempAt (size_t i)
{
  return static_cast<int16_t> (pgm_read_word (&ntcTable[i].temp_c));
}
int32_t
Sensor::ntcResAt (size_t i)
{
  return static_cast<int32_t> (pgm_read_dword (&ntcTable[i].resistance));
}

int
Sensor::getAcp ()
{
  return _acp;
}

void
Sensor::sort (float *array, size_t size)
{
  if (size < 2)
    return;

  for (size_t a = 1; a < size; a++)
    {
      for (size_t b = size - 1; b >= a; b--)
        {
          if (array[b] < array[b - 1])
            {
              float t = array[b - 1];
              array[b - 1] = array[b];
              array[b] = t;
            }
        }
    }
}

float
Sensor::getTemp ()
{
  uint32_t now = millis ();
  if (now - _lastSampleMs < TEMP_INTERVAL)
    return _lastTemp + static_cast<float> (_correctInt);

  _lastSampleMs = now;

  int rawAdc = analogRead (_pin);
  if (_adcFilter < 0.0F)
    _adcFilter = static_cast<float> (rawAdc);
  else
    // EMA: alpha * new + (1 - alpha) * old
    _adcFilter = FILTER_ALPHA * rawAdc + (1.0F - FILTER_ALPHA) * _adcFilter;

  // round to the nearest whole ADC count before the table lookup: the filter
  // output is fractional, and the table is indexed by an integer count
  _samples[_sampleIdx]
      = getTempFromTable (static_cast<int> (_adcFilter + 0.5F));
  _sampleIdx++;

  if (_sampleIdx >= ATTEMPTS)
    {
      sort (_samples, ATTEMPTS);
      _lastTemp = getAvarageValue (&_samples[1], ATTEMPTS - 2);
      _sampleIdx = 0;
    }

  return _lastTemp + static_cast<float> (_correctInt);
}

float
Sensor::getTempFromTable (int rawAcp)
{
  _acp = rawAcp;
  if (rawAcp >= MAX_ACP)
    rawAcp = MAX_ACP - 1;
  if (rawAcp <= 0)
    rawAcp = 1;

  _resist = GET_RES (rawAcp);

  // get max or min value
  if (_resist >= ntcResAt (0))
    return static_cast<float> (ntcTempAt (0));
  if (_resist <= ntcResAt (NTC_TABLE_SIZE - 1))
    return static_cast<float> (ntcTempAt (NTC_TABLE_SIZE - 1));

  for (size_t i = 0; i + 1 < NTC_TABLE_SIZE; i++)
    {
      int16_t temp = ntcTempAt (i);
      int32_t res = ntcResAt (i);

      if (_resist > res)
        continue;
      if (_resist < ntcResAt (i + 1))
        continue;

      // interpolate between the two bracketing table rows
      float fraction
          = static_cast<float> (res - _resist) / (res - ntcResAt (i + 1));
      return static_cast<float> (temp)
             + fraction * static_cast<float> (ntcTempAt (i + 1) - temp);
    }
  return 0.0F;
}

Sensor &
Sensor::setCorrectInt (int v)
{
  if (v < MIN_CORRECT_INT)
    v = MIN_CORRECT_INT;
  if (v > MAX_CORRECT_INT)
    v = MAX_CORRECT_INT;
  _correctInt = v;
  return *this;
}

int
Sensor::getCorrectInt ()
{
  return _correctInt;
}