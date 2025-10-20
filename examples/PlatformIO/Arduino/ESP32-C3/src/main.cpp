#include <Arduino.h>
#include "ChronoLog.h"
#include "HMS_StatusLED_DRIVER.h"


ChronoLogger    statusLEDLogger("StatusLED", CHRONOLOG_LEVEL_INFO);
HMS_StatusLED   led(30, HMS_STATUSLED_TYPE_WS281XX, HMS_STATUSLED_ORDER_GRB);

void testColors();
void blinkEffect();
void chaserEffect();
void rainbowEffect();
void testBrightness();
void testOnOffFeature();
void testIndividualLEDs();
void testBreathingEffect();
uint32_t wheel(uint8_t wheelPos);

void setup() {
  Serial.begin(115200);
  statusLEDLogger.info("=== HMS StatusLED Driver Example ===");
  statusLEDLogger.info("Features: Colors, Effects, Brightness Control, On/Off");

  if (led.begin(5, RMT_CHANNEL_0) != HMS_STATUSLED_OK) {
    statusLEDLogger.error("Failed to initialize LED driver!");
    return;
  }

  statusLEDLogger.info("LED driver initialized successfully!");

  led.clear();                                                                                              // Clear all LEDs and show
  led.show();
    
  statusLEDLogger.info("Startup sequence...");                                                              // Quick startup sequence to show new features

  led.setPixelColor(HMS_STATUSLED_RGB888_ORANGE, 0);                                                        // Set first LED to orange at different brightness levels
  
  for (int brightness = 0; brightness <= 255; brightness += 15) {                                           // Demonstrate brightness control
    led.setBrightness(brightness);
    led.show();
    delay(50);
  }
  
  led.setBrightness(255);                                                                                   // Set full brightness for main loop
  led.show();

  statusLEDLogger.info("Ready to start main test sequence!");
  delay(1000);
}

void loop() {
    statusLEDLogger.info("=== Starting LED Test Sequence ===");

    testColors();           delay(1000);                                                                    // Test 1: Basic colors
    testIndividualLEDs();   delay(1000);                                                                    // Test 2: Individual LEDs
    chaserEffect();         delay(1000);                                                                    // Test 3: Chaser effect
    rainbowEffect();        delay(1000);                                                                    // Test 4: Rainbow effect
    blinkEffect();          delay(1000);                                                                    // Test 5: Blink effect
    testBrightness();       delay(1000);                                                                    // Test 6: NEW - Brightness control
    testOnOffFeature();     delay(1000);                                                                    // Test 7: NEW - On/Off feature
    testBreathingEffect();  delay(1000);                                                                    // Test 8: NEW - Breathing effect (combines brightness + color)

    statusLEDLogger.info("=== LED Test Sequence Complete ===");
    delay(3000);                                                                                            // Wait 3 seconds before repeating
}

void testColors() {
    statusLEDLogger.info("Testing basic colors...");
    
    statusLEDLogger.info("Red");                                                                            // Test Red
    for (int i = 0; i < 30; i++) {  led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 0, 0), i);  }
    led.show();
    delay(2000);
    
    statusLEDLogger.info("Green");                                                                          // Test Green
    for (int i = 0; i < 30; i++) {  led.setPixelColor(HMS_STATUSLED_RGB_TO_888(0, 255, 0), i);  }
    led.show();
    delay(2000);
    
    statusLEDLogger.info("Blue");                                                                           // Test Blue
    for (int i = 0; i < 30; i++) {  led.setPixelColor(HMS_STATUSLED_RGB_TO_888(0, 0, 255), i);  }
    led.show();
    delay(2000);


    statusLEDLogger.info("White");                                                                          // Test White
    for (int i = 0; i < 30; i++) {  led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 255, 255), i);  }
    led.show();
    delay(2000);
    

    statusLEDLogger.info("Orange");                                                                         // Test Orange
    for (int i = 0; i < 30; i++) {  led.setPixelColor(HMS_STATUSLED_RGB888_ORANGE, i);  }
    led.show();
    delay(2000);
    
    statusLEDLogger.info("Purple");                                                                         // Test Purple
    for (int i = 0; i < 30; i++) {  led.setPixelColor(HMS_STATUSLED_RGB_TO_888(128, 0, 128), i);  }
    led.show();
    delay(2000);
}

void blinkEffect() {
    statusLEDLogger.info("Running blink effect...");
    
    for (int blinks = 0; blinks < 5; blinks++) {
        for (int i = 0; i < 30; i++) {                                                                      // All on White
            led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 255, 255), i);
        }
        led.show();
        delay(500);
        
        led.clear();                                                                                        // All off
        led.show();
        delay(500);
    }
}

void chaserEffect() {
    statusLEDLogger.info("Running chaser effect...");

    for (int cycle = 0; cycle < 3; cycle++) {
        for (int i = 0; i < 30; i++) {                                                                      // Forward chaser
            led.clear();
            led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 100, 0), i);                                    // Orange chaser
            led.show();
            delay(100);
        }
        
        for (int i = 29; i >= 0; i--) {                                                                     // Reverse chaser
            led.clear();
            led.setPixelColor(HMS_STATUSLED_RGB_TO_888(0, 100, 255), i);                                    // Blue chaser
            led.show();
            delay(100);
        }
    }
}

void rainbowEffect() {
    statusLEDLogger.info("Running rainbow effect...");
    
    for (int cycle = 0; cycle < 2; cycle++) {
        for (int hue = 0; hue < 256; hue += 4) {
            for (int i = 0; i < 30; i++) {
                uint32_t color = wheel((hue + i * 8) & 255);
                led.setPixelColor(color, i);
            }
            led.show();
            delay(50);
        }
    }
}

void testBrightness() {
    statusLEDLogger.info("Testing brightness control...");
    
    for (int i = 0; i < 30; i++) {                                                                          // Set all LEDs to a nice rainbow pattern first
        uint32_t color = wheel((i * 8) & 255);
        led.setPixelColor(color, i);
    }
    
    statusLEDLogger.info("Full brightness (255)");                                                          // Test different brightness levels
    led.setBrightness(255);
    led.show();
    delay(1500);

    statusLEDLogger.info("75% brightness (192)");
    led.setBrightness(192);
    led.show();
    delay(1500);

    statusLEDLogger.info("50% brightness (128)");
    led.setBrightness(128);
    led.show();
    delay(1500);

    statusLEDLogger.info("25% brightness (64)");
    led.setBrightness(64);
    led.show();
    delay(1500);

    statusLEDLogger.info("10% brightness (26)");
    led.setBrightness(26);
    led.show();
    delay(1500);
    
    
    statusLEDLogger.info("Smooth fade down...");                                                            // Smooth brightness fade
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
        led.setBrightness(brightness);
        led.show();
        delay(30);
    }

    statusLEDLogger.info("Smooth fade up...");
    for (int brightness = 0; brightness <= 255; brightness += 5) {
        led.setBrightness(brightness);
        led.show();
        delay(30);
    }
    
    led.setBrightness(255);                                                                                 // Reset to full brightness
    led.show();
}

void testOnOffFeature() {
    statusLEDLogger.info("Testing on/off feature...");
    
    led.setPixelColor(HMS_STATUSLED_RGB888_RED, 0);                                                         // Create a colorful pattern
    led.setPixelColor(HMS_STATUSLED_RGB888_GREEN, 1);
    led.setPixelColor(HMS_STATUSLED_RGB888_BLUE, 2);
    led.setPixelColor(HMS_STATUSLED_RGB888_ORANGE, 3);
    led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 255, 0), 4);                                            // Yellow
    led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 0, 255), 5);                                            // Magenta
    led.setPixelColor(HMS_STATUSLED_RGB_TO_888(0, 255, 255), 6);                                            // Cyan
    led.setPixelColor(HMS_STATUSLED_RGB_TO_888(255, 255, 255), 7);                                          // White

    for (int i = 8; i < 30; i++) {
        uint32_t color = wheel((i * 16) & 255);
        led.setPixelColor(color, i);
    }
    
    statusLEDLogger.info("Showing colorful pattern...");
    led.show();
    delay(2000);
    
    statusLEDLogger.info("Turning OFF (saves current state)...");                                           // Test turn off feature
    led.turnOff();
    led.show();
    delay(2000);

    statusLEDLogger.info("Turning ON (restores saved state)...");
    led.turnOn();
    led.show();
    delay(2000);
    
    for (int cycle = 0; cycle < 3; cycle++) {                                                               // Test multiple on/off cycles
        statusLEDLogger.info("On/Off cycle " + String(cycle + 1));

        led.turnOff();
        led.show();
        delay(500);
        
        led.turnOn();
        led.show();
        delay(500);
    }

    statusLEDLogger.info("On/Off test complete");
}

void testIndividualLEDs() {
    statusLEDLogger.info("Testing individual LEDs...");
    led.clear();
    
    for (int i = 0; i < 30; i++) {
        statusLEDLogger.info("LED ");
        statusLEDLogger.info(i);

        led.clear();
        led.setPixelColor(HMS_STATUSLED_RGB_TO_888(0, 255, 0), i);                                          // Green for current LED
        led.show();
        delay(300);
    }
    
    led.clear();
    led.show();
    delay(500);
}

void testBreathingEffect() {
    statusLEDLogger.info("Running breathing effect with brightness control...");
    
    for (int i = 0; i < 30; i++) {                                                                          // Set a solid color pattern
        led.setPixelColor(HMS_STATUSLED_RGB_TO_888(0, 100, 255), i);                                        // Nice blue color
    }
    
    for (int cycle = 0; cycle < 3; cycle++) {                                                               // Breathing effect using brightness control
        for (int brightness = 0; brightness <= 255; brightness += 3) {                                      // Breathe in (fade up)
            led.setBrightness(brightness);
            led.show();
            delay(20);
        }
        
        delay(200);                                                                                         // Hold
        
        for (int brightness = 255; brightness >= 0; brightness -= 3) {                                      // Breathe out (fade down)
            led.setBrightness(brightness);
            led.show();
            delay(20);
        }
        
        delay(200);                                                                                         // Hold
    }
    
    led.setBrightness(255);                                                                                 // Reset to full brightness
    led.show();
}

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