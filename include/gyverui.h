#pragma once
#if UI_BACKEND == UI_BACKEND_GYVER
#include "constants/constants.h"
#include "sensor.h"

#include <GyverOLED.h>

#include "constants/gyverui.h"
#include "interface/display.h"
#include "macro/getter_setter_sensor.h"
#include "menuUi.h"

template <int _TYPE, int _BUFF = GYVER_BUFFER, int _CONN = GYVER_CONN,
          int8_t _CS = GYVER_CS, int8_t _DC = GYVER_DC,
          int8_t _RST = GYVER_RST>
class GyverUI : public GyverOLED<_TYPE, _BUFF, _CONN, _CS, _DC, _RST>,
                public IDisplay
{
  using Base = GyverOLED<_TYPE, _BUFF, _CONN, _CS, _DC, _RST>;

private:
  float _temperature = 0.0F;
  float _resistance = 0.0F;
  int _errorCode = 0;
  int _acp = 0;
  char tempText[DEFAULT_SIZE];
  char acpText[DEFAULT_SIZE];
  char resText[DEFAULT_SIZE];

  MenuUI *menuUI = nullptr;
  uint8_t _fontScale = 1;

  SENSOR_MEMBERS;
  void main () override;

public:
  explicit GyverUI (uint8_t address = GYVER_ADDRESS);
  ~GyverUI () override;

  void clearDisplay () override;
  void setFont (UiFont font) override;
  void drawStr (int x, int y, const char *str) override;
  int getStrWidth (const char *str) override;
  int getFontHeight () override;

  void initDisplay (int sda = -1, int scl = -1) override;
  void initUI () override;
  IDisplay &setTemperature (float) override;
  void draw () override;
  IDisplay &setAcp (int) override;
  IDisplay &setRes (float) override;
  MenuUI &getMenuUI () override;
  void startWindow () override;
  SENSOR_GETTER_SETTER (IDisplay);
};

#endif
