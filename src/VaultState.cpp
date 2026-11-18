#include <Arduino.h>
#include "Configuration.h"
#include "Store.h"
#include "CodeType.h"
#include "DoorVault.h"
#include "MatrixDisplay.h"

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

//Pour savoir s'il faut rotationner la porte du servo
void updateServo() {
  //Si c'est un dessous du temps que la porte peut se rotationné, elle rotationne
  if ((now - doorMoveMillis) < SERVO_180_TIME) {
    isVaultDoorMoving = true;
  }
  //Sinon elle ne rotationne pas
  else {
    isVaultDoorMoving = false;
  }
  doorVaultMove(currentState, isVaultDoorMoving);
}

//Pour comparer si le code que l'on vient de taper est le même que lui de notre coffre
bool compareCodes() {
  for (int i = 0; i < 4; i++) {
    if (vaultCode[i] != combination[i]) {
      return false;
    }
  }
  return true;
}

//État du coffre non programmé
void notProgrammed() {
  //Peser sur le bouton B
  if (digitalRead(BTN_B_PIN) == LOW && !buttonBPressed) {
    delay(50);
    //Mettre l'état pesé du bouton à vrai
    if (digitalRead(BTN_B_PIN) == LOW) {
      actionMillis = millis();
      buttonBPressed = true;
    }
  }
  //Si le bouton B est pesé 3 secondes de temps, on change d'état
  if (digitalRead(BTN_B_PIN) == LOW && buttonBPressed && (millis() - actionMillis) > 3000) {
    //Mettre l'état pesé du bouton à vrai
    buttonBPressed = false;
    currentState = PROG_SAISIE;
  }
  //Remettre l'état pesé du bouton B à faux
  if (digitalRead(BTN_B_PIN) == HIGH) {
    buttonBPressed = false;
  }
}

//État du coffre pour entrer une première fois le code
void enteringCode() {
  //Faire que la matrice flash orange tout le long de la saisie
  continuousFlash(ORANGE);

  //Taper le code du coffre et garder le code en mémoire
  if (combinationTyper()) {
    for (int i = 0; i < 4; i++) {
      vaultCode[i] = combination[i];
    }
    //Changer d'état et réinitialiser le display
    currentState = PROG_CONFIRMATION;
    resetCombinationTyper();
  }
}

//État du coffre pour confirmer notre code
void confirmingCode() {
  //Faire que la matrice flash orange tout le long de la programmation
  continuousFlash(ORANGE);

  //Taper le code du coffre et s'assurer qu'il est le même que celui gardé en mémoire
  if (combinationTyper()) {
    //Comparer les codes
    if (compareCodes()) {
      //Si le code est valide
      isValid = true;
      wrongCodeCounter = 0;
      flashColor(VERT);
      add(CHANGEMENT_CODE);
      //Changement d'état
      currentState = OUVERT;
    }
    else {
      //Si le code est invalide
      flashColor(ROUGE);
      wrongCodeCounter += 1;
      //Après le troisième essaie, on retourne en état de saisie
      if (wrongCodeCounter >= 3) {
        wrongCodeCounter = 0;
        //Changement d'état
        currentState = PROG_SAISIE;
      }
    }
    //Réinitialiser le display
    resetCombinationTyper();
  }
}

//État du coffre ouvert
void opened() {
  //Changer la couleur de la matrice seulement quand la matrice ne flash pas
  if (!isFlashing) {
    changeColor(VERT);
  }

  //Peser sur le bouton A
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
  //Si le bouton A est pesé, on ferme le coffre
  if (digitalRead(BTN_A_PIN) == HIGH && buttonAPressed) {
    buttonAPressed = false;
    add(FERMETURE);
    //On ferme la porte
    doorMoveMillis = millis();
    //Changement d'état
    currentState = FERME;
  }

  //Peser sur le bouton B
  if (digitalRead(BTN_B_PIN) == LOW && !buttonBPressed) {
    delay(50);
    //Mettre l'état pesé du bouton à vrai
    if (digitalRead(BTN_B_PIN) == LOW) {
      actionMillis = millis();
      buttonBPressed = true;
    }
  }
  //Si le bouton B est pesé 3 secondes de temps, on change d'état
  if (digitalRead(BTN_B_PIN) == LOW && buttonBPressed && (millis() - actionMillis) > 3000) {
    isValid = false;
    //Mettre l'état pesé du bouton à vrai
    buttonBPressed = false;
    //Changement d'état
    currentState = PROG_SAISIE;
  }
  //Remettre l'état pesé du bouton B à faux
  if (digitalRead(BTN_B_PIN) == HIGH) {
    buttonBPressed = false;
  }
}

//État du coffre fermé
void closed() {
  //Changer la couleur de la matrice seulement quand la matrice ne flash pas
  if (!isFlashing) {
    changeColor(ROUGE);
  }
  
  //Taper le code du coffre et s'assurer qu'il est le même que celui gardé en mémoire
  if (combinationTyper()) {
    //Comparer les codes
    if (compareCodes()) {
      //Si le code est valide
      isValid = true;
      flashColor(VERT);
      add(OUVERTURE);
      //On ouvre la porte
      doorMoveMillis = millis();
      //Changement d'état
      currentState = OUVERT;
    }
    else {
      //Si le code est invalide
      flashColor(ROUGE);
    }
    resetCombinationTyper();
  }
}

//Pour faire des événements selon l'état qu'on est rendu
void updateCurrentState() {
  now = millis();

  //Changement d'événement
  switch (currentState) {
    case NON_PROGRAMME:
      notProgrammed();
      break;
    case PROG_SAISIE:
      enteringCode();
      break;
    case PROG_CONFIRMATION:
      confirmingCode();
      break;
    case OUVERT:
      opened();
      break;
    case FERME:
      closed();
      break;
  }
}
