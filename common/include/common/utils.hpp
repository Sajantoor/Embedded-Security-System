#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <functional>
#include <string>

void sleepForMs(long long ms);
void sleepForNs(long long ns);
long long getTimeInMs(void);
void runCommand(std::string command, bool show_output);
void sleepWhileCheckingConditon(std::function<bool()> condition, int timeoutMs);

#endif