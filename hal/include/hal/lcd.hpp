// Header file for lcd.cpp
// Unless specified, RS Pin must be set to 0

#pragma once
#include "hal/gpio.hpp"
//TODO: set D0-3 pin values
enum LcdGpioPins {
                D0, D1, D2, D3, D4 = 66, D5 = 69, D6 = 115, 
                D7 = 48, RS = 68, E = 67
};
enum LcdDisplayMode {fourBit, eightBit};

class LCD {
    private:
        // LcdDisplayMode dispMode;
        GPIO gpio;
        void write8bits(uint8_t value);
        void write4bits(uint8_t value);
        void enablePulse();
        
        // Set DDRAM addr to 0, shifts display to original position
        // But does not clear DDRAM values
        void returnHome();
        // Set cursor move direction (id=1 is move right), specifies display shift (s=0 for no shift)
        void entryModeSet(uint8_t id, uint8_t s);
        // Set display on (d=1) or off (d=0), cursor on (c=1) or off (c=0),
        // blinking on (b=1) or off (b=0)
        void displayControl(uint8_t d, uint8_t c, uint8_t b);
        // Sove cursor or display shift without changing DDRAM contents
        void cursorOrDisplayShift(uint8_t sc, uint8_t rl);
        // Set 8bit mode (dl=1) or 4bit mode (dl=0),
        // number of display lines: 1 line (n=0), 2 line (n=1),
        // character font: 5x8 (f=0), 5x10 (f=1)
        void functionSet(uint8_t dl, uint8_t n, uint8_t f);
        // Set CGRAM (Character Generator Ram) address
        void setCgramAddress(uint8_t addr);
        // Set DDRAM (Display Data RAM) address
        void setDdramAddress(uint8_t addr);

    public:
        LCD(LcdDisplayMode displayMode);
        ~LCD();
        // Writes data to DDRAM. RS Pin must be set to 1
        void writeDataToLCD(std::string msg);
        // Clears display, sets DDRAM addr to 0, clears DDRAM values
        void clearDisplay();
};