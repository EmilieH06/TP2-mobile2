#include <Arduino.h>
#include "Configuration.h"
#include <TM1637Display.h>

extern TM1637Display display;
extern const uint8_t showC;
extern const uint8_t showH;

void SetupDisplay();

void ShowTemperature();
void ShowHumidity();
void ShowDisplay();