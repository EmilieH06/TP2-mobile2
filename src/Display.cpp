#include <Arduino.h>
#include "Configuration.h"
#include "Display.h"
#include "MatrixDisplay.h"
#include "TemperatureReader.h"
#include <TM1637Display.h>

const uint8_t showC = { // C
  SEG_A | SEG_F | SEG_E | SEG_D
};

const uint8_t showH = { // H
  SEG_F | SEG_E | SEG_G | SEG_B | SEG_C
};

TM1637Display display(CLK_PIN, DIO_PIN);

void SetupDisplay() {
  display.setBrightness(0x0f);
  // luminosité : 0x00 (faible) à 0x0f (maximum)
  display.clear();
}

void ShowTemperature() {
  //https://forum.arduino.cc/t/dht11-temperature-code-library/297878/3
  //Lire la température
  float temp = dht.readTemperature(false);

  //Changer la couleur de la matrice
  ChangeColor(temp);

  int temp10 = (int)round(temp * 10);

  //Remplir les cases du display
  uint8_t data[] = {
    display.encodeDigit((temp10 / 100) % 10),
    display.encodeDigit((temp10 / 10) % 10),
    display.encodeDigit(temp10 % 10),
    showC
  };

  display.setSegments(data);
  //Ajouter les ":" entre le nombre et la décimale
  display.showNumberDecEx(temp10, 0b01000000, false, 3, 0); 
}

void ShowHumidity() {
  //https://forum.arduino.cc/t/dht11-temperature-code-library/297878/3
  //Lire l'humidité
  float hum = dht.readHumidity();
  int hum10 = (int)round(hum * 10);

  //Remplir les cases du display
  uint8_t data[] = {
    display.encodeDigit((hum10 / 100) % 10),
    display.encodeDigit((hum10 / 10) % 10),
    display.encodeDigit(hum10 % 10),
    showH
  };

  display.setSegments(data);
  //Ajouter les ":" entre le nombre et la décimale
  display.showNumberDecEx(hum10, 0b01000000, false, 3, 0); 
}

void ShowDisplay() {
  if(Serial.read()) {
    //Afficher la température et l'humidité en alternance
    ShowTemperature();
    delay(DELAY);

    ShowHumidity();
    delay(DELAY);
  }
  else {
    Serial.write("Un problème est survenue");
    delay(DELAY);
  }
}
