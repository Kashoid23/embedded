# STM32F411: від карти пам'яті до біта в `GPIOB->ODR` і HAL

Приклад: вивід **PB5** на STM32F411CEU6.

## 1. Карта пам'яті → базова адреса

RM0383, *Table 1* (розділ 2.3):

| Діапазон адрес | Периферія | Шина |
|---|---|---|
| `0x4002 0800 – 0x4002 0BFF` | GPIOC | AHB1 |
| **`0x4002 0400 – 0x4002 07FF`** | **GPIOB** | **AHB1** |
| `0x4002 0000 – 0x4002 03FF` | GPIOA | AHB1 |

Базова адреса GPIOB: **`0x4002 0400`**.

## 2. Зміщення регістрів

RM0383, розділ 8.4.11. Кожен регістр займає 4 байти.

| Регістр | Зміщення | Адреса для GPIOB |
|---|---|---|
| `MODER` | `0x00` | `0x4002 0400` |
| `OTYPER` | `0x04` | `0x4002 0404` |
| `OSPEEDR` | `0x08` | `0x4002 0408` |
| `PUPDR` | `0x0C` | `0x4002 040C` |
| `IDR` | `0x10` | `0x4002 0410` |
| **`ODR`** | **`0x14`** | **`0x4002 0414`** |
| `BSRR` | `0x18` | `0x4002 0418` |
| `LCKR` | `0x1C` | `0x4002 041C` |
| `AFR[0]`, `AFR[1]` | `0x20`, `0x24` | `0x4002 0420`, `0x4002 0424` |

```
GPIOB->ODR = 0x4002 0400 + 0x14 = 0x4002 0414
```

## 3. Структура і вказівник (CMSIS)

`stm32f411xe.h`:

```c
typedef struct
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
#define AHB1PERIPH_BASE  (PERIPH_BASE + 0x00020000UL)   /* 0x4002 0000 */
#define GPIOB_BASE       (AHB1PERIPH_BASE + 0x0400UL)   /* 0x4002 0400 */
#define GPIOB            ((GPIO_TypeDef *) GPIOB_BASE)
```

- Поля структури йдуть у тому самому порядку, що й регістри в RM, тому зміщення збігаються.
- `__IO` означає `volatile`: компілятор щоразу реально звертається до регістра.
- `GPIOB->ODR` компілятор перетворює на звернення за адресою `0x4002 0414`.

## 4. Позиція й маска

```c
#define GPIO_ODR_OD5_Pos   (5U)
#define GPIO_ODR_OD5_Msk   (0x1UL << GPIO_ODR_OD5_Pos)  /* 0x00000020 */
#define GPIO_ODR_OD5       GPIO_ODR_OD5_Msk
```

- Біти нумеруються справа наліво з 0. Біт *n* у ODR відповідає виводу PB*n*. Працюють біти 0–15, біти 16–31 зарезервовані.
- `0x1UL` — це одиниця як 32-бітне число без знака (`U` = unsigned, `L` = long). Тому зсув до біта 31 безпечний.

## 5. Побітові операції

| a | b | `a \| b` | `a & b` | `a ^ b` |
|:-:|:-:|:-:|:-:|:-:|
| 0 | 0 | 0 | 0 | 0 |
| 0 | 1 | 1 | 0 | 1 |
| 1 | 0 | 1 | 0 | 1 |
| 1 | 1 | 1 | 1 | 0 |

`~` перевертає кожен біт.

| Дія | Код |
|---|---|
| Set | `GPIOB->ODR \|= GPIO_ODR_OD5;` |
| Clear | `GPIOB->ODR &= ~GPIO_ODR_OD5;` |
| Toggle | `GPIOB->ODR ^= GPIO_ODR_OD5;` |

> `GPIOB->ODR = GPIO_ODR_OD5;` встановлює PB5 = 1, але решту виводів порту B скидає в 0.

## 6. BSRR: атомарний запис

`ODR |= маска` процесор виконує як «прочитати → змінити → записати» (read-modify-write). Якщо між читанням і записом спрацює переривання і змінить інший біт ODR, ця зміна затреться.

BSRR змінює вивід одним записом, без читання:
- одиниця в бітах **0–15** встановлює вивід у 1 (BS);
- одиниця в бітах **16–31** скидає вивід у 0 (BR);
- нулі нічого не змінюють.

```c
GPIOB->BSRR = GPIO_BSRR_BS5;                  /* PB5 = 1 */
GPIOB->BSRR = GPIO_BSRR_BR5;                  /* PB5 = 0 */
GPIOB->BSRR = GPIO_BSRR_BS3 | GPIO_BSRR_BR4;  /* PB3 = 1, PB4 = 0 */
```

## 7. HAL

`GPIO_PIN_5` = `0x0020` — та сама маска.

```c
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  if (PinState != GPIO_PIN_RESET)
    GPIOx->BSRR = GPIO_Pin;                   /* set */
  else
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;  /* reset */
}
```

| Дія | Регістри | HAL |
|---|---|---|
| PB5 = 1 | `GPIOB->BSRR = GPIO_BSRR_BS5;` | `HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);` |
| PB5 = 0 | `GPIOB->BSRR = GPIO_BSRR_BR5;` | `HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);` |
| Toggle | `GPIOB->ODR ^= GPIO_ODR_OD5;` | `HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);` |
| Read | `(GPIOB->IDR & GPIO_IDR_ID5) != 0U` | `HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)` |

---

Джерела: RM0383, CMSIS `stm32f411xe.h`, STM32CubeF4 `stm32f4xx_hal_gpio.c`.
