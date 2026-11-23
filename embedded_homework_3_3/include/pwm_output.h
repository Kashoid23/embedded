#pragma once

#include <stdint.h>

#include "driver/ledc.h"

void pwmOutputInit(ledc_timer_t timer, ledc_channel_t channel, int gpioNum, uint32_t frequencyHz);
void pwmOutputSetDuty(ledc_channel_t channel, uint32_t duty);
