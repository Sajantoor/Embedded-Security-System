#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "displayManager.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/led.hpp"
#include "hal/relay.hpp"
#include "password.hpp"

void testRelay();
void testDisplayWithKeypad(LCD& lcd, Keypad& keypad);
void testLCD();

int main(void) {
    Password password;
    LCD lcd;
    Keypad keypad(4);
    std::cout << "Hello, World!" << std::endl;
    // testRelay();
    // testLCD();
    testDisplayWithKeypad(lcd, keypad);
    keypad.stop();
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

void testDisplayWithKeypad(LCD& lcd, Keypad& keypad) {
    DisplayManager displayManager = DisplayManager(lcd, keypad);
    displayManager.displayMessage("Hello World!");
    displayManager.displayMessage("Enter password:", 0, true);
    std::cout << "Password entered: " << keypad.getInput() << std::endl;
    displayManager.displayMessage("Door is locked", 5000);
    displayManager.displayMessage("Confirm password:", 0, true);
    std::cout << "Password entered: " << keypad.getInput() << std::endl;
    displayManager.displayMessage("Door is unlocked", 5000);
    sleepForMs(6000);
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