#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "pot_reader.h"
#include "servo.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// RAW 0..4095 -> 500..2500 us (rounded), same formula as the STM32 version.
static uint32_t potToPulseUs(uint32_t raw)
{
	const uint32_t pulseRange = SERVO_MAX_US - SERVO_MIN_US; // 2000

	return SERVO_MIN_US + (raw * pulseRange + ADC_RAW_MAX / 2u) / ADC_RAW_MAX;
}

static uint32_t pulseToAngle(uint32_t pulseUs)
{
	return (pulseUs - SERVO_MIN_US) * SERVO_ANGLE_MAX / (SERVO_MAX_US - SERVO_MIN_US);
}

static void printSettings(void)
{
	printf("\nHW 3.5 - Pot -> servo position, ESP32-S3\n");
	printf("Parameter        Value\n");
	printf("--------------------------------------------\n");
	printf("Potentiometer    GPIO%d, ADC1 CH3, 12 dB, avg %u\n", POT_GPIO_NUM, (unsigned)POT_SAMPLES);
	printf("Servo PWM        GPIO%d, timer 0 / ch 0, %u Hz\n", SERVO_GPIO_NUM, (unsigned)SERVO_PWM_FREQ_HZ);
	printf("Resolution       14 bit (%u steps / %u us)\n", (unsigned)SERVO_DUTY_STEPS, (unsigned)SERVO_PERIOD_US);
	printf("Pulse range      %u..%u us (center %u)\n",
	       (unsigned)SERVO_MIN_US, (unsigned)SERVO_MAX_US, (unsigned)SERVO_CENTER_US);
	printf("Update period    %u ms\n", (unsigned)CONTROL_PERIOD_MS);
	printf("\n RAW   Pulse us   Duty   Angle\n");
}

void app_main(void)
{
	potReaderInit();
	servoInit();
	printSettings();

	uint32_t cycle = 0;
	TickType_t lastWakeTime = xTaskGetTickCount();

	while (1) {
		const uint32_t raw = potReaderReadRaw();
		const uint32_t pulseUs = potToPulseUs(raw);
		const uint32_t duty = servoSetPulseUs(pulseUs);

		if (cycle % LOG_EVERY_N == 0) {
			printf("%4u %10u %6u %6u\n",
			       (unsigned)raw, (unsigned)pulseUs, (unsigned)duty, (unsigned)pulseToAngle(pulseUs));
		}
		cycle++;

		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
	}
}
