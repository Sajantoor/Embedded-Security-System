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

void LCD::displayMessage(std::string msg) {
    clearDisplay();

    gpio.setPinValue(LcdGpioPins::RS, "1");
    for(unsigned int i = 0; i < msg.length(); i++) {
        write4bits(msg[i] >> 4);
        write4bits(msg[i] & 0xF);

        if(i == LCD_LENGTH-1) {
            setDramAddress(0x40);
            gpio.setPinValue(LcdGpioPins::RS, "1");
        }
    }
}

void LCD::setDramAddress(uint8_t addy) {
    gpio.setPinValue(LcdGpioPins::RS, "0");
    std::cout << (0x8+(addy >> 4)) << std::endl;
    std::cout << (addy & 0xF) << std::endl;
    write4bits(0x8+(addy >> 4));
    write4bits(addy & 0xF);
    sleepForNs(64000);
}


