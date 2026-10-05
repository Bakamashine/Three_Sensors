#pragma once

class Error
{

private:
  static bool _errorStatus;
  static int _errorCode;

protected:
  static void setErrorStatus (bool);
  static void setErrorCode (int);

public:
  static bool getErrorStatus ();
  static const char *getStaticErrorMessage (int code);
  static const char *getErrorMessage ();
};