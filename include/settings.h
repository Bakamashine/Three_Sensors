#pragma once

class Settings
{
private:
  static int _userTemp;
  static bool _burnerStatus;
  static bool _settingsStatus;
  static bool _errorStatus;
  static int _hysteresis; // burner hysteresis deadband
  static int _correctInt;
  static int _maxPermOffset;
  static int _minPermOffset;

public:
  static int getUserTemp();
  static void setSettingsStatus(bool);
  static bool getSettingsStatus();
  static bool getErrorStatus();
  static void upUserTemp();
  static void downUserTemp();
  static void setBurnerStatus(bool);
  static void setHysteresis(int);
  static int getHysteresis();
  static void setCorrectInt(int);
  static int getCorrectInt();
  static int getMaxPermOffset();
  static int getMinPermOffset();
  static void setMaxPermOffset(int);
  static void setMinPermOffset(int);
};