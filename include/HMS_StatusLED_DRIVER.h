#ifndef HMS_STATUSLED_DRIVER_H
#define HMS_STATUSLED_DRIVER_H

#if defined(ARDUINO)                                                                                       // Platform detection
  #define HMS_STATUSLED_PLATFORM_ARDUINO
  #if defined(ESP32)
    #define HMS_STATUSLED_PLATFORM_ARDUINO_ESP32
  #endif
#elif defined(ESP_PLATFORM)
  #define HMS_STATUSLED_PLATFORM_ESP_IDF
#elif defined(__ZEPHYR__)
  #define HMS_STATUSLED_PLATFORM_ZEPHYR
#elif defined( __STM32__)
  #define HMS_STATUSLED_PLATFORM_STM32_HAL
#endif

#if defined(HMS_STATUSLED_PLATFORM_ARDUINO)
  #include <vector>
  #include <stdio.h>
  #include <stdint.h>
  #include <Arduino.h>
  #if defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32)
    #include <driver/rmt.h>
  #endif
#elif defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
  #include <stdio.h>
  #include <stdint.h>
  #include <vector>
  #include "driver/rmt.h"
  #include <vector>
  #include <stdio.h>
  #include <stdint.h>
  #include <zephyr/device.h>
  #include <zephyr/drivers/i2c.h>
#elif defined(HMS_STATUSLED_PLATFORM_STM32_HAL)
  #include "main.h"
  #include <vector>
  #include <stdio.h>
  #include <stdint.h>
#endif

#include "HMS_StatusLED_Config.h"

#if defined(HMS_STATUSLED_DEBUG_ENABLED) && (HMS_STATUSLED_DEBUG_ENABLED == 1)
  #define HMS_STATUSLED_LOGGER_ENABLED
#endif

typedef enum {
  HMS_STATUSLED_TYPE_WS281XX = 0
} HMS_StatusLED_Type;

typedef enum {
  HMS_STATUSLED_OK       = 0x00,
  HMS_STATUSLED_ERROR    = 0x01,
} HMS_StatusLED_StatusTypeDef;

typedef enum {
  HMS_STATUSLED_ORDER_RGB = 0,
  HMS_STATUSLED_ORDER_BGR = 1,
  HMS_STATUSLED_ORDER_GRB = 2,
} HMS_StatusLED_OrderType;

class HMS_StatusLED {
  public:
  HMS_StatusLED(
      uint16_t maxPixels = HMS_STATUSLED_MAX_PIXEL_COUNT,
      HMS_StatusLED_Type type = HMS_STATUSLED_TYPE_WS281XX, 
      HMS_StatusLED_OrderType colorOrder = HMS_STATUSLED_DEFAULT_COLOR_ORDER
    );
    ~HMS_StatusLED();

    #if defined(HMS_STATUSLED_PLATFORM_ARDUINO)
      #if defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32)
        HMS_StatusLED_StatusTypeDef begin(uint8_t pin, rmt_channel_t rmtChannel = RMT_CHANNEL_0);
      #else
        HMS_StatusLED_StatusTypeDef begin(uint8_t pin);
      #endif
    #elif defined(HMS_STATUSLED_PLATFORM_ZEPHYR)
    #elif defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
        HMS_StatusLED_StatusTypeDef begin(uint8_t pin, rmt_channel_t rmtChannel = RMT_CHANNEL_0);
    #elif defined(HMS_STATUSLED_PLATFORM_STM32_HAL)
        HMS_StatusLED_StatusTypeDef begin(TIM_HandleTypeDef *hTim, uint16_t timerBusFrequencyMHz, uint8_t channel);
    #endif

    void clear();
    // void turnOn(void);
    // void turnOff(void);
    // void setBrightness(uint8_t brightness);
    void setColorOrder(HMS_StatusLED_OrderType order);

    HMS_StatusLED_StatusTypeDef show();
    HMS_StatusLED_StatusTypeDef setPixelColor(uint32_t color, uint16_t pixelIndex);
    HMS_StatusLED_StatusTypeDef setPixelColor(uint32_t color, uint16_t pixelIndex, HMS_StatusLED_OrderType colorOrder);

  private:
    #if defined(HMS_STATUSLED_PLATFORM_ARDUINO)
      #if defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32) || defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
        rmt_channel_t                   rmtChannel;
        uint8_t                         outputPin;
        rmt_item32_t*                   rmtItems;
      #else
        uint8_t                         outputPin;
      #endif
    #elif defined(HMS_STATUSLED_PLATFORM_ZEPHYR)
    #elif defined(HMS_STATUSLED_PLATFORM_STM32_HAL)
      uint8_t                           timerChannel         = 0;
      uint32_t                          autoReloadValue      = 0;
      static TIM_HandleTypeDef          *statusLED_hTim;
    #endif

    uint16_t                            pulse0              = 0;
    uint16_t                            pulse1              = 0;
    uint16_t                            maxPixel;
    HMS_StatusLED_Type                  ledType;
    HMS_StatusLED_OrderType             colorOrder;
    std::vector<uint8_t>                buffer;
    std::vector<std::vector<uint8_t>>   pixel;

    #if defined(HMS_STATUSLED_PLATFORM_ARDUINO_ESP32) || defined(HMS_STATUSLED_PLATFORM_ESP_IDF)
      void updateRMTBuffer();                                                                                                     // Convert pixel data to RMT format
    #elif defined(HMS_STATUSLED_PLATFORM_STM32_HAL)
      void updateDMABuffer();                                                                                                     // Convert pixel data to DMA buffer format
    #endif
};

#endif // HMS_STATUSLED_DRIVER_H