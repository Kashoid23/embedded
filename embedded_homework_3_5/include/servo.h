#pragma once

#include <stdint.h>

// LEDC timer 50 Hz, 14 bit + channel on SERVO_GPIO_NUM; starts at SERVO_CENTER_US.
void servoInit(void);

// Pulse width in microseconds, clamped to SERVO_MIN_US..SERVO_MAX_US.
// Returns the LEDC duty that was written.
uint32_t servoSetPulseUs(uint32_t pulseUs);
