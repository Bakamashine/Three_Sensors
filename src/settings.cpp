#include <Arduino.h>
#include "settings.h"
#include "temperature.h"
#include "page.h"
#include "constants/constants.h"
#include "constants/settings.h"

int Settings::_userTemp = DEFAULT_USER_TEMP;
bool Settings::_burnerStatus = false;
bool Settings::_settingsStatus = false;
bool Settings::_errorStatus = false;
int Settings::_hysteresis = DEFAULT_HYSTERESIS;
int Settings::_correctInt = 0;

int Settings::getUserTemp()
{
  return _userTemp;
}
void Settings::upUserTemp()
{
  if (Temperature::getMaxT() <= _userTemp)
    return;
  _userTemp++;
}

void Settings::downUserTemp()
{
  if (Temperature::getMinT() >= _userTemp)
    return;
  _userTemp--;
}

void Settings::setBurnerStatus(bool st)
{
  _burnerStatus = st;
}

void Settings::setSettingsStatus(bool st)
{
  if (st)
  {
    Page::setCurrentPage(SELECT_SETTINGS);
  }
  else
  {
    Page::setCurrentPage(MAIN_PAGE);
  }
  _settingsStatus = st;
}

bool Settings::getSettingsStatus()
{
  return _settingsStatus;
}

bool Settings::getErrorStatus()
{
  return _errorStatus;
}

void Settings::setHysteresis(int v)
{
  _hysteresis = v;
}

int Settings::getHysteresis()
{
  return _hysteresis;
}

int Settings::getCorrectInt()
{
  return _correctInt;
}

void Settings::setCorrectInt(int v)
{
  _correctInt = v;
}