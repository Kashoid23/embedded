#include "moving_average.h"
#include "config.h"

static uint16_t s_values[CFG_AVERAGE_WINDOW_SIZE];
static uint32_t s_sum;
static uint16_t s_next_index;
static uint16_t s_filled_count;

void moving_average_reset(void)
{
    for (uint16_t i = 0u; i < CFG_AVERAGE_WINDOW_SIZE; i++)
    {
        s_values[i] = 0u;
    }
    s_sum = 0u;
    s_next_index = 0u;
    s_filled_count = 0u;
}

uint16_t moving_average_add(uint16_t new_value)
{
    uint16_t oldest_value = s_values[s_next_index];

    s_sum = s_sum - oldest_value + new_value;
    s_values[s_next_index] = new_value;

    s_next_index++;
    if (s_next_index == CFG_AVERAGE_WINDOW_SIZE)
    {
        s_next_index = 0u;
    }

    if (s_filled_count < CFG_AVERAGE_WINDOW_SIZE)
    {
        s_filled_count++;
    }

    return (uint16_t)(s_sum / s_filled_count);
}
