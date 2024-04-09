#ifndef _BUZZER_HPP_
#define _BUZZER_HPP_

#include <functional>
#include <thread>

enum BuzzerSounds { HIT, MISS, NONE };

class Buzzer {
   private:
    void updatePwm(unsigned int period, unsigned int dutyCycle);
    void playHitSound(void);
    void playMissSound(void);
    bool sleepWhileCheckingConditon(std::function<bool()> condition,
                                    int timeoutMs);
    bool isStopped;
    BuzzerSounds currentSound;
    std::thread buzzerThread;

   public:
    Buzzer(void);

    // stops the buzzer thread
    void stop(void);

    // turns off the buzzer sound
    void disableBuzzerSound(bool shouldSetSoundToNone = true);

    // plays a buzzer sound
    void buzz(BuzzerSounds sound);
};

#endif
