#include "menuUi.h"
#include <stdio.h>
#include "helperUi.h"
#include "sensor.h"
#include "constants/constants.h"
#include "constants/ui.h"
#include "constants/settings.h"
#include "contest.h"
#include "macro/ui.h"
#include "settings.h"
#include "page.h"

MenuUI::MenuUI(IDisplay *display) : _display(display)
{
}

void MenuUI::buildRows()
{
  _rows[0].cls = &_display->getMainSensor();
  _rows[0].row = U8G2_SECOND_ROW;
  _rows[0].col = U8G2_FIRST_COLUMN;
  _rows[0].preview = PREVIEW_T1;

  _rows[1].cls = &_display->getFirstReserveSensor();
  _rows[1].row = U8G2_THIRD_ROW;
  _rows[1].col = U8G2_FIRST_COLUMN;
  _rows[1].preview = PREVIEW_T2;

  _rows[2].cls = &_display->getSecondReserveSensor();
  _rows[2].row = U8G2_FOURTH_ROW;
  _rows[2].col = U8G2_FIRST_COLUMN;
  _rows[2].preview = PREVIEW_T3;

  _rows[3].cls = &_display->getStreetSensor();
  _rows[3].row = U8G2_FIFTH_ROW;
  _rows[3].col = U8G2_FIRST_COLUMN;
  _rows[3].preview = PREVIEW_T4;
}

void MenuUI::drawPoint()
{
  for (int i = 0; i < MENU_ROWS_COUNT; i++)
  {
    _display->drawStr(U8G2_MENU_MARKER_X, _rows[i].row, i == _selected ? ">" : " ");
  }
}

void MenuUI::drawPreviews()
{
  for (int i = 0; i < MENU_ROWS_COUNT; i++)
  {
    if (_rows[i].preview)
      _display->drawStr(_rows[i].col, _rows[i].row, _rows[i].preview);
  }
}

void MenuUI::goToUp()
{
  _selected = (_selected - 1 + MENU_ROWS_COUNT) % MENU_ROWS_COUNT;
}

void MenuUI::goToDown()
{
  _selected = (_selected + 1) % MENU_ROWS_COUNT;
}

void MenuUI::draw()
{
  _display->setFont(FONT_UI);
  _display->drawStr(_CENTER_X(*_display, PREVIEW_MENU), _CENTER_Y(*_display), PREVIEW_MENU);

  
  buildRows();
  drawPreviews();
  drawPoint();

  int ci_main_sensor = _display->getMainSensor().getCorrectInt();
  int ci_first_sensor = _display->getFirstReserveSensor().getCorrectInt();
  int ci_second_sensor = _display->getSecondReserveSensor().getCorrectInt();
  int ci_street_sensor = _display->getStreetSensor().getCorrectInt();

  const int size = 8;
  char ci_main_sensor_str[size];
  char ci_first_sensor_str[size];
  char ci_second_sensor_str[size];
  char ci_street_sensor_str[size];
  setIntText(ci_main_sensor_str, size, ci_main_sensor);
  setIntText(ci_first_sensor_str, size, ci_first_sensor);
  setIntText(ci_second_sensor_str, size, ci_second_sensor);
  setIntText(ci_street_sensor_str, size, ci_street_sensor);
  RAW_WRITE_ROW(U8G2_SECOND_ROW, PREVIEW_T1, ci_main_sensor_str, nullptr, _display);
  RAW_WRITE_ROW(U8G2_THIRD_ROW, PREVIEW_T2, ci_first_sensor_str, nullptr, _display);
  RAW_WRITE_ROW(U8G2_FOURTH_ROW, PREVIEW_T3, ci_second_sensor_str, nullptr, _display);
  RAW_WRITE_ROW(U8G2_FIFTH_ROW, PREVIEW_T4, ci_street_sensor_str, _isValueOpen ? "ON" : nullptr, _display);

  const int message_size = 16;
  char hyst[message_size];
  char max_permitted_offset[message_size];
  char min_permitted_offset[message_size];
  snprintf(max_permitted_offset, message_size, "%s %d", PREVIEW_MAX, Settings::getMaxPermOffset());
  snprintf(min_permitted_offset, message_size, "%s %d", PREVIEW_MIN, Settings::getMinPermOffset());
  snprintf(hyst, message_size, "%s %d", PREVIEW_HYST, Settings::getHysteresis());
  RAW_WRITE_ROW(U8G2_SECOND_ROW, PREVIEW_T1, nullptr, hyst, _display);
  RAW_WRITE_ROW(U8G2_THIRD_ROW, PREVIEW_T2, nullptr, max_permitted_offset, _display);
  RAW_WRITE_ROW(U8G2_FOURTH_ROW, PREVIEW_T3, nullptr, min_permitted_offset, _display);
}

void MenuUI::setValueStatus(bool st) { _isValueOpen = st; }
bool MenuUI::isValueOpen() { return _isValueOpen; }

void MenuUI::increaseValue()

{
  Sensor *cls = _rows[_selected].cls;
  int old_value = cls->getCorrectInt();
  cls->setCorrectInt(++old_value);
}

void MenuUI::decreaseValue()
{
  Sensor *cls = _rows[_selected].cls;
  int old_value = cls->getCorrectInt();
  cls->setCorrectInt(--old_value);
}
