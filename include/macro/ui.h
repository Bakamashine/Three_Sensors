#pragma once

#define _CENTER_X(display, text)                                              \
  ((OLED_WIDTH - (display).getStrWidth (text)) / 2)
#define _CENTER_Y(display)                                                    \
  ((OLED_HEIGHT + (display).getFontHeight ()) / 2 - 25)

#define RAW_WRITE_ROW(row, text, v1, v2, p_cls)                               \
  do                                                                          \
    {                                                                         \
      if (text)                                                               \
        (p_cls)->drawStr (U8G2_FIRST_COLUMN, row, text);                      \
      if (v1)                                                                 \
        (p_cls)->drawStr (U8G2_SECOND_COLUMN, row, v1);                       \
      if (v2)                                                                 \
        (p_cls)->drawStr (U8G2_THIRD_COLUMN, row, v2);                        \
    }                                                                         \
  while (0)

#define AT_WRITE_ROW(row, text, p_cls)                                        \
  do                                                                          \
    {                                                                         \
      char acpBuf[8];                                                         \
      char tempBuf[8];                                                        \
      snprintf (tempBuf, sizeof (tempBuf), "%d", (p_cls).getTemp ());         \
      snprintf (acpBuf, sizeof (acpBuf), "%d", (p_cls).getAcp ());            \
      RAW_WRITE_ROW (row, text, acpBuf, tempBuf, this);                       \
    }                                                                         \
  while (0)

#define T_WRITE_ROW(row, text, p_cls)                                         \
  do                                                                          \
    {                                                                         \
      char tempBuf[8];                                                        \
      snprintf (tempBuf, sizeof (tempBuf), "%d", (p_cls).getTemp ());         \
      if (text)                                                               \
        _display->drawStr (U8G2_FIRST_COLUMN, row, text);                     \
      _display->drawStr (U8G2_SECOND_COLUMN, row, tempBuf, this);             \
    }                                                                         \
  while (0)