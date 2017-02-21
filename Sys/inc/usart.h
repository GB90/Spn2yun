#ifndef __USART_H
#define __USART_H

#include "Def.h"
#include "sys.h" 
#include "adc.h" 

#define RS485_TX_EN		PAout(8)			//485模式控制.0,接收;1,发送.
#define MB_ADDR		    1
#define MB_TIMEOUT      2500                //5S
typedef enum {R_CRC,R_FUC,R_RESPOND,R_END}MBFSM;//modbus运行状态机

typedef struct
{
    u8 MbStart:1;       //开始接收
    u8 MbInData:1;      //输入数据是否有变
    u8 MbRicved:1;      //成功接收
    u8 MbOnline:1;      //Modbus在线
    u8 MbSend:1;        //响应
    u8 MbErr:3;         //错误码
}MB_STATUS;

typedef struct
{
    u8 Addr;
    u8 FucCode;
    u8 LocaHi;
    u8 LocaLo;
    u8 NumHi;
    u8 NumLo;
    u8 CrcLo;
    u8 CrcHi;
}MB_strRICV;

typedef union
{
    MB_strRICV  MbstrRicv;
    u8 DataRicv[8];
}MB_unRICV;

typedef struct
{
    u8 Addr;
    u8 FucCode;
    u8 Num;
    u8 Data[20];
    u8 CrcLo;
    u8 CrcHi;
}MB_strSEND03;

typedef struct
{
    u8 Addr;
    u8 FucCode;
    u8 LocaHi;
    u8 LocaLo;
    u8 DataHi;
    u8 DataLo;
    u8 CrcLo;
    u8 CrcHi;
}MB_strSEND06;

typedef union
{
    u16 Word;
    struct
    {
        u8 ByteL;
        u8 ByteH;
    }Byte;
}WB;//字与字节

typedef struct
{
    u16 MbSwitch;          //开关量
    u16 MbVavlePer;        //阀位百分比
    u16 MbCloseDir;        //关方向
    u16 MbCloseMode;       //关模式
    u16 MbEsdMode;         //ESD动作
    u16 MbLow;             //丢信动作
    u16 MbRemotMode;       //远程模式 模拟量或开关量
    u16 MbRelay[9];        //继电器组态信息
    u16 MbCPUTemp;         //CPU温度
    u16 MbVmaPer;          //模拟量百分比
    u16 MbSN;              //序列号
    u16 MbTimer;           //生产日期
    u16 MbVer;             //版本
    u16 MbAl;             //错误报警
}SMBDATA;                  //Modbus写入参数缓冲区

typedef union
{
    u16 MbDataArray[22];
    SMBDATA sMbData;
}UMBDATA;                  //Modbus写入参数缓冲区

extern UMBDATA RMbData, WMbData;
extern MB_STATUS MbStatus;
extern u32 MbTimer;

void uart_init(u32 bound);
void ModBusDataInit(void);
void ModBusRun(void);
#endif


