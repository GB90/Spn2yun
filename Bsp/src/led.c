#include "led.h"
	   

/*********************************************************
function name: LED IO初始化

fun	:		初始化PC8,PC9,PA11,PA12为输出口(对应LED1,2,3,4).并使能这两个口的时钟	
*********************************************************/
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PC端口时钟
/*	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED1-->PC.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.8
 GPIO_SetBits(GPIOC,GPIO_Pin_8);						 //PC.8 输出高
*/
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED2-->PC.9 端口配置
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.9
 GPIO_SetBits(GPIOC,GPIO_Pin_9);						 //PC.9 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    			 //LED3-->PA.11 端口配置, 推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 
 GPIO_SetBits(GPIOA,GPIO_Pin_11); 						 //PA.11 输出高 

 /*
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 	//LED4-->PA.12 端口配置, 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_12); 						 //PA.12 输出高 
*/

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//LED4-->PA.15 端口配置, 推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
GPIO_Init(GPIOA, &GPIO_InitStructure);					
GPIO_SetBits(GPIOA,GPIO_Pin_15);						//PA.15 输出高 

}

 
