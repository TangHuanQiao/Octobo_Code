#include "ble_spp_server.h"
#include "OctoboProtocol.h"
#include "Octobo_MainApp.h"
#include "LED_Ctr.h"



void OctoboProtocolHandler(uint8_t *buf,uint8_t len)
{
		uint8_t TempData=0;
		uint8_t i=0;

		if(buf[0]==START_FLAG&&buf[1]==STRING_FORMAT)
			{
				switch(buf[3])
					{
						case P2O_RFID_CMD:

						break;

						case P2O_LED_CMD:
							if(buf[2]==RGB_LED_MaxNUM*3)
								for(i=0;i<RGB_LED_MaxNUM*3;i++)
									LED_Brightness_Set(i,buf[4+i]);

						break;


						case P2O_VOLTAGE_CMD:
							TempData=GetBaterryState();
							OctoboProtocolSendPack(O2P_VOLTAGE_CMD,&TempData,1);
						
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









