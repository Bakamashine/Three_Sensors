#include <Arduino.h>
#include "debugUi.h"
#include "helperUi.h"

void DebugUI::printValue(const char *v1, const char *v2)
{
  Serial.print(v1);
  Serial.print(": ");
  Serial.println(v2);
}

void DebugUI::printValue(const char *v1, long v2)
{
  Serial.print(v1);
  Serial.print(": ");
  Serial.println(v2);
}

void DebugUI::fprintValue(const char *v1, float v2)
{
  char buf[24];
  setFloatText(buf, sizeof(buf), "", v2);
  Serial.println(buf);
}
