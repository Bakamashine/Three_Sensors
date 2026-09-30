#include <Arduino.h>
#include <stdarg.h>
#include "debugUi.h"
#include "float.h"
#include "settings.h"

void DebugUI::printValue(const char *v1, const char *v2)
{
  auto val = fmtAlloc("%s: %s", v1, v2);
  Serial.println(val);
  free(val);
}
void DebugUI::printValue(const char *v1, long v2)
{
  auto val = fmtAlloc("%s: %d", v1, v2);
  Serial.println(val);
  free(val);
}
void DebugUI::fprintValue(const char *v1, float v2)
{
  char *s = Float::getFloat(v2);
  if (s != NULL)
  {
    printValue(v1, s);
    free(s);
  }
}

char *DebugUI::fmtAlloc(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int needed = vsnprintf(NULL, 0, fmt, args);
  va_end(args);
  if (needed < 0)
    return NULL;
  char *buf = static_cast<char *>(malloc(needed + 1));
  if (buf != NULL)
  {
    va_start(args, fmt);
    vsnprintf(buf, needed + 1, fmt, args);
    va_end(args);
  }

  return buf;
}
