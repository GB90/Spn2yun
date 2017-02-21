/***************************************************
*MDK517
*Keil.STM32F1xx_DFP.2.1.0.pack
*STM32F103VET6
*OSC:8MHZ
****************************************************/

#include "Def.h"
#include "math.h"
#include "Stmflash.h" 

u16 AdInTemp=0;
u16 AdTemp=0;
u16 AdTemp1=0;
u8 KeyTemp=0;
u8 PBStus;

OutputPortStus POStus;
InputPortStus PIStus;
UTIMER uTimer;

int main(void)
{	
	delay_init();	    	 	//延时函数初始化	  
	NVIC_Configuration();       //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为9600
 	Remote_Init();			    //IR初始化
 	LED_Init();			     	//LED端口初始化
	KEY_Init();          		//初始化与按键连接的硬件接口
	GetDataFromFlash();			//恢复已经保存的数据
    LCD_Init();          		//初始化LCD(160160)
	T_Adc_Init();
	TIM2_Int_Init(99,7199);		//10ms  
	TIM3_PWM_Init(4095,150);	//PWM频率=100hz
	TIM1_Int_Init(1000,360);
	IO_Init();
	IWDG_init();				//看门狗初始化
	//GetDataFromFlash();		//恢复已经保存的数据
	Display_Init();
	clear_screen();
	LCD_Back = 0;
	ModBusDataInit();               //初始化WMbData值
    //TIM_SetCompare1(TIM2,LCDBACK);
 	while(1)
	{
		IWDG_feed();		    //喂狗
        Sample();               //采样
		Remote_Scan();          //红外遥控器
		KEY_Scan(1);
        ModBusRun();
        IWDG_feed();		    //喂狗
		Display();
		Output();
	}
 }

