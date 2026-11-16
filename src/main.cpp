#include <Arduino.h>
#include "Configuration.h"
#include "Secret.h"
#include "Display.h"
#include "Store.h"
#include "CodeType.h"
#include "MatrixDisplay.h"
#include <TM1637Display.h>
#include <ESP32Servo.h>
#include <WiFi.h>

enum State {
  NON_PROGRAMME,
  PROG_SAISIE,
  PROG_CONFIRMATION,
  OUVERT,
  FERME
};

Servo servo;

bool isValid = true;
bool buttonAPressed = false;
bool buttonBPressed = false;

unsigned long now = 0;
unsigned long actionMillis = 0;

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
  
  SetupDisplay();
  SetupMatrixDisplay();

  servo.attach(SERVO_PIN);
  servo.write(90);
}

void loop() {
  now = millis();
  UpdateFlashColor();

  switch (currentState) {
    case NON_PROGRAMME:
      //servo.write(0);

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
        delay(DELAY);
      }
      if (digitalRead(BTN_B_PIN) == HIGH) {
        buttonBPressed = false;
      }
      break;

    case PROG_SAISIE:
      Serial.write("PROG_SAISIE ");
      FlashColor(false);
      //servo.write(0);
      // Matrice: orange clignotante
      // Afficheur : saisie

      if (combinationTyper()) {
        for (int i = 0; i < 4; i++) {
          vaultCode[i] = combination[i];
        }
        currentState = PROG_CONFIRMATION;

        resetCombinationTyper();
      }
      break;

    case PROG_CONFIRMATION:
      Serial.write("PROG_CONFIRMATION ");
      FlashColor(false);
      //servo.write(0);
      // Matrice: orange clignotante
      // Afficheur : code

      //si codes différents 
      // 3 clignotements rouges
      //currentState = PROG_SAISIE;
      //sinon codes identiques
      // 3 clignotements verts
      //currentState = OUVERT;

      //temp pour tester
      if (digitalRead(BTN_A_PIN) == LOW) {
        delay(50);

        if (digitalRead(BTN_A_PIN) == LOW) {
          Serial.write("FERME A");
          actionMillis = millis();
          buttonAPressed = true;
          
          while (digitalRead(BTN_A_PIN) == LOW) {
            delay(10);
          }
          delay(50);
        }
      }
      if (digitalRead(BTN_A_PIN) == HIGH && buttonAPressed) {
        isValid = true;
        buttonAPressed = false;
        FlashColor(isValid);
        currentState = OUVERT;
      }
      //
      break;

    case OUVERT:
      Serial.write("OUVERT ");
      ChangeColor(true);
      //servo.write(0);
      // Matrice: vert fixe
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
      Serial.write("FERME ");
      ChangeColor(false);
      //servo.write(180);
      // Matrice: rouge fixe
      // Afficheur : code

      //si bon code
      //currentState = OUVERT;
      //sinon 
      // Clignotement rouge
      //currentState = FERME;

      //temp pour tester
      if (digitalRead(BTN_A_PIN) == LOW) {
        delay(50);

        if (digitalRead(BTN_A_PIN) == LOW) {
          Serial.write("FERME A");
          actionMillis = millis();
          buttonAPressed = true;
          
          while (digitalRead(BTN_A_PIN) == LOW) {
            delay(10);
          }
          delay(50);
        }
      }
      if (digitalRead(BTN_A_PIN) == HIGH && buttonAPressed) {
        isValid = true;
        buttonAPressed = false;
        FlashColor(isValid);
        currentState = OUVERT;
      }
      //
      break;
  }
}
