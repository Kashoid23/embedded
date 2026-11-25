#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h" /* CMSIS: адреси регістрів і маски бітів. */

/* Частоти таймерів і період оновлення серво. */
#define SYSTICK_HZ         1000U
#define SERVO_TIMER_HZ     1000000U
#define SERVO_PWM_HZ       50U
#define SERVO_UPDATE_MS    (1000U / SERVO_PWM_HZ)

/* Межі імпульсу: калібрування повного ходу серво. */
#define SERVO_MIN_US       500U
#define SERVO_CENTER_US    1500U
#define SERVO_MAX_US       2500U
#define ADC_MAX_VALUE      4095U
#define ADC_AVERAGE_SAMPLES 8U

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
