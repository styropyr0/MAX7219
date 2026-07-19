#include "MAX7219.h"

MAX7219Settings::MAX7219Settings(MAX7219DecodeMode decodeMode, uint8_t intensity, uint8_t scanLimit, bool shutdown, bool displayTest)
    : decodeMode(decodeMode), intensity(intensity), scanLimit(scanLimit), shutdown(shutdown), displayTest(displayTest) {}

void MAX7219::begin(uint8_t csPin)
{
    _csPin = csPin;
    pinMode(_csPin, OUTPUT);
    SPI.begin();
    writeSettings();
}

void MAX7219::begin(uint8_t csPinSPISettings settings)
{
    _csPin = csPin;
    _spiSettings = settings;
    pinMode(_csPin, OUTPUT);
    SPI.begin();
    writeSettings();
}

void MAX7219::begin()
{
    writeSettings();
}

void MAX7219::begin(SPISettings settings)
{
    _spiSettings = settings;
    writeSettings();
}

void MAX7219::setSettings(MAX7219Settings setting)
{
    _settings = setting;
    writeSettings();
}

void MAX7219::setPreset(uint8_t preset)
{
    switch (preset)
    {
    case MAX7219_MATRIX_MODE:
        _settings.decodeMode = MAX7219_DECODE_MODE_NONE;
        _settings.scanLimit = 0x07;
        this->preset = MAX7219_MATRIX_MODE;
        break;
    case MAX7219_7SEG_MODE:
        _settings.decodeMode = MAX7219_DECODE_MODE_CODEB;
        _settings.scanLimit = 0x07;
        this->preset = MAX7219_7SEG_MODE;
        break;
    default:
        _settings.decodeMode = MAX7219_DECODE_MODE_NONE;
        _settings.intensity = 0x01;
        this->preset = MAX7219_NO_PRESETS;
        break;
    }
    writeSettings();
}

void MAX7219::shutdown(bool enable)
{
    _settings.shutdown = enable;
    writeSettings();
}

void MAX7219::setIntensity(uint8_t intensity)
{
    _settings.intensity = intensity > 15 ? 15 : intensity;
    writeSettings();
}

void MAX7219::setScanLimit(uint8_t limit)
{
    _settings.scanLimit = limit > 7 ? 7 : limit;
    writeSettings();
}

void MAX7219::setDecodeMode(uint8_t mode)
{
    if (mode > 2)
        return;
    _settings.decodeMode = mode;
    writeSettings();
}

void MAX7219::displayTest(bool enable)
{
    _settings.displayTest = enable;
    writeSettings();
}

void MAX7219::clearDisplay()
{
    for (uint8_t i = 0; i < 8; i++)
        sendCommand(MAX7219_DIGIT_START_REG + i, 0x00);
}

void MAX7219::setDigit(uint8_t digit, uint8_t value)
{
    if (digit > 7)
        return;
    sendCommand(MAX7219_DIGIT_START_REG + digit, value);
}

void MAX7219::setColumn(uint8_t column, uint8_t value)
{
    if (column > 7)
        return;
    sendCommand(MAX7219_DIGIT_START_REG + column, value);
}

void MAX7219::setXY(uint8_t x, uint8_t y, bool state)
{
    if (x > 7 || y > 7)
        return;
    sendCommand(MAX7219_DIGIT_START_REG + y, (state ? 0x01 : 0x00) << x);
}

float MAX7219::computePowerDissipation()
{
    float currentPerSegment = 0.0005;
    float voltage = 5.0;             
    float totalCurrent = currentPerSegment * 8 * (_settings.scanLimit + 1);
    return voltage * totalCurrent;
}

void MAX7219::writeSettings()
{
    sendCommand(MAX7219_DECODE_MODE_REG, _settings.decodeMode);
    sendCommand(MAX7219_INTENSITY_REG, _settings.intensity > 15 ? 15 : _settings.intensity);
    sendCommand(MAX7219_SCAN_LIMIT_REG, _settings.scanLimit > 7 ? 7 : _settings.scanLimit);
    sendCommand(MAX7219_SHUTDOWN_REG, _settings.shutdown ? 0x00 : 0x01);
    sendCommand(MAX7219_DISPLAY_TEST_REG, _settings.displayTest ? 0x01 : 0x00);
}

void sendCommand(uint8_t command, uint8_t data)
{
    SPI.beginTransaction(_spiSettings);
    setCS(true);
    SPI.transfer(command);
    SPI.transfer(data);
    setCS(false);
    SPI.endTransaction();
}

void MAX7219::setCS(bool state)
{
    if (csPin != -1)
        digitalWrite(_csPin, state ? LOW : HIGH);
}