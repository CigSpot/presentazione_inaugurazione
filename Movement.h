#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>
#include "IMU.h"
#include "Motor.h"
#include "config.h"


#define FORWARD   1
#define BACKWARD  0
#define LEFT      1
#define RIGHT     0

#define MAX_SPEED 255

/**
 * @file
 * @version 1.0
 * @author Andrea Grillo
 * 
 * Class to control the whole robot.
 */
class Movement {
  public:
    //Movement();
    void begin();
    void go(bool direction, int speed, int distance);
    void turn(bool direction, int speed, int theta);
    void write(int motorLeft, int motorRight);
    void goTurning(bool avance_direction, bool turn_direction, int speed, int distance, int theta);
  private:
    Motor *rear_right = new Motor(M1_PWM, M1_IN1, M1_IN2); //RR
    Motor *front_right = new Motor(M2_PWM, M2_IN1, M2_IN2); //FR
    Motor *front_left = new Motor(M3_PWM, M3_IN1, M3_IN2); //FL
    Motor *rear_left = new Motor(M4_PWM, M4_IN1, M4_IN2); //RL
    IMU imu;


};

#endif
