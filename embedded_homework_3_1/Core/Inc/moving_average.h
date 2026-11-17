#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <stdint.h>

void     moving_average_reset(void);
uint16_t moving_average_add(uint16_t new_value);

#endif /* MOVING_AVERAGE_H */
