#include <Arduino.h>
#include "debugUi.h"
#include "helperUi.h"
#define LINE(size)                        \
  do                                       \
  {                                        \
    char s[16] = {};                       \
    const size_t n = (size) / 2;           \
    if (n < sizeof(s))                     \
    {                                      \
      memset(s, '#', n);                   \
      Serial.print(s);                     \
    }                                      \
  } while (0)

void DebugUI::printValue(const char *v1, const char *v2)
{
  Serial.print(v1);
  Serial.print(": ");
  Serial.println(v2);
}

void DebugUI::printTitle(const char *v)
{
  const int total_size = 30;
  LINE(total_size);
  Serial.print(' ');
  Serial.print(v);
  const size_t v_len = strlen(v);
  if (v_len == 0 || v[v_len - 1] != '\n')
    Serial.print(' ');
  Serial.println();
  LINE(total_size);
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
