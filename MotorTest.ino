#include <Arduino.h>
#include "config.h"
#include "Motor.h"
#include "Movement.h"
#include <Servo.h>

#define MAX_SERVO_1 110
#define MAX_SERVO_2 150

#define MIN_SERVO_2 10
#define MIN_SERVO_1 0

#define VEL_SERVO_1 2
#define VEL_SERVO_2 2

// COSTANTI DI CONFIGURAZIONE
const int DELTA_LAMBDA = 20;      // semi-ampiezza banda modalità pivot DA OTTIMIZZARE
const int DELTA_NU = 16;          // semi-ampiezza banda avanzamento rettilineo DA OTTIMIZZARE
const int DELTA_NU0 = 4;          // semi-ampiezza banda morta modalità pivot DA OTTIMIZZARE

//// Fattori di scala per il mapping ////
const int FACTOR_MAPPING_PIVOT = (100 - DELTA_NU0);     /// TODO aggiungere descrizioni
const int MAX_ADV_N = (100 - DELTA_LAMBDA);             /// TODO aggiungere descrizioni
const int FACTOR_MAPPING_DRIVE = MAX_ADV_N;             /// TODO aggiungere descrizioni

const float N_MAX = 255;          // RPM massima velocità a vuoto del motore


Movement movement;

Servo myservo1,myservo2;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int refMotorL = 0, refMotorR = 0;


int pos1 = MIN_SERVO_1;    // variable to store the servo position
int pos2 = MIN_SERVO_2;
bool rising1 = true;
bool rising2 = true;


//la funzione map() NON è ottimale per i numeri reali (float), quindi è stata creata la seguente:
float map_float(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void mappaPivot(int lambda, int nu, int *n_MFL_ref, int *n_MFR_ref)
{
    int delta_n;
    if(nu < -DELTA_NU0)      delta_n = nu + DELTA_NU0;
    else if(nu > +DELTA_NU0) delta_n = nu - DELTA_NU0;
    else                         delta_n = 0;

    *n_MFL_ref = delta_n/2;

    *n_MFR_ref = -*n_MFL_ref;		// AG un motore gira al contrario rispetto all'altro

    *n_MFL_ref = map_float(*n_MFL_ref, -FACTOR_MAPPING_PIVOT / 2, FACTOR_MAPPING_PIVOT / 2 , -N_MAX / 2, N_MAX / 2);      // normalizzazione dei dati MAX 50% in pivot
    *n_MFR_ref = map_float(*n_MFR_ref, -FACTOR_MAPPING_PIVOT / 2, FACTOR_MAPPING_PIVOT / 2, -N_MAX / 2, N_MAX / 2);       // normalizzazione dati MAX 50% in pivot
}

void mappaDrive(int lambda, int nu, int *n_MFL_ref, int *n_MFR_ref)
{
    int delta_n,n_i;


    if(nu < -DELTA_NU)      delta_n = nu + DELTA_NU;
    else if(nu > +DELTA_NU) delta_n = nu - DELTA_NU;
    else                        delta_n = 0;


    if(lambda < -DELTA_LAMBDA)      n_i = lambda + DELTA_LAMBDA;
    else if(lambda > +DELTA_LAMBDA) n_i = lambda - DELTA_LAMBDA;
    else                                n_i = 0;

    *n_MFL_ref = n_i + delta_n/2;

    *n_MFL_ref = constrain(*n_MFL_ref, -MAX_ADV_N, MAX_ADV_N);

    *n_MFR_ref = n_i - delta_n/2;

    *n_MFR_ref = constrain(*n_MFR_ref, -MAX_ADV_N, MAX_ADV_N);

    *n_MFL_ref = map_float(*n_MFL_ref, -FACTOR_MAPPING_DRIVE, FACTOR_MAPPING_DRIVE, -N_MAX, N_MAX);               // normalizzazione dei dati
    *n_MFR_ref = map_float(*n_MFR_ref, -FACTOR_MAPPING_DRIVE, FACTOR_MAPPING_DRIVE, -N_MAX, N_MAX);               // normalizzazione dei dati
}





void setup() {
  Serial.begin(9600);
  Serial.println("hola!");
  myservo1.attach(10);
  myservo2.attach(11); 
  movement.begin();
}
void loop() {
  delay(50);
  int lambda, nu;
  String readString;
  int serialData;

if(rising1)
  {
    pos1 += VEL_SERVO_1;
    if(pos1 >= MAX_SERVO_1) {Serial.println("scendo1");rising1 = false;}
  } else {
    pos1 -= VEL_SERVO_1;
    if(pos1 <= MIN_SERVO_1) {Serial.println("salgo1");rising1 = true;}
  }
  if(rising2)
  {
    pos2 += VEL_SERVO_2;
    if(pos2 >= MAX_SERVO_2) {Serial.println("scendo2");rising2 = false;}
  } else {
    pos2 -= VEL_SERVO_2;
    if(pos2 <= MIN_SERVO_2) {Serial.println("salgo2");rising2 = true;}
  }
  myservo1.write(pos1);
  myservo2.write(pos2);



  if (Serial.available())
  {
    lambda = Serial.parseInt(); 
    nu = Serial.parseInt();
    
/*
    Serial.println("received!");
    //Serial.print("A: ");
    Serial.print("Lambda: ");
    Serial.print(lambda);
    Serial.print(" nu: ");
    Serial.println(nu);*/

    if(lambda == 0 && nu == 0) return;
        //// MAPPATURA
      if (abs(lambda) < DELTA_LAMBDA)     // mappatura pivot, ruoto su me stesso
      {Serial.println("pivot");
          mappaPivot(lambda,nu, &refMotorL,&refMotorR);}
      else                                        // mappatura drive, cammino ed eventualmente ruoto
          mappaDrive(lambda,nu,&refMotorL,&refMotorR);
/*
    Serial.print("motoresx = ");
    Serial.print(refMotorL);
    Serial.print("    motoredx = ");
    Serial.println(refMotorR);*/
    // scrivo sui motori
    movement.write(refMotorL,refMotorR);
  } 

}
