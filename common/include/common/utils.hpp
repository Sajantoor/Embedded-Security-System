#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <functional>
#include <string>

void sleepForMs(long long ms);
void sleepForNs(long long ns);
long long getTimeInMs(void);
/**
 * Runs a Linux command
*/
void runCommand(std::string command, bool show_output);
/**
 * Sleeps while actively checking a condition every second, if 
 * the condition is met, it will return early
*/
void sleepWhileCheckingConditon(std::function<bool()> condition, int timeoutMs);

#endif