#pragma once
#include <Arduino.h>

class DebugUI
{
public:
  static void printValue (const char *, long);
  static void printTitle (const char *);
};
