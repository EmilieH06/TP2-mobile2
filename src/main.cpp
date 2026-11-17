#include <Arduino.h>
#include "Configuration.h"
#include "Secret.h"
#include "Store.h"
#include "CodeType.h"
#include "DoorVault.h"
#include "MatrixDisplay.h"
#include <WiFi.h>

bool isValid = true;
bool isVaultDoorMoving = true;
bool buttonAPressed = false;
bool buttonBPressed = false;

unsigned long now = 0;
unsigned long actionMillis = 0;
unsigned long doorMoveMillis = 100000;
unsigned int wrongCodeCounter = 0;

State currentState = NON_PROGRAMME;
int vaultCode[4];

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

  connectWifi();
  connectDatabase();

  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  
  setupDisplay();
  setupMatrixDisplay();
  setupServo();
}

void updateServo() {
  if ((now - doorMoveMillis) < SERVO_180_TIME) {
    isVaultDoorMoving = true;
  }
  else {
    isVaultDoorMoving = false;
  }
  doorVaultMove(currentState, isVaultDoorMoving);
}

bool compareCodes() {
  for (int i = 0; i < 4; i++) {
    if (vaultCode[i] != combination[i]) {
      return false;
    }
  }
  return true;
}

void updateCurrentState() {
  switch (currentState) {
    case NON_PROGRAMME:
      if (digitalRead(BTN_B_PIN) == LOW && !buttonBPressed) {
        delay(50);
        if (digitalRead(BTN_B_PIN) == LOW) {
          actionMillis = millis();
          buttonBPressed = true;
        }
      }
      if (digitalRead(BTN_B_PIN) == LOW && buttonBPressed && (millis() - actionMillis) > 3000) {
        buttonBPressed = false;
        currentState = PROG_SAISIE;
      }
      if (digitalRead(BTN_B_PIN) == HIGH) {
        buttonBPressed = false;
      }
      break;

    case PROG_SAISIE:
      continuousFlash(ORANGE);

      if (combinationTyper()) {
        for (int i = 0; i < 4; i++) {
          vaultCode[i] = combination[i];
        }
        currentState = PROG_CONFIRMATION;
        resetCombinationTyper();
      }
      break;

    case PROG_CONFIRMATION:
      continuousFlash(ORANGE);

      if (combinationTyper()) {
       if (compareCodes()) {
          isValid = true;
          wrongCodeCounter = 0;
          flashColor(VERT);
          add(CHANGEMENT_CODE);
          currentState = OUVERT;
        }
        else {
          flashColor(ROUGE);
          wrongCodeCounter += 1;
          if (wrongCodeCounter >= 3) {
            wrongCodeCounter = 0;
            currentState = PROG_SAISIE;
          }
        }
        resetCombinationTyper();
      }
      break;

    case OUVERT:
      if (!isFlashing) {
        changeColor(VERT);
      }

      if (digitalRead(BTN_A_PIN) == LOW) {
        delay(50);
        if (digitalRead(BTN_A_PIN) == LOW) {
          actionMillis = millis();
          buttonAPressed = true;
          
          while (digitalRead(BTN_A_PIN) == LOW) {
            delay(10);
          }
          delay(50);
        }
      }
      if (digitalRead(BTN_A_PIN) == HIGH && buttonAPressed) {
        buttonAPressed = false;
        add(FERMETURE);
        doorMoveMillis = millis();
        currentState = FERME;
      }

      if (digitalRead(BTN_B_PIN) == LOW && !buttonBPressed) {
        delay(50);
        if (digitalRead(BTN_B_PIN) == LOW) {
          actionMillis = millis();
          buttonBPressed = true;
        }
      }
      if (digitalRead(BTN_B_PIN) == LOW && buttonBPressed && (millis() - actionMillis) > 3000) {
        isValid = false;
        buttonBPressed = false;
        currentState = PROG_SAISIE;
      }
      if (digitalRead(BTN_B_PIN) == HIGH) {
        buttonBPressed = false;
      }
      break;

    case FERME:
      if (!isFlashing) {
        changeColor(ROUGE);
      }
      
      if (combinationTyper()) {
        if (compareCodes()) {
          isValid = true;
          flashColor(VERT);
          add(OUVERTURE);
          doorMoveMillis = millis();
          currentState = OUVERT;
        }
        else {
          flashColor(ROUGE);
        }
        resetCombinationTyper();
      }
      break;
  }
}

void loop() {
  now = millis();
  updateCurrentState();
  updateServo();
  updateFlashColor();
}
