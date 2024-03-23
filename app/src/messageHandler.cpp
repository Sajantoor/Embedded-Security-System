#include "messageHandler.hpp"

MessageHandler::MessageHandler(Socket* socket) : socket(socket) {
    isRunning = true;
    handleUDPMessages();
}

void MessageHandler::handleUDPMessages(void) {
    // TODO: This is still unimplemented, once we pick what messages we want to
    // send and receive, we can implement this function.
    messageHandlerThread = std::thread([this] {
        while (isRunning) {
            UdpMessage* message = socket->receive();
            std::string messageString = message->getMessage();

            // Remove newline character
            messageString = messageString.substr(0, messageString.find("\n"));
            std::string command =
                messageString.substr(0, messageString.find(" "));
            std::string argument =
                messageString.substr(messageString.find(" ") + 1);

            /**
             * commands:
             *
             * lock
             * unlock
             * changePassword [password]
             * setDisplayMessage [message] [timeout (in seconds) optional]
             * shutdown
             *
             * (once timeout is reached, goes back to default message which
             * shows the status of the door)
             */
            if (command == "lock") {
                // handleLock();
            } else if (command == "unlock") {
                // handleUnlock();
            } else if (command == "changePassword") {
                // handleChangePassword(argument);
            } else if (command == "setDisplayMessage") {
                // handleSetDisplayMessage(argument);
            } else if (command == "shutdown") {
                // handleShutdown();
            } else {
                std::cout << "Invalid command" << std::endl;
            }

            delete message;
        }
    });
}

void MessageHandler::stop(void) {
    isRunning = false;
    messageHandlerThread.join();
}
