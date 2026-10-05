#pragma once
#include "constants/constants.h"
#if UI_BACKEND == UI_BACKEND_U8G2
#include "constants/constants.h"
#include "interface/display.h"
#include "macro/getter_setter_sensor.h"
#include "menuUi.h"
#include "sensor.h"
#include <U8g2lib.h>
#include <stdint.h>

class UI : public OLED_CLASS, public IDisplay
{
private:
  float _temperature;
  float _resistance;
  int _errorCode = 0;
  int _acp;
  MenuUI *menuUI;

  SENSOR_MEMBERS;
  SENSOR_TEMP_MEMBERS;
  SENSOR_ACP_MEMBERS;
  void main () override;
  void permanent () override;

public:
  UI ();
  ~UI () override;

  void clearDisplay () override;
  void setFont (UiFont font) override;
  void drawStr (int x, int y, const char *str) override;
  int getStrWidth (const char *str) override;
  int getFontHeight () override;

  void initDisplay (int sda = -1, int scl = -1) override;
  void initUI () override;
  IDisplay &setTemperature (float) override;
  void draw () override;
  MenuUI &getMenuUI () override;
  void startWindow () override;
  SENSOR_GETTER_SETTER (IDisplay);
  O_SENSOR_TEMP_GETTER_SETTER (IDisplay);
  O_SENSOR_ACP_GETTER_SETTER (IDisplay);

  void error () override;
  int drawCentered (const char *, int padding_top = 0, int padding_bottom = 0,
                    int padding_left = 0, int padding_right = 0);
};
#endif
