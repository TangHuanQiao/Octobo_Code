
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

 #if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */


#define UINT32 unsigned int


typedef struct _KEY_DEVICE_CLASS
{
    UINT32           KeyCnt;
    UINT32           KeyBackup;
	
}KEY_DEVICE_CLASS;


#define KEY_TIME_SCAN		10	//ms
//乘以扫描时间
#define KEY_TIME_DOWN               2    //按键按下
#define KEY_TIME_PRESS_START        300   //开始判定为长按
#define KEY_TIME_PRESS_STEP         10   //长按一步的时间范围
#define KEY_TIME_DOUBLE				15  //连击动作时间


#define KEY_STATUS_NONE             ((UINT32)0xffffffff)
#define KEY_STATUS_DOWN             ((UINT32)0x0001 << 28)      //0x1000,0000
#define KEY_STATUS_PRESS_START      ((UINT32)0x0002 << 28)      //0x2000,0000
#define KEY_STATUS_PRESS            ((UINT32)0x0003 << 28)      //0x3000,0000
#define KEY_STATUS_SHORT_UP         ((UINT32)0x0004 << 28)      //0x4000,0000
#define KEY_STATUS_LONG_UP          ((UINT32)0x0005 << 28)      //0x5000,0000
#define KEY_STATUS_DOUBLE_CLICK		((UINT32)0x0006 << 28) 


#define KEY_VAL_NONE            ((UINT32)0x0000)
#define KEY_VAL_POWER  			((UINT32)0x0001 << 0)
#define KEY_VAL_TOUCH1			((UINT32)0x0001 << 1)
#define KEY_VAL_TOUCH2			((UINT32)0x0001 << 2)
#define KEY_VAL_TOUCH3			((UINT32)0x0001 << 3)
#define KEY_VAL_TOUCH4			((UINT32)0x0001 << 4)
#define KEY_VAL_TOUCH5			((UINT32)0x0001 << 5)
#define KEY_VAL_TOUCH6			((UINT32)0x0001 << 6)
#define KEY_VAL_TOUCH7			((UINT32)0x0001 << 7)
#define KEY_VAL_TOUCH8			((UINT32)0x0001 << 8)



#define KEY_VAL_POWER_DOWN           ((KEY_VAL_POWER)|(KEY_STATUS_DOWN))
#define KEY_VAL_POWER_PRESS_START    ((KEY_VAL_POWER)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_POWER_PRESS          ((KEY_VAL_POWER)|(KEY_STATUS_PRESS))
#define KEY_VAL_POWER_SHORT_UP       ((KEY_VAL_POWER)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_POWER_LONG_UP        ((KEY_VAL_POWER)|(KEY_STATUS_LONG_UP))
#define KEY_VAL_POWER_DOUBLE_CLICK	 ((KEY_VAL_POWER)|(KEY_STATUS_DOUBLE_CLICK))


#define KEY_VAL_TOUCH1_DOWN           ((KEY_VAL_TOUCH1)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH1_PRESS_START    ((KEY_VAL_TOUCH1)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH1_PRESS          ((KEY_VAL_TOUCH1)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH1_SHORT_UP       ((KEY_VAL_TOUCH1)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH1_LONG_UP        ((KEY_VAL_TOUCH1)|(KEY_STATUS_LONG_UP))


#define KEY_VAL_TOUCH2_DOWN           ((KEY_VAL_TOUCH2)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH2_PRESS_START    ((KEY_VAL_TOUCH2)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH2_PRESS          ((KEY_VAL_TOUCH2)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH2_SHORT_UP       ((KEY_VAL_TOUCH2)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH2_LONG_UP        ((KEY_VAL_TOUCH2)|(KEY_STATUS_LONG_UP))

#define KEY_VAL_TOUCH3_DOWN           ((KEY_VAL_TOUCH3)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH3_PRESS_START    ((KEY_VAL_TOUCH3)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH3_PRESS          ((KEY_VAL_TOUCH3)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH3_SHORT_UP       ((KEY_VAL_TOUCH3)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH3_LONG_UP        ((KEY_VAL_TOUCH3)|(KEY_STATUS_LONG_UP))


#define KEY_VAL_TOUCH4_DOWN           ((KEY_VAL_TOUCH4)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH4_PRESS_START    ((KEY_VAL_TOUCH4)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH4_PRESS          ((KEY_VAL_TOUCH4)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH4_SHORT_UP       ((KEY_VAL_TOUCH4)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH4_LONG_UP        ((KEY_VAL_TOUCH4)|(KEY_STATUS_LONG_UP))


#define KEY_VAL_TOUCH5_DOWN           ((KEY_VAL_TOUCH5)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH5_PRESS_START    ((KEY_VAL_TOUCH5)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH5_PRESS          ((KEY_VAL_TOUCH5)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH5_SHORT_UP       ((KEY_VAL_TOUCH5)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH5_LONG_UP        ((KEY_VAL_TOUCH5)|(KEY_STATUS_LONG_UP))

#define KEY_VAL_TOUCH6_DOWN           ((KEY_VAL_TOUCH6)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH6_PRESS_START    ((KEY_VAL_TOUCH6)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH6_PRESS          ((KEY_VAL_TOUCH6)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH6_SHORT_UP       ((KEY_VAL_TOUCH6)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH6_LONG_UP        ((KEY_VAL_TOUCH6)|(KEY_STATUS_LONG_UP))

#define KEY_VAL_TOUCH7_DOWN           ((KEY_VAL_TOUCH7)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH7_PRESS_START    ((KEY_VAL_TOUCH7)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH7_PRESS          ((KEY_VAL_TOUCH7)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH7_SHORT_UP       ((KEY_VAL_TOUCH7)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH7_LONG_UP        ((KEY_VAL_TOUCH7)|(KEY_STATUS_LONG_UP))

#define KEY_VAL_TOUCH8_DOWN           ((KEY_VAL_TOUCH8)|(KEY_STATUS_DOWN))
#define KEY_VAL_TOUCH8_PRESS_START    ((KEY_VAL_TOUCH8)|(KEY_STATUS_PRESS_START))
#define KEY_VAL_TOUCH8_PRESS          ((KEY_VAL_TOUCH8)|(KEY_STATUS_PRESS))
#define KEY_VAL_TOUCH8_SHORT_UP       ((KEY_VAL_TOUCH8)|(KEY_STATUS_SHORT_UP))
#define KEY_VAL_TOUCH8_LONG_UP        ((KEY_VAL_TOUCH8)|(KEY_STATUS_LONG_UP))


void keyScanTask(void);
void KeyEventCallBack(UINT32 KeyVal);
void KeyValConvert(UINT32 *pKeyVal);


#endif

