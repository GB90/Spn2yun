#ifndef __USART_H
#define __USART_H

#include "Def.h"
#include "sys.h" 
#include "adc.h" 

#define RS485_TX_EN		PAout(8)			//485ģʽ����.0,����;1,����.
#define MB_ADDR		    1
#define MB_TIMEOUT      2500                //5S
typedef enum {R_CRC,R_FUC,R_RESPOND,R_END}MBFSM;//modbus����״̬��

typedef struct
{
    u8 MbStart:1;       //��ʼ����
    u8 MbInData:1;      //���������Ƿ��б�
    u8 MbRicved:1;      //�ɹ�����
    u8 MbOnline:1;      //Modbus����
    u8 MbSend:1;        //��Ӧ
    u8 MbErr:3;         //������
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
}WB;//�����ֽ�

typedef struct
{
    u16 MbSwitch;          //������
    u16 MbVavlePer;        //��λ�ٷֱ�
    u16 MbCloseDir;        //�ط���
    u16 MbCloseMode;       //��ģʽ
    u16 MbEsdMode;         //ESD����
    u16 MbLow;             //���Ŷ���
    u16 MbRemotMode;       //Զ��ģʽ ģ�����򿪹���
    u16 MbRelay[9];        //�̵�����̬��Ϣ
    u16 MbCPUTemp;         //CPU�¶�
    u16 MbVmaPer;          //ģ�����ٷֱ�
    u16 MbSN;              //���к�
    u16 MbTimer;           //��������
    u16 MbVer;             //�汾
    u16 MbAl;             //���󱨾�
}SMBDATA;                  //Modbusд�����������

typedef union
{
    u16 MbDataArray[22];
    SMBDATA sMbData;
}UMBDATA;                  //Modbusд�����������

extern UMBDATA RMbData, WMbData;
extern MB_STATUS MbStatus;
extern u32 MbTimer;

void uart_init(u32 bound);
void ModBusDataInit(void);
void ModBusRun(void);
#endif


