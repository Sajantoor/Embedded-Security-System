#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/led.hpp"
#include "hal/lcd.hpp"

int main(void) {
    std::cout << "Hello, World!" << std::endl;
    LCD l = LCD(LcdDisplayMode::eightBit);

    // first 16 chars shown ("My ")
    // 24 chars not shown
    // then 16 shown ("working")
    // final 19 chars not displayed
    // l.displayMessage("Hello World!!! My Name is Jimmy.");
    
    l.clearDisplay();
    l.writeDataToLCD("abcdefghi ");
    l.writeDataToLCD("jklmnopqrstuvwxyz0123456789");
    return 0;
}
