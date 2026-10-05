#include "error.h"

bool Error::_errorStatus = false;
int Error::_errorCode = 0;

const char *
Error::getStaticErrorMessage (int code)
{
  switch (code)
    {
    case 1:
      return "Error 1: Closing";
    case 2:
      return "Error 2: Break";
    case 3:
      return "Error 3: Burner is broken";
    case 4:
      return "Error 4: Any sensor is not working";

    case 101:
      return "Error 101: main sensor is defective";
    case 102:
      return "Error 102: first reserve sensor is defective";
    case 103:
      return "Error 103: second reserve sensor is defective";
    case 104:
      return "Error 104: street reserve sensor is defective";
    case 201:
      return "Error 201: average temperature is too high";
    case 202:
      return "Error 202: average temperature is too low";
    default:
      return "No error";
    }
}

void
Error::setErrorStatus (bool st)
{
  _errorStatus = st;
}

bool
Error::getErrorStatus ()
{
  return _errorStatus;
}

const char *
Error::getErrorMessage ()
{
  return Error::getStaticErrorMessage (_errorCode);
}

void
Error::setErrorCode (int code)
{
  _errorCode = code;
}