#include <Arduino.h>
#include <TM1637Display.h>

extern TM1637Display display;
extern int combination[4];

void setupDisplay();
void incrementNumber();
bool combinationTyper();
void resetCombinationTyper();
void clearCombinationTyper();
void updateCombinationDisplay();