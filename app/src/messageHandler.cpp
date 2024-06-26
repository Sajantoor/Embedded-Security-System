#include "messageHandler.hpp"
#include <sstream>

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
    displayManager->displayMessage("Locking door...", DISPLAY_TIME, false);
    notifier->notify(DOOR_CLOSED);
    relay->close();
}

void MessageHandler::handleUnlock(void) {
    displayManager->displayMessage("Opening door...", DISPLAY_TIME, false);
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
        displayManager->displayMessage("Password changed", DISPLAY_TIME, false);
        notifier->notify(PASSWORD_CHANGED);
    } else {
        notifier->notify(FAILED_PASSWORD, "Incorrect password");
    }
}

// Sets the display message to the given message for the given timeout
void MessageHandler::handleSetDisplayMessage(std::vector<std::string> arguments) {
    if (arguments.size() < 2) {
        notifier->notify(DISPLAY_MESSAGE_FAILED, "Invalid number of arguments");
        return;
    }

    unsigned int timeout = std::stoi(arguments[0]) * 1000;
    std::string message = arguments[1];

    displayManager->displayMessage(message, timeout);
    notifier->notify(DISPLAY_MESSAGE_SET);
}

void MessageHandler::handleShutdown(void) {
    displayManager->displayMessage("Shutting down...", 0, false);
    isRunning = false;
    shutdownHandler->shutdown();
}

std::vector<std::string> splitString(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

void MessageHandler::handleUDPMessages(void) {
    messageHandlerThread = std::thread([this] {
        while (isRunning) {
            UdpMessage* message = socket->receive();
            std::string messageString = message->getMessage();

            // Ignore all messages while disabled
            if (shutdownHandler->isSystemDisabled()) {
                delete message;
                continue;
            }

            std::vector<std::string> tokens = splitString(messageString);
            std::string command = tokens[0];
            std::vector<std::string> arguments = {};

            for (unsigned int i = 1; i < tokens.size(); i++) {
                if (i > 2) {
                    arguments[1] += " " + tokens[i];
                }

                arguments.push_back(tokens[i]);
            }

            /**
             * commands:
             *
             * lock
             * unlock
             * changePassword [currentPassword] [newPassword]
             * setDisplayMessage [timeout (in seconds) optional] [message]
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
