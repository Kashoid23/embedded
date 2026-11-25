#pragma once

#include <stdint.h>

void buzzer_init(void);
void buzzer_on(uint32_t freq_hz);
void buzzer_off(void);
