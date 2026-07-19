#ifndef MAX7219_H
#define MAX7219_H

/**
 * @file MAX7219.cpp
 * @brief Implementation of the MAX7219 LED driver
 * @author Saurav Sajeev
 * @version 1.0
 */

#include <Arduino.h>
#include "MAX7219DEFS.h"
#include <SPI.h>

#ifdef __cplusplus

// MAX7219 Presets
#define MAX7219_NO_PRESETS 0x00  // No presets, user must set all settings manually
#define MAX7219_MATRIX_MODE 0x01 // No decode for digits 0-7 (For matrix displays)
#define MAX7219_7SEG_MODE 0x02   // All digits use Code B font, and appropriate scan settings (For 7-segment displays)

/**
 * @enum MAX7219DecodeMode
 * @brief Enumeration for the decode mode of the MAX7219
 * @note The decode mode determines how the data sent to the MAX7219 is interpreted.
 */
enum class MAX7219DecodeMode : uint8_t
{
    None = MAX7219_DECODE_MODE_NONE,
    Digit0 = 0x01,
    Digits0_1 = 0x03,
    Digits0_2 = 0x07,
    Digits0_3 = 0x0F,
    Digits0_4 = 0x1F,
    Digits0_5 = 0x3F,
    Digits0_6 = 0x7F,
    AllDigits = MAX7219_DECODE_MODE_CODEB
};

/**
 * @class MAX7219Settings
 * @brief Class to hold the settings for the MAX7219
 * @note This class encapsulates the various settings that can be configured for the MAX7219.
 */
class MAX7219Settings
{
public:
    MAX7219DecodeMode decodeMode = MAX7219DecodeMode::None;
    uint8_t intensity = 0x0A;
    uint8_t scanLimit = 0x07;
    bool shutdown = false;
    bool displayTest = false;

    MAX7219Settings(MAX7219DecodeMode decodeMode, uint8_t intensity, uint8_t scanLimit, bool shutdown, bool displayTest);
};

/**
 * @class MAX7219
 * @brief Class to control the MAX7219 LED driver
 * @note This class provides a simple interface to control the MAX7219 LED driver.
 *
 * @author Saurav Sajeev
 * @version 1.0
 */
class MAX7219
{
public:
    /**
     * @brief Initializes the MAX7219 with the specified chip select pin.
     * @param csPin The chip select pin to use for SPI communication.
     */
    void begin(uint8_t csPin);

    /**
     * @brief Initializes the MAX7219 with the specified chip select pin and SPI settings.
     * @param csPin The chip select pin to use for SPI communication.
     * @param settings The SPI settings to use.
     */
    void begin(uint8_t csPin, SPISettings settings);

    /**
     * @brief Sets the settings for the MAX7219.
     * @param settings The settings to apply to the MAX7219.
     */
    void setSettings(MAX7219Settings settings);

    /**
     * @brief Sets a preset configuration for the MAX7219.
     * @param preset The preset configuration to apply. Use one of the predefined constants: MAX7219_MATRIX_MODE, MAX7219_7SEG_MODE, or MAX7219_NO_PRESETS.
     * @note This function allows you to quickly configure the MAX7219 for common use cases. The available presets are:
     * - MAX7219_MATRIX_MODE: Configures the MAX7219 for use with an 8x8 LED matrix.
     * - MAX7219_7SEG_MODE: Configures the MAX7219 for use with 7-segment displays.
     * - MAX7219_NO_PRESETS: Resets the MAX7219 to a default
     * @addtogroup Presets
     */
    void setPreset(uint8_t preset);

    /**
     * @brief Enables or disables the shutdown mode of the MAX7219.
     * @param enable Set to true to enable shutdown mode, false to disable.
     * @note When shutdown mode is enabled, the MAX7219 will not drive the connected LEDs, effectively turning them off.
     * This can be useful for power saving or when the display is not needed.
     */
    void shutdown(bool enable);

    /**
     * @brief Sets the intensity of the MAX7219 display.
     * @param intensity The intensity level (0-15) to set for the display.
     * @note The intensity level controls the brightness of the LEDs driven by the MAX7219.
     * A value of 0 corresponds to the dimmest setting, while a value of 15 corresponds to the brightest setting.
     */
    void setIntensity(uint8_t intensity);

    /**
     * @brief Sets the scan limit of the MAX7219.
     * @param limit The scan limit (0-7) to set for the display.
     * @note The scan limit determines how many digits (or rows) of the display are
     */
    void setScanLimit(uint8_t limit);

    /**
     * @brief Sets the decode mode of the MAX7219.
     * @param mode The decode mode to set for the display. Use one of the values from the MAX7219DecodeMode enumeration.
     * @note The decode mode determines how the data sent to the MAX7219 is interpreted.
     * For example, in "None" mode, the data is treated as raw binary values, while in "AllDigits" mode, the data is interpreted as BCD values for 7-segment displays.
     */
    void setDecodeMode(MAX7219DecodeMode mode);

    /**
     * @brief Enables or disables the display test mode of the MAX7219.
     * @param enable Set to true to enable display test mode, false to disable.
     * @note When display test mode is enabled, all segments of the display will be turned on, regardless of the data sent to the MAX7219.
     */
    void displayTest(bool enable);

    /**
     * @brief Clears the display by turning off all segments.
     * @note This function sends commands to the MAX7219 to turn off all segments of
     * the connected display, effectively clearing any displayed content.
     */
    void clearDisplay();

    /**
     * @brief Sets the value of a specific digit on the display.
     * @param digit The digit position (1-8) to set.
     * @note Use with preset mode MAX7219_7SEG_MODE. The digit parameter specifies which digit to set, and the value parameter specifies the value to display on that digit.
     * The value should be a valid BCD value (0-9) or a special character code as defined in the MAX7219 datasheet.
     * @note If the digit parameter is outside the range of 1-8, the function will return without making any changes to the display.
     */
    void setDigit(uint8_t digit, uint8_t value);

    /**
     * @brief Sets the value of a specific column on the display.
     * @param column The column position (1-8) to set.
     * @param value The value to set for the specified column.
     * @note Use with preset mode MAX7219_MATRIX_MODE. The column parameter specifies which column to set, and the value parameter specifies the data to display in that column.
     * The value should be a byte where each bit represents the state of a row in that column (1 for on, 0 for off).
     */
    void setColumn(uint8_t column, uint8_t value);

    /**
     * @brief Sets the state of a specific pixel on the display.
     * @param x The x-coordinate (column) of the pixel to set (0-7).
     * @param y The y-coordinate (row) of the pixel to set (0-7).
     * @param state The state to set for the specified pixel (true for on, false for off).
     */
    void setXY(uint8_t x, uint8_t y, bool state);

    /**
     * @brief Computes the power dissipation of the MAX7219.
     * @return The computed power dissipation in watts.
     */
    float computePowerDissipation();

private:
    void writeSettings();
    void sendCommand(uint8_t command, uint8_t data);
    void setCS(bool state);

    int _csPin = -1;
    uint8_t preset = MAX7219_NO_PRESETS;
    MAX7219Settings _settings = MAX7219Settings(MAX7219DecodeMode::None, 0x0A, 0x07, false, false);
    SPISettings _spiSettings = SPISettings(1000000, MSBFIRST, SPI_MODE0);
};

#endif // __cplusplus
#endif // MAX7219_H
