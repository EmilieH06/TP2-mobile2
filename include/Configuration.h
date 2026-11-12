#include <Arduino.h>

const char* SSID = "";//
const char* MDP = "";//
const char* BROKER = "broker.hivemq.com";
const int PORT = 1883;
const char* TOPIC = "cours/esp32/2435295/mouvement";

#define LED_PIN 25
#define MVT_DETECTOR 5
#define CLK_PIN 19
#define DIO_PIN 23
#define BTN_PIN 17

#define DELAY 2000
#define LED_DELAY 5000
#define COUNTER_DELAY 2000

#define DEL_MAX_INTENSITY 255
#define DEL_MIN_INTENSITY 0
