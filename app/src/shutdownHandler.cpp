#include "shutdownHandler.hpp"
#include "common/utils.hpp"

ShutdownHandler::ShutdownHandler(LCD* lcd, Keypad* keypad)
    : lcd(lcd), keypad(keypad) {
}

void ShutdownHandler::shutdown(void) {
    lcd->stop();
    keypad->stop();
}

void ShutdownHandler::setIsRunning(bool newIsRunning) {
    isRunning = newIsRunning;
}
bool ShutdownHandler::getIsRunning(void) {
    return isRunning;
}