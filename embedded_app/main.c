#include "stm32f10x.h"
#include <string.h>
#include <stdint.h>

int main(void);

// vector table
__attribute__((section(".isr_vector")))
void * const g_pfnVectors[] = {
    (void*)0x20002000,   // initial stack pointer
    (void*)main          // reset handler
};

void USART2_Init(void) {
    // Enable clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 clock

    // Configure PA2 (TX)
    GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    GPIOA->CRL |= (GPIO_MODE_OUTPUT_50 << 8) | (GPIO_CNF_AF_PP << 10);

    // Configure PA3 (RX) for input floating
    GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
    GPIOA->CRL |= (GPIO_CNF_INPUT_FLOAT << 12);

    // Configure baud rate
    USART2->BRR = 0x1D4C;
    
    // Enable USART
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void LED_Init(void) {
    // Enable GPIOC clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    GPIOC->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);
    GPIOC->CRH |= (GPIO_MODE_OUTPUT_10 << 0) | (GPIO_CNF_OUTPUT_PP << 2);
    
    // Configure PC9 as output push-pull (10MHz)
    GPIOC->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOC->CRH |= (GPIO_MODE_OUTPUT_10 << 4) | (GPIO_CNF_OUTPUT_PP << 6);
    
    // Turn off LEDs (active low)
    GPIOC->ODR |= GPIO_ODR_ODR8 | GPIO_ODR_ODR9;
}

void USART_Send(const char *msg) {
    while(*msg) {
        while(!(USART2->SR & USART_SR_TXE));
        USART2->DR = *msg++;
    }
}

int main(void) {
    LED_Init();
    USART2_Init();
    USART_Send("System Ready\r\n");

    char cmd[32];
    uint8_t idx = 0;
    
    while(1) {
        if(USART2->SR & USART_SR_RXNE) {
            char c = USART2->DR;
            if(c == '\r' || c == '\n') {
                cmd[idx] = '\0';
                idx = 0;
                
                if(strcmp(cmd, "LED1_ON") == 0) {
                    GPIOC->ODR &= ~GPIO_ODR_ODR8;
                    USART_Send("LED1: ON\r\n");
                }
                else if(strcmp(cmd, "LED1_OFF") == 0) {
                    GPIOC->ODR |= GPIO_ODR_ODR8;
                    USART_Send("LED1: OFF\r\n");
                }
                else if(strcmp(cmd, "LED2_ON") == 0) {
                    GPIOC->ODR &= ~GPIO_ODR_ODR9;
                    USART_Send("LED2: ON\r\n");
                }
                else if(strcmp(cmd, "LED2_OFF") == 0) {
                    GPIOC->ODR |= GPIO_ODR_ODR9;
                    USART_Send("LED2: OFF\r\n");
                }
                else if(strcmp(cmd, "STATUS") == 0) {
                    USART_Send("LED1: ");
                    USART_Send((GPIOC->ODR & GPIO_ODR_ODR8) ? "OFF" : "ON");
                    USART_Send(", LED2: ");
                    USART_Send((GPIOC->ODR & GPIO_ODR_ODR9) ? "OFF" : "ON");
                    USART_Send("\r\n");
                }
                else {
                    USART_Send("Unknown command: ");
                    USART_Send(cmd);
                    USART_Send("\r\n");
                }
            } else if (idx < sizeof(cmd) - 1) {
                cmd[idx++] = c;
            }
        }
    }
}