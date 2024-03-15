#ifndef _KEYPAD_HPP_
#define _KEYPAD_HPP_

#include <string>

class Keypad {
private:
	std::string buttonsPressed = "";
	unsigned int inputLength;
public:
	// Initializes the keypad with the number of digits required until input is considered complete
	// If inputLength is set to 4, then startInput waits for 4 digits are entered
	Keypad(unsigned int inputLength);

	// Wait for the user to press inputLength digits
	void startInput(void);

	// Return the input that was entered by the user after startInput exits
	std::string getInput(void);

	// Getters and setters to update the input length
	void setInputLength(unsigned int length);
	unsigned int getInputLength(void);
};

#endif