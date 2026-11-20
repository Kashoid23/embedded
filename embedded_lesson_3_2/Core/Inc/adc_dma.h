#ifndef ADC_DMA_H
#define ADC_DMA_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define ADC_DMA_CHANNEL_COUNT    (2U)
#define ADC_DMA_HALF_COUNT       (2U)
#define ADC_DMA_PAIRS_PER_HALF   (16U)
#define ADC_DMA_VALUES_PER_HALF  (ADC_DMA_PAIRS_PER_HALF * ADC_DMA_CHANNEL_COUNT)
#define ADC_DMA_BUFFER_LENGTH    (ADC_DMA_HALF_COUNT * ADC_DMA_VALUES_PER_HALF)

#define ADC_DMA_ADC_MAX          (4095U)
#define ADC_DMA_VREF_MV          (3300U)

typedef struct
{
  uint16_t potentiometer;
  uint16_t ldr;
} AdcDma_Pair;

typedef struct
{
  uint16_t samples[ADC_DMA_HALF_COUNT][ADC_DMA_PAIRS_PER_HALF];
  uint16_t last;
  uint16_t average;
  uint16_t millivolts;
} AdcDma_Channel;

typedef struct
{
  AdcDma_Pair buffer[ADC_DMA_HALF_COUNT][ADC_DMA_PAIRS_PER_HALF];
  AdcDma_Channel ldr;
  AdcDma_Channel potentiometer;
  uint32_t processed_halves;
  uint32_t dropped_halves;
  uint32_t error_code;
} AdcDma_Data;

extern volatile AdcDma_Data adc_dma;

HAL_StatusTypeDef AdcDma_Start(void);
bool AdcDma_Process(void);
HAL_StatusTypeDef AdcDma_Stop(void);

#endif
