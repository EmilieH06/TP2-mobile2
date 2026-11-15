#include <Arduino.h>
#include "Configuration.h"
#include "Secret.h"
#include "Display.h"
#include "Store.h"
#include "MatrixDisplay.h"
#include <TM1637Display.h>
#include <ESP32Servo.h>
#include <WiFi.h>

Servo servo;

bool isValid = true;
bool isVaultOpen = true;
bool buttonPressed = false;
unsigned long now = 0;
unsigned long actionMillis = 0;
unsigned long beginActionMillis = 0;

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

void CheckPressed(bool valid) {
  Serial.write("check");
  bool pressLength = (millis() - actionMillis) > 3000;
  buttonPressed = false;

  if (valid) {
    Serial.write("check valid");
    isValid = true;
    isVaultOpen = true;
    FlashColor(isValid);
  } 
  else if (pressLength && !valid) {
    Serial.write("check invalid");
    isValid = false;
    isVaultOpen = false;
    FlashColor(isValid);
    ChangeColor(isValid);

    add("FERMETURE");//
  }
}

void setup() {
  Serial.begin(9600);

  connectWifi();//
  connectDatabase();//

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
      actionMillis = millis();
      buttonPressed = true;
      
      while (digitalRead(BTN_A_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
  if (digitalRead(BTN_A_PIN) == HIGH && buttonPressed) {
    CheckPressed(true);
  }
  if (digitalRead(BTN_B_PIN) == LOW) {
    delay(50);

    if (digitalRead(BTN_B_PIN) == LOW) {
      Serial.write("invalid");
      actionMillis = millis();
      buttonPressed = true;
      
      while (digitalRead(BTN_B_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
  if (digitalRead(BTN_B_PIN) == HIGH && buttonPressed) {
    CheckPressed(false);
  }

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
  else {
    servo.write(90);
  }
}