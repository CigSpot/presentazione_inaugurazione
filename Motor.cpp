#include "Motor.h"

/**
 * Constructor for the Motor object.
 * 
 * This constructor simply saves the pins used by the motor, 
 * without further initializing them or the motor.
 * 
 * @param pwm pin for the pwm control
 * @param in1 pin connected to input 1
 * @param in2 pin connected to input 2
 */
Motor::Motor(byte pwm, byte in1, byte in2) {
  this->pwm = pwm;
  this->in1 = in1;
  this->in2 = in2;
}

/**
 * Initializes the motor.
 * 
 * Method that initializes the pins used by the motor, 
 * and then call Robot::stop to make sure the motor doesn't move.
 * 
 * It's necessary for this to be separated from the constructor 
 * because the constructor could be potentially executed before
 * everything necessary to interact with the hardware is ready. 
 * Instead a separated function can only be called when the user 
 * is allowed to call a function, like inside the setup in Arduino.
 */
void Motor::begin() {
  pinMode(pwm, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  stop();
}

/**
 * Starts the motor.
 * 
 * Method used to start the rotation of a motor with a given speed and direction.
 * By default the motor turns clockwise, using the invert parameter we can turn it
 * counterclockwise.
 * 
 * @param speed A value from 0 to 255 indicating the motor speed.
 * @param invert Direction of the motor, when set to false the motor turns clockwise.
 */
void Motor::go(int speed, bool invert) {
  analogWrite(pwm, speed);
  digitalWrite(in1, invert);
  digitalWrite(in2, !invert);
}

/**
 * Stops the motor.
 * 
 * Method for stopping the motor, using the "short brake" mode of the TB6612FNG motor
 * driver. This should put the driver output in a low impedence mode, making the motor
 * slow down more than bu just disconnecting it.
 */
void Motor::stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
