#include "synth.h"
#include "buzzer.h"
#include "config.h"
#include "notes.h"

#include <stdint.h>
#include "driver/gpio.h"

static const gpio_num_t BTN_PIN[2] = { BTN_A_GPIO, BTN_B_GPIO };

static const uint16_t COMBO_NOTE[4] = { 0, NOTE_C4, NOTE_D4, NOTE_E4 };

static int s_raw_prev[2];
static int s_pressed[2];
static uint16_t s_playing_hz;

void synth_init(void)
{
    for (int i = 0; i < 2; i++) {
        gpio_reset_pin(BTN_PIN[i]);
        gpio_set_direction(BTN_PIN[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(BTN_PIN[i], GPIO_PULLUP_ONLY);
    }
}

void synth_read_buttons(void)
{
    for (int i = 0; i < 2; i++) {
        int raw = (gpio_get_level(BTN_PIN[i]) == 0);
        if (raw == s_raw_prev[i]) {
            s_pressed[i] = raw;
        }
        s_raw_prev[i] = raw;
    }
}

bool synth_any_pressed(void)
{
    return s_pressed[0] || s_pressed[1];
}

void synth_tick(void)
{
    int combo = s_pressed[0] + 2 * s_pressed[1];
    uint16_t want_hz = COMBO_NOTE[combo];

    if (want_hz == s_playing_hz) {
        return;
    }

    if (want_hz == 0) {
        buzzer_off();
    } else {
        buzzer_on(want_hz);
    }
    s_playing_hz = want_hz;
}
