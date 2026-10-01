#pragma once
#include "interface/display.h"



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

  
  void setValueStatus(bool);
  bool isValueOpen();
  void increaseValue();
  void decreaseValue();

  void drawPoint();
};
