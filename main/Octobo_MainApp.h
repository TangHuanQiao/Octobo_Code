
#include "sky1311_drv.h"

#define HOME_KEY_IO     34
#define GPIO_INPUT_PIN_SEL  ((1ULL<<HOME_KEY_IO) | (1ULL<<RFID_INT_IO))


#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<RFID_POWER_IO) | (1ULL<<RFID_POWER_IO))

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)


#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling





uint8_t GetBaterryState(void);












