#pragma once

#define _CENTER_X(text) ((OLED_WIDTH - getStrWidth(text)) / 2)
#define _CENTER_Y ((OLED_HEIGHT + getFontHeight()) / 2 - 25)