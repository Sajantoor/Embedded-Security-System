#include "displayManager.hpp"
#include <string>
#include <thread>
#include "common/utils.hpp"

DisplayManager::DisplayManager(LCD* lcd, Keypad* keypad, Relay* relay) : lcd(lcd), keypad(keypad), relay(relay) {}

void DisplayManager::displayMessage(std::string message, unsigned int timeoutInMs, bool requireKeypadInput) {
    if (message == prevMsg) {
        return;
    }

    if (wasRequiringInput) {
        return;
    }

    lcd->displayToLCD(message);
    prevMsg = message;

    if (timeoutInMs > 0) {
        std::thread([this, timeoutInMs] {
            sleepForMs(timeoutInMs);
            lcd->clearDisplay();
            wasRequiringInput = false;
            showDefaultMessage();
        }).detach();
    } else if (requireKeypadInput) {
        wasRequiringInput = true;
        keypad->startInput();
        std::string prevInput = "";

        while (isRunning) {
            std::string input = keypad->getInput();
            if (input != prevInput) {
                lcd->displayToLCD(input);
                prevInput = input;
            } else if (keypad->isInputComplete()) {
                std::string completeInput = keypad->getInput();
                lcd->displayToLCD(completeInput);
                wasRequiringInput = false;
                break;
            }
        }
    }
}

void DisplayManager::showDefaultMessage() {
    if (relay->isOpen()) {
        displayMessage("Door is open", 0, false);
    } else {
        displayMessage("Door is closed", 0, false);
    }
}

void DisplayManager::stop() {
    isRunning = false;
}
