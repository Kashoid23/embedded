#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_timer.h"

#include "buzzer.h"
#include "config.h"
#include "player.h"
#include "synth.h"

static esp_timer_handle_t s_tick_timer;
static int s_tick_count = 0;
static int s_led_on = 0;

static void tick_cb(void *arg)
{
    (void)arg;

    synth_read_buttons();

    if (player_is_playing() && synth_any_pressed()) {
        player_stop();
    }

    if (player_is_playing()) {
        player_tick();
    } else {
        synth_tick();
    }

    s_tick_count++;
    if (s_tick_count % LED_BLINK_TICKS == 0) {
        s_led_on = !s_led_on;
        gpio_set_level(LED_GPIO, s_led_on);
    }
}

void app_main(void)
{
    buzzer_init();
    synth_init();

    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);

    player_start();

    esp_timer_create_args_t args = {
        .callback = tick_cb,
        .name     = "tick",
    };
    ESP_ERROR_CHECK(esp_timer_create(&args, &s_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(s_tick_timer, TICK_MS * 1000));
}
