#ifndef CONFIG_H
#define CONFIG_H

#define CFG_MEASURE_PERIOD_MS          100u

#define CFG_ADC_MAX_VALUE              4095.0f
#define CFG_ADC_TIMEOUT_MS             10u

#define CFG_SENSOR_BROKEN_BELOW        50u
#define CFG_SENSOR_SHORTED_ABOVE       4045u

#define CFG_FIXED_RESISTOR_OHM         10000.0f
#define CFG_NTC_RESISTANCE_AT_25C_OHM  10000.0f
#define CFG_NTC_BETA                   3950.0f

#define CFG_KELVIN_AT_25C              298.15f
#define CFG_KELVIN_AT_0C               273.15f
#define CFG_CALIBRATION_OFFSET_C       0.0f

#define CFG_AVERAGE_WINDOW_SIZE        16u

#define CFG_LED_ON_ABOVE_C             28.0f
#define CFG_LED_OFF_BELOW_C            26.0f

#endif /* CONFIG_H */
