
# Embedded Development

| Project | Board | Components | Framework | Tools | Description | Demo |
|---|---|---|---|---|---|---|
| HW 3.3 | ESP32-S3 N16R8 | Potentiometer, LED, DC motor, 2N2222A, 1N4007 | ESP-IDF + FreeRTOS | PlatformIO | ADC oneshot + averaging, LEDC PWM on two independent timers/channels, dead zone + motor start duty, transistor motor drive | |
| L 3.3 | STM32F411CEU6 | Potentiometer, DC motor, 2N2222A, 1N4007, LED | STM32 HAL | STM32CubeMX, STM32CubeIDE | ADC continuous + EOC interrupt, TIM1 complementary PWM (CH1/CH1N), transistor motor drive | [Video](https://drive.google.com/file/d/1jGUrRYAqdqHug8NbKJ6crWiLG430yuwL/view?usp=sharing) |
| HW 3.2 | ESP32-S3 N16R8 | Potentiometer | ESP-IDF + FreeRTOS | PlatformIO | ADC oneshot, eFuse calibration (curve fitting), formula vs calibrated voltage, error table | [Image](https://drive.google.com/file/d/1CVvHVtBypNyGOdBuzx9xFkgLijWejXGU/view?usp=sharing) |
| L 3.2 | STM32F411CEU6 | Potentiometer, LDR | STM32 HAL | STM32CubeMX, STM32CubeIDE | ADC scan + DMA circular, TIM2 TRGO trigger, double buffering, overrun polling | [Image](https://drive.google.com/file/d/16tFEBL3yaKEbQNvQkKMLtgbmX1cwukwt/view?usp=sharing) |
| HW 3.1 | STM32F411CEU6 | NTC thermistor 10k, LED | STM32 HAL | STM32CubeMX, STM32CubeIDE | ADC oneshot, SMA ring buffer, Beta equation, hysteresis | [Image](https://drive.google.com/file/d/16tFEBL3yaKEbQNvQkKMLtgbmX1cwukwt/view?usp=sharing) |
| HW 2.6 | ESP32-S3 N16R8 | Button, 2 LEDs | ESP-IDF + FreeRTOS | PlatformIO | GPIO interrupt, debounce, esp_timer | - |
| HW 2.5 | STM32F411CEU6 | Relay module JQC-3F, DC motor | STM32 HAL | STM32CubeMX, STM32CubeIDE | Hardware timer PWM, watchdog (IWDG), open-drain relay drive | [Video](https://drive.google.com/file/d/1bVQc7zcX7CAADzsK44qWClTHseDBUTlR/view?usp=sharing) |
| L 2.5 | ESP32-S3 N16R8 | KY-040 encoder, passive buzzer | ESP-IDF + FreeRTOS | PlatformIO | Quadrature encoder, hardware timer, PWM (LEDC) | [Video](https://drive.google.com/file/d/1ptMVeSaPct5mZ0770ErXQSRtwLNW-cbK/view?usp=sharing) |
| HW 2.4 | STM32F411CEU6 | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | Debounce strategies: time-based, state-based, polling FSM | [Image](https://drive.google.com/file/d/1UhdtwTDa1bzkMOFi1JDQ5X1AK0cVzC5J/view?usp=sharing) |
| L 2.4 | STM32F411CEU6 | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | EXTI debounce, non-blocking timing | - |
| HW 2.3 | STM32F411CEU6 | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | Superloop, non-blocking timing, debounce | - |
| HW 2.2 | STM32F411CEU6 | Relay module JQC-3F, logic analyzer | STM32 HAL | STM32CubeMX, STM32CubeIDE, PulseView | Relay switching time measurement, EXTI, open-drain | [Video](https://drive.google.com/file/d/1JIcO3V_hDTBrTLFzdYlbB36XDn7QN_Bk/view?usp=sharing) |
| HW 2.1 | ESP32-S3 N16R8 | Button, LED | Arduino | PlatformIO | Non-blocking timing, driver classes, ISR | - |
| L 1.7 | STM32F411CEU6 | Button, LED | STM32 HAL | STM32CubeMX, STM32CubeIDE | GPIO, HAL basics | - |
| HW 1.6 | ESP32-S3 N16R8 | LDR | Arduino | PlatformIO | ADC, voltage divider, measurement error | [Wokwi](https://wokwi.com/projects/478118996856095745) |
| L 1.6 | ESP32-S3 N16R8 | LDR, LED | Arduino | PlatformIO | ADC, averaging, hysteresis | [Video](https://drive.google.com/file/d/1gxRNAMV3uwsGTd6CSP4lNnThisYcWrQT/view?usp=sharing) |
| HW 1.5 | ESP32-S3 N16R8 | Button | Arduino | PlatformIO | Interrupts (ISR), volatile | - |
| L 1.5 | ESP32-S3 N16R8 | Button, logic analyzer | Arduino | PlatformIO, PulseView | Debounce, interrupt vs polling | [Image](https://drive.google.com/file/d/13Gm6B7kNh5ganqlp2WylpV0dobQ9wBDe/view?usp=sharing) |
| HW 1.4 | ESP32-S3 N16R8 | Button, 2 LEDs | Arduino | PlatformIO | Button polling, mode FSM | [Video](https://drive.google.com/file/d/1XsS6z0-O4zRYEV8U6v-IdS0XfYwgkg9b/view?usp=sharing) |
| L 1.4 | ESP32-S3 N16R8 | Button | Arduino | PlatformIO | GPIO input, pull-up | [Wokwi](https://wokwi.com/projects/477697142352863233) |
| HW 1.3 | ESP32-S3 N16R8 | 2 LEDs | Arduino | PlatformIO | GPIO output, blink patterns | [Video](https://drive.google.com/file/d/1cy69lFgOryp7S8-vPTs_1xius7wdKys4/view?usp=sharing) |
| L 1.3 | ESP32-S3 N16R8 | LED | Arduino | PlatformIO | GPIO output | [Video](https://drive.google.com/file/d/1o0ntKAXLnozGxvl3AXdaBfcLYxaJPf4C/view?usp=sharing) |
