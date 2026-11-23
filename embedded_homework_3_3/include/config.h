#pragma once

#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

#define POT_GPIO_NUM      4
#define LED_GPIO_NUM      5
#define MOTOR_GPIO_NUM    6

// ADC
#define POT_ADC_UNIT      ADC_UNIT_1
#define POT_ADC_CHANNEL   ADC_CHANNEL_3    // GPIO4
#define POT_ADC_ATTEN     ADC_ATTEN_DB_12  // ~0..3100 mV
#define POT_ADC_BITWIDTH  ADC_BITWIDTH_12  // RAW 0..4095
#define ADC_RAW_MAX       4095u

#define POT_SAMPLES       8u
#define POT_DEADZONE_RAW  100u // RAW < 100 = OFF

// PWM (LEDC)
#define PWM_SPEED_MODE    LEDC_LOW_SPEED_MODE
#define PWM_RESOLUTION    LEDC_TIMER_10_BIT // duty 0..1023
#define PWM_DUTY_MAX      1023u

#define LED_PWM_TIMER     LEDC_TIMER_0
#define LED_PWM_CHANNEL   LEDC_CHANNEL_0
#define LED_PWM_FREQ_HZ   5000u // 5 kHz

#define MOTOR_PWM_TIMER   LEDC_TIMER_1
#define MOTOR_PWM_CHANNEL LEDC_CHANNEL_1
#define MOTOR_PWM_FREQ_HZ 20000u // 20 kHz
#define MOTOR_MIN_DUTY    300u

#define CONTROL_PERIOD_MS 20u // 50 updates / second
#define LOG_EVERY_N       25u // 25 cycles = 500 ms
