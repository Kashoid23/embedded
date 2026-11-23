#include "pwm_output.h"
#include "config.h"

#include "driver/ledc.h"
#include "esp_err.h"

void pwmOutputInit(ledc_timer_t timer, ledc_channel_t channel, int gpioNum, uint32_t frequencyHz)
{
	const ledc_timer_config_t timerConfig = {
		.speed_mode = PWM_SPEED_MODE,
		.timer_num = timer,
		.duty_resolution = PWM_RESOLUTION,
		.freq_hz = frequencyHz,
		.clk_cfg = LEDC_AUTO_CLK,
	};
	ESP_ERROR_CHECK(ledc_timer_config(&timerConfig));

	const ledc_channel_config_t channelConfig = {
		.gpio_num = gpioNum,
		.speed_mode = PWM_SPEED_MODE,
		.channel = channel,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = timer,
		.duty = 0,
		.hpoint = 0,
	};
	ESP_ERROR_CHECK(ledc_channel_config(&channelConfig));
}

void pwmOutputSetDuty(ledc_channel_t channel, uint32_t duty)
{
	if (duty > PWM_DUTY_MAX) {
		duty = PWM_DUTY_MAX;
	}

	ESP_ERROR_CHECK(ledc_set_duty(PWM_SPEED_MODE, channel, duty));
	ESP_ERROR_CHECK(ledc_update_duty(PWM_SPEED_MODE, channel));
}
