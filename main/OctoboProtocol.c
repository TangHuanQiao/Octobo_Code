#include "ble_spp_server.h"
#include "OctoboProtocol.h"



void OctoboProtocolHandler(uint8_t *buf,uint8_t len)
{
		if(buf[0]==START_FLAG&&buf[1]==STRING_FORMAT)
			{
				switch(buf[3])
					{
						case P2O_RFID_CMD:

						break;

						case P2O_LED_CMD:

						break;


						case P2O_VOLTAGE_CMD:
						
						break;


						case P2O_LEDMOD_CMD:
						
						break;


					}
			}





}






void OctoboProtocolSendPack(uint8_t cmd,uint8_t *pData, uint8_t dataLen)
{

	uint8_t* pPack=NULL;

	pPack = (uint8_t *)malloc(dataLen+4);

	pPack[0]=START_FLAG;
	pPack[1]=STRING_FORMAT;
	pPack[2]=dataLen;
	pPack[3]=cmd;

	memcpy(pData+4,pData,dataLen);

	ble_spp_server_send(pPack,(dataLen+4));
		
	free(pPack);
}









