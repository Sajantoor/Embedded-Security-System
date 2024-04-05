#include <iostream>

#include "common/utils.hpp"
#include "displayManager.hpp"
#include "hal/keypad.hpp"
#include "hal/lcd.hpp"
#include "hal/led.hpp"
#include "hal/relay.hpp"

void testRelay(void);
void testDisplayWithKeypad(LCD& lcd, Keypad& keypad);
void testLCD(void);
