#include "displayManager.hpp"
#include "common/utils.hpp"
#include <thread>
#include <string>

DisplayManager::DisplayManager(LCD& lcd, Keypad& keypad) : lcd(lcd), keypad(keypad) {
}

void DisplayManager::displayMessage(std::string message, unsigned int timeoutInMs, bool requireKeypadInput) {
	lcd.clearDisplay();
	lcd.displayToLCD(message);
	if (timeoutInMs > 0) {
		std::thread([this, timeoutInMs] {
			sleepForMs(timeoutInMs);
			lcd.clearDisplay();
			}).detach();
	}
	else if (requireKeypadInput) {
		keypad.startInput();
		std::string prevInput = "";
		while (true) {
			std::string input = keypad.getInput();
			if (input != prevInput) {
				lcd.clearDisplay();
				lcd.displayToLCD(input);
				prevInput = input;
			}
			else if (keypad.isInputComplete()) {
				std::string completeInput = keypad.getInput();
				lcd.clearDisplay();
				lcd.displayToLCD(completeInput);
				break;
			}
		}
	}
}
