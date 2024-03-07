#include "hal/relay.hpp"
#include "hal/gpio.hpp"

#define RELAY_PIN 13
static GPIO gpioRelay;

Relay::Relay() {
    // Configure pins to GPIO
    gpioRelay.configPin(P9, 19, "gpio");
    gpioRelay.configPin(P9, 20, "gpio");

    //Export pins to use
    gpioRelay.exportPin(RELAY_PIN);
    gpioRelay.exportPin(12);
    gpioRelay.setPinDirection(RELAY_PIN, "out");
}

Relay::~Relay() {

}

void Relay::openRelay() {
    gpioRelay.setPinValue(RELAY_PIN, 0);
}

void Relay::closeRelay() {
    gpioRelay.setPinValue(RELAY_PIN, 1);
}