# Етапи GCC на прикладі `HAL_GPIO_WritePin` (PB5, STM32F411)

Тулчейн (toolchain): `arm-none-eabi-gcc 13.2`, прапорці `-mcpu=cortex-m4 -mthumb -O1`.

`arm-none-eabi-` — крос-тулчейн: `arm` — процесор ARM, `none` — без ОС, `eabi` — правила Embedded ABI. Звичайний `gcc` генерує код для процесора комп'ютера (x86-64), а не для STM32.

```
main.c     ──-E──▶ main.i ──-S──▶ main.s ──-c──▶ main.o     ─┐
hal_gpio.c ────────────────────────────────────▶ hal_gpio.o ─┴──ld──▶ firmware.elf
```

## 1. Файли

`hal_mini.h` — скорочена копія `stm32f4xx.h` + `stm32f4xx_hal_gpio.h`:

```c
#include <stdint.h>             /* типи uint16_t, uint32_t */
#define __IO volatile           /* volatile: щоразу реально читати/писати пам'ять */

typedef struct                  /* опис розташування регістрів GPIO в пам'яті */
{
  __IO uint32_t MODER;    /* 0x00 */
  __IO uint32_t OTYPER;   /* 0x04 */
  __IO uint32_t OSPEEDR;  /* 0x08 */
  __IO uint32_t PUPDR;    /* 0x0C */
  __IO uint32_t IDR;      /* 0x10 */
  __IO uint32_t ODR;      /* 0x14 */
  __IO uint32_t BSRR;     /* 0x18 */
  __IO uint32_t LCKR;     /* 0x1C */
  __IO uint32_t AFR[2];   /* 0x20, 0x24 */
} GPIO_TypeDef;

#define PERIPH_BASE      0x40000000UL
#define AHB1PERIPH_BASE  (PERIPH_BASE + 0x00020000UL)
#define GPIOB_BASE       (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOB            ((GPIO_TypeDef *) GPIOB_BASE)   /* «за адресою GPIOB_BASE лежить GPIO_TypeDef» */

#define GPIO_PIN_5       ((uint16_t)0x0020)              /* маска біта 5 */

typedef enum                    /* enum: іменовані цілі числа */
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET                  /* = 1 */
} GPIO_PinState;

/* Оголошення (declaration): «така функція існує», без коду */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
```

`hal_gpio.c` — скорочена копія `stm32f4xx_hal_gpio.c`. Тут **визначення (definition)**, тобто сам код функції:

```c
#include "hal_mini.h"

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  if (PinState != GPIO_PIN_RESET)
    GPIOx->BSRR = GPIO_Pin;                   /* set */
  else
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;  /* reset */
}
```

`main.c`:

```c
#include "hal_mini.h"

int main(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);  /* PB5 = 1 */
    while (1) {
    }
}
```

`mini.ld` — спрощений linker script: каже лінкеру, де у мікроконтролера Flash і RAM і що куди класти.

```ld
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K   /* rx  = читання, виконання */
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K   /* rwx = + запис */
}
SECTIONS
{
  .text : { *(.text*) } > FLASH   /* весь код (.text) з усіх .o → у FLASH */
}
```

## Команди і прапорці

| Прапорець | Значення |
|---|---|
| `-mcpu=cortex-m4` | Генерувати код для ядра Cortex-M4 (STM32F411) |
| `-mthumb` | Інструкції Thumb (по 2 або 4 байти). Cortex-M інших не підтримує |
| `-O1` | Рівень оптимізації 1. З `-O0` асемблер довший і важче читається |
| `-E` | Зупинитись після препроцесора → `.i` |
| `-S` | Зупинитись після компілятора → `.s` |
| `-c` | Зупинитись після асемблера → `.o` (без лінкування) |
| без `-E/-S/-c` | Пройти всі етапи, включно з лінкером |
| `-o файл` | Ім'я вихідного файлу |
| `-T mini.ld` | Linker script |
| `-nostdlib` | Не додавати стандартну бібліотеку і startup-код (для простоти прикладу) |
| `-e main` | Точка входу (entry point) — `main` |

| Утиліта | Що показує |
|---|---|
| `arm-none-eabi-objdump -d` | Дизасемблювання: байти → інструкції асемблера |
| `arm-none-eabi-objdump -r` | Записи relocation |
| `arm-none-eabi-nm` | Таблицю символів (`-n` — відсортувати за адресою) |
| `arm-none-eabi-size` | Розміри секцій |

## 2. Препроцесор: `-E` → `main.i`

Обробляє рядки з `#`: вставляє `#include`, підставляє `#define`, видаляє коментарі.

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -E main.c -o main.i
```

```c
int main(void)
{
    HAL_GPIO_WritePin(((GPIO_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x0400UL)), ((uint16_t)0x0020), GPIO_PIN_SET);
    while (1) {
    }
}
```

- Вище в `main.i` буквально вставлений увесь текст `hal_mini.h` (і `stdint.h`) на місці `#include`.
- `GPIOB` і `GPIO_PIN_5` — макроси, тому вони замінені текстом.
- `GPIO_PIN_SET` — не макрос, а значення `enum`. Препроцесор його не чіпає, його обробить компілятор.
- `HAL_GPIO_WritePin` — назва функції, вона лишається як є.
- Препроцесор не рахує: суму `0x40000000 + 0x20000 + 0x400` він залишає текстом.

## 3. Компілятор: `-S` → `main.s`

Перекладає C на асемблер Cortex-M4. `r0`, `r1`, ... — регістри процесора: 32-бітні комірки всередині CPU. Не плутати з регістрами периферії (`BSRR`, `ODR`), які лежать за адресами в пам'яті.

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -S main.c -o main.s
```

```asm
main:
    push  {r3, lr}
    movs  r2, #1              @ 3-й аргумент: GPIO_PIN_SET = 1
    movs  r1, #32             @ 2-й аргумент: GPIO_PIN_5 = 0x20
    ldr   r0, .L4             @ 1-й аргумент: GPIOB = 0x40020400
    bl    HAL_GPIO_WritePin   @ виклик за назвою, адреса невідома
.L2:
    b     .L2                 @ while (1)
.L4:
    .word 1073873920          @ = 0x40020400
```

| Інструкція | Що робить |
|---|---|
| `push {r3, lr}` | Зберегти `lr` у стек: `bl` його перезапише. `r3` зберігається лише для вирівнювання стеку на 8 байтів |
| `movs rX, #n` | Записати невелике число в регістр |
| `ldr r0, .L4` | Завантажити 32-бітне число з пам'яті за міткою `.L4`. Велику константу не можна вписати в одну інструкцію, тому вона лежить поруч з кодом (literal pool) |
| `bl функція` | Branch with Link: записати адресу повернення в `lr` і перейти до функції |
| `b мітка` | Безумовний перехід. `b .L2` на самого себе = нескінченний цикл |
| `.L2:`, `.L4:` | Мітки: імена адрес усередині файлу |
| `.word n` | Не інструкція, а 4 байти даних |
| `@ ...` | Коментар в асемблері ARM |

| Аргумент C | Регістр | Значення |
|---|---|---|
| `GPIOB` | `r0` | `0x40020400` |
| `GPIO_PIN_5` | `r1` | `0x20` |
| `GPIO_PIN_SET` | `r2` | `1` |

На Cortex-M перші 4 аргументи функції передаються в регістрах `r0`–`r3` (правило AAPCS).

Компілятор не знає, де лежить `HAL_GPIO_WritePin`. Він вірить оголошенню з `hal_mini.h` і пише виклик за назвою.

## 4. Асемблер: `-c` → `main.o`

Перетворює текст асемблера на машинні байти. Результат — object-файл: код одного `.c`, ще без реальних адрес.

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -c main.c -o main.o
arm-none-eabi-objdump -d -r main.o
```

```
00000000 <main>:
   0:  b508        push   {r3, lr}
   2:  2201        movs   r2, #1
   4:  2120        movs   r1, #32
   6:  4802        ldr    r0, [pc, #8]
   8:  f7ff fffe   bl     0 <HAL_GPIO_WritePin>
               8: R_ARM_THM_CALL  HAL_GPIO_WritePin
   c:  e7fe        b.n    c <main+0xc>
   e:  bf00        nop
  10:  40020400    .word  0x40020400
```

Як читати рядок `   2:  2201        movs   r2, #1`:

| `2:` | `2201` | `movs r2, #1` |
|---|---|---|
| адреса (зміщення від початку `main`) | машинний код у hex | та сама інструкція текстом |

- Адреси починаються з `0`: асемблер не знає, де у Flash буде `main`.
- `ldr r0, [pc, #8]` — те саме, що `ldr r0, .L4`: константа лежить на 8 байтів далі від поточної позиції.
- `nop` (no operation) — порожня інструкція для вирівнювання константи на 4 байти.
- `f7ff fffe` — заглушка на місці адреси.
- `R_ARM_THM_CALL HAL_GPIO_WritePin` — relocation, записка лінкеру: «встав сюди адресу».

Символ (symbol) — назва функції або глобальної змінної, яку бачить лінкер:

```bash
arm-none-eabi-nm main.o
         U HAL_GPIO_WritePin    # U = undefined: потрібна, але не визначена тут
00000000 T main                 # T = text: визначена тут, у коді

arm-none-eabi-nm hal_gpio.o
00000000 T HAL_GPIO_WritePin
```

## 5. Лінкер → `firmware.elf`

З'єднує всі `.o` і призначає функціям реальні адреси за `mini.ld`. ELF — формат `.o` і `.elf`: машинний код + таблиця символів + службова інформація. У Flash записуються лише байти коду і даних.

Без `hal_gpio.o`:

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -nostdlib -T mini.ld -e main main.o -o bad.elf
```

```
undefined reference to `HAL_GPIO_WritePin'
```

`main.c` скомпілювався, але лінкер не знайшов жодного `.o`, де `HAL_GPIO_WritePin` позначена як `T`.

З `hal_gpio.o`:

```bash
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -nostdlib -T mini.ld -e main main.o hal_gpio.o -o firmware.elf
arm-none-eabi-objdump -d firmware.elf
```

```
08000000 <main>:
 8000000:  b508        push   {r3, lr}
 8000002:  2201        movs   r2, #1
 8000004:  2120        movs   r1, #32
 8000006:  4802        ldr    r0, [pc, #8]
 8000008:  f000 f804   bl     8000014 <HAL_GPIO_WritePin>   # заглушку заповнено
 800000c:  e7fe        b.n    800000c <main+0xc>
 800000e:  bf00        nop
 8000010:  40020400    .word  0x40020400

08000014 <HAL_GPIO_WritePin>:
 8000014:  b10a        cbz    r2, 800001a      @ PinState == 0 → гілка reset
 8000016:  6181        str    r1, [r0, #24]    @ set:   BSRR = GPIO_Pin
 8000018:  4770        bx     lr               @ повернення
 800001a:  0409        lsls   r1, r1, #16      @ reset: GPIO_Pin << 16
 800001c:  6181        str    r1, [r0, #24]    @        BSRR = GPIO_Pin << 16
 800001e:  4770        bx     lr
```

| Інструкція | Що робить |
|---|---|
| `cbz r2, адреса` | Compare and Branch if Zero: якщо `r2 == 0`, перейти за адресою |
| `str r1, [r0, #24]` | Записати `r1` у пам'ять за адресою `r0 + 24` |
| `lsls r1, r1, #16` | Зсунути `r1` вліво на 16 бітів (`<< 16`) |
| `bx lr` | Перейти за адресою з `lr`, тобто повернутися туди, звідки викликали |

- Адреси тепер реальні: з `0x08000000` (початок FLASH у `mini.ld`).
- `[r0, #24]`: `r0` = `0x40020400`, `#24` = `0x18` = зміщення BSRR.
- `if` перетворився на `cbz`: «якщо `r2` = 0, перейти на гілку reset».
- Сам запис у пін — одна інструкція `str`.
- Адресу `0x40020400` лінкер не змінював: вона відома з `#define`. Лінкер розміщує лише функції та змінні.

```bash
arm-none-eabi-size firmware.elf
   text    data     bss     dec     hex filename
     32       0       0      32      20 firmware.elf
```

| Колонка | Значення |
|---|---|
| `text` | Код і константи → Flash |
| `data` | Глобальні змінні з початковим значенням → Flash (копія) + RAM |
| `bss` | Глобальні змінні без значення (нулі) → RAM |
| `dec`, `hex` | Сума трьох колонок у десятковому і шістнадцятковому вигляді |

> `mini.ld` спрощений: у справжньому проєкті CubeIDE за адресою `0x08000000` лежить таблиця векторів, а не `main`.

## 6. Make: збирання однією командою

Make — програма, яка запускає команди збирання за правилами з файлу `Makefile` і перезбирає **лише те, що змінилося**. Вона порівнює час зміни файлів: якщо `.c` новіший за свій `.o`, `.o` треба перезібрати.

`Makefile`:

```make
CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m4 -mthumb -O1
LDFLAGS = -nostdlib -T mini.ld -e main

firmware.elf: main.o hal_gpio.o
	$(CC) $(CFLAGS) $(LDFLAGS) main.o hal_gpio.o -o firmware.elf

main.o: main.c hal_mini.h
	$(CC) $(CFLAGS) -c main.c -o main.o

hal_gpio.o: hal_gpio.c hal_mini.h
	$(CC) $(CFLAGS) -c hal_gpio.c -o hal_gpio.o

clean:
	rm -f *.o firmware.elf
```

Як читати правило:

```make
ціль: залежності
	команда
```

| Частина | Значення |
|---|---|
| ціль (target) | Файл, який треба отримати (`main.o`) |
| залежності (prerequisites) | Файли, з яких він робиться (`main.c hal_mini.h`) |
| команда (recipe) | Як його зробити. Рядок **обов'язково** починається з табуляції, не з пробілів |
| `CC = ...` | Змінна. `$(CC)` підставляє її значення |
| `clean` | Ціль без файлу: просто команда видалення результатів збірки |

`make` без аргументів будує першу ціль у файлі (`firmware.elf`). Для неї потрібні `main.o` і `hal_gpio.o`, тому спершу Make будує їх.

### Перша збірка: усе

```bash
make
```

```
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -c main.c -o main.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -c hal_gpio.c -o hal_gpio.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -nostdlib -T mini.ld -e main main.o hal_gpio.o -o firmware.elf
```

### Нічого не змінено: нічого не робиться

```bash
make
```

```
make: 'firmware.elf' is up to date.
```

### Змінено лише `main.c`: `hal_gpio.o` не перезбирається

```bash
make
```

```
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -c main.c -o main.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -nostdlib -T mini.ld -e main main.o hal_gpio.o -o firmware.elf
```

### Змінено `hal_mini.h`: перезбирається все

Обидва `.o` залежать від `hal_mini.h`.

```
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -c main.c -o main.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -c hal_gpio.c -o hal_gpio.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 -nostdlib -T mini.ld -e main main.o hal_gpio.o -o firmware.elf
```

> Якщо не вказати `hal_mini.h` у залежностях, після зміни заголовка Make вирішить, що все актуально, і прошивка збереться зі старими значеннями.

### Make у CubeIDE

CubeIDE сам генерує `Makefile` у папці `Debug/` (або `Release/`), а кнопка Build запускає `make`. У вкладці Console видно ті самі команди `arm-none-eabi-gcc ... -c ...`.

## 7. Підсумок

| Етап | Що сталося з `HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)` |
|---|---|
| Препроцесор | `GPIOB`, `GPIO_PIN_5` → числовий текст |
| Компілятор | аргументи → `r0`, `r1`, `r2`; виклик → `bl HAL_GPIO_WritePin` |
| Асемблер | байти; адреса функції = заглушка + relocation |
| Лінкер | `bl 0x08000014`, код HAL доданий у прошивку |
| Make | запускає ці етапи лише для змінених файлів |
