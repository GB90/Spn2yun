#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

//定义板类型
#define	BoardType	TP201
#define	TP201	1
#define	NoTP201	0				//最小板，测试用


#if(BoardType==TP201)	//如果是TP201板:
//	//IO方向设置
//	#define SDA_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=8<<24;}		//PB6
//	#define SDA_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=3<<24;}		//PB6

//	//IO操作函数	 
//	#define IIC_SCL    PBout(7) //SCL
//	#define IIC_SDA    PBout(6) //SDA	 
//	#define READ_SDA   PBin(6)  //输入SDA 
    
#define SCL_H         (GPIOB->BSRR = GPIO_Pin_7)
#define SCL_L         (GPIOB->BRR  = GPIO_Pin_7) 
   
#define SDA_H         (GPIOB->BSRR = GPIO_Pin_6)
#define SDA_L         (GPIOB->BRR  = GPIO_Pin_6)

#define SCL_read      (GPIOB->IDR  & GPIO_Pin_7)
#define SDA_read      (GPIOB->IDR  & GPIO_Pin_6)

    
#else	//如果是最小板:
	//IO方向设置
	#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}		//PB7
	#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}		//PB7

	//IO操作函数	 
	#define IIC_SCL    PBout(6) //SCL
	#define IIC_SDA    PBout(7) //SDA	 
	#define READ_SDA   PBin(7)  //输入SDA 
#endif

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















