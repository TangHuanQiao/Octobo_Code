#include <stdint.h>

#define RFID_DISABLE 0
#define RFID_ENABLE 1

void RFID_Init(void);

void SetRFID_State(uint8_t state);

uint8_t GetRFID_State(void);


uint8_t Get_rfid_ReportAppState();


void Set_RFID_ReportAppState(uint8_t state);
































