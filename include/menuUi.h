#pragma once
#include "interface/display.h"

enum MenuItem
{
  CHANGE_HYSTERESIS,
  CHANGE_TEMPERATURE,
  CHANGE_CORRECT_INT,
  MENU_ITEMS_COUNT,
};

class MenuUI
{
private:
  IDisplay *_display;
  int _selected;
  bool _isValueOpen = false;

public:
  explicit MenuUI(IDisplay *display);
  void goToUp();
  void goToDown();
  void draw();
  void openValue();
  void closeValue();
  bool isValueOpen();
  void increaseValue();
  void decreaseValue();
};
