/*
Update display when keypad is pressed
Set custom messages on the display like "Door is locked" or any custom message with an optional timeout.
These messages disappear when the keypad is pressed, showing keypad input.
*/

#ifndef _DISPLAY_MANAGER_HPP_
#define _DISPLAY_MANAGER_HPP_

#include <stdbool.h>
#include <atomic>
#include <string>
#include <thread>
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/relay.hpp"

#define DISPLAY_TIME 2000
#define ERROR_DISPLAY_TIME 15000

class DisplayManager {
  private:
    LCD* lcd;
    Keypad* keypad;
    Relay* relay;
    std::string currentMessage = "";
    bool isRunning = true;
    bool wasRequiringInput = false;
    std::thread displayManagerThread;
    std::atomic<long long> currentTimeout = 0;
    std::string messageToClear = "";
    // Display default message based on the state of the relay
    void showDefaultMessage(void);

    void clearDisplayAfterTimeout(unsigned int timeoutInMs);
    void run(void);

  public:
    DisplayManager(LCD* lcd, Keypad* keypad, Relay* relay);

    // Display a message on the LCD screen
    // If timeout is set, the message will be cleared after the timeout
    // If requireKeypadInput is set to true, keypad input will replace the message
    void displayMessage(std::string message, unsigned int timeoutInMs = 0, bool requireKeypadInput = false);
    // Sets isRunning to false
    void stop();

    // Returns the current message on the display
    std::string getCurrentMessage();
};

#endif
