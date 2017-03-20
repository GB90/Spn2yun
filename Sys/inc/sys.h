#ifndef __SYS_H
#define __SYS_H	

#include "stm32f10x.h"

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_UCOS		0		//定义系统文件夹是否支持UCOS
																	    
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

typedef struct
{
	u16 end;
	u16 strat;
	u16 now;
}DevicePos;

typedef struct
{
	//对开关量:零表示输出0,非零表示输出1
	u16 FK_IN_Pers;		//反馈输入电压//换算成相对值%(最高位为符号位)
	u16 VMA_IN_Pers;	//输入电压或电流//换算成相对值%(最高位为符号位)
	u16 FK_IN;			//反馈输入电压//模拟量,相对值，AD计数值
	u16 VMA_IN;			//输入电压或电流//模拟量,相对值，AD计数值
	u16 BM1;			//开关量，编码器
	u16 BM2;			//开关量，编码器
	u16 BM3;			//开关量，编码器
	u16 OTL;			//开关量，过热检测
	u16 CTS;			//开关量，闭力矩检测
	u16 OTS;			//开关量，开力矩检测
	u16 ACLS;			//开关量，关辅助限位	
	u16 AOLS;			//开关量，开辅助限位	
	u16 IN_RCL;			//开关量，远程 / 本地
	u16 IN_ESD;			//开关量，远程ESD	
	u16 IN_BC;			//开关量，远程保持
    u16 PHASE;		    //开关量，相序
    u16 PHASE_LOST;		//开关量，缺相
	u16 IN_SHUT;		//开关量，远程关
	u16 IN_OPEN;		//开关量，远程开
}InputPortStus;

typedef struct
{
	//以开关量低8位非零表示输出1，零表示输出0；高8位表示输出到的通道编号。如MRTS=0x0300，表示从3通道输出0
	u16 MD;				//开关量， 中间位置输出
	u16 SHUTRUN;		//开关量， 关运行输出
	u16 OPENRUN;		//开关量， 开运行输出	
	u16 RCL;			//开关量， 远程本地输出	
	u16 CTSOUT;			//开关量， 闭力矩输出		
 	u16 OTSOUT;			//开关量， 开力矩输出		
 	u16 ACLSOUT;		//开关量， 关辅助限位输出	
	u16 AOLSOUT;		//开关量， 开辅助限位输出			
	u16 AL;				//开关量， 故障报警输出		
	u16 M_SW;			//开关量， 电机正转输出
	u16 M_OS;			//开关量， 电机反转输出	
	u16 CV_IO;			//开关量， 电压/电流输出模式转换
	u16 MA_OUT;		    //数字量，输出电流的表达值(输出电流4mA~20mA对应的PWM占空比值)
	u16 V_OUT;			//数字量，输出电压的表达值(输出电压0V~10V对应的PWM占空比值)
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

