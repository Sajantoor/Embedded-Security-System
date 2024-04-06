#include "hal/lcd.hpp"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <bitset>
#include <iostream>

#include "common/utils.hpp"
#include "hal/gpio.hpp"

#define LCD_LENGTH 16
#define LCD_WIDTH 2
#define DDRAM_SIZE 80
#define SECOND_LINE_ADDRESS 0x40

LCD::LCD() {
    GPIO gpio = GPIO();
    this->gpio = gpio;

    gpio.exportPin(LcdGpioPins::D4);
    gpio.exportPin(LcdGpioPins::D5);
    gpio.exportPin(LcdGpioPins::D6);
    gpio.exportPin(LcdGpioPins::D7);
    gpio.exportPin(LcdGpioPins::RS);
    gpio.exportPin(LcdGpioPins::E);

    gpio.configPin(P9, 27, "gpio");
    gpio.configPin(P9, 15, "gpio");
    gpio.configPin(P8, 7, "gpio");
    gpio.configPin(P8, 9, "gpio");
    gpio.configPin(P8, 10, "gpio");
    gpio.configPin(P8, 8, "gpio");

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

    initLCD();

    functionSet(0, 1, 0);  // Function set: 2 line display, 5x8 dot char font
    sleepForNs(128000);

    displayControl(0, 0, 0);  // Display off, cursor off, blinking off
    sleepForNs(128000);

    clearDisplay();  // clear display
    sleepForNs(64000);

    entryModeSet(1, 0);  // Entry Mode set: Increment by 1, no shift
    sleepForNs(128000);

    displayControl(1, 1, 1);  // Display on, cursor on, blinking on
    sleepForNs(64000);

    // initialize the scroll thread
    scrollingThread = std::thread(&LCD::scrollTextThread, this);
}

void LCD::stop(void) {
    isShutdown = true;
    isScrolling = false;
    scrollingThread.join();
    clearDisplay();
}

void LCD::initLCD() {
    // The first 3 function set calls lets the lcd know to reset and
    // reinitialize on the 4th function set
    write4bits(0x03);  // Function set 1.
    sleepForMs(5);

    write4bits(0x3);  // Function set 2
    sleepForNs(128000);

    write4bits(0x3);  // Function set 3
    sleepForNs(128000);

    write4bits(0x2);  // Function set to change to 4 bit mode
    sleepForMs(1);
}

void LCD::write4bits(uint8_t value) {
    uint8_t bit = value & 0x01;
    gpio.setPinValue(LcdGpioPins::D4, bit);

    bit = (value >> 1) & 0x01;
    gpio.setPinValue(LcdGpioPins::D5, bit);

    bit = (value >> 2) & 0x01;
    gpio.setPinValue(LcdGpioPins::D6, bit);

    bit = (value >> 3) & 0x01;
    gpio.setPinValue(LcdGpioPins::D7, bit);

    enablePulse();
}

void LCD::enablePulse() {
    gpio.setPinValue(LcdGpioPins::E, 1);
    sleepForMs(1);
    gpio.setPinValue(LcdGpioPins::E, 0);
    sleepForMs(1);
}

void LCD::clearDisplayWithoutLock(bool stopScroll) {
    if (stopScroll) {
        isScrolling = false;
    }

    gpio.setPinValue(LcdGpioPins::RS, 0);
    write4bits(0x0);
    write4bits(0x1);
    sleepForNs(64000);
}

void LCD::clearDisplay(bool stopScroll) {
    if (stopScroll) {
        isScrolling = false;
    }

    // must accquire a lock
    displayMutex.lock();
    clearDisplayWithoutLock(false);
    displayMutex.unlock();
}

void LCD::returnHome() {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    write4bits(0x0);
    write4bits(0x2);
    sleepForMs(2);
}

void LCD::entryModeSet(bool cursorDir, uint8_t displayShift) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t id = cursorDir << 1;
    uint8_t val = 0x4 + id + displayShift;
    write4bits(0x0);
    write4bits(val & 0xF);
    sleepForNs(64000);
}

void LCD::displayControl(bool dispOn, bool cursorOn, bool blinkOn) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t d = dispOn << 2;
    uint8_t c = cursorOn << 1;
    uint8_t val = 0x8 + d + c + blinkOn;
    write4bits(0x0);
    write4bits(val & 0xF);
    sleepForNs(64000);
}

void LCD::cursorOrDisplayShift(bool shiftOrCursor, bool rightOrLeft) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t sc = shiftOrCursor << 3;
    uint8_t rl = rightOrLeft << 2;
    uint8_t val = sc + rl;
    write4bits(0x1);
    write4bits(val & 0xF);
    sleepForNs(64000);
}

void LCD::functionSet(bool dispMode, bool numOfLines, bool fontSize) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t upperVal = 0x2 + dispMode;
    uint8_t n = numOfLines << 3;
    uint8_t f = fontSize << 2;
    uint8_t lowerVal = (n + f);
    write4bits(upperVal);
    write4bits(lowerVal & 0xF);
    sleepForNs(64000);
}

void LCD::setCgramAddress(uint8_t addr) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t upperVal = 0x4 + (addr >> 4);
    uint8_t lowerVal = addr;
    write4bits(upperVal);
    write4bits(lowerVal & 0xF);
    sleepForNs(64000);
}

void LCD::setDdramAddress(uint8_t addr) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    write4bits(0x8 + (addr >> 4));
    write4bits(addr & 0xF);
    sleepForNs(64000);
}

void LCD::writeCharacter(char c) {
    write4bits(c >> 4);
    write4bits(c & 0xF);
    sleepForNs(64000);
}

void LCD::scrollText(std::string message) {
    isScrolling = true;
    currentMessage = message;
}

void LCD::scrollTextThread() {
    while (!isShutdown) {
        unsigned int msgIndex = 0;
        bool isFirstLoop = true;
        bool hasLock = false;

        while (isScrolling) {
            if (!hasLock) {
                displayMutex.lock();
                hasLock = true;
            }

            gpio.setPinValue(LcdGpioPins::RS, 1);

            if (msgIndex > currentMessage.length()) {
                msgIndex = 0;
            }

            for (unsigned int i = 0; i < LCD_LENGTH; i++) {
                unsigned int currIndex =
                    msgIndex + i < currentMessage.length() ? msgIndex + i : (msgIndex + i) % currentMessage.length();

                if (currIndex == 0 && !isFirstLoop) {
                    for (int j = 0; j < 5; j++) {
                        writeCharacter(' ');
                    }
                }

                writeCharacter(currentMessage[currIndex]);
            }

            msgIndex++;
            sleepForMs(300);

            if (isFirstLoop && isScrolling) {
                sleepForMs(1500);
                isFirstLoop = false;
            }

            clearDisplayWithoutLock(false);
        }

        if (hasLock) {
            displayMutex.unlock();
            hasLock = false;
        }
    }
}

void LCD::displayNonScrollingText(std::string msg) {
    displayMutex.lock();

    for (unsigned int i = 0; i < msg.length(); i++) {
        gpio.setPinValue(LcdGpioPins::RS, 1);
        writeCharacter(msg[i]);

        if (i == LCD_LENGTH - 1) {
            // jump addresses once you reach 16 chars displayed because
            // displayable ram is not contiguous
            // second line is at 0x40
            setDdramAddress(SECOND_LINE_ADDRESS);
        }
    }

    displayMutex.unlock();
}

void LCD::displayToLCD(std::string msg) {
    // clear display and stop scrolling
    clearDisplay();

    if (msg.length() > DDRAM_SIZE) {
        std::cerr << "message length cannot be greater than 80 chars" << std::endl;
    } else if (msg.length() > LCD_LENGTH * LCD_WIDTH) {
        scrollText(msg);
    } else {
        displayNonScrollingText(msg);
    }
}
