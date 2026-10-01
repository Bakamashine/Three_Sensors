#pragma once
#include <stdint.h>

class MenuUI;
class Sensor;

#define DEFAULT_SIZE 24

enum UiFont
{
  FONT_UI,
  FONT_START,
};

class IDisplay
{
private:
  virtual void main();

protected:
  virtual ~IDisplay() = default;

public:
  virtual void clearDisplay() = 0;
  virtual void setFont(UiFont font) = 0;
  virtual void drawStr(int x, int y, const char *str) = 0;
  virtual int getStrWidth(const char *str) = 0;
  virtual int getFontHeight() = 0;

  virtual void initDisplay(int sda = -1, int scl = -1) = 0;
  virtual void initUI() = 0;
  virtual IDisplay &setTemperature(float) = 0;
  virtual void draw() = 0;
  virtual IDisplay &setAcp(int) = 0;
  virtual IDisplay &setRes(float) = 0;
  virtual MenuUI &getMenuUI() = 0;
  virtual void startWindow() = 0;
  virtual IDisplay &setMainSensor(Sensor &) = 0;
  virtual IDisplay &setFirstReserveSensor(Sensor &) = 0;
  virtual IDisplay &setStreetSensor(Sensor &) = 0;
  virtual Sensor &getMainSensor() = 0;
  virtual Sensor &getFirstReserveSensor() = 0;
  virtual IDisplay &setSecondReserveSensor(Sensor &) = 0;
  virtual Sensor &getSecondReserveSensor() = 0;
  virtual Sensor &getStreetSensor() = 0;

// #if UI_BACKEND == UI_BACKEND_U8G2
//   virtual OLED_CLASS &getParent() = 0;
// #endif
  IDisplay &operator=(const IDisplay &) = delete;
};
