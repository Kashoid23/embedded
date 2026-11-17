# Embedded Development

Lessons and homework from an embedded development course, collected into one repository.
Each folder is a standalone project that used to be its own repository; its full commit history is preserved here.

| Project | Description |
|---|---|
| embedded_lesson_1_3 | LED blink, GPIO output (ESP32-S3, Arduino, PlatformIO) |
| embedded_homework_1_3 | Red and blue LED blink pattern, Wokwi bonus circuits (ESP32-S3, Arduino, PlatformIO) |
| embedded_lesson_1_4 | Button read with internal pull-up, Serial output (ESP32-S3, Arduino, PlatformIO) |
| embedded_homework_1_4 | Button switches two-LED blink modes, enum mode state (ESP32-S3, Arduino, PlatformIO) |
| embedded_lesson_1_5 | Button debounce, interrupt vs polling, verified on logic analyzer (ESP32-S3, Arduino, PlatformIO) |
| embedded_homework_1_5 | Button press counter, falling-edge ISR (ESP32-S3, Arduino, PlatformIO) |
| embedded_lesson_1_6 | Light sensor switches an LED with hysteresis, ADC averaging, Serial Plotter (ESP32-S3, Arduino, PlatformIO) |
| embedded_homework_1_6 | LDR voltage divider measurement, ADC calibration error (ESP32-S3, Arduino, PlatformIO) |
| embedded_lesson_1_7 | Button toggles LED, first HAL project (STM32F411, HAL, CubeMX) |
| embedded_homework_2_1 | Non-blocking LED modes, Button / Led / Timer classes, ISR button (ESP32-S3, Arduino, PlatformIO) |
| embedded_homework_2_2 | Relay switching time measurement, open-drain drive, EXTI on relay contact, logic analyzer (STM32F411, HAL, CubeMX) |
| embedded_homework_2_3 | Super-loop, two LEDs at independent periods, debounced pause button (STM32F411, HAL, CubeMX) |
| embedded_lesson_2_4 | EXTI button debounce, heartbeat LED (STM32F411, HAL, CubeMX) |
| embedded_homework_2_4 | Four debounce modes: none, time-based, state-based EXTI, polling FSM, press counter (STM32F411, HAL, CubeMX) |
| embedded_lesson_2_5 | Rotary encoder sets buzzer pitch, quadrature decode on hardware timer, PWM buzzer melody (ESP32-S3, ESP-IDF, FreeRTOS) |
| embedded_homework_2_5 | Fan relay on a 15/45 min hardware PWM schedule, open-drain relay drive, watchdog (STM32F411, HAL, CubeMX) |
| embedded_homework_2_6 | GPIO interrupt button debounce with esp_timer, LED toggle (ESP32-S3, ESP-IDF, FreeRTOS) |

## Building

- **PlatformIO projects** (folders with `platformio.ini`): open in VS Code with PlatformIO, or run `pio run -t upload` inside the folder.
- **STM32 projects** (folders with a `.ioc` file): import into STM32CubeIDE as an existing project, then build and flash.
