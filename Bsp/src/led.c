#include "led.h"
	   

/*********************************************************
function name: LED IO��ʼ��

fun	:		��ʼ��PC8,PC9,PA11,PA12Ϊ�����(��ӦLED1,2,3,4).��ʹ���������ڵ�ʱ��	
*********************************************************/
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PC�˿�ʱ��
/*	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED1-->PC.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.8
 GPIO_SetBits(GPIOC,GPIO_Pin_8);						 //PC.8 �����
*/
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED2-->PC.9 �˿�����
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.9
 GPIO_SetBits(GPIOC,GPIO_Pin_9);						 //PC.9 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	    			 //LED3-->PA.11 �˿�����, �������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 
 GPIO_SetBits(GPIOA,GPIO_Pin_11); 						 //PA.11 ����� 

 /*
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	    		 	//LED4-->PA.12 �˿�����, �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_12); 						 //PA.12 ����� 
*/

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//LED4-->PA.15 �˿�����, �������
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
GPIO_Init(GPIOA, &GPIO_InitStructure);					
GPIO_SetBits(GPIOA,GPIO_Pin_15);						//PA.15 ����� 

}

 
