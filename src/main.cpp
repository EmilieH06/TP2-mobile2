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

  connectWifi();//
  connectDatabase();//

  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  
  setupDisplay();
  setupMatrixDisplay();
  setupServo();
}

void updateServo() {
  if ((now - doorMoveMillis) < SERVO_180_TIME) {
    isVaultDoorMoving = true;
    doorVaultMove(currentState, isVaultDoorMoving);
  }
  else {
    isVaultDoorMoving = false;
    doorVaultMove(currentState, isVaultDoorMoving);
  }
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
      flashColor(ORANGE);

      if (combinationTyper()) {
        for (int i = 0; i < 4; i++) {
          vaultCode[i] = combination[i];
        }
        currentState = PROG_CONFIRMATION;

        resetCombinationTyper();
      }
      break;

    case PROG_CONFIRMATION:
      flashColor(ORANGE);

      if (combinationTyper()) {
        bool isCorrectCode = true;

        for (int i = 0; i < 4; i++) {
          if (vaultCode[i] != combination[i]) {
            isCorrectCode = false;
            break;
          }
        }
        if (isCorrectCode) {
          isValid = true;
          wrongCodeCounter = 0;
          // 3 clignotements verts
          flashColor(VERT);
          add(CHANGEMENT_CODE);//
          currentState = OUVERT;
        }
        else {
          // 3 clignotements rouges
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
      changeColor(VERT);

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
        add(FERMETURE);//
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
      changeColor(ROUGE);
      
      // Afficheur : code
      if (combinationTyper()) {
        //function
        //si bon code
        // vert
        //currentState = OUVERT;
        //sinon 
        // clignotements rouges
        //currentState = FERME;
        //

        isValid = true;
        buttonAPressed = false;
        flashColor(VERT);
        doorMoveMillis = millis();
        add(OUVERTURE);//
        currentState = OUVERT;
        resetCombinationTyper();
      }
      break;
  }
}

void loop() {
  now = millis();
  updateFlashColor();
  //updateServo();
  updateCurrentState();
}
