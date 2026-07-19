#ifndef MAX7219_H
#define MAX7219_H

/**
 * @file MAX7219.cpp
 * @brief Implementation of the MAX7219 LED driver
 * @author Saurav Sajeev
 */

#include <Arduino.h>
#include "MAX7219DEFS.h"
#include <SPI.h>

#ifdef __cplusplus

class MAX7219Settings
{
    MAX7219DecodeMode decodeMode = MAX7219_DECODE_MODE_NONE;
    uint8_t intensity = 0x0A;
    uint8_t scanLimit = 0x07;
    bool shutdown = false;
    bool displayTest = false;

public:
    MAX7219Settings(MAX7219DecodeMode decodeMode, uint8_t intensity, uint8_t scanLimit, bool shutdown, bool displayTest);
};

enum MAX7219DecodeMode
{
    none = MAX7219_DECODE_MODE_NONE,     // No decode for digits 0-7 (For matrix displays)
    codeB = MAX7219_DECODE_MODE_CODEB,   // All digits use Code B font (For 7-segment displays)
    digit0 = MAX7219_DECODE_MODE_DIGIT0, // Decode mode for digit 0 only
    digit1 = MAX7219_DECODE_MODE_DIGIT1, // Decode mode for digits 0-3 only
};

class MAX7219
{
public:
    void begin(uint8_t csPin);
    void begin(uint8_t csPin, SPISettings settings);
    void begin();
    void begin(SPISettings settings);
    void setSettings(MAX7219Settings setting);
    void setPreset(uint8_t preset);
    void shutdown(bool enable);
    void setIntensity(uint8_t intensity);
    void setScanLimit(uint8_t limit);
    void setDecodeMode(uint8_t mode);
    void displayTest(bool enable);
    void clearDisplay();
    void setDigit(uint8_t digit, uint8_t value);
    void setColumn(uint8_t column, uint8_t value);
    void setXY(uint8_t x, uint8_t y, bool state);
    float computePowerDissipation();

private:
    void writeSettings();
    void sendCommand(uint8_t command, uint8_t data);
    void setCS(bool state);

    uint8_t _csPin = -1;
    uint8_t preset = MAX7219_NO_PRESETS;
    MAX7219Settings _settings = MAX7219Settings(MAX7219_DECODE_MODE_NONE, 0x0A, 0x07, false, false);
    SPISettings _spiSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
}

#endif // __cplusplus
#endif // MAX7219_H
