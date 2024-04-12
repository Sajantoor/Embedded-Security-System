#ifndef _HEARTBEAT_HPP_
#define _HEARTBEAT_HPP_

#include "displayManager.hpp"
#include "notifier.hpp"

/**
 * Class to send "heartbeat" or "ping" messages to the server, indicating the 
 * device is still running, it's uptime and the current message on the display.
*/
class Heartbeat {
  public:
    /**
     * Create the heartbeat class and start the heartbeat thread
    */
    Heartbeat(Notifier* notifier, DisplayManager* displayManager);
    /**
     * Stop the heartbeat thread and join it
    */
    void stop(void);

  private:
    Notifier* notifier;
    DisplayManager* displayManager;
    std::thread heartbeatThread;
    long long uptime;  // in seconds
    bool isRunning;
    void run(void);
};

#endif