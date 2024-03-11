#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/led.hpp"
#include "hal/lcd.hpp"

int main(void) {
    std::cout << "Hello, World!" << std::endl;
    LCD l = LCD(LcdDisplayMode::eightBit);

    l.displayMessage("Hello!!!");
    return 0;
}
