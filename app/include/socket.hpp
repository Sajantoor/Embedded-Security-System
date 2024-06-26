
#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#include <thread>

static constexpr const int BUFFER_SIZE = 1024;
static constexpr const int PORT = 12345;

static const std::string IP_ADDRESS_STREAMING = "192.168.6.1";
static constexpr int PORT_STREAMING = 1234;

static const std::string IP_ADDRESS_SERVER = "192.168.6.1";
static constexpr int PORT_SERVER = 7070;

/**
 * Represents a message that is sent or recieved from the UDP socket.
 * Contains the message, the ip address of the sender and the port of the
 * sender.
 */
class UdpPacket {
  protected:
    std::string ip;
    unsigned int port;

  public:
    UdpPacket(std::string ip, unsigned int port) : ip(ip), port(port) {}
    std::string getIp(void) { return ip; }
    unsigned int getPort(void) { return port; }
    virtual const void* getData(void) = 0;
    virtual unsigned int getSize(void) = 0;
    virtual ~UdpPacket() {}  //Needed for delete
};

/**
 * Represents a message that is sent or recieved from the UDP socket.
*/
class UdpMessage : public UdpPacket {
  public:
    UdpMessage(std::string message, std::string ip, unsigned int port) : UdpPacket(ip, port), message(message) {}

    const void* getData(void) override { return message.c_str(); }
    std::string getMessage(void) { return message; }
    unsigned int getSize(void) override { return message.size(); }
    void setMessage(std::string message) { this->message = message; }

  private:
    std::string message;
};

/**
 * Represents a stream of data that is sent or recieved from the UDP socket.
*/
class UdpStream : public UdpPacket {
  public:
    UdpStream(const void* data, unsigned int size, std::string ip, unsigned int port)
        : UdpPacket(ip, port), data(data), size(size) {}

    const void* getData(void) override { return data; }
    unsigned int getSize(void) { return size; }

  private:
    const void* data;
    unsigned int size;
};

/*
 Handles the UDP socket implementation
*/
class Socket {
  public:
    Socket(void);
    /**
     * CLoses the socket and stops the recieving thread.
     */
    void closeSocket(void);
    /**
     * Recieves a message from the UDP socket. UdpMessage is allocated using new
     * and must be freed by the caller.
     */
    UdpMessage* receive(void);
    /**
     * Sends a message
     */
    void send(UdpPacket* message);
    /**
     * Returns true if the socket is currently recieving messages.
     */
    bool getIsRecieving(void);
    /**
     * Stops recieving messages from the socket.
     */
    void stopRecieving(void);

    /**
     * Sends a message to the web server
    */
    void sendToWebServer(std::string message);

    void sendDataToWebServer(const void* data, unsigned int size);

  private:
    int socketFd;
    bool isRecieving;
    static Socket* instance;
};

#endif
