
#include "sky1311_drv.h"
#include "LED_Ctr.h"

#define GPIO_TOUCH_FUN


#define TOUCH_CH9_IO  32
#define TOUCH_CH8_IO  33
#define TOUCH_CH2_IO  2
#define TOUCH_CH3_IO  15
#define TOUCH_CH4_IO  13
#define TOUCH_CH5_IO  12 
#define TOUCH_CH6_IO  14
#define TOUCH_CH7_IO  27

#define TOUCH_POWER_IO 16
#define TOUCH_POWER_OFF()  gpio_set_level(TOUCH_POWER_IO,0)
#define TOUCH_POWER_ON()  gpio_set_level(TOUCH_POWER_IO,1)

#define HOME_KEY_IO     34



#ifdef GPIO_TOUCH_FUN
#define GPIO_INPUT_PIN_SEL  ((1ULL<<HOME_KEY_IO) | (1ULL<<RFID_INT_IO) \
														| (1ULL<<TOUCH_CH9_IO) | (1ULL<<TOUCH_CH8_IO)| (1ULL<<TOUCH_CH2_IO) | (1ULL<<TOUCH_CH3_IO)\
														| (1ULL<<TOUCH_CH4_IO) | (1ULL<<TOUCH_CH5_IO)| (1ULL<<TOUCH_CH6_IO) | (1ULL<<TOUCH_CH7_IO))
#else
#define GPIO_INPUT_PIN_SEL  ((1ULL<<HOME_KEY_IO) | (1ULL<<RFID_INT_IO))
#endif


#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<LED_RESET_IO) | (1ULL<<RFID_POWER_IO)|(1ULL<<TOUCH_POWER_IO))

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)


#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling





uint16_t GetBaterryState(void);












