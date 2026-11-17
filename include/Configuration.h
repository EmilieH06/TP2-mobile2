#include <Arduino.h>

#define CLK_PIN 19
#define DIO_PIN 23

#define BTN_A_PIN 27
#define BTN_B_PIN 16

#define MATRIX_PIN 5
#define BRIGHTNESS 10

#define SERVO_PIN 25

#define DELAY 2000

#define SERVO_180_TIME 300

enum State {
  NON_PROGRAMME,
  PROG_SAISIE,
  PROG_CONFIRMATION,
  OUVERT,
  FERME
};

enum Color {
  VERT,
  ORANGE,
  ROUGE,
  VIDE
};

enum Journal {
  OUVERTURE,
  FERMETURE,
  CHANGEMENT_CODE
};
