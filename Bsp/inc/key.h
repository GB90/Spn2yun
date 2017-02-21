#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP
 
//#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����0
//#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����1
//#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����2 
//#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP) 
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)//��ȡ����1
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//��ȡ����2
#define KEY3  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)//��ȡ����4

//������λ��
#define KeyD1  0x20		//����1
#define KeyD2  0x10		//����2
#define KeyD3  0x40		//����3
#define KeyD4  0x80		//����4
#define KeyU1  0x20		//����1
#define KeyU2  0x10		//����2
#define KeyU3  0x40		//����3
#define KeyU4  0x80		//����4
//��(˫)����λ��
#define KeyD12  0x30		//����12
#define KeyD13  0x60		//����13
#define KeyD23  0x50		//����23
#define KeyD24  0x90		//����24
#define KeyU12  0x30		//����12
#define KeyU13  0x60		//����13
#define KeyU23  0x50		//����23
#define KeyU24  0x90		//����24

//#define KEY_UP 		4
//#define KEY_LEFT	3
//#define KEY_DOWN	2
//#define KEY_RIGHT	1
//extern u8 key;		

extern u8 KeyHoldStusA;	//���ⰴ���ж�ֵ	
extern u16 KTCnt; 	//

void KEY_Init(void);//IO��ʼ��
void KEY_Scan(u8);  	//����ɨ�躯��					    

#endif

