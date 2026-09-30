#include "error.h"

Error::Error() {}

void Error::setErrorCodeAndStatus(int code)
{
  this->_code = code;
}

const char *Error::getErrorMessage(int code)
{
  switch (code)
  {
  case 1:
    return "Error 1: Closing";
  case 2:
    return "Error 2: Break";
  case 3:
    return "Error 3: Burner is broken";
  default:
    return "No error";
  }
}