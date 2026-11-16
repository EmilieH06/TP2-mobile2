#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"

int combinationStep = 0;

int combination[4] = {0, 0, 0, 0};

bool buttonACodePressed = false;
bool buttonBCodePressed = false;

void updateCombinationDisplay() {
  uint8_t data[4];

  for (int i = 0; i <= combinationStep && i < 4; i++) {
    data[i] = display.encodeDigit(combination[i]);
  }

  display.setSegments(data);
}

void incrementNumber() {
  if (combinationStep < 4) {
    combination[combinationStep] += 1;

    if (combination[combinationStep] > 9) {
      combination[combinationStep] = 0;
    }

    updateCombinationDisplay();
  }
}

void resetCombinationTyper() {
  combinationStep = 0;

  for (int i = 0; i < 4; i++) {
    combination[i] = 0;
  }

  buttonACodePressed = false;
  buttonBCodePressed = false;

  updateCombinationDisplay();
}

bool combinationTyper() {
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

  if (digitalRead(BTN_B_PIN) == LOW && !buttonBCodePressed) {
    delay(50);

    if (digitalRead(BTN_B_PIN) == LOW) {
      buttonBCodePressed = true;
      combinationStep++;

      if (combinationStep >= 4) {
        return true;
      }
      while (digitalRead(BTN_B_PIN) == LOW) {
        delay(10);
      }
      delay(50);

      updateCombinationDisplay();
    }
  }

  if (digitalRead(BTN_B_PIN) == HIGH) {
    buttonBCodePressed = false;
  }

  return false;
}
