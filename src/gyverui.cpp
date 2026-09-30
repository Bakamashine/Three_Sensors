#include "constants/constants.h"
#if UI_BACKEND == UI_BACKEND_GYVER

#include <Arduino.h>
#include <string.h>

#include "gyverui.h"
#include "settings.h"
#include "page.h"
#include "constants/ui.h"
#include "menuUi.h"
#include "helperUi.h"
#include "contest.h"
#include "macro/ui.h"

#define UI_CTOR                                                                   \
  template <int _TYPE, int _BUFF, int _CONN, int8_t _CS, int8_t _DC, int8_t _RST> \
  GyverUI<_TYPE, _BUFF, _CONN, _CS, _DC, _RST>::

#define UI_METHOD(ReturnType)                                                     \
  template <int _TYPE, int _BUFF, int _CONN, int8_t _CS, int8_t _DC, int8_t _RST> \
  ReturnType GyverUI<_TYPE, _BUFF, _CONN, _CS, _DC, _RST>::

// #define WRITE_ROW(row, text, acp, t)                                  \
//   do                                                                    \
//   {                                                                     \
//     if (text) drawStr(G_CENTERED_X(G_FIRST_COLUMN, text), row, text);    \
//     drawStr(G_CENTERED_X(G_ACP_X, acp), row, acp);                       \
//     drawStr(G_CENTERED_X(G_TEMP_X, t), row, t);                          \
//   } while (0)

// raw writing row
#define _WRITE_ROW(row, text, acp, t) \
  drawStr(G_FIRST_COLUMN, row, text);  \
  drawStr(G_SECOND_COLUMN, row, acp);  \
  drawStr(G_THIRD_COLUMN, row, t);

#define WRITE_ROW(row, text, p_cls)                             \
  do                                                            \
  {                                                             \
    char acpBuf[8];                                             \
    char tempBuf[8];                                            \
    snprintf(tempBuf, sizeof(tempBuf), "%d", (p_cls).getTemp()); \
    snprintf(acpBuf, sizeof(acpBuf), "%d", (p_cls).getAcp());     \
    _WRITE_ROW(row, text, acpBuf, tempBuf);                      \
  } while (0)

const unsigned char epd_bitmap_Capture[] PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xe0, 0xff, 0xff, 0xff, 0xfd, 0x1f, 0xff, 0xff, 0x3f, 0xbf, 0xff, 0x6f, 0xff, 0xe8, 0xdd, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x9a, 0xff, 0xff, 0x3f, 0x3f, 0xfc, 0xff, 0x68, 0xfe, 0xbc, 0xf7, 0xff, 0xbd, 0x3f, 0xf9, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xc5, 0xff, 0x3e, 0xff, 0xfc, 0xff, 0xff, 0xf2, 0x3f, 0x9f, 0xff, 0xff, 0xff, 0xed, 0xff, 0xff, 0xdf, 0x3f, 0xff, 0xff, 0xff, 0xbf, 0xff, 0x7b, 0xff, 0xdf, 0x3f, 0xff, 0xff, 0xbf, 0xbf, 0x6f, 0xff, 0xff, 0xff, 0x3f, 0xfb, 0xdf, 0x8f, 0xff, 0xfd, 0xbf, 0xff, 0xfd, 0x3f, 0xbb, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0x3b, 0xff,
    0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xe7, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0x08, 0x3f, 0x4f, 0xff, 0xfb, 0x7f, 0xef, 0xff, 0xff, 0x7a, 0x3f, 0xdf, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x7e, 0xff, 0xef, 0x3f, 0xf7, 0xff, 0xc7, 0xff, 0xfa, 0xff, 0xff, 0xf7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x8f, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x47, 0x3f, 0xcc, 0xff, 0xef, 0xff, 0xf7, 0xfd, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xdf, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xbd, 0xff, 0xff, 0xf7, 0x9e, 0xef, 0xff, 0xff, 0x3f, 0x02, 0xff, 0xe3, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xbf, 0xff, 0xf4, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0xc2, 0xff, 0x7f, 0xff, 0xfb, 0xa3, 0xff, 0xdf, 0x3f, 0x5d, 0xff, 0x7f, 0xff, 0xfb, 0xf7, 0xff, 0xfb, 0x3f, 0xf7, 0xff, 0xed, 0xff, 0x7f, 0xfe, 0xff, 0xfd, 0x3f, 0x43, 0xff, 0xf1, 0xff, 0xc9, 0xbf, 0xff, 0xef, 0x3f, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xdf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

UI_CTOR GyverUI(uint8_t address)
    : Base(address)
{
  tempText[0] = '\0';
  resText[0] = '\0';
  acpText[0] = '\0';
  menuUI = new MenuUI(this);
}

UI_CTOR ~GyverUI()
{
  delete menuUI;
}

UI_METHOD(void)
initDisplay(int sda, int scl)
{
  Base::init(sda, scl);
  clearDisplay();
  Base::update();
}

UI_METHOD(MenuUI &)
getMenuUI()
{
  return *menuUI;
}

UI_METHOD(IDisplay &)
setTemperature(float temp)
{
  _temperature = temp;
  return *this;
}

UI_METHOD(IDisplay &)
setAcp(int acp)
{
  _acp = acp;
  return *this;
}

UI_METHOD(IDisplay &)
setRes(float v)
{
  _resistance = v;
  return *this;
}

UI_METHOD(void)
initUI()
{
  snprintf(tempText, sizeof(tempText), "Temperature: %d", static_cast<int>(_temperature));
  setFloatText(resText, sizeof(resText), "Resistance", _resistance);
  snprintf(acpText, sizeof(acpText), "ACP: %d", _acp);
}

UI_METHOD(void)
main()
{

  // columns
  _WRITE_ROW(G_FIRST_ROW, nullptr, PREVIEW_ACP_TEXT, PREVIEW_COLUMN_TEMP);
  // rows

  // main sensor

  if (_mainSensor)
    WRITE_ROW(G_SECOND_ROW, PREVIEW_TEMP, *_mainSensor);

  // first reserve sensor
  if (_firstReserveSensor)
    WRITE_ROW(G_THIRD_ROW, PREVIEW_FIRST_RESERVE, *_firstReserveSensor);

  // second reserve sensor
  if (_secondReserveSensor)
    WRITE_ROW(G_FOURTH_ROW, PREVIEW_SECOND_RESERVE, *_secondReserveSensor);
  
  // street sensor
  if (_streetSensor)
    WRITE_ROW(G_FIFTH_ROW, PREVIEW_STREET, *_streetSensor);

  // _WRITE_ROW(G_THIRD_ROW, PREVIEW_FIRST_RESERVE, "0.00", "0.00");

  // WRITE_ROW(G_FOURTH_ROW, PREVIEW_STREET, "0.00", "0.00");

  // drawStr(TEMP_X, TEMP_Y, tempText);
  // drawStr(VOLT_X, VOLT_Y, resText);
  // drawStr(BURNER_X, BURNER_Y, acpText);
}

UI_METHOD(void)
draw()
{
  initUI();
  clearDisplay();
  setFont(FONT_UI);
  switch (Page::getCurrentPage())
  {
  case MAIN_PAGE:
    main();
    break;
  case SELECT_SETTINGS:
    menuUI->draw();
    break;
  default:
    break;
  }
  Base::update();
}

UI_METHOD(void)
startWindow()
{
  clearDisplay();
  setFont(FONT_START);
  int x = _CENTER_X(FACTORY_NAME);
  int y = _CENTER_Y;
  drawStr(x, y, FACTORY_NAME);
  Base::drawBitmap(CENTER_X / 2, CENTER_Y / 2, epd_bitmap_Capture, 70, 40);
  Base::update();
  delay(START_MENU_DURATION);
}

UI_METHOD(void)
clearDisplay()
{
  Base::clear();
}

UI_METHOD(void)
setFont(UiFont font)
{
  const uint8_t scale = 1;
  if (scale != _fontScale)
  {
    _fontScale = scale;
    Base::setScale(scale);
  }
}

UI_METHOD(void)
drawStr(int x, int y, const char *str)
{
  Base::setCursorXY(x, y);
  Base::print(str);
}

UI_METHOD(int)
getStrWidth(const char *str)
{
  return 6 * _fontScale * static_cast<int>(strlen(str));
}

UI_METHOD(int)
getFontHeight()
{
  return 8 * _fontScale;
}

UI_METHOD(IDisplay &)
setMainSensor(Sensor &sn)
{
  _mainSensor = &sn;
  return *this;
}

UI_METHOD(IDisplay &)
setFirstReserveSensor(Sensor &sn)
{
  _firstReserveSensor = &sn;
  return *this;
}

UI_METHOD(IDisplay &)
setStreetSensor(Sensor &sn)
{
  _streetSensor = &sn;
  return *this;
}

UI_METHOD(Sensor &)
getMainSensor()
{
  return *_mainSensor;
}

UI_METHOD(Sensor &)
getFirstReserveSensor()
{
  return *_firstReserveSensor;
}

UI_METHOD(Sensor &)
getStreetSensor()
{
  return *_streetSensor;
}

UI_METHOD(IDisplay &)
setSecondReserveSensor(Sensor &sn)
{
  _secondReserveSensor = &sn;
  return *this;
}

UI_METHOD(Sensor &)
getSecondReserveSensor()
{
  return *_secondReserveSensor;
}

template class GyverUI<GYVER_PANEL, GYVER_BUFFER, GYVER_CONN, GYVER_CS, GYVER_DC, GYVER_RST>;

#endif