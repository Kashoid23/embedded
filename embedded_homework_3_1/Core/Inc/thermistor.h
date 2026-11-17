#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <stdint.h>
#include <stdbool.h>

uint16_t thermistor_read_adc(void);
bool     thermistor_is_connected(uint16_t adc_value);
float    thermistor_adc_to_celsius(uint16_t adc_value);

#endif /* THERMISTOR_H */
