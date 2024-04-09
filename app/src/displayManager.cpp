#include "displayManager.hpp"
#include <string>
#include <thread>
#include "common/utils.hpp"

DisplayManager::DisplayManager(LCD& lcd, Keypad& keypad) : lcd(lcd), keypad(keypad) {}

void DisplayManager::displayMessage(std::string message, unsigned int timeoutInMs, bool requireKeypadInput) {
    if (message == prevMsg) { return; }

    lcd.displayToLCD(message);
    prevMsg = message;

    if (timeoutInMs > 0) {
        std::thread([this, timeoutInMs] {
            sleepForMs(timeoutInMs);
            lcd.clearDisplay();
        }).detach();
    } else if (requireKeypadInput) {
        keypad.startInput();
        std::string prevInput = "";

        while (isRunning) {
            std::string input = keypad.getInput();
            if (input != prevInput) {
                lcd.displayToLCD(input);
                prevInput = input;
            } else if (keypad.isInputComplete()) {
                std::string completeInput = keypad.getInput();
                lcd.displayToLCD(completeInput);
                break;
            }
        }
    }
}

void DisplayManager::stop() {
    isRunning = false;
}
