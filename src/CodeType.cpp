#include <Arduino.h>
#include "Configuration.h"
#include <TM1637Display.h>

TM1637Display display(CLK_PIN, DIO_PIN);

int combinationStep = 0;
int combination[4] = {-1, -1, -1, -1};

bool buttonACodePressed = true;
bool buttonBCodePressed = true;

//Setup du display lors du démarrage
void setupDisplay() {
  display.setBrightness(0x0f);
  display.clear();
}

//Updater la combinaison sur le display
void updateCombinationDisplay() {
  //Change un nombre invalide (qui ne s'affiche pas sur le display) 
  // en nombre valide (qui s'affiche sur le display)
  if (combination[combinationStep] == -1) {
    combination[combinationStep] = 0;
  }

  uint8_t data[4];

  //Écrit la combinaison sur le display si le nombre est valide
  for (int i = 0; i < 4; i++) {
    if (combination[i] != -1) {
      data[i] = display.encodeDigit(combination[i]);
    } else {
      data[i] = 0x00;
    }
  }

  display.setSegments(data);
}

//Incrémenter le nombre où on est rendu
void incrementNumber() {
  if (combinationStep < 4) {
    combination[combinationStep] += 1;

    //Remet le nombre à 0 si on est plus haut que 9
    if (combination[combinationStep] > 9) {
      combination[combinationStep] = 0;
    }
  }
}

//Vide le display
void clearCombinationTyper() {
  display.clear();
}

//Réinitialise les données
void resetCombinationTyper() {
  clearCombinationTyper();
  combinationStep = 0;

  for (int i = 0; i < 4; i++) {
    combination[i] = -1;
  }

  buttonACodePressed = true;
  buttonBCodePressed = true;
}

//Pour entrer la combinaison
bool combinationTyper() {
  //Pèse sur le bouton A et incrémente le nombre où on est rendu
  if (digitalRead(BTN_A_PIN) == LOW && !buttonACodePressed) {
    delay(50);

    if (digitalRead(BTN_A_PIN) == LOW) {
      incrementNumber();
      buttonACodePressed = true;

      while (digitalRead(BTN_A_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
  if (digitalRead(BTN_A_PIN) == HIGH) {
    buttonACodePressed = false;
  }

  //Pèse sur le bouton B et passe au prochain nombre
  if (digitalRead(BTN_B_PIN) == LOW && !buttonBCodePressed) {
    delay(50);
    if (digitalRead(BTN_B_PIN) == LOW) {
      buttonBCodePressed = true;
      combinationStep++;

      //Arrête après 4 nombres tappés
      if (combinationStep >= 4) {
        return true;
      }
      while (digitalRead(BTN_B_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
  if (digitalRead(BTN_B_PIN) == HIGH) {
    buttonBCodePressed = false;
  }

  //Update le display à chaque changement
  updateCombinationDisplay();
  return false;
}
