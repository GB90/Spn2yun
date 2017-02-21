#include "Def.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


u16 keybuf0=0;								    
u16 keybuf1=0;								    
u16 keybuf2=0;								    

u8 KeyHoldStusA=0;		//开光按键判断值

//键值寄存器
u16 KeyDn;								    
u16 KeyUp;								    

u16 KTCnt=0; 	//
u8 key_up=1;//按键按松开标志

/*********************************************************
function name: 按键初始化函数

fun	:	
*********************************************************/
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化KEY1-->GPIOD.9, KEY2-->GPIOD.8, KEY3-->GPIOD.10, KEY4-->GPIOD.7  上拉输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //使能PD,PE端口时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//GPIOD 7,8,9,10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD 7,8,9,10

	//初始化 WK_UP-->GPIOA.0	  下拉输入
    //	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    //	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
    //	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

	//键值寄存器
	KeyDn=0;									
	KeyUp=0;									


}


/*********************************************************
function name: 按键处理函数

fun	:		返回按键值  mode:0,不支持连续按;1,支持连续按;
			0，没有任何按键按下
			1，KEY1按下
			2，KEY2按下
			3，KEY3按下 
			4，KEY4按下
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4!!
*********************************************************/
void KEY_Scan(u8 mode)
{
    if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
//		delay_ms(10);//去抖动 
		if(KTCnt<65535)KTCnt++;//zhaoxw 有意义？
		if(KTCnt>=3)
		{
			if(KeyHoldStusA==0)
			{
				key_up=0;
				KeyHoldStusA=1;
				if(KEY1==0)key = 0x14;
				else if(KEY2==0)key =  0x13;
				else if(KEY3==0)key =  0x12;
				else if(KEY4==0)key =  0x11;
			}
			else
			{
				key|=0x20;				//一直在按键
			}
			if(KTCnt>=50)
			{
				key|=0x40;				//按键长按
			}
		}
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	{
		key_up=1; 	
        key|=0x80;				        //按键抬起     
		KTCnt=0;
		KeyHoldStusA=0;
	}
}


