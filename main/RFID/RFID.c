#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sky1311_drv.h"

static void RFID_Task(void* arg);





void RFID_Init(void)
{
	SPI_Config();

	xTaskCreate(RFID_Task, "RFID_Task", 1024 * 2, (void* ) 0, 10, NULL);



}



void RFID_Task(void* arg)
{


	for(;;)
		{
			uint8_t data[2]={0x81,0x81};
			SPI_SendData(data,sizeof(data));

			 vTaskDelay(100 / portTICK_PERIOD_MS);	  

		}


}




