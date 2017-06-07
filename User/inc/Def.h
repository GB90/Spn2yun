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

//#define __Debug__                                         //测试选项
#define LCDBACK         40                                  //背光亮度0~99  10% 约 7mA
#define DISTEMP         0                                   //温度显示          0:不显示        1:显示
#define SOFTLOCK        0                                   //软件锁            0:无效          1:有效
#define MB_MODE         1                                   //modbus模式        0:读写          1:只读 2:无
#define MB_LOST         1                                   //modbus超时掉线    0:无超时        1:带超时 
#define POWER_MODE      1                                   //电源模式          0:220V          1:380V
#define POWER_MODE_DISP 1                                   //POWER_MODE = 1时 才显示相序状态  当前相序与行程设置时候的相序比较

#define VERSION        ((const u8*)("V:0.1.41")) 			//版本号
#define DATE           ((const u8*)("2016/10/27"))			//日期
#define FLASH_SAVE_ADDR  0X08070000 		//
#define EN_INT          __enable_irq();     //系统开全局中断  
#define DIS_INT         __disable_irq();    //系统关全局中断  

//输入量代号
//#define P_BM1 		PAin(4)		//输入口，编码器
//#define P_BM2 		PAin(5)		//输入口，编码器
//#define P_BM3 		PAin(7)		//输入口，编码器
#define P_OTL 		PEin(7)		    //输入口，过热检测
#define P_CTS 		PBin(0)		    //输入口，闭力矩检测
#define P_OTS 		PCin(4)		    //输入口，开力矩检测
#define P_ACLS		PCin(3)		    //输入口，关辅助限位	
#define P_AOLS		PCin(1)		    //输入口，开辅助限位	
#define P_IN_RCL	PCin(0)		    //输入口，远程 自动/调节
#define P_IN_ESD	PCin(13)	    //输入口，远程ESD	
#define P_IN_BC		PEin(5)		    //输入口，远程保持
#define P_IN_SHUT 	PEin(4)		    //输入口，远程关
#define P_IN_OPEN 	PEin(3)		    //输入口，远程开
#define P_PHASE 	PBin(15)		//输入口，相序
#define P_PHASELOST PEin(2)		    //输入口，缺相

//#define P_MRTS		PBout(15)	//输出口1， 正在运行/过矩输出
//#define P_MD		    PBout(14)	//输出口2， 中间位置输出
//#define P_SHUTRUN 	PBout(13)	//输出口3， 关运行输出
//#define P_OPENRUN 	PBout(12)	//输出口4， 开运行输出	
//#define P_RCL 		PBout(11)	//输出口5， 远程本地输出 
//#define P_CTSOUT	    PBout(10)	//输出口6， 闭力矩输出		
//#define P_OTSOUT	    PBout(1)	//输出口7， 开力矩输出
//#define P_ACLSOUT 	PCout(5)	//输出口8， 关辅助限位输出	
//#define P_AOLSOUT 	PAout(6)	//输出口9， 开辅助限位输出			
//#define P_AL		    PCout(2)	//输出口10，故障报警输出 	
//#define P_M_SW	    PCout(6)	//输出口，  电机驱动输出 
//#define P_M_OS		PCout(7)	//输出口，  电机正反转输出	
//#define P_C_IO		PEout(6)	//输出口，  电压/电流输出模式转换


#define P_AL		    PCout(2)	//输出口10，故障报警输出 

#define P_AOLSOUT 	    PAout(6)	//输出口9， 开辅助限位输出		
#define P_ACLSOUT 	    PCout(5)	//输出口8， 关辅助限位输出		
#define P_OTSOUT	    PBout(1)	//输出口7， 开力矩输出
#define P_CTSOUT	    PBout(10)	//输出口6， 闭力矩输出	
	

//开关量输出通道1~10定义:
#define Po10		PBout(15)		//输出口10， 默认为正在运行/过矩输出
#define Po9		    PBout(14)		//输出口9，  默认为中间位置输出
#define Po8 		PBout(13)		//输出口8，  默认为关运行输出
#define Po7 		PBout(12)		//输出口7，  默认为开运行输出	
#define Po6 		PBout(11)		//输出口6，  默认为远程本地输出 
#define Po5		    PBout(10)		//输出口5，  默认为闭力矩输出		
#define Po4		    PBout(1)		//输出口4，  默认为开力矩输出
#define Po3 		PCout(5)		//输出口3，  默认为关辅助限位输出	
#define Po2 		PAout(6)		//输出口2，  默认为开辅助限位输出			
#define Po1		    PCout(2)		//输出口1，  默认为故障报警输出 	

#define P_M_SW		PCout(6)		//输出口，  电机驱动输出 
#define P_M_OS		PCout(7)		//输出口，  电机正反转输出	
#define P_C_IO		PEout(6)		//输出口，  电压/电流输出模式转换

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

/*******************错误所在bit*********************************/
#define ERROTS      BIT0            //开过矩
#define ERRCTS      BIT1            //关过矩
#define VMALOW      BIT2            //丢信
#define INTTHIGH    BIT3            //内部温度过高    
#define EXTTHIGH    BIT4            //外部温度过高(电机)
#define ERRDIR      BIT5            //转向错误
#define ERRCMD      BIT6            //开关同在
#define SHUTBLOCK   BIT7            //关堵转
#define OPENBLOCK   BIT8            //开堵转
#define PHASELOST   BIT9            //缺相
#define MBONLINE    BIT10           //ModBus 掉线
//#define _XianFan		3					//线反
//#define _ZhenDang		5					//震荡

typedef struct
{                                   //最大值
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

