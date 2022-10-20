#include <Arduino.h>
#include "config.h"
#include "Motor.h"
#include "Movement.h"

Movement movement;

void setup() {
  Serial.begin(115200);
  Serial.println("hola!");

  pinMode(STBY_A,OUTPUT);
  pinMode(STBY_B,OUTPUT);
  digitalWrite(STBY_A,HIGH);
  digitalWrite(STBY_B,HIGH);
  movement.begin();
}

void loop() {
  //delay(50);
  if (Serial.available())
  {
    int a = Serial.parseInt();
    int b = Serial.parseInt();
    if(a==0 && b==0)
      return;
    movement.write(a-1000,b-1000);
    //Serial.parseInt();
    
  }
}
