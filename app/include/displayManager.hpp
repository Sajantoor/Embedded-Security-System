/*
Update display when keypad is pressed
Set custom messages on the display like "Door is locked" or any custom message with an optional timeout.
These messages disappear when the keypad is pressed, showing keypad input.
*/

#ifndef _DISPLAY_MANAGER_HPP_
#define _DISPLAY_MANAGER_HPP_

#include <stdbool.h>
#include <string>
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"

class DisplayManager {
  private:
    LCD& lcd;
    Keypad& keypad;
    std::string prevMsg = "";

  public:
    DisplayManager(LCD& lcd, Keypad& keypad);

    // Display a message on the LCD screen
    // If timeout is set, the message will be cleared after the timeout
    // If requireKeypadInput is set to true, keypad input will replace the message
    void displayMessage(std::string message, unsigned int timeoutInMs = 0, bool requireKeypadInput = false);
};

#endif
