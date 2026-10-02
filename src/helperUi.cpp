#include "helperUi.h"
#include <stdio.h>

void
setFloatText (char *buf, size_t size, const char *label, float v)
{
  const bool negative = v < 0;
  if (negative)
    v = -v;

  long whole = static_cast<long> (v);
  long frac
      = static_cast<long> ((v - static_cast<float> (whole)) * 100.0F + 0.5F);
  if (frac >= 100)
    {
      frac -= 100;
      whole += 1;
    }

  snprintf (buf, size, "%s%s%ld.%02ld", label, negative ? "-" : "", whole,
            frac);
}

void
setIntText (char *buf, size_t size, int v)
{
  if (!buf || size == 0)
    return;

  char tmp[11];
  size_t i = 0;
  bool negative = v < 0;
  unsigned int uv = negative ? static_cast<unsigned int> (-(v + 1)) + 1u
                             : static_cast<unsigned int> (v);

  do
    {
      tmp[i++] = static_cast<char> ('0' + (uv % 10u));
      uv /= 10u;
    }
  while (uv != 0 && i < sizeof (tmp));

  size_t out = 0;
  if (negative && out < size - 1)
    buf[out++] = '-';

  while (i > 0 && out < size - 1)
    buf[out++] = tmp[--i];

  buf[out] = '\0';
}
