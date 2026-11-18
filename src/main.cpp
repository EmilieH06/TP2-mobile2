#include <Arduino.h>
#include "Configuration.h"
#include "ConnectWifi.h"
#include "Store.h"
#include "CodeType.h"
#include "DoorVault.h"
#include "MatrixDisplay.h"
#include "VaultState.h"

//Pour setup le programme
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

//Update tous nos éléments
void loop() {
  updateCurrentState();
  updateServo();
  updateFlashColor();
}
