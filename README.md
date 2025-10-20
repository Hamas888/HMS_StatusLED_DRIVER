# HMS StatusLED Driver

A versatile LED strip driver library for STM32, Arduino, ESP32, and Zephyr platforms, with automatic RGB565/RGB888 color format detection and multiple color order support.

## Features

- ✅ **Auto Color Format Detection**: Automatically detects RGB565 vs RGB888 formats
- ✅ **Multiple Color Orders**: RGB, BGR, GRB support for different LED strips
- ✅ **Gamma Correction**: Optional gamma correction for better color accuracy
- ✅ **Multi-Platform**: STM32 HAL, Arduino, ESP-IDF, Zephyr support
- ✅ **DMA Support**: Efficient DMA-based transmission on STM32
- ✅ **Comprehensive Color Library**: 565 and 888 format color definitions
- ✅ **Power Control**: Turn LEDs on/off while preserving state
- ✅ **Brightness Control**: Global brightness adjustment (0-255)
- ✅ **State Memory**: Automatic state saving/restoration

## Quick Start (STM32)

### 1. STM32CubeMX Configuration

Configure a timer for PWM DMA output:

```
Timer Settings:
- Timer: TIM2 (or any available timer)
- Mode: PWM Generation CH1
- Prescaler: 0
- Counter Period: Auto-calculated by driver
- DMA: Enable for TIMx_CHx, Mode: Normal, Priority: High
- GPIO: Configure as TIMx_CHx output, High speed
```

### 2. Basic Usage

```cpp
#include "HMS_StatusLED_DRIVER.h"

extern TIM_HandleTypeDef htim2;

void setup_leds() {
    // Create instance: 60 LEDs, WS281X type, GRB order
    HMS_StatusLED led(60, HMS_STATUSLED_TYPE_WS281XX, HMS_STATUSLED_ORDER_GRB);
    
    // Initialize with 72MHz timer
    if (led.begin(&htim2, 72, TIM_CHANNEL_1) == HMS_STATUSLED_OK) {
        
        // Set colors (auto-detects RGB565/RGB888)
        led.setPixelColor(HMS_STATUSLED_RGB888_RED, 0);      // RGB888 red
        led.setPixelColor(HMS_STATUSLED_RGB565_BLUE, 1);     // RGB565 blue
        led.setPixelColor(0xFF00FF, 2);                      // RGB888 magenta
        led.setPixelColor(0xF81F, 3);                        // RGB565 magenta
        
        // Send to LEDs
        led.show();
    }
}
```

### 3. Advanced Usage

```cpp
// Create custom colors
uint32_t purple = HMS_STATUSLED_RGB_TO_888(128, 0, 128);
uint16_t orange = HMS_STATUSLED_RGB_TO_565(255, 165, 0);

// Set with specific color order
led.setPixelColor(HMS_STATUSLED_RGB888_WHITE, 5, HMS_STATUSLED_ORDER_RGB);

// Change default color order
led.setColorOrder(HMS_STATUSLED_ORDER_BGR);

// Clear all pixels
led.clear();
led.show();
```

### 4. Power Control & Brightness

```cpp
// Set brightness (0-255, default is 255)
led.setBrightness(128);  // 50% brightness
led.show();              // Apply brightness to current display

// Turn off LEDs (saves current state)
led.turnOff();
led.show();              // LEDs go dark

// Turn on LEDs (restores previous state)
led.turnOn();
led.show();              // LEDs restore with saved colors

// Brightness can be changed while on or off
led.setBrightness(64);   // 25% brightness
led.turnOn();            // Restore state with new brightness
led.show();

// Create breathing effect
for (int b = 0; b <= 255; b += 5) {
    led.setBrightness(b);
    led.show();
    HAL_Delay(50);
}
```

## Color Format Detection

The library automatically detects color format based on value range:

- **RGB565**: Values ≤ 0xFFFF (65535) → 16-bit format
- **RGB888**: Values > 0xFFFF → 24-bit format

```cpp
led.setPixelColor(0xF800, 0);    // Auto-detected as RGB565 red
led.setPixelColor(0xFF0000, 1);  // Auto-detected as RGB888 red
```

## Color Orders

Different LED strips use different color orders:

- **HMS_STATUSLED_ORDER_RGB**: Standard Red-Green-Blue
- **HMS_STATUSLED_ORDER_BGR**: Blue-Green-Red  
- **HMS_STATUSLED_ORDER_GRB**: Green-Red-Blue (WS2812B)

## Configuration

Edit `HMS_StatusLED_Config.h`:

```cpp
#define HMS_STATUSLED_MAX_PIXEL_COUNT      256
#define HMS_STATUSLED_GAMMA                true
#define HMS_STATUSLED_DEFAULT_COLOR_ORDER  HMS_STATUSLED_ORDER_GRB
#define HMS_STATUSLED_DEBUG_ENABLED        1  // Enable logging
```

## API Reference

### Constructor
```cpp
HMS_StatusLED(uint16_t maxPixels, HMS_StatusLED_Type type, HMS_StatusLED_OrderType colorOrder)
```

### Core Functions
```cpp
HMS_StatusLED_StatusTypeDef begin(TIM_HandleTypeDef *hTim, uint16_t timerFreqMHz, uint8_t channel);
HMS_StatusLED_StatusTypeDef setPixelColor(uint32_t color, uint16_t pixelIndex);
HMS_StatusLED_StatusTypeDef setPixelColor(uint32_t color, uint16_t pixelIndex, HMS_StatusLED_OrderType colorOrder);
HMS_StatusLED_StatusTypeDef show();
void clear();
void setColorOrder(HMS_StatusLED_OrderType order);
```

### Power Control & Brightness
```cpp
void turnOff();                    // Turn off LEDs, save current state
void turnOn();                     // Restore previously saved state  
void setBrightness(uint8_t level); // Set global brightness (0-255)
```

## Predefined Colors

The library includes comprehensive color definitions:

```cpp
// RGB888 Colors
HMS_STATUSLED_RGB888_RED       // 0xFF0000
HMS_STATUSLED_RGB888_GREEN     // 0x00FF00
HMS_STATUSLED_RGB888_BLUE      // 0x0000FF
// ... many more

// RGB565 Colors  
HMS_STATUSLED_RGB565_RED       // 0xF800
HMS_STATUSLED_RGB565_GREEN     // 0x07E0
HMS_STATUSLED_RGB565_BLUE      // 0x001F
// ... many more
```

## Platform Support

- **STM32 HAL**: Full DMA support with PWM timers
- **Arduino**: GPIO bit-banging (planned)
- **ESP-IDF**: RMT peripheral support (planned)
- **Zephyr**: Device tree integration (planned)

## Troubleshooting

### STM32 Issues

1. **DMA not working**: Ensure DMA is enabled for timer channel in CubeMX
2. **Wrong colors**: Check color order (GRB is common for WS2812B)
3. **No output**: Verify GPIO configuration and timer frequency
4. **Timing issues**: Ensure timer frequency calculation is correct

### Timing Requirements

WS2812B timing (automatically handled):
- T0H: 0.4µs ± 0.15µs
- T0L: 0.85µs ± 0.15µs  
- T1H: 0.8µs ± 0.15µs
- T1L: 0.45µs ± 0.15µs
- Reset: >50µs low

## License

MIT License - see LICENSE file for details.

## Author

Hamas Saeed - hamasaeed@gmail.com