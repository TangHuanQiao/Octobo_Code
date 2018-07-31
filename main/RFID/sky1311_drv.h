/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SKY1311_DRV_H
#define __SKY1311_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"




#define SKY1311_IRQ_READ()   ;

#define SKY1311_DISABLE()    ;
#define SKY1311_ENABLE()     ;
#define sky1311Disable()    SKY1311_DISABLE()
#define sky1311Enable()     SKY1311_ENABLE()





void sky1311WriteCmd(uint8_t cmd);
void sky1311WriteReg(uint8_t regAdd, uint8_t data);
uint8_t sky1311ReadReg(uint8_t regAdd);
void sky1311WriteFifo(uint8_t *data, uint8_t count);
void sky1311ReadFifo(uint8_t *data, uint8_t count);
//void SKY1311_SYSCLK(void);

void SPI_Config(void);
void SPI_SendData( uint8_t *data, int len);
void DelayMS(uint32_t x);
void delay(volatile uint8_t i);


#endif
