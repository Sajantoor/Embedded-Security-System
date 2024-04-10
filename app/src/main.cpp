#include <iostream>
#include <string>
#include <vector>

#include "common/utils.hpp"
#include "displayManager.hpp"
#include "hal/buzzer.hpp"
#include "hal/joystick.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/motionSensor.hpp"
#include "hal/relay.hpp"
#include "messageHandler.hpp"
#include "notifier.hpp"
#include "password.hpp"
#include "shutdownHandler.hpp"
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
    Buzzer buzzer;
    ShutdownHandler shutdownHandler(&lcd, &keypad, &displayManager, &buzzer);
    Notifier notifier(&socket);
    MessageHandler messageHandler(&socket, &relay, &password, &displayManager, &shutdownHandler, &notifier);

    // Close the relay at the start of the program
    relay.close();

    // initialize password
    if (!password.doesPasswordExist()) {
        displayManager.displayMessage("Please enter a password", 0, true);
        password.savePassword(keypad.getInput());
        notifier.notify(PASSWORD_SET);
        sleepForMs(1000);
    }

    int failedPasswordAttempts = 0;
    while (!shutdownHandler.isShutdown()) {
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
                notifier.notify(DOOR_OPEN);
                failedPasswordAttempts = 0;
            } else {
                displayManager.displayMessage("Password incorrect", 0, false);
                failedPasswordAttempts++;
                buzzer.buzz();

                if (failedPasswordAttempts >= 3) {
                    std::string message =
                        "Multiple failed password attempts (" + std::to_string(failedPasswordAttempts) + ")";

                    displayManager.displayMessage(message, 0, false);
                    notifier.notify(FAILED_PASSWORD, message);
                }

                // disable at 5 failed password attempts
                if (failedPasswordAttempts >= 5) {
                    displayManager.displayMessage("System disabled for 2 minutes", 0, false);
                    sleepForMs(1000 * 2 * 60);
                }
            }

            sleepForMs(1000);
        }

        // If door is open and joystick pressed down, close the door
        // If joystick is pressed may not work, need to debounce the joystick.
        // Ideally we change this to joystick pressed or button is pressed but yeah.
        if (joystick.isButtonPressed() && relay.isOpen()) {
            relay.close();
            notifier.notify(DOOR_CLOSED);
        }

        // This is temporary for testing, password change
        if (joystick.getDirection() == UP) {
            displayManager.displayMessage("Enter old password", 0, true);
            std::string oldPassword = keypad.getInput();

            displayManager.displayMessage("Enter new password", 0, true);
            std::string newPassword = keypad.getInput();

            if (password.changePassword(oldPassword, newPassword)) {
                displayManager.displayMessage("Password changed", 0, false);
                notifier.notify(PASSWORD_CHANGED);
            } else {
                displayManager.displayMessage("Password incorrect. try again", 0, false);
                notifier.notify(PASSWORD_CHANGE_FAILED, "Incorrect password");
                buzzer.buzz();
            }

            sleepForMs(1000);
        }
    }
    messageHandler.stop();

    return 0;
}
