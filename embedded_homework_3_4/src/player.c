#include "player.h"
#include "buzzer.h"
#include "notes.h"

#include <stdint.h>

/* Темп: чверть = 6 тіків (300 мс), половина = 12, ціла = 24. Мінімум 2 тіки. */

/* Jingle Bells: E E E | E E E | E G C D E */
static const uint16_t MELODY_FREQ[] = {
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
};
static const uint8_t MELODY_TICKS[] = {
    6, 6, 12,
    6, 6, 12,
    6, 6, 9, 3, 24,
};

#define MELODY_LEN  ((int)(sizeof(MELODY_FREQ) / sizeof(MELODY_FREQ[0])))

static int s_note = -1;
static int s_tick = 0;

static void start_note(int note)
{
    s_note = note;
    s_tick = 0;
    buzzer_on(MELODY_FREQ[note]);
}

void player_start(void)
{
    start_note(0);
}

void player_stop(void)
{
    s_note = -1;
    buzzer_off();
}

bool player_is_playing(void)
{
    return s_note >= 0;
}

void player_tick(void)
{
    if (s_note < 0) {
        return;
    }

    s_tick++;

    if (s_tick == MELODY_TICKS[s_note] - 1) {
        buzzer_off();
    }

    if (s_tick >= MELODY_TICKS[s_note]) {
        if (s_note + 1 < MELODY_LEN) {
            start_note(s_note + 1);
        } else {
            s_note = -1;
        }
    }
}
