#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"

int combinationStep = 0;

int combination[4] = {-1, -1, -1, -1};

bool buttonACodePressed = true;
bool buttonBCodePressed = true;

void updateCombinationDisplay() {
  if (combination[combinationStep] == -1) {
    combination[combinationStep] = 0;
  }

  uint8_t data[4];

  for (int i = 0; i < 4; i++) {
    if (combination[i] != -1) {
      data[i] = display.encodeDigit(combination[i]);
    } else {
      data[i] = 0x00;
    }
  }

  display.setSegments(data);
}

void incrementNumber() {
  if (combinationStep < 4) {
    combination[combinationStep] += 1;

    if (combination[combinationStep] > 9) {
      combination[combinationStep] = 0;
    }
  }
}

void clearCombinationTyper() {
  display.clear();
}

void resetCombinationTyper() {
  clearCombinationTyper();
  combinationStep = 0;

  for (int i = 0; i < 4; i++) {
    combination[i] = -1;
  }

  buttonACodePressed = true;
  buttonBCodePressed = true;
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
    }
  }
  if (digitalRead(BTN_B_PIN) == HIGH) {
    buttonBCodePressed = false;
  }

  updateCombinationDisplay();
  return false;
}
