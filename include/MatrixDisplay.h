#include <Arduino.h>
#include "Configuration.h"
#include <Adafruit_NeoMatrix.h>

extern Adafruit_NeoMatrix matrix;

void SetupMatrixDisplay();

void ChangeColor(bool valid);
void FlashColor(bool valid);
void UpdateFlashColor();