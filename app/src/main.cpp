#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/led.hpp"
#include "hal/relay.hpp"

void testRelay();
void testKeypad();
void testLCD();

int main(void) {
    std::cout << "Hello, World!" << std::endl;
    // testRelay();
    testLCD();
    // testKeypad();
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

void testKeypad() {
    Keypad keypad(4);
    keypad.startInput();
    std::cout << "You entered: " << keypad.getInput() << std::endl;
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