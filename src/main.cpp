#include <Arduino.h>
#include "Configuration.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <TM1637Display.h>

TM1637Display display(CLK_PIN, DIO_PIN);

bool actionDetected = false;
unsigned long actionMillis = 0;
unsigned long counterMillis = 0;
unsigned long now = 0;
unsigned int count = 0;

void assurerConnexions() {
  SetupMatrixDisplay();
}

void setup() {
  Serial.begin(9600);
  
  pinMode(BTN_PIN, INPUT_PULLUP);
  
  display.setBrightness(0x0f);
  display.clear();
  display.showNumberDec(count);
}

void loop() {
  assurerConnexions();
  now = millis();

  if (mqtt.connected()) mqtt.loop();

  if (digitalRead(MVT_DETECTOR) == LOW && now - actionMillis > COUNTER_DELAY) {
    actionDetected = false;
    actionMillis = millis();
    Serial.write("Calme");
  }

  if (digitalRead(MVT_DETECTOR) == HIGH && now - actionMillis > COUNTER_DELAY) {
    Serial.write("Mouvement");
    actionMillis = millis();
    actionDetected = true;

    if (now - counterMillis > COUNTER_DELAY) {
      count++;
      counterMillis = millis();
      display.showNumberDec(count);
      publier(count);
    }
  }

  if (now - actionMillis < LED_DELAY || actionDetected) {
    digitalWrite(LED_PIN, DEL_MAX_INTENSITY);
  }
  else {
    digitalWrite(LED_PIN, DEL_MIN_INTENSITY);
  }

  if (digitalRead(BTN_PIN) == LOW) {
    delay(50);

    if (digitalRead(BTN_PIN) == LOW) {
      count = 0;
      display.showNumberDec(count);
      
      while (digitalRead(BTN_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
}