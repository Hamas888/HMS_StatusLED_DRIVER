#include "HMS_StatusLED_DRIVER.h"

#ifdef HMS_STATUSLED_LOGGER_ENABLED
  #include "ChronoLog.h"
  ChronoLoger statusLEDLogger("HMS_StatusLED", HMS_STATUSLED_DEBUG_ENABLED);
#endif

#if defined(HMS_STATUSLED_PLATFORM_STM32_HAL)
  TIM_HandleTypeDef* HMS_StatusLED::statusLED_hTim = nullptr;
#endif

#if (HMS_STATUSLED_GAMMA == true)
const uint8_t gammaLut[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
    2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
    6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
    11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
    19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
    29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
    40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
    90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
    111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
    135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
    162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
    191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
    222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255
};

#endif

HMS_StatusLED::HMS_StatusLED(uint16_t maxPixels, HMS_StatusLED_Type type, HMS_StatusLED_OrderType colorOrder) 
  : maxPixel(maxPixels), ledType(type), colorOrder(colorOrder) {
  #ifdef HMS_STATUSLED_LOGGER_ENABLED
    statusLEDLogger.debug("HMS_StatusLED Driver Instance created");
  #endif
    if (type == HMS_STATUSLED_TYPE_WS281XX) {
        #if defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32) || defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
            rmtItems = (rmt_item32_t*)malloc(maxPixel * 24 * sizeof(rmt_item32_t));                                 // For ESP32, we'll use RMT items for efficient transmission
        #else
            buffer.resize((maxPixel * 24) + 50, 0);                                                                 // For STM32 HAL, we'll use a buffer for DMA transmission
        #endif
        pixel.resize(maxPixel, std::vector<uint8_t>(3, 0));
    }
}

HMS_StatusLED::~HMS_StatusLED() {
    #ifdef HMS_STATUSLED_LOGGER_ENABLED
      statusLEDLogger.debug("HMS_StatusLED Driver Instance destroyed");
    #endif
    
    #if defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32) || defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
        if (rmtItems) {
            free(rmtItems);
            rmtItems = nullptr;
        }
        rmt_driver_uninstall(rmtChannel);                                                                           // Deinitialize RMT channel
    #else
        buffer.clear();
    #endif
    pixel.clear();
}

#if defined(HMS_STATUSLED_PLATFORM_ARDUINO)
#elif defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32) || defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
HMS_StatusLED_StatusTypeDef HMS_StatusLED::begin(uint8_t pin, rmt_channel_t channel) {
    outputPin = pin;
    rmtChannel = channel;

    rmt_config_t rmtConfig = {};                                                                                    // Configure RMT for WS2812B timing
    rmtConfig.rmt_mode = RMT_MODE_TX;
    rmtConfig.channel = rmtChannel;
    rmtConfig.gpio_num = (gpio_num_t)pin;
    rmtConfig.clk_div = 2;                                                                                          // 40MHz (80MHz / 2)
    rmtConfig.mem_block_num = 1;
    rmtConfig.flags = 0;
    
    rmtConfig.tx_config.carrier_en = false;
    rmtConfig.tx_config.loop_en = false;
    rmtConfig.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    rmtConfig.tx_config.idle_output_en = true;
    
    esp_err_t result = rmt_config(&rmtConfig);                                                                      // Configure RMT
    if (result != ESP_OK) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
            statusLEDLogger.debug("Error: RMT configuration failed");
        #endif
        return HMS_STATUSLED_ERROR;
    }
    
    result = rmt_driver_install(rmtChannel, 0, 0);                                                                  // Install RMT driver
    if (result != ESP_OK) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
            statusLEDLogger.debug("Error: RMT driver installation failed");
        #endif
        return HMS_STATUSLED_ERROR;
    }
    
    clear();                                                                                                        // Clear pixels
    
    #ifdef HMS_STATUSLED_LOGGER_ENABLED
        statusLEDLogger.debug("ESP32 RMT Driver Started on pin %d, channel %d", pin, rmtChannel);
    #endif
    
    return HMS_STATUSLED_OK;
}

void HMS_StatusLED::updateRMTBuffer() {
    if (!rmtItems) return;
    
    uint32_t itemIndex = 0;
    
    for (uint16_t pixelIdx = 0; pixelIdx < maxPixel; pixelIdx++) {                                                  // Convert pixel data to RMT items
        for (uint8_t colorComponent = 0; colorComponent < 3; colorComponent++) {
            uint8_t colorValue = pixel[pixelIdx][colorComponent];
            
            for (int8_t bit = 7; bit >= 0; bit--) {                                                                 // Convert each bit to RMT item (WS2812B timing)
                if (colorValue & (1 << bit)) {
                    rmtItems[itemIndex].level0 = 1;                                                                 // High bit: T1H=0.8µs, T1L=0.45µs (32 ticks, 18 ticks at 40MHz)
                    rmtItems[itemIndex].duration0 = 32;                                                             // 0.8µs
                    rmtItems[itemIndex].level1 = 0;
                    rmtItems[itemIndex].duration1 = 18;                                                             // 0.45µs
                } else {
                    rmtItems[itemIndex].level0 = 1;                                                                 // Low bit: T0H=0.4µs, T0L=0.85µs (16 ticks, 34 ticks at 40MHz)
                    rmtItems[itemIndex].duration0 = 16;                                                             // 0.4µs
                    rmtItems[itemIndex].level1 = 0;
                    rmtItems[itemIndex].duration1 = 34;                                                             // 0.85µs
                }
                itemIndex++;
            }
        }
    }
}

HMS_StatusLED_StatusTypeDef HMS_StatusLED::show() {
    updateRMTBuffer();                                                                                              // Update RMT buffer with current pixel data
    
    esp_err_t result = rmt_write_items(rmtChannel, rmtItems, maxPixel * 24, true);                                  // Send data via RMT
    if (result != ESP_OK) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
            statusLEDLogger.debug("Error: RMT transmission failed");
        #endif
        return HMS_STATUSLED_ERROR;
    }
    
    #ifdef HMS_STATUSLED_LOGGER_ENABLED
        statusLEDLogger.debug("LED data sent via RMT");
    #endif
    
    return HMS_STATUSLED_OK;
}
#elif defined(HMS_STATUSLED_PLATFORM_ZEPHYR)
#elif defined(HMS_STATUSLED_PLATFORM_STM32_HAL)
void HMS_StatusLED::updateDMABuffer() {
    uint32_t bufferIndex = 0;
    
    // Convert pixel data to PWM duty cycles for DMA
    for (uint16_t i = 0; i < maxPixel; i++) {
        for (uint8_t colorComponent = 0; colorComponent < 3; colorComponent++) {
            uint8_t colorValue = pixel[i][colorComponent];
            
            // Convert each bit of the color value to PWM duty cycle
            for (int8_t bit = 7; bit >= 0; bit--) {
                if (colorValue & (1 << bit)) {
                    buffer[bufferIndex] = pulse1;  // High bit (T1H)
                } else {
                    buffer[bufferIndex] = pulse0;  // Low bit (T0H)
                }
                bufferIndex++;
            }
        }
    }
    
    // Add reset pulse (50µs of low) - WS2812B needs >50µs reset time
    for (uint16_t i = 0; i < 50; i++) {
        if (bufferIndex < buffer.size()) {
            buffer[bufferIndex++] = 0;
        }
    }
}

HMS_StatusLED_StatusTypeDef HMS_StatusLED::show() {
    if (!statusLED_hTim) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("Error: Timer not initialized. Call begin() first.");
        #endif
        return HMS_STATUSLED_ERROR;
    }
    
    // Update DMA buffer with current pixel data
    updateDMABuffer();
    
    // Start DMA transfer
    HAL_StatusTypeDef halStatus = HAL_TIM_PWM_Start_DMA(
        statusLED_hTim, 
        timerChannel, 
        (uint32_t*)buffer.data(), 
        buffer.size()
    );
    
    if (halStatus != HAL_OK) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("Error: Failed to start DMA transfer");
        #endif
        return HMS_STATUSLED_ERROR;
    }
    
    #ifdef HMS_STATUSLED_LOGGER_ENABLED
      statusLEDLogger.debug("LED data sent via DMA");
    #endif
    
    return HMS_STATUSLED_OK;
}

HMS_StatusLED_StatusTypeDef HMS_StatusLED::begin(TIM_HandleTypeDef *hTim, uint16_t timerBusFrequencyMHz, uint8_t channel) {
    if(!hTim) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("Error: Invalid Timer Handle");
        #endif
        return HMS_STATUSLED_ERROR;
    }

    // Validate channel (STM32 HAL uses TIM_CHANNEL_x constants)
    if(channel != TIM_CHANNEL_1 && channel != TIM_CHANNEL_2 && 
       channel != TIM_CHANNEL_3 && channel != TIM_CHANNEL_4) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("Error: Invalid Timer Channel");
        #endif
        return HMS_STATUSLED_ERROR;
    }

    statusLED_hTim = hTim;
    timerChannel = channel;

    // Calculate timer values based on frequency and WS2812B timing requirements
    // WS2812B timing: T0H=0.4µs, T0L=0.85µs, T1H=0.8µs, T1L=0.45µs, Period=1.25µs
    float timerFrequencyMHz = (float)timerBusFrequencyMHz;
    autoReloadValue = (uint32_t)((timerFrequencyMHz * HMS_STATUSLED_PULSE_LENGTH_NS) / 1000.0f) - 1;
    
    // Calculate pulse widths
    pulse0 = (uint16_t)((timerFrequencyMHz * HMS_STATUSLED_PULSE_0_NS) / 1000.0f);
    pulse1 = (uint16_t)((timerFrequencyMHz * HMS_STATUSLED_PULSE_1_NS) / 1000.0f);

    // Configure timer
    __HAL_TIM_SET_AUTORELOAD(hTim, autoReloadValue);
    __HAL_TIM_SET_PRESCALER(hTim, 0);

    // Clear buffers
    std::fill(buffer.begin(), buffer.end(), 0);
    for(auto& pixelData : pixel) {
        std::fill(pixelData.begin(), pixelData.end(), 0);
    }

    // Initialize DMA buffer with reset values (low for reset pulse)
    updateDMABuffer();

    #ifdef HMS_STATUSLED_LOGGER_ENABLED
      statusLEDLogger.debug("Timer configured: ARR=%lu, Pulse0=%d, Pulse1=%d", autoReloadValue, pulse0, pulse1);
      statusLEDLogger.debug("HMS_StatusLED Driver Started");
    #endif

    return HMS_STATUSLED_OK;
}
#endif 

HMS_StatusLED_StatusTypeDef HMS_StatusLED::setPixelColor(uint32_t color, uint16_t pixelIndex) {
    return setPixelColor(color, pixelIndex, colorOrder);
}

HMS_StatusLED_StatusTypeDef HMS_StatusLED::setPixelColor(uint32_t color, uint16_t pixelIndex, HMS_StatusLED_OrderType colorOrder) {
    // Validate pixel index
    if (pixelIndex >= maxPixel) {
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("Error: Pixel index out of range");
        #endif
        return HMS_STATUSLED_ERROR;
    }

    uint8_t r, g, b;
    
    // Auto-detect color format based on value range
    // RGB565: max value is 0xFFFF (65535)
    // RGB888: max value is 0xFFFFFF (16777215)
    if (color <= 0xFFFF) {
        // Detected RGB565 format
        r = HMS_STATUSLED_GET_RED_565(color);
        g = HMS_STATUSLED_GET_GREEN_565(color);
        b = HMS_STATUSLED_GET_BLUE_565(color);
        
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("RGB565 color detected");
        #endif
    } else {
        // Detected RGB888 format
        r = HMS_STATUSLED_GET_RED_888(color);
        g = HMS_STATUSLED_GET_GREEN_888(color);
        b = HMS_STATUSLED_GET_BLUE_888(color);
        
        #ifdef HMS_STATUSLED_LOGGER_ENABLED
          statusLEDLogger.debug("RGB888 color detected");
        #endif
    }

    // Apply gamma correction if enabled
    #if (HMS_STATUSLED_GAMMA == true)
        r = gammaLut[r];
        g = gammaLut[g];
        b = gammaLut[b];
    #endif

    // Load pixel vector according to selected color order
    switch (colorOrder) {
        case HMS_STATUSLED_ORDER_RGB:
            pixel[pixelIndex][0] = r;
            pixel[pixelIndex][1] = g;
            pixel[pixelIndex][2] = b;
            break;
            
        case HMS_STATUSLED_ORDER_BGR:
            pixel[pixelIndex][0] = b;
            pixel[pixelIndex][1] = g;
            pixel[pixelIndex][2] = r;
            break;
            
        case HMS_STATUSLED_ORDER_GRB:
            pixel[pixelIndex][0] = g;
            pixel[pixelIndex][1] = r;
            pixel[pixelIndex][2] = b;
            break;
            
        default:
            // Default to RGB order
            pixel[pixelIndex][0] = r;
            pixel[pixelIndex][1] = g;
            pixel[pixelIndex][2] = b;
            break;
    }

    #ifdef HMS_STATUSLED_LOGGER_ENABLED
        statusLEDLogger.debug("Pixel %d set to R:%d G:%d B:%d (Order: %d)", pixelIndex, r, g, b, colorOrder);
    #endif

    return HMS_STATUSLED_OK;
}

void HMS_StatusLED::setColorOrder(HMS_StatusLED_OrderType order) {
    colorOrder = order;
    
    #ifdef HMS_STATUSLED_LOGGER_ENABLED
      char logMessage[50];
      sprintf(logMessage, "Color order set to: %d", order);
      statusLEDLogger.debug(logMessage);
    #endif
}

void HMS_StatusLED::clear() {
    // Clear all pixel data
    for (auto& pixelData : pixel) {
        std::fill(pixelData.begin(), pixelData.end(), 0);
    }
    
    #ifdef HMS_STATUSLED_LOGGER_ENABLED
      statusLEDLogger.debug("All pixels cleared");
    #endif
}