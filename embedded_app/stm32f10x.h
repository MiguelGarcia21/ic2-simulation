#pragma once

#include <stdint.h>

// Memory map
#define PERIPH_BASE        0x40000000U
#define APB1PERIPH_BASE    PERIPH_BASE
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x10000U)
#define AHBPERIPH_BASE     (PERIPH_BASE + 0x20000U)

#define RCC_BASE           (AHBPERIPH_BASE + 0x1000U)
#define GPIOA_BASE         (APB2PERIPH_BASE + 0x0800U)
#define GPIOC_BASE         (APB2PERIPH_BASE + 0x1000U)
#define USART2_BASE        (APB1PERIPH_BASE + 0x4400U)

// Registers
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

// Peripheral pointers
#define RCC        ((RCC_TypeDef *) RCC_BASE)
#define GPIOA      ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC      ((GPIO_TypeDef *) GPIOC_BASE)
#define USART2     ((USART_TypeDef *) USART2_BASE)

// Register bits
#define RCC_APB2ENR_IOPAEN   (1 << 2)
#define RCC_APB2ENR_IOPCEN   (1 << 4)
#define RCC_APB1ENR_USART2EN (1 << 17)

// GPIO configuration
// For CRL (pins 0-7)
#define GPIO_CRL_CNF2        (0x3 << 10)  // CNF2[1:0] bits
#define GPIO_CRL_CNF3        (0x3 << 12)  // CNF3[1:0] bits
#define GPIO_CRL_MODE2       (0x3 << 8)   // MODE2[1:0] bits
#define GPIO_CRL_MODE3       (0x3 << 12)  // MODE3[1:0] bits

// For CRH (pins 8-15)
#define GPIO_CRH_CNF8        (0x3 << 2)   // CNF8[1:0] bits
#define GPIO_CRH_CNF9        (0x3 << 6)   // CNF9[1:0] bits
#define GPIO_CRH_MODE8       (0x3 << 0)   // MODE8[1:0] bits
#define GPIO_CRH_MODE9       (0x3 << 4)   // MODE9[1:0] bits

// Configuration values
#define GPIO_CNF_OUTPUT_PP   0x0
#define GPIO_CNF_OUTPUT_OD   0x1
#define GPIO_CNF_AF_PP       0x2
#define GPIO_CNF_AF_OD       0x3
#define GPIO_CNF_INPUT_AN    0x0
#define GPIO_CNF_INPUT_FLOAT 0x1
#define GPIO_CNF_INPUT_PUPD  0x2

// Mode values
#define GPIO_MODE_INPUT      0x0
#define GPIO_MODE_OUTPUT_10  0x1
#define GPIO_MODE_OUTPUT_2   0x2
#define GPIO_MODE_OUTPUT_50  0x3

// Output data register
#define GPIO_ODR_ODR8        (1 << 8)
#define GPIO_ODR_ODR9        (1 << 9)

// USART status and control
#define USART_SR_TXE         (1 << 7)
#define USART_SR_RXNE        (1 << 5)
#define USART_CR1_UE         (1 << 13)
#define USART_CR1_TE         (1 << 3)
#define USART_CR1_RE         (1 << 2)