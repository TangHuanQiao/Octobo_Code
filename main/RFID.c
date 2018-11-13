#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sky1311_drv.h"
#include "mcu.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "cardoper.h"
#include "userdef.h"

#include "RFID.h"


uint16_t rcValue;
boolean hasCardIn=false;
#define  CARDREMOVED        0
#define  HASCARDIN          1
#define  READCARDOVER       2
#define  WAITREMOVE         4
uint8_t  cardStatus = CARDREMOVED;


static uint8_t RFID_State=RFID_ENABLE;

static uint8_t rfid_ReportAppState=0;

static void RFID_Task(void* arg);


void SetRFID_State(uint8_t state)
{
	RFID_State=state;
}

uint8_t GetRFID_State(void)
{
  return RFID_State;
}


uint8_t Get_rfid_ReportAppState()
{

  return rfid_ReportAppState;
}

void Set_RFID_ReportAppState(uint8_t state)
{

	 rfid_ReportAppState=state;
}




void RFID_Init(void)
{
	SPI_Config();

	xTaskCreate(RFID_Task, "RFID_Task", 1024*3, (void* ) 0, 1, NULL);


}




void RFID_Task(void* arg)
{

 	DelayMS(500);
	printf("Enter RFID Task...\r\n");



#if 1

//	sky1311WriteReg(ADDR_MFOUT_SEL, 0x33);      // MFOUT SEL

	while(1)
	{
		DelayMS(50);

		if(GetRFID_State())
			{
			    if(Ok != TypeA_test())              // read Type A card
			    {
			    
					#if 0
					DelayMS(100);
			        if(Ok!= SmartTypeB_test())      // read Type B card
			        {

			        }
					#endif
			    }

			    sky1311Reset();
			}

	}
#else
	/* RC 频率校准，获得最接近13.56M的RC值以及对应的AD值
	   校准时不能有卡或者其它物体靠近天线，否则校准的值不正确
	*/
	rcValue = sky1311RCFreqCali();
#if DEBUG==1
	if(rcValue == 0)        // 没有采集到合适的AD值
	{
	  uart_puts("RC Cali Error\n");

	}
#endif
	while(1)
	{
		vTaskDelay(50 / portTICK_PERIOD_MS);



	    if(cardStatus == CARDREMOVED)           // 没卡在场内（已经移走）
	    {
	        irqClearAll();              // 清除SKY1311的所有中断
	        checkCardInit(rcValue);     // 配置进入自动检卡状态

//			while(1)
			 { 
				xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY);
				
				 printf("io_num====%d\r\n",io_num);
				 vTaskDelay(100 / portTICK_PERIOD_MS);
			 } 


	        /* 以下是MCU 休眠后唤醒 */
	        if(SKY1311_IRQ_READ())       // 检卡中断产生，设置了标记
	        {
#if DEBUG==1
	            uint8_t currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5);
	            NEW_ROW();
	            PUTS("CurrAdcVal=");
	            PRINT(currAdcVal);
	            NEW_ROW();
#endif
	            irq_sta = sky1311ReadReg(ADDR_IRQ_STA); // 读中断寄存器
	            if((0==irq_sta) &&                  // 没有任何中断产生，且
	               (0!=SKY1311_IRQ_READ())) // IRQ 引脚变高电平
	            {
	                checkCardDeinit();          // 停止自动检卡
	                cardStatus = HASCARDIN;     // 设置标记，有卡靠近
	            }

#if DEBUG == 1
	            uart_puts("\r\n**** Detected Card ****\r\n");
#endif
	        }
	    }
	    else if(cardStatus == HASCARDIN)        // 有卡在场内
	    {
	        if(Ok == TypeA_test())          // 尝试读卡，判断是否Type A 卡，
	        {
	            cardStatus |= READCARDOVER; // 标记读卡完成
	        }
	        else
	        {
	            if(Ok== SmartTypeB_test())  // 尝试读卡，是否Type B 卡（身份证）
	            {
	                cardStatus |= READCARDOVER; // 标记读卡完成
	            }
	            else        // 没有读到卡
	            {
	                //rcValue = sky1311RCFreqCali(); // 重新校准RC频率
	                cardStatus = CARDREMOVED;

	            }
	        }
	    }
	    else if(cardStatus == (HASCARDIN + READCARDOVER))   // 有卡在场内，且已经读过
	    {
	        checkCardRemoveConfig(rcValue);     // 配置寄存器，判断卡离开
	        cardStatus = WAITREMOVE;            // 标记等待卡离开
	    }
	    else if(cardStatus == WAITREMOVE)       // 等待卡离开
	    {
	        DelayMS(100);
	        if(checkCardRemove((uint8_t)(rcValue&0xff)))    // 判断卡是否离开
	        {
	            cardStatus = CARDREMOVED;       // 标记卡已经离开

	        }
	    }
	}
#endif

	


}




