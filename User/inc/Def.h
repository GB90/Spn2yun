#ifndef	__DEF_H__
#define	__DEF_H__

#include "Stmflash.h" 
#include "sys.h"
#include "timer.h"
#include "delay.h"
#include "myiic.h"
#include "adc.h"
#include "usart.h"
#include "LCD_160160.h"

#include "led.h"
#include "key.h"
#include "Display.h"
#include "remote.h"
#include "BIO.h"
#include "iwdg.h"

//#define __Debug__                                         //����ѡ��
#define LCDBACK         40                                  //��������0~99  10% Լ 7mA
#define DISTEMP         0                                   //�¶���ʾ          0:����ʾ        1:��ʾ
#define SOFTLOCK        0                                   //�����            0:��Ч          1:��Ч
#define MB_MODE         1                                   //modbusģʽ        0:��д          1:ֻ�� 2:��
#define MB_LOST         1                                   //modbus��ʱ����    0:�޳�ʱ        1:����ʱ 
#define POWER_MODE      1                                   //��Դģʽ          0:220V          1:380V
#define POWER_MODE_DISP 1                                   //POWER_MODE = 1ʱ ����ʾ����״̬  ��ǰ�������г�����ʱ�������Ƚ�

#define VERSION        ((const u8*)("V:0.1.41")) 			//�汾��
#define DATE           ((const u8*)("2016/10/27"))			//����
#define FLASH_SAVE_ADDR  0X08070000 		//
#define EN_INT          __enable_irq();     //ϵͳ��ȫ���ж�  
#define DIS_INT         __disable_irq();    //ϵͳ��ȫ���ж�  

//����������
//#define P_BM1 		PAin(4)		//����ڣ�������
//#define P_BM2 		PAin(5)		//����ڣ�������
//#define P_BM3 		PAin(7)		//����ڣ�������
#define P_OTL 		PEin(7)		    //����ڣ����ȼ��
#define P_CTS 		PBin(0)		    //����ڣ������ؼ��
#define P_OTS 		PCin(4)		    //����ڣ������ؼ��
#define P_ACLS		PCin(3)		    //����ڣ��ظ�����λ	
#define P_AOLS		PCin(1)		    //����ڣ���������λ	
#define P_IN_RCL	PCin(0)		    //����ڣ�Զ�� �Զ�/����
#define P_IN_ESD	PCin(13)	    //����ڣ�Զ��ESD	
#define P_IN_BC		PEin(5)		    //����ڣ�Զ�̱���
#define P_IN_SHUT 	PEin(4)		    //����ڣ�Զ�̹�
#define P_IN_OPEN 	PEin(3)		    //����ڣ�Զ�̿�
#define P_PHASE 	PBin(15)		//����ڣ�����
#define P_PHASELOST PEin(2)		    //����ڣ�ȱ��

//#define P_MRTS		PBout(15)	//�����1�� ��������/�������
//#define P_MD		    PBout(14)	//�����2�� �м�λ�����
//#define P_SHUTRUN 	PBout(13)	//�����3�� ���������
//#define P_OPENRUN 	PBout(12)	//�����4�� ���������	
//#define P_RCL 		PBout(11)	//�����5�� Զ�̱������ 
//#define P_CTSOUT	    PBout(10)	//�����6�� ���������		
//#define P_OTSOUT	    PBout(1)	//�����7�� ���������
//#define P_ACLSOUT 	PCout(5)	//�����8�� �ظ�����λ���	
//#define P_AOLSOUT 	PAout(6)	//�����9�� ��������λ���			
//#define P_AL		    PCout(2)	//�����10�����ϱ������ 	
//#define P_M_SW	    PCout(6)	//����ڣ�  ���������� 
//#define P_M_OS		PCout(7)	//����ڣ�  �������ת���	
//#define P_C_IO		PEout(6)	//����ڣ�  ��ѹ/�������ģʽת��


#define P_AL		    PCout(2)	//�����10�����ϱ������ 

#define P_AOLSOUT 	    PAout(6)	//�����9�� ��������λ���		
#define P_ACLSOUT 	    PCout(5)	//�����8�� �ظ�����λ���		
#define P_OTSOUT	    PBout(1)	//�����7�� ���������
#define P_CTSOUT	    PBout(10)	//�����6�� ���������	
	

//���������ͨ��1~10����:
#define Po10		PBout(15)		//�����10�� Ĭ��Ϊ��������/�������
#define Po9		    PBout(14)		//�����9��  Ĭ��Ϊ�м�λ�����
#define Po8 		PBout(13)		//�����8��  Ĭ��Ϊ���������
#define Po7 		PBout(12)		//�����7��  Ĭ��Ϊ���������	
#define Po6 		PBout(11)		//�����6��  Ĭ��ΪԶ�̱������ 
#define Po5		    PBout(10)		//�����5��  Ĭ��Ϊ���������		
#define Po4		    PBout(1)		//�����4��  Ĭ��Ϊ���������
#define Po3 		PCout(5)		//�����3��  Ĭ��Ϊ�ظ�����λ���	
#define Po2 		PAout(6)		//�����2��  Ĭ��Ϊ��������λ���			
#define Po1		    PCout(2)		//�����1��  Ĭ��Ϊ���ϱ������ 	

#define P_M_SW		PCout(6)		//����ڣ�  ���������� 
#define P_M_OS		PCout(7)		//����ڣ�  �������ת���	
#define P_C_IO		PEout(6)		//����ڣ�  ��ѹ/�������ģʽת��

#define BIT0		((u16)0x0001)
#define BIT1		((u16)0x0002)
#define BIT2		((u16)0x0004)
#define BIT3		((u16)0x0008)
#define BIT4		((u16)0x0010)
#define BIT5		((u16)0x0020)
#define BIT6		((u16)0x0040)
#define BIT7		((u16)0x0080)
#define BIT8		((u16)0x0100)
#define BIT9		((u16)0x0200)
#define BIT10		((u16)0x0400)
#define BIT11		((u16)0x0800)
#define BIT12		((u16)0x1000)
#define BIT13		((u16)0x2000)
#define BIT14		((u16)0x4000)
#define BIT15		((u16)0x8000)

/*******************��������bit*********************************/
#define ERROTS      BIT0            //������
#define ERRCTS      BIT1            //�ع���
#define VMALOW      BIT2            //����
#define INTTHIGH    BIT3            //�ڲ��¶ȹ���    
#define EXTTHIGH    BIT4            //�ⲿ�¶ȹ���(���)
#define ERRDIR      BIT5            //ת�����
#define ERRCMD      BIT6            //����ͬ��
#define SHUTBLOCK   BIT7            //�ض�ת
#define OPENBLOCK   BIT8            //����ת
#define PHASELOST   BIT9            //ȱ��
#define MBONLINE    BIT10           //ModBus ����
//#define _XianFan		3					//�߷�
//#define _ZhenDang		5					//��

typedef struct
{                                   //���ֵ
    u32 Year:6;                     //64
    u32 Month:4;                    //16
    u32 Day:5;                      //32
    u32 Hour:5;                     //32
    u32 Minute:6;                   //64
    u32 Second:6;                   //64
}STIMER;

typedef union
{
    u32 UTimer;
    STIMER STimer;
}UTIMER;

extern OutputPortStus POStus;
extern InputPortStus PIStus;
extern UTIMER uTimer;
#endif

