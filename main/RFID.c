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
	/* RC Ƶ��У׼�������ӽ�13.56M��RCֵ�Լ���Ӧ��ADֵ
	   У׼ʱ�����п������������忿�����ߣ�����У׼��ֵ����ȷ
	*/
	rcValue = sky1311RCFreqCali();
#if DEBUG==1
	if(rcValue == 0)        // û�вɼ������ʵ�ADֵ
	{
	  uart_puts("RC Cali Error\n");

	}
#endif
	while(1)
	{
		vTaskDelay(50 / portTICK_PERIOD_MS);



	    if(cardStatus == CARDREMOVED)           // û���ڳ��ڣ��Ѿ����ߣ�
	    {
	        irqClearAll();              // ���SKY1311�������ж�
	        checkCardInit(rcValue);     // ���ý����Զ��쿨״̬

//			while(1)
			 { 
				xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY);
				
				 printf("io_num====%d\r\n",io_num);
				 vTaskDelay(100 / portTICK_PERIOD_MS);
			 } 


	        /* ������MCU ���ߺ��� */
	        if(SKY1311_IRQ_READ())       // �쿨�жϲ����������˱��
	        {
#if DEBUG==1
	            uint8_t currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5);
	            NEW_ROW();
	            PUTS("CurrAdcVal=");
	            PRINT(currAdcVal);
	            NEW_ROW();
#endif
	            irq_sta = sky1311ReadReg(ADDR_IRQ_STA); // ���жϼĴ���
	            if((0==irq_sta) &&                  // û���κ��жϲ�������
	               (0!=SKY1311_IRQ_READ())) // IRQ ���ű�ߵ�ƽ
	            {
	                checkCardDeinit();          // ֹͣ�Զ��쿨
	                cardStatus = HASCARDIN;     // ���ñ�ǣ��п�����
	            }

#if DEBUG == 1
	            uart_puts("\r\n**** Detected Card ****\r\n");
#endif
	        }
	    }
	    else if(cardStatus == HASCARDIN)        // �п��ڳ���
	    {
	        if(Ok == TypeA_test())          // ���Զ������ж��Ƿ�Type A ����
	        {
	            cardStatus |= READCARDOVER; // ��Ƕ������
	        }
	        else
	        {
	            if(Ok== SmartTypeB_test())  // ���Զ������Ƿ�Type B �������֤��
	            {
	                cardStatus |= READCARDOVER; // ��Ƕ������
	            }
	            else        // û�ж�����
	            {
	                //rcValue = sky1311RCFreqCali(); // ����У׼RCƵ��
	                cardStatus = CARDREMOVED;

	            }
	        }
	    }
	    else if(cardStatus == (HASCARDIN + READCARDOVER))   // �п��ڳ��ڣ����Ѿ�����
	    {
	        checkCardRemoveConfig(rcValue);     // ���üĴ������жϿ��뿪
	        cardStatus = WAITREMOVE;            // ��ǵȴ����뿪
	    }
	    else if(cardStatus == WAITREMOVE)       // �ȴ����뿪
	    {
	        DelayMS(100);
	        if(checkCardRemove((uint8_t)(rcValue&0xff)))    // �жϿ��Ƿ��뿪
	        {
	            cardStatus = CARDREMOVED;       // ��ǿ��Ѿ��뿪

	        }
	    }
	}
#endif

	


}




