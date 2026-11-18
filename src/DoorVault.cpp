#include <Arduino.h>
#include "Configuration.h"
#include <ESP32Servo.h>

Servo servo;

//Setup du servo moteur lors du démarrage
void setupServo() {
  servo.attach(SERVO_PIN);
  servo.write(90);
}

//Rotationne le servo selon son state
void doorVaultMove(State currentState, bool isVaultDoorMoving) {
  //S'il faut rotationner la porte du coffre
  if (isVaultDoorMoving) {
    //Rotationne à 180 s'il est fermé
    if (currentState == FERME) {
      servo.write(180);
    }
    //Sinon rotationne à 0
    else {
      servo.write(0);
    }
  }
  //Sinon on le laisse stable
  else {
    servo.write(90);
  }
}