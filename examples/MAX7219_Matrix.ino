#include <SPI.h>

#define CS_PIN D8
SPISettings settings = SPISettings(1000000, MSBFIRST, SPI_MODE0);

void setup() {
  pinMode(CS_PIN, OUTPUT);
  setCS(false);

  SPI.begin();

  initMAX7219();
}

void writeSample() {
  SPI.beginTransaction(settings);
  writeWord(0x01FF);
  SPI.endTransaction();
}

void initMAX7219() {
  SPI.beginTransaction(settings);
  writeWord(0x0C01);
  writeWord(0x0900);
  writeWord(0x0B07);
  writeWord(0x0A01);
  writeWord(0x0F00);
  SPI.endTransaction();
}

void drawSmiley() {
  SPI.beginTransaction(settings);

  writeWord(0x0100);
  writeWord(0x023E);
  writeWord(0x0308);
  writeWord(0x0408);
  writeWord(0x053E);
  writeWord(0x0600);
  writeWord(0x073A);
  writeWord(0x0800);

  SPI.endTransaction();
}

void drawHeart() {
  SPI.beginTransaction(settings);

  writeWord(0x010E);
  writeWord(0x021F);
  writeWord(0x033F);
  writeWord(0x047E);
  writeWord(0x057E);
  writeWord(0x063F);
  writeWord(0x071F);
  writeWord(0x080E);

  SPI.endTransaction();
}

void writeWord(uint16_t command) {
  setCS(true);

  SPI.transfer(command >> 8);
  SPI.transfer(command & 0xFF);

  setCS(false);
}

void setCS(bool state) {
  digitalWrite(CS_PIN, state ? LOW : HIGH);
}

void delayedCall(void (*func)()) {
  delay(1000);
  func();
}

void loop() {
  delayedCall(&drawSmiley);
  delayedCall(&drawHeart);
}