#ifndef	__BIOS_H__
#define	__BIOS_H__
#include "sys.h"
#include "Delay.h"
#define _StarPos			0
#define _EndPos			    0
#define _ResetVal			0

#define ENCODER_MAX			((u32)4095)//32bit ���������ֵ
#define ENCODER_ZERO    	((u8)100)//���ķ�Χ ��100

#define Moto_REV_Drv		{if(1 == CloseDirection){POStus.M_OS=0xb00;POStus.M_SW=0xb01;}\
							 else{POStus.M_SW=0xb00;POStus.M_OS=0xb01;}}					//CloseDirection=1 ��ת��CloseDirection=0 ��ת
							 
#define Moto_FWD_Drv		{if(1 == CloseDirection){POStus.M_SW=0xb00;POStus.M_OS=0xb01;}\
							 else{POStus.M_OS=0xb00;POStus.M_SW=0xb01;}}					//CloseDirection=1 ��ת��CloseDirection=0��ת
#if (POWER_MODE == 1)
#define Moto_STOP_Drv		{if(!Moto_PARK_Chk){POStus.M_SW=POStus.M_OS=0xb00;}			    //ɲ��	(ɲ��20ms��Ȼ��ֹͣ)                             
#else
#define Moto_STOP_Drv		{if(!Moto_PARK_Chk){POStus.M_SW=0xb01;POStus.M_OS=0xb01;\
  							POStus.M_SW=POStus.M_OS=0xb00;}}			    //ɲ��	(ɲ��20ms��Ȼ��ֹͣ)                       
#endif
							
//#define Moto_PARK_Drv		{if(!Moto_PARK_Chk){POStus.M_SW=POStus.M_OS=0xb00;}}	
#define Moto_PARK_Drv		{POStus.M_SW=POStus.M_OS=0xb00;}                                            //ֹͣ
//���״̬��ѯ
#define Moto_FWD_Chk		(((POStus.M_SW & 0xff) ==0) && (POStus.M_OS & 0xff))		//��ת״̬
#define Moto_REV_Chk		((POStus.M_SW & 0xff) && ((POStus.M_OS & 0xff)==0))		    //��ת״̬
#define Moto_PARK_Chk		(((POStus.M_SW & 0xff) ==0)&&((POStus.M_OS & 0xff) ==0))    //ֹͣ״̬
#define Moto_STOP_Chk		(( POStus.M_SW & 0xff )&&(POStus.M_OS & 0xff))  		 	//ɲ��״̬

//����λ�ò�ѯ��
#define _LessLdD		0	//��Ч����֮��
#define _LessTarget		1	//С��Ŀ��ֵ
#define _InTarget		2	//����Ŀ��ֵ
#define _MoreTarget		3	//����Ŀ��ֵ
#define _MoreLdU		4	//��Ч����֮��

#define  DIO_IN()       {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=8<<20;}
#define  DIO_OUT()      {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=3<<20;}
#define  DIO_SET()      {PAout(5) = 1;}
#define  DIO_CLR()      {PAout(5) = 0;}
#define  READ_DIO()     PAin(5)


#define  SCLK_SET()     {PAout(4) = 1;}
#define  SCLK_CLR()     {PAout(4) = 0;}
 
#define  CS_SET()       {PAout(7) = 1;}
#define  CS_CLR()       {PAout(7) = 0;}

typedef union
{
	u8 ByteArray[16];
	struct
	{   //��̬��Щ״̬
        u8 RelayShut;       //�ص�λ
		u8 RelayOpen;       //����λ
		u8 RelayCTor;       //�ع���
		u8 RelayOTor;       //������
        u8 RelayTor;        //����
		u8 RelayNowShut;    //���ڹ�
		u8 RelayNowOpen;    //���ڿ�
		u8 RelayLocal;      //�͵�
		u8 RelayESD;        //ESD�ź�
		u8 RelayMid1;       //�м�λ��1
        u8 RelayMid2;       //�м�λ��1
		u8 RelayFault;      //����
        u8 RelayLost;       //ȱ��
	}Byte;
}RELAY_BYTE;

typedef struct
{
	u8 Relay0:1;
	u8 Relay1:1;
	u8 Relay2:1;
	u8 Relay3:1;
	
	u8 Relay4:1;
	u8 Relay5:1;
	u8 Relay6:1;
	u8 Relay7:1;
	
	u8 Relay8:1;
	u8 Relay9:1;
	u8 Relay10:1;
	u8 Relay11:1;
	
	u8 Relay12:1;
	u8 Relay13:1;
	u8 Relay14:1;
	u8 Relay15:1;
}RELAY_BIT;


extern u16 ChanlCnt;
extern u16 ChSel[]; //10������ڵ����ͨ���š���Ĭ��ֵ : MRTS ChS = 9;MDChS = 8;SHUTRUN ChS = 7;OPENRUN ChS = 6;
				//RCL ChS = 5;CTSOUT ChS = 4;OTSOUT ChS = 3;ACLSOUT ChS = 2;AOLSOUT ChS = 1;AL ChS = 0;
extern u16 WaitMoto;
extern u16 PhaseDir;

extern u16 MRTSChS;
extern u16 MDChS;
extern u16 SHUTRUNChS;
extern u16 OPENRUNChS;
extern u16 RCLChS;
extern u16 CTSOUTChS;
extern u16 OTSOUTChS;
extern u16 ACLSOUTChS;
extern u16 AOLSOUTChS;
extern u16 ALChS;
//extern u8 RelayRunFlag;
extern u16 InputTimer;
extern u16 RelayType[9];       //����̵�����Ҫ��̬���������
extern RELAY_BYTE RelayByte;
extern void Output(void);
void Sample(void);
void IO_Init(void);
u8 GetMotoPos(void);
u8 IsEncoderZero(u8 Dir);
extern u16 GetMedianNum(u16 *bArray, u8 iFilterLen);
#endif

