#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "hal/gpio.hpp"
#include "hal/lcd.hpp"
#include "common/utils.hpp"

#define LCD_LENGTH 16
#define LCD_WIDTH 2

LCD::LCD(LcdDisplayMode displayMode) {
    std::cout << "Starting lcd init" << std::endl;

    GPIO gpio = GPIO();
    // this->dispMode = displayMode;
    this->gpio = gpio;
    gpio.setPinDirection(LcdGpioPins::D4, "out");
    gpio.setPinDirection(LcdGpioPins::D5, "out");
    gpio.setPinDirection(LcdGpioPins::D6, "out");
    gpio.setPinDirection(LcdGpioPins::D7, "out");
    gpio.setPinDirection(LcdGpioPins::RS, "out");
    gpio.setPinDirection(LcdGpioPins::E, "out");

    std::cout << "Resetting Pins to 0" << std::endl;
    gpio.setPinValue(LcdGpioPins::D4, "0");
    gpio.setPinValue(LcdGpioPins::D5, "0");
    gpio.setPinValue(LcdGpioPins::D6, "0");
    gpio.setPinValue(LcdGpioPins::D7, "0");
    gpio.setPinValue(LcdGpioPins::RS, "0");
    gpio.setPinValue(LcdGpioPins::E, "0");

    std::cout << "Initializing LCD" << std::endl;
    // The first 3 function set calls lets the lcd know to reset and
    // reinitialize on the 4th function set    
    
    write4bits(0x03); // Function set 1. 
    sleepForMs(5);

    write4bits(0x3); // Function set 2
    sleepForNs(128000);

    write4bits(0x3); // Function set 3
    sleepForNs(128000);

    write4bits(0x2); // Function set to change to 4 bit mode
    sleepForMs(1);
    write4bits(0x2); // Function set: 2 line display, 5x8 dot char font
    write4bits(0x8);
    sleepForNs(128000);

    write4bits(0x0); // Display off, cursor off, blinking off
    write4bits(0x8); 
    sleepForNs(128000);

    write4bits(0x0); // clear display
    write4bits(0x1);
    sleepForNs(64000);

    write4bits(0x0); // Entry Mode set: Increment by 1, no shift
    write4bits(0x6);
    sleepForNs(128000);

    write4bits(0x0); // Display on, cursor on, blinking on
    write4bits(0xF);
    sleepForNs(64000);


    // write8bits(0x30); // Function set 1. 
    // sleepForMs(5);

    // write8bits(0x30); // Function set 2
    // sleepForNs(128000);

    // write8bits(0x30); // Function set 3
    // sleepForNs(128000);

    // write8bits(0x38); // Function set: 2 line display, 5x8 dot char font
    // sleepForNs(128000);

    // write8bits(0x08); // Display off, cursor off, blinking off
    // sleepForNs(128000);

    // write8bits(0x01); // clear display
    // sleepForNs(64000);

    // write8bits(0x06); // Entry Mode set: Increment by 1, no shift
    // sleepForNs(128000);

    // write8bits(0x0F); // Display on, cursor on, blinking on
    // sleepForNs(64000);

    // gpio.setPinValue(LcdGpioPins::RS, "1"); // RS = 1 lets you write data to lcd
}
LCD::~LCD() {
}

// Not used
// void LCD::write8bits(uint8_t value) {
//     char strBit[2];
//     strBit[1] = '\0'; 

//     strBit[0] = (value & 0x01 ? 1 : 0) + '0';
// 	gpio.setPinValue(LcdGpioPins::D0, strBit);
    
//     strBit[0] = ((value >> 1) & 0x01 ? 1 : 0) + '0';
// 	gpio.setPinValue(LcdGpioPins::D1, strBit);
    
//     strBit[0] = ((value >> 2) & 0x01 ? 1 : 0) + '0';
// 	gpio.setPinValue(LcdGpioPins::D2, strBit);
    
//     strBit[0] = ((value >> 3) & 0x01 ? 1 : 0) + '0';
// 	gpio.setPinValue(LcdGpioPins::D3, strBit);

//     strBit[0] = ((value >> 4) & 0x01 ? 1 : 0) + '0';
// 	gpio.setPinValue(LcdGpioPins::D4, strBit);

//     strBit[0] = ((value >> 5) & 0x01 ? 1 : 0) + '0';
//     gpio.setPinValue(LcdGpioPins::D5, strBit);

//     strBit[0] = ((value >> 6) & 0x01 ? 1 : 0) + '0';
//     gpio.setPinValue(LcdGpioPins::D6, strBit);

//     strBit[0] = ((value >> 7) & 0x01 ? 1 : 0) + '0';
//     gpio.setPinValue(LcdGpioPins::D7, strBit);
// }

void LCD::write4bits(uint8_t value) {
    char strBit[2];
    strBit[1] = '\0'; 

    strBit[0] = (value & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D4, strBit);
    
    strBit[0] = ((value >> 1) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D5, strBit);
    
    strBit[0] = ((value >> 2) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D6, strBit);
    
    strBit[0] = ((value >> 3) & 0x01 ? 1 : 0) + '0';
	gpio.setPinValue(LcdGpioPins::D7, strBit);

    enablePulse();
}

void LCD::enablePulse() {
    gpio.setPinValue(LcdGpioPins::E, "1");
    sleepForMs(1);
    gpio.setPinValue(LcdGpioPins::E, "0");
    sleepForMs(1);
}

void LCD::clearDisplay() {
    gpio.setPinValue(LcdGpioPins::RS, "0");
    write4bits(0x0);
    write4bits(0x1);
    sleepForNs(64000);
}

void LCD::returnHome() {
    write4bits(0x0);
    write4bits(0x2);
    sleepForMs(2);
}

void LCD::entryModeSet(uint8_t id, uint8_t s) {
    uint8_t val = 0x4 + id + s;
    write4bits(val >> 4);
    write4bits(val & 0xF);
    sleepForNs(64000);
}

void LCD::displayControl(uint8_t d, uint8_t c, uint8_t b) {
    uint8_t val = 0x8 + d + c + b;
    write4bits(val >> 4);
    write4bits(val & 0xF);
    sleepForNs(64000);
}

void LCD::cursorOrDisplayShift(uint8_t sc, uint8_t rl) {
    uint8_t val = (sc + rl) << 2;
    write4bits(0x1);
    write4bits(val & 0xF);
    sleepForNs(64000);
}

void LCD::functionSet(uint8_t dl, uint8_t n, uint8_t f) {
    uint8_t upperVal = 0x2 + dl;
    uint8_t lowerVal = (n + f) << 2;
    write4bits(upperVal);
    write4bits(lowerVal & 0xF);
    sleepForNs(64000);
}

void LCD::setCgramAddress(uint8_t addr) {
    uint8_t upperVal = 0x4 + (addr >> 4);
    uint8_t lowerVal = addr;
    write4bits(upperVal);
    write4bits(lowerVal & 0xF);
    sleepForNs(64000);
}

void LCD::setDdramAddress(uint8_t addr) {
    gpio.setPinValue(LcdGpioPins::RS, "0");
    std::cout << (0x8+(addr >> 4)) << std::endl;
    std::cout << (addr & 0xF) << std::endl;
    write4bits(0x8+(addr >> 4));
    write4bits(addr & 0xF);
    sleepForNs(64000);
}

void LCD::writeDataToLCD(std::string msg) {
    gpio.setPinValue(LcdGpioPins::RS, "1");
    for(unsigned int i = 0; i < msg.length(); i++) {
        write4bits(msg[i] >> 4);
        write4bits(msg[i] & 0xF);

        if(i == LCD_LENGTH-1) {
            setDdramAddress(0x40);
            gpio.setPinValue(LcdGpioPins::RS, "1");
        }
        sleepForNs(64000);
    }
}