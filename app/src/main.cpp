#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/led.hpp"
#include "hal/relay.hpp"
#include "password.hpp"
#include "hal/webcam.hpp"

void testRelay();
void testKeypad(Password& password);
void testLCD();
void testWebcam();

int main(void) {
    Password password;
    std::cout << "Hello, World!" << std::endl;
    // testRelay();
    // testLCD();
    // testKeypad(password);
    return 0;
}

void testRelay() {
    Relay relay;
    sleepForMs(1000);

    // Closing lock
    relay.open();
    sleepForMs(1000);

    // Opening lock
    relay.close();
    sleepForMs(500);
}

void testKeypad(Password& password) {
    Keypad keypad(4);
    keypad.startInput();
    const std::string input = keypad.getInput();
    std::cout << "You entered: " << input << std::endl;
    password.savePassword(input);
    std::cout << "Password saved. Let's now change the password." << std::endl;
    std::cout << "Enter current password." << std::endl;
    keypad.startInput();
    const std::string oldPassword = keypad.getInput();
    std::cout << "Enter new password." << std::endl;
    keypad.startInput();
    const std::string newPassword = keypad.getInput();
    std::cout << "New password entered: " << newPassword << std::endl;
    if (password.changePassword(oldPassword, newPassword)) {
        std::cout << "Password changed" << std::endl;
    }
    else {
        std::cout << "Password not changed" << std::endl;
    }
    password.removePassword();
}

void testLCD() {
    LCD lcd = LCD();

    // first 16 chars shown ("My ")
    // 24 chars not shown
    // then 16 shown ("working")
    // final 19 chars not displayed
    // lcd.displayMessage("Hello World!!! My Name is Jimmy.");

    lcd.clearDisplay();
    sleepForMs(2000);
    lcd.displayToLCD("abcdefghi ");
    lcd.displayToLCD("jklmnopqrstuvwxyz0123456789");
}

void testWebcam(){
    startStream();
}