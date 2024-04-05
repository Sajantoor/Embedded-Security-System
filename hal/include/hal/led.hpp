/*
Contains functions for interacting with the LEDs.
*/
#ifndef _LED_HPP_
#define _LED_HPP_

#include <stddef.h>
#include <string>
#include <vector>

typedef enum { LED0, LED1, LED2, LED3 } ledId;

class LED {
  private:
    void updateTrigger(ledId id, const std::string trigger);
    void removeTriggers();
    void turnOffLeds();

  public:
    LED();
    ~LED();

    void updateBrightness(ledId id, bool on);
    void flash(const std::vector<ledId> leds, int hertz, int durationMs);
};

#endif
