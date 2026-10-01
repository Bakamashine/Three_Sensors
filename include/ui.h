#pragma once
#include "constants/constants.h"
#if UI_BACKEND == UI_BACKEND_U8G2
#include <stdint.h>
#include <U8g2lib.h>
#include "menuUi.h"
#include "sensor.h"
#include "constants/constants.h"
#include "interface/display.h"

class UI : public OLED_CLASS, public IDisplay
{
private:
  float _temperature;
  float _resistance;
  int _errorCode = 0;
  int _acp;
  MenuUI *menuUI;

  Sensor *_mainSensor = nullptr;
  Sensor *_firstReserveSensor = nullptr;
  Sensor *_secondReserveSensor = nullptr;
  Sensor *_streetSensor = nullptr;
  void main() override;

public:
  UI();
  ~UI() override;

  void clearDisplay() override;
  void setFont(UiFont font) override;
  void drawStr(int x, int y, const char *str) override;
  int getStrWidth(const char *str) override;
  int getFontHeight() override;

  void initDisplay(int sda = -1, int scl = -1) override;
  void initUI() override;
  IDisplay &setTemperature(float) override;
  void draw() override;
  IDisplay &setAcp(int) override;
  IDisplay &setRes(float) override;
  MenuUI &getMenuUI() override;
  void startWindow() override;
  IDisplay &setMainSensor(Sensor &) override;
  IDisplay &setFirstReserveSensor(Sensor &) override;
  IDisplay &setSecondReserveSensor(Sensor &) override;
  IDisplay &setStreetSensor(Sensor &) override;
  Sensor &getMainSensor() override;
  Sensor &getFirstReserveSensor() override;
  Sensor &getSecondReserveSensor() override;
  Sensor &getStreetSensor() override;
  int drawCentered(const char *, int padding_top = 0, int padding_bottom = 0, int padding_left = 0, int padding_right = 0);
};
#endif
