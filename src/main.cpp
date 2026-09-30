#include <Arduino.h>
#include "constants/constants.h"
#include "debugUi.h"
#include "temperature.h"
#include "ui.h"
#include "constants/constants.h"
// #include <OneButton.h>
#include "validate.h"
#include "constants/pin.h"
#include "settings.h"

#define IF_SETTINGS_OPEN_RETURN ({  if (!Settings::getSettingsStatus()) return ; })
typedef unsigned long ul;
void sensorSetup();
// void buttonSetup();
// void toggleSetup();

DebugUI debugUI;
Temperature temp(SENSOR_PIN);
UI ui;
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

  ui.begin();
  ui.initUI();
#if defined(SHOW_START_WINDOW) && !defined(DEBUG)
// #ifdef SHOW_START_WINDOW
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
  temperature = temp.getTemperature();
  res = temp.getRes();
#ifdef DEBUG
  // Serial.println(temp.getVolt());
  Serial.print("Resistance: ");
  Serial.println(res);
  // Serial.println(temperature);
  // debugUI.fprintValue("Volt", temp.getVolt());
  // debugUI.fprintValue("Resistance", temp.getRes());
  debugUI.printValue("ACP", temp.getAcp());
  debugUI.printValue("Temperature", temperature);
#endif
  ui
      .setTemperature(temperature)
      .setAcp(temp.getAcp())
      .setRes(res)
      .draw();

  /// validate
  // int code = val.setTemperature().executePipelineValidate();
  // Settings::setErrorStatus(code);
  // if (code>0) ui.setError(code);

  /// burner
  // if (!systemHalted)
  // {
  //   if (temp.getTemperature() <= Settings::getUserTemp() - Settings::getHysteresis())
  //   {
  //     Settings::setBurnerStatus(true);

  //   }
  // }

  // Settings::setSettingsStatus(digitalRead(TOGGLE_PIN) == HIGH);
}

void sensorSetup()
{
  pinMode(SENSOR_PIN, INPUT);
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