#ifndef __RED_H
#define __RED_H 
#include "sys.h"   

  
#define RDATA PBin(9)	 //红外数据输入脚

#define	K1		1							//返回键
#define	K2		2							//确认键
#define	K3		3							//+键
#define	K4		4							//-键
#define	K1Dn	(key&0x1f)==0x11			//返回键
#define	K2Dn	(key&0x1f)==0x12			//确认键
#define	K3Dn	(key&0x1f)==0x13			//+键
#define	K4Dn	(key&0x1f)==0x14			//-键
#define	K5Dn	(key&0x1f)==0x15			//测试键

#define	K1Up	(key&0x9f)==0x91			//返回键
#define	K2Up	(key&0x9f)==0x92			//确认键
#define	K3Up	(key&0x9f)==0x93			//+键
#define	K4Up	(key&0x9f)==0x94			//-键

#define	KDn		(key&0x0f)					//键按下
#define	Klong	(key&0x40)				    //键长按
#define	ClsKDn		key&=0xef				//清除键值
#define	ClsKlong	key&=0xbf				//清除长按键值		


//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0x44
#define REMOTE_ID 0x44      		   
//我们选用的遥控器键码
//#define K1v 0xAF      		   
//#define K2v 0x0F      		   
//#define K3v 0xD7      		   
//#define K4v 0xB7      		   

#define K1v 0x50      		   
#define K2v 0xF0      		   
#define K3v 0x28      		   
#define K4v 0x48      		   

#define RKup key&0x20      		   
#define RKdn key&0x0f      		   
#define RKLdn key&0x4f      		   

extern u8 RmTCnt;		//按键按下的次数	  
extern u8 key;		
extern u8 KeyHoldStus;	//红外遥控按键判断值	
//extern u8 KeyHoldStus;		

void Remote_Init(void);    //红外传感器接收头引脚初始化
void Remote_Scan(void);	     

#endif















