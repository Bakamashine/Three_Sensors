#include "validate.h"
#include "constants/error.h"
#include "helper.h"
#include "settings.h"
#include <Arduino.h>

int
Validate::checkSensors ()
{
  /*
  codes:
  100 - all sensors agree
  101 - main sensor is defective
  102 - first reserve sensor is defective
  103 - second reserve sensor is defective

  _streetTemp is deliberately NOT cross-checked: it measures a different
  environment, so a legitimate difference there would trip the check
  constantly. Code 104 stays reserved for it in case that ever changes.
  */
  if (_firstResTemp + PERMITTED_TEMP_DIFFERENCE <= _mainTemp
      || _secondResTemp + PERMITTED_TEMP_DIFFERENCE <= _mainTemp)
    {
      return 101;
    }
  if (_mainTemp + PERMITTED_TEMP_DIFFERENCE <= _firstResTemp
      || _secondResTemp + PERMITTED_TEMP_DIFFERENCE <= _firstResTemp)
    {
      return 102;
    }
  if (_mainTemp + PERMITTED_TEMP_DIFFERENCE <= _secondResTemp
      || _firstResTemp + PERMITTED_TEMP_DIFFERENCE <= _secondResTemp)
    {
      return 103;
    }

  return 100; // no error
}

int
Validate::checkTemp ()
{
  // street sensor excluded on purpose, see checkSensors()
  float temp[] = { _mainTemp, _firstResTemp, _secondResTemp };
  float avarageVal = getAvarageValue (temp, sizeof (temp) / sizeof (temp[0]));

  // the bounds themselves are valid readings, so compare strictly
  if (Settings::getMaxPermOffset () < avarageVal)
    {
      return 201; // average too hot
    }
  if (Settings::getMinPermOffset () > avarageVal)
    {
      return 202; // average too cold
    }

  return 100; // no error
}

void
Validate::pipeline ()
{
  // a disagreement between sensors is more urgent than a slow drift
  int code = checkSensors ();
  if (code <= 100)
    {
      code = checkTemp ();
    }

  if (code > 100)
    {
      Error::setErrorStatus (true);
      Error::setErrorCode (code);
      return;
    }

  // readings are fine again: drop a previously latched error, otherwise the
  // system would stay halted forever after a single bad reading
  Error::setErrorStatus (false);
  Error::setErrorCode (0);
}