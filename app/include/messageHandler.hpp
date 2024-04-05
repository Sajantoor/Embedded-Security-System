#ifndef _MESSAGE_HANDLER_HPP_
#define _MESSAGE_HANDLER_HPP_

#include "socket.hpp"

/**
 Responsible for handling the messages received from the
 UDP socket and calling the appropriate functions in the Sampler class.
 Also responsible for calling the shutdown funciton.
*/
class MessageHandler {
  public:
    MessageHandler(Socket* socket);
    /**
     * Starts the message handler on a new thread. This function will only shut
     * down once it receives a stop message from the UDP socket.
     */
    void init(void);
    /**
     * Stops the message handler.
     */
    void stop(void);

  private:
    Socket* socket;
    std::thread messageHandlerThread;
    bool isRunning;
    void handleUDPMessages(void);
};

#endif
