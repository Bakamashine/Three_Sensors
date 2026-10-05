#pragma once
#include <Arduino.h>
template <typename T>
T
getAvarageValue (T *array, size_t size)
{
  if (size == 0)
    return 0;
  T sum = 0;
  for (size_t i = 0; i < size; i++)
    sum += array[i];
  // round to nearest, not toward zero: plain sum / size biases the result
  // downwards, so 20 / 20 / 21 would read 20 instead of 20.33
  T n = static_cast<T> (size);
  T half = static_cast<T> (size / 2);
  if (sum >= 0)
    return static_cast<T> ((sum + half) / n);
  return static_cast<T> ((sum - half) / n);
}