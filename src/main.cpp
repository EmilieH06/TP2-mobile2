#include <Arduino.h>
#include "Configuration.h"
#include "Secret.h"
#include "Display.h"
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

  //connectWifi();//
  //connectDatabase();//

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

void loop() {
  now = millis();
  updateFlashColor();
  //updateServo();

  switch (currentState) {
    case NON_PROGRAMME:
      if (digitalRead(BTN_B_PIN) == LOW && !buttonBPressed) {
        delay(50);

        if (digitalRead(BTN_B_PIN) == LOW) {
          Serial.write("NON_PROGRAMME B");
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
      flashColor(false); //orange

      if (combinationTyper()) {
        for (int i = 0; i < 4; i++) {
          vaultCode[i] = combination[i];
        }
        currentState = PROG_CONFIRMATION;

        resetCombinationTyper();
      }
      break;

    case PROG_CONFIRMATION:
      flashColor(false); //orange

      // Afficheur : code
      if (combinationTyper()) {
        //function
        //si codes différents 
        // 3 clignotements rouges
        //currentState = PROG_SAISIE;
        //sinon codes identiques
        // 3 clignotements verts
        //currentState = OUVERT;
        //

        isValid = true;
        flashColor(isValid);
        //add("CHANGEMENT_CODE");// //journalstate
        currentState = OUVERT;
        resetCombinationTyper();
      }
      break;

    case OUVERT:
      changeColor(true); //vert
      // Afficheur: vide

      if (digitalRead(BTN_A_PIN) == LOW) {
        delay(50);

        if (digitalRead(BTN_A_PIN) == LOW) {
          Serial.write("OUVERT A");
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
        //add("FERMETURE");// //journalstate
        doorMoveMillis = millis();
        currentState = FERME;
      }

      if (digitalRead(BTN_B_PIN) == LOW && !buttonBPressed) {
        delay(50);

        if (digitalRead(BTN_B_PIN) == LOW) {
          Serial.write("OUVERT B");
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
      changeColor(false); //rouge
      
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
        flashColor(isValid);
        doorMoveMillis = millis();
        //add("OUVERTURE");// //journalstate
        currentState = OUVERT;
        resetCombinationTyper();
      }
      break;
  }
}
