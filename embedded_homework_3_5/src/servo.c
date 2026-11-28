#include "servo.h"
#include "config.h"

#include "driver/ledc.h"
#include "esp_err.h"

// us -> LEDC duty: duty = pulse * 16384 / 20000 (rounded).
// One duty step = 20000 / 16384 = ~1.22 us.
static uint32_t pulseUsToDuty(uint32_t pulseUs)
{
	return (pulseUs * SERVO_DUTY_STEPS + SERVO_PERIOD_US / 2u) / SERVO_PERIOD_US;
}

void servoInit(void)
{
	const ledc_timer_config_t timerConfig = {
		.speed_mode = SERVO_SPEED_MODE,
		.timer_num = SERVO_PWM_TIMER,
		.duty_resolution = SERVO_RESOLUTION,
		.freq_hz = SERVO_PWM_FREQ_HZ,
		.clk_cfg = LEDC_AUTO_CLK,
	};
	ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

	// First pulse is already the center position: the servo does not jump to an edge.
	const ledc_channel_config_t channelConfig = {
		.gpio_num = SERVO_GPIO_NUM,
		.speed_mode = SERVO_SPEED_MODE,
		.channel = SERVO_PWM_CHANNEL,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = SERVO_PWM_TIMER,
		.duty = pulseUsToDuty(SERVO_CENTER_US),
		.hpoint = 0,
	};
	ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
}

uint32_t servoSetPulseUs(uint32_t pulseUs)
{
	if (pulseUs < SERVO_MIN_US) {
		pulseUs = SERVO_MIN_US;
	}
	if (pulseUs > SERVO_MAX_US) {
		pulseUs = SERVO_MAX_US;
	}

	const uint32_t duty = pulseUsToDuty(pulseUs);

	// New duty is applied at the start of the next PWM period (no cut pulse).
	ESP_ERROR_CHECK(ledc_set_duty(SERVO_SPEED_MODE, SERVO_PWM_CHANNEL, duty));
	ESP_ERROR_CHECK(ledc_update_duty(SERVO_SPEED_MODE, SERVO_PWM_CHANNEL));

	return duty;
}
