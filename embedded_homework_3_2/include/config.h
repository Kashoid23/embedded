#pragma once

#include "esp_adc/adc_oneshot.h"

// Wiring: potentiometer wiper -> GPIO4 = ADC1 channel 3
#define POT_ADC_UNIT      ADC_UNIT_1
#define POT_ADC_CHANNEL   ADC_CHANNEL_3    // GPIO4
#define POT_ADC_ATTEN     ADC_ATTEN_DB_12  // input range ~0..3100 mV
#define POT_ADC_BITWIDTH  ADC_BITWIDTH_12  // RAW 0..4095

// Manual formula: U = RAW * ADC_FULL_SCALE_MV / ADC_RAW_MAX
#define ADC_RAW_MAX        4095
#define ADC_FULL_SCALE_MV  3100.0f  // datasheet range for 12 dB attenuation

#define SAMPLE_PERIOD_MS   100u  // one measurement every 100 ms
#define ROWS_PER_HEADER    20u   // repeat table header every 20 rows
