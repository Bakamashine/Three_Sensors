#pragma once
#include "sensor.h"
#include <Arduino.h>

/// Serial line buffer. Must hold the longest accepted command plus a CRLF
/// terminator and the NUL; "sens1=-100" is 10 bytes, so 20 leaves headroom.
/// Overlong lines are rejected rather than truncated, since a truncated line
/// can turn into a different, valid-looking command.
#define CMD_BUF_SIZE 20

class Command
{
private:
  char _cmd[CMD_BUF_SIZE] = {};
  size_t _cmdLen = 0;
  bool _linePending = false;

  // Both slices point into _cmd, never into their own storage, so splitting
  // the line cannot run past the end of either half.
  char *_first_slice = nullptr;
  char *_second_slice = nullptr;

  void help ();
  bool tryParse ();
  void runCmd ();
  void readCommand ();
  void reset ();

  Sensor *_mainSensor = nullptr;
  Sensor *_firstReserveSensor = nullptr;
  Sensor *_secondReserveSensor = nullptr;
  Sensor *_streetSensor = nullptr;

public:
  /// Consumes whatever Serial has buffered without blocking. Executes the
  /// command as soon as a complete line has arrived. Returns true if one ran.
  bool feedSerial ();

  Command &setMainSensor (Sensor &);
  Command &setFirstReserveSensor (Sensor &);
  Command &setSecondReserveSensor (Sensor &);
  Command &setStreetSensor (Sensor &);
};