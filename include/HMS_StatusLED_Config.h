   /*
 ====================================================================================================
 * File:        HMS_StatusLED_Config.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Oct 10 2025
 * Brief:       This Package Provide Status LED Driver Configuration.
 * 
 ====================================================================================================
 * License: 
 * MIT License
 * 
 * Copyright (c) 2025 Hamas Saeed
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For any inquiries, contact Hamas Saeed at hamasaeed@gmail.com
 *
 ====================================================================================================
 */

#ifndef HMS_STATUSLED_CONFIG_H
#define HMS_STATUSLED_CONFIG_H

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     Enable only if ChronoLog is included                      │
  │ Requires: ChronoLog library → https://github.com/Hamas888/ChronoLog │
  └─────────────────────────────────────────────────────────────────────┘
*/
#define HMS_STATUSLED_DEBUG_ENABLED           0                                 // Enable debug messages (1=enabled, 0=disabled)

/*
    ┌───────────────────────────────────────────────────────────────────┐
    │ Note:    Driver selection for Status LED                          │
    │ Support: WS28XX                                                   │
    │ Usage:   Uncomment the LED you want to use as default             │
    └───────────────────────────────────────────────────────────────────┘
*/

#define HMS_STATUSLED_WS281XX                                                   // Define the WS28XX LED to be used


#define HMS_STATUSLED_MAX_PIXEL_COUNT      256                                  // Maximum number of pixels in the LED strip (adjust as needed)
#define HMS_STATUSLED_PULSE_LENGTH_NS      1250                                 // Pulse length in nanoseconds (typically 1250ns for WS2812B)
#define HMS_STATUSLED_PULSE_0_NS           400
#define HMS_STATUSLED_PULSE_1_NS           800
#define HMS_STATUSLED_GAMMA                true                                 // Enable gamma correction (true/false)
#define HMS_STATUSLED_DEFAULT_COLOR_ORDER  HMS_STATUSLED_ORDER_RGB              // Default color order (RGB, BGR, GRB)

/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     RGB565 color definitions (16-bit format)                  │
  │           VS Code will show color previews for these hex values     │
  └─────────────────────────────────────────────────────────────────────┘
*/

#define HMS_STATUSLED_RGB565_RED           0xF800      // #FF0000 → 255,   0,   0
#define HMS_STATUSLED_RGB565_TAN           0xD5B1      // #D2B48C → 210, 180, 140
#define HMS_STATUSLED_RGB565_GRAY          0x8410      // #808080 → 128, 128, 128
#define HMS_STATUSLED_RGB565_GOLD          0xFEA0      // #FFD700 → 255, 215,   0
#define HMS_STATUSLED_RGB565_BLUE          0x001F      // #0000FF →   0,   0, 255
#define HMS_STATUSLED_RGB565_CYAN          0x07FF      // #00FFFF →   0, 255, 255
#define HMS_STATUSLED_RGB565_PINK          0xFC18      // #FFC0CB → 255, 192, 203
#define HMS_STATUSLED_RGB565_TEAL          0x0410      // #008080 →   0, 128, 128
#define HMS_STATUSLED_RGB565_LIME          0x07E0      // #00FF00 →   0, 255,   0
#define HMS_STATUSLED_RGB565_AQUA          0x07FF      // #00FFFF →   0, 255, 255
#define HMS_STATUSLED_RGB565_NAVY          0x000F      // #000080 →   0,   0, 128
#define HMS_STATUSLED_RGB565_OLIVE         0x8400      // #808000 → 128, 128,   0
#define HMS_STATUSLED_RGB565_BLACK         0x0000      // #000000 →   0,   0,   0
#define HMS_STATUSLED_RGB565_WHITE         0xFFFF      // #FFFFFF → 255, 255, 255
#define HMS_STATUSLED_RGB565_GREEN         0x07E0      // #00FF00 →   0, 255,   0
#define HMS_STATUSLED_RGB565_BROWN         0xA145      // #A52A2A → 165,  42,  42
#define HMS_STATUSLED_RGB565_CORAL         0xFBEA      // #FF7F50 → 255, 127,  80
#define HMS_STATUSLED_RGB565_INDIGO        0x4810      // #4B0082 →  75,   0, 130
#define HMS_STATUSLED_RGB565_TOMATO        0xFB08      // #FF6347 → 255,  99,  71
#define HMS_STATUSLED_RGB565_SILVER        0xC618      // #C0C0C0 → 192, 192, 192
#define HMS_STATUSLED_RGB565_VIOLET        0x8A16      // #8A2BE2 → 138,  43, 226
#define HMS_STATUSLED_RGB565_MAROON        0x7800      // #800000 → 128,   0,   0
#define HMS_STATUSLED_RGB565_YELLOW        0xFFE0      // #FFFF00 → 255, 255,   0
#define HMS_STATUSLED_RGB565_ORANGE        0xFD20      // #FFA500 → 255, 165,   0
#define HMS_STATUSLED_RGB565_PURPLE        0x780F      // #800080 → 128,   0, 128
#define HMS_STATUSLED_RGB565_DIMGRAY       0x6B4D      // #696969 → 105, 105, 105
#define HMS_STATUSLED_RGB565_CRIMSON       0xD8A7      // #DC143C → 220,  20,  60
#define HMS_STATUSLED_RGB565_MAGENTA       0xF81F      // #FF00FF → 255,   0, 255
#define HMS_STATUSLED_RGB565_SKYBLUE       0x867D      // #87CEEB → 135, 206, 235
#define HMS_STATUSLED_RGB565_LAVENDER      0xE73F      // #E6E6FA → 230, 230, 250
#define HMS_STATUSLED_RGB565_SEAGREEN      0x2E5B      // #2E8B57 →  46, 139,  87
#define HMS_STATUSLED_RGB565_CHOCOLATE     0xD343      // #D2691E → 210, 105,  30
#define HMS_STATUSLED_RGB565_FIREBRICK     0xB104      // #B22222 → 178,  34,  34
#define HMS_STATUSLED_RGB565_SLATEGRAY     0x7412      // #708090 → 112, 128, 144
#define HMS_STATUSLED_RGB565_DARKORANGE    0xFC60      // #FF8C00 → 255, 140,   0
#define HMS_STATUSLED_RGB565_FORESTGREEN   0x2444      // #228B22 →  34, 139,  34
#define HMS_STATUSLED_RGB565_DARKSLATEGRAY 0x2A69      // #2F4F4F →  47,  79,  79


/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Note:     RGB888 color definitions (24-bit format)                  │
  │           VS Code will show color previews for these hex values     │
  └─────────────────────────────────────────────────────────────────────┘
*/

// RGB888 Colors (24-bit) - VS Code Color Preview Compatible
#define HMS_STATUSLED_RGB888_RED           0xFF0000    // #FF0000 → 255,   0,   0
#define HMS_STATUSLED_RGB888_TAN           0xD2B48C    // #D2B48C → 210, 180, 140
#define HMS_STATUSLED_RGB888_GRAY          0x808080    // #808080 → 128, 128, 128
#define HMS_STATUSLED_RGB888_GOLD          0xFFD700    // #FFD700 → 255, 215,   0
#define HMS_STATUSLED_RGB888_BLUE          0x0000FF    // #0000FF →   0,   0, 255
#define HMS_STATUSLED_RGB888_CYAN          0x00FFFF    // #00FFFF →   0, 255, 255
#define HMS_STATUSLED_RGB888_PINK          0xFFC0CB    // #FFC0CB → 255, 192, 203
#define HMS_STATUSLED_RGB888_TEAL          0x008080    // #008080 →   0, 128, 128
#define HMS_STATUSLED_RGB888_LIME          0x00FF00    // #00FF00 →   0, 255,   0
#define HMS_STATUSLED_RGB888_AQUA          0x00FFFF    // #00FFFF →   0, 255, 255
#define HMS_STATUSLED_RGB888_NAVY          0x000080    // #000080 →   0,   0, 128
#define HMS_STATUSLED_RGB888_OLIVE         0x808000    // #808000 → 128, 128,   0
#define HMS_STATUSLED_RGB888_BLACK         0x000000    // #000000 →   0,   0,   0
#define HMS_STATUSLED_RGB888_WHITE         0xFFFFFF    // #FFFFFF → 255, 255, 255
#define HMS_STATUSLED_RGB888_GREEN         0x00FF00    // #00FF00 →   0, 255,   0
#define HMS_STATUSLED_RGB888_BROWN         0xA52A2A    // #A52A2A → 165,  42,  42
#define HMS_STATUSLED_RGB888_CORAL         0xFF7F50    // #FF7F50 → 255, 127,  80
#define HMS_STATUSLED_RGB888_INDIGO        0x4B0082    // #4B0082 →  75,   0, 130
#define HMS_STATUSLED_RGB888_TOMATO        0xFF6347    // #FF6347 → 255,  99,  71
#define HMS_STATUSLED_RGB888_SILVER        0xC0C0C0    // #C0C0C0 → 192, 192, 192
#define HMS_STATUSLED_RGB888_VIOLET        0x8A2BE2    // #8A2BE2 → 138,  43, 226
#define HMS_STATUSLED_RGB888_MAROON        0x800000    // #800000 → 128,   0,   0
#define HMS_STATUSLED_RGB888_YELLOW        0xFFFF00    // #FFFF00 → 255, 255,   0
#define HMS_STATUSLED_RGB888_ORANGE        0xFFA500    // #FFA500 → 255, 165,   0
#define HMS_STATUSLED_RGB888_PURPLE        0x800080    // #800080 → 128,   0, 128
#define HMS_STATUSLED_RGB888_DIMGRAY       0x696969    // #696969 → 105, 105, 105
#define HMS_STATUSLED_RGB888_CRIMSON       0xDC143C    // #DC143C → 220,  20,  60
#define HMS_STATUSLED_RGB888_MAGENTA       0xFF00FF    // #FF00FF → 255,   0, 255
#define HMS_STATUSLED_RGB888_SKYBLUE       0x87CEEB    // #87CEEB → 135, 206, 235
#define HMS_STATUSLED_RGB888_LAVENDER      0xE6E6FA    // #E6E6FA → 230, 230, 250
#define HMS_STATUSLED_RGB888_SEAGREEN      0x2E8B57    // #2E8B57 →  46, 139,  87
#define HMS_STATUSLED_RGB888_CHOCOLATE     0xD2691E    // #D2691E → 210, 105,  30
#define HMS_STATUSLED_RGB888_FIREBRICK     0xB22222    // #B22222 → 178,  34,  34
#define HMS_STATUSLED_RGB888_SLATEGRAY     0x708090    // #708090 → 112, 128, 144
#define HMS_STATUSLED_RGB888_DARKORANGE    0xFF8C00    // #FF8C00 → 255, 140,   0
#define HMS_STATUSLED_RGB888_FORESTGREEN   0x228B22    // #228B22 →  34, 139,  34
#define HMS_STATUSLED_RGB888_DARKSLATEGRAY 0x2F4F4F    // #2F4F4F →  47,  79,  79


/*
  ┌─────────────────────────────────────────────────────────────────────┐
  │ Color Conversion and Utility Macros                                 │
  └─────────────────────────────────────────────────────────────────────┘
*/

// Basic RGB to Format Conversion Macros
#define HMS_STATUSLED_RGB_TO_565(R, G, B)    (((((uint16_t)(R)) & 0xF8) << 8) | ((((uint16_t)(G)) & 0xFC) << 3) | (((uint16_t)(B)) >> 3))
#define HMS_STATUSLED_RGB_TO_888(R, G, B)    ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// RGB565 to RGB888 Conversion (Expanding 5-6-5 bits to 8-8-8 bits)
#define HMS_STATUSLED_565_TO_888(RGB565)     (((((RGB565) & 0xF800) >> 8) | (((RGB565) & 0xE000) >> 13)) << 16) | \
                                             (((((RGB565) & 0x07E0) >> 3) | (((RGB565) & 0x0600) >> 9)) << 8) | \
                                             (((RGB565) & 0x001F) << 3) | (((RGB565) & 0x001C) >> 2)

// RGB888 to RGB565 Conversion (Reducing 8-8-8 bits to 5-6-5 bits)
#define HMS_STATUSLED_888_TO_565(RGB888)     ((((RGB888) & 0xF80000) >> 8) | (((RGB888) & 0x00FC00) >> 5) | (((RGB888) & 0x0000F8) >> 3))

// Extract Individual Color Components
#define HMS_STATUSLED_GET_RED_888(RGB888)    (((RGB888) >> 16) & 0xFF)
#define HMS_STATUSLED_GET_GREEN_888(RGB888)  (((RGB888) >> 8) & 0xFF)
#define HMS_STATUSLED_GET_BLUE_888(RGB888)   ((RGB888) & 0xFF)

#define HMS_STATUSLED_GET_RED_565(RGB565)    (((RGB565) >> 8) & 0xF8)
#define HMS_STATUSLED_GET_GREEN_565(RGB565)  (((RGB565) >> 3) & 0xFC)
#define HMS_STATUSLED_GET_BLUE_565(RGB565)   (((RGB565) << 3) & 0xF8)

// Get Hex Values with 0x Prefix (Easy Access Macros)
#define HMS_STATUSLED_HEX565(COLOR)          ((uint16_t)(COLOR))
#define HMS_STATUSLED_HEX888(COLOR)          ((uint32_t)(COLOR))

// Convert Color Definitions to Opposite Format
#define HMS_STATUSLED_565_TO_HEX888(COLOR565) HMS_STATUSLED_565_TO_888(COLOR565)
#define HMS_STATUSLED_888_TO_HEX565(COLOR888) HMS_STATUSLED_888_TO_565(COLOR888)

// Legacy Compatibility Macros (keeping old naming for backward compatibility)
#define HMS_STATUSLED_GET_RGB565_COLOR(R, G, B)  HMS_STATUSLED_RGB_TO_565(R, G, B)
#define HMS_STATUSLED_GET_RGB888_COLOR(R, G, B)  HMS_STATUSLED_RGB_TO_888(R, G, B)
#define HMS_STATUSLED_GET_COLOR_HEX565(COLOR)    HMS_STATUSLED_HEX565(COLOR)
#define HMS_STATUSLED_GET_COLOR_HEX888(COLOR)    HMS_STATUSLED_HEX888(COLOR)

/*
  ┌────────────────────────────────────────────────────────────────────────┐
  │ Usage Examples:                                                        │
  │                                                                        │
  │ // Get hex value with 0x prefix:                                       │
  │ uint16_t red565 = HMS_STATUSLED_HEX565(HMS_STATUSLED_RGB565_RED);      │
  │ uint32_t red888 = HMS_STATUSLED_HEX888(HMS_STATUSLED_RGB888_RED);      │
  │                                                                        │
  │ // Convert RGB components to color formats:                            │
  │ uint16_t customColor565 = HMS_STATUSLED_RGB_TO_565(128, 64, 192);      │
  │ uint32_t customColor888 = HMS_STATUSLED_RGB_TO_888(128, 64, 192);      │
  │                                                                        │
  │ // Convert between formats:                                            │
  │ uint32_t rgb888 = HMS_STATUSLED_565_TO_888(HMS_STATUSLED_RGB565_BLUE); │
  │ uint16_t rgb565 = HMS_STATUSLED_888_TO_565(HMS_STATUSLED_RGB888_BLUE); │
  │                                                                        │
  │ // Extract color components:                                           │
  │ uint8_t red = HMS_STATUSLED_GET_RED_888(HMS_STATUSLED_RGB888_PURPLE);  │
  │ uint8_t green = HMS_STATUSLED_GET_GREEN_565(HMS_STATUSLED_RGB565_TEAL);│
  │                                                                        │
  │ // SetPixelColor examples (auto-detects RGB565/RGB888):                │
  │ led.setPixelColor(HMS_STATUSLED_RGB565_RED, 0);    // RGB565 detected  │
  │ led.setPixelColor(HMS_STATUSLED_RGB888_BLUE, 1);   // RGB888 detected  │
  │ led.setPixelColor(0xFF00FF, 2);                    // RGB888 magenta   │
  │ led.setPixelColor(0xF81F, 3);                      // RGB565 magenta   │
  │                                                                        │
  │ // SetPixelColor with specific color order:                            │
  │ led.setPixelColor(0xFF0000, 0, HMS_STATUSLED_ORDER_RGB); // Red as RGB │
  │ led.setPixelColor(0xFF0000, 1, HMS_STATUSLED_ORDER_BGR); // Red as BGR │
  │ led.setPixelColor(0xFF0000, 2, HMS_STATUSLED_ORDER_GRB); // Red as GRB │
  │                                                                        │
  │ // Change default color order:                                         │
  │ led.setColorOrder(HMS_STATUSLED_ORDER_GRB);  // Common for WS2812B     │
  └────────────────────────────────────────────────────────────────────────┘
*/

#endif // HMS_STATUSLED_CONFIG_H