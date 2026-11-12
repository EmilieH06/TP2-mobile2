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
  if (value > WARM_TEMPERATURE) {
    //Si la valeur (température) est plus grande que WARM_TEMPERATURE, affiche un écran orange

    //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
    matrix.fillScreen(matrix.Color(255, 100, 0));
  }
  else if (value < COLD_TEMPERATURE) {
    //Si la valeur (température) est plus petite que COLD_TEMPERATURE, affiche un écran bleu

    //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
    matrix.fillScreen(matrix.Color(0, 0, 255));
  }
  else {
    //Si la valeur (température) est entre COLD_TEMPERATURE et WARM_TEMPERATURE, affiche un écran vert

    //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
    matrix.fillScreen(matrix.Color(0, 255, 0));
  }
  matrix.show();
}