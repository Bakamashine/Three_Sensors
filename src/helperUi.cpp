#include "helperUi.h"
#include <stdio.h>

void setFloatText(char *buf, size_t size, const char *label, float v)
{
  const bool negative = v < 0;
  if (negative)
    v = -v;

  long whole = static_cast<long>(v);
  long frac = static_cast<long>((v - static_cast<float>(whole)) * 100.0F + 0.5F);
  if (frac >= 100)
  {
    frac -= 100;
    whole += 1;
  }

  snprintf(buf, size, "%s%s%ld.%02ld", label, negative ? "-" : "", whole, frac);
}
