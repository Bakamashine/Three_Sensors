#pragma once
#include "constants/constants.h"
#include "macro/general.h"
#include "macro/shortcut.h"
#include <Arduino.h>

class Button
{
private:
  uint8_t _pin = 0;
  ul _debounceMs = DEBOUNCE_DURATION;
  int _mode = INPUT_PULLUP;
  bool _activeLow = true;
  ul _longPressTime = 500; // ms
  int _lastRaw = HIGH;
  ul _lastChangeMs = 0;
  bool _pressed = false;
  bool _longFired = false;
  ul _pressStartMs = 0;
  void (*_longPressHandle) () = nullptr;
  void (*_onePressHandle) () = nullptr;
  bool isActive (int raw) const;

public:
  Button (uint8_t pin);
  Button ();
  Button &setDebounceMs (ul ms);
  Button &setLongPressTime (ul ms);
  Button &setup (uint8_t pin, int mode, bool activeLow);
  Button &attachClick (void (*fn) ());
  Button &attachLongPressStart (void (*fn) ());
  void tick ();
};