#include "notifier.hpp"

Notifier::Notifier(Socket* socket) {
    this->socket = socket;
}

time_t Notifier::getCurrentTimestamp() {
    std::time_t result = std::time(nullptr);
    return result;
}

void Notifier::notify(NotificationType type, std::string message) {
    std::time_t timestamp = getCurrentTimestamp();
    std::string timestampString = std::to_string(timestamp);

    switch (type) {
        case NotificationType::DOOR_STATUS:
            socket->sendToWebServer("doorStatus " + timestampString + " " + message);
            break;
        case NotificationType::FAILED_PASSWORD:
            socket->sendToWebServer("failedPassword " + timestampString + " " + message);
            break;
        case NotificationType::MOTION_DETECTED:
            socket->sendToWebServer("motionDetected " + timestampString);
            break;
        default:
            break;
    }
}
