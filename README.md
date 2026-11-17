# Embedded Development

Lessons and homework from an embedded development course, collected into one repository.
Each folder is a standalone project that used to be its own repository; its full commit history is preserved here.

## Hardware & tools

| Board | Framework | Tooling |
|---|---|---|
| ESP32-S3 (Freenove WROOM / DevKitC-1) | Arduino, ESP-IDF | PlatformIO, Wokwi |
| STM32F411CEU6 (Black Pill) | STM32 HAL | STM32CubeMX, STM32CubeIDE |

Logic analyzer captures were viewed in PulseView.

## Module 1 — ESP32-S3 basics (Arduino)

| Project | Topic |
|---|---|
| [embedded_lesson_1_3](embedded_lesson_1_3) | Blinking an LED with GPIO output |
| [embedded_homework_1_3](embedded_homework_1_3) | Blinking two LEDs in a pattern |
| [embedded_lesson_1_4](embedded_lesson_1_4) | Reading a button with an internal pull-up |
| [embedded_homework_1_4](embedded_homework_1_4) | Switching LED blink modes with a button |
| [embedded_lesson_1_5](embedded_lesson_1_5) | Button debouncing: polling vs. interrupts, checked on a logic analyzer |
| [embedded_homework_1_5](embedded_homework_1_5) | Counting button presses with an interrupt (ISR) |
| [embedded_lesson_1_6](embedded_lesson_1_6) | Reading an analog sensor (ADC) with averaging and LED hysteresis |
| [embedded_homework_1_6](embedded_homework_1_6) | Measuring LDR voltage with the ADC and estimating the error |
| [embedded_lesson_1_7](embedded_lesson_1_7) | First STM32 project: toggling an LED with a button (HAL) |

## Module 2 — Timing, debouncing and peripherals

| Project | Board | Topic |
|---|---|---|
| [embedded_homework_2_1](embedded_homework_2_1) | ESP32-S3 | Non-blocking LED modes with Button / Led / Timer classes |
| [embedded_homework_2_2](embedded_homework_2_2) | STM32 | Measuring relay switching time with a logic analyzer |
| [embedded_homework_2_3](embedded_homework_2_3) | STM32 | Super-loop with two LEDs and a pause button |
| [embedded_lesson_2_4](embedded_lesson_2_4) | STM32 | Interrupt-driven button debounce plus a heartbeat LED |
| [embedded_homework_2_4](embedded_homework_2_4) | STM32 | Button debounce with a press counter and a heartbeat LED |
| [embedded_lesson_2_5](embedded_lesson_2_5) | ESP32-S3 (ESP-IDF) | Rotary encoder (KY-040) controlling a passive buzzer |
| [embedded_homework_2_5](embedded_homework_2_5) | STM32 | Relay-driven fan or motor controlled by a hardware timer (PWM) with a watchdog |
| [embedded_homework_2_6](embedded_homework_2_6) | ESP32-S3 (ESP-IDF) | Debounced button with interrupts in FreeRTOS, toggling LEDs |

Many projects have their own `README.md` with wiring diagrams, CubeMX settings and results.

## Building

- **PlatformIO projects** (folders with `platformio.ini`): open the folder in VS Code with PlatformIO, or run `pio run -t upload` inside it.
- **STM32 projects** (folders with a `.ioc` file): import the folder into STM32CubeIDE as an existing project, then build and flash.
