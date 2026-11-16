#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"
#include "MatrixDisplay.h"
#include <TM1637Display.h>

TM1637Display display(CLK_PIN, DIO_PIN);

void setupDisplay() {
  display.setBrightness(0x0f);
  // luminosité : 0x00 (faible) à 0x0f (maximum)
  display.clear();
}

void show() {
  changeColor(false);

  uint8_t data[] = {
    display.encodeDigit(1),
    display.encodeDigit(1),
    display.encodeDigit(1),
    display.encodeDigit(1)
  };

  display.setSegments(data);
}

void showDisplay() {
  if(Serial.read()) {
    show();
  }
  else {
    Serial.write("Un problème est survenue");
    delay(DELAY);
  }
}
