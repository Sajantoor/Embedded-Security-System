#ifndef _KEYPAD_HPP_
#define _KEYPAD_HPP_

#include <stdbool.h>
#include <string>
#include <thread>

/**
 * Class to handle the keypad input
*/
class Keypad {
  private:
    std::string buttonsPressed = "";
    unsigned int inputLength;
    bool isRunning = true;
    std::thread keypadThread;
    void readKeys(void);

  public:
    // Initializes the keypad with the number of digits required until input is considered complete
    // If inputLength is set to 4, then startInput waits for 4 digits are entered
    Keypad(unsigned int inputLength);

    // Stops the thread
    void stop(void);

    // Resets the input and stores new input
    void startInput(void);

    // Return the input that has been entered by the user so far
    std::string getInput(void);

    // Checks if the input length has been reached
    bool isInputComplete(void);

    // Getters and setters to update the input length
    void setInputLength(unsigned int length);
    unsigned int getInputLength(void);
};

#endif