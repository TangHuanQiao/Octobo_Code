
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
#include "I2C_Demo.h"
#include "TouchSensor.h"


static const uint32_t Touch_ChIO_Tab[]={TOUCH_CH9_IO,TOUCH_CH8_IO,TOUCH_CH7_IO,TOUCH_CH6_IO,
										TOUCH_CH5_IO,TOUCH_CH4_IO,TOUCH_CH3_IO,TOUCH_CH2_IO,};

static const uint8_t Touch_Ch_Tab[]={9,8,7,6,
									 5,4,3,2};

static const uint32_t Touch_KEY_VAL_Tab[]={KEY_VAL_TOUCH1,KEY_VAL_TOUCH2,KEY_VAL_TOUCH3,KEY_VAL_TOUCH4,
							     		  KEY_VAL_TOUCH5,KEY_VAL_TOUCH6,KEY_VAL_TOUCH7,KEY_VAL_TOUCH8};

static const uint16_t Touch_Press_Threshold[]={750,750,750,750,
												750,750,750,750};


static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_7;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

static uint16_t BaterryState=BAT_NORMAL;



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

 
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
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

#ifndef GPIO_TOUCH_FUN	
	for(uint8_t i=0;i<sizeof(Touch_Ch_Tab);i++)
	touch_pad_config(Touch_Ch_Tab[i], TOUCH_THRESH_NO_USE);
#else
	touch_pad_config(1, TOUCH_THRESH_NO_USE);	
#endif

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

   
	switch(KeyVal)
		{
			case KEY_VAL_POWER_DOWN :
				
			break;

			case KEY_VAL_POWER_SHORT_UP:
				tempDataBuf[0]=BUTTON_PRESS;
				OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,1);
				printf("---KEY_VAL_POWER_SHORT_UP---\r\n");
			break;

			
			case KEY_VAL_POWER_PRESS_START:
			case KEY_VAL_POWER_PRESS:
				printf("----------------long long long long long------------\r\n");
				tempDataBuf[0]=BUTTON_LONG_PRESS;
				OctoboProtocolSendPack(O2P_KEY_CMD,tempDataBuf,1);	

				SKY1311_DISABLE();	
				TOUCH_POWER_OFF();
				
				esp_sleep_enable_ext0_wakeup(HOME_KEY_IO,1);
				printf("KEY_VAL_POWER_PRESS Entering deep sleep\n");
				vTaskDelay(100 / portTICK_PERIOD_MS);	  
				esp_deep_sleep_start(); 

			break;


		}
}



 void KeyValConvert(UINT32 *pKeyVal)
{
	
			   uint16_t  TouchKeyValue=0;
		static uint16_t TouchKeyValueBack=0;		
		uint8_t tempDataBuf[2];
		
		static uint8_t count=0;
		count++;


//-------------key-------------------		
		if(gpio_get_level(HOME_KEY_IO)!=0)			
			*pKeyVal|=KEY_VAL_POWER;


		
//-------------touch--------------------		
  		for(uint8_t i=0;i<sizeof(Touch_Ch_Tab);i++)
		{
#ifdef GPIO_TOUCH_FUN
				if(gpio_get_level(Touch_ChIO_Tab[i])!=0)
					TouchKeyValue|=Touch_KEY_VAL_Tab[i];

					
#else
				uint16_t touch_filter_value;

				touch_pad_read_filtered(Touch_Ch_Tab[i], &touch_filter_value);

				if(touch_filter_value<Touch_Press_Threshold[i])			
					{
					 TouchKeyValue|=Touch_KEY_VAL_Tab[i];				 
					}


//				if(count%10==0)
//					{
//						printf("TOUCH %d:[%4d] ",i+1, touch_filter_value);
//						if(i==sizeof(Touch_Ch_Tab)-1)
//						printf("\r\n");
//					}
#endif
				
  		}
		







	   for(uint8_t i=0;i<sizeof(Touch_Ch_Tab);i++)
	   	{
			if((TouchKeyValue&Touch_KEY_VAL_Tab[i])&&!(TouchKeyValueBack&Touch_KEY_VAL_Tab[i]))
				{
					printf("-----%d TOUCH is Down-------\r\n",i+1);
					TouchKeyValueBack|=Touch_KEY_VAL_Tab[i];

					tempDataBuf[0]=i+1;
					tempDataBuf[1]=TOUCH_PRESS;
					OctoboProtocolSendPack(O2P_TOUCH_CMD,tempDataBuf,2);	

				}

			else if(!(TouchKeyValue&Touch_KEY_VAL_Tab[i])&&(TouchKeyValueBack&Touch_KEY_VAL_Tab[i]))
				{
					printf("-----%d TOUCH is UP-------\r\n",i+1);
					TouchKeyValueBack&=~(Touch_KEY_VAL_Tab[i]);


					tempDataBuf[0]=i+1;
					tempDataBuf[1]=TOUCH_RELEASE;
					OctoboProtocolSendPack(O2P_TOUCH_CMD,tempDataBuf,2);	

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
	 voltage=voltage*13/2;		
//	 printf("\r\n====voltage=%d===\r\n",voltage);


	BaterryState=voltage;


}


uint16_t GetBaterryState(void)
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

	TOUCH_POWER_ON();

	BSP_TouchPad_Init(); //不配置该函数 RFID功能不正常 原因未明

	BSP_ADC_Init();

	i2c_demo_init();
	
	LED_Ctr_Init();

	RFID_Init();

	ble_spp_server_start();


	for(;;)
	{
	  
	  BatteyCheck();
	  keyScanTask();
	  vTaskDelay(KEY_TIME_SCAN/ portTICK_PERIOD_MS);
	  
	}

}









