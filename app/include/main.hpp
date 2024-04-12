#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "displayManager.hpp"
#include "hal/buzzer.hpp"
#include "hal/keypad.hpp"
#include "notifier.hpp"
#include "password.hpp"
#include "shutdownHandler.hpp"

void initializePassword(Password* password, DisplayManager* displayManager, Keypad* keypad, Notifier* notifier);
void handlePasswordChange(DisplayManager* displayManager, Keypad* keypad, Password* password, Notifier* notifier,
                          Buzzer* buzzer);
void disableSystemMessage(DisplayManager* displayManager, ShutdownHandler* shutdownHandler);

#endif