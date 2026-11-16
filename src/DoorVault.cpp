#include <Arduino.h>
#include "Configuration.h"
#include <ESP32Servo.h>

Servo servo;

void SetupServo() {
  servo.attach(SERVO_PIN);
  servo.write(90);
}

void DoorVaultMove(State currentState, bool isVaultDoorMoving) {
  if (isVaultDoorMoving && currentState == FERME) {
    servo.write(180);
  } else if (isVaultDoorMoving && currentState != FERME) {
    servo.write(0);
  }
  else {
    servo.write(90);
  }
}