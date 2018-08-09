
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ble_spp_server.h"
#include "Keyboard.h"
#include "driver/touch_pad.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "OctoboProtocol.h"
#include "RFID.h"
#include "Octobo_MainApp.h"



static const uint8_t Touch_Ch_Tab[]={9,8,2,3,4,5,6,7};
static const uint32_t Touch_KEY_VAL_Tab[]={KEY_VAL_TOUCH1,KEY_VAL_TOUCH2,KEY_VAL_TOUCH3,KEY_VAL_TOUCH4,
							       KEY_VAL_TOUCH5,KEY_VAL_TOUCH6,KEY_VAL_TOUCH7,KEY_VAL_TOUCH8};




static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_7;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

static uint8_t BaterryState=BAT_NORMAL;



void BSP_Gpio_Init(void)
{
	gpio_config_t io_conf;

	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	//	gpio_set_level(GPIO_OUTPUT_IO_0,0);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;	
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_down_en= 1;
	gpio_config(&io_conf);	

}

void BSP_TouchPad_Init(void)
{

	// Initialize touch pad peripheral.
    // The default fsm mode is software trigger mode.
    touch_pad_init();
	touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
	for(uint8_t i=0;i<sizeof(Touch_Ch_Tab);i++)
	touch_pad_config(Touch_Ch_Tab[i], TOUCH_THRESH_NO_USE);
	
	touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);


}
	

void BSP_ADC_Init(void)
{


//	dac_output_enable(DAC_CHANNEL_1);
//	dac_output_voltage( DAC_CHANNEL_1, 20);


	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(channel, atten);
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
 

}

 void KeyEventCallBack(UINT32 KeyVal)
{
   uint8_t tempDataBuf[2];
   uint32_t TOUCH_Num;

  	
   TOUCH_Num=KeyVal&0xffff;
    for(int i=1;TOUCH_Num>0;i++)
   	{
		TOUCH_Num=TOUCH_Num>>1;
		if(TOUCH_Num==1)
			{
				TOUCH_Num=i;
				break;
			}
   	}
 
   
	switch(KeyVal)
		{
			case KEY_VAL_POWER_DOWN :
				tempDataBuf[0]=BUTTON_PRESS;
				OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,1);
			break;

			
			case KEY_VAL_POWER_PRESS_START:
			case KEY_VAL_POWER_PRESS:
				
				tempDataBuf[0]=BUTTON_LONG_PRESS;
				OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,1);			
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

			tempDataBuf[0]=TOUCH_Num;
			tempDataBuf[1]=TOUCH_PRESS;
			OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,2);					
			printf("-----%d TOUCH is Down-------\r\n",TOUCH_Num);

			break;


			case KEY_VAL_TOUCH1_SHORT_UP:
			case KEY_VAL_TOUCH2_SHORT_UP:			
			case KEY_VAL_TOUCH3_SHORT_UP:
			case KEY_VAL_TOUCH4_SHORT_UP:
			case KEY_VAL_TOUCH5_SHORT_UP:			
			case KEY_VAL_TOUCH6_SHORT_UP:
			case KEY_VAL_TOUCH7_SHORT_UP:			
			case KEY_VAL_TOUCH8_SHORT_UP:

			
			tempDataBuf[0]=TOUCH_Num;
			tempDataBuf[1]=TOUCH_RELEASE;
			OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,2);				
			printf("----%d TOUCH is SHORT_UP--\r\n",TOUCH_Num);

			break;

			case KEY_VAL_TOUCH1_LONG_UP:
			case KEY_VAL_TOUCH2_LONG_UP:			
			case KEY_VAL_TOUCH3_LONG_UP:
			case KEY_VAL_TOUCH4_LONG_UP:
			case KEY_VAL_TOUCH5_LONG_UP:			
			case KEY_VAL_TOUCH6_LONG_UP:
			case KEY_VAL_TOUCH7_LONG_UP:			
			case KEY_VAL_TOUCH8_LONG_UP:
					
			tempDataBuf[0]=TOUCH_Num;
			tempDataBuf[1]=TOUCH_RELEASE;
			OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,2);
			printf("----%d TOUCH is LONG_UP--\r\n",TOUCH_Num);

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
				 *pKeyVal|=Touch_KEY_VAL_Tab[i];
				 printf("Touch_Ch %d:[%4d]\n",Touch_Ch_Tab[i], touch_filter_value);
				}
				
  		}
			
		
}





static void BatteyCheck(void)
{
	uint32_t adc_reading = 0;
	 //Multisampling
	 for (int i = 0; i < NO_OF_SAMPLES; i++) 
	 	{
			 adc_reading += adc1_get_raw((adc1_channel_t)channel);
		}
	 
	 adc_reading /= NO_OF_SAMPLES;
	 //Convert adc_reading to voltage in mV
	 uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

	 if(voltage>1)
		BaterryState=BAT_NORMAL;
	 else
	 	BaterryState=BAT_LOW;

}


uint8_t GetBaterryState(void)
{

	return BaterryState;
}


void app_main()
{


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
	

	BSP_Gpio_Init();

	BSP_TouchPad_Init();

	BSP_ADC_Init();
	
	LED_Ctr_Init();

	RFID_Init();

	ble_spp_server_start();



	LED_Test_Dispaly();


	for(;;)
	{
	  
	  BatteyCheck();
	  keyScanTask();
	  vTaskDelay(KEY_TIME_SCAN/ portTICK_PERIOD_MS);
	  
	}

}









