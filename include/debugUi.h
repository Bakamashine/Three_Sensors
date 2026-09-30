#pragma once
#include <Arduino.h>

class DebugUI
{
public:
  static void printValue(const char *, const char *);
  static void printValue(const char *, long);
  static void fprintValue(const char *, float);
};
