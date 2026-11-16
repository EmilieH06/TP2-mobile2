#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>

extern Adafruit_NeoMatrix matrix;

void setupMatrixDisplay();

void changeColor(bool valid);
void flashColor(bool valid);
void updateFlashColor();