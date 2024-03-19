#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/led.hpp"
#include "hal/relay.hpp"
#include "hal/keypad.hpp"

void testRelay();
void testKeypad();

int main(void) {
	std::cout << "Hello, World!" << std::endl;
	// testRelay();
	testKeypad();
	return 0;
}

void testRelay() {
	Relay relay;
	sleepForMs(1000);

	// Closing lock
	relay.open();
	sleepForMs(1000);

	// Opening lock
	relay.close();
	sleepForMs(500);
}

void testKeypad() {
	Keypad keypad(4);
	keypad.startInput();
	std::cout << "You entered: " << keypad.getInput() << std::endl;
}
