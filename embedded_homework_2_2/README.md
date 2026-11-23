## Обладнання

- **STM32F411CEU6**
- модуль реле **JQC-3F-05VDC-C**
- окреме джерело **5 В DC** для модуля реле
- логічний аналізатор

## Середовище

- **STM32CubeMX**
- **STM32CubeIDE**
- **PulseView**

## Схема підключення

<img width="2720" height="1901" alt="stm32f411_relay_timing_measurement_wiring" src="https://github.com/user-attachments/assets/9e9cec6f-b3f6-4ce9-ad24-a82d8c1df898" />

## Налаштування CubeMX

| Розділ | Налаштування |
|---|---|
| **SYS** | Debug: Serial Wire |
| **RCC** | HSE: Crystal/Ceramic Resonator |
| **Clock** | HSE 25 МГц → PLL (M = 12, N = 96, P = 2) → **100 МГц** |
| **PA1** | GPIO_Output, **Open Drain**, Output level **High**, No pull, User Label **RELAY** |
| **PA4** | GPIO_EXTI4, **Rising/Falling edge**, **Pull-up**, User Label **RELAY_CONTACT** |
| **NVIC** | EXTI line4 interrupt ✅ |

## Структура коду

```
Core/Inc/relay_test.h
Core/Src/relay_test.c
Core/Src/main.c
Core/Src/stm32f4xx_it.c (EXTI4_IRQHandler)
```

## Як працює вимірювання

Кожне вмикання і вимикання реле вимірюється так:

1. Програма запам'ятовує час і подає команду на PA1:
   - `LOW` – увімкнути реле;
   - `HIGH` – вимкнути реле.
2. Контакт реле перемикається, і на PA4 спрацьовує переривання. Воно запам'ятовує час першої зміни контакту.
3. Контакт кілька разів «брязкає». Кожен відскок знову викликає переривання і оновлює час останньої зміни.
4. Якщо контакт 20 мс не змінюється, брязкіт закінчився.
5. Програма перевіряє, що контакт у правильному стані:
   - після вмикання – замкнений (`LOW`);
   - після вимикання – розімкнений (`HIGH`).
6. Записуються результати:
   - **час спрацювання** – від команди до першої зміни контакту;
   - **брязкіт** – від першої до останньої зміни контакту.

Якщо реле не відреагувало за 500 мс, результат дорівнює `0` і в середнє значення не входить.

Між перемиканнями є пауза 300 мс.

## Запуск і результати

1. Зібрати проєкт у STM32CubeIDE і запустити **Debug**, потім **Resume**.
2. У **Live Expressions** додати змінну `results`.

<img width="670" height="591" alt="Знімок екрана 2026-10-29 142437" src="https://github.com/user-attachments/assets/f65b71f8-48b0-4992-b8fa-fbef42e89f1f" />

| Поле | Зміст |
|---|---|
| `switchOnMs[10]` | час вмикання, мс |
| `switchOffMs[10]` | час вимикання, мс |
| `switchOnBounceMs[10]`, `switchOffBounceMs[10]` | брязкіт, мс |
| `averageOnMs`, `averageOffMs` | середній час, мс |
| `done` | тест завершено |

Час рахується в мілісекундах. Брязкіт зазвичай коротший за 1 мс, тому в результатах він дорівнює `0`. Побачити його можна на логічному аналізаторі.

<img width="1919" height="631" alt="Знімок екрана 2026-10-29 144003" src="https://github.com/user-attachments/assets/670b0a86-e9bc-4813-9772-0abda7195280" />
<img width="1919" height="586" alt="Знімок екрана 2026-10-29 144053" src="https://github.com/user-attachments/assets/cb6e45e0-e9e0-4885-8027-3ccced7d3618" />

## Відповідність завданню

| Вимога (ESP32 / Arduino) | Реалізація (STM32 / HAL) |
|---|---|
| GPIO керує реле через модуль | PA1 → IN модуля |
| GPIO читає контакт, `INPUT_PULLUP` | PA4, Pull-up |
| `millis()` | `HAL_GetTick()` |
| `attachInterrupt` | EXTI4, обидва фронти |
| Врахувати брязкіт | 20 мс без змін = кінець брязкоту |
| ≥ 10 вимірювань, середнє | 10 циклів, `averageOnMs`, `averageOffMs` |
| Serial Monitor | Live Expressions |
