#include <Arduino.h>

#include "constants/constants.h"

#ifdef ENABLE_COMMANDS
#include "command.h"
#endif
#include "constants/pin.h"
#include "debugUi.h"
#include "error.h"
#include "macro/shortcut.h"
#include "page.h"
#include "sensor.h"
#include "settings.h"

// button classes
#if BTN_BACKEND == BTN_BACKEND_ONE_BUTTON_FULL
#include <OneButton.h>
#define BTN_CLASS OneButton
#elif BTN_BACKEND == BTN_BACKEND_ONE_BUTTON_TINY
#include <OneButtonTiny.h>
#define BTN_CLASS OneButtonTiny
#elif BTN_BACKEND == BTN_BACKEND_CUSTOM
#include "button.h"
#define BTN_CLASS Button
#endif

// ui includes
#if UI_BACKEND == UI_BACKEND_U8G2
#include "ui.h"
UI uiImpl;
#elif UI_BACKEND == UI_BACKEND_GYVER
#include "gyverui.h"
GyverUI<GYVER_PANEL> uiImpl;
#endif

#ifdef ENABLE_VALIDATE
#include "validate.h"
Validate validate;
#endif

void sensorSetup ();

// main
// reserve 1
// reserve 2
// street
Sensor main_sensor (MAIN_SENSOR_PIN);
Sensor first_res_sensor (FIRST_RESERVE_SENSOR_PIN);
Sensor second_res_sensor (SECOND_RESERVE_SENSOR_PIN);
Sensor street_sensor (STREET_SENSOR_PIN);

// classes
IDisplay &ui = uiImpl;
MenuUI &menuUI = ui.getMenuUI ();
#ifdef ENABLE_COMMANDS
Command cmd;
#endif

float main_temp = 0;
float first_res_temp = 0;
float second_res_temp = 0;
float street_res_temp = 0;

int main_acp = 0;
int first_res_acp = 0;
int second_res_acp = 0;
int street_res_acp = 0;

BTN_CLASS btn_plus (PLUS_BTN_PIN);
BTN_CLASS btn_minus (MINUS_BTN_PIN);
BTN_CLASS btn_menu (MENU_BTN_PIN);

bool systemHalted = false;
void haltSystem ();

// button events
void btnPlusOneClick ();
void btnPlusLongPress ();
void btnMinusOneClick ();
void btnMinusLongPress ();
void btnMenuOnClick ();
void btnMenuLongPress ();
void btnSetup ();

void ledSetup ();
void setupCommands ();
#ifdef ENABLE_LED_DEBUG
void ledDebug ();
#endif

void
setup ()
{
  Serial.begin (BOD);
  DebugUI::printTitle ("Setup");

  ui.initDisplay ();
  // #if defined(SHOW_START_WINDOW) && !defined(DEBUG)
#ifdef SHOW_START_WINDOW
  ui.startWindow ();
#endif

  sensorSetup ();
  // toggleSetup();
  btnSetup ();
#ifdef ENABLE_COMMANDS
  setupCommands ();
#endif
  ledSetup ();
}

void
loop ()
{
#ifdef ENABLE_LED_DEBUG
  ledDebug ();
#endif
  // DebugUI::printTitle ("main loop");
  btn_plus.tick ();
  btn_minus.tick ();
  btn_menu.tick ();

  // setting temperature and acp
  main_temp = main_sensor.getTemp ();
  first_res_temp = first_res_sensor.getTemp ();
  second_res_temp = second_res_sensor.getTemp ();
  street_res_temp = street_sensor.getTemp ();
  main_acp = main_sensor.getAcp ();
  first_res_acp = first_res_sensor.getAcp ();
  second_res_acp = second_res_sensor.getAcp ();
  street_res_acp = street_sensor.getAcp ();

#ifdef DEBUG
  DebugUI::printValue ("street_res_acp", street_res_acp);
#endif

#ifdef ENABLE_COMMANDS
  cmd.feedSerial ();
#endif
  ui.setMainTemp (main_temp)
      .setFirstResTemp (first_res_temp)
      .setSecondResTemp (second_res_temp)
      .setStreetTemp (street_res_temp)
      .setMainAcp (main_acp)
      .setFirstResAcp (first_res_acp)
      .setSecondResAcp (second_res_acp)
      .setStreetAcp (street_res_acp)
      .draw ();

#ifdef ENABLE_VALIDATE
  validate.setMainTemp (main_temp)
      .setFirstResTemp (first_res_temp)
      .setSecondResTemp (second_res_temp)
      .setStreetTemp (street_res_temp)
      .pipeline ();
  if (Error::getErrorStatus ())
    haltSystem ();
#endif
}

void
sensorSetup ()
{
  pinMode (MAIN_SENSOR_PIN, INPUT);
  pinMode (FIRST_RESERVE_SENSOR_PIN, INPUT);
  pinMode (SECOND_RESERVE_SENSOR_PIN, INPUT);
  pinMode (STREET_SENSOR_PIN, INPUT);

  ui.setMainSensor (main_sensor)
      .setFirstReserveSensor (first_res_sensor)
      .setSecondReserveSensor (second_res_sensor)
      .setStreetSensor (street_sensor);
}

void
btnMenuOnClick ()
{
  if (Page::getCurrentPage () == SETTINGS)
    Page::setCurrentPage (MAIN_PAGE);
  else
    {
      Page::setCurrentPage (SETTINGS);
      menuUI.setValueStatus (false);
    }

#ifdef DEBUG
  Serial.println ("menuClick");
#endif
}
void
btnMenuLongPress ()
{
}

void
btnPlusLongPress ()
{
  if (Page::getCurrentPage () == SETTINGS)
    menuUI.setValueStatus (true);
}

void
btnPlusOneClick ()
{
  if (Page::getCurrentPage () == SETTINGS && !menuUI.isValueOpen ())
    {
      menuUI.goToUp ();
#ifdef DEBUG
      Serial.println ("goToUp");
#endif
    }
  else if (Page::getCurrentPage () == SETTINGS && menuUI.isValueOpen ())
    {
      menuUI.increaseValue ();
    }
}

void
btnMinusOneClick ()
{
  if (Page::getCurrentPage () == SETTINGS && !menuUI.isValueOpen ())
    {
      menuUI.goToDown ();
#ifdef DEBUG
      Serial.println ("goToDown");
#endif
    }
  else if (Page::getCurrentPage () == SETTINGS && menuUI.isValueOpen ())
    {
      menuUI.decreaseValue ();
    }
}

void
btnMinusLongPress ()
{
  if (Page::getCurrentPage () == SETTINGS)
    menuUI.setValueStatus (false);
}

void
btnSetup ()
{
#if BTN_BACKEND != BTN_BACKEND_ONE_BUTTON_TINY
  bool activeLow = true;
#endif
  const int debounce = 20;
  auto mode = INPUT_PULLUP;
  pinMode (MENU_BTN_PIN, mode);
  pinMode (PLUS_BTN_PIN, mode);
  pinMode (MINUS_BTN_PIN, mode);

#if BTN_BACKEND == BTN_BACKEND_ONE_BUTTON_FULL                                \
    || BTN_BACKEND == BTN_BACKEND_ONE_BUTTON_TINY
  btn_menu.setDebounceMs (debounce);
  btn_plus.setDebounceMs (debounce);
  btn_minus.setDebounceMs (debounce);
#if BTN_BACKEND == BTN_BACKEND_ONE_BUTTON_FULL
  btn_menu.setup (MENU_BTN_PIN, activeLow, true);
  btn_plus.setup (PLUS_BTN_PIN, activeLow, true);
  btn_minus.setup (MINUS_BTN_PIN, activeLow, true);
#endif
  btn_menu.attachClick (btnMenuOnClick);
  btn_menu.attachLongPressStart (btnMenuLongPress);
  btn_plus.attachClick (btnPlusOneClick);
  btn_plus.attachLongPressStart (btnPlusLongPress);
  btn_minus.attachClick (btnMinusOneClick);
  btn_minus.attachLongPressStart (btnMinusLongPress);

#elif BTN_BACKEND == BTN_BACKEND_CUSTOM
  btn_menu.setup (MENU_BTN_PIN, mode, activeLow)
      .setDebounceMs (debounce)
      .attachClick (btnMenuOnClick)
      .attachLongPressStart (btnMenuLongPress);
  btn_plus.setup (PLUS_BTN_PIN, mode, activeLow)
      .setDebounceMs (debounce)
      .attachClick (btnPlusOneClick)
      .attachLongPressStart (btnPlusLongPress);
  btn_minus.setup (MINUS_BTN_PIN, mode, activeLow)
      .setDebounceMs (debounce)
      .attachClick (btnMinusOneClick)
      .attachLongPressStart (btnMinusLongPress);

#endif
}

#ifdef ENABLE_COMMANDS
void
setupCommands ()
{
  cmd.setMainSensor (main_sensor)
      .setFirstReserveSensor (first_res_sensor)
      .setSecondReserveSensor (second_res_sensor)
      .setStreetSensor (street_sensor);
}
#endif

void
ledSetup ()
{
  // burner
  pinMode (BURNER_PIN, OUTPUT);

  // error
  pinMode (ERROR_PIN, OUTPUT);
}

#ifdef ENABLE_LED_DEBUG
void
ledDebug ()
{

  digitalWrite (BURNER_PIN, HIGH);
  digitalWrite (ERROR_PIN, LOW);
}
#endif

void
haltSystem ()
{
  if (!systemHalted)
    {
      Serial.println ("ERROR");
      digitalWrite (BURNER_PIN, LOW);
      digitalWrite (ERROR_PIN, HIGH);
      Settings::setBurnerStatus (false);
    }
  systemHalted = true;
  Page::setCurrentPage (ERROR);
}