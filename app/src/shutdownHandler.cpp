#include "shutdownHandler.hpp"
#include "common/utils.hpp"

ShutdownHandler::ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager)
    : lcd(lcd), keypad(keypad), displayManager(displayManager)  {
}

void ShutdownHandler::shutdown(void) {
    lcd->stop();
    keypad->stop();
    displayManager->stop();
    isRunning = false;
}

bool ShutdownHandler::isShutdown(void) {
    return !isRunning;
}