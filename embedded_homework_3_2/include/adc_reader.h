#pragma once

#include <stdbool.h>

// Creates ADC1 unit, configures the potentiometer channel
// and tries to create the calibration scheme (curve fitting, eFuse data).
void adcReaderInit(void);

// One conversion: RAW code 0..4095.
int adcReaderReadRaw(void);

// true if the chip has calibration data and the scheme was created.
bool adcReaderIsCalibrated(void);

// Calibrated voltage in mV for a RAW code, or -1 if calibration is unavailable.
int adcReaderCalibratedMv(int raw);
