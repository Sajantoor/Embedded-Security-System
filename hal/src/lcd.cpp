#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "hal/gpio.hpp"
#include "hal/lcd.hpp"
#include "common/utils.hpp"


LCD::LCD(LcdDisplayMode displayMode) {
    GPIO gpio = GPIO();
    // this->dispMode = displayMode;
    this->gpio = gpio;

    gpio.setPinDirection(LcdGpioPins::D4, "out");
    gpio.setPinDirection(LcdGpioPins::D5, "out");
    gpio.setPinDirection(LcdGpioPins::D6, "out");
    gpio.setPinDirection(LcdGpioPins::D7, "out");
    gpio.setPinDirection(LcdGpioPins::RS, "out");
    gpio.setPinDirection(LcdGpioPins::E, "out");

    gpio.setPinValue(LcdGpioPins::D4, 0);
    gpio.setPinValue(LcdGpioPins::D5, 0);
    gpio.setPinValue(LcdGpioPins::D6, 0);
    gpio.setPinValue(LcdGpioPins::D7, 0);
    gpio.setPinValue(LcdGpioPins::RS, 0);
    gpio.setPinValue(LcdGpioPins::E, 0);

    write8bits(0x38);
    sleepForNs(128000);

    write8bits(0x08);
    sleepForNs(128000);

    write8bits(0x01);
    sleepForNs(64000);

    write8bits(0x06);
    sleepForNs(128000);

    write8bits(0x0F);
    sleepForNs(64000);
}
LCD::~LCD() {
}

void LCD::write8bits(uint8_t value) {
    char strBit[2];
    strBit[1] = '\0'; 

    strBit[0] = (value & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D0, strBit);
    
    strBit[0] = ((value >> 1) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D1, strBit);
    
    strBit[0] = ((value >> 2) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D2, strBit);
    
    strBit[0] = ((value >> 3) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D3, strBit);

    strBit[0] = ((value >> 4) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D4, strBit);

    strBit[0] = ((value >> 5) & 0x01 ? 1 : 0) + '0';
    gpio.setPinValue(LcdGpioPins::D5, strBit);

    strBit[0] = ((value >> 6) & 0x01 ? 1 : 0) + '0';
    gpio.setPinValue(LcdGpioPins::D6, strBit);

    strBit[0] = ((value >> 7) & 0x01 ? 1 : 0) + '0';
    gpio.setPinValue(LcdGpioPins::D7, strBit);
}


void LCD::displayMessage(std::string msg) {
    
}

