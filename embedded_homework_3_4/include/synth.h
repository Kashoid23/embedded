#pragma once

#include <stdbool.h>

void synth_init(void);
void synth_read_buttons(void);
bool synth_any_pressed(void);
void synth_tick(void);
