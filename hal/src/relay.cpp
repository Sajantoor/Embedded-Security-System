#include "hal/relay.hpp"
#include "hal/gpio.hpp"

Relay::Relay() {
    // Configure pins to GPIO
    GPIO gpioRelay;
    gpioRelay.configPin(P9, 19, "gpio");
    gpioRelay.configPin(P9, 20, "gpio");

    //Export pins to use
    gpioRelay.exportPin(13);
    gpioRelay.exportPin(12);
    gpioRelay.setPinDirection(13, "out");
}

Relay::~Relay() {

}

void Relay::openRelay() {
    GPIO relay;
    relay.setPinValue(13, 0);
}

void Relay::closeRelay() {
    GPIO relay;
    relay.setPinValue(13, 1);
}