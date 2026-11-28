#pragma once

#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

#define POT_GPIO_NUM      4
#define SERVO_GPIO_NUM    5

// ADC
#define POT_ADC_UNIT      ADC_UNIT_1
#define POT_ADC_CHANNEL   ADC_CHANNEL_3    // GPIO4
#define POT_ADC_ATTEN     ADC_ATTEN_DB_12  // ~0..3100 mV
#define POT_ADC_BITWIDTH  ADC_BITWIDTH_12  // RAW 0..4095
#define ADC_RAW_MAX       4095u

#define POT_SAMPLES       8u // average of 8 conversions

// Servo PWM (LEDC): 50 Hz = 20 ms period
#define SERVO_SPEED_MODE  LEDC_LOW_SPEED_MODE
#define SERVO_PWM_TIMER   LEDC_TIMER_0
#define SERVO_PWM_CHANNEL LEDC_CHANNEL_0
#define SERVO_RESOLUTION  LEDC_TIMER_14_BIT // duty 0..16384
#define SERVO_DUTY_STEPS  16384u            // 2^14 steps per period
#define SERVO_PWM_FREQ_HZ 50u
#define SERVO_PERIOD_US   (1000000u / SERVO_PWM_FREQ_HZ) // 20000 us

// Pulse limits: calibrate to the full travel of your servo
#define SERVO_MIN_US      500u
#define SERVO_CENTER_US   1500u
#define SERVO_MAX_US      2500u
#define SERVO_ANGLE_MAX   180u // degrees at SERVO_MAX_US

#define CONTROL_PERIOD_MS (1000u / SERVO_PWM_FREQ_HZ) // 20 ms = one PWM period
#define LOG_EVERY_N       25u                         // 25 cycles = 500 ms
