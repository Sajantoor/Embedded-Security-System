#include "shutdownHandler.hpp"
#include "common/utils.hpp"

ShutdownHandler::ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager, Buzzer* buzzer)
    : lcd(lcd), keypad(keypad), displayManager(displayManager), buzzer(buzzer) {}

void ShutdownHandler::shutdown(void) {
    lcd->stop();
    keypad->stop();
    buzzer->stop();
    displayManager->stop();
    isRunning = false;
}

bool ShutdownHandler::isShutdown(void) {
    return !isRunning;
}