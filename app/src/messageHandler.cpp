#include "messageHandler.hpp"

MessageHandler::MessageHandler(Socket* socket, Relay* relay, Password* password, DisplayManager* displayManager,
                               ShutdownHandler* shutdownHandler, Notifier* notifier)
    : socket(socket),
      relay(relay),
      password(password),
      displayManager(displayManager),
      shutdownHandler(shutdownHandler),
      notifier(notifier) {

    isRunning = true;
    handleUDPMessages();
}

void MessageHandler::handleLock(void) {
    notifier->notify(DOOR_CLOSED);
    relay->close();
}

void MessageHandler::handleUnlock(void) {
    notifier->notify(DOOR_OPEN);
    relay->open();
}

// Takes in the current password and the new password, and updates the password if the current password is correct
void MessageHandler::handleChangePassword(std::vector<std::string> arguments) {
    if (arguments.size() != 2) {
        notifier->notify(FAILED_PASSWORD, "Invalid number of arguments");
        return;
    }

    const std::string currentPassword = arguments[0];
    const std::string newPassword = arguments[1];

    if (password->changePassword(currentPassword, newPassword)) {
        notifier->notify(PASSWORD_CHANGED);
    } else {
        notifier->notify(FAILED_PASSWORD, "Incorrect password");
    }
}

// Sets the display message to the given message for the given timeout
void MessageHandler::handleSetDisplayMessage(std::vector<std::string> arguments) {
    if (arguments.size() < 1) {
        notifier->notify(DISPLAY_MESSAGE_FAILED, "Invalid number of arguments");
        return;
    }

    std::string message = arguments[0];
    unsigned int timeout = 0;

    if (arguments.size() == 2) {
        // convert seconds to milliseconds
        timeout = std::stoi(arguments[1]) * 1000;
    }

    displayManager->displayMessage(message, timeout);
    notifier->notify(DISPLAY_MESSAGE_SET);
}

void MessageHandler::handleShutdown(void) {
    isRunning = false;
    shutdownHandler->shutdown();
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
    messageHandlerThread.join();
}
