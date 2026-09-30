#pragma once

#define FONT u8g2_font_squeezed_b7_tr
#define START_FONT u8g2_font_squeezed_b7_tr
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define CENTER_X OLED_WIDTH / 2
#define CENTER_Y OLED_HEIGHT / 2
// columns

#define U8G2_SECOND_COLUMN_X 80
#define U8G2_FIRST_COLUMN_X 0

// temperature
#define TEMP_Y 10
#define TEMP_X U8G2_FIRST_COLUMN_X

// deltaValue
#define HYSTERESIS_Y 10
#define HYSTERESIS_X U8G2_SECOND_COLUMN_X

// voltage
#define VOLT_Y 30
#define VOLT_X U8G2_FIRST_COLUMN_X

// user temperature
#define USERTEMP_Y 30
#define USERTEMP_X U8G2_SECOND_COLUMN_X

// burner
#define BURNER_Y 50
#define BURNER_X 0
