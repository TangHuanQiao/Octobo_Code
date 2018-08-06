
#define LED_OFF 0
#define LED_ON  0XFF

#define RGB_LED_MaxNUM 5

enum LED_CtrMode
{
	ON_OFF_MOD=1,
	RL_ROLLING_MOD,
	LR_ROLLING_MOD,
	COLOR_ROLLING_MOD,
	OUTWARD_MOD,
	INWARD_MOD,	
	
};


void LED_Ctr_Init(void);

void LED_Brightness_Set(uint8_t Led_Index,uint8_t Brightness);

void LED_Test_Dispaly(void);










