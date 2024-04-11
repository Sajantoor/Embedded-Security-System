/*
Provides the shutdown state and shuts down the system.
*/

#ifndef _SHUTDOWN_HANDLER_HPP_
#define _SHUTDOWN_HANDLER_HPP_

#include "displayManager.hpp"
#include "hal/buzzer.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/webcam.hpp"
#include "surveillance.hpp"

class ShutdownHandler {
  private:
    LCD* lcd;
    Keypad* keypad;
    DisplayManager* displayManager;
    Buzzer* buzzer;
    Surveillance* surveillance;
    bool isRunning = true;
    Webcam* webcam;

  public:
    ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager, Buzzer* buzzer,
                    Surveillance* surveillance, Webcam* webcam);
    void shutdown(void);
    bool isShutdown(void);
};

#endif