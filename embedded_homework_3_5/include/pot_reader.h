#pragma once

#include <stdint.h>

// Creates ADC1 unit and configures the potentiometer channel.
void potReaderInit(void);

// Average of POT_SAMPLES conversions: RAW 0..4095.
uint32_t potReaderReadRaw(void);