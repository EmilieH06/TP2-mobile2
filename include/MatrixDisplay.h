#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>

extern Adafruit_NeoMatrix matrix;
extern bool isFlashing;

void setupMatrixDisplay();
void changeColor(Color color);
void continuousFlash(Color color);
void flashColor(Color color);
void updateFlashColor();