#pragma once

/**
 * Motion sensor interface
 */
class MotionSensor {
  public:
    static bool isMotionDetected(void);

  private:
    static unsigned int getA2DReading(void);
};