#include "heartbeat.hpp"
#include <string>
#include "common/utils.hpp"

Heartbeat::Heartbeat(Notifier* notifier, DisplayManager* displayManager)
    : notifier(notifier), displayManager(displayManager) {
    uptime = 0;
    isRunning = true;
    heartbeatThread = std::thread(&Heartbeat::run, this);
}

void Heartbeat::stop(void) {
    isRunning = false;
    heartbeatThread.join();
}

void Heartbeat::run(void) {
    while (isRunning) {
        uptime++;
        std::string currentMessage = displayManager->getCurrentMessage();
        // message of format
        // [uptime] [currentMessage]
        std::string message = std::to_string(uptime) + " " + currentMessage;
        notifier->notify(HEARTBEAT, message);
        sleepForMs(1000);
    }
}
