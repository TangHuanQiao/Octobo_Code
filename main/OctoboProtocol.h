#include <stdint.h>

#define START_FLAG ':'
#define STRING_FORMAT 'b'
#define P2O_RFID_CMD 'n'
#define P2O_LED_CMD 'l'
#define P2O_VOLTAGE_CMD 'v'
#define P2O_LEDMOD_CMD 'm'
#define P2O_SLEEP_CMD 's'


#define O2P_KEY_CMD 'K'
#define O2P_RFID_CMD 'N'
#define O2P_TOUCH_CMD 'T'
#define O2P_VOLTAGE_CMD 'N'



#define BUTTON_PRESS 0
#define BUTTON_LONG_PRESS 1

#define TOUCH_PRESS 1
#define TOUCH_RELEASE 0

#define BAT_NORMAL 0
#define BAT_LOW  1

void OctoboProtocolHandler(uint8_t *buf,uint8_t len);


void OctoboProtocolSendPack(uint8_t cmd,uint8_t *pData, uint8_t dataLen);




























