#include "Movement.h"

/**
 * Initializes the Movement class.
 * 
 * Method that initializes the 4 motors used by the robot.
 * 
 * It's necessary for this to be separated from the constructor 
 * because the constructor could be potentially executed before
 * everything necessary to interact with the hardware is ready. 
 * Instead a separated function can only be called when the user 
 * is allowed to call a function, like inside the setup in Arduino.
 */
void Movement::begin() {
	front_left->begin();
	front_right->begin();
	rear_left->begin();
	rear_right->begin();	
}

void Movement::write(int motorLeft, int motorRight) {
  front_left->go(abs(motorLeft),motorLeft>0);
  rear_left->go(abs(motorLeft),motorLeft>0);
  
  front_right->go(abs(motorRight),motorRight>0);
  rear_right->go(abs(motorRight),motorRight>0);
}

/*
 * Makes the robot move in a straight line.
 *
 * @param direction set the direction of the movement (1 forward, 0 backwards)
 * @param speed set the speed of the movement
 * @param distance set the distance to cover
 
 */
void Movement::go(bool direction, int speed, int distance) {
  unsigned long timeOfStart;

  front_left->go(speed,direction);
  front_right->go(speed,direction);
  rear_left->go(speed,direction);
  rear_right->go(speed,direction);
  
  /*
   * AG - @Marco don't kill me.
   * I'll wait for Mike to implement his magic 
   * formulas to integrate the acceleration and get the distance.
   * 
   * at the moment i'll just use distance as the number of seconds to keep going
  */
  Serial.println("start going forward!");
  timeOfStart = millis();
  while((millis() - timeOfStart) < (distance*1000));
  Serial.println("stop going forward!");


  front_left->stop();
  front_right->stop();
  rear_left->stop();
  rear_right->stop();
}

/*
 * Makes the robot turn while not displacing.
 *
 * @param direction set the direction of the rotation (1 left, 0 right)
 * @param speed set the speed of the movement
 * @param theta set the angle to cover 
 */
void Movement::turn(bool direction, int speed, int theta) {
  unsigned long timeOfStart;

  front_left->go(speed,direction);
  front_right->go(speed,!direction);
  rear_left->go(speed,direction);
  rear_right->go(speed,!direction);
  
  /*
   * AG - @Marco don't kill me.
   * I'll wait for Mike to implement his magic 
   * formulas to integrate the acceleration and get the distance.
   * 
   * at the moment i'll just use distance as the number of seconds to keep turning
  */
    Serial.println("start turning!");

  timeOfStart = millis();
  while((millis() - timeOfStart) < (theta*1000));
Serial.println("stop turning!");

  front_left->stop();
  front_right->stop();
  rear_left->stop();
  rear_right->stop();
}

/*
 * Makes the robot move in a curved line.
 * 
 * toDo implement mathematical formulas to get this to work
 *
 * @param avance_direction set the direction of the movement (1 forward, 0 backwards)
 * @param turn_direction set the direction of the rotation (1 left, 0 right)
 * @param speed set the speed of the movement
 * @param distance set the distance to cover
 * @param theta set the angle to cover 
 *  
 */
void Movement::goTurning(bool avance_direction, bool turn_direction, int speed, int distance, int theta) {
  unsigned long timeOfStart;

  /*
   * AG - @Marco don't kill me
   * At the moment I just set the speed of the internal motors to half the total speed and just go
   * I'll wait for Mike to implement his magic formulas.
  */

  front_left->go(turn_direction?speed:(speed/2),avance_direction);
  front_right->go(turn_direction?speed:(speed/2),avance_direction);
  rear_left->go(turn_direction?(speed/2):speed,avance_direction);
  rear_right->go((turn_direction)?(speed/2):speed,avance_direction);
  

  /*
   * AG - @Marco don't kill me.
   * I'll wait for Mike to implement his magic 
   * formulas to integrate the acceleration and get the distance.
   * 
   * at the moment i'll just use distance as the number of seconds to keep going
  */
Serial.println("start go turning!");
  timeOfStart = millis();
  while((millis() - timeOfStart) < (distance*1000));
Serial.println("stop go turning!");
  front_left->stop();
  front_right->stop();
  rear_left->stop();
  rear_right->stop();
}
