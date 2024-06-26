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

    initGpioForLCD();
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

void LCD::initGpioForLCD() {
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
    uint8_t cursorDirInt = cursorDir << 1;
    uint8_t lcdInstruction = (0x4 + cursorDirInt + displayShift) & 0xF;
    write4bits(0x0);
    write4bits(lcdInstruction);
    sleepForNs(64000);
}

void LCD::displayControl(bool dispOn, bool cursorOn, bool blinkOn) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t dispOnInt = dispOn << 2;
    uint8_t cursonOnInt = cursorOn << 1;
    uint8_t lcdInstruction = (0x8 + dispOnInt + cursonOnInt + blinkOn) & 0xF;
    write4bits(0x0);
    write4bits(lcdInstruction);
    sleepForNs(64000);
}

void LCD::cursorOrDisplayShift(bool shiftOrCursor, bool rightOrLeft) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t shiftOrCursorInt = shiftOrCursor << 3;
    uint8_t rightOrLeftInt = rightOrLeft << 2;
    uint8_t lcdInstruction = (shiftOrCursorInt + rightOrLeftInt) & 0xF;
    write4bits(0x1);
    write4bits(lcdInstruction);
    sleepForNs(64000);
}

void LCD::functionSet(bool dispMode, bool numOfLines, bool fontSize) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t lcdInstructionUpperHalf = 0x2 + dispMode;
    uint8_t numOfLinesInt = numOfLines << 3;
    uint8_t fontSizeInt = fontSize << 2;
    uint8_t lcdInstructionLowerHalf = (numOfLinesInt + fontSizeInt) & 0xF;
    write4bits(lcdInstructionUpperHalf);
    write4bits(lcdInstructionLowerHalf);
    sleepForNs(64000);
}

void LCD::setCgramAddress(uint8_t addr) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t lcdInstructionUpperHalf = 0x4 + (addr >> 4);
    uint8_t lcdInstructionLowerHalf = addr & 0xF;
    write4bits(lcdInstructionUpperHalf);
    write4bits(lcdInstructionLowerHalf);
    sleepForNs(64000);
}

void LCD::setDdramAddress(uint8_t addr) {
    gpio.setPinValue(LcdGpioPins::RS, 0);
    uint8_t lcdInstructionUpperHalf = 0x8 + (addr >> 4);
    uint8_t lcdInstructionLowerHalf = addr & 0xF;
    write4bits(lcdInstructionUpperHalf);
    write4bits(lcdInstructionLowerHalf);
    sleepForNs(64000);
}

void LCD::writeCharacter(char character) {
    uint8_t lcdInstructionUpperHalf = character >> 4;
    uint8_t lcdInstructionLowerHalf = character & 0xF;
    write4bits(lcdInstructionUpperHalf);
    write4bits(lcdInstructionLowerHalf);
    sleepForNs(64000);
}

void LCD::scrollText(std::string message) {
    isScrolling = true;
    currentMessage = message;
    // Insert 5 spaces into message at index 0 so that when message loops around there is space between the start and the end
    currentMessage.insert(0, "     ");
}

void LCD::scrollTextThread() {
    unsigned int msgIndex = 0;
    unsigned int currIndex = 0;
    bool hasLock = false;

    while (!isShutdown) {
        msgIndex = 0;
        hasLock = false;

        while (isScrolling) {
            if (!hasLock) {
                displayMutex.lock();
                hasLock = true;
            }

            gpio.setPinValue(LcdGpioPins::RS, 1);

            for (unsigned int i = 0; i < LCD_LENGTH; i++) {
                currIndex = msgIndex + i;
                if (currIndex >= currentMessage.length()) {
                    currIndex = currIndex % currentMessage.length();
                }
                writeCharacter(currentMessage[currIndex]);
            }

            msgIndex = (msgIndex >= currentMessage.length() - 1) ? 0 : msgIndex + 1;
            sleepForMs(400);
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

    if (msg.length() > LCD_LENGTH * LCD_WIDTH) {
        scrollText(msg);
    } else {
        displayNonScrollingText(msg);
    }
}
