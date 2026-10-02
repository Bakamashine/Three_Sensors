#pragma once
#include "sensor.h"
#include <Arduino.h>
class Command
{
private:
  int buffer_size = 0;
  char *_first_slice = nullptr;
  char *_second_slice = nullptr;
  char *_cmd = nullptr;
  void help ();
  bool tryParse ();
  void runCmd ();

  Sensor *_mainSensor = nullptr;
  Sensor *_firstReserveSensor = nullptr;
  Sensor *_secondReserveSensor = nullptr;
  Sensor *_streetSensor = nullptr;

public:
  Command &allocate (int);
  Command &setCmd (char *);
  void readCommand ();

  Command &setMainSensor (Sensor &);
  Command &setFirstReserveSensor (Sensor &);
  Command &setSecondReserveSensor (Sensor &);
  Command &setStreetSensor (Sensor &);
  Sensor &getMainSensor ();
  Sensor &getFirstReserveSensor ();
  Sensor &getSecondReserveSensor ();
  Sensor &getStreetSensor ();
  void freeData ();
};