/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SKY1311_DRV_H
#define __SKY1311_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"
#include "driver/gpio.h"

#define RFID_POWER_IO  22
#define RFID_INT_IO    21

#define SKY1311_IRQ_READ()   gpio_get_level(RFID_INT_IO)
#define SKY1311_DISABLE()   gpio_set_level(RFID_POWER_IO,0)
#define SKY1311_ENABLE()    gpio_set_level(RFID_POWER_IO,1)
#define sky1311Disable()    SKY1311_DISABLE()
#define sky1311Enable()     SKY1311_ENABLE()

#define NEW_ROW()   uart_newrow()
#define PUTS(x)     uart_puts(x)
#define PRINT(x)    uart_putHex(x)




void sky1311WriteCmd(uint8_t cmd);
void sky1311WriteReg(uint8_t regAdd, uint8_t data);
uint8_t sky1311ReadReg(uint8_t regAdd);
void sky1311WriteFifo(uint8_t *data, uint8_t count);
void sky1311ReadFifo(uint8_t *data, uint8_t count);
//void SKY1311_SYSCLK(void);

void SPI_Config(void);
void DelayMS(uint32_t x);
void delay(volatile uint8_t i);
void uart_puts(char *str);
void uart_newrow();
void uart_printBuffer(uint8_t* buff, uint16_t size);
void uart_putHex(uint8_t data);
void uart_putChar(uint8_t ch);


#endif
