#ifndef __NOTIFIER_HPP_
#define __NOTIFIER_HPP_

#include <ctime>
#include <string>

#include "socket.hpp"

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED,
    FAILED_PASSWORD,
    MOTION_DETECTED,
    PASSWORD_CHANGED,
    PASSWORD_SET,
    PASSWORD_CHANGE_FAILED,
    DISPLAY_MESSAGE_SET,
    DISPLAY_MESSAGE_FAILED,
    HEARTBEAT
} NotificationType;

/**
 * Class used to send notifications to the web server.
*/
class Notifier {
  public:
    Notifier(Socket* socket);

    /**
     * Gets current timestamp in seconds since UNIX epoch.
    */
    std::string getCurrentTimestamp(void);

    /**
     * Sends a notification to the web server.
    */
    void notify(NotificationType type, std::string message = "");

  private:
    Socket* socket = nullptr;
};

#endif