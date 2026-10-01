#include <Arduino.h>
#include "settings.h"
#include "sensor.h"
#include "page.h"
#include "constants/constants.h"
#include "constants/settings.h"

#define MAX_PERM_OFFSET 95
#define MIN_PERM_OFFSET 15

int Settings::_userTemp = DEFAULT_USER_TEMP;
bool Settings::_burnerStatus = false;
bool Settings::_settingsStatus = false;
bool Settings::_errorStatus = false;
int Settings::_hysteresis = DEFAULT_HYSTERESIS;
int Settings::_correctInt = 0;
int Settings::_minPermOffset = MIN_PERM_OFFSET;
int Settings::_maxPermOffset = MAX_PERM_OFFSET;

void Settings::upUserTemp()
{
  if (Sensor::getMaxT() <= _userTemp)
    return;
  _userTemp++;
}

void Settings::downUserTemp()
{
  if (Sensor::getMinT() >= _userTemp)
    return;
  _userTemp--;
}

void Settings::setSettingsStatus(bool st)
{
  if (st)
  {
    Page::setCurrentPage(SETTINGS);
  }
  else
  {
    Page::setCurrentPage(MAIN_PAGE);
  }
  _settingsStatus = st;
}
int Settings::getUserTemp() { return _userTemp; }
void Settings::setBurnerStatus(bool st) { _burnerStatus = st; }
bool Settings::getSettingsStatus() { return _settingsStatus; }
bool Settings::getErrorStatus() { return _errorStatus; }
void Settings::setHysteresis(int v) { _hysteresis = v; }
int Settings::getHysteresis() { return _hysteresis; }
int Settings::getCorrectInt() { return _correctInt; }
void Settings::setCorrectInt(int v) { _correctInt = v; }
void Settings::setMaxPermOffset(int v) { _maxPermOffset = v; }
int Settings::getMaxPermOffset() { return _maxPermOffset; }
void Settings::setMinPermOffset(int v) { _minPermOffset = v; }
int Settings::getMinPermOffset() { return _minPermOffset; }