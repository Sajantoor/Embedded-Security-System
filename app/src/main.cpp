#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/keypad.hpp"
#include "hal/led.hpp"
#include "hal/motionSensor.hpp"
#include "hal/relay.hpp"

#define PASSWORD_FILE "password.txt"

// TODO: Add encryption to the password file, plain text is file for now
void writeToFile(std::string filename, std::string content) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return;
    }

    file << content;
    file.close();
}

// TODO: Add encryption to the password file, plain text is file for now
std::string readFromFile(std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::string content;
    file >> content;
    file.close();
    return content;
}

int main(void) {
    // Init hardware
    Relay relay;
    Keypad keypad(4);

    // lock the relay at the start
    relay.close();

    std::string password = "";

    // look for password file, if not found, create one
    if (std::filesystem::exists(PASSWORD_FILE)) {
        std::cout << "Password file found" << std::endl;
        password = readFromFile(PASSWORD_FILE);
    } else {
        std::cout << "Password file not found, please enter a new password"
                  << std::endl;
        //  TODO: Show input on the LCD display
        keypad.startInput();
        password = keypad.getInput();
        writeToFile(PASSWORD_FILE, password);
    }

    // Input loop: wait for password input and handle it. This should be on
    // another thread.
    while (true) {
        std::cout << "Enter password: " << std::endl;

        // TODO: Show input on the LCD display
        keypad.startInput();
        std::string input = keypad.getInput();

        if (input == password) {
            std::cout << "Password correct, unlocking" << std::endl;
            relay.open();
            // TODO: Add a trigger to close the door...

        } else {
            std::cout << "Password incorrect, try again" << std::endl;

            if (relay.isOpen()) {
                relay.close();
            }

            // TODO: Could use BBG buzzer here to indicate incorrect password...

            /**
             * TODO: This could be done when multiple incorrect password
             * attempts are made, or just after 1:
             *
             * We send a notification with the date and time of the attempt to
             * the owner or owners using the web server (UDP message to web
             * server).
             *
             * We also send webcam photo of the last 10-30 seconds.
             */
        }
    }

    // This is the main application loop:
    while (true) {
        /**
         * Send the current state of the door and the last time it was
            opened show any notifications that have been sent show live feed
        from webcam


        * Check motion sensor for any movement and start recording if there
            is any, until there is no movement.


        * Check for any messages from the web server, ie lock or unlock door
            events and act accordingly. Check for change password events

        * Show a message on the LCD? "Door is locked" or "Door is unlocked", etc
        */
    }

    return 0;
}
