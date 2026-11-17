# Embedded Development

| Project | Board | Components | Framework | Tools | Description |
|---|---|---|---|---|---|
| H 2.6 | ESP32-S3 N16R8 | Button, 2 LEDs | ESP-IDF + FreeRTOS | PlatformIO | GPIO interrupt, debounce, esp_timer |
| H 2.5 | STM32F411CEU6 | Relay module JQC-3F, DC motor | STM32 HAL | STM32CubeMX, STM32CubeIDE | Hardware timer PWM, watchdog (IWDG), open-drain relay drive |
| L 2.5 | ESP32-S3 N16R8 | KY-040 encoder, passive buzzer | ESP-IDF + FreeRTOS | PlatformIO | Quadrature encoder, hardware timer, PWM (LEDC) |
| H 2.4 | STM32F411CEU6 | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | Debounce strategies: time-based, state-based, polling FSM |
| L 2.4 | STM32F411CEU6 | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | EXTI debounce, non-blocking timing |
| H 2.3 | STM32F411CEU6 | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | Superloop, non-blocking timing, debounce |
| H 2.2 | STM32F411CEU6 | Relay module JQC-3F, logic analyzer | STM32 HAL | STM32CubeMX, STM32CubeIDE, PulseView | Relay switching time measurement, EXTI, open-drain |
| H 2.1 | ESP32-S3 N16R8 | Button, LED | Arduino | PlatformIO | Non-blocking timing, driver classes, ISR |
| L 1.7 | STM32F411CEU6 | Button, LED | STM32 HAL | STM32CubeMX, STM32CubeIDE | GPIO, HAL basics |
| H 1.6 | ESP32-S3 N16R8 | LDR | Arduino | PlatformIO | ADC, voltage divider, measurement error |
| L 1.6 | ESP32-S3 N16R8 | Light sensor, LED | Arduino | PlatformIO | ADC, averaging, hysteresis |
| H 1.5 | ESP32-S3 N16R8 | Button | Arduino | PlatformIO | Interrupts (ISR), volatile |
| L 1.5 | ESP32-S3 N16R8 | Button, logic analyzer | Arduino | PlatformIO, PulseView | Debounce, interrupt vs polling |
| H 1.4 | ESP32-S3 N16R8 | Button, 2 LEDs | Arduino | PlatformIO | Button polling, mode FSM |
| L 1.4 | ESP32-S3 N16R8 | Button | Arduino | PlatformIO | GPIO input, pull-up |
| H 1.3 | ESP32-S3 N16R8 | 2 LEDs | Arduino | PlatformIO | GPIO output, blink patterns |
| L 1.3 | ESP32-S3 N16R8 | LED | Arduino | PlatformIO | GPIO output |
