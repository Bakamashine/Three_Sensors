#pragma once
#include "interface/display.h"

/// menu rows: one per sensor row of the ACP table
#define MENU_ROWS_COUNT 4

struct Records
{
  Sensor *cls = nullptr;
  int row = 0;
  int col = 0;
  const char *preview = nullptr;
};

class MenuUI
{
private:
  IDisplay *_display;
  int _selected = 0;
  bool _isValueOpen = false;
  Records _rows[MENU_ROWS_COUNT];

  void buildRows();

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
  void drawPreviews();
};
