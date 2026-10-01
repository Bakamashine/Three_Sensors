#include <Arduino.h>
#include "constants/constants.h"
#include "debugUi.h"
#include "sensor.h"
#include "constants/constants.h"
#include <OneButton.h>
#include "validate.h"
#include "constants/pin.h"
#include "settings.h"
#include "page.h"

// ui includes
#if UI_BACKEND == UI_BACKEND_U8G2
#include "ui.h"
#elif UI_BACKEND == UI_BACKEND_GYVER
#include "gyverui.h"
#endif

#define IF_SETTINGS_OPEN_RETURN ({  if (!Settings::getSettingsStatus()) return ; })
typedef unsigned long ul;
void sensorSetup();

DebugUI debugUI;
// main
// reserve 1
// reserve 2
// street
Sensor main_sensor(MAIN_SENSOR_PIN);
Sensor first_reserve_sensor(FIRST_RESERVE_SENSOR_PIN);
Sensor second_reserve_sensor(SECOND_RESERVE_SENSOR_PIN);
Sensor street_sensor(STREET_SENSOR_PIN);

#if UI_BACKEND == UI_BACKEND_U8G2
UI uiImpl;
#elif UI_BACKEND == UI_BACKEND_GYVER
GyverUI<GYVER_PANEL> uiImpl;
#endif
IDisplay &ui = uiImpl;
MenuUI &menuUI = ui.getMenuUI();
Validate val;

float temperature = 0.0F;
float res = 0.0F;
OneButton btn_plus;
OneButton btn_minus;
OneButton btn_menu;

// bool systemHalted = false;
// void systemHalt();

// button events
void btnPlusOneClick();
void btnPlusLongPress();
void btnMinusOneClick();
void btnMinusLongPress();
void btnMenuOnClick();
void btnMenuLongPress();
void btnSetup();

void ledProgramStatus(bool);

void setup()
{
  Serial.begin(BOD);
  DebugUI::printTitle("Setup");
  // ui.begin();
  // ui.initUI();

  ui.initDisplay();
  // #if defined(SHOW_START_WINDOW) && !defined(DEBUG)
#ifdef SHOW_START_WINDOW
  ui.startWindow();
#endif

  sensorSetup();
  // toggleSetup();
  btnSetup();
}

void loop()
{
  btn_plus.tick();
  btn_minus.tick();
  btn_menu.tick();
  // static uint32_t lastMs = 0;
  // const uint32_t now = millis();
  // if (now - lastMs < TEMP_DELAY)
  //   return;
  // lastMs = now;

  temperature = main_sensor.getTemp();
  res = main_sensor.getRes();
#ifdef DEBUG
  // Serial.println(temp.getVolt());
  Serial.print("Resistance: ");
  Serial.println(res);
  // Serial.println(temperature);
  // debugUI.fprintValue("Volt", temp.getVolt());
  // debugUI.fprintValue("Resistance", temp.getRes());
  debugUI.printValue("ACP", main_sensor.getAcp());
  debugUI.printValue("Temperature", temperature);
#endif
  ui
      .draw();

  /// validate
  // int code = val.setTemperature().executePipelineValidate();
  // Settings::setErrorStatus(code);
  // if (code>0) ui.setError(code);

  /// burner
  // if (!systemHalted)
  // {
  //   if (temp.getTemp() <= Settings::getUserTemp() - Settings::getHysteresis())
  //   {
  //     Settings::setBurnerStatus(true);

  //   }
  // }

  // Settings::setSettingsStatus(digitalRead(TOGGLE_PIN) == HIGH);
}

void sensorSetup()
{
  pinMode(MAIN_SENSOR_PIN, INPUT);
  pinMode(FIRST_RESERVE_SENSOR_PIN, INPUT);
  pinMode(SECOND_RESERVE_SENSOR_PIN, INPUT);
  pinMode(STREET_SENSOR_PIN, INPUT);

  ui
      .setMainSensor(main_sensor)
      .setFirstReserveSensor(first_reserve_sensor)
      .setSecondReserveSensor(second_reserve_sensor)
      .setStreetSensor(street_sensor);
}

void btnMenuOnClick()
{
  if (Page::getCurrentPage() == SETTINGS)
    Page::setCurrentPage(MAIN_PAGE);
  else
  {
    Page::setCurrentPage(SETTINGS);
    menuUI.setValueStatus(false);
  }

#ifdef DEBUG
  Serial.println("menuClick");
#endif
}
void btnMenuLongPress() {}

void btnPlusLongPress()
{
  if (Page::getCurrentPage() == SETTINGS)
    menuUI.setValueStatus(true);
}

void btnPlusOneClick()
{
  if (Page::getCurrentPage() == SETTINGS && !menuUI.isValueOpen())
  {
    menuUI.goToUp();
#ifdef DEBUG
    Serial.println("goToUp");
#endif
  }
  else if (Page::getCurrentPage() == SETTINGS && menuUI.isValueOpen())
  {
    menuUI.increaseValue();
  }
}

void btnMinusOneClick()
{
  if (Page::getCurrentPage() == SETTINGS && !menuUI.isValueOpen())
  {
    menuUI.goToDown();
#ifdef DEBUG
    Serial.println("goToDown");
#endif
  }
  else if (Page::getCurrentPage() == SETTINGS && menuUI.isValueOpen())
  {
    menuUI.decreaseValue();
  }
}

void btnMinusLongPress()
{
  if (Page::getCurrentPage() == SETTINGS)
    menuUI.setValueStatus(false);
}

void btnSetup()
{
  auto mode = INPUT_PULLUP;
  bool activeLow = true;
  const int debounce = 20;
  pinMode(MENU_BTN_PIN, mode);
  pinMode(PLUS_BTN_PIN, mode);
  pinMode(MINUS_BTN_PIN, mode);
  btn_plus.setDebounceMs(debounce);
  btn_minus.setDebounceMs(debounce);
  btn_menu.setDebounceMs(debounce);
  btn_menu.setup(MENU_BTN_PIN, mode, activeLow);
  btn_plus.setup(PLUS_BTN_PIN, mode, activeLow);
  btn_minus.setup(MINUS_BTN_PIN, mode, activeLow);
  btn_menu.attachClick(btnMenuOnClick);
  btn_menu.attachDuringLongPress(btnMenuLongPress);
  btn_plus.attachClick(btnPlusOneClick);
  btn_plus.attachLongPressStart(btnPlusLongPress);
  btn_minus.attachClick(btnMinusOneClick);
  btn_minus.attachLongPressStart(btnMinusLongPress);
}

// void ledProgramStatus(bool status)
// {
// }

// void haltSystem()
// {
//   digitalWrite(BURNER_PIN, LOW);
//   Settings::setBurnerStatus(false);
//   Settings::setSettingsStatus(false); // show the error overlay instead of the menu
// }
