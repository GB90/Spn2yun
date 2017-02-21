#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

//���������
#define	BoardType	TP201
#define	TP201	1
#define	NoTP201	0				//��С�壬������


#if(BoardType==TP201)	//�����TP201��:
//	//IO��������
//	#define SDA_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=8<<24;}		//PB6
//	#define SDA_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=3<<24;}		//PB6

//	//IO��������	 
//	#define IIC_SCL    PBout(7) //SCL
//	#define IIC_SDA    PBout(6) //SDA	 
//	#define READ_SDA   PBin(6)  //����SDA 
    
#define SCL_H         (GPIOB->BSRR = GPIO_Pin_7)
#define SCL_L         (GPIOB->BRR  = GPIO_Pin_7) 
   
#define SDA_H         (GPIOB->BSRR = GPIO_Pin_6)
#define SDA_L         (GPIOB->BRR  = GPIO_Pin_6)

#define SCL_read      (GPIOB->IDR  & GPIO_Pin_7)
#define SDA_read      (GPIOB->IDR  & GPIO_Pin_6)

    
#else	//�������С��:
	//IO��������
	#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}		//PB7
	#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}		//PB7

	//IO��������	 
	#define IIC_SCL    PBout(6) //SCL
	#define IIC_SDA    PBout(7) //SDA	 
	#define READ_SDA   PBin(7)  //����SDA 
#endif

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















