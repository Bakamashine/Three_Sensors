#include "settings.h"
#include "constants/constants.h"
#include "constants/settings.h"
#include "page.h"
#include "sensor.h"
#include <Arduino.h>

#define MAX_PERM_OFFSET 95
#define MIN_PERM_OFFSET 15

bool Settings::_burnerStatus = false;
int Settings::_hysteresis = DEFAULT_HYSTERESIS;
int Settings::_minPermOffset = MIN_PERM_OFFSET;
int Settings::_maxPermOffset = MAX_PERM_OFFSET;

void
Settings::setBurnerStatus (bool st)
{
  _burnerStatus = st;
}
void
Settings::setHysteresis (int v)
{
  _hysteresis = v;
}
int
Settings::getHysteresis ()
{
  return _hysteresis;
}
void
Settings::setMaxPermOffset (int v)
{
  if (v > _minPermOffset)
    _maxPermOffset = v;
}
int
Settings::getMaxPermOffset ()
{
  return _maxPermOffset;
}
void
Settings::setMinPermOffset (int v)
{
  if (v < _maxPermOffset)
    _minPermOffset = v;
}
int
Settings::getMinPermOffset ()
{
  return _minPermOffset;
}
