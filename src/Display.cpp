#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"
#include "MatrixDisplay.h"
#include <TM1637Display.h>

const uint8_t showC = { // C
  SEG_A | SEG_F | SEG_E | SEG_D
};

TM1637Display display(CLK_PIN, DIO_PIN);

void SetupDisplay() {
  display.setBrightness(0x0f);
  // luminosité : 0x00 (faible) à 0x0f (maximum)
  display.clear();
}

void Show() {
  ChangeColor(1);

  uint8_t data[] = {
    display.encodeDigit(1),
    display.encodeDigit(1),
    display.encodeDigit(1),
    showC
  };

  display.setSegments(data);
}

void ShowDisplay() {
  if(Serial.read()) {
    Show();
  }
  else {
    Serial.write("Un problème est survenue");
    delay(DELAY);
  }
}
