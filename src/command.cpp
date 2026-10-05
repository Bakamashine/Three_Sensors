#include "constants/constants.h"
#ifdef ENABLE_COMMANDS
#include "command.h"
#include "settings.h"
#include <Arduino.h>

Command &
Command::allocate (int size)
{
  int slice_size = (size >> 1) + 1;
  _first_slice = static_cast<char *> (malloc (sizeof (char) * slice_size));
  _second_slice = static_cast<char *> (malloc (sizeof (char) * slice_size));
  return *this;
}

void
Command::help ()
{
  Serial.println (F ("ALL COMMANDS:"));
  Serial.println (F ("help              - show this list"));
  Serial.println (F ("hyst=<int>        - set burner hysteresis"));
  Serial.println (F ("sens1=<int>       - set main sensor correction"));
  Serial.println (
      F ("sens2=<int>       - set first reserve sensor correction"));
  Serial.println (
      F ("sens3=<int>       - set second reserve sensor correction"));
  Serial.println (F ("sens4=<int>       - set street sensor correction"));
  Serial.println (F ("max=<int>         - set max permitted offset"));
  Serial.println (F ("min=<int>         - set min permitted offset"));
  Serial.println (F ("<other>=<int>     - not a command; try 'help'"));
}

Command &
Command::setMainSensor (Sensor &sn)
{
  _mainSensor = &sn;
  return *this;
}

Command &
Command::setFirstReserveSensor (Sensor &sn)
{
  _firstReserveSensor = &sn;
  return *this;
}

Command &
Command::setSecondReserveSensor (Sensor &sn)
{
  _secondReserveSensor = &sn;
  return *this;
}

Command &
Command::setStreetSensor (Sensor &sn)
{
  _streetSensor = &sn;
  return *this;
}

Sensor &
Command::getFirstReserveSensor ()
{
  return *_firstReserveSensor;
}

Sensor &
Command::getMainSensor ()
{
  return *_mainSensor;
}

Command &
Command::setCmd (char *cmd)
{
  _cmd = cmd;
  return *this;
}

void
Command::readCommand ()
{
  if (_cmd == nullptr)
    return;
#ifdef DEBUG_COMMAND
  Serial.print ("readCommand: '");
  Serial.print (_cmd);
  Serial.println ("'");
#endif
  if (strcmp (_cmd, "help") == 0)
    {
      help ();
    }
  else if (tryParse ())
    {
#ifdef DEBUG_COMMAND
      Serial.print ("parsed first='");
      Serial.print (_first_slice);
      Serial.print ("' second='");
      Serial.print (_second_slice);
      Serial.println ("'");
#endif
      runCmd ();
      return;
    }
  else
    Serial.println ("Command not found");
}

void
Command::freeData ()
{
  free (_first_slice);
  free (_second_slice);
  free (_cmd);
}

void
Command::runCmd ()
{
#ifdef DEBUG_COMMAND
  Serial.print ("BEFORE  cmd='");
  Serial.print (_cmd);
  Serial.print ("' first='");
  Serial.print (_first_slice);
  Serial.print ("' second='");
  Serial.print (_second_slice);
  Serial.println ("'");
#endif
  if (_mainSensor == nullptr || _firstReserveSensor == nullptr
      || _secondReserveSensor == nullptr || _streetSensor == nullptr)
    return;
  // hyst=<int> - set the burner hysteresis deadband
  int second_slice_value = atoi (_second_slice);
  if (strcmp (_first_slice, "hyst") == 0)
    {
      Settings::setHysteresis (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    hyst applied");
#endif
    }
  // sens1=<int> - set the correction added to the main sensor reading

  // !FIXME: Critical Error. Maybe memory leak
  else if (strcmp (_first_slice, "sens1") == 0)
    {
      _mainSensor->setCorrectInt (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    sens1 applied");
#endif
    }
  // sens2=<int> - set the correction for the first reserve sensor
  else if (strcmp (_first_slice, "sens2") == 0)
    {
      _firstReserveSensor->setCorrectInt (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    sens2 applied");
#endif
    }
  // sens3=<int> - set the correction for the second reserve sensor
  else if (strcmp (_first_slice, "sens3") == 0)
    {
      _secondReserveSensor->setCorrectInt (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    sens3 applied");
#endif
    }
  // sens4=<int> - set the correction for the street sensor
  else if (strcmp (_first_slice, "sens4") == 0)
    {
      _streetSensor->setCorrectInt (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    sens4 applied");
#endif
    }
  // max=<int> - set the maximum permitted offset
  else if (strcmp (_first_slice, "max") == 0)
    {
      Settings::setMaxPermOffset (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    max applied");
#endif
    }
  // min=<int> - set the minimum permitted offset
  else if (strcmp (_first_slice, "min") == 0)
    {
      Settings::setMinPermOffset (second_slice_value);
#ifdef DEBUG_COMMAND
      Serial.println ("RUN    min applied");
#endif
    }
#ifdef DEBUG_COMMAND
  else
    Serial.println ("RUN    no branch matched");
#endif
}

bool
Command::tryParse ()
{
  char *eq = strchr (_cmd, '=');
  if (eq == nullptr || eq == _cmd)
    return false;

  int first_size = eq - _cmd;
  strncpy (_first_slice, _cmd, first_size);
  _first_slice[first_size] = '\0';
  strcpy (_second_slice, eq + 1);

  return true;
}
#endif