#pragma once
#include "error.h"
#include "macro/getter_setter_sensor.h"

class Validate : public Error
{
private:
  SENSOR_TEMP_MEMBERS;

public:
  SENSOR_TEMP_GETTER_SETTER (Validate);

  int checkSensors ();
  int checkTemp ();
  void pipeline ();
};