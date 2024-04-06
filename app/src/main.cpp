#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "displayManager.hpp"
#include "hal/joystick.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/led.hpp"
#include "hal/motionSensor.hpp"
#include "hal/relay.hpp"
#include "messageHandler.hpp"
#include "password.hpp"
#include "socket.hpp"

int main(void) {
    // Init hardware
    Relay relay;
    Keypad keypad(4);
    JoyStick joystick;
    LCD lcd;
    DisplayManager displayManager(lcd, keypad);
    Password password;
    Socket socket;
    MessageHandler messageHandler(&socket, &relay, &password, &displayManager);

    // Close the relay at the start of the program
    relay.close();

    // initialize password
    if (!password.doesPasswordExist()) {
        displayManager.displayMessage("Please enter a password", 0, true);
        password.savePassword(keypad.getInput());
        sleepForMs(1000);
    }

    // TODO: use shutdown module for this instead
    bool isStopped = false;

    while (!isStopped) {
        if (relay.isOpen()) {
            displayManager.displayMessage("Door is open", 0, false);
        } else {
            displayManager.displayMessage("Door is closed", 0, false);
        }

        // if joystick is pressed and door is closed, enter the password.
        if (joystick.isButtonPressed() && !relay.isOpen()) {
            displayManager.displayMessage("Enter password", 0, true);
            std::string input = keypad.getInput();

            if (password.isPasswordCorrect(input)) {
                displayManager.displayMessage("Password correct", 0, false);
                relay.open();
            } else {
                displayManager.displayMessage("Password incorrect", 0, false);
                relay.close();
            }

            sleepForMs(1000);
        }

        // If door is open and joystick pressed down, close the door
        // If joystick is pressed may not work, need to debounce the joystick.
        // Ideally we change this to joystick pressed or button is pressed but yeah.
        if (joystick.getDirection() == DOWN && relay.isOpen()) {
            relay.close();
        }

        // This is temporary for testing, password change
        if (joystick.getDirection() == UP) {
            displayManager.displayMessage("Enter old password", 0, true);
            std::string oldPassword = keypad.getInput();

            displayManager.displayMessage("Enter new password", 0, true);
            std::string newPassword = keypad.getInput();

            if (password.changePassword(oldPassword, newPassword)) {
                displayManager.displayMessage("Password changed", 0, false);
            } else {
                displayManager.displayMessage("Password incorrect. try again", 0, false);
            }

            sleepForMs(1000);
        }
    }

    return 0;
}
