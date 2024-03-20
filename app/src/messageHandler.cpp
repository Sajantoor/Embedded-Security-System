#include "messageHandler.hpp"

MessageHandler::MessageHandler(Socket* socket) : socket(socket) {}

void MessageHandler::init(void) { handleUDPMessages(); }

void MessageHandler::handleUDPMessages(void) {
    // TODO: This is still unimplemented, once we pick what messages we want to
    // send and receive, we can implement this function.
    while (true) {
        UdpMessage* message = socket->receive();
        std::string messageString = message->getMessage();

        // Remove newline character
        messageString = messageString.substr(0, messageString.find("\n"));
        // split messageString into command and argument
        std::string command = messageString.substr(0, messageString.find(" "));
        std::string argument =
            messageString.substr(messageString.find(" ") + 1);

        if (command == "whatever") {
            // handle the command
        }

        delete message;
    }
}
