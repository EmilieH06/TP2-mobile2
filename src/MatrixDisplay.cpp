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
bool isValidFlash = false;

void SetupMatrixDisplay() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);
  matrix.fillScreen(matrix.Color(0, 255, 0));
  matrix.show();
}

void ChangeColor(bool valid) {
  if (valid) {
    //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
    matrix.fillScreen(matrix.Color(0, 255, 0));
  }
  else {
    //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
    matrix.fillScreen(matrix.Color(255, 0, 0));
  }
  matrix.show();
}

void FlashColor(bool valid) {
  if (!isFlashing) {
    isFlashing = true;
    isValidFlash = valid;
    flashStep = 0;
    previousMillisColor = millis();
  }
}

void UpdateFlashColor() {
  if (isFlashing) {
    unsigned long currentMillisColor = millis();

    if (currentMillisColor - previousMillisColor >= intervalColor) {
      previousMillisColor = currentMillisColor;
      
      switch (flashStep) {
        case 0:
          //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
          matrix.fillScreen(matrix.Color(0, 0, 0));
          break;

        case 1:
          if (isValidFlash) {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(0, 255, 0));
          }
          else {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(255, 150, 0));
          }
          break;

        case 2:
          //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
          matrix.fillScreen(matrix.Color(0, 0, 0));
          break;

        case 3:
          if (isValidFlash) {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(0, 255, 0));
          }
          else {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(255, 150, 0));
          }
          break;

        case 4:
          //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
          matrix.fillScreen(matrix.Color(0, 0, 0));
          break;

        case 5:
          if (isValidFlash) {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(0, 255, 0));
          }
          else {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(255, 150, 0));
          }
          break;

        case 6:
          if (!isValidFlash) {
            //https://github.com/adafruit/Adafruit_NeoMatrix/blob/master/Adafruit_NeoMatrix.h
            matrix.fillScreen(matrix.Color(255, 0, 0));
          }
          isFlashing = false;
          break;
      }
      matrix.show();
      flashStep++;
    }
  }
}