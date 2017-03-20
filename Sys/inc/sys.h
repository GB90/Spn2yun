#ifndef __SYS_H
#define __SYS_H	

#include "stm32f10x.h"

//0,��֧��ucos
//1,֧��ucos
#define SYSTEM_SUPPORT_UCOS		0		//����ϵͳ�ļ����Ƿ�֧��UCOS
																	    
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

typedef struct
{
	u16 end;
	u16 strat;
	u16 now;
}DevicePos;

typedef struct
{
	//�Կ�����:���ʾ���0,�����ʾ���1
	u16 FK_IN_Pers;		//���������ѹ//��������ֵ%(���λΪ����λ)
	u16 VMA_IN_Pers;	//�����ѹ�����//��������ֵ%(���λΪ����λ)
	u16 FK_IN;			//���������ѹ//ģ����,���ֵ��AD����ֵ
	u16 VMA_IN;			//�����ѹ�����//ģ����,���ֵ��AD����ֵ
	u16 BM1;			//��������������
	u16 BM2;			//��������������
	u16 BM3;			//��������������
	u16 OTL;			//�����������ȼ��
	u16 CTS;			//�������������ؼ��
	u16 OTS;			//�������������ؼ��
	u16 ACLS;			//���������ظ�����λ	
	u16 AOLS;			//����������������λ	
	u16 IN_RCL;			//��������Զ�� / ����
	u16 IN_ESD;			//��������Զ��ESD	
	u16 IN_BC;			//��������Զ�̱���
    u16 PHASE;		    //������������
    u16 PHASE_LOST;		//��������ȱ��
	u16 IN_SHUT;		//��������Զ�̹�
	u16 IN_OPEN;		//��������Զ�̿�
}InputPortStus;

typedef struct
{
	//�Կ�������8λ�����ʾ���1�����ʾ���0����8λ��ʾ�������ͨ����š���MRTS=0x0300����ʾ��3ͨ�����0
	u16 MD;				//�������� �м�λ�����
	u16 SHUTRUN;		//�������� ���������
	u16 OPENRUN;		//�������� ���������	
	u16 RCL;			//�������� Զ�̱������	
	u16 CTSOUT;			//�������� ���������		
 	u16 OTSOUT;			//�������� ���������		
 	u16 ACLSOUT;		//�������� �ظ�����λ���	
	u16 AOLSOUT;		//�������� ��������λ���			
	u16 AL;				//�������� ���ϱ������		
	u16 M_SW;			//�������� �����ת���
	u16 M_OS;			//�������� �����ת���	
	u16 CV_IO;			//�������� ��ѹ/�������ģʽת��
	u16 MA_OUT;		    //����������������ı��ֵ(�������4mA~20mA��Ӧ��PWMռ�ձ�ֵ)
	u16 V_OUT;			//�������������ѹ�ı��ֵ(�����ѹ0V~10V��Ӧ��PWMռ�ձ�ֵ)
}OutputPortStus;		//	

typedef union
{
    u32 Myu32;
    struct
    {
        u16 Myu16L;
        u16 Myu16H;
    }Myu16;
}MYU32;		//	

void NVIC_Configuration(void);

#endif

