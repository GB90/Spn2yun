#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
	 
#define ADC_CH_TEMP  	ADC_Channel_16 		//温度传感器通道

//求平均值的次数定义
#define	_adavtimes	1

//输入量代号
#define _WD			0						//温度
#define _FK			1						//反馈
#define _SD			2						//给定电流
#define _IN			3						//给定电压
#define _IT			4						//内部温度AD
#define _IV			5						//内部参照电压
#define _IT_C		6						//内部温度(度)
//[0]~[3]:AD通道0~3(板上RT电压值WD，反馈电压值FK_IN，0~5V电压SD_ADC，
//40~20mA转换电压IN_ADC_U)电压平均值,[4]:内部温度传感器温度AD值,
//[5]:内部的参照电压测量值(1.2V标准电压,参考电压约3.3V),[6]:内部温度传感器温度值(度C)扩大10倍
extern u32 temp_val[10];
extern u16 AvVal[10];					
extern u16 adcx;								//内部参考电压AD值
extern u16 LRtime[10];							//堵转时间
extern s16 CPUtemp;                             //CPU温度
//extern u16 RealTimeFKVal, RealTimeSDVal, RealTimeINVal;
   
u16  T_Get_Temp(void);  					//取得温度值
void T_Adc_Init(void); 						//ADC通道初始化
u16  T_Get_Adc(u8 ch); 					//获得某个通道值  
u16  T_Get_Adc_Average(u8 ch,u8 times);	//得到某个通道10次采样的平均值 	  
void  Get_Adc_Average(void);				//得到0~3通道10次采样的平均值 	  
#endif 

