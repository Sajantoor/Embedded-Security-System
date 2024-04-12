#ifndef _BUZZER_HPP_
#define _BUZZER_HPP_

#include <atomic>
#include <functional>
#include <thread>

/**
 * Class to handle the buzzer sound
*/
class Buzzer {
  private:
    void updatePwm(unsigned int period, unsigned int dutyCycle);
    std::atomic<bool> playSound = false;
    bool isStopped = false;
    std::thread buzzerThread;

  public:
    Buzzer(void);

    // stops the buzzer thread
    void stop(void);

    // turns off the buzzer sound
    void disableBuzzerSound();

    // plays the buzzer sound
    void buzz();
};

#endif
