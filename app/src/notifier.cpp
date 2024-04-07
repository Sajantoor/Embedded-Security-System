#include "notifier.hpp"

Notifier::Notifier(Socket* socket) {
    this->socket = socket;
}

std::string Notifier::getCurrentTimestamp(void) {
    std::time_t timestamp = std::time(nullptr);
    std::string timestampString = std::to_string(timestamp);
    return timestampString;
}

void Notifier::notify(NotificationType type, std::string message) {
    std::string timestamp = getCurrentTimestamp();

    switch (type) {
        case NotificationType::DOOR_STATUS:
            socket->sendToWebServer("doorStatus " + timestamp + " " + message);
            break;
        case NotificationType::FAILED_PASSWORD:
            socket->sendToWebServer("failedPassword " + timestamp + " " + message);
            break;
        case NotificationType::MOTION_DETECTED:
            socket->sendToWebServer("motionDetected " + timestamp);
            break;
        case NotificationType::PASSWORD_CHANGED:
            socket->sendToWebServer("passwordChanged " + timestamp + " " + message);
            break;
        default:
            break;
    }
}
