#ifndef _MESSAGE_HANDLER_HPP_
#define _MESSAGE_HANDLER_HPP_

#include <vector>
#include "displayManager.hpp"
#include "hal/relay.hpp"
#include "password.hpp"
#include "socket.hpp"
#include "shutdownHandler.hpp"

/**
 Responsible for handling the messages received from the
 UDP socket and calling the appropriate functions in the Sampler class.
 Also responsible for calling the shutdown funciton.
*/
class MessageHandler {
  public:
    /**
     * Starts the message handler on a new thread. This function will only shut
     * down once it receives a stop message from the UDP socket.
     */
    MessageHandler(Socket* socket, Relay* relay, Password* password, DisplayManager* displayManager, ShutdownHandler* ShutdownHandler);
    
    /**
     * Stops the message handler.
     */
    void stop(void);

  private:
    Socket* socket;
    Relay* relay;
    Password* password;
    DisplayManager* displayManager;
    ShutdownHandler* shutdownHandler;
    std::thread messageHandlerThread;
    bool isRunning;

    void handleUDPMessages(void);
    void handleShutdown(void);
    void handleLock(void);
    void handleUnlock(void);
    void handleChangePassword(std::vector<std::string> arguments);
    void handleSetDisplayMessage(std::vector<std::string> arguments);
};

#endif
