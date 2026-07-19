# MAX7219 Driver for Arduino

Driver for controlling the MAX7219 LED driver chip. This library supports both 8x8 matrix displays and 7-segment displays, with easy brightness, decode mode, and display control.

## Features

- Auto initialize MAX7219 over SPI (provide custom settings if you want to)
- Configure decode mode for matrix or 7-segment displays
- Adjust intensity and scan limit
- Enable shutdown and display test modes
- Clear the display or update individual pixels, columns, and digits
- Compute estimated power dissipation

## Installation

1. Install the library from Arduino Library Manager

OR

1. Copy the `MAX7219` folder into your Arduino `libraries` directory.
2. Restart the Arduino IDE.

## Wiring

Typical wiring for an SPI-based MAX7219 module:

- `DIN` → Arduino `MOSI`
- `CLK` → Arduino `SCK`
- `CS`  → any digital pin
- `VCC` → `5V`
- `GND` → `GND`

## Example

```cpp
#include <MAX7219.h>

#define CS_PIN D8

MAX7219Settings settings = MAX7219Settings(MAX7219DecodeMode::None, 0x0A, 0x07, false, false);
MAX7219 controller;

void setup() {
  controller.begin(CS_PIN);
  controller.setSettings(settings);
  controller.clearDisplay();

  // Draw a single column
  controller.setColumn(1, 0b10101010);
  controller.setXY(3, 3, true);
}

void loop() {
  // Update display content here
}
```

## Matrix Display Usage

For an 8x8 matrix display, use `MAX7219_MATRIX_MODE` or the default `MAX7219DecodeMode::None`:

```cpp
controller.setPreset(MAX7219_MATRIX_MODE);
controller.clearDisplay();
controller.setColumn(1, 0b01010101);
controller.setXY(0, 0, true);
```

## 7-Segment Display Usage

For a 7-segment display, use `MAX7219_7SEG_MODE`:

```cpp
controller.setPreset(MAX7219_7SEG_MODE);
controller.setDigit(1, 0x01); // display "1" on digit 1
controller.setDigit(2, 0x02); // display "2" on digit 2
```

## API Reference

### Initialization

- `controller.begin(uint8_t csPin)`
- `controller.begin(uint8_t csPin, SPISettings settings)`

### Configuration

- `controller.setSettings(MAX7219Settings settings)`
- `controller.setPreset(uint8_t preset)`
- `controller.shutdown(bool enable)`
- `controller.setIntensity(uint8_t intensity)`
- `controller.setScanLimit(uint8_t limit)`
- `controller.setDecodeMode(MAX7219DecodeMode mode)`
- `controller.displayTest(bool enable)`

### Display Control

- `controller.clearDisplay()`
- `controller.setDigit(uint8_t digit, uint8_t value)`
- `controller.setColumn(uint8_t column, uint8_t value)`
- `controller.setXY(uint8_t x, uint8_t y, bool state)`

### Utility

- `controller.computePowerDissipation()`

## Presets

- `MAX7219_NO_PRESETS` - no preset configuration
- `MAX7219_MATRIX_MODE` - configure for an 8x8 matrix
- `MAX7219_7SEG_MODE` - configure for 7-segment displays

## Notes

- Intensity values are `0` to `15`.
- Scan limit values are `0` to `7`.
- Digit and column positions use `1` through `8`.
- Pixel coordinates use `x` and `y` values from `0` to `7`.

## License

This library is released under the MIT License.
