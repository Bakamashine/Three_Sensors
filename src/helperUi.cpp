#include "helperUi.h"
#include <stdarg.h>

void setText(char *buf, size_t size, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, size, fmt, args);
  va_end(args);
}

void setFloatText(char *buf, size_t size, const char *label, float v)
{
  if (v < 0)
    v = -v;
  long whole = static_cast<long>(v);
  long frac = static_cast<long>((v - static_cast<float>(whole)) * 100.0F + 0.5F);
  if (frac >= 100)
  {
    frac -= 100;
    whole += 1;
  }

  setText(buf, size, "%s: %ld.%02ld", label, whole, frac);
}