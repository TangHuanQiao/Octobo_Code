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
 ** 文 件 名： typedef.h
 **
 ** 文件简述： 程序中变量的基本类型、结构体和其它类型定义
 **
 ** 版本历史:
 ** 2015-10-05 V1.00  Mark   正式版本
 **
 ******************************************************************************/

#ifndef  __TYPEDEF_H_
#define  __TYPEDEF_H_

#include <stddef.h>
//#include "stdint.h"
#include "mcu.h"

#define NOCHANGE    0
#define LARGEN      1
#define LESSEN      2
#define MINIMUM     3
#define MAXIMUM     4
/**
 ******************************************************************************
 ** \简述：基本类型定义
 **
 ******************************************************************************/
#ifndef TRUE
	/** Value is true (boolean_t type) */
    #define TRUE        ((boolean) 1)
    #define true        ((boolean) 1)
#endif

#ifndef FALSE
	/** Value is false (boolean_t type) */
    #define FALSE       ((boolean) 0)
	#define false       ((boolean) 0)
#endif

typedef uint8_t         boolean;

/** logical datatype (only values are TRUE and FALSE) */
typedef uint8_t      boolean_t;

/** single precision floating point number (4 byte) */
typedef float        float32_t;

/** double precision floating point number (8 byte) */
typedef double       float64_t;

/** ASCCI character for string generation (8 bit) */
typedef char         char_t;

/** function pointer type to void/void function */
typedef void         (*func_ptr_t)(void);

/** function pointer type to void/uint8_t function */
typedef void         (*func_ptr_arg1_t)(uint8_t);


typedef enum sta_result
{
    Ok                      = 0,  ///< 无错，操作成功
    Error                   = 1,  ///< 非特定错误
    Timeout                 = 3,  ///< 超时错误
    RxParity			    = 4,  ///< 接收奇偶校验错
    RxCheckCRC			    = 5,  ///< 接收CRC校验错
    FifoFull                = 6,  ///< FIFO满
    FifoEmpty               = 7,  ///< FIFO空
    Collision               = 8,  ///< 防冲突错误
    Framing                 = 9,  ///< 数据帧错误
    UIDFormat               = 10, ///< UID格式错误
    M1Error                 = 11, ///< 操作M1卡错误

    ErrorRequest            = 12,  ///< 执行REQA/WUPA 命令时出错
    ErrorAnticollision      = 13,  ///< 执行防冲突命令ANTICOLLISION时出错
    ErrorSelect             = 14,  ///< 执行SELECT 命令时出错
    ErrorAts                = 15,  ///< 执行ATS命令出错
    ErrorInvalidMode            = 16,  ///< Operation not allowed in current mode
    ErrorUninitialized          = 17,  ///< Module (or part of it) was not initialized properly
    ErrorBufferFull             = 18,  ///< Circular buffer can not be written because the buffer is full
    ErrorTimeout                = 19,  ///< Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.)
    ErrorNotReady               = 20, ///< A requested final state is not reached
    OperationInProgress         = 21,  ///< Indicator for operation in progress

	UnknowError				= 0x7F,
    NoResponse              = 0xFF
}sta_result_t,en_result_t;


typedef enum sta_field
{
    NoObjectIn              = 0,    ///< 读卡器场内无卡或异物
    PiccIn                  = 1,    ///< 有卡在场内
    OtherObjectIn           = 2
}sta_field_t;


/** Returns the minimum value out of two values */
#define MIN( X, Y )  ((X) < (Y) ? (X) : (Y))

/** Returns the maximum value out of two values */
#define MAX( X, Y )  ((X) > (Y) ? (X) : (Y))

/** Returns the dimension of an array */
#define DIM( X )  (sizeof(X) / sizeof(X[0]))



#endif