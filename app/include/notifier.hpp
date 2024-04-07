#include <ctime>
#include <string>

#include "socket.hpp"

enum class NotificationType { DOOR_STATUS, FAILED_PASSWORD, MOTION_DETECTED };

class Notifier {
  public:
    Notifier(Socket* socket);

    // get current timestamp
    time_t getCurrentTimestamp(void);

    void notify(NotificationType type, std::string message);

  private:
    Socket* socket = nullptr;
};
