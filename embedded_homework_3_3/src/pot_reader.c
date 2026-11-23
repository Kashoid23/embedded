#include "pot_reader.h"
#include "config.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"

#include <stddef.h>

static adc_oneshot_unit_handle_t adcUnit = NULL;

void potReaderInit(void)
{
	const adc_oneshot_unit_init_cfg_t unitConfig = {
		.unit_id = POT_ADC_UNIT,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
	};
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&unitConfig, &adcUnit));

	const adc_oneshot_chan_cfg_t channelConfig = {
		.atten = POT_ADC_ATTEN,
		.bitwidth = POT_ADC_BITWIDTH,
	};
	ESP_ERROR_CHECK(adc_oneshot_config_channel(adcUnit, POT_ADC_CHANNEL, &channelConfig));
}

uint32_t potReaderReadRaw(void)
{
	uint32_t sum = 0;

	for (uint32_t i = 0; i < POT_SAMPLES; i++) {
		int raw = 0;
		ESP_ERROR_CHECK(adc_oneshot_read(adcUnit, POT_ADC_CHANNEL, &raw));
		sum += (uint32_t)raw;
	}

	return sum / POT_SAMPLES;
}
