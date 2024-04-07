
#include "hal/buzzer.hpp"

#include <fstream>
#include <iostream>

#include "common/utils.hpp"
#include "hal/gpio.hpp"

static constexpr const char* PWM_DIRECTORY = "/dev/bone/pwm/0/a/";
static constexpr const char* PWM_PERIOD_FILE = "period";
static constexpr const char* PWM_DUTY_CYCLE_FILE = "duty_cycle";
static constexpr const char* PWM_ENABLE_FILE = "enable";

Buzzer::Buzzer(void) {
    GPIO::configPin(P9, 22, "pwm");
    buzzerThread = std::thread([this]() {
        while (!isStopped) {
            if (playSound) {
                updatePwm(800000, 400000);
                sleepForMs(150);
                updatePwm(400000, 200000);
                sleepForMs(150);
                disableBuzzerSound();
                playSound = false;
            }
        }
    });
}

void Buzzer::stop(void) {
    isStopped = true;
    playSound = false;
    buzzerThread.join();
    disableBuzzerSound();
}

void Buzzer::disableBuzzerSound() {
    std::ofstream enableFile(std::string(PWM_DIRECTORY) + PWM_ENABLE_FILE);
    if (enableFile.is_open()) {
        enableFile << 0;
        enableFile.close();
    } else {
        std::cerr << "Error opening enable file" << std::endl;
    }
}

void Buzzer::updatePwm(unsigned int period, unsigned int dutyCycle) {
    std::ofstream dutyCycleReset(std::string(PWM_DIRECTORY) + PWM_DUTY_CYCLE_FILE);
    if (dutyCycleReset.is_open()) {
        dutyCycleReset << 0;
        dutyCycleReset.close();
    } else {
        std::cerr << "Error opening duty cycle file" << std::endl;
    }

    std::ofstream periodFile(std::string(PWM_DIRECTORY) + PWM_PERIOD_FILE);
    if (periodFile.is_open()) {
        periodFile << period;
        periodFile.close();
    } else {
        std::cerr << "Error opening period file" << std::endl;
    }

    std::ofstream dutyCycleFile(std::string(PWM_DIRECTORY) + PWM_DUTY_CYCLE_FILE);
    if (dutyCycleFile.is_open()) {
        dutyCycleFile << dutyCycle;
        dutyCycleFile.close();
    } else {
        std::cerr << "Error opening duty cycle file" << std::endl;
    }

    std::ofstream enableFile(std::string(PWM_DIRECTORY) + PWM_ENABLE_FILE);
    if (enableFile.is_open()) {
        enableFile << 1;
        enableFile.close();
    } else {
        std::cerr << "Error opening enable file" << std::endl;
    }
}

void Buzzer::buzz() {
    playSound = isStopped ? false : true;
}