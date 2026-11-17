#include "led.h"
#include "main.h"

static bool s_is_on = false;

void led_on(void)
{
    HAL_GPIO_WritePin(LED_EXT_GPIO_Port, LED_EXT_Pin, GPIO_PIN_SET);
    s_is_on = true;
}

void led_off(void)
{
    HAL_GPIO_WritePin(LED_EXT_GPIO_Port, LED_EXT_Pin, GPIO_PIN_RESET);
    s_is_on = false;
}

void led_toggle(void)
{
    if (s_is_on)
    {
        led_off();
    }
    else
    {
        led_on();
    }
}

bool led_is_on(void)
{
    return s_is_on;
}
