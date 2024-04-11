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
#include "hal/webcam.hpp"
#include "heartbeat.hpp"
#include "main.hpp"
#include "messageHandler.hpp"
#include "notifier.hpp"
#include "password.hpp"
#include "shutdownHandler.hpp"
#include "socket.hpp"
#include "surveillance.hpp"

int main(void) {
    std::cout << "Initializing hardware" << std::endl;

    Relay relay;
    Keypad keypad(4);
    JoyStick joystick;
    LCD lcd;
    DisplayManager displayManager(&lcd, &keypad, &relay);
    Password password;
    Socket socket;
    Buzzer buzzer;
    Notifier notifier(&socket);
    MotionSensor motionSensor;
    Surveillance surveillance(&motionSensor, &notifier);
    Heartbeat heartbeat(&notifier, &displayManager);
    ShutdownHandler shutdownHandler(&lcd, &keypad, &displayManager, &buzzer, &surveillance, &heartbeat);
    MessageHandler messageHandler(&socket, &relay, &password, &displayManager, &shutdownHandler, &notifier);

    std::cout << "Initialization finished, starting program" << std::endl;
    startStream();

    // Close the relay and start stream at the start of the program
    relay.close();
    displayManager.displayMessage("Starting program...", DISPLAY_TIME, false);

    // initialize password
    initializePassword(&password, &displayManager, &keypad, &notifier);

    int failedPasswordAttempts = 0;

    while (!shutdownHandler.isShutdown()) {
        // if joystick is pressed and door is closed, enter the password.
        if (joystick.isButtonPressed() && !relay.isOpen()) {
            displayManager.displayMessage("Enter password", 0, true);
            std::string input = keypad.getInput();

            if (password.isPasswordCorrect(input)) {
                displayManager.displayMessage("Password correct", DISPLAY_TIME, false);
                relay.open();
                notifier.notify(DOOR_OPEN);
                failedPasswordAttempts = 0;
            } else {
                displayManager.displayMessage("Password incorrect", DISPLAY_TIME, false);
                failedPasswordAttempts++;
                buzzer.buzz();

                if (failedPasswordAttempts >= 3) {
                    std::string message = "Multiple failed attempts (" + std::to_string(failedPasswordAttempts) + ")";

                    displayManager.displayMessage(message, ERROR_DISPLAY_TIME, false);
                    notifier.notify(FAILED_PASSWORD, message);
                }

                // disable at 5 failed password attempts
                if (failedPasswordAttempts >= 5) {
                    displayManager.displayMessage("System disabled for 2 minutes", 1000 * 2 * 60, false);
                    sleepWhileCheckingConditon([&] { return !shutdownHandler.isShutdown(); }, 1000 * 2 * 60);
                }
            }
        }

        // If door is open and joystick pressed down, close the door
        else if (joystick.isButtonPressed() && relay.isOpen()) {
            displayManager.displayMessage("Closing door...", DISPLAY_TIME, false);
            relay.close();
            notifier.notify(DOOR_CLOSED);
            sleepForMs(1000);
        }

        // Handle password change
        else if (joystick.getDirection() == UP) {
            handlePasswordChange(&displayManager, &keypad, &password, &notifier, &buzzer);
        }
    }

    messageHandler.stop();
    return 0;
}

void handlePasswordChange(DisplayManager* displayManager, Keypad* keypad, Password* password, Notifier* notifier,
                          Buzzer* buzzer) {
    displayManager->displayMessage("Enter old password", 0, true);
    std::string oldPassword = keypad->getInput();

    displayManager->displayMessage("Enter new password", 0, true);
    std::string newPassword = keypad->getInput();

    if (password->changePassword(oldPassword, newPassword)) {
        displayManager->displayMessage("Password changed", DISPLAY_TIME, false);
        notifier->notify(PASSWORD_CHANGED);
    } else {
        displayManager->displayMessage("Password incorrect. try again", ERROR_DISPLAY_TIME, false);
        notifier->notify(PASSWORD_CHANGE_FAILED, "Incorrect password");
        buzzer->buzz();
    }

    sleepForMs(1000);
}

void initializePassword(Password* password, DisplayManager* displayManager, Keypad* keypad, Notifier* notifier) {
    if (!password->doesPasswordExist()) {
        displayManager->displayMessage("Please enter a password", 0, true);
        password->savePassword(keypad->getInput());
        notifier->notify(PASSWORD_SET);
        sleepForMs(1000);
    }
}
