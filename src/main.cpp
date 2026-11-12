#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"
#include "MatrixDisplay.h"
#include <TM1637Display.h>
#include <ESP32Servo.h>
#include <WiFi.h>

Servo servo;

bool isValid = true;
bool isVaultOpen = true;
unsigned long now = 0;
unsigned long actionMillis = 0;

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.write("Not connected");
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.write("Connected");
  }
}

void setup() {
  Serial.begin(9600);
  //connectWifi();

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
      Serial.write("valid");
      isValid = true;
      isVaultOpen = true;
      actionMillis = millis();
      
      while (digitalRead(BTN_A_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
  if (digitalRead(BTN_B_PIN) == LOW) {
    delay(50);

    if (digitalRead(BTN_B_PIN) == LOW) {
      Serial.write("invalid");
      isValid = false;
      isVaultOpen = false;
      actionMillis = millis();
      
      while (digitalRead(BTN_B_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }

  /*if (now - actionMillis < DELAY || isValid) {
    ChangeColor(isValid);
  }
  else {
    isValid = false;*/
    ChangeColor(isValid);
  //}

  if (now - actionMillis < DELAY && isVaultOpen) {
    //servo.write(180);
    delay(50);
    isVaultOpen = false;
  }
  else if (now - actionMillis < DELAY && !isVaultOpen) {
    //servo.write(0);
    delay(50);
    isVaultOpen = true;
  }
}