/*
  ====================================================================================================
  * File:        HMS_MQXXX_DRIVER.h
  * Author:      Hamas Saeed
  * Version:     Rev_1.0.0
  * Date:        Sep 28 2025
  * Brief:       This Package Provide MQXXX Driver Library for Cross Platform (STM/ESP/nRF)
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


 #ifndef HMS_MQXXX_DRIVER_H
 #define HMS_MQXXX_DRIVER_H

#if defined(ARDUINO)                                                                                       // Platform detection
  #define HMS_MQXXX_PLATFORM_ARDUINO
#elif defined(ESP_PLATFORM)
  #define HMS_MQXXX_PLATFORM_ESP_IDF
#elif defined(__ZEPHYR__)
  #define HMS_MQXXX_PLATFORM_ZEPHYR
#elif defined( __STM32__)
  #define HMS_MQXXX_PLATFORM_STM32_HAL
#endif

#if defined(HMS_MQXXX_PLATFORM_ARDUINO)
  #include <Arduino.h>
  #include <stdint.h>
  #include <float.h>
  #include <math.h>
#elif defined(HMS_MQXXX_PLATFORM_ESP_IDF)
#elif defined(HMS_MQXXX_PLATFORM_ZEPHYR)
  #include <stdio.h>
  #include <zephyr/device.h>
  #include <zephyr/drivers/i2c.h>
#elif defined(HMS_MQXXX_PLATFORM_STM32_HAL)
  #include "main.h"
  #include <math.h>
  #include <float.h>
  #include <stdio.h>
#endif

#include "HMS_MQXXX_Config.h"

#if defined(HMS_MQXXX_DEBUG_ENABLED) && (HMS_MQXXX_DEBUG_ENABLED == 1)
  #define HMS_MQXXX_LOGGER_ENABLED
#endif

typedef enum {
  HMS_MQXXX_MQ2,
  HMS_MQXXX_MQ131,
  HMS_MQXXX_MQ135,
  HMS_MQXXX_MQ303A
} HMS_MQXXX_Type;

typedef enum {
  HMS_MQXXX_LINEAR      = 2,
  HMS_MQXXX_EXPONENTIAL = 1
} HMS_MQXXX_Regression;

typedef enum {
  HMS_MQXXX_OK       = 0x00,
  HMS_MQXXX_ERROR    = 0x01,
  HMS_MQXXX_NOT_FOUND= 0x04
} HMS_MQXXX_StatusTypeDef;

class HMS_MQXXX {
  public:
    #if defined(HMS_MQXXX_PLATFORM_ARDUINO)
      HMS_MQXXX(uint8_t pin = A0, HMS_MQXXX_Type type = HMS_MQXXX_DEFAULT_TYPE);
    #elif defined(HMS_MQXXX_PLATFORM_STM32_HAL)
      HMS_MQXXX(ADC_HandleTypeDef *hadc, HMS_MQXXX_Type type = HMS_MQXXX_DEFAULT_TYPE);
    #elif defined(HMS_MQXXX_PLATFORM_ESP_IDF)
      HMS_MQXXX(uint8_t pin = 36, HMS_MQXXX_Type type = HMS_MQXXX_DEFAULT_TYPE);
    #elif defined(HMS_MQXXX_PLATFORM_ZEPHYR)
      HMS_MQXXX(uint8_t pin = 0, HMS_MQXXX_Type type = HMS_MQXXX_DEFAULT_TYPE);
    #endif

    HMS_MQXXX_StatusTypeDef init();
    HMS_MQXXX_StatusTypeDef update();
    float readSensor(float correctionFactor = 0.0);
    float setRatioAndGetPPM(float ratioValue);
    float calibrate(float ratioInCleanAir, float correctionFactor = 0.0);

    void setA(float value);
    void setB(float value);
    float setRsR0RatioGetPPM(float value);

    void setR0(float value = 10)                            { r0 = value;                 }
    void setRL(float value = 10)                            { rl = value;                 }
    void setVCC(float value = 5)                            { vcc = value;                }
    void setVoltResolution(float value = 5)                 { voltageResolution = value;  }
    void setRegressionMethod(HMS_MQXXX_Regression method)   { regression = method;        }

    float getRS();  
    float getVoltage(bool read = true, bool injected = false, int value = 0);

    float getA() const                                      { return a;                   }
    float getB() const                                      { return b;                   }
    float getR0() const                                     { return r0;                  }
    float getRL() const                                     { return rl;                  }
    float getADC() const                                    { return adc;                 }
    float getVCC() const                                    { return vcc;                 }
    float getVoltResolution() const                         { return voltageResolution;   }
    HMS_MQXXX_Type getType() const                          { return type;                }
    HMS_MQXXX_Regression getRegressionMethod() const        { return regression;          }

  private:
    #if defined(HMS_MQXXX_PLATFORM_ARDUINO)
      float           voltageResolution   = 5.0;  
      uint8_t         adcBitResolution    = 10;
      uint8_t         pin                 = A0;
    #elif defined(HMS_MQXXX_PLATFORM_ZEPHYR)
      float           voltageResolution   = 3.3;
      uint8_t         adcBitResolution    = 12;
      const struct device *adc_dev        = NULL;
      uint8_t         pin                 = 0;
      uint8_t         channel             = 0;
      int16_t         adc_raw             = 0;
      int             ret                 = 0;
    #elif defined(HMS_MQXXX_PLATFORM_ESP_IDF)
      float           voltageResolution   = 3.3;
      uint8_t         adcBitResolution    = 10;
      uint8_t         pin                 = 36;
    #elif defined(HMS_MQXXX_PLATFORM_STM32_HAL)
      float             voltageResolution   = 3.3;  
      uint8_t           adcBitResolution    = 12;
      ADC_HandleTypeDef *MQXXX_hadc;
    #endif
            
    bool                        firstFlag           = false;                // Flag for first initialization
    float                       vcc                 = 5.0;                  // Sensor supply voltage
    float                       rl                  = 10;                   // Load resistance in kilo ohms
    float                       a;                                          // Coefficient a for the equation
    float                       b;                                          // Coefficient b for the equation
    float                       adc;                                        // Raw ADC value
    float                       r0;                                         // Sensor resistance in clean air
    float                       ppm;                                        // Calculated ppm value
    float                       rsAir;                                      // Sensor resistance in clean air
    float                       ratio;                                      // Rs/R0 ratio
    float                       rsCalc;                                     // Calculated sensor resistance
    float                       sensorVolt;                                 // Sensor voltage
    uint8_t                     retries             = 2;                    // Number of read retries
    uint8_t                     retryInterval       = 20;                   // Retry interval in milliseconds
    HMS_MQXXX_Type              type;                                       // Sensor type
    HMS_MQXXX_Regression        regression;                                 // Regression method

    void mqDelay(uint32_t ms);
    void setDefaultValues();                                                // Helper function to set default sensor values
};

#endif // HMS_MQXXX_DRIVER_H