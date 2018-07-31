/******************************************************************************/
/*               (C) 斯凯瑞利（北京）科技有限公司(SKYRELAY)                   */
/*                                                                            */
/* 此代码是斯凯瑞利（北京）科技有限公司为支持客户编写的示例程序的一部分       */
/* 所有使用斯凯瑞利相关芯片的用户可以无偿使用该代码，但需保留本声明部分       */
/* 本代码仅为演示使用，不保证符合所有的应用要求，当有特定规范或需求时，       */
/* 使用者需自行确认代码是否符合要求，不合要求时自行做出相应的修改。           */
/* (V1.00)                                                                    */
/******************************************************************************/
/**
 ******************************************************************************
 ** 文 件 名： sky1311reg.h
 **
 ** 文件简述： sky1311s的命令字、寄存器和位定义头文件
 **
 ** 版本历史:
 ** 2015-10-05 V1.00  Mark      正式版本
 ** 2016-4-25         Mark      增加TX_EN 和 RX_EN的定义
 ** 2018-06-12 V1.1.2 Release   重新整理发布
 ******************************************************************************/
#ifndef  __SKY1311_H_
#define  __SKY1311_H_
/**
 ******************************************************************************
 ** \简述：sky1311 命令定义
 **
 ******************************************************************************/
#define     CMD_IDLE                (0x00)
#define     CMD_CLR_FF              (0x03)
#define     CMD_TX                  (0x04)
#define     CMD_RX                  (0x08)
#define     CMD_TX_RX               (0x0C)
#define     CMD_SW_RST              (0x0F)
#define     CMD_AUTO_M1             (0x16)
/**
 ******************************************************************************
 ** \简述：sky1311 寄存器地址定义
 **
 ******************************************************************************/
#define     ADDR_FIFO_LEN           (0x00)
#define     ADDR_FIFO_STA           (0x01)
#define     ADDR_FIFO_CTRL          (0x02)
#define     ADDR_FIFO               (0x03)
#define     ADDR_TX_CTRL            (0x04)
#define     ADDR_TX_PUL_WID         (0x05)
#define     ADDR_TX_BYTE_NUM        (0x06)
#define     ADDR_TX_BIT_NUM         (0x07)
#define     ADDR_TX_FWAIT           (0x08)
#define     ADDR_TIME_OUT0          (0x09)
#define     ADDR_TIME_OUT1          (0x0A)
#define     ADDR_TIME_OUT2          (0x0B)
#define     ADDR_FGUD_RX            (0x0C)
#define     ADDR_RX_CTRL            (0x0D)
#define     ADDR_RX_PUL_DETA        (0x0E)
#define     ADDR_CRC_CTRL           (0x0F)
#define     ADDR_RX_START_BIT_NUM   (0x10)
#define     ADDR_CRC_IN             (0x11)
#define     ADDR_IRQ_EN             (0x12)
#define     ADDR_IRQ_STA            (0x13)
#define     ADDR_ERR_STA            (0x14)
#define     ADDR_RX_NUM_L           (0x15)
#define     ADDR_RX_NUM_H           (0x16)
#define     ADDR_TIMER_CNT0         (0x17)
#define     ADDR_TIMER_CNT1         (0x18)
#define     ADDR_FSM_STATE          (0x19)
#define     ADDR_PA_OFFSET          (0x1A)
#define     ADDR_MOD_SRC            (0x1B)
#define     ADDR_MFOUT_SEL          (0x1C)
#define     ADDR_ANA_CFG0           (0x1D)
#define     ADDR_ANA_CFG1           (0x1E)
#define     ADDR_ANA_CFG2           (0x1F)
#define     ADDR_RATE_CTRL          (0x20)
#define     ADDR_RATE_THRES         (0x21)
#define     ADDR_RATE_FRAME_END     (0x22)
#define     ADDR_RATE_SUB_THRES     (0x23)
#define     ADDR_RATE_RX_BYTE       (0x24)
#define     ADDR_RATE_RX_BIT        (0x25)
#define     ADDR_M1_SUC_STATE       (0x26)
#define     ADDR_M1_SUC64_0         (0x27)
#define     ADDR_M1_SUC64_1         (0x28)
#define     ADDR_M1_SUC64_2         (0x29)
#define     ADDR_M1_SUC64_3         (0x2A)
#define     ADDR_M1_SUC96_0         (0x2B)
#define     ADDR_M1_SUC96_1         (0x2C)
#define     ADDR_M1_SUC96_2         (0x2D)
#define     ADDR_M1_SUC96_3         (0x2E)
#define     ADDR_M1_CTRL            (0x2F)
#define     ADDR_M1_KEY             (0x30)
#define     ADDR_M1_ID              (0x31)
#define     ADDR_RX_PRE_PROC        (0x32)
#define     ADDR_TX_B_CTRL          (0x33)
#define     ADDR_TX_B_EGT_NUM       (0x34)
#define     ADDR_TX_B_BYTE_NUM      (0x35)
#define     ADDR_RX_B_CTRL          (0x36)
#define     ADDR_RX_B_BYTE_NUM      (0x37)
#define     ADDR_RX_B_PRE           (0x38)
#define     ADDR_RX_MANC_SEC_PULSE  (0x39)
#define     ADDR_ANA_CFG3           (0x3A)
#define     ADDR_ANA_CFG4           (0x3B)
#define     ADDR_ANA_CFG5           (0x3C)
#define     ADDR_ANA_CFG6           (0x3D)
#define     ADDR_ANA_CFG7           (0x3E)
#define     ADDR_ANA_CFG8           (0x3F)
/**
 ******************************************************************************
 ** \简述：sky1311 寄存器位定义
 **
 ******************************************************************************/
#define     TX_106                  (0x00)
#define     TX_212                  (0x40)
#define     TX_424                  (0x80)
#define     TX_POLE_HIGH            (1<<3)
#define     TX_CRC_EN               (1<<2)
#define     TX_PARITY_EVEN          (0x01)
#define     TX_PARITY_ODD           (0x03)
#define     TX_POLE                 (1<<3)
#define     RX_PARITY_EN            (1<<0)
#define     RX_CRC_EN               (1<<1)
#define     RX_PARITY_ODD           (1<<2)
#define     RX_MIFARE_ON            (1<<3)
#define     RX_Cal_CTRL_0           (1<<6)
#define     RX_Cal_CTRL_1           (1<<7)
#define     IRQ_M1_EN               (1<<7)
#define     IRQ_TOUT_EN             (1<<6)
#define     IRQ_TX_EN               (1<<5)
#define     IRQ_RX_EN               (1<<4)
#define     IRQ_HIGH_EN             (1<<3)
#define     IRQ_LOW_EN              (1<<2)
#define     IRQ_OSC_EN              (1<<1)
#define     IRQ_ERR_EN              (1<<0)
#define     IRQ_M1                  (1<<7)
#define     IRQ_TOUT                (1<<6)
#define     IRQ_TX                  (1<<5)
#define     IRQ_RX                  (1<<4)
#define     IRQ_HIGH                (1<<3)
#define     IRQ_LOW                 (1<<2)
#define     IRQ_OSC                 (1<<1)
#define     IRQ_ERR                 (1<<0)
#define     MFOUT_RX_PHASE          (0x00)
#define     MFOUT_BIT_TX            (0x01)
#define     MFOUT_RX_BIT            (0x02)
#define     MFOUT_ANALOG_RX         (0x03)
#define     MFOUT_DO                (0x04)
#define     COLL_EN                 (1<<1)
#define     RX_FORBID               (1<<2)
#define     COLL_FLAG               (1<<3)
#define     RX_RATE_WID_0           (0x00)
#define     RX_RATE_WID_2           (0x40)
#define     RX_RATE_WID_4           (0x80)
#define     RX_RATE_212             (0x10)
#define     RX_RATE_424             (0x20)
#define     RX_RATE_PAR_ODD         (0x04)
#define     RX_RATE_PAR_EVEN        (0x00)
#define     RX_RATE_CRC_EN          (0x02)
#define     RX_RATE_PAR_EN          (0x01)
#define     TX_B_EOF_L0             (1<<7)
#define     TX_B_SOF_L1             (1<<6)
#define     TX_B_SOF_L0             (1<<5)
#define     TX_B_EGT_S              (1<<4)
#define     TX_B_CRC_EN             (1<<3)
#define     TX_B_EOF_EN             (1<<2)
#define     TX_B_SOF_EN             (1<<1)
#define     TX_B_POLE               (1<<0)
#define     RX_B_CRC_EN             (1<<0)
#define     TX_EN                   (1<<7)
#define     RX_EN                   (1<<6)

#define     PA_3P3V                 (3<<2)
#define     PA_3P0V                 (2<<2)
#define     PA_2P5V                 (1<<2)
#define     PA_2P0V                 (0<<2)
#define     WKU_EN                  (1<<7)
#define     WKU_100MS               (0<<4)
#define     WKU_200MS               (1<<4)
#define     WKU_300MS               (2<<4)
#define     WKU_400MS               (3<<4)
#define     WKU_500MS               (4<<4)
#define     WKU_600MS               (5<<4)
#define     WKU_800MS               (6<<4)
#define     WKU_1S                  (7<<4)
#define     WKU_DETA                (0<<2)
#define     WKU_ABS                 (1<<2)
#define     WKU_AND                 (2<<2)
#define     WKU_OR                  (3<<2)
#define     RSSI_2AVG               (0   )
#define     RSSI_4AVG               (1)
#define     RSSI_8AVG               (2)
#define     RSSI_16AVG              (3)
#define     ADC_SAMPLE_5US          (0<<7)
#define     ADC_SAMPLE_10US         (1<<7)
#define     TX_SETTLE_0US           (0<<5)
#define     TX_SETTLE_5US           (1<<5)
#define     TX_SETTLE_10US          (2<<5)
#define     TX_SETTLE_15US          (3<<5)

/**
 ******************************************************************************
 ** \简述：sky1311 部分命令字定义
 **
 ******************************************************************************/
#define     TYPE_A_SEL              (0x00)
#define     TYPE_B_SEL              (0x40)
#define     RATE_SEL                (0x80)
#define     CRC_A                   (0x01)
#define     CRC_B                   (0x04)
#define     TYPE_A                  (0x01)
#define     TYPE_B                  (0x02)
#define     RATE_ON                 (0x01)
#define     RATE_OFF                (0x00)
#define     ANA2_A                  (0xA0)
#define     ANA2_B                  (0x98)
#define     COLL_NO                 (0x00)
#define     COLL_YES                (0x01)

#define     PARITY_CRC_ERROR        (0xC0)
#define     PARITY_ERROR            (0x80)
#define     CRC_ERROR               (0x40)
#define     NO_ANS                  (-1)
#define     M1_ERROR                (-2)

#endif
