#include <ctime>
#include <string>

#include "socket.hpp"

typedef enum { DOOR_STATUS, FAILED_PASSWORD, MOTION_DETECTED, PASSWORD_CHANGED } NotificationType;

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
    void notify(NotificationType type, std::string message);

  private:
    Socket* socket = nullptr;
};
