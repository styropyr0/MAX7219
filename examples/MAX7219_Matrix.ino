#include <MAX7219.h>

#define CS_PIN D8
MAX7219Settings settings = MAX7219Settings(MAX7219DecodeMode::None, 0x0A, 0x07, false, false);
MAX7219 controller;

void setup() {
  controller.begin(CS_PIN);
  controller.setSettings(settings);
}

void writeSample() {
  SPI.beginTransaction(settings);
  writeWord(0x01FF);
  SPI.endTransaction();
}

void drawSmiley() {
  writeWord(0x0100);
  writeWord(0x023E);
  writeWord(0x0308);
  writeWord(0x0408);
  writeWord(0x053E);
  writeWord(0x0600);
  writeWord(0x073A);
  writeWord(0x0800);
}

void drawHeart() {
  writeWord(0x010E);
  writeWord(0x021F);
  writeWord(0x033F);
  writeWord(0x047E);
  writeWord(0x057E);
  writeWord(0x063F);
  writeWord(0x071F);
  writeWord(0x080E);
}

void writeWord(uint16_t command) {
  controller.setColumn((command >> 8) & 0xFF, command & 0xFF);
}

void delayedCall(void (*func)()) {
  delay(1000);
  func();
}

void loop() {
  delayedCall(&drawSmiley);
  delayedCall(&drawHeart);
}