#pragma once

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

    public:
        LCD(LcdDisplayMode displayMode);
        ~LCD();
        void displayMessage(std::string msg);
};