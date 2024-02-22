#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>

#include "hal/sampler.hpp"
#include "socket.hpp"

class MessageHandler {
   public:
    MessageHandler(Socket* socket, Sampler* sampler);
    ~MessageHandler();
    void init();
    void shutdown();

   private:
    Socket* socket;
    Sampler* sampler;

    void handleUDPMessages();
    void handleStop(UdpMessage* message);
    void handleHelp(UdpMessage* message);
    void handleCount(UdpMessage* message);
    void handleLength(UdpMessage* message);
    void handleDips(UdpMessage* message);
    void handleHistory(UdpMessage* message);
};

#endif