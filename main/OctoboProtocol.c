#include "ble_spp_server.h"
#include "OctoboProtocol.h"
#include "Octobo_MainApp.h"
#include "LED_Ctr.h"
#include "RFID.h"



void OctoboProtocolHandler(uint8_t *buf,uint8_t len)
{
		uint8_t i=0;

		if(buf[0]==START_FLAG&&buf[1]==STRING_FORMAT&&(len==buf[2]+4))
			{
				switch(buf[3])
					{
						case P2O_RFID_CMD:
							SetRFID_State(buf[4]);
						break;

						case P2O_LED_CMD:
							if((buf[2]==RGB_LED_MaxNUM*3))
								for(i=0;i<RGB_LED_MaxNUM*3;i++)
									LED_Brightness_Set(i,buf[4+i]);

						break;


						case P2O_VOLTAGE_CMD:
							{
								uint16_t u16TempData=GetBaterryState();
								uint8_t TempDatabuf[2]={0};
								TempDatabuf[0]=(u16TempData&0xff00)>>8;
								TempDatabuf[1]=u16TempData&0x00ff;
								OctoboProtocolSendPack(O2P_VOLTAGE_CMD,TempDatabuf,2);
							}
						
						break;
						

						case P2O_SLEEP_CMD :

						SKY1311_DISABLE();	
						TOUCH_POWER_OFF();
						
						printf("P2O_SLEEP_CMD Entering deep sleep\n");						
						esp_sleep_enable_ext0_wakeup(HOME_KEY_IO,1);
						vTaskDelay(100 / portTICK_PERIOD_MS);	  
						esp_deep_sleep_start(); 

						break;



					}
			}





}






void OctoboProtocolSendPack(uint8_t cmd,uint8_t *pData, uint8_t dataLen)
{

	uint8_t* Pack=NULL;

	Pack = (uint8_t *)malloc(dataLen+4);

	Pack[0]=START_FLAG;
	Pack[1]=STRING_FORMAT;
	Pack[2]=dataLen;
	Pack[3]=cmd;

	memcpy(Pack+4,pData,dataLen);

	ble_spp_server_send(Pack,(dataLen+4));
		
	free(Pack);
}









