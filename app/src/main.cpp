#include <iostream>
#include <vector>

#include "common/utils.hpp"
#include "hal/led.hpp"
#include "hal/relay.hpp"

void testRelay();

int main(void) {
    std::cout << "Hello, World!" << std::endl;
    testRelay();
    return 0;
}

void testRelay() {
    Relay relay;
    sleepForMs(1000);

    //Closing lock
    relay.openRelay();
    sleepForMs(1000);
    
    //Opening lock
    relay.closeRelay();
    sleepForMs(500);
}
