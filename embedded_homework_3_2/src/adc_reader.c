#include "adc_reader.h"
#include "config.h"

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "adc_reader";

static adc_oneshot_unit_handle_t adcUnit = NULL;  // "ticket" for ADC1
static adc_cali_handle_t calibration = NULL;      // "ticket" for calibration
static bool calibrated = false;

void adcReaderInit(void)
{
	// 1. Take ADC1 for our program
	const adc_oneshot_unit_init_cfg_t unitConfig = {
		.unit_id = POT_ADC_UNIT,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
	};
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&unitConfig, &adcUnit));

	// 2. Configure the channel of GPIO4
	const adc_oneshot_chan_cfg_t channelConfig = {
		.atten = POT_ADC_ATTEN,
		.bitwidth = POT_ADC_BITWIDTH,
	};
	ESP_ERROR_CHECK(adc_oneshot_config_channel(adcUnit, POT_ADC_CHANNEL, &channelConfig));

	// 3. Calibration: same unit, channel, attenuation and bitwidth as above
	const adc_cali_curve_fitting_config_t calibrationConfig = {
		.unit_id = POT_ADC_UNIT,
		.chan = POT_ADC_CHANNEL,
		.atten = POT_ADC_ATTEN,
		.bitwidth = POT_ADC_BITWIDTH,
	};
	const esp_err_t result = adc_cali_create_scheme_curve_fitting(&calibrationConfig, &calibration);

	if (result == ESP_OK) {
		calibrated = true;
		ESP_LOGI(TAG, "Calibration: curve fitting (eFuse data)");
	} else {
		ESP_LOGW(TAG, "Calibration unavailable: %s", esp_err_to_name(result));
	}
}

int adcReaderReadRaw(void)
{
	int raw = 0;
	ESP_ERROR_CHECK(adc_oneshot_read(adcUnit, POT_ADC_CHANNEL, &raw));
	return raw;
}

bool adcReaderIsCalibrated(void)
{
	return calibrated;
}

int adcReaderCalibratedMv(int raw)
{
	if (!calibrated) {
		return -1;
	}

	int millivolts = 0;
	ESP_ERROR_CHECK(adc_cali_raw_to_voltage(calibration, raw, &millivolts));
	return millivolts;
}
