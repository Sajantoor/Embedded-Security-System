#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "displayManager.hpp"
#include "hal/buzzer.hpp"
#include "hal/keypad.hpp"
#include "notifier.hpp"
#include "password.hpp"

void initializePassword(Password* password, DisplayManager* displayManager, Keypad* keypad, Notifier* notifier);
void handlePasswordChange(DisplayManager* displayManager, Keypad* keypad, Password* password, Notifier* notifier,
                          Buzzer* buzzer);

#endif