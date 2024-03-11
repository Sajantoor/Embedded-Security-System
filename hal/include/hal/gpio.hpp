/*
Contains functions for interacting with the GPIO pins.
*/
#ifndef _GPIO_HPP_
#define _GPIO_HPP_

#include <string>

typedef enum { P8 = 8, P9 = 9 } headerType;

class GPIO {
  public:
	GPIO();
	~GPIO();

	void exportPin(int pin);
	void configPin(headerType header, int pin, std::string setting);
	void setPinValue(int pin, std::string val);
	int getPinValue(int pin);
	void setPinActiveLow(int pin, int activeLow);
	bool isPinActive(int pin);
	void setPinDirection(int pin, std::string direction);
};

#endif
