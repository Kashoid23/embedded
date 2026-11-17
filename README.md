# Embedded Development

Lessons and homework from an embedded development course, collected into one repository.
Each folder is a standalone project that used to be its own repository; its full commit history is preserved here.

| Project | Board | Components | Framework | Tools | Description |
|---|---|---|---|---|---|
| embedded_homework_2_6 | ESP32-S3 | Button, 2 LEDs | ESP-IDF + FreeRTOS | PlatformIO | GPIO interrupt button debounce with esp_timer, LED toggle |
| embedded_homework_2_5 | STM32F411 Black Pill | Relay module JQC-3F, DC motor | STM32 HAL | STM32CubeMX, STM32CubeIDE | Fan relay on a 15/45 min hardware PWM schedule, open-drain relay drive, watchdog |
| embedded_lesson_2_5 | ESP32-S3 | KY-040 encoder, passive buzzer | ESP-IDF + FreeRTOS | PlatformIO | Rotary encoder sets buzzer pitch, quadrature decode on hardware timer, PWM buzzer melody |
| embedded_homework_2_4 | STM32F411 Black Pill | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | Four debounce modes: none, time-based, state-based EXTI, polling FSM, press counter |
| embedded_lesson_2_4 | STM32F411 Black Pill | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | EXTI button debounce, heartbeat LED |
| embedded_homework_2_3 | STM32F411 Black Pill | Button, 2 LEDs | STM32 HAL | STM32CubeMX, STM32CubeIDE | Super-loop, two LEDs at independent periods, debounced pause button |
| embedded_homework_2_2 | STM32F411 Black Pill | Relay module JQC-3F, logic analyzer | STM32 HAL | STM32CubeMX, STM32CubeIDE, PulseView | Relay switching time measurement, open-drain drive, EXTI on relay contact |
| embedded_homework_2_1 | ESP32-S3 | Button, LED | Arduino | PlatformIO | Non-blocking LED modes, Button / Led / Timer classes, ISR button |
| embedded_lesson_1_7 | STM32F411 Black Pill | Button, LED | STM32 HAL | STM32CubeMX, STM32CubeIDE | Button toggles LED, first HAL project |
| embedded_homework_1_6 | ESP32-S3 | LDR | Arduino | PlatformIO | LDR voltage divider measurement, ADC calibration error |
| embedded_lesson_1_6 | ESP32-S3 | Light sensor, LED | Arduino | PlatformIO | Light sensor switches an LED with hysteresis, ADC averaging, Serial Plotter |
| embedded_homework_1_5 | ESP32-S3 | Button | Arduino | PlatformIO | Button press counter, falling-edge ISR |
| embedded_lesson_1_5 | ESP32-S3 | Button, logic analyzer | Arduino | PlatformIO, PulseView | Button debounce, interrupt vs polling |
| embedded_homework_1_4 | ESP32-S3 | Button, 2 LEDs | Arduino | PlatformIO | Button switches two-LED blink modes, enum mode state |
| embedded_lesson_1_4 | ESP32-S3 | Button | Arduino | PlatformIO | Button read with internal pull-up, Serial output |
| embedded_homework_1_3 | ESP32-S3 | 2 LEDs | Arduino | PlatformIO, Wokwi | Red and blue LED blink pattern, Wokwi bonus circuits |
| embedded_lesson_1_3 | ESP32-S3 | LED | Arduino | PlatformIO | LED blink, GPIO output |

## Building

- **PlatformIO projects** (folders with `platformio.ini`): open in VS Code with PlatformIO, or run `pio run -t upload` inside the folder.
- **STM32 projects** (folders with a `.ioc` file): import into STM32CubeIDE as an existing project, then build and flash.
