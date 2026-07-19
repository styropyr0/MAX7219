#include <MAX7219.h>

#define CS_PIN D8

MAX7219 controller;

void setup() {
  Serial.begin(115200);

  controller.begin(CS_PIN);
  controller.setPreset(MAX7219_7SEG_MODE);
}

void draw12345678() {
  for (uint8_t i = 1; i <= 8; i++)
    controller.setDigit(i, i - 1);
}

void draw87654321() {
  for (uint8_t i = 1; i <= 8; i++)
    controller.setDigit(i, 8 - i);
}

void delayedCall(void (*func)()) {
  delay(1000);
  func();
}

void printPowerDissipation() {
  Serial.print("Power Dissipation: ");
  Serial.print(controller.computePowerDissipation(5.0f, MAX7219LEDColor::Red));
  Serial.println(" W");
}

void loop() {
  delayedCall(draw12345678);
  printPowerDissipation();

  delayedCall(draw87654321);
  printPowerDissipation();
}