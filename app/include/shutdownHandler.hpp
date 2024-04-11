/*
Provides the shutdown state and shuts down the system.
*/

#ifndef _SHUTDOWN_HANDLER_HPP_
#define _SHUTDOWN_HANDLER_HPP_

#include "displayManager.hpp"
#include "hal/buzzer.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "heartbeat.hpp"
#include "surveillance.hpp"

class ShutdownHandler {
  private:
    LCD* lcd;
    Keypad* keypad;
    DisplayManager* displayManager;
    Buzzer* buzzer;
    Surveillance* surveillance;
    Heartbeat* heartbeat;
    bool isRunning = true;

  public:
    ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager, Buzzer* buzzer,
                    Surveillance* surveillance, Heartbeat* heartbeat);
    void shutdown(void);
    bool isShutdown(void);
};

#endif