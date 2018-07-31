#ifndef __CARDOPER_H
#define __CARDOPER_H

#include "mcu.h"
#include "typedef.h"

extern uint8_t block_length;
extern uint8_t mifare_block_buff[256];

uint8_t mifare1_readDemo(void);
uint8_t SmartTypeA_readDemo(void);

sta_result_t TypeA_test(void);
uint8_t SmartTypeB_test(void);
#endif