#pragma once

#include "hal/keypad.hpp"
#include "hal/lcd.hpp"

class ShutdownHandler {
  private:
    LCD* lcd;
    Keypad* keypad;
    bool isRunning = true;

  public:
    ShutdownHandler(LCD* lcd, Keypad* keypad);
    void shutdown(void);
    void setIsRunning(bool newIsRunning);
    bool getIsRunning(void);
};