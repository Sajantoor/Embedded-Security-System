#include "displayManager.hpp"
#include <string>
#include "common/utils.hpp"

DisplayManager::DisplayManager(LCD* lcd, Keypad* keypad, Relay* relay) : lcd(lcd), keypad(keypad), relay(relay) {
    isRunning = true;
    currentTimeout = 0;
    displayManagerThread = std::thread(&DisplayManager::run, this);
}

void DisplayManager::displayMessage(std::string message, unsigned int timeoutInMs, bool requireKeypadInput) {
    if (message == currentMessage) {
        return;
    }

    if (wasRequiringInput) {
        return;
    }

    lcd->displayToLCD(message);
    currentMessage = message;

    if (timeoutInMs > 0) {
        clearDisplayAfterTimeout(timeoutInMs);
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
    displayManagerThread.join();
}

std::string DisplayManager::getCurrentMessage() {
    return currentMessage;
}

void DisplayManager::clearDisplayAfterTimeout(unsigned int timeoutInMs) {
    currentTimeout.store(timeoutInMs);
    messageToClear = currentMessage;
}

void DisplayManager::run() {
    while (isRunning) {
        if (currentTimeout > 0) {
            while (isRunning && currentTimeout > 0) {
                sleepForMs(1000);
                currentTimeout -= 1000;
            }

            if (isRunning && currentMessage == messageToClear) {
                lcd->clearDisplay();
                showDefaultMessage();
                currentMessage = "";
                messageToClear = "";
            }
        }
    }
}
