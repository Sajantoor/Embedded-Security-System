#pragma once

#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "displayManager.hpp"

class ShutdownHandler {
  private:
    LCD* lcd;
    Keypad* keypad;
    DisplayManager* displayManager;
    bool isRunning = true;

  public:
    ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager);
    void shutdown(void);
    bool isShutdown(void);
};