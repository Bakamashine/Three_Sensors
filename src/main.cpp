#include <Arduino.h>
#include "constants/constants.h"
#include "debugUi.h"
#include "sensor.h"
#include "constants/constants.h"
// #include <OneButton.h>
#include "validate.h"
#include "constants/pin.h"
#include "settings.h"

// ui includes
#if UI_BACKEND == UI_BACKEND_U8G2
#include "ui.h"
#elif UI_BACKEND == UI_BACKEND_GYVER
#include "gyverui.h"
#endif

#define IF_SETTINGS_OPEN_RETURN ({  if (!Settings::getSettingsStatus()) return ; })
typedef unsigned long ul;
void sensorSetup();
// void buttonSetup();
// void toggleSetup();

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
// OneButton btn_plus;
// OneButton btn_minus;

// bool systemHalted = false;
// void systemHalt();

// button events
// void btnPlusOneClick();
// void btnPlusLongPress();
// void btnMinusOneClick();
// void btnMinusLongPress();

void ledProgramStatus(bool);

void setup()
{
  Serial.begin(BOD);

  // ui.begin();
  // ui.initUI();

  ui.initDisplay();
  // #if defined(SHOW_START_WINDOW) && !defined(DEBUG)
#ifdef SHOW_START_WINDOW
  ui.startWindow();
#endif

  sensorSetup();
  // toggleSetup();
  // buttonSetup();
}

void loop()
{
  // btn_plus.tick();
  // btn_minus.tick();
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
  // ui
  //     .setTemperature(temperature)
  //     .setAcp(main_sensor.getAcp())
  //     .setRes(res)
  //     .setMainSensor(main_sensor)
  //     .draw();

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

// void buttonSetup()
// {
//   const int debounce = 20;
//   btn_plus.setup(BUTTON_PIN_PLUS, INPUT_PULLUP, true);
//   btn_minus.setup(BUTTON_PIN_MINUS, INPUT_PULLUP, true);
//   btn_plus.setDebounceMs(debounce);
//   btn_minus.setDebounceMs(debounce);

//   btn_plus.attachClick(btnPlusOneClick);
//   btn_plus.attachLongPressStart(btnPlusLongPress);
//   btn_minus.attachClick(btnMinusOneClick);
//   btn_minus.attachLongPressStart(btnMinusLongPress);
// }

// void btnPlusLongPress()
// {
//   IF_SETTINGS_OPEN_RETURN;
// #ifdef DEBUG
//   Serial.println("longPress btn_plus");
// #endif
//   menuUI.openValue();
// }

// void btnPlusOneClick()
// {
//   IF_SETTINGS_OPEN_RETURN;
//   if (menuUI.isValueOpen())
//     menuUI.increaseValue();
//   else
//     menuUI.goToUp();
// }

// void btnMinusOneClick()
// {
//   IF_SETTINGS_OPEN_RETURN;

//   if (menuUI.isValueOpen())
//     menuUI.decreaseValue();
//   else
//     menuUI.goToDown();
// }

// void btnMinusLongPress()
// {
//   IF_SETTINGS_OPEN_RETURN;
// #ifdef DEBUG
//   Serial.println("longPress btn_minus");
// #endif
//   menuUI.closeValue();
// }

// void ledProgramStatus(bool status)
// {
// }

// void haltSystem()
// {
//   digitalWrite(BURNER_PIN, LOW);
//   Settings::setBurnerStatus(false);
//   Settings::setSettingsStatus(false); // show the error overlay instead of the menu
// }

// void toggleSetup()
// {
//   pinMode(TOGGLE_PIN, INPUT_PULLUP);
// }