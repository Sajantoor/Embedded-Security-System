#ifndef _MOTION_SENSOR_HPP_
#define _MOTION_SENSOR_HPP_

/**
 * Motion sensor interface
 */
class MotionSensor {
  public:
    static bool isMotionDetected(void);

  private:
    static unsigned int getA2DReading(void);
};

#endif