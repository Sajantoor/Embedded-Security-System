#include "surveillance.hpp"
#include <iostream>

Surveillance::Surveillance(MotionSensor* motionSensor, Notifier* notifer) {
    this->motionSensor = motionSensor;
    this->notifier = notifer;
    isRunning = true;
    this->thread = new std::thread(&Surveillance::run, this);
}

void Surveillance::stop(void) {
    isRunning = false;
    this->thread->join();
}

void Surveillance::run(void) {
    int motionCounter = 0;
    std::time_t timestamp = 0;

    while (isRunning) {
        if (motionSensor->isMotionDetected()) {
            motionCounter++;
        } else {
            if (motionCounter > 0) {
                motionCounter--;
            }
        }

        if (motionCounter > MOTION_THRESHOLD && std::time(nullptr) - timestamp > MOTION_TIMEOUT) {
            notifier->notify(MOTION_DETECTED);
            timestamp = std::time(nullptr);
            motionCounter = 0;
        }
    }
}
