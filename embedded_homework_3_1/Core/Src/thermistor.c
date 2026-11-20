#include "thermistor.h"
#include "main.h"
#include "config.h"
#include <math.h>

extern ADC_HandleTypeDef hadc1;

uint16_t thermistor_read_adc(void)
{
    uint16_t adc_value = 0u;

    if (HAL_ADC_Start(&hadc1) == HAL_OK)
    {
        if (HAL_ADC_PollForConversion(&hadc1, CFG_ADC_TIMEOUT_MS) == HAL_OK)
        {
            adc_value = (uint16_t)HAL_ADC_GetValue(&hadc1);
        }
        (void)HAL_ADC_Stop(&hadc1);
    }

    return adc_value;
}

bool thermistor_is_connected(uint16_t adc_value)
{
    return (adc_value >= CFG_SENSOR_BROKEN_BELOW) && (adc_value <= CFG_SENSOR_SHORTED_ABOVE);
}

float thermistor_adc_to_celsius(uint16_t adc_value)
{
    /* 1. R = R_fixed * (4095 / code - 1) */
    float ntc_resistance_ohm =
        CFG_FIXED_RESISTOR_OHM * ((CFG_ADC_MAX_VALUE / (float)adc_value) - 1.0f);

    /* 2. 1/T = 1/T25 + ln(R / R25) / B */
    float resistance_ratio = ntc_resistance_ohm / CFG_NTC_RESISTANCE_AT_25C_OHM;
    float inverse_kelvin   = (1.0f / CFG_KELVIN_AT_25C) + (logf(resistance_ratio) / CFG_NTC_BETA);
    float temperature_k    = 1.0f / inverse_kelvin;

    /* 3. °C */
    return temperature_k - CFG_KELVIN_AT_0C + CFG_CALIBRATION_OFFSET_C;
}
