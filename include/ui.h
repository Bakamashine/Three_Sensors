#pragma once
#include <stdint.h>
#include <U8g2lib.h>
#include "menuUi.h"
#include "constants/constants.h"

#define DEFAULT_SIZE 24

class UI : public OLED_CLASS
{
private:
  float _temperature;
  float _resistance;
  int _errorCode = 0;
  int _acp;
  char tempText[DEFAULT_SIZE];
  // char hysteresisText[DEFAULT_SIZE];
  char acpText[DEFAULT_SIZE];
  char resText[DEFAULT_SIZE];
  // char userTempText[DEFAULT_SIZE];
  // char burnerText[DEFAULT_SIZE];

  // void setText(char*, size_t, const char*, ...);
  MenuUI *menuUI;
  void main();

public:
  UI();
  void initUI();
  ~UI()
  {
    free(menuUI);
  }
  UI &setTemperature(float);
  void draw();
  UI &setAcp(int);
  UI &setRes(float);
  MenuUI &getMenuUI();
  void startWindow();
  int drawCentered(const char *, int padding_top = 0, int padding_bottom = 0, int padding_left = 0, int padding_right = 0);
};