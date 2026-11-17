#include <Arduino.h>
#include "Configuration.h"
#include <ESP32Servo.h>

Servo servo;

void setupServo() {
  servo.attach(SERVO_PIN);
  servo.write(90);
}

void doorVaultMove(State currentState, bool isVaultDoorMoving) {
  if (isVaultDoorMoving) {
    if (currentState == FERME) {
      servo.write(180);
    }
    else {
      servo.write(0);
    }
  }
  else {
    servo.write(90);
  }
}