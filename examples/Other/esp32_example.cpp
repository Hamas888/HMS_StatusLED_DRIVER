/*
 ====================================================================================================
 * HMS StatusLED Driver - ESP32 Usage Example
 * 
 * This example shows how to use the HMS StatusLED Driver with ESP32 (Arduino Framework)
 * Uses the RMT peripheral for precise WS2812B timing
 ====================================================================================================
 */

#include "HMS_StatusLED_DRIVER.h"

// Create HMS StatusLED instance
// Parameters: maxPixels, ledType, colorOrder
HMS_StatusLED led(60, HMS_STATUSLED_TYPE_WS281XX, HMS_STATUSLED_ORDER_GRB);

void setup() {
    Serial.begin(115200);
    Serial.println("HMS StatusLED Driver - ESP32 Example");
    
    // Initialize the driver with GPIO pin and RMT channel
    // Parameters: pin, rmt_channel (optional, defaults to RMT_CHANNEL_0)
    if (led.begin(5, RMT_CHANNEL_0) != HMS_STATUSLED_OK) {
        Serial.println("Failed to initialize LED driver!");
        return;
    }
    
    Serial.println("LED driver initialized successfully!");
    
    // Clear all LEDs
    led.clear();
    led.show();
    
    delay(1000);
}

void loop() {
    // Rainbow effect example
    rainbowCycle();
    
    // Color wheel demonstration
    colorWheel();
    
    // Breathing effect with different colors
    breathingEffect();
}

void rainbowCycle() {
    Serial.println("Running rainbow cycle...");
    
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int hue = 0; hue < 256; hue += 2) {
            for (int i = 0; i < 60; i++) {
                uint32_t color = wheel((hue + i * 4) & 255);
                led.setPixelColor(color, i);
            }
            led.show();
            delay(20);
        }
    }
}

void colorWheel() {
    Serial.println("Running color wheel...");
    
    // Using RGB888 colors (auto-detected)
    uint32_t colors[] = {
        HMS_STATUSLED_RGB888_RED,
        HMS_STATUSLED_RGB888_ORANGE,
        HMS_STATUSLED_RGB888_YELLOW,
        HMS_STATUSLED_RGB888_GREEN,
        HMS_STATUSLED_RGB888_CYAN,
        HMS_STATUSLED_RGB888_BLUE,
        HMS_STATUSLED_RGB888_PURPLE,
        HMS_STATUSLED_RGB888_MAGENTA
    };
    
    for (int cycle = 0; cycle < 3; cycle++) {
        for (int colorIndex = 0; colorIndex < 8; colorIndex++) {
            // Fill all LEDs with the current color
            for (int i = 0; i < 60; i++) {
                led.setPixelColor(colors[colorIndex], i);
            }
            led.show();
            delay(500);
            
            // Fade out
            led.clear();
            led.show();
            delay(200);
        }
    }
}

void breathingEffect() {
    Serial.println("Running breathing effect...");
    
    uint32_t colors[] = {
        HMS_STATUSLED_RGB888_BLUE,
        HMS_STATUSLED_RGB888_GREEN,
        HMS_STATUSLED_RGB888_RED
    };
    
    for (int colorIndex = 0; colorIndex < 3; colorIndex++) {
        // Extract RGB components
        uint8_t r = HMS_STATUSLED_GET_RED_888(colors[colorIndex]);
        uint8_t g = HMS_STATUSLED_GET_GREEN_888(colors[colorIndex]);
        uint8_t b = HMS_STATUSLED_GET_BLUE_888(colors[colorIndex]);
        
        // Breathing in (fade up)
        for (int brightness = 0; brightness <= 255; brightness += 5) {
            uint8_t br = (r * brightness) / 255;
            uint8_t bg = (g * brightness) / 255;
            uint8_t bb = (b * brightness) / 255;
            
            uint32_t color = HMS_STATUSLED_RGB_TO_888(br, bg, bb);
            
            for (int i = 0; i < 60; i++) {
                led.setPixelColor(color, i);
            }
            led.show();
            delay(20);
        }
        
        delay(500);
        
        // Breathing out (fade down)
        for (int brightness = 255; brightness >= 0; brightness -= 5) {
            uint8_t br = (r * brightness) / 255;
            uint8_t bg = (g * brightness) / 255;
            uint8_t bb = (b * brightness) / 255;
            
            uint32_t color = HMS_STATUSLED_RGB_TO_888(br, bg, bb);
            
            for (int i = 0; i < 60; i++) {
                led.setPixelColor(color, i);
            }
            led.show();
            delay(20);
        }
        
        delay(500);
    }
}

// Color wheel function to generate rainbow colors
uint32_t wheel(uint8_t wheelPos) {
    wheelPos = 255 - wheelPos;
    if (wheelPos < 85) {
        return HMS_STATUSLED_RGB_TO_888(255 - wheelPos * 3, 0, wheelPos * 3);
    }
    if (wheelPos < 170) {
        wheelPos -= 85;
        return HMS_STATUSLED_RGB_TO_888(0, wheelPos * 3, 255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return HMS_STATUSLED_RGB_TO_888(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Simple patterns for testing
void testPattern() {
    Serial.println("Running test pattern...");
    
    // Set individual pixels using different methods
    led.setPixelColor(HMS_STATUSLED_RGB888_RED, 0);      // Pixel 0: Red
    led.setPixelColor(HMS_STATUSLED_RGB888_GREEN, 1);    // Pixel 1: Green  
    led.setPixelColor(HMS_STATUSLED_RGB888_BLUE, 2);     // Pixel 2: Blue
    led.setPixelColor(0xFF00FF, 3);                      // Pixel 3: Magenta (RGB888)
    
    // Using RGB565 colors (auto-detected)
    led.setPixelColor(HMS_STATUSLED_RGB565_YELLOW, 4);   // Pixel 4: Yellow
    led.setPixelColor(HMS_STATUSLED_RGB565_CYAN, 5);     // Pixel 5: Cyan
    
    // Using custom RGB values
    uint32_t purple = HMS_STATUSLED_RGB_TO_888(128, 0, 128);
    led.setPixelColor(purple, 6);                        // Pixel 6: Purple
    
    // Set color with specific order (overrides default)
    led.setPixelColor(HMS_STATUSLED_RGB888_WHITE, 7, HMS_STATUSLED_ORDER_RGB);
    
    // Send data to LEDs
    led.show();
    
    delay(2000);
    
    // Clear all LEDs
    led.clear();
    led.show();
}