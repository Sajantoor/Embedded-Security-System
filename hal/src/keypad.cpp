#include "hal/keypad.hpp"
#include <vector>
#include <hal/gpio.hpp>
#include <common/utils.hpp>

/*
Keypad pins are from left to right at the bottom of the keypad.
The GPIO pins are on the P8 header.

Keypad pin # | GPIO Pin | Keypad Button    | P8 Header Pin (corresponding to GPIO pin)
1     		 | N/A      | Power and Input  | N/A
2     		 | N/A      | Unused		   | N/A
3     		 | N/A      | *				   | N/A
4     		 | 86	    | 7				   | 27
5     		 | 78	    | 4				   | 37
6     		 | 8    	| 1				   | 35
7     		 | N/A      | 0				   | N/A
8     		 | 9	    | 8				   | 33
9    		 | 10       | 5				   | 31
10    		 | 87       | 2				   | 29
11    		 | N/A      | #				   | N/A
12    		 | N/A 	    | 9				   | N/A
13   		 | 89       | 6				   | 30
14    		 | 88       | 3				   | 28
*/

// Keypad buttons are mapped to the GPIO pins below by index
static const std::vector<char> KEYPAD_BUTTONS = { '1', '2', '3', '4', '5', '6', '7', '8' };
static const std::vector<int> KEYPAD_GPIO_PINS = { 8, 87, 88, 78, 10, 89, 86, 9 };
static const int BUTTON_DOWN_PIN_VALUE = 0;

Keypad::Keypad(unsigned int inputLength) {
	buttonsPressed = "";
	this->inputLength = inputLength;
	for (char pin : KEYPAD_GPIO_PINS) {
		GPIO::exportPin(pin);
		GPIO::setPinDirection(pin, "in");
	}
	keypadThread = std::thread([this] {
		while (isRunning) {
			readKeys();
		}
		});
}

void Keypad::stop(void) {
	isRunning = false;
	keypadThread.join();
}

std::string Keypad::getInput(void) {
	return buttonsPressed;
}

void Keypad::readKeys(void) {
	for (size_t i = 0; i < KEYPAD_GPIO_PINS.size(); i++) {
		if (buttonsPressed.length() >= inputLength) break;
		if (GPIO::getPinValue(KEYPAD_GPIO_PINS[i]) == BUTTON_DOWN_PIN_VALUE) {
			buttonsPressed.append(1, KEYPAD_BUTTONS[i]);
			// Wait for button to be released
			while (GPIO::getPinValue(KEYPAD_GPIO_PINS[i]) == BUTTON_DOWN_PIN_VALUE) {};
		}
	}
}

bool Keypad::isInputComplete(void) {
	return buttonsPressed.length() >= inputLength;
}

void Keypad::startInput(void) {
	buttonsPressed = "";
}

void Keypad::setInputLength(unsigned int length) {
	inputLength = length;
}

unsigned int Keypad::getInputLength(void) {
	return inputLength;
}