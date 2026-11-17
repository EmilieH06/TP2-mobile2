#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>

extern Adafruit_NeoMatrix matrix;

void setupMatrixDisplay();

void changeColor(Color color);
void flashColor(Color color);
void updateFlashColor();