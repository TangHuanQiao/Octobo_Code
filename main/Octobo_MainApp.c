
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ble_spp_server.h"
#include "Keyboard.h"
#include "driver/touch_pad.h"



#define HOME_KEY_IO     34
#define GPIO_INPUT_PIN_SEL  ((1ULL<<HOME_KEY_IO) | (1ULL<<HOME_KEY_IO))

#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_0))



#define TOUCH_THRESH_NO_USE   (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)
const uint8_t Touch_Ch_Tab[]={9,8,2,3,4,5,6,7};
const uint32_t Touch_KEY_VAL_Tab[]={KEY_VAL_TOUCH1,KEY_VAL_TOUCH2,KEY_VAL_TOUCH3,KEY_VAL_TOUCH4,
							       KEY_VAL_TOUCH5,KEY_VAL_TOUCH6,KEY_VAL_TOUCH7,KEY_VAL_TOUCH8};
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



	 // Initialize touch pad peripheral.
    // The default fsm mode is software trigger mode.
    touch_pad_init();
	touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
	for(uint8_t i=0;i<sizeof(Touch_Ch_Tab);i++)
	touch_pad_config(Touch_Ch_Tab[i], TOUCH_THRESH_NO_USE);
	
	touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);



	Ble_spp_Server_Start();


	for(;;)
	{
	  keyScanTask();
	  vTaskDelay(KEY_TIME_SCAN/ portTICK_PERIOD_MS);

	  

	}

}











 void KeyEventCallBack(UINT32 KeyVal)
{
	switch(KeyVal)
		{
			case KEY_VAL_POWER_PRESS:
				esp_sleep_enable_ext0_wakeup(HOME_KEY_IO,1);
				printf("KEY_VAL_POWER_PRESS Entering deep sleep\n");
				vTaskDelay(100 / portTICK_PERIOD_MS);	  
				esp_deep_sleep_start();
			break;

			case KEY_VAL_TOUCH1_DOWN:
			case KEY_VAL_TOUCH2_DOWN:			
			case KEY_VAL_TOUCH3_DOWN:
			case KEY_VAL_TOUCH4_DOWN:
			case KEY_VAL_TOUCH5_DOWN:			
			case KEY_VAL_TOUCH6_DOWN:
			case KEY_VAL_TOUCH7_DOWN:			
			case KEY_VAL_TOUCH8_DOWN:
				
			printf("------TOUCH is Down-------\r\n");

			break;


			case KEY_VAL_TOUCH1_SHORT_UP:
			case KEY_VAL_TOUCH2_SHORT_UP:			
			case KEY_VAL_TOUCH3_SHORT_UP:
			case KEY_VAL_TOUCH4_SHORT_UP:
			case KEY_VAL_TOUCH5_SHORT_UP:			
			case KEY_VAL_TOUCH6_SHORT_UP:
			case KEY_VAL_TOUCH7_SHORT_UP:			
			case KEY_VAL_TOUCH8_SHORT_UP:
						
			printf("--TOUCH is SHORT_UP--\r\n");

			break;

			case KEY_VAL_TOUCH1_LONG_UP:
			case KEY_VAL_TOUCH2_LONG_UP:			
			case KEY_VAL_TOUCH3_LONG_UP:
			case KEY_VAL_TOUCH4_LONG_UP:
			case KEY_VAL_TOUCH5_LONG_UP:			
			case KEY_VAL_TOUCH6_LONG_UP:
			case KEY_VAL_TOUCH7_LONG_UP:			
			case KEY_VAL_TOUCH8_LONG_UP:
						
			printf("--TOUCH is LONG_UP--\r\n");

			break;			


		}
}



 void KeyValConvert(UINT32 *pKeyVal)
{
	    uint16_t touch_filter_value;


		if(gpio_get_level(HOME_KEY_IO)!=0)			
			*pKeyVal|=KEY_VAL_POWER;
		
  		for(uint8_t i=0;i<sizeof(Touch_Ch_Tab);i++)
		{
			touch_pad_read_filtered(Touch_Ch_Tab[i], &touch_filter_value);

			if(touch_filter_value<600)			
				{
				 printf("TOUCH%d:[%4d]\n", i+1, touch_filter_value);
				 *pKeyVal|=Touch_KEY_VAL_Tab[i];
				}




				
  		}
			
		
}


















