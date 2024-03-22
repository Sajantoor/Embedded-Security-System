#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/keypad.hpp"
#include "hal/led.hpp"
#include "hal/motionSensor.hpp"
#include "hal/relay.hpp"

void testRelay();
void testKeypad();
void testMotionSensor();

int main(void) {
    std::cout << "Hello, World!" << std::endl;
    testRelay();
    testKeypad();
    sleepForMs(5000);
    testMotionSensor();
    return 0;
}

void testRelay() {
    Relay relay;
    sleepForMs(500);

    // Closing lock
    relay.open();
    sleepForMs(500);

    // Opening lock
    relay.close();
    sleepForMs(500);
}

void testKeypad() {
    Keypad keypad(4);
    keypad.startInput();
    std::cout << "You entered: " << keypad.getInput() << std::endl;
}

void testMotionSensor() {
    while (true) {
        bool isMotionDetected = MotionSensor::isMotionDetected();
        std::cout << "Motion sensor: " << isMotionDetected << std::endl;
    }
}
