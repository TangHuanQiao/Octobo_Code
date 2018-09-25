
#include "TouchSensor.h"
#include "I2C_Demo.h"

#define SLAVE_ADD 0X10

esp_err_t Touch_i2c_read(uint8_t RegAddr,uint8_t* pData);

void TouchReadState(void)
{
	uint8_t data=0;
	
	Touch_i2c_read(0,&data);
	
//	printf("======TouchReadState==%d===\r\n",data);

}



  esp_err_t Touch_i2c_read(uint8_t RegAddr,uint8_t* pData)
{
	i2c_demo_write(SLAVE_ADD,I2C_NUM_1,&RegAddr,1);

	return 0;
}













