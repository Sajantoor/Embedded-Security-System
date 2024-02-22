#include "hal/led.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "common/utils.hpp"

#define LED_PATH "/sys/class/leds/beaglebone:green:usr"
#define LED_TRIGGER "/trigger"
#define LED_BRIGHTNESS "/brightness"

LED::LED() {
	removeTriggers();
	turnOffLeds();
}

LED::~LED() {
	updateTrigger(LED0, "heartbeat");
	updateTrigger(LED1, "mmc0");
	updateTrigger(LED2, "cpu0");
	updateTrigger(LED3, "mmc1");
}

// Public methods
void LED::updateBrightness(ledId id, bool on) {
	std::ostringstream pathStream;
	pathStream << LED_PATH << id << LED_BRIGHTNESS;
	std::string path = pathStream.str();

	std::ofstream ledBrightnessFile(path);
	if (ledBrightnessFile.is_open()) {
		ledBrightnessFile << (on ? 1 : 0);
		ledBrightnessFile.close();
	} else {
		std::cerr << "Error opening LED brightness file for LED " << id << std::endl;
	}
}

void LED::flash(const std::vector<ledId> leds, int hertz, int durationMs) {
	// the period is the time for one cycle in milliseconds
	int periodMs = 1000 / hertz;
	int numCycles = (durationMs + periodMs - 1) / periodMs;

	for (int cycle = 0; cycle < numCycles; cycle++) {
		for (size_t i = 0; i < leds.size(); i++) {
			updateBrightness(leds[i], true);
		}

		// sleep for half the period
		sleepForMs(periodMs / 2);

		for (size_t i = 0; i < leds.size(); i++) {
			updateBrightness(leds[i], false);
		}
		sleepForMs(periodMs / 2);
	}
}

// Private methods
void LED::updateTrigger(ledId id, std::string trigger) {
	std::ostringstream pathStream;
	pathStream << LED_PATH << id << LED_TRIGGER;
	std::string path = pathStream.str();

	std::ofstream ledTriggerFile(path);
	if (ledTriggerFile.is_open()) {
		ledTriggerFile << trigger;
		ledTriggerFile.close();
	} else {
		std::cerr << "Error opening LED trigger file for LED " << id << std::endl;
	}
}

void LED::removeTriggers(void) {
	for (int i = LED0; i <= LED3; i++) {
		updateTrigger(static_cast<ledId>(i), "none");
	}
}

void LED::turnOffLeds(void) {
	for (int i = LED0; i <= LED3; i++) {
		updateBrightness(static_cast<ledId>(i), false);
	}
}
