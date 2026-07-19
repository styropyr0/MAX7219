#ifndef MAX7219_DEFS_H
#define MAX7219_DEFS_H

#pragma once

#ifdef __cplusplus

// MAX7219 Presets
#define MAX7219_NO_PRESETS 0x00 // No presets, user must set all settings manually
#define MAX7219_MATRIX_MODE 0x01 // No decode for digits 0-7 (For matrix displays)
#define MAX7219_7SEG_MODE 0x02 // All digits use Code B font, and appropriate scan settings (For 7-segment displays)

// MAX7219 Register Definitions
#define MAX7219_NOOP_REG 0x00
#define MAX7219_SHUTDOWN_REG 0x0C
#define MAX7219_INTENSITY_REG 0x0A
#define MAX7219_DECODE_MODE_REG 0x09
#define MAX7219_SCAN_LIMIT_REG 0x0B
#define MAX7219_DISPLAY_TEST_REG 0x0F
#define MAX7219_DIGIT_START_REG 0x01

// Decode Mode Settings
#define MAX7219_DECODE_MODE_NONE 0x00 // No decode for digits 0-7 (For matrix displays)
#define MAX7219_DECODE_MODE_CODEB 0xFF // All digits use Code B font (For 7-segment displays)
#define MAX7219_DECODE_MODE_DIGIT0 0x01 // Decode mode for digit 0 only
#define MAX7219_DECODE_MODE_DIGIT1 0x02 // Decode mode for digits 0-3 only

// Intensity Ranges
#define MAX7219_INTENSITY_MIN 0x00 // Minimum intensity (0)
#define MAX7219_INTENSITY_MAX 0x0F // Maximum intensity (15)

#endif // __cplusplus
#endif // MAX7219_DEFS_H