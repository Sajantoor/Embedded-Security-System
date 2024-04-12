#include "shutdownHandler.hpp"
#include "common/utils.hpp"
#include "hal/webcam.hpp"

ShutdownHandler::ShutdownHandler(LCD* lcd, Keypad* keypad, DisplayManager* displayManager, Buzzer* buzzer,
                                 Surveillance* surveillance, Heartbeat* heartbeat)
    : lcd(lcd),
      keypad(keypad),
      displayManager(displayManager),
      buzzer(buzzer),
      surveillance(surveillance),
      heartbeat(heartbeat) {}

void ShutdownHandler::shutdown(void) {
    lcd->stop();
    keypad->stop();
    buzzer->stop();
    displayManager->stop();
    surveillance->stop();
    heartbeat->stop();
    stopStream();
    isRunning = false;
}

bool ShutdownHandler::isShutdown(void) {
    return !isRunning;
}

bool ShutdownHandler::isSystemDisabled(void) {
    return isDisabled;
}

void ShutdownHandler::disableSystem(void) {
    isDisabled = true;
}

void ShutdownHandler::enableSystem(void) {
    isDisabled = false;
}
