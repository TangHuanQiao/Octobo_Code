
#include "LED_Ctr.h"





#define SLAVE_DEV_ADDR      	           0x5B             /*!< ESP32 slave address, you can set any 7bit value */







#define AW9623B_ID 0x23
#define AW9623B_ID_REG 0x10

#define GLOBAL_CONIFG_REG 0x11
#define P0_LED_Mode_REG 0X12
#define P1_LED_Mode_REG 0X13

#define P0_0_DIM_REG 0X24
#define P0_1_DIM_REG 0X25
#define P0_2_DIM_REG 0X26
#define P0_3_DIM_REG 0X27
#define P0_4_DIM_REG 0X28
#define P0_5_DIM_REG 0X29
#define P0_6_DIM_REG 0X2A
#define P0_7_DIM_REG 0X2B

#define P1_0_DIM_REG 0X20
#define P1_1_DIM_REG 0X21
#define P1_2_DIM_REG 0X22
#define P1_3_DIM_REG 0X23
#define P1_4_DIM_REG 0X2C
#define P1_5_DIM_REG 0X2D
#define P1_6_DIM_REG 0X2E
#define P1_7_DIM_REG 0X2F



static const uint8_t Led_RGB_Tab[RGB_LED_MaxNUM][3]={
												{P1_3_DIM_REG,P0_0_DIM_REG,P0_1_DIM_REG},
												{P1_0_DIM_REG,P1_1_DIM_REG,P1_2_DIM_REG},
												{P1_4_DIM_REG,P1_5_DIM_REG,P1_6_DIM_REG},
												{P0_5_DIM_REG,P0_6_DIM_REG,P0_7_DIM_REG},
												{P0_2_DIM_REG,P0_3_DIM_REG,P0_4_DIM_REG},												
												
										    };



 static esp_err_t AW9623B_i2c_write(uint8_t RegAddr,uint8_t Data)
 {
 	uint8_t TempData[2]={0};
	TempData[0]=RegAddr;
	TempData[1]=Data;	
	 return i2c_demo_write(SLAVE_DEV_ADDR,I2C_NUM_1,TempData,sizeof(TempData));
	
 }

   esp_err_t AW9623B_i2c_read(uint8_t RegAddr,uint8_t* pData)
 {
 	 i2c_demo_write(SLAVE_DEV_ADDR,I2C_NUM_1,&RegAddr,1);

	 return i2c_demo_read(SLAVE_DEV_ADDR,I2C_NUM_1,pData,1);
 }





  

 void LED_Ctr_Init(void)
 {
 	uint8_t TempID=0;

	LED_RESET_HIGH;

	AW9623B_i2c_read(AW9623B_ID_REG,&TempID);
	
	if(AW9623B_ID==TempID)
	{
		AW9623B_i2c_write(GLOBAL_CONIFG_REG,0x03);
		AW9623B_i2c_write(P0_LED_Mode_REG,0X00);
		AW9623B_i2c_write(P1_LED_Mode_REG,0X00);


		printf("Enter LED_Ctr_Init...\r\n");

	}
		
 }
 	

void LED_Brightness_Set(uint8_t Led_Index,uint8_t Brightness)
{

		Led_Index=Led_RGB_Tab[Led_Index/3][Led_Index%3];
			
		AW9623B_i2c_write(Led_Index,Brightness);


}



void LED_All_Clean(void)
{

	uint8_t TempIndex;
	for(TempIndex=0;TempIndex<RGB_LED_MaxNUM*3;TempIndex++)
	{
	LED_Brightness_Set(TempIndex,0); 
	}


}





