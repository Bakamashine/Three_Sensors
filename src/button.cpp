#include "button.h"
#if BTN_BACKEND == BTN_BACKEND_CUSTOM
#include "constants/constants.h"
#include "macro/general.h"
#include "macro/shortcut.h"
#include <Arduino.h>

Button::Button (uint8_t pin) : _pin (pin) {}
Button::Button () {}

Button &
Button::setup (uint8_t pin, int mode, bool activeLow)
{
  _pin = pin;
  _mode = mode;
  _activeLow = activeLow;
  pinMode (_pin, _mode);
  _pressed = false;
  _longFired = false;
  return *this;
}

Button &
Button::attachClick (void (*fn) ())
{
  _onePressHandle = fn;
  return *this;
}

Button &
Button::attachLongPressStart (void (*fn) ())
{
  _longPressHandle = fn;
  return *this;
}

Button &
Button::setDebounceMs (ul ms)
{
  _debounceMs = ms;
  return *this;
}

Button &
Button::setLongPressTime (ul ms)
{
  _longPressTime = ms;
  return *this;
}

bool
Button::isActive (int raw) const
{
  return raw == (_activeLow ? LOW : HIGH);
}

void
Button::tick ()
{
  const ul now = millis ();
  const int raw = digitalRead (_pin);
  const bool active = isActive (raw);

  // 1) debounce the raw line
  if (raw != _lastRaw)
    {
      _lastRaw = raw;
      _lastChangeMs = now;
    }
  const bool stable = (now - _lastChangeMs) >= _debounceMs;

  if (!stable)
    return;

  // 2) rising edge: the button started to register as pressed
  if (active && !_pressed)
    {
      _pressed = true;
      _longFired = false;
      _pressStartMs = now;
    }

  // 3) falling edge: the button was released
  if (!active && _pressed)
    {
      _pressed = false;
      if (_onePressHandle != nullptr && !_longFired)
        _onePressHandle ();
    }

  // 4) held long enough -> long press fires once
  if (_pressed && !_longFired && (now - _pressStartMs) >= _longPressTime)
    {
      _longFired = true;
      if (_longPressHandle != nullptr)
        _longPressHandle ();
    }
}

#endif