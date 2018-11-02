#include "mcu.h"
#include "sky1311_drv.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "userdef.h"
#include "OctoboProtocol.h"
#include "RFID.h"



#if 0
/* �����ַ������10������ */
void Dec2Str(uint32_t DecNum, uint8_t *DecStr, uint8_t *size)
{
    uint8_t temp[22];
    uint8_t i=0;
    *size = 0;
    do{
        temp[i++] = (uint8_t)(DecNum%10)+0x30;
        DecNum/=10;

    }while(DecNum>0);
    *size = i;

    for(i=0;i<*size;i++)
        DecStr[i] = temp[(*size)-i-1];

}
#endif
#if 0
sta_result_t sky1311_ReadTest(void)
{
    uint8_t tmpBuf[64];
    uint16_t tmpSize;
    sta_result_t sta;
    //uint16_t temp1, temp2;
    sta = piccATS(tmpBuf);
    if(sta!=Ok)
        return sta;
    /* direct wakeup */
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x01;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    /*  read all 16 words (32 bytes) EEPROM */

    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x10;
    tmpBuf[2] = 0x00;
    tmpBuf[3] = 0x10;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 4);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,4,tmpBuf,&tmpSize);
    if(sta != Ok)
        return sta;

    /*
    // write EEPROM 00,01,  UID:0--3 = 11223344
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x11;
    tmpBuf[2] = 0x00;
    tmpBuf[3] = 0x22;
    tmpBuf[4] = 0x11;
    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x11;
    tmpBuf[2] = 0x01;
    tmpBuf[3] = 0x44;
    tmpBuf[4] = 0x33;
    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    // write EEPROM 02, 03, UID:4--7 = AABBCCDD
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x11;
    tmpBuf[2] = 0x02;
    tmpBuf[3] = 0xBB;
    tmpBuf[4] = 0xAA;
    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x11;
    tmpBuf[2] = 0x03;
    tmpBuf[3] = 0xDD;
    tmpBuf[4] = 0xCC;

    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    */

    /* // write EEPRM 04 = 0x2000, enable iso14443-4 mode
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x11;
    tmpBuf[2] = 0x04;
    tmpBuf[3] = 0x20;
    tmpBuf[4] = 0x00;
    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    */
    /*  ��������1 */
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x03;
    tmpBuf[2] = 0x21;
    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    if(sta!=Ok)
        return sta;
    //temp1 = (uint16_t)(tmpBuf[2]*0x100) + tmpBuf[3];
    uart_puts("\r\nNTC temperature AD Value=0x\r\n");
    uart_putHex(tmpBuf[2]); uart_putHex(tmpBuf[3]);

    /*  ���������¶� */
    tmpBuf[0] = 0x55;
    tmpBuf[1] = 0x03;
    tmpBuf[2] = 0x22;
    sky1311_fifo_tx(TYPE_A, tmpBuf, 5);
    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    if(sta!=Ok)
        return sta;
    //temp2 = (uint16_t)(tmpBuf[2]*0x100) + tmpBuf[3];
    uart_puts("\r\nDiode temperature AD Value=0x\r\n");
    uart_putHex(tmpBuf[2]); uart_putHex(tmpBuf[3]);
    return Ok;
}
#endif

#if 0
/***************************************************************
    CPU������ʾ��
   ��ȡCPU����UID�ȣ���ʹ�ò���APDU������
   Ѱ���Ȳ���ʱ����ʾ�������
   �д���ʱ��ʾ������Ϣ
****************************************************************/
uint8_t SmartTypeA_readDemo(void)
{
    sta_result_t sta;
    uint8_t UID[10];
    uint8_t tmpBuf[100];
    uint16_t tmpSize;
    SKY1311_ENABLE(); 	    // chip enable PD2
    DelayMS(1);
    sky1311Init();			// init sky1311s
    resetPicc();            // ��λ��Ƭ
    typeAOperate();
    sta = piccRequestA(tmpBuf);
    if(Ok == sta)
    {
        uart_puts("\r\nType-A Card test\r\n");
        uart_puts("ATQA:");
        uart_printBuffer(tmpBuf,1);
        uart_newrow();
        LED_ON();
    }
    else
    {
        sky1311Reset();
        return 1;
    }
    sta = piccAntiA(SEL1, 0x01, UID);
    if(Ok == sta)
    {

        uart_puts("UID:");
        uart_printBuffer(UID,4);
        uart_newrow();
    }
    else
    {
        sky1311Reset();
        uart_puts("Anti error!");
        return 2;
    }
    sta = piccSelectA(SEL1,UID, tmpBuf);
    if(Ok == sta)
    {
        uart_puts("SAK:");
        uart_printBuffer(tmpBuf,1);
        uart_newrow();
    }
    else
    {
        sky1311Reset();
        uart_puts("Select error!");
        return 3;
    }
    sta = piccATS(tmpBuf);
    if(sta == NoResponse){
        sky1311Reset();
        uart_puts("RATS error!");
        return 4;
    }
    else{

    	uart_puts("RATS : ");
    	uart_printBuffer(tmpBuf,32);
        uart_newrow();
    }

     /* select MF file */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x3F;
    tmpBuf[7] = 0x00;   // MF
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 8);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    	return NoResponse;
    else{
        uart_puts("Select MF:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
    }
#if 0
    /*��ȡ�����*/
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0x84;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x08;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 6);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    	return NoResponse;
    else{
        uart_puts("RM:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
    }
#endif
    sky1311Reset();
    LED_OFF();  //GPIO_ResetBits(GPIOD, 1);   //LED off
    return 0;
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/***************************************************************
    M1����ȡ��������ʾ��
   ��ȡ�������ݲ�ͨ�����ڴ�ӡ����
   Ѱ���Ȳ���ʱ����ʾ�������
   �д���ʱ��ʾ������Ϣ
****************************************************************/
sta_result_t mifare1_ReadTest(uint8_t *UID)
{
    sta_result_t sta;
    uint8_t tmpBuf[20];
    uint8_t default_key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t sector_num=0, block_num=0;

#if DEBUG==1
    uart_puts("\r\nRead Mifare1 test\r\n");
#endif
    /* read test Mifare 1K card (0--63 block) */
    for(sector_num=0;sector_num<16;sector_num++)// read sector0--16
    //for(block_num=0;block_num<64;) // read block0--63
    {
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key , UID, block_num);

        if(Ok == sta)
        {
            /*  read 4 block from 1 sector */
            for(uint8_t i=0;i<4;i++)
            {
                sta = M1_Read(block_num, tmpBuf);
                if(sta==Ok)
                {

					if(block_num==0&&Get_rfid_ReportAppState()==0)
						{	
							uint8_t TempData=1;
							Set_RFID_ReportAppState(1);
							OctoboProtocolSendPack(O2P_RFID_CMD,&TempData,1);							
						}
#if DEBUG==1
                    uart_puts("Block ");
                    uart_putHex(block_num);
                    uart_puts(" : ");
                    uart_printBuffer(tmpBuf,16);
                    uart_newrow();
#endif
                    block_num++;

                }
                else{
#if DEBUG==1
                    uart_puts("Block 0x");
                    uart_putHex(block_num);
                    uart_puts("Reader Error\r\n");
#endif
                    //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    //return sta;
                }
            }
        }
        else
        {
#if DEBUG==1
            uart_puts("Sector 0x");
            uart_putHex(sector_num);
            uart_puts("Authentication Error\r\n");
#endif
            //sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            //return sta;
        }



#if DEBUG==1
        uart_newrow();
#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);    // next Authentication
    }
#if 0
    /* read test Mifare 1K card (64--127 block) */
    //for(sector_num=16;sector_num<32;sector_num++)// read sector16--31  (block64--127 )
    for(block_num=64;block_num<128;)
    {
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
        if(Ok == sta)
        {
            for(uint8_t i=0;i<4;i++)    /* 4 blocks/sector */
            {
                sta = M1_Read(block_num, tmpBuf);
                if(sta==Ok)
                {
#if DEBUG==1
                    uart_puts("Block ");
                    uart_putHex(block_num);
                    uart_puts(" : ");
                    uart_printBuffer(tmpBuf,16);
                    uart_newrow();
#endif
                    block_num++;
                }
                else
                {
                    sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    return sta;

                }
            }
        }
        else{
#if DEBUG==1
            uart_puts("Block 0x");
            uart_putHex(block_num);
            uart_puts("Authentication Error\r\n");
#endif
            sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            return sta;
        }
#if DEBUG==1
        uart_newrow();
#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);
        /*
        piccHaltA();
        DelayMS(5);
        sta = piccWakeupA(tmpBuf);
        if(Ok == sta)
        {
            sta = piccSelectA(SEL1,UID, tmpBuf);
            if(Ok != sta){
#if DEBUG==1
                uart_puts("PICC Select Error!\r\n");
#endif
            }
        }
        else
        {
#if DEBUG==1
            uart_puts("PICC Request Error! @ ");
            uart_puts("Block 0x");
            uart_putHex(block_num);
#endif
        } */
    }

    /* read test Mifare 4K card (128--255 block) */
    //for(sector_num=32;sector_num<40;sector_num++)// read sector32--39  (block128--255 )
    for(block_num=128;block_num<255;)
    {
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
        if(Ok == sta)
        {
            for(uint8_t i=0;i<16;i++)       /* 16 blocks/sector */
            {
                sta = M1_Read(block_num, tmpBuf);
                if(sta==Ok)
                {
#if DEBUG==1
                    uart_puts("Block ");
                    uart_putHex(block_num);
                    uart_puts(" : ");
                    uart_printBuffer(tmpBuf,16);
                    uart_newrow();
#endif
                    if(block_num == 255)
                    {
                        sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                        return Ok;
                    }
                    else
                    {
                        block_num++;
                    }
                }
                else
                {
                    sky1311WriteReg(ADDR_M1_CTRL, 0x00);
                    return sta;
                }
            }
        }
        else{
#if DEBUG==1
            uart_puts("Block 0x");
            uart_putHex(block_num);
            uart_puts("Authentication Error\r\n");
#endif
            sky1311WriteReg(ADDR_M1_CTRL, 0x00);
            return Ok;
        }
#if DEBUG==1
        uart_newrow();
#endif
        sky1311WriteReg(ADDR_M1_CTRL, 0x81);
        /*
        piccHaltA();
        DelayMS(3);
        sta = piccWakeupA(tmpBuf);
        if(Ok == sta)
        {
            sta = piccSelectA(SEL1,UID, tmpBuf);
            if(Ok != sta){
#if DEBUG==1
                uart_puts("PICC Select Error!\r\n");
#endif
            }
        }
        else
        {
#if DEBUG==1
            uart_puts("PICC Request Error! @ ");
            uart_puts("Block 0x");
            uart_putHex(block_num);
#endif
        } */

    }
#endif
    //sky1311Reset();
    return Ok;
}

/***************************************************************
    M1��д�����ʾ��
    M1��д��Block4 (Sector 1) ��Block8��Sector2��������
   Ѱ���Ȳ���ʱ����ʾ�������
   �д���ʱ��ʾ������Ϣ
****************************************************************/
sta_result_t mifare1_WriteTest(uint8_t *UID)
{
    sta_result_t sta;
    uint8_t tmpBuf[20];
    uint8_t default_key[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t i;
    uint8_t block_num,block_array[3] = {2,4,8};
    uint8_t writeData[] = "ABCDEF1234567890";
    uint8_t keya[]={1,2,3,4,5,6};
    uint8_t keyb[]={6,5,4,3,2,1};
#if DEBUG==1
    uart_puts("\r\nWrite Mifare1 test\r\n");
#endif

    sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, 4);
    if(Ok==sta)
        sta = M1_PwdChange(4,keya,keyb);
    for(i=0;i<3;i++)
    {
        block_num = block_array[i];
        //sta = M1_Authentication(M1_AUTH_KEYA, default_key, UID, block_num);
        sta = M1_Authentication_Auto(M1_AUTH_KEYA, default_key, UID, block_num);
        if(Ok == sta)
        {
            sta = M1_Write(block_num, writeData);
#if DEBUG==1
            uart_puts("Block write ");
            uart_putHex(block_num);
#endif
            if(Ok == sta)
            {
#if DEBUG==1
                uart_puts("Success\r\n");
#endif
            }
            else{
#if DEBUG==1
                uart_puts("Failed\r\n");
#endif
            }
        }
        else{
#if DEBUG==1
            uart_puts("Block");
            uart_putHex(block_num);
            uart_puts(" Authentication Error\r\n");
#endif
        }
        sky1311WriteReg(ADDR_M1_CTRL, 0x00);
        piccHaltA();
        DelayMS(1);
        sta = piccRequestA(tmpBuf);
        if(Ok == sta)
        {
            sta = piccSelectA(SEL1,UID, tmpBuf);
            if(Ok != sta){
#if DEBUG==1
                uart_puts("PICC Select Error!\r\n");
#endif
            }
        }
        else
        {
#if DEBUG==1
            uart_puts("PICC Request Error!\r\n");
#endif
        }
    }
    //sky1311Reset();
    return Ok;
}

/***************************************************************
    CPU������ʾ��
   ��ȡCPU����UID�ȣ���ʹ�ò���APDU������
   Ѱ���Ȳ���ʱ����ʾ�������
   �д���ʱ��ʾ������Ϣ
****************************************************************/
sta_result_t smartTypeA_test(void)
{
    sta_result_t sta=Error;
    uint8_t tmpBuf[100];
    uint16_t tmpSize=0;
	//uint8_t i;
//    uint32_t DecNum=0;
//    uint8_t DecLength=0;
#if DEBUG==1
    uart_puts("\r\nSmart Type A test\r\n");
#endif
    sta = piccATS(tmpBuf);
    if(sta == NoResponse){
        sky1311Reset();
#if DEBUG==1
        uart_puts("\r\nRATS error!\r\n");
#endif
        return ErrorAts;
    }
    else{
#if DEBUG==1
    	uart_puts("RATS : ");
    	uart_printBuffer(tmpBuf,32);
        uart_newrow();
#endif
    }
#if 0
    /* 00 a4 04 00 00 */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x04;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x00;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 6);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Select SD:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
#endif
    }
#endif

     /* select MF file */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;   // command: select file
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x3F;
    tmpBuf[7] = 0x00;   // MF
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 8);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
		if(tmpSize<sizeof(tmpBuf))
			{
		        uart_puts("Select MF:");
		    	uart_printBuffer(tmpBuf,tmpSize-2);
		        uart_newrow();
			}	
#endif
    }
#if 0
    /*��ȡ�����*/
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0x84;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x08;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 6);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("RM:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
#endif
    }
#endif

#if 0
    /* ��һ��ͨ�йص���Ϣ */
    tmpBuf[0] = 0x03;
    tmpBuf[1] = 0x00;
    tmpBuf[2] = 0xA4;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x00;
    tmpBuf[5] = 0x02;
    tmpBuf[6] = 0x10;
    tmpBuf[7] = 0x01;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 8);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,8,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Read Card Info:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
        uart_puts("Card No: ");
        DecNum = (uint32_t) tmpBuf[40]<<24;
        DecNum += (uint32_t) tmpBuf[39]<<16;
        DecNum += (uint32_t) tmpBuf[38]<<8;
        DecNum += (uint32_t) tmpBuf[37];

        Dec2Str(DecNum,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_newrow();

#endif
    }
    /* ����� */
    tmpBuf[0] = 0x02;
    tmpBuf[1] = 0x80;
    tmpBuf[2] = 0x5C;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x02;
    tmpBuf[5] = 0x04;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 6);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1
        uart_puts("Read Balance:");
    	uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
        uart_puts("Balance: ��");
        DecNum = (uint32_t) (tmpBuf[1]&0x0f)<<24;
        DecNum += (uint32_t) tmpBuf[2]<<16;
        DecNum += (uint32_t) tmpBuf[3]<<8;
        DecNum += (uint32_t) tmpBuf[4];
        uint8_t decimal = DecNum%100;
        DecNum = DecNum/100;
        Dec2Str(DecNum,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_putChar('.');
        Dec2Str(decimal,tmpBuf,&DecLength);
        uart_sendData(tmpBuf,DecLength);
        uart_newrow();

#endif
    }

    for(i=1;i<=10;i++)
    {
        /* ��ȡ���׼�¼ */
        tmpBuf[0] = 0x02;
        tmpBuf[1] = 0x00;   // CLA Class
        tmpBuf[2] = 0xB2;   // INS Instruction
        tmpBuf[3] = i;//0x01;   // P1 Parameter 1
        tmpBuf[4] = 0xC4;//0xC5;   // P2 Parameter 2
        tmpBuf[5] = 0x00;   // Le
        //sky1311_fifo_tx(TYPE_A, tmpBuf, 6);
        //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
        sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,6,tmpBuf,&tmpSize);
        if(sta == NoResponse)
        {
            return NoResponse;
        }
        else{
    #if DEBUG==1
            uart_puts("Read Record:");
            uart_printBuffer(tmpBuf,tmpSize-2);
            uart_newrow();
    #endif
        }
    }
#endif //if 0
    sky1311Reset();
    //LED_OFF();  //GPIO_ResetBits(GPIOD, 1);   //LED off
    return Ok;
}
/******************************************************************************/
/*  Type A PICC test */
sta_result_t TypeA_test(void)
{
    sta_result_t sta;
    uint8_t SAK=0;
    uint8_t UID[10];
    uint8_t tmpBuf[10];

    SKY1311_ENABLE(); 	    // chip enable PD2
    sky1311Init();			// init sky1311s
    //resetPicc();            // ��λ��Ƭ()
    DelayMS(5);             // �ȴ���Ƭ����
    typeAOperate();
    sky1311WriteReg(ADDR_ANA_CFG3, 0x81);
    //sta = piccRequestA(tmpBuf);     // send REQA command to card
    sta = piccWakeupA(tmpBuf);        // send WUPA command to card
    if(Ok == sta)
    {
//        LED_ON();
#if DEBUG==1
        uart_puts("\r\nType-A Card Test\r\n");
        uart_puts("ATQA:");
        uart_printBuffer(tmpBuf,1);
        uart_newrow();

#endif
    }
    else
    {	
        sky1311Reset();
        return ErrorRequest;            // ���ﷵ�أ��п�����û������û����
    }
    sta = piccAntiA(SEL1, 0x01, UID);   // ����ײѭ�������Ի��һ�ſ���UID������0x0��ʾ������ͻʱѡ��1
    if(Ok == sta)
    {
        uint8_t checkSum = UID[0] ^ UID[1] ^ UID[2] ^ UID[3];
        if(checkSum != UID[4])
        {
            sky1311Reset();
            return ErrorAnticollision;
        }
#if DEBUG==1
        uart_puts("UID:");
        uart_printBuffer(UID,4);
        uart_newrow();
#endif
    }
    else
    {
        sky1311Reset();
#if DEBUG==1
        uart_puts("\r\nAnti error!\r\n");
#endif
        return ErrorAnticollision;
    }
    /*************������ѡ���Լ��Կ��Ĳ�������ֻ��UID�Ļ����˽���***********/
    sta = piccSelectA(SEL1,UID, tmpBuf);
    if(Ok == sta)
    {
        SAK = tmpBuf[0];
#if DEBUG==1
        uart_puts("SAK:");
        uart_printBuffer(tmpBuf,1);
#endif
    }
    else
    {
        sky1311Reset();
#if DEBUG==1
        uart_puts("\r\nSelect error!\r\n");
#endif
        return ErrorSelect;
    }

#if CARDTYPE==AUTO

    if((SAK & 0x24) == 0x20){           //UID complete, PICC compliant with ISO/IEC 14443-4
        sta = smartTypeA_test();
    }
    else if((SAK & 0x24) == 0x00){      // UID complete, PICC not compliant with ISO/IEC 14443-4
        //sta = mifare1_WriteTest(UID);
        sta = mifare1_ReadTest(UID);
    }
#elif CARDTYPE==ONLYM1
    sta = mifare1_ReadTest(UID);
#elif CARDTYPE==ONLYSMART
    sta = smartTypeA_test();
#elif CARDTYPE==SKY1311TEST
    sta = sky1311_ReadTest();
#endif
    sky1311Reset();
    return sta;
}

/***************************************************************
    TYPE B������ʾ��
   ��ȡTYPE B����UID�ȣ���ʹ�ò���APDU������
   Ѱ���Ȳ���ʱ����ʾ�������
   �д���ʱ��ʾ������Ϣ
****************************************************************/
uint8_t SmartTypeB_test(void)
{
    sta_result_t sta;

    nAQTB_t ATQB;
    uint8_t tmpBuf[100];
    uint16_t tmpSize;
    uint8_t  attSize;

    SKY1311_ENABLE(); 	    // chip enable PD2
    //DelayMS(10);
    sky1311Init();			// init sky1311s
    //resetPicc();            // ��λ��Ƭ
    DelayMS(5);             // �ȴ���Ƭ����
    typeBOperate();

    sta = piccRequestB(REQALL,0, 0, &ATQB);
    if(Ok != sta)
    {
        sky1311Reset();
        return NoResponse;
    }
#if DEBUG==1	
    uart_puts("\r\nType-B Card test\r\n");
#endif
//    LED_ON();
    sta = piccAttrib(ATQB.PUPI, 0, 8, 0, 1, tmpBuf, 0, tmpBuf, &attSize);
    if(Ok != sta)
    {
        sky1311Reset();
        return NoResponse;
    }

    /* �����֤ID  ISO14443-4 */
    tmpBuf[0] = 0x00;
    tmpBuf[1] = 0x36;
    tmpBuf[2] = 0x00;
    tmpBuf[3] = 0x00;
    tmpBuf[4] = 0x08;
    //sky1311_fifo_tx(TYPE_B, tmpBuf, 5);
    //sta = sky1311_fifo_rx(TYPE_B, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,5,tmpBuf,&tmpSize);
    if(sta == NoResponse)
    {
        sky1311Reset();
    	return NoResponse;
    }
    else{
#if DEBUG==1			
        uart_puts("ID Card:");
        uart_printBuffer(tmpBuf,tmpSize-2);
        uart_newrow();
#endif		
    }
    sky1311Reset();
    return 0;
}
