#pragma once

#include "displayManager.hpp"
#include "hal/buzzer.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"

class ShutdownHandler {
  private:
    LCD* lcd;
    Keypad* keypad;
    DisplayManager* displayManager;
    Buzzer* buzzer;
    bool isRunning = true;

  public:
    ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager, Buzzer* buzzer);
    void shutdown(void);
    bool isShutdown(void);
};