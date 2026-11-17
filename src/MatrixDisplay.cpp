#include <Arduino.h>
#include "Configuration.h"
#include "MatrixDisplay.h"
#include <Adafruit_NeoMatrix.h>

Adafruit_NeoMatrix matrix(8, 8, MATRIX_PIN,
NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS +
NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

unsigned long previousMillisColor = 0;
const unsigned long intervalColor = 500;

int flashStep = 0;
bool isFlashing = false;
Color colorFlash = VERT;

void setupMatrixDisplay() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(matrix.Color(0, 255, 0));
  matrix.show();
}

void changeColor(Color color) {
  switch (color) {
    case VERT:
      //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
      matrix.fillScreen(matrix.Color(0, 255, 0));
      break;
    case ORANGE:
      //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
      matrix.fillScreen(matrix.Color(255, 150, 0));
      break;
    case ROUGE:
      //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
      matrix.fillScreen(matrix.Color(255, 0, 0));
      break;
    case VIDE:
      //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
      matrix.fillScreen(matrix.Color(0, 0, 0));
      break;
  }
  matrix.show();
}

void flashColor(Color color) {
  if (!isFlashing) {
    isFlashing = true;
    colorFlash = color;
    flashStep = 0;
    previousMillisColor = millis();
  }
}

void updateFlashColor() {
  if (isFlashing) {
    unsigned long currentMillisColor = millis();

    if (currentMillisColor - previousMillisColor >= intervalColor) {
      previousMillisColor = currentMillisColor;
      
      switch (flashStep) {
        case 0:
        case 2:
        case 4:
          changeColor(VIDE);
          break;
        case 1:
        case 3:
          changeColor(colorFlash);
          break;
        case 5:
          changeColor(colorFlash);
          isFlashing = false;
          break;
      }
      matrix.show();
      flashStep++;
    }
  }
}