// Unless specified, each function will set RS Pin to 0
#ifndef _LCD_HPP_
#define _LCD_HPP_

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include "hal/gpio.hpp"

enum LcdGpioPins { D4 = 66, D5 = 69, D6 = 115, D7 = 48, RS = 68, E = 67 };

/**
 * Class to handle the LCD display
*/
class LCD {
  private:
    GPIO gpio;
    bool isScrolling = false;
    // Mutex for display functions
    std::mutex displayMutex = std::mutex();
    std::string currentMessage = "";
    bool isShutdown = false;
    std::thread scrollingThread;

    void write8bits(uint8_t value);
    void write4bits(uint8_t value);
    // Pulse lets LCD know that first 4 bits have been written, and that the new 4 bits will be written next (only needed in 4bit mode)
    void enablePulse();

    // Set DDRAM addr to 0, shifts display to original position
    // But does not clear DDRAM values
    void returnHome();
    // Set cursor move direction (cursorDir=1 is move right),
    // specifies display shift (displayShift=0 for no shift)
    void entryModeSet(bool cursorDir, uint8_t displayShift);
    // Set display on (dispOn=1), cursor on (cursorOn=1),
    // cursor blinking on (blinkOn=1)
    void displayControl(bool dispOn, bool cursorOn, bool blinkOn);
    // Move cursor or display shift without changing DDRAM contents
    // cursor move (shiftOrCursor = 0), go right (rightOrLeft=1)
    void cursorOrDisplayShift(bool shiftOrCursor, bool rightOrLeft);
    // NOTE: only 4bit mode is currently supported
    // Set 8bit mode (dispMode=1) or 4bit mode (dispMode=0),
    // display lines: 1 line (numOfLines=0), 2 line (numOfLines=1),
    // character font: 5x8 (fontSize=0), 5x10 (fontSize=1)
    void functionSet(bool dispMode, bool numOfLines, bool fontSize);
    // Set CGRAM (Character Generator Ram) address
    void setCgramAddress(uint8_t addr);
    // Set DDRAM (Display Data RAM) address
    void setDdramAddress(uint8_t addr);

    // Scrolls text on the LCD
    void scrollText(std::string msg);
    // Function for scroll text thread, should not be called outside of constructor
    void scrollTextThread(void);
    // Displays non scrolling text
    void displayNonScrollingText(std::string msg);
    // Initializes LCD into 4 bit mode
    void initLCD();
    // export and config gpio pins
    void initGpioForLCD();
    // Writes character to DDRAM
    void writeCharacter(char c);
    // Clears the display and specifies whether the function calling it has a lock or not
    void clearDisplayWithoutLock(bool stopScroll);

  public:
    LCD();
    void stop(void);
    // Writes data to DDRAM. RS Pin will be set to 1
    void displayToLCD(std::string msg);
    // Clears display, sets DDRAM addr to 0, clears DDRAM values
    void clearDisplay(bool stopScroll = true);
};

#endif