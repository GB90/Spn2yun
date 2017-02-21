#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP
 
//#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
//#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
//#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
//#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)//读取按键1
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//读取按键2
#define KEY3  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)//读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)//读取按键4

//单键键位码
#define KeyD1  0x20		//按键1
#define KeyD2  0x10		//按键2
#define KeyD3  0x40		//按键3
#define KeyD4  0x80		//按键4
#define KeyU1  0x20		//按键1
#define KeyU2  0x10		//按键2
#define KeyU3  0x40		//按键3
#define KeyU4  0x80		//按键4
//多(双)键键位码
#define KeyD12  0x30		//按键12
#define KeyD13  0x60		//按键13
#define KeyD23  0x50		//按键23
#define KeyD24  0x90		//按键24
#define KeyU12  0x30		//按键12
#define KeyU13  0x60		//按键13
#define KeyU23  0x50		//按键23
#define KeyU24  0x90		//按键24

//#define KEY_UP 		4
//#define KEY_LEFT	3
//#define KEY_DOWN	2
//#define KEY_RIGHT	1
//extern u8 key;		

extern u8 KeyHoldStusA;	//开光按键判断值	
extern u16 KTCnt; 	//

void KEY_Init(void);//IO初始化
void KEY_Scan(u8);  	//按键扫描函数					    

#endif

