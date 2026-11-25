#include "buzzer.h"
#include "config.h"

#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_MODE     LEDC_LOW_SPEED_MODE
#define LEDC_TIMER    LEDC_TIMER_0
#define LEDC_CHANNEL  LEDC_CHANNEL_0
#define DUTY_ON       512  /* 10 біт: 512 з 1024 = 50 % */
#define DUTY_OFF      0

void buzzer_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode      = LEDC_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num       = LEDC_TIMER,
        .freq_hz         = 1000,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer));

    ledc_channel_config_t channel = {
        .gpio_num   = BUZZER_GPIO,
        .speed_mode = LEDC_MODE,
        .channel    = LEDC_CHANNEL,
        .timer_sel  = LEDC_TIMER,
        .duty       = DUTY_OFF,
        .hpoint     = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel));
}

void buzzer_on(uint32_t freq_hz)
{
    ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq_hz);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, DUTY_ON);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void buzzer_off(void)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, DUTY_OFF);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}
