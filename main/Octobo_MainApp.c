
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ble_spp_server.h"
#include "Keyboard.h"


#define HOME_KEY_IO     34
#define GPIO_INPUT_PIN_SEL  ((1ULL<<HOME_KEY_IO) | (1ULL<<HOME_KEY_IO))

#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_0))



void app_main()
{

	gpio_config_t io_conf;
	uint8_t HOME_KEY_FliterCnt=0;


	esp_sleep_enable_ext0_wakeup(HOME_KEY_IO,1);

	if (esp_sleep_get_wakeup_cause()==ESP_SLEEP_WAKEUP_EXT0)
	{
		for(;;)
		{

		  if(gpio_get_level(HOME_KEY_IO)!=0)	
			 HOME_KEY_FliterCnt++;
		  else
		  	{		 
			 printf("Again sleep\n");
			 vTaskDelay(300 / portTICK_PERIOD_MS);	 
			 
			 esp_deep_sleep_start(); 

		  	}
		  			
		  if(HOME_KEY_FliterCnt>=10){HOME_KEY_FliterCnt=0; printf("Wake up from GPIO\n"); break;}

		  vTaskDelay(300 / portTICK_PERIOD_MS);	  

		}
	   
	}
	

	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

//	gpio_set_level(GPIO_OUTPUT_IO_0,0);


	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;  
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_down_en= 1;
	gpio_config(&io_conf);	

	Ble_spp_Server_Start();


	for(;;)
	{
	  keyScanTask();
	  vTaskDelay(30/ portTICK_PERIOD_MS);

	  

	}

}











 void KeyEventCallBack(UINT32 KeyVal)
{
	switch(KeyVal)
		{
			case KEY_VAL_POWER_PRESS:
				esp_sleep_enable_ext0_wakeup(HOME_KEY_IO,1);
				printf("KEY_VAL_POWER_PRESS Entering deep sleep\n");
				vTaskDelay(300 / portTICK_PERIOD_MS);	  
				esp_deep_sleep_start();
			break;

		}
}



 void KeyValConvert(UINT32 *pKeyVal)
{

		if(gpio_get_level(HOME_KEY_IO)!=0)			
			*pKeyVal|=KEY_VAL_POWER;
			
		
}


















