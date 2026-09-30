#include "menuUi.h"
#include "constants/constants.h"
#include "constants/ui.h"
#include "constants/settings.h"
#include "settings.h"
#include "page.h"

static const char *const names[MENU_ITEMS_COUNT] = {"Change hysteresis", "Change temperature", "Ch. value for cor. sensor"};

MenuUI::MenuUI(U8G2 *_display)
    : _display(_display), _selected(CHANGE_HYSTERESIS)
{
}

void MenuUI::goToUp()
{
  _selected = (_selected - 1 + MENU_ITEMS_COUNT) % MENU_ITEMS_COUNT;
}

void MenuUI::goToDown()
{
  _selected = (_selected + 1) % MENU_ITEMS_COUNT;
}

void MenuUI::draw()
{
  _display->setFont(FONT);

  if (_isValueOpen)
  {
    char buf[32];
    const char *label;
    int value;
    switch (_selected)
    {
    case CHANGE_HYSTERESIS:
      label = "Hysteresis:";
      value = Settings::getHysteresis();
      break;
    case CHANGE_CORRECT_INT:
      label = "CorrectInt:";
      value = Settings::getCorrectInt();
      break;
    default:
      label = "UserTemp:";
      value = Settings::getUserTemp();
      break;
    }
    snprintf(buf, sizeof(buf), "%s %d", label, value);
    _display->drawStr(TEMP_X, TEMP_Y, buf);
    return;
  }

  int y = 15;
  for (int i = 0; i < MENU_ITEMS_COUNT; i++)
  {
    _display->drawStr(0, y, i == _selected ? ">" : " ");
    _display->drawStr(8, y, names[i]);
    y += 12;
  }
}

void MenuUI::openValue()
{
  _isValueOpen = true;
}

void MenuUI::closeValue()
{
  _isValueOpen = false;
}

bool MenuUI::isValueOpen()
{
  return _isValueOpen;
}

void MenuUI::increaseValue()
{
  switch (_selected)
  {
  case CHANGE_HYSTERESIS:
  {
    int d = Settings::getHysteresis() + 1;
    if (d >= MIN_DELTA && d <= MAX_DELTA)
      Settings::setHysteresis(d);
    break;
  }
  case CHANGE_TEMPERATURE:
    Settings::upUserTemp();
    break;
  case CHANGE_CORRECT_INT:
    Settings::setCorrectInt(Settings::getCorrectInt() + 1);
    break;
  default:
    break;
  }
}

void MenuUI::decreaseValue()
{
  switch (_selected)
  {
  case CHANGE_HYSTERESIS:
  {
    int d = Settings::getHysteresis() - 1;
    if (d >= MIN_DELTA && d <= MAX_DELTA)
      Settings::setHysteresis(d);
    break;
  }
  case CHANGE_TEMPERATURE:
    Settings::downUserTemp();
    break;
  case CHANGE_CORRECT_INT:
    Settings::setCorrectInt(Settings::getCorrectInt() - 1);
    break;
  default:
    break;
  }
}