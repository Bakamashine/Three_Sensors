#include <Arduino.h>
#include "debugUi.h"
#include "helperUi.h"
#define LINE(size)                     \
  for (int i = 0; i < (size) / 2; i++) \
    Serial.print("#");

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
  size_t v_len = strlen(v);
  size_t t_size = v_len + 3;
  char *t = (char *)malloc(t_size);
  if (!t)
    return;
  t[0] = ' ';
  size_t k = 1;
  for (size_t j = 0; j < v_len; j++)
  {
    t[k++] = v[j];
  }

  if (v_len > 0 && v[v_len - 1] == '\n')
    t[k++] = '\n';
  else
    t[k++] = ' ';
  t[k] = '\0';

  Serial.print(t);
  LINE(total_size);
  Serial.print("\n");
  free(t);
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
