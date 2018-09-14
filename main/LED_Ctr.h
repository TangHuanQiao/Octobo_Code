#include <stdint.h>
#include "I2C_Demo.h"





#define LED_RESET_IO       4
#define LED_RESET_HIGH	gpio_set_level(LED_RESET_IO,1)
#define LED_RESET_LOW	gpio_set_level(LED_RESET_IO,0)



#define LED_OFF 0
#define LED_ON  0XFF

#define RGB_LED_MaxNUM 5

//enum LED_CtrMode
//{
//	ON_OFF_MOD=1,
//	RL_ROLLING_MOD,
//	LR_ROLLING_MOD,
//	COLOR_ROLLING_MOD,
//	OUTWARD_MOD,
//	INWARD_MOD,	
//	
//};


void LED_Ctr_Init(void);

void LED_Brightness_Set(uint8_t Led_Index,uint8_t Brightness);

void LED_Test_Dispaly(void);


esp_err_t AW9623B_i2c_read(uint8_t RegAddr,uint8_t* pData);








