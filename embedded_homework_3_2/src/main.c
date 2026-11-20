#include <stdint.h>
#include <stdio.h>

#include "adc_reader.h"
#include "config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// U_manual = RAW * full scale / max code
static float rawToMillivolts(int raw)
{
	return (float)raw * ADC_FULL_SCALE_MV / ADC_RAW_MAX;
}

// Error(%) = (U_manual - U_cali) / U_cali * 100
static float errorPercent(float manualMv, int calibratedMv)
{
	return (manualMv - (float)calibratedMv) / (float)calibratedMv * 100.0f;
}

static void printSettings(void)
{
	printf("\nHW 3.2 - ADC calibration, ESP32-S3\n");
	printf("Parameter        Value\n");
	printf("-----------------------------------\n");
	printf("ADC / channel    ADC1 / CH3 (GPIO4)\n");
	printf("Attenuation      12 dB\n");
	printf("Resolution       12 bit (0..%d)\n", ADC_RAW_MAX);
	printf("Full scale       %.0f mV\n", ADC_FULL_SCALE_MV);
	printf("1 LSB            %.3f mV\n", ADC_FULL_SCALE_MV / ADC_RAW_MAX);
	printf("Calibration      %s\n", adcReaderIsCalibrated() ? "curve fitting (eFuse)" : "not available");
	printf("Period           %u ms\n", (unsigned)SAMPLE_PERIOD_MS);
}

static void printHeader(void)
{
	printf("\n RAW   U_manual(mV)   U_cali(mV)   Error(%%)\n");
	printf("------------------------------------------\n");
}

static void printRow(int raw, float manualMv, int calibratedMv)
{
	if (calibratedMv > 0) {
		printf("%4d %14.1f %12d %10.2f\n",
		       raw, manualMv, calibratedMv, errorPercent(manualMv, calibratedMv));
	} else {
		// no calibration (-1) or 0 mV: error cannot be divided by zero
		printf("%4d %14.1f %12d %10s\n", raw, manualMv, calibratedMv, "n/a");
	}
}

void app_main(void)
{
	adcReaderInit();
	printSettings();

	uint32_t rowCount = 0;
	TickType_t lastWakeTime = xTaskGetTickCount();

	while (1) {
		if (rowCount % ROWS_PER_HEADER == 0) {
			printHeader();
		}

		// One RAW for both columns: the difference is only the conversion method
		const int raw = adcReaderReadRaw();
		const float manualMv = rawToMillivolts(raw);
		const int calibratedMv = adcReaderCalibratedMv(raw);

		printRow(raw, manualMv, calibratedMv);
		rowCount++;

		// Wake up exactly every SAMPLE_PERIOD_MS, counted from the previous wake-up
		vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SAMPLE_PERIOD_MS));
	}
}
