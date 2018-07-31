/******************************************************************************/
/*               (C) 斯凯瑞利（北京）科技有限公司(SKYRELAY)                   */
/*                                                                            */
/* 此代码是斯凯瑞利（北京）科技有限公司为支持客户编写的示例程序的一部分       */
/* 所有使用斯凯瑞利相关芯片的用户可以无偿使用该代码，但需保留本声明部分       */
/* 本代码仅为演示使用，不保证符合所有的应用要求，当有特定规范或需求时，       */
/* 使用者需自行确认代码是否符合要求，不合要求时自行做出相应的修改。           */
/* (V1.10)                                                                    */
/******************************************************************************/
/**
 ******************************************************************************
 ** 文 件 名： sky1311s.h
 **
 ** 文件简述： 操作sky1311s的所有函数，常量和变量
 **
 ** 版本历史:
 ** 2015-10-05 V1.00  EH   第一个正式版本
 ** 2017-3-25  V.10  Release    更新代码和函数结构，重新发布
 ** 2018-06-12 V1.1.2 Release   增加数据收发函数，修改部分错误，重新整理发布
 ******************************************************************************/
#ifndef  __SKY1311S_H_
#define  __SKY1311S_H_
#include "sky1311t_reg.h"
#include "typedef.h"
#include "mcu.h"
/*
typedef enum sta_result
{
    Ok                      = 0,  ///< ??,????
    Error                   = 1,  ///< ?????
    Timeout                 = 3,  ///< ????
    RxParity			    = 4,  ///< ???????
    RxCheckCRC			    = 5,  ///< ??CRC???
    FifoFull                = 6,  ///< FIFO?
    FifoEmpty               = 7,  ///< FIFO?
    Collision               = 8,  ///< ?????
    Framing                 = 9,  ///< ?????
    UIDFormat               = 10, ///< UID????
    M1Error                 = 11, ///< ??M1???

    ErrorAddressAlignment       = 12,  ///< Address alignment does not match
    ErrorAccessRights           = 13,  ///< Wrong mode (e.g. user/system) mode is set
    ErrorInvalidParameter       = 14,  ///< Provided parameter is not valid
    ErrorOperationInProgress    = 15,  ///< A conflicting or requested operation is still in progress
    ErrorInvalidMode            = 16,  ///< Operation not allowed in current mode
    ErrorUninitialized          = 17,  ///< Module (or part of it) was not initialized properly
    ErrorBufferFull             = 18,  ///< Circular buffer can not be written because the buffer is full
    ErrorTimeout                = 19,  ///< Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.)
    ErrorNotReady               = 20, ///< A requested final state is not reached
    OperationInProgress         = 21,  ///< Indicator for operation in progress
		DisConnect                  = 22,

	UnknowError				= 0x7F,
    NoResponse              = 0xFF
}sta_result_t,en_result_t;
*/
/**
 ******************************************************************************
 ** \简述：全局预定义
 **
 ******************************************************************************/

/**
 ******************************************************************************
 ** \简述： 全局变量定义
 **
 ******************************************************************************/
//extern uint8_t MaxRCFreqVal;             // AD最大频率
//extern uint8_t bak_advalue;	            // 备份的AD值
//extern uint8_t curr_ad_val;                 // 当前AD值
//extern uint8_t maxADVal;
//extern uint8_t  tx_buf[];
//extern uint8_t  rx_buf[];
//extern uint16_t rx_len;
//extern uint8_t  err_sta;
extern uint8_t  irq_sta;

/**
 ******************************************************************************
 ** \简述： 函数声明
 **
 ******************************************************************************/
void SetBitMask(uint8_t regAddr, uint8_t mask);
void ClearBitMask(uint8_t regAddr, uint8_t mask);
void analogInit(void);
void directModeInit(uint8_t mfout_sel);
void pcdAntennaOn(uint8_t pcdType);
void pcdAntennaOff(void);
void irqClearAll(void);
void irqClear(uint8_t irq);
void bitRateSet(uint16_t high_rate);
void typeAOperate(void);
void typeBOperate(void);
void sky1311Init(void);
void sky1311Reset(void);
void resetPicc(void);

sta_result_t bitCollisionTrans(uint8_t* txBuf, uint8_t txLen, uint8_t lastBitNum);
void sky1311_fifo_tx(uint8_t txType, uint8_t *txBuff, uint16_t txSize);
sta_result_t sky1311_fifo_rx(uint8_t rxType, uint8_t rateType, uint8_t *rxBuff, uint16_t *rxSize);
sta_result_t ExchangeData(uint8_t cardType, uint8_t rateType,
                    uint8_t *pSendDataBuf, uint16_t ucSendLen,
                    uint8_t *pRcvDataBuf,  uint16_t *pRcvLen);
uint16_t sky1311RCFreqCali(void);
uint8_t checkCardNear(uint8_t MaxRCFreqVal);
uint8_t checkCardNearXtal(void);
void checkCardInit(uint16_t rc_val);
void checkCardDeinit(void);
void sky1311CardRemove(uint16_t rc_val);
void checkCardRemoveConfig(uint16_t rc_val);
boolean checkCardRemove(uint8_t maxADCVal);
uint8_t getCheckCardAdcValue(uint8_t rcVal);


#endif
