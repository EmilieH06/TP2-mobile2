#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"
#include "MatrixDisplay.h"
#include <TM1637Display.h>
#include <ESP32Servo.h>

unsigned long now = 0;

Servo servo;

void setup() {
  Serial.begin(9600);
  
  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  
  SetupDisplay();
  SetupMatrixDisplay();

  servo.attach(SERVO_PIN);
  servo.write(90);
}

void loop() {
  now = millis();

  if (digitalRead(BTN_A_PIN) == LOW) {
    delay(50);

    if (digitalRead(BTN_A_PIN) == LOW) {
      Serial.write("pressed");
      
      while (digitalRead(BTN_A_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
  if (digitalRead(BTN_B_PIN) == LOW) {
    delay(50);

    if (digitalRead(BTN_B_PIN) == LOW) {
      Serial.write("PressedB");
      
      while (digitalRead(BTN_B_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
}