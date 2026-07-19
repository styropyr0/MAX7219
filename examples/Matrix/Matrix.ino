#include <MAX7219.h>

#define CS_PIN D8

MAX7219Settings settings(MAX7219DecodeMode::None, 0x03, 0x07, false, false);  // Or you can use setPreset(MAX7219_MATRIX_MODE);
MAX7219 controller;

void setup() {
  Serial.begin(115200);
  controller.begin(CS_PIN);
  controller.setSettings(settings);
}

void drawHi() {
  uint8_t hi[] = { 0x00, 0x3E, 0x08, 0x08, 0x3E, 0x00, 0x3A, 0x00 };
  for (uint8_t i = 1; i <= 8; i++)
    wtd(i, hi[i - 1]);
}

void drawHeart() {
  const uint8_t heart[] PROGMEM = { 0x0E, 0x1F, 0x3F, 0x7E, 0x7E, 0x3F, 0x1F, 0x0E };
  for (uint8_t i = 1; i <= 8; i++)
    wtd(i, heart[i - 1]);
}

void wtd(uint8_t column, uint8_t data) {
  controller.setColumn(column, data);
}

void delayedCall(void (*func)()) {
  delay(1000);
  func();
}

void printPowerDissipation() {
  Serial.print("Power Dissipation: ");
  Serial.println("" + (String)controller.computePowerDissipation(5.0f, MAX7219LEDColor::Red) + "W");
}

void loop() {
  delayedCall(&drawHi);
  printPowerDissipation();
  delayedCall(&drawHeart);
  printPowerDissipation();
}