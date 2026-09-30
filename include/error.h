#pragma once

class Error
{
private:
  int _code = 0;

public:
  Error();
  static const char *getErrorMessage(int code);
  void setErrorCodeAndStatus(int code);
};