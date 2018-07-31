/******************************************************************************/
/*               (C) 斯凯瑞利（北京）科技有限公司(SKYRELAY)                     */
/*                                                                            */
/* 此代码是斯凯瑞利（北京）科技有限公司为支持客户编写的示例程序的一部分           */
/* 所有使用斯凯瑞利相关芯片的用户可以无偿使用该代码，但需保留本声明部分           */
/* 本代码仅为演示使用，不保证符合所有的应用要求，当有特定规范或需求时，           */
/* 使用者需自行确认代码是否符合要求，不合要求时自行做出相应的修改。               */
/* (V1.10)                                                                    */
/******************************************************************************/
/**
 ******************************************************************************
 ** 文 件 名： iso14443_3b.c
 **
 ** 文件简述： 符合ISO14443-3 type B的操作函数，常量和变量
 **
 ** 版本历史:
 ** 2015-10-05 V1.00  EH        第一个正式版本
 ** 2017-3-25  V.10  Release    更新代码和函数结构，重新发布
 ** 2018-06-12 V1.1.2 Release   修改部分错误，重新整理发布
 **
 ******************************************************************************/
#include "iso14443.h"
/**
 ******************************************************************************
 ** \简  述  type B 请求命令REQB, 返回值是 ATQB。
 **
 **
 ** \参  数  uint8_t ucReqCode:请求代码 ISO14443_3B_REQIDL 0x00 -- 空闲，就绪的卡
 **								  ISO14443_3B_REQALL 0x08 -- 空闲，就绪，退出的卡
 **			 uint8_t ucAFI ：应用标识符，0x00：全选
 **			 uint8_t N：时隙总数,取值范围0--4。
 **          nAQTB_t *pATQB 请求应答返回数据指针，12字节
 ** \返回值  操作状态，Ok：成功， 其它值：失败
 ******************************************************************************/
sta_result_t piccRequestB(uint8_t ucReqCode, uint8_t ucAFI, uint8_t N, nAQTB_t *pATQB)
{
    sta_result_t sta;
    uint8_t tmpBuf[16];
    uint16_t tmpSize;

    tmpBuf[0] = APF_CODE;         // APf = 0x05
    tmpBuf[1] = ucAFI;            // AFI, 00,选择所有PICC
    tmpBuf[2] = (ucReqCode & 0x08)|(N & 0x07);
    //sky1311_fifo_tx(TYPE_B, tmpBuf, 3);
    //sta = sky1311_fifo_rx(TYPE_B, false, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,3,tmpBuf,&tmpSize);

    if(sta == Ok){
        pATQB->PUPI[0] = tmpBuf[1];
        pATQB->PUPI[1] = tmpBuf[2];
        pATQB->PUPI[2] = tmpBuf[3];
        pATQB->PUPI[3] = tmpBuf[4];             // 4 Bytes PUPI
        pATQB->AppDat[0] = tmpBuf[5];
        pATQB->AppDat[1] = tmpBuf[6];
        pATQB->AppDat[2] = tmpBuf[7];
        pATQB->AppDat[3] = tmpBuf[8];           // 4 Bytes Application data
        pATQB->ProtInf[0] = tmpBuf[9];
        pATQB->ProtInf[1] = tmpBuf[10];
        pATQB->ProtInf[2] = tmpBuf[11];         // 3 bytes protocol info
    }
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  type B 时隙标记防碰撞 Slot-Marker，返回值是ATQB
 **
 ** \参  数  uint8_t 时隙数目1--15,与piccRequestB中的N参数有关
 **          nAQTB_t *pATQB 请求应答返回数据指针，12字节
 ** \返回值  状态，Ok：正确， 其它值：错误
 ******************************************************************************/
sta_result_t piccSlotMarker(uint8_t N, nAQTB_t *pATQB)
{
    sta_result_t sta;
    uint8_t tmpBuf[16];
    uint16_t tmpSize;
    if(N>16)
        return Error;
    N--;

    tmpBuf[0] = ( (N<<4)|APN_CODE );      // APn = xxxx0101b

    //sky1311_fifo_tx(TYPE_B, tmpBuf, 1);
    //sta = sky1311_fifo_rx(TYPE_B, false, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,1,tmpBuf,&tmpSize);

    if(sta == Ok){
        pATQB->PUPI[0] = tmpBuf[1];
        pATQB->PUPI[1] = tmpBuf[2];
        pATQB->PUPI[2] = tmpBuf[3];
        pATQB->PUPI[3] = tmpBuf[4];             // 4 Bytes PUPI
        pATQB->AppDat[0] = tmpBuf[5];
        pATQB->AppDat[1] = tmpBuf[6];
        pATQB->AppDat[2] = tmpBuf[7];
        pATQB->AppDat[3] = tmpBuf[8];           // 4 Bytes Application data
        pATQB->ProtInf[0] = tmpBuf[9];
        pATQB->ProtInf[1] = tmpBuf[10];
        pATQB->ProtInf[2] = tmpBuf[11];         // 3 bytes protocol info
    }
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  选择type B PICC
 **
 ** \参  数     uint8_t *pPUPI					    // 4字节PICC标识符
//				uint8_t ucDSI_DRI					// PCD<-->PICC 速率选择
//				uint8_t MAX_FSDI				    // PCD最大接收缓冲区大小
//				uint8_t ucCID						// 0 - 14,若不支持CID，则设置为0000
//				uint8_t ucProType					// 支持的协议，由请求回应中的ProtocolType指定
//				uint8_t *pHigherLayerINF			// 高层命令信息
//				uint8_t ucINFLen					// 高层命令字节数
// 出口参数:    uint8_t *pAATTRIB					// ATTRIB命令回应
//				uint8_t *pRLen					    // ATTRIB命令回应的字节数
 ** \返回值  状态 Ok成功， 其它失败
 ******************************************************************************/
sta_result_t piccAttrib(uint8_t *pPUPI, uint8_t ucDSI_DRI,uint8_t MAX_FSDI,
				 uint8_t ucCID, uint8_t ucProType, uint8_t *pHigherLayerINF, uint8_t ucINFLen,
				 uint8_t *pAATTRIB, uint8_t *pRLen)
{
    sta_result_t sta;
    uint8_t tmpBuf[32];
    uint16_t tmpSize;
    tmpBuf[0] = APC_CODE;     // 0x1D
    tmpBuf[1] = pPUPI[0];
    tmpBuf[2] = pPUPI[1];
    tmpBuf[3] = pPUPI[2];
    tmpBuf[4] = pPUPI[3];                             // 4 Bytes PUPI
    tmpBuf[5] = 0x00;                               // param 1
    tmpBuf[6] = ((ucDSI_DRI << 4) | (MAX_FSDI & 0x0F)) & 0xFF;// param 2, rate and length
    tmpBuf[7] =  ucProType & 0x0f;

    tmpBuf[8] = ucCID & 0x0f;
    if (ucINFLen>0)
	{
		//memcpy(&tmpBuf[9], pHigherLayerINF, ucINFLen);
        for(tmpSize=0;tmpSize<ucINFLen;tmpSize++)
            tmpBuf[9+tmpSize] = *pHigherLayerINF++;
	}
    //sky1311_fifo_tx(TYPE_B, tmpBuf, );
    //sta = sky1311_fifo_rx(TYPE_B, false, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,9+ucINFLen,tmpBuf,&tmpSize);
    if(sta == Ok){
        *pRLen = tmpSize;
        //memcpy(pAATTRIB, tmpBuf, tmpSize);
        while(tmpSize--)
        {
            pAATTRIB[tmpSize] = tmpBuf[tmpSize];
        }

    }
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  将指定的type B PICC操作挂起
 **
 ** \参  数  uint8_t* pPUPI // 4字节PICC标识符
 ** \返回值  状态
 ******************************************************************************/
sta_result_t piccHaltB(uint8_t *pPUPI)
{
    sta_result_t sta;
    uint8_t tmpBuf[5];
    uint16_t tmpSize;
    tmpBuf[0] = HALTB_CODE;      // 0x50;
    tmpBuf[1] = pPUPI[0];
    tmpBuf[2] = pPUPI[1];
    tmpBuf[3] = pPUPI[2];
    tmpBuf[4] = pPUPI[3];

    //sky1311_fifo_tx(TYPE_B, tmpBuf, 5);
	//sta = sky1311_fifo_rx(TYPE_B, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,5,tmpBuf,&tmpSize);
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  取消type B PICC选定
 **
 ** \参  数  uint8_t* 参数首地址
 ** \返回值  状态
 ******************************************************************************/
sta_result_t piccDeselectB(uint8_t *param)
{
    sta_result_t sta;
    uint8_t tmpBuf[5];
    uint16_t tmpSize;
    tmpBuf[0] = 0xCA;
    tmpBuf[1] = param[1];

    //sky1311_fifo_tx(TYPE_B, tmpBuf, 2);
	//sta = sky1311_fifo_rx(TYPE_B, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_B,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    return sta;
}
