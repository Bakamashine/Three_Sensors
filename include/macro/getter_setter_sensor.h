#pragma once
#include "macro/getter_setter_generic.h"
#include <stdint.h>

/// Sensor layer: the only file that knows the project has four sensors.
///
/// Everything here is a DEFINITION and belongs inside a class body, so nothing
/// needs to appear in a .cpp.

/// the four sensor pointers
#define SENSOR_MEMBERS                                                        \
  Sensor *_mainSensor = nullptr;                                              \
  Sensor *_firstReserveSensor = nullptr;                                      \
  Sensor *_secondReserveSensor = nullptr;                                     \
  Sensor *_streetSensor = nullptr;

/// temperature cache: float, so the table interpolation and the correction are
/// kept fractional instead of being rounded to whole degrees at every step.
/// Costs 2 extra bytes per member over int16_t.
#define SENSOR_TEMP_MEMBERS                                                   \
  float _mainTemp = 0;                                                        \
  float _firstResTemp = 0;                                                    \
  float _secondResTemp = 0;                                                   \
  float _streetTemp = 0;

#define SENSOR_ACP_MEMBERS                                                    \
  int _mainAcp = 0;                                                           \
  int _firstResAcp = 0;                                                       \
  int _secondResAcp = 0;                                                      \
  int _streetAcp = 0;

/// getter + setter pair for one sensor, overriding an interface.
/// ret is the interface name without & - the setter adds it, the getter
/// already hands back a Sensor reference. Members are pointers, so the PTR
/// variants are the right ones: the setter stores &sn, the getter returns
/// *_mainSensor.
#define SENSOR_ACCESSOR(ret, Method, Member)                                  \
  MACRO_GETTER_SETTER_PTR_DECL_OVERRIDE (ret &, Sensor &, Method, Member,     \
                                         (Sensor & sn), sn)

/// all four pairs; place inside the class body
///   SENSOR_GETTER_SETTER (IDisplay);
#define SENSOR_GETTER_SETTER(ret)                                             \
  SENSOR_ACCESSOR (ret, MainSensor, _mainSensor);                             \
  SENSOR_ACCESSOR (ret, FirstReserveSensor, _firstReserveSensor);             \
  SENSOR_ACCESSOR (ret, SecondReserveSensor, _secondReserveSensor);           \
  SENSOR_ACCESSOR (ret, StreetSensor, _streetSensor);

// OVERRIDE version. For temperature
#define O_SENSOR_TEMP_GETTER_SETTER(ret)                                      \
  ret &setMainTemp (float v) override                                         \
  {                                                                           \
    _mainTemp = v;                                                            \
    return *this;                                                             \
  }                                                                           \
  ret &setFirstResTemp (float v) override                                     \
  {                                                                           \
    _firstResTemp = v;                                                        \
    return *this;                                                             \
  }                                                                           \
  ret &setSecondResTemp (float v) override                                    \
  {                                                                           \
    _secondResTemp = v;                                                       \
    return *this;                                                             \
  }                                                                           \
  ret &setStreetTemp (float v) override                                       \
  {                                                                           \
    _streetTemp = v;                                                          \
    return *this;                                                             \
  }                                                                           \
  float getMainTemp () override { return _mainTemp; }                         \
  float getFirstResTemp () override { return _firstResTemp; }                 \
  float getSecondResTemp () override { return _secondResTemp; }               \
  float getStreetTemp () override { return _streetTemp; }

// OVERRIDE version. For ACP
#define O_SENSOR_ACP_GETTER_SETTER(ret)                                       \
  ret &setMainAcp (int v) override                                            \
  {                                                                           \
    _mainAcp = v;                                                             \
    return *this;                                                             \
  }                                                                           \
  ret &setFirstResAcp (int v) override                                        \
  {                                                                           \
    _firstResAcp = v;                                                         \
    return *this;                                                             \
  }                                                                           \
  ret &setSecondResAcp (int v) override                                       \
  {                                                                           \
    _secondResAcp = v;                                                        \
    return *this;                                                             \
  }                                                                           \
  ret &setStreetAcp (int v) override                                          \
  {                                                                           \
    _streetAcp = v;                                                           \
    return *this;                                                             \
  }                                                                           \
  int getMainAcp () override { return _mainAcp; }                             \
  int getFirstResAcp () override { return _firstResAcp; }                     \
  int getSecondResAcp () override { return _secondResAcp; }                   \
  int getStreetAcp () override { return _streetAcp; }

// For temperature
#define SENSOR_TEMP_GETTER_SETTER(ret)                                        \
  ret &setMainTemp (float v)                                                  \
  {                                                                           \
    _mainTemp = v;                                                            \
    return *this;                                                             \
  }                                                                           \
  ret &setFirstResTemp (float v)                                              \
  {                                                                           \
    _firstResTemp = v;                                                        \
    return *this;                                                             \
  }                                                                           \
  ret &setSecondResTemp (float v)                                             \
  {                                                                           \
    _secondResTemp = v;                                                       \
    return *this;                                                             \
  }                                                                           \
  ret &setStreetTemp (float v)                                                \
  {                                                                           \
    _streetTemp = v;                                                          \
    return *this;                                                             \
  }                                                                           \
  float getMainTemp () { return _mainTemp; }                                  \
  float getFirstResTemp () { return _firstResTemp; }                          \
  float getSecondResTemp () { return _secondResTemp; }                        \
  float getStreetTemp () { return _streetTemp; }
