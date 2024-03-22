#include "hal/relay.hpp"
#include "hal/gpio.hpp"

#define RELAY_PIN 13
#define SECONDARY_RELAY_PIN 12

Relay::Relay() {
    // Configure pins to GPIO
    GPIO::configPin(P9, 19, "gpio");
    GPIO::configPin(P9, 20, "gpio");

    //Export pins to use
    GPIO::exportPin(RELAY_PIN);
    GPIO::exportPin(SECONDARY_RELAY_PIN);
    GPIO::setPinDirection(RELAY_PIN, "out");
}

void Relay::open() {
    GPIO::setPinValue(RELAY_PIN, 0);
}

void Relay::close() {
    GPIO::setPinValue(RELAY_PIN, 1);
}

bool Relay::isOpen() {
    // 1 for closed, 0 for open. 
    return !GPIO::getPinValue(RELAY_PIN);
}
