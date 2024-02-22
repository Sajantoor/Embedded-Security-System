#include "hal/gpio.hpp"
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "common/utils.hpp"

#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_EXPORT GPIO_PATH "/export"
#define GPIO_VALUE "/value"
#define GPIO_ACTIVE_LOW "/active_low"

void GPIO::exportPin(int pin) {
	std::ostringstream pathStream;
	pathStream << GPIO_PATH << pin << GPIO_VALUE;
	std::string path = pathStream.str();

	if (std::filesystem::exists(path)) return;

	std::ofstream exportFile(GPIO_EXPORT);
	if (exportFile.is_open()) {
		exportFile << pin;
		exportFile.close();
	} else {
		std::cerr << "Error opening GPIO export file for pin " << pin << std::endl;
	}

	// wait for pin to be exported
	sleepForMs(300);
}

void GPIO::configPin(headerType header, int pin, std::string setting) {
	std::stringstream commandStream;
	commandStream << "config-pin p" << header << "." << pin << " " << setting;
	runCommand(commandStream.str(), false);
}

int getPinValue(int pin) {
	std::ostringstream pathStream;
	pathStream << GPIO_PATH << pin << GPIO_VALUE;
	std::string path = pathStream.str();

	std::ifstream valueFile(path);
	if (valueFile.is_open()) {
		int value;
		valueFile >> value;
		valueFile.close();
		return value;
	} else {
		std::cerr << "Error opening GPIO value file for pin " << pin << std::endl;
		return -1;
	}
}

void GPIO::setPinActiveLow(int pin, int activeLow) {
	std::ostringstream pathStream;
	pathStream << GPIO_PATH << pin << GPIO_ACTIVE_LOW;
	std::string path = pathStream.str();

	std::ofstream activeLowFile(path);
	if (activeLowFile.is_open()) {
		activeLowFile << activeLow;
		activeLowFile.close();
	} else {
		std::cerr << "Error opening GPIO active_low file for pin " << pin << std::endl;
	}
}

bool GPIO::isPinActive(int pin) {
	return getPinValue(pin) == 1;
}
