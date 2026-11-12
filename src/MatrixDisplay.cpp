#include <Arduino.h>
#include "Configuration.h"
#include "MatrixDisplay.h"
#include <Adafruit_NeoMatrix.h>

Adafruit_NeoMatrix matrix(8, 8, MATRIX_PIN,
NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS +
NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

void SetupMatrixDisplay() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(0);
  matrix.show();
}

void ChangeColor(int value) {
  //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
  matrix.fillScreen(matrix.Color(255, 100, 0));
  matrix.show();
}