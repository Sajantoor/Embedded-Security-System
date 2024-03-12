#include "hal/relay.hpp"
#include "hal/gpio.hpp"

#define RELAY_PIN 13

Relay::Relay() {
    // Configure pins to GPIO
    GPIO::configPin(P9, 19, "gpio");
    GPIO::configPin(P9, 20, "gpio");

    //Export pins to use
    GPIO::exportPin(RELAY_PIN);
    GPIO::exportPin(12);
    GPIO::setPinDirection(RELAY_PIN, "out");
}

void Relay::open() {
    GPIO::setPinValue(RELAY_PIN, 0);
}

void Relay::close() {
    GPIO::setPinValue(RELAY_PIN, 1);
}
