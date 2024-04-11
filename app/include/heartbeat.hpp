#ifndef _HEARTBEAT_HPP_
#define _HEARTBEAT_HPP_

#include "displayManager.hpp"
#include "notifier.hpp"

class Heartbeat {
  public:
    Heartbeat(Notifier* notifier, DisplayManager* displayManager);
    void stop(void);
    void run(void);

  private:
    Notifier* notifier;
    DisplayManager* displayManager;
    std::thread heartbeatThread;
    long long uptime;  // in seconds
    bool isRunning;
};

#endif