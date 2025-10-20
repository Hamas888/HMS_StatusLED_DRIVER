/*
 ====================================================================================================
 * HMS StatusLED Driver - STM32 Usage Example
 * 
 * This example shows how to use the HMS StatusLED Driver with STM32 HAL
 * Make sure to configure your timer for PWM DMA output in STM32CubeMX
 * 
 * NEW FEATURES DEMONSTRATED:
 * - turnOff(): Saves current LED state and turns off all LEDs
 * - turnOn():  Restores the previously saved LED state  
 * - setBrightness(0-255): Controls overall brightness of all LEDs
 * - Brightness control preserves original colors and can be changed anytime
 ====================================================================================================
 */

#include "HMS_StatusLED_DRIVER.h"

// Example for STM32 with Timer 2, Channel 1, running at 72MHz
extern TIM_HandleTypeDef htim2;  // Configured in main.c via STM32CubeMX

void statusLED_example() {
    // Create HMS StatusLED instance
    // Parameters: maxPixels, ledType, colorOrder
    HMS_StatusLED led(60, HMS_STATUSLED_TYPE_WS281XX, HMS_STATUSLED_ORDER_GRB);
    
    // Initialize the driver with timer and channel
    // Parameters: timer_handle, timer_frequency_MHz, channel
    if (led.begin(&htim2, 72, TIM_CHANNEL_1) != HMS_STATUSLED_OK) {
        // Handle initialization error
        return;
    }
    
    // Clear all LEDs
    led.clear();
    led.show();
    
    // Set individual pixels using different color formats
    
    // Using RGB888 colors (auto-detected)
    led.setPixelColor(HMS_STATUSLED_RGB888_RED, 0);      // Pixel 0: Red
    led.setPixelColor(HMS_STATUSLED_RGB888_GREEN, 1);    // Pixel 1: Green  
    led.setPixelColor(HMS_STATUSLED_RGB888_BLUE, 2);     // Pixel 2: Blue
    led.setPixelColor(0xFF00FF, 3);                      // Pixel 3: Magenta
    
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
    
    HAL_Delay(1000);
    
    // Demonstrate brightness control
    led.setBrightness(255);  // Full brightness
    led.show();
    HAL_Delay(500);
    
    led.setBrightness(128);  // Half brightness
    led.show();
    HAL_Delay(500);
    
    led.setBrightness(64);   // Quarter brightness
    led.show();
    HAL_Delay(500);
    
    led.setBrightness(255);  // Back to full brightness
    led.show();
    HAL_Delay(500);
    
    // Demonstrate turn off/on functionality
    led.turnOff();           // Turn off LEDs but save current state
    led.show();              // Apply the off state
    HAL_Delay(1000);
    
    led.turnOn();            // Restore previous state
    led.show();              // Apply the restored state
    HAL_Delay(1000);
    
    // Rainbow effect example
    for (uint16_t i = 0; i < 60; i++) {
        uint8_t hue = (i * 255) / 60;  // Calculate hue for rainbow
        uint32_t color = hsv_to_rgb888(hue, 255, 128);  // Convert HSV to RGB888
        led.setPixelColor(color, i);
    }
    led.show();
    
    HAL_Delay(2000);
    
    // Demonstrate breathing effect with brightness
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
        led.setBrightness(brightness);
        led.show();
        HAL_Delay(50);
    }
    
    for (int brightness = 0; brightness <= 255; brightness += 5) {
        led.setBrightness(brightness);
        led.show();
        HAL_Delay(50);
    }
    
    // Clear and turn off
    led.clear();
    led.show();
}

// Helper function to convert HSV to RGB888
uint32_t hsv_to_rgb888(uint8_t h, uint8_t s, uint8_t v) {
    uint8_t r, g, b;
    uint8_t region, remainder, p, q, t;

    if (s == 0) {
        r = g = b = v;
        return HMS_STATUSLED_RGB_TO_888(r, g, b);
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }

    return HMS_STATUSLED_RGB_TO_888(r, g, b);
}

/*
 ====================================================================================================
 * STM32CubeMX Timer Configuration Requirements:
 * 
 * 1. Enable Timer (e.g., TIM2)
 * 2. Set Mode: PWM Generation CH1 (or desired channel)
 * 3. Set Prescaler: 0 (no prescaling)
 * 4. Set Counter Period (ARR): Will be set by driver, typically ~90 for 72MHz
 * 5. Set Pulse (CCR): Will be controlled by DMA
 * 6. Enable DMA for the timer channel:
 *    - DMA Settings -> Add -> TIM2_CH1 (or your channel)
 *    - Mode: Normal
 *    - Priority: High
 *    - Data Width: Word to Word
 * 7. Set GPIO pin as TIM2_CH1 (or your timer/channel)
 * 8. No pull-up/pull-down, High speed
 * 
 * Example main.c integration:
 * 
 * int main(void) {
 *   HAL_Init();
 *   SystemClock_Config();
 *   MX_GPIO_Init();
 *   MX_DMA_Init();
 *   MX_TIM2_Init();
 *   
 *   statusLED_example();
 *   
 *   while (1) {
 *     // Your main loop
 *   }
 * }
 ====================================================================================================
 */