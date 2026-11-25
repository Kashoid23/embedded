/*
 * Модуль 3.5: STM32F411CEU6, керування серво від АЦП на регістрах.
 * PB6 = TIM4_CH1 (AF2), PA0 = ADC1_IN0.
 * Адреси й маски: stm32f411xe.h (CMSIS); назви режимів: заголовки HAL.
 */
#include "main.h"
#include "stm32f4xx_hal.h" /* Лише константи HAL. */

volatile uint16_t adc_raw;
volatile uint16_t adc_filtered;
volatile uint16_t servo_pulse_us = SERVO_CENTER_US;

static void Clock_Init(void);
static void SysTick_Init(void);
static void Delay_ms(uint32_t milliseconds);
static void Servo_Init(void);
static void ADC1_Init(void);
static uint16_t ADC1_Read(void);

int main(void)
{
  Clock_Init();
  SysTick_Init();
  Servo_Init();
  ADC1_Init();

  while (1)
  {
    /* Середнє арифметичне вимірювань АЦП. */
    uint32_t adc_sum = 0U;
    for (uint32_t i = 0U; i < ADC_AVERAGE_SAMPLES; ++i)
    {
      adc_raw = ADC1_Read();
      adc_sum += adc_raw;
    }
    adc_filtered = (uint16_t)(adc_sum / ADC_AVERAGE_SAMPLES);
    /* ADC 0..4095 -> 500..2500 мкс; крайні положення калібруються під серво. */
    servo_pulse_us = SERVO_MIN_US + ((uint32_t)adc_filtered * (SERVO_MAX_US - SERVO_MIN_US) + ADC_MAX_VALUE / 2U) / ADC_MAX_VALUE;
    TIM4->CCR1 = servo_pulse_us;                   /* TIM4: ширина імпульсу */
    Delay_ms(SERVO_UPDATE_MS);
  }
}

static void Clock_Init(void)
{
  RCC->CR |= RCC_CR_HSION;                        /* RCC: увімкнення HSI */
  while ((RCC->CR & RCC_CR_HSIRDY) == 0U) {}      /* RCC: готовність HSI */
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_SYSCLKSOURCE_HSI; /* RCC: вибір HSI */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {} /* RCC: підтвердження джерела */
  /* AHB /1, APB1 /1, APB2 /1: ядро та TIM4 отримують 16 МГц. */
  RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2); /* RCC: дільники шин */
  RCC->DCKCFGR &= ~RCC_DCKCFGR_TIMPRE;             /* RCC: тактування таймерів */
  SystemCoreClock = HSI_VALUE;                    /* Змінна CMSIS: частота */
}

static void SysTick_Init(void)
{
  SysTick->CTRL = 0U;                            /* SysTick: зупинка таймера */
  SysTick->LOAD = (SystemCoreClock / SYSTICK_HZ) - 1U; /* SysTick: мілісекундний період */
  SysTick->VAL = 0U;                             /* SysTick: поточний відлік */
  /* TICKINT = 0: без переривань. */
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; /* SysTick: запуск опитування */
}

static void Delay_ms(uint32_t milliseconds)
{
  SysTick->VAL = 0U;                             /* SysTick: скидання відліку */
  while (milliseconds > 0U)
  {
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) {} /* SysTick: прапорець переповнення */
    --milliseconds;
  }
}

static void Servo_Init(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;             /* RCC: тактування GPIOB */
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;              /* RCC: тактування TIM4 */
  (void)RCC->AHB1ENR;                             /* RCC: підтвердження тактування */
  (void)RCC->APB1ENR;                             /* RCC: підтвердження тактування */
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM4RST;           /* RCC: скидання TIM4 */
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM4RST;          /* RCC: зняття скидання */
  GPIOB->OTYPER &= ~GPIO_PIN_6;                   /* GPIOB: тип виходу */
  GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED6) | (GPIO_SPEED_FREQ_LOW << GPIO_OSPEEDR_OSPEED6_Pos); /* GPIOB: швидкість перемикання */
  GPIOB->PUPDR = (GPIOB->PUPDR & ~GPIO_PUPDR_PUPD6) | (GPIO_NOPULL << GPIO_PUPDR_PUPD6_Pos); /* GPIOB: вимкнення підтягування */
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFSEL6) | ((uint32_t)GPIO_AF2_TIM4 << GPIO_AFRL_AFSEL6_Pos); /* GPIOB: вибір AF2 */
  /* 16 МГц / 16 = 1 МГц: такт 1 мкс; 20000 тактів = 50 Гц. */
  TIM4->CR1 = TIM_AUTORELOAD_PRELOAD_ENABLE;      /* TIM4: буферизація періоду */
  TIM4->PSC = (HSI_VALUE / SERVO_TIMER_HZ) - 1U;  /* TIM4: дільник частоти */
  TIM4->ARR = (SERVO_TIMER_HZ / SERVO_PWM_HZ) - 1U; /* TIM4: межа лічильника */
  TIM4->CCR1 = SERVO_CENTER_US;                   /* TIM4: ширина імпульсу */
  TIM4->CCMR1 = TIM_OCMODE_PWM1 | TIM_CCMR1_OC1PE; /* TIM4: режим PWM1 */
  TIM4->CCER = 0U;                               /* TIM4: вимкнення виходу */
  TIM4->EGR = TIM_EGR_UG;                         /* TIM4: примусове оновлення */
  TIM4->SR = 0U;                                 /* TIM4: скидання прапорців */
  TIM4->CNT = 0U;                                /* TIM4: поточний відлік */
  GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE6) | ((GPIO_MODE_AF_PP & GPIO_MODE) << GPIO_MODER_MODE6_Pos); /* GPIOB: альтернативний режим */
  TIM4->CCER = TIM_OUTPUTSTATE_ENABLE | TIM_OCPOLARITY_HIGH; /* TIM4: увімкнення CH1 */
  TIM4->CR1 |= TIM_CR1_CEN;                       /* TIM4: запуск лічильника */
}

static void ADC1_Init(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;             /* RCC: тактування GPIOA */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;              /* RCC: тактування ADC1 */
  (void)RCC->AHB1ENR;                             /* RCC: підтвердження тактування */
  (void)RCC->APB2ENR;                             /* RCC: підтвердження тактування */
  GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE0) | (GPIO_MODE_ANALOG << GPIO_MODER_MODE0_Pos); /* GPIOA: аналоговий режим */
  GPIOA->PUPDR = (GPIOA->PUPDR & ~GPIO_PUPDR_PUPD0) | (GPIO_NOPULL << GPIO_PUPDR_PUPD0_Pos); /* GPIOA: вимкнення підтягування */
  RCC->APB2RSTR |= RCC_APB2RSTR_ADCRST;            /* RCC: скидання ADC1 */
  RCC->APB2RSTR &= ~RCC_APB2RSTR_ADCRST;           /* RCC: зняття скидання */
  /* PCLK2 /4 = 4 МГц; одиночне перетворення, вирівнювання праворуч. */
  ADC->CCR = (ADC->CCR & ~ADC_CCR_ADCPRE) | ADC_CCR_ADCPRE_0; /* ADC: дільник /4 */
  ADC1->CR1 = 0U;                                /* ADC1: 12-бітне перетворення */
  ADC1->CR2 = 0U;                                /* ADC1: одиночний режим */
  ADC1->SMPR1 = 0U;                              /* ADC1: вибірка CH10–18 */
  ADC1->SMPR2 = ADC_SMPR2_SMP0;                   /* ADC1: 480-тактна вибірка */
  ADC1->SQR1 = 0U;                               /* ADC1: довжина послідовності */
  ADC1->SQR2 = 0U;                               /* ADC1: послідовність SQ7–12 */
  ADC1->SQR3 = 0U;                               /* ADC1: канал PA0 */
  ADC1->SR = 0U;                                 /* ADC1: скидання прапорців */
  ADC1->CR2 = ADC_CR2_ADON;                       /* ADC1: увімкнення живлення */
  Delay_ms(1U);                                 /* Стабілізація живлення АЦП. */
}

static uint16_t ADC1_Read(void)
{
  ADC1->SR = 0U;                                 /* ADC1: скидання прапорців */
  ADC1->CR2 |= ADC_CR2_SWSTART;                   /* ADC1: запуск перетворення */
  while ((ADC1->SR & ADC_SR_EOC) == 0U) {}        /* ADC1: готовність результату */
  return (uint16_t)(ADC1->DR & ADC_MAX_VALUE);    /* ADC1: результат перетворення */
}
