#pragma once
#include <Arduino.h>

/// AVR's toolchain ships no <type_traits>, so the one trait needed here is
/// declared locally: only float needs to take the "return the exact mean"
/// path.
template <typename T> struct IsFloat
{
  static const bool value = false;
};

template <> struct IsFloat<float>
{
  static const bool value = true;
};

/// Arithmetic mean of array[0..size), rounded to the nearest integer for
/// integer T. For float the exact mean is returned instead: adding a
/// half-divisor is an integer-rounding trick and would corrupt the value.
template <typename T>
T
getAvarageValue (T *array, size_t size)
{
  if (size == 0)
    return 0;

  T sum = 0;
  for (size_t i = 0; i < size; i++)
    sum += array[i];

  T n = static_cast<T> (size);

  if (IsFloat<T>::value)
    return sum / n;

  // round to nearest, not toward zero: plain sum / size biases the result
  // downwards, so 20 / 20 / 21 would read 20 instead of 20.33
  T half = static_cast<T> (size / 2);
  if (sum >= 0)
    return static_cast<T> ((sum + half) / n);
  return static_cast<T> ((sum - half) / n);
}