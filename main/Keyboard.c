#include "Keyboard.h"

static KEY_DEVICE_CLASS KeyISRDevHandle={0};

void keyScanTask(void)
{

		UINT32 KeyVal = 0;
		UINT32 KeyValBack = 0;
		static UINT32 KeyDoubleBackup=0;
		static UINT32 KeyDoubleCnt=0;
		
		KEY_DEVICE_CLASS* KeyDevHandle = &KeyISRDevHandle;

        KeyValConvert(&KeyVal);

		KeyValBack = KeyVal;

		if(KeyDoubleCnt)
			KeyDoubleCnt--;
		
		if ((KeyVal != KEY_VAL_NONE) && (KeyVal ==	KeyDevHandle->KeyBackup))  //·ÀÖ¹¶¶¶¯
		{
			KeyDevHandle->KeyCnt++;

			if (KeyDevHandle->KeyCnt == 0)
			{
				KeyDevHandle->KeyCnt = KEY_TIME_PRESS_START + KEY_TIME_PRESS_STEP;
			}
			else if (KeyDevHandle->KeyCnt == KEY_TIME_DOWN)
			{
				
				//Double Click
				if(KeyDoubleCnt==0)
					{
						KeyDoubleBackup=KeyDevHandle->KeyBackup;
						KeyDoubleCnt=KEY_TIME_DOUBLE;
					}
				else if(KeyDoubleBackup==KeyDevHandle->KeyBackup&&KeyDoubleCnt)
					{									
						KeyDoubleBackup=0;
						KeyDoubleCnt=0;

						KeyVal |= KEY_STATUS_DOUBLE_CLICK;
						goto BACK;	
						
					}
			
				//key down
				KeyVal |= KEY_STATUS_DOWN;
				goto BACK;
			}
			else if (KeyDevHandle->KeyCnt == KEY_TIME_PRESS_START)
			{
				//press start
				KeyVal |= KEY_STATUS_PRESS_START;
				goto BACK;
			}
			else if (KeyDevHandle->KeyCnt > KEY_TIME_PRESS_START)
			{
				//press
				if (((KeyDevHandle->KeyCnt - KEY_TIME_PRESS_START) % KEY_TIME_PRESS_STEP) == 0)
				{
					KeyVal |= KEY_STATUS_PRESS;
					goto BACK;
				}
			}
			
		}
		else
		{
			if (KeyDevHandle->KeyCnt >= KEY_TIME_PRESS_START)
			{
				KeyVal = (KeyDevHandle->KeyBackup | KEY_STATUS_LONG_UP);
				KeyDevHandle->KeyCnt = 0;
				goto BACK;
			}
			else if (KeyDevHandle->KeyCnt >= KEY_TIME_DOWN)
			{
				KeyVal = (KeyDevHandle->KeyBackup | KEY_STATUS_SHORT_UP);
				KeyDevHandle->KeyCnt = 0;
				goto BACK;
			}
			else
			{
				KeyDevHandle->KeyCnt = 0;
			
			}
		}

		KeyDevHandle->KeyBackup = KeyValBack;			
		return;

		
	
	BACK:
	KeyEventCallBack(KeyVal);





}




__weak void KeyEventCallBack(UINT32 KeyVal)
{

}



__weak void KeyValConvert(UINT32 *pKeyVal)
{

}






