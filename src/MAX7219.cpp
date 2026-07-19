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

void MAX7219::begin(uint8_t csPin, SPISettings settings)
{
    _csPin = csPin;
    _spiSettings = settings;
    pinMode(_csPin, OUTPUT);
    SPI.begin();
    writeSettings();
}

void MAX7219::setSettings(MAX7219Settings settings)
{
    _settings = settings;
    writeSettings();
}

void MAX7219::setPreset(uint8_t preset)
{
    switch (preset)
    {
    case MAX7219_MATRIX_MODE:
        _settings.decodeMode = MAX7219DecodeMode::None;
        _settings.scanLimit = 0x07;
        this->preset = MAX7219_MATRIX_MODE;
        break;
    case MAX7219_7SEG_MODE:
        _settings.decodeMode = MAX7219DecodeMode::AllDigits;
        _settings.scanLimit = 0x07;
        this->preset = MAX7219_7SEG_MODE;
        break;
    default:
        _settings.decodeMode = MAX7219DecodeMode::None;
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

void MAX7219::setDecodeMode(MAX7219DecodeMode mode)
{
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
    if (digit > 8 || digit < 1)
        return;
    sendCommand(MAX7219_DIGIT_START_REG + (digit - 1), value);
}

void MAX7219::setColumn(uint8_t column, uint8_t value)
{
    if (column > 8 || column < 1)
        return;
    sendCommand(MAX7219_DIGIT_START_REG + (column - 1), value);
}

void MAX7219::setXY(uint8_t x, uint8_t y, bool state)
{
    if (x > 7 || y > 7)
        return;
    sendCommand(MAX7219_DIGIT_START_REG + y, (state ? 0x01 : 0x00) << x);
}

float MAX7219::computePowerDissipation(float vcc, MAX7219LEDColor ledColor)
{
    float duty = (2.0f * _settings.intensity + 1.0f) / 32.0f;
    uint8_t activeSegments = getActivePixelCount();
    float iSeg = 0.040f;

    float vLed = 0.0f;
    switch (ledColor)
    {
    case MAX7219LEDColor::Red:
        vLed = 2.0f;
        break;

    case MAX7219LEDColor::Yellow:
        vLed = 2.1f;
        break;

    case MAX7219LEDColor::Green:
        vLed = 3.2f;
        break;

    case MAX7219LEDColor::Blue:
        vLed = 3.3f;
        break;

    case MAX7219LEDColor::White:
        vLed = 3.2f;
        break;

    default:
        vLed = 2.0f;
        break;
    }

    return (vcc * 0.008f) + ((vcc - vLed) * duty * iSeg * activeSegments);
}

void MAX7219::writeSettings()
{
    sendCommand(MAX7219_DECODE_MODE_REG, static_cast<uint8_t>(_settings.decodeMode));
    sendCommand(MAX7219_INTENSITY_REG, _settings.intensity > 15 ? 15 : _settings.intensity);
    sendCommand(MAX7219_SCAN_LIMIT_REG, _settings.scanLimit > 7 ? 7 : _settings.scanLimit);
    sendCommand(MAX7219_SHUTDOWN_REG, _settings.shutdown ? 0x00 : 0x01);
    sendCommand(MAX7219_DISPLAY_TEST_REG, _settings.displayTest ? 0x01 : 0x00);
}

void MAX7219::sendCommand(uint8_t command, uint8_t data)
{
    if (command >= MAX7219_DIGIT_START_REG && command <= MAX7219_DIGIT_START_REG + 7)
        _displayBuffer[command - MAX7219_DIGIT_START_REG] = data;

    SPI.beginTransaction(_spiSettings);
    setCS(true);
    SPI.transfer(command);
    SPI.transfer(data);
    setCS(false);
    SPI.endTransaction();
}

void MAX7219::setCS(bool state)
{
    if (_csPin >= 0)
        digitalWrite(_csPin, state ? LOW : HIGH);
}

uint8_t MAX7219::getActivePixelCount()
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t columnData = _displayBuffer[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (columnData & (1 << j))
                count++;
        }
    }
    return count;
}