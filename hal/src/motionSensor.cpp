#include "hal/motionSensor.hpp"

#include <fstream>
#include <iostream>

#define A2D_VOLTAGE_REF 1.8
#define A2D_MAX_READING 4095
#define MOTION_THRESHOLD 1000

static constexpr const char* VOLTAGE_FILE =
    "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";

bool MotionSensor::isMotionDetected(void) {
    return getA2DReading() > MOTION_THRESHOLD;
}

/**
 * Reads the A2D voltage from the file
 */
unsigned int MotionSensor::getA2DReading(void) {
    std::ifstream voltageFile(VOLTAGE_FILE);
    if (voltageFile.is_open()) {
        unsigned int voltage;
        voltageFile >> voltage;
        voltageFile.close();
        return voltage;
    } else {
        std::cerr << "Error opening voltage file" << std::endl;
    }
    return 0;
}
