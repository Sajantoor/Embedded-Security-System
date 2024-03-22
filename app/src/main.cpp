#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/led.hpp"
#include "hal/lcd.hpp"

int main(void) {
    std::cout << "Hello, World!" << std::endl;
    LCD lcd = LCD();

    // first 16 chars shown ("My ")
    // 24 chars not shown
    // then 16 shown ("working")
    // final 19 chars not displayed
    // lcd.displayMessage("Hello World!!! My Name is Jimmy.");
    
    lcd.clearDisplay();
    lcd.displayToLCD("abcdefghi ");
    lcd.displayToLCD("jklmnopqrstuvwxyz0123456789");
    return 0;
}
