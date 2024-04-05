#include "messageHandler.hpp"

MessageHandler::MessageHandler(Socket* socket, Relay* relay, Password* password, DisplayManager* displayManager)
    : socket(socket), relay(relay), password(password), displayManager(displayManager) {}

void MessageHandler::init(void) {
    isRunning = true;
    handleUDPMessages();
}

void MessageHandler::handleLock(void) {
    relay->close();
}

void MessageHandler::handleUnlock(void) {
    relay->open();
}

void MessageHandler::handleChangePassword(std::vector<std::string> arguments) {
    if (arguments.size() != 2) {
        socket->sendToWebServer("Invalid number of arguments\n");
        return;
    }

    const std::string currentPassword = arguments[0];
    const std::string newPassword = arguments[1];

    if (password->changePassword(currentPassword, newPassword)) {
        socket->sendToWebServer("Password changed successfully\n");
    } else {
        socket->sendToWebServer("Invalid current password\n");
    }
}

void MessageHandler::handleSetDisplayMessage(std::vector<std::string> arguments) {
    if (arguments.size() < 1) {
        socket->sendToWebServer("Invalid number of arguments\n");
        return;
    }

    std::string message = arguments[0];
    unsigned int timeout = 0;

    if (arguments.size() == 2) {
        // convert seconds to milliseconds
        timeout = std::stoi(arguments[1]) * 1000;

        if (timeout < 0) {
            socket->sendToWebServer("Invalid timeout\n");
            return;
        }
    }

    displayManager->displayMessage(message, timeout);
}

void MessageHandler::handleShutdown(void) {
    // TODO: Implement shutdown
}

void MessageHandler::handleUDPMessages(void) {
    messageHandlerThread = std::thread([this] {
        while (isRunning) {
            UdpMessage* message = socket->receive();
            std::string messageString = message->getMessage();

            // Remove newline character
            messageString = messageString.substr(0, messageString.find("\n"));
            // split messageString into command and argument
            std::string command = messageString.substr(0, messageString.find(" "));
            std::vector<std::string> arguments = {};

            if (messageString.find(" ") != std::string::npos) {
                std::string argument = messageString.substr(messageString.find(" ") + 1);
                arguments.push_back(argument);
            }

            /**
             * commands:
             *
             * lock
             * unlock
             * changePassword [currentPassword] [newPassword]
             * setDisplayMessage [message] [timeout (in seconds) optional]
             * shutdown
             *
             */
            if (command == "lock") {
                handleLock();
            } else if (command == "unlock") {
                handleUnlock();
            } else if (command == "changePassword") {
                handleChangePassword(arguments);
            } else if (command == "setDisplayMessage") {
                handleSetDisplayMessage(arguments);
            } else if (command == "shutdown") {
                handleShutdown();
            } else {
                socket->sendToWebServer("Invalid command\n");
            }

            delete message;
        }
    });
}

void MessageHandler::stop(void) {
    isRunning = false;
    messageHandlerThread.join();
}
