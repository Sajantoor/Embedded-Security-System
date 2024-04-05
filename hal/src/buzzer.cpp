
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
    isStopped = false;
    currentSound = NONE;

    buzzerThread = std::thread([this]() {
        while (!isStopped) {
            if (currentSound != NONE) {
                switch (currentSound) {
                    case HIT:
                        playHitSound();
                        break;
                    case MISS:
                        playMissSound();
                        break;
                    default:
                        break;
                }
            }
        }
    });
}

void Buzzer::stop(void) {
    isStopped = true;
    currentSound = NONE;
    buzzerThread.join();
}

void Buzzer::disableBuzzerSound(bool shouldSetSoundToNone) {
    if (shouldSetSoundToNone) {
        currentSound = NONE;
    }

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

void Buzzer::playHitSound(void) {
    updatePwm(800000, 400000);
    sleepForMs(150);
    updatePwm(400000, 200000);
    sleepForMs(150);
    updatePwm(800000, 400000);
    sleepForMs(150);
    disableBuzzerSound();
}

void Buzzer::playMissSound(void) {
    updatePwm(1000000, 500000);
    bool didConditionBreak = sleepWhileCheckingConditon([this]() { return currentSound == MISS; }, 250);

    if (didConditionBreak) {
        disableBuzzerSound(false);
        return;
    }

    updatePwm(1000000, 20000);
    didConditionBreak = sleepWhileCheckingConditon([this]() { return currentSound == MISS; }, 1000);

    if (didConditionBreak) {
        disableBuzzerSound(false);
        return;
    }

    disableBuzzerSound();
}

void Buzzer::buzz(BuzzerSounds sound) {
    if (isStopped) return;
    if (currentSound == HIT) currentSound = sound;
}

bool Buzzer::sleepWhileCheckingConditon(std::function<bool()> condition, int timeoutMs) {
    int timeout = 0;
    while (condition() && timeout < timeoutMs && !isStopped) {
        sleepForMs(1);
        timeout++;
    }

    if (timeout >= timeoutMs) {
        return false;
    }

    return true;
}
