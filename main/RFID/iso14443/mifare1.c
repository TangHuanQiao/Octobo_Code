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
 ** 文 件 名： mifare1.c
 **
 ** 文件简述： 符合Mifare卡操作的所有函数，常量和变量
 **
 ** 版本历史:
 ** 2015-10-05 V1.00  EH   第一个正式版本
 ** 2017-03-25 V1.10  Release    更新代码和函数结构，重新发布
 ** 2018-06-12 V1.1.2 Release   修改部分错误，重新整理发布
 **
 ******************************************************************************/
#include "iso14443.h"

void sky1311RxCrcOn(void);
void sky1311RxCrcOff(void);
void sky1311TxCrcOn(void);
void sky1311TxCrcOff(void);

/**
 ******************************************************************************
 ** \简  述  操作M1卡时打开接收的CRC校验(sky1311s)
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void sky1311RxCrcOn(void)
{
    sky1311WriteReg(ADDR_RX_CTRL, RX_MIFARE_ON|RX_CRC_EN|RX_PARITY_EN|RX_PARITY_ODD);
}
/**
 ******************************************************************************
 ** \简  述  操作M1卡时关闭接收的CRC校验(sky1311s)
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void sky1311RxCrcOff(void)
{
    sky1311WriteReg(ADDR_RX_CTRL, RX_MIFARE_ON|RX_PARITY_EN|RX_PARITY_ODD);
}
/**
 ******************************************************************************
 ** \简  述  操作M1卡时打开发送的CRC校验(sky1311s)
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void sky1311TxCrcOn(void)
{
    sky1311WriteReg(ADDR_TX_CTRL, TX_CRC_EN|TX_PARITY_ODD|TX_POLE_HIGH);
}
/**
 ******************************************************************************
 ** \简  述  操作M1卡时关闭发送的CRC校验(sky1311s)
 **
 ** \参  数  none
 ** \返回值  none
 ******************************************************************************/
void sky1311TxCrcOff(void)
{
    sky1311WriteReg(ADDR_TX_CTRL, (!TX_CRC_EN)|TX_PARITY_ODD|TX_POLE_HIGH);
}

/**
 ******************************************************************************
 ** \简  述  M1卡的加密验证,
 **
 ** \参  数  uint8_t auth_mode 验证的密钥（Key_A 或者 Key B)
 **          uint8_t* m1Key 密钥
             uint8_t* UID 卡的UID号
             uint8_t blockAddr 要操作的扇区地址（0--63）
 ** \返回值  验证状态
 ******************************************************************************/
sta_result_t M1_Authentication_Auto(uint8_t auth_mode,uint8_t *m1Key,uint8_t *uid, uint8_t blockAddr)
{
    //uint8_t irq_sta;
    volatile uint16_t delayCount;

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    // load M1_KEY ( 低字节在前，先写入)
    sky1311WriteReg(ADDR_M1_KEY, m1Key[5]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[4]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[3]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[2]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[1]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[0]);

    // load UID （ 低字节在前，先写入）
    sky1311WriteReg(ADDR_M1_ID, uid[3]);
    sky1311WriteReg(ADDR_M1_ID, uid[2]);
    sky1311WriteReg(ADDR_M1_ID, uid[1]);
    sky1311WriteReg(ADDR_M1_ID, uid[0]);

    // set auth mode & begin
    sky1311WriteReg(ADDR_TX_BYTE_NUM, 2);
    sky1311WriteReg(ADDR_TX_BIT_NUM, 8);
    sky1311WriteReg(ADDR_FIFO, auth_mode);
    sky1311WriteReg(ADDR_FIFO, blockAddr);
    irqClearAll();
    sky1311WriteCmd(CMD_AUTO_M1);

    // wait PICC return result
    delayCount = 0x7FFF;
    while((0==(irq_sta=sky1311ReadReg(ADDR_IRQ_STA))) && --delayCount);
    irq_sta = sky1311ReadReg(ADDR_IRQ_STA);
    if(irq_sta & IRQ_M1)
    {
        irqClearAll();//irqClear(IRQ_M1);
        if(sky1311ReadReg(ADDR_M1_SUC_STATE) & 0x04)
            return M1Error;
        if(sky1311ReadReg(ADDR_M1_SUC_STATE) & 0x08)
            return Ok;
    }
    else if((delayCount==0) || (irq_sta & IRQ_TOUT))
    {
        irqClear(IRQ_TOUT);
        sky1311WriteCmd(CMD_IDLE);
        return NoResponse;
    }
    else
    {
        irqClearAll();
        return Error;
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \简  述  M1卡的加密验证,
 **
 ** \参  数  uint8_t auth_mode 验证的密钥（Key_A 或者 Key B)
 **          uint8_t* m1Key 密钥
             uint8_t* UID 卡的UID号
             uint8_t blockAddr 要操作的扇区地址（0--63）
 ** \返回值  验证状态
 ******************************************************************************/
sta_result_t M1_Authentication(uint8_t auth_mode,uint8_t *m1Key,uint8_t *uid, uint8_t blockAddr)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    volatile uint16_t delayCount;

    uint32_t RA = 0xAACBD156;       // 随机数，这里用随意的数值代替

    sky1311WriteReg(ADDR_M1_CTRL, 0x00);    // 复位M1控制寄存器
    sky1311TxCrcOn();
    sky1311RxCrcOff();
    // load M1_KEY ( 低字节在前，先写入)
    sky1311WriteReg(ADDR_M1_KEY, m1Key[5]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[4]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[3]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[2]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[1]);
    sky1311WriteReg(ADDR_M1_KEY, m1Key[0]);

   // load UID （ 低字节在前，先写入）
    sky1311WriteReg(ADDR_M1_ID, uid[3]);
    sky1311WriteReg(ADDR_M1_ID, uid[2]);
    sky1311WriteReg(ADDR_M1_ID, uid[1]);
    sky1311WriteReg(ADDR_M1_ID, uid[0]);

    sky1311WriteReg(ADDR_M1_CTRL, 0x53);      // pass1: 加载密码和UID，与M1卡通信开始

    tmpBuf[0] = auth_mode;//M1_AUTH;          // 验证 Key A or Key B
    tmpBuf[1] = blockAddr;

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);  //获取 Token RB
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
      return NoResponse;

    sky1311WriteReg(ADDR_M1_CTRL, 0x05);       // pass2:
    sky1311TxCrcOff();
    sky1311RxCrcOff();
    tmpBuf[0] = (uint8_t)(RA >> 24);
    tmpBuf[1] = (uint8_t)(RA >> 16);
    tmpBuf[2] = (uint8_t)(RA >> 8);
    tmpBuf[3] = (uint8_t)(RA >> 0);

    delayCount = 0x7FFF;
    while(((sky1311ReadReg(ADDR_M1_SUC_STATE)&0x01)==0) && delayCount--);    // waiting for SUC64 ready

    tmpBuf[4] = sky1311ReadReg(ADDR_M1_SUC64_3);
    tmpBuf[5] = sky1311ReadReg(ADDR_M1_SUC64_2);
    tmpBuf[6] = sky1311ReadReg(ADDR_M1_SUC64_1);
    tmpBuf[7] = sky1311ReadReg(ADDR_M1_SUC64_0);

    sky1311_fifo_tx(TYPE_A, tmpBuf, 8);             // 发送 Token AB
    sky1311WriteReg(ADDR_M1_CTRL, 0x09);            // pass3，此后数据通信加密

    sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    //  验证 Token BA
    if(sky1311ReadReg(ADDR_M1_SUC96_3) == tmpBuf[0] &&
       sky1311ReadReg(ADDR_M1_SUC96_2) == tmpBuf[1] &&
       sky1311ReadReg(ADDR_M1_SUC96_1) == tmpBuf[2] &&
       sky1311ReadReg(ADDR_M1_SUC96_0) == tmpBuf[3]
    )
        return Ok;
    else
        return NoResponse;

}
/**
 ******************************************************************************
 ** \简  述  M1卡修改扇区密码
 **
 ** \参  数  uint8_t blockAddr 要操作的扇区块地址（0--63）
 **          uint8_t* key_a 密钥A
             uint8_t* key_b 密码B
 ** \返回值  操作状态
 ******************************************************************************/
sta_result_t M1_PwdChange(uint8_t blockAddr, uint8_t *key_a, uint8_t *key_b)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    tmpBuf[0] = M1_WRITE;
    tmpBuf[1] = blockAddr;

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f)!= M1_ACK)
        return M1Error;                    // check ACK

    tmpBuf[0]    =   key_a[0];
    tmpBuf[1]    =   key_a[1];
    tmpBuf[2]    =   key_a[2];
    tmpBuf[3]    =   key_a[3];
    tmpBuf[4]    =   key_a[4];
    tmpBuf[5]    =   key_a[5];
    tmpBuf[6]    =   0xff;
    tmpBuf[7]    =   0x07;
    tmpBuf[8]    =   0x80;
    tmpBuf[9]    =   0x69;          // FFH 07H 80H 69H 是默认的控制字
    tmpBuf[10]   =   key_b[0];
    tmpBuf[11]   =   key_b[1];
    tmpBuf[12]   =   key_b[2];
    tmpBuf[13]   =   key_b[3];
    tmpBuf[14]   =   key_b[4];
    tmpBuf[15]   =   key_b[5];

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 16);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,16,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] &0x0f) != M1_ACK)
        return M1Error;                    // check ACK
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  M1卡写扇区数据。一次写入16个字节数据
 **
 ** \参  数  uint8_t blockAddr        要操作的扇区块地址（0--63）
             uint8_t* data           要写入的数据
 ** \返回值  操作状态 Ok表示成功， 其它表示有错误
 ******************************************************************************/
sta_result_t M1_Write(uint8_t blockAddr, uint8_t* data)
{
    sta_result_t sta;
    uint8_t tmpBuf[16];
    uint16_t tmpSize;

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    tmpBuf[0] = M1_WRITE;       // CMD = 0xA0
    tmpBuf[1] = blockAddr;      //

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f) != M1_ACK)
        return M1Error;

    //sky1311_fifo_tx(TYPE_A, data, 16);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,data,16,tmpBuf,&tmpSize);
    if(sta == NoResponse)
      return NoResponse;
    if((tmpBuf[0] &0x0f) != M1_ACK)
      return M1Error;

    return sta;
}
/**
 ******************************************************************************
 ** \简  述  M1卡读扇区数据，
 **
 ** \参  数  uint8_t blockAddr    要操作的扇区块地址（0--63）
 **          uint8_t *blockBuff  读取的数据存储地址
 ** \返回值  操作状态
 ******************************************************************************/
sta_result_t M1_Read(uint8_t blockAddr, uint8_t *blockBuff)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    sky1311TxCrcOn();
    sky1311RxCrcOn();

    tmpBuf[0] = M1_READ;    // 0x30
    tmpBuf[1] = blockAddr;
    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, blockBuff, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,blockBuff,&tmpSize);
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  M1卡扇区数据加值
 **
 ** \参  数  uint8_t blockAddr 要操作的扇区块地址（0--63）
             uint32_t 要增加的数
 ** \返回值  操作状态
 ******************************************************************************/
sta_result_t M1_Increment(uint8_t blockAddr, uint32_t value)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    tmpBuf[0] = M1_INCREMENT;
    tmpBuf[1] = blockAddr;

    //sky1311_fifo_tx(TYPE_A, tmpBuf, 2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;

    if((tmpBuf[0] & 0x0f) != M1_ACK)
        return M1Error;

    tmpBuf[0] = (uint8_t)value;
    tmpBuf[1] = (uint8_t)(value>>8);
    tmpBuf[2] = (uint8_t)(value>>16);
    tmpBuf[3] = (uint8_t)(value>>24);

    //sky1311_fifo_tx(TYPE_A, tmpBuf,4);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,4,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f) != M1_ACK)
        return M1Error;
    /*
    if(sta != Ok)
    {
      return sta;
    }

    delay(1);

    sta = M1_Transfer(blockAddr);
    */
    return sta;
}
/**
 ******************************************************************************
 ** \简  述  M1卡扇区数据减值
 **
 ** \参  数  uint8_t blockAddr 要操作的扇区块地址（0--63）
             uint32_t 要减少的数
 ** \返回值  操作状态
 ******************************************************************************/
sta_result_t M1_Decrement(uint8_t blockAddr, uint32_t value)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    tmpBuf[0] = M1_DECREMENT;
    tmpBuf[1] = blockAddr;

    //sky1311_fifo_tx(TYPE_A, tmpBuf,2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f) != M1_ACK)
        return M1Error;

    tmpBuf[0] = (uint8_t)value;
    tmpBuf[1] = (uint8_t)(value>>8);
    tmpBuf[2] = (uint8_t)(value>>16);
    tmpBuf[3] = (uint8_t)(value>>24);
    //sky1311_fifo_tx(TYPE_A, tmpBuf,4);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);// no answer
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,4,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f) != M1_ACK)
        return M1Error;
    /*
    if(sta != Ok)
    {
      return sta;
    }

    delay(1);

    sta = M1_Transfer(blockAddr);
    */
    return sta;
}

/**
 ******************************************************************************
 ** \简  述  M1卡数据恢复，将block中的数复制到易失性寄存器
 **
 ** \参  数  uint8_t blockAddr 要操作的扇区块地址（0--63）
 ** \返回值  操作状态
 ******************************************************************************/
sta_result_t M1_Restore(uint8_t blockAddr)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    tmpBuf[0] = M1_RESTORE;
    tmpBuf[1] = blockAddr;

    //sky1311_fifo_tx(TYPE_A, tmpBuf,2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f) != M1_ACK)
        return M1Error;
    return sta;
}

/**
 ******************************************************************************
 ** \简  述  将保存在易失性寄存器中的数存放到指定的Block（EEPROM）中
 **
 ** \参  数  uint8_t blockAddr 要操作的扇区块地址（0--63）
 ** \返回值  操作状态
 ******************************************************************************/
sta_result_t M1_Transfer(uint8_t blockAddr)
{
    sta_result_t sta;
    uint16_t tmpSize;
    uint8_t tmpBuf[16];

    sky1311TxCrcOn();
    sky1311RxCrcOff();

    tmpBuf[0] = M1_TRANSFER;
    tmpBuf[1] = blockAddr;

    //sky1311_fifo_tx(TYPE_A, tmpBuf,2);
    //sta = sky1311_fifo_rx(TYPE_A, RATE_OFF, tmpBuf, &tmpSize);
    sta = ExchangeData(TYPE_A,RATE_OFF,tmpBuf,2,tmpBuf,&tmpSize);
    if(sta == NoResponse)
        return NoResponse;
    if((tmpBuf[0] & 0x0f)!= M1_ACK)
        return M1Error;

    return sta;
}

