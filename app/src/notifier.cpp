#include "notifier.hpp"
#include <optional>

Notifier::Notifier(Socket* socket) : socket(socket) {}

std::string Notifier::getCurrentTimestamp(void) {
    std::time_t timestamp = std::time(nullptr);
    std::string timestampString = std::to_string(timestamp);
    return timestampString;
}

void Notifier::notify(NotificationType type, std::string message) {
    std::string timestamp = getCurrentTimestamp();

    switch (type) {
        case DOOR_OPEN:
            socket->sendToWebServer("doorOpen " + timestamp);
            break;
        case DOOR_CLOSED:
            socket->sendToWebServer("doorClosed " + timestamp);
            break;
        case FAILED_PASSWORD:
            socket->sendToWebServer("failedPassword " + timestamp + " " + message);
            break;
        case MOTION_DETECTED:
            socket->sendToWebServer("motionDetected " + timestamp);
            break;
        case PASSWORD_CHANGED:
            socket->sendToWebServer("passwordChanged " + timestamp);
            break;
        case PASSWORD_SET:
            socket->sendToWebServer("passwordSet " + timestamp);
            break;
        case PASSWORD_CHANGE_FAILED:
            socket->sendToWebServer("passwordChangeFailed " + timestamp);
            break;
        case DISPLAY_MESSAGE_SET:
            socket->sendToWebServer("displayMessageSet " + timestamp);
            break;
        case DISPLAY_MESSAGE_FAILED:
            socket->sendToWebServer("displayMessageFailed " + timestamp + " " + message);
            break;
        default:
            break;
    }
}
