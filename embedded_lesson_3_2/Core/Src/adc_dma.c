#include "adc_dma.h"
#include "main.h"

volatile AdcDma_Data adc_dma;

typedef struct
{
  volatile uint32_t completed_halves;
  volatile uint32_t ready_half;
  volatile uint32_t error;
  uint32_t consumed_halves;
  bool running;
} AdcDma_State;

static AdcDma_State adc_dma_state;

static uint32_t GetDmaActiveHalf(void)
{
  const uint32_t remaining = __HAL_DMA_GET_COUNTER(hadc1.DMA_Handle);
  uint32_t active_half;

  if ((remaining == 0U) || (remaining > ADC_DMA_VALUES_PER_HALF))
  {
    active_half = 0U;
  }
  else
  {
    active_half = 1U;
  }

  return active_half;
}

static void UpdateChannel(volatile AdcDma_Channel *channel, uint32_t half,
                          const uint16_t samples[ADC_DMA_PAIRS_PER_HALF],
                          uint32_t sum)
{
  const uint32_t average =
    (sum + (ADC_DMA_PAIRS_PER_HALF / 2U)) / ADC_DMA_PAIRS_PER_HALF;
  const uint32_t millivolts =
    ((average * ADC_DMA_VREF_MV) + (ADC_DMA_ADC_MAX / 2U)) / ADC_DMA_ADC_MAX;
  uint32_t index;

  for (index = 0U; index < ADC_DMA_PAIRS_PER_HALF; index++)
  {
    channel->samples[half][index] = samples[index];
  }

  channel->last = samples[ADC_DMA_PAIRS_PER_HALF - 1U];
  channel->average = (uint16_t)average;
  channel->millivolts = (uint16_t)millivolts;
}

static bool ProcessHalf(uint32_t sequence, uint32_t half)
{
  uint16_t ldr[ADC_DMA_PAIRS_PER_HALF];
  uint16_t potentiometer[ADC_DMA_PAIRS_PER_HALF];
  uint32_t ldr_sum = 0U;
  uint32_t potentiometer_sum = 0U;
  uint32_t index;
  bool is_sequence_unchanged;
  bool is_half_idle;
  bool is_valid;

  for (index = 0U; index < ADC_DMA_PAIRS_PER_HALF; index++)
  {
    const AdcDma_Pair pair = adc_dma.buffer[half][index];

    ldr[index] = pair.ldr;
    potentiometer[index] = pair.potentiometer;
    ldr_sum += pair.ldr;
    potentiometer_sum += pair.potentiometer;
  }

  is_sequence_unchanged = (adc_dma_state.completed_halves == sequence);
  is_half_idle = (GetDmaActiveHalf() != half);
  is_valid = is_sequence_unchanged && is_half_idle;

  if (is_valid)
  {
    UpdateChannel(&adc_dma.ldr, half, ldr, ldr_sum);
    UpdateChannel(&adc_dma.potentiometer, half, potentiometer,
                  potentiometer_sum);

    adc_dma.dropped_halves += (sequence - adc_dma_state.consumed_halves) - 1U;
    adc_dma_state.consumed_halves = sequence;
    adc_dma.processed_halves++;
  }

  return is_valid;
}

HAL_StatusTypeDef AdcDma_Start(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (adc_dma_state.running)
  {
    status = HAL_BUSY;
  }
  else
  {
    adc_dma_state = (AdcDma_State){ 0 };
    adc_dma = (AdcDma_Data){ 0 };

    __HAL_TIM_SET_COUNTER(&htim2, 0U);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_dma.buffer,
                          ADC_DMA_BUFFER_LENGTH) != HAL_OK)
    {
      (void)HAL_ADC_Stop_DMA(&hadc1);
      status = HAL_ERROR;
    }
    else
    {
      __HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_OVR);
      adc_dma_state.running = true;

      if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
      {
        (void)AdcDma_Stop();
        status = HAL_ERROR;
      }
    }
  }

  return status;
}

bool AdcDma_Process(void)
{
  bool is_processed = false;

  if (adc_dma_state.running)
  {
    const bool is_overrun = __HAL_ADC_GET_FLAG(&hadc1, ADC_FLAG_OVR);
    const uint32_t error = adc_dma_state.error;

    if (is_overrun)
    {
      adc_dma.error_code = HAL_ADC_ERROR_OVR;
      (void)AdcDma_Stop();
    }
    else if (error != HAL_ADC_ERROR_NONE)
    {
      adc_dma.error_code = error;
      (void)AdcDma_Stop();
    }
    else
    {
      const uint32_t sequence = adc_dma_state.completed_halves;
      const uint32_t half = adc_dma_state.ready_half;

      if ((sequence != adc_dma_state.consumed_halves) &&
          (half < ADC_DMA_HALF_COUNT))
      {
        is_processed = ProcessHalf(sequence, half);
      }
    }
  }

  return is_processed;
}

HAL_StatusTypeDef AdcDma_Stop(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  HAL_StatusTypeDef timer_status;
  HAL_StatusTypeDef adc_status;

  timer_status = HAL_TIM_Base_Stop(&htim2);
  adc_status = HAL_ADC_Stop_DMA(&hadc1);
  adc_dma_state.running = false;

  if ((timer_status != HAL_OK) || (adc_status != HAL_OK))
  {
    status = HAL_ERROR;
  }

  return status;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc == &hadc1)
  {
    adc_dma_state.ready_half = 0U;
    adc_dma_state.completed_halves++;
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc == &hadc1)
  {
    adc_dma_state.ready_half = 1U;
    adc_dma_state.completed_halves++;
  }
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc == &hadc1)
  {
    const uint32_t error = HAL_ADC_GetError(hadc);

    if (error != HAL_ADC_ERROR_NONE)
    {
      adc_dma_state.error = error;
    }
    else
    {
      adc_dma_state.error = HAL_ADC_ERROR_INTERNAL;
    }
  }
}
