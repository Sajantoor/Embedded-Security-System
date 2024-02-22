#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <thread>

static constexpr const int BUFFER_SIZE = 1024;
static constexpr const int PORT = 12345;

class UdpMessage {
   public:
    UdpMessage(std::string message, std::string ip, unsigned int port) {
        this->message = message;
        this->ip = ip;
        this->port = port;
    }

    std::string getMessage() { return message; }
    std::string getIp() { return ip; }
    unsigned int getPort() { return port; }
    void setMessage(std::string message) { this->message = message; }

   private:
    std::string message;
    std::string ip;
    unsigned int port;
};

class Socket {
   public:
    Socket();
    ~Socket();
    UdpMessage* receive();
    void send(UdpMessage* message);
    bool getIsRecieving();
    void stopRecieving();

   private:
    int socketFd;
    bool isRecieving;
    static Socket* instance;
};

#endif