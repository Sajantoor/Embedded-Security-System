#ifndef _SURVEILLANCE_HPP_
#define _SURVEILLANCE_HPP_

#include <thread>

#include "hal/motionSensor.hpp"
#include "notifier.hpp"

#define MOTION_THRESHOLD 2000
// Have a timeout of 2 minutes
#define MOTION_TIMEOUT 120

class Surveillance {
  public:
    /**
     * Starts the surveillance
     */
    Surveillance(MotionSensor* motionSensor, Notifier* notifier);

    /**
     * Stops the surveillance
     */
    void stop(void);

  private:
    void run(void);
    MotionSensor* motionSensor;
    Notifier* notifier;
    std::thread thread;
    bool isRunning;
};

#endif