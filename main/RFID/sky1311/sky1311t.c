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
 ** 文 件 名： sky1311s.c
 **
 ** 文件简述： 操作sky1311s的所有函数，常量和变量
 **
 ** 版本历史:
 ** 2015-10-05 V1.00  EH   第一个正式版本
 ** 2017-3-25  V.10  Release    更新代码和函数结构，重新发布
 ** 2018-06-12 V1.1.2 Release   增加数据收发函数，修改部分错误，重新整理发布
 ******************************************************************************/
#include "mcu.h"
#include "sky1311_drv.h"
#include "sky1311t_reg.h"
#include "typedef.h"
#include "userdef.h"
/**
 ******************************************************************************
 ** \简述： 全局变量定义
 **
 ******************************************************************************/
#if CHECKCARD==1
    uint8_t maxRCADVal=MAXADVAL;         // 检测到的最大AD值
    boolean freqScanNeed=true;  // 系统需要RC频率扫描
#endif

uint8_t  err_sta;   // 错误状态代码
uint8_t  irq_sta;   // 中断请求状态代码

/**
 ******************************************************************************
 ** \简  述  设置寄存器掩码位
 **
 ** \参  数  uint8_t regAddr 寄存器地址， uint8_t mask 要设置的"掩码"字节
 ** \返回值  none
 ******************************************************************************/
void SetBitMask(uint8_t regAddr, uint8_t mask)
{
    uint8_t tmp;
    tmp = sky1311ReadReg(regAddr);
    sky1311WriteReg(regAddr, tmp|mask);
}
/**
 ******************************************************************************
 ** \简  述  清除寄存器掩码位
 **
 ** \参  数  uint8_t regAddr 寄存器地址， uint8_t mask 要清除的"掩码"字节
 ** \返回值  none
 ******************************************************************************/
void ClearBitMask(uint8_t regAddr, uint8_t mask)
{
    uint8_t tmp;
    tmp = sky1311ReadReg(regAddr);
    sky1311WriteReg(regAddr, tmp & ~mask);
}

/**
 ******************************************************************************
 ** \简  述  模拟参数初始化，配置模拟参数寄存器
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
static void analogInit(void)
{
    sky1311WriteReg(ADDR_ANA_CFG0, PA_2P5V);    // 7-4: RC OSC Freq
                                             // 3-2: PA Driver
                                             //   1: ixtal
                                             //   0: External LDO
    sky1311WriteReg(ADDR_ANA_CFG1, 0xEC);    //   7: txen
                                             //   6: rxen
                                             // 5-4: 00(osc off, xtal off) 01(osc on, xtal off) 10(osc off, xtal 13.56) 11(osc off, xtal 27.12)
    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
}
/**
 ******************************************************************************
 ** \简  述  输出调试参数初始化
 **
 ** \参  数  选择通过MFOUT脚输出的信号，
 **          bit2,1,0
 **             0 0 0 rx phase signal after demod, before sample
 **             0 0 1 tx bit stream
 **             0 1 0 rx bit stream
 **             0 1 1 rx data from analog
 **             1 0 0 DO from digital
 ** \返回值  none
 ******************************************************************************/
/*static void directModeInit(uint8_t mfout_sel)
{
    sky1311WriteReg(ADDR_MFOUT_SEL,  mfout_sel);
}*/

/**
 ******************************************************************************
 ** \简  述  打开PCD天线
 **
 ** \参  数  PCD类型， TYPE_A, TYPE_B
 ** \返回值  none
 ******************************************************************************/
void pcdAntennaOn(uint8_t pcdType)
{
    if(pcdType == TYPE_A){
        sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
    }else if(pcdType == TYPE_B){
        sky1311WriteReg(ADDR_ANA_CFG2, ANA2_B);
    }
    SetBitMask(ADDR_TX_CTRL, TX_POLE);
}

/**
 ******************************************************************************
 ** \简  述  关闭PCD天线
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void pcdAntennaOff(void)
{
    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);
    ClearBitMask(ADDR_TX_CTRL, TX_POLE);
}
/**
 ******************************************************************************
 ** \简  述  清除所有中断标记
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void irqClearAll(void)
{
    sky1311WriteReg(ADDR_IRQ_STA, 0xFF);
}
/**
 ******************************************************************************
 ** \简  述  清除指定的中断标记
 **
 ** \参  数  uint8_t irq
 ** \返回值  none
 ******************************************************************************/
void irqClear(uint8_t irq)
{
    sky1311WriteReg(ADDR_IRQ_STA,irq);
}
/**
 ******************************************************************************
 ** \简  述  设置sky1311与卡通信的倍速, ISO14443-4部分规定，多倍速的PICC
 **
 ** \参  数  设置的倍速，106，212或424
 ** \返回值  none
 ******************************************************************************/
void bitRateSet(uint16_t high_rate)
{
    sky1311WriteReg(ADDR_FSM_STATE, RATE_SEL);                           // enable high-speed mode
    if(high_rate == 212){
        sky1311WriteReg(ADDR_TX_CTRL, TX_212|TX_POLE_HIGH|TX_CRC_EN|TX_PARITY_ODD);
        sky1311WriteReg(ADDR_RATE_CTRL, RX_RATE_WID_2|RX_RATE_212|RX_RATE_PAR_ODD|RX_RATE_CRC_EN|RX_RATE_PAR_EN);
        sky1311WriteReg(ADDR_TX_PUL_WID,0x14);                           // tx pulse width = 1.5u
        sky1311WriteReg(ADDR_RATE_FRAME_END,0x40);                       // stop counter = 0x40
        sky1311WriteReg(ADDR_RATE_THRES,0x20);                           // threshold = 0x20
    }else if(high_rate == 424){
        sky1311WriteReg(ADDR_TX_CTRL, TX_424|TX_POLE_HIGH|TX_CRC_EN|TX_PARITY_ODD);
        sky1311WriteReg(ADDR_RATE_CTRL, RX_RATE_WID_2|RX_RATE_424|RX_RATE_PAR_ODD|RX_RATE_CRC_EN|RX_RATE_PAR_EN);
        sky1311WriteReg(ADDR_TX_PUL_WID,0x0A);                           // tx pulse width = 0.8u
        sky1311WriteReg(ADDR_RATE_FRAME_END,0x20);                       // stop counter = 0x20
        sky1311WriteReg(ADDR_RATE_THRES,0x10);                           // threshold = 0x10
    }
}
/**
 ******************************************************************************
 ** \简  述  选择type A 卡作为操作对象
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void typeAOperate(void)
{
    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_A);             // analogA select
    sky1311WriteReg(ADDR_FSM_STATE, TYPE_A_SEL);        // typeA select
    sky1311WriteReg(ADDR_TX_PUL_WID,0x26);              // set to default value
    sky1311WriteReg(ADDR_CRC_CTRL, CRC_A);              // crcA enable
    sky1311WriteReg(ADDR_M1_CTRL, 0x00);                // disable M1 operation
    sky1311WriteReg(ADDR_ANA_CFG3, 0x81);
}
/**
 ******************************************************************************
 ** \简  述  选择type B 卡作为操作对象
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void typeBOperate(void)
{
    sky1311WriteReg(ADDR_ANA_CFG2, ANA2_B);            // analogB select
    sky1311WriteReg(ADDR_FSM_STATE, TYPE_B_SEL);       // typeB select
    sky1311WriteReg(ADDR_CRC_CTRL, CRC_B);             // crcB enable

    sky1311WriteReg(ADDR_TX_PUL_WID,0x26);              // set to default value
    sky1311WriteReg(ADDR_M1_CTRL, 0x00);                // disable M1 operation
    sky1311WriteReg(ADDR_ANA_CFG3, 0x81);
}
/**
 ******************************************************************************
 ** \简  述  初始化SKY1311S
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void sky1311Init(void)
{
    //sky1311WriteCmd(CMD_SW_RST);                // reset all register
    analogInit();
    DelayMS(3);  //Crystal Stable time
    //directModeInit(MFOUT_ANALOG_RX);
    sky1311WriteReg(ADDR_TIME_OUT2, 0x8F);       // time_out timer stop condition = beginning of RX SOF
    sky1311WriteReg(ADDR_TIME_OUT1, 0xFF);
    sky1311WriteReg(ADDR_TIME_OUT0, 0xFF);
    //sky1311WriteReg(ADDR_RX_PUL_DETA, 0x44);
    sky1311WriteReg(ADDR_RX_PUL_DETA, 0x64);     // 高4位识别曼彻斯特码脉宽，越大容错能力越强
    //sky1311WriteReg(ADDR_RX_PRE_PROC, 0x26);
    sky1311WriteReg(ADDR_IRQ_EN, IRQ_M1_EN|IRQ_TOUT_EN|IRQ_TX_EN|IRQ_RX_EN|IRQ_HIGH_EN|IRQ_LOW_EN);    // enable all IRQ
}
/**
 ******************************************************************************
 ** \简  述  SKY1311S复位，并处于disable状态
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void sky1311Reset(void)
{
    ClearBitMask(ADDR_ANA_CFG1,TX_EN);  // 关场
    sky1311WriteCmd(CMD_SW_RST);        // 状态复位
    SKY1311_DISABLE();                  // 芯片关闭
}

/**
 ******************************************************************************
 ** \简  述  复位PICC，关闭场5ms,再打开
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void resetPicc(void)
{
    ClearBitMask(ADDR_ANA_CFG1,TX_EN);
    DelayMS(5);
    SetBitMask(ADDR_ANA_CFG1,TX_EN);
    DelayMS(10);

}
/**
 ******************************************************************************
 ** \简  述  数据发射函数，将数据写到FIFO中并通过射频接口发送给PICC
 **
 ** \参  数   uint8_t txType,        // 发射类型（TYPE_A | TYPE_B）
 **           uint8_t *txBuff,      // 数据内容
 **           uint16_t txSize       // 数据大小
 ** \返回值  none
 ******************************************************************************/
void sky1311_fifo_tx(uint8_t txType, uint8_t *txBuff, uint16_t txSize)
{
    uint16_t tx_res = txSize;
    //uint8_t regval;
    volatile uint16_t delayCount;
    sky1311WriteCmd(CMD_IDLE);              // reset state machine to Idle mode
    sky1311WriteCmd(CMD_CLR_FF);            // clear FIFO
    irqClearAll();                          // clear all IRQ state

    //sky1311WriteReg(ADDR_FIFO_CTRL,8);      // set water-level of FIFO
    /* write numbers */
    if(txType == TYPE_A){
        sky1311WriteReg(ADDR_TX_BYTE_NUM, txSize & 0x00ff);
        if(txSize > 255)
            sky1311WriteReg(ADDR_TX_BIT_NUM, ((txSize & 0x0f00)>>4|8));// write the length to tx_byte register
    }else if(txType == TYPE_B){
        sky1311WriteReg(ADDR_TX_B_BYTE_NUM, txSize & 0x00ff);
        if(txSize > 255)
            sky1311WriteReg(ADDR_TX_B_EGT_NUM, (txSize & 0x0300)>>4);
    }
    /* when TX length<=FIFO's depth, write all data to FIFO */
    if( txSize <= 64){
        sky1311WriteFifo(txBuff, txSize);
        sky1311WriteCmd(CMD_TX_RX);             // transceive & into receive mode
    }
    /* when TX length > FIFO's depth */
    else{               // txLen > 64
        sky1311WriteFifo(txBuff, 64);       // send 64 bytes
        sky1311WriteCmd(CMD_TX_RX);
        tx_res = txSize - 64;
        while(tx_res>0){                    // send remain bytes
            if(0!= (irq_sta = sky1311ReadReg(ADDR_IRQ_STA))){
                //irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
                if(irq_sta & IRQ_LOW){  // FIFO low
                    if(tx_res>=56){
                        sky1311WriteFifo(&txBuff[txSize - tx_res], 56);
                        tx_res -=56;
                    }
                    else{
                        sky1311WriteFifo(&txBuff[txSize - tx_res], tx_res);
                        tx_res = 0;
                    }
                    //irqClearAll();
                    irqClear(irq_sta);
                }
                else        // the other IRQ
                    //return;
                    irqClear(irq_sta);
            }
        }
    }
    /* wait TX finished */
    while(1){
        delayCount = 0xFF;
        while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);   // waiting for TX STOP IRQ
        if((delayCount==0)||(irq_sta & IRQ_TOUT)){             // tiemout
            sky1311WriteCmd(CMD_IDLE);
            irqClearAll();
            return;
        }
        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
        if(irq_sta & IRQ_TX){
            irqClear(IRQ_TX);
            return;
        }
        else{
            irqClear(irq_sta);  //irqClearAll();
        }
    }
}
/**
 ******************************************************************************
 ** \简  述  数据接收函数，将FIFO中数据读出来
 **
 ** \参  数   uint8_t txType,        // 发射类型（TYPE_A | TYPE_B）
              uint8_t rateType,      // 是否多倍速
 **           uint8_t *rxBuff,      // 数据内容
 **           uint16_t rxSize       // 数据大小
 ** \返回值  Ok,正确接收到数据 ; 其它，产生错误
 ******************************************************************************/
sta_result_t sky1311_fifo_rx(uint8_t rxType, uint8_t rateType, uint8_t *rxBuff, uint16_t *rxSize)
{
    uint16_t rx_buf_cnt=0;
    uint8_t  byte_num_H;
    uint8_t  byte_num_L;
    uint8_t  temp_len;
    uint8_t  bit_n=0;

    volatile uint16_t delayCount;

    while(1){
        delayCount=0xFF;                        // delay
        while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);

        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
        err_sta = sky1311ReadReg(ADDR_ERR_STA);

        if((delayCount==0)||(irq_sta & IRQ_TOUT)){             // tiemout
            sky1311WriteCmd(CMD_IDLE);
            irqClearAll();
            return NoResponse;
        }
        else if(irq_sta & IRQ_HIGH){        // FIFO High
            sky1311ReadFifo(&rxBuff[rx_buf_cnt], 56);   // load next 56 bytes into FIFO
            rx_buf_cnt += 56;

            irqClear(IRQ_HIGH);
        }
        else if(irq_sta & IRQ_RX){          // Received
            if( ((sky1311ReadReg(ADDR_FIFO_LEN))<1) || (err_sta & 0xC0) ){
                sky1311WriteCmd(CMD_CLR_FF);           // noise occur, restart the rx
                sky1311WriteCmd(CMD_RX);
                irqClear(IRQ_RX);  //irqClearAll();
                return Error;
            }
            else{
                irqClearAll();
                if(err_sta & 0xc0)
                    return Error;
                temp_len = sky1311ReadReg(ADDR_FIFO_LEN);               // get FIFO length
                sky1311ReadFifo(&rxBuff[rx_buf_cnt], temp_len);        // get data ,FIFO-->rx_buf
                rx_buf_cnt += temp_len;

                if(rxType == TYPE_A){
                    if(rateType){
                        byte_num_H = sky1311ReadReg(ADDR_RATE_RX_BIT);
                        byte_num_L = sky1311ReadReg(ADDR_RATE_RX_BYTE);
                    }else{
                        byte_num_H = sky1311ReadReg(ADDR_RX_NUM_H);
                        byte_num_L = sky1311ReadReg(ADDR_RX_NUM_L);
                        bit_n = ((sky1311ReadReg(ADDR_RX_NUM_H)) & 0xf0)>>4;
                        if(bit_n) sky1311ReadFifo(&rxBuff[rx_buf_cnt], 1);
                    }
                        *rxSize  = ((byte_num_H & 0x01) << 8) | byte_num_L;

                        if(bit_n)
                        {
                            *rxSize = ((*rxSize) + 1);
                        }
                }
                else if(rxType == TYPE_B){
                    byte_num_H = sky1311ReadReg(ADDR_RX_B_CTRL);
                    byte_num_L = sky1311ReadReg(ADDR_RX_B_BYTE_NUM);
                    *rxSize  = ( (byte_num_H & 0x80) << 1 ) | byte_num_L;
                }

                return Ok;
            }
        }
        else if(irq_sta & IRQ_LOW){    //清除IRQ_LOW中断
          irqClear(IRQ_LOW);
        }
        else{
            irqClearAll();
            //return NoResponse;
        }
    }
}
/**
 ******************************************************************************
 ** \简  述  发送数据后立即转入接收数据
 **          要发送到数据保存在变量pSendDataBuf中（发送缓冲区）
 **          接收的数据保存在变量pRcvDataBuf，接收到的数量保存在变量pRcvLen中
 ** \参  数  uint8_t cardType, 卡类型 （TYPE_A | TYPE_B）
 **          uint8_t rateType, 是否支持多倍速，0不支持，1支持
 **          uint8_t *pSendDataBuf, 发送缓冲区
 **          uint8_t ucSendLen 发送数据的长度，
 **          uint8_t *pRcvDataBuf, 接收数据缓冲区
 **          uint16_t *pRcvLen ， 接收数据长度
 **
 ** \返回值  Ok: 接收到返回的数据， 其它：没有收到返回的数据
 ******************************************************************************/
sta_result_t ExchangeData(uint8_t cardType, uint8_t rateType,
                    uint8_t *pSendDataBuf, uint16_t ucSendLen,
                    uint8_t *pRcvDataBuf,  uint16_t *pRcvLen)
{
    sta_result_t sta;
    sky1311_fifo_tx( cardType, pSendDataBuf, ucSendLen);
    sta = sky1311_fifo_rx(cardType, rateType, pRcvDataBuf, pRcvLen);
    return sta;
}

/**
 ******************************************************************************
 ** \简  述  Type A 防冲突数据发送函数，将数据写到FIFO中并通过射频接口发送给PICC
 **          发送的数据保存在全局变量tx_buf中，发送缓冲区
 **
 ** \参  数  uint8_t* txBuf           数据缓冲区
 **          uint8_t txLen          发射的数据长度（包括不完整的部分）,
 **          uint8_t lastBitNum     最后一个字节有效位数
 ** \返回值  Ok--成功发射，Timeout--超时, Error -- 其它错误
 ******************************************************************************/
sta_result_t bitCollisionTrans(uint8_t* txBuf, uint8_t txLen, uint8_t lastBitNum)
{
    volatile uint16_t delayCount;
    if(txLen>7)          // The maximum length of for transmission from PCD to PICC shall be 55 data bits
        return Error;
    sky1311WriteCmd(CMD_IDLE);              // reset state machine to Idle mode
    sky1311WriteCmd(CMD_CLR_FF);            // clear FIFO
    irqClearAll();                          // clear all IRQ state
    /* write numbers */
    sky1311WriteReg(ADDR_TX_BYTE_NUM, txLen);
    sky1311WriteReg(ADDR_TX_BIT_NUM,lastBitNum);
    sky1311WriteFifo(txBuf, txLen);         // write data to FIFO
    sky1311WriteCmd(CMD_TX_RX);             // transceive & into receive mode
    /* wait TX finished */
    while(1){
        delayCount = 0xFF;                // timeout count
        while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);   // waiting for TX STOP IRQ
        irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
        if((delayCount==0)||(irq_sta & IRQ_TOUT)){
            sky1311WriteCmd(CMD_IDLE);
            irqClearAll();
            return Timeout;
        }
        else if(irq_sta & IRQ_TX){
            irqClear(IRQ_TX);
            break;
        }
        else{
            irqClearAll();
        }
    }
    /* 等待接收数据完成 */
    delayCount = 0xFF;                            // timeout count
    while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);     // waiting for RX STOP IRQ

    irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
    if((delayCount==0)||(irq_sta & IRQ_TOUT)){
        sky1311WriteCmd(CMD_IDLE);
        irqClearAll();
        return Timeout;
    }else if(!(irq_sta & IRQ_RX)){
        irqClearAll();
        return Error;
    }
    irqClearAll();
    return Ok;
}


/**
 ******************************************************************************
 ** \简  述  扫描最优的RC频率，为低功耗询卡用，
 **          注意：需要在读卡器场内没有任何导电物质遮挡时扫描
 **
 ** \参  数  none
 ** \返回值  低8位采集到的最大AD值，
         bit8--bit11 表示幅值最大时的频率值，对应Analog0寄存器的bit7--bit4
   		 bit12 表示是否扫描到最大值，1--扫描到，0--没有。
   		 bit13--15 没有使用，设置为0
 ******************************************************************************/
uint16_t sky1311RCFreqCali(void)
{
    uint8_t currAdcVal=0;       // 当前AD值
    uint8_t maxAdcVal=0;        // 最大AD值
    uint8_t currRCFreqVal=0;     // 当前频率值
    uint8_t MaxRCFreqVal = 0;    // 最大的频率值
    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C); //OSC Enable
    sky1311WriteReg(ADDR_ANA_CFG2, 0xA0);
	for(currRCFreqVal=0;currRCFreqVal<16;currRCFreqVal++)   // from 0000 to 1111 scan
    {
        sky1311WriteReg(ADDR_ANA_CFG0, (currRCFreqVal << 4) | PA_2P0V);

        sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x40);   // pull up RSSI_EN
        delay(30);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x60);   // pull up TX_EN
        delay(10);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x70);   // pull up ADC_EN
        delay(10);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x50);   // pull low TX_EN
        delay(20);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x00);   // pull low ADC_EN, RSSI_EN
        delay(10);
        /*
        sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
        delay(10);
        sky1311WriteReg(ADDR_ANA_CFG4, 0x80);     // AD转换 Trigger
        delay(100);         // 延时约20微秒等待AD转换结束
        */
        currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5); // read ADC value from analog5 register
        //sky1311WriteReg(ADDR_ANA_CFG4, 0x00);   // Disable B6: RSSI_EN B5: TX_EN B4: ADC_EN
        if(currAdcVal > maxAdcVal){			// 当前频率的AD值若大于最大值
            maxAdcVal = currAdcVal; 			// 用当前值取代最大值
            MaxRCFreqVal = currRCFreqVal;   	// 记下当前频率值
        }
        //delay(50);
	}
    if(maxAdcVal>MINADVAL){
        maxRCADVal = maxAdcVal;
#if DEBUG==1
        NEW_ROW();
        PUTS("--RC frequency calibate, ");
        PUTS("RC Param = ");
        PRINT(MaxRCFreqVal);
        PUTS("    AD Value=");
        PRINT(maxRCADVal);
        NEW_ROW();
#endif
        return ((0x10 | MaxRCFreqVal) << 8 | maxAdcVal);		// 返回成功标记和频率值
    }
    else{
        maxRCADVal = MAXADVAL;
#if DEBUG==1
        NEW_ROW();
        PUTS("--RC frequency calibrate: Error!");
        NEW_ROW();
#endif
        return 0;
    }
}

/**
 ******************************************************************************
 ** \简  述  获得低功耗检卡时的AD值
 **
 ** \参  数  uint8_t RC频率设定值
 ** \返回值  uint8_t 无卡时的AD值
 ******************************************************************************/
/*
uint8_t getCheckCardAdcValue(uint8_t rcVal)
{
    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG0, rcVal | PA_2P0V);
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C);
    sky1311WriteReg(ADDR_ANA_CFG2, 0x10);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
    sky1311WriteReg(ADDR_ANA_CFG6, WKU_EN | WKU_OR | WKU_100MS | RSSI_2AVG);
    sky1311WriteReg(ADDR_ANA_CFG7, ADC_SAMPLE_5US | TX_SETTLE_0US | 0x0F);
    sky1311WriteReg(ADDR_ANA_CFG8, 0);
    SKY1311_DISABLE();
    DelayMS(210);
    return(sky1311ReadReg(ADDR_ANA_CFG5));
}
*/

/**
 ******************************************************************************
 ** \简  述  检查是否有卡进入或者离开场(低功耗询卡)
 **
 ** \参  数  uint8_t RC频率设定值
 ** \返回值  没有动作，0  (设置低功耗询卡标记)
 **          有卡进入，1  (设置读卡标记)
 **          有卡离开，2  (设置重新校准查询卡频率标记)
 ******************************************************************************/
void checkCardInit(uint16_t rc_val)
{
    uint8_t MaxRCFreqVal, MaxADCVal;

    MaxRCFreqVal = (uint8_t)((rc_val >> 4) & 0xF0); // RC频率校准值

    MaxADCVal = (uint8_t)((rc_val & 0xFF));         // 对应的最大AD值
    /* RC校准时获得的AD值可能比实际自动检卡时的AD值偏大，
    如果这样可以将ＭaxADCVal值减去一定的值，减去的值根据实际情况确定 */
    //MaxADCVal -= 10;

    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG0, MaxRCFreqVal | PA_2P0V);
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C);
    sky1311WriteReg(ADDR_ANA_CFG2, 0x10);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
    sky1311WriteReg(ADDR_ANA_CFG6, WKU_EN | WKU_OR | WKU_300MS | RSSI_4AVG);
    sky1311WriteReg(ADDR_ANA_CFG7, ADC_SAMPLE_5US | TX_SETTLE_0US | RSSI_DELTA);
    sky1311WriteReg(ADDR_ANA_CFG8, MaxADCVal-RSSI_ABS);
    SKY1311_DISABLE();
}

void checkCardDeinit(void)
{
    SKY1311_ENABLE();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG6, 0x22);
    sky1311WriteReg(ADDR_ANA_CFG7, 0x3F);
    sky1311WriteReg(ADDR_ANA_CFG8, 0xFF);
    SKY1311_DISABLE();
}

/**
 ******************************************************************************
 ** \简  述  检查是否有卡离开场配置
 **
 ** \参  数  uint16_t RC频率扫描返回值
 ** \返回值  无
 ******************************************************************************/
void checkCardRemoveConfig(uint16_t rc_val)
{

    uint8_t MaxRCFreqVal;
    MaxRCFreqVal = (uint8_t)((rc_val >> 4) & 0xF0);
    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG0, MaxRCFreqVal | PA_2P0V);
    sky1311WriteReg(ADDR_ANA_CFG1, 0x1C);
    sky1311WriteReg(ADDR_ANA_CFG2, 0xA0);
    sky1311Disable();

}
/**
 ******************************************************************************
 ** \简  述  检查是否有卡离开场
 **
 ** \参  数  uint8_t RC频率扫描得到的最大AD值
 ** \返回值  true,卡已经离开； false， 卡没有离开
 ******************************************************************************/
boolean checkCardRemove(uint8_t maxAdcVal)
{
    uint8_t currAdcVal;       // 当前AD值
    sky1311Enable();
    DelayMS(5);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
    delay(10);
    sky1311WriteReg(ADDR_ANA_CFG4, 0x80);
    delay(100);
    currAdcVal = sky1311ReadReg(ADDR_ANA_CFG5);
#if DEBUG==1
        PUTS("currAdcVal=");
        PRINT(currAdcVal);
        uart_putChar('\r');
#endif
    sky1311WriteReg(ADDR_ANA_CFG4, 0x00);
    sky1311Disable();
    if(currAdcVal > (maxAdcVal - RSSI_ABS))
    {
        return true;
    }
    else
        return false;

}
