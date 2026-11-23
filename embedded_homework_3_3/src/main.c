#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "pot_reader.h"
#include "pwm_output.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static uint32_t potToDuty(uint32_t raw, uint32_t minDuty)
{
	if (raw < POT_DEADZONE_RAW) {
		return 0;
	}

	const uint32_t position = raw - POT_DEADZONE_RAW; // 0..3995
	const uint32_t positionMax = ADC_RAW_MAX - POT_DEADZONE_RAW; // 3995
	const uint32_t dutyRange = PWM_DUTY_MAX - minDuty;

	return minDuty + position * dutyRange / positionMax;
}

static void printSettings(void)
{
	printf("\nHW 3.3 - Pot -> LED brightness + motor speed, ESP32-S3\n");
	printf("Parameter        Value\n");
	printf("--------------------------------------------\n");
	printf("Potentiometer    GPIO%d, ADC1 CH3, 12 dB, avg %u\n", POT_GPIO_NUM, (unsigned)POT_SAMPLES);
	printf("LED PWM          GPIO%d, timer 0 / ch 0, %u Hz\n", LED_GPIO_NUM, (unsigned)LED_PWM_FREQ_HZ);
	printf("Motor PWM        GPIO%d, timer 1 / ch 1, %u Hz\n", MOTOR_GPIO_NUM, (unsigned)MOTOR_PWM_FREQ_HZ);
	printf("Resolution       10 bit (0..%u)\n", (unsigned)PWM_DUTY_MAX);
	printf("Dead zone        RAW < %u -> OFF\n", (unsigned)POT_DEADZONE_RAW);
	printf("Motor min duty   %u\n", (unsigned)MOTOR_MIN_DUTY);
	printf("\n RAW   LED duty   LED %%   Motor duty   Motor %%\n");
}

void app_main(void)
{
	potReaderInit();
	pwmOutputInit(LED_PWM_TIMER, LED_PWM_CHANNEL, LED_GPIO_NUM, LED_PWM_FREQ_HZ);
	pwmOutputInit(MOTOR_PWM_TIMER, MOTOR_PWM_CHANNEL, MOTOR_GPIO_NUM, MOTOR_PWM_FREQ_HZ);
	printSettings();

	uint32_t cycle = 0;
	TickType_t lastWakeTime = xTaskGetTickCount();

	while (1) {
		const uint32_t raw = potReaderReadRaw();
		const uint32_t ledDuty = potToDuty(raw, 0);
		const uint32_t motorDuty = potToDuty(raw, MOTOR_MIN_DUTY);

		pwmOutputSetDuty(LED_PWM_CHANNEL, ledDuty);
		pwmOutputSetDuty(MOTOR_PWM_CHANNEL, motorDuty);

		if (cycle % LOG_EVERY_N == 0) {
			printf("%4u %10u %7u %12u %9u\n",
			       (unsigned)raw,
			       (unsigned)ledDuty, (unsigned)(ledDuty * 100u / PWM_DUTY_MAX),
			       (unsigned)motorDuty, (unsigned)(motorDuty * 100u / PWM_DUTY_MAX));
		}
		cycle++;

		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
	}
}
