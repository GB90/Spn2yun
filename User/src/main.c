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
	delay_init();	    	 	//��ʱ������ʼ��	  
	NVIC_Configuration();       //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ9600
 	Remote_Init();			    //IR��ʼ��
 	LED_Init();			     	//LED�˿ڳ�ʼ��
	KEY_Init();          		//��ʼ���밴�����ӵ�Ӳ���ӿ�
	GetDataFromFlash();			//�ָ��Ѿ����������
    LCD_Init();          		//��ʼ��LCD(160160)
	T_Adc_Init();
	TIM2_Int_Init(99,7199);		//10ms  
	TIM3_PWM_Init(4095,150);	//PWMƵ��=100hz
	TIM1_Int_Init(1000,360);
	IO_Init();
	IWDG_init();				//���Ź���ʼ��
	//GetDataFromFlash();		//�ָ��Ѿ����������
	Display_Init();
	clear_screen();
	LCD_Back = 0;
	ModBusDataInit();               //��ʼ��WMbDataֵ
    //TIM_SetCompare1(TIM2,LCDBACK);
 	while(1)
	{
		IWDG_feed();		    //ι��
        Sample();               //����
		Remote_Scan();          //����ң����
		KEY_Scan(1);
        ModBusRun();
        IWDG_feed();		    //ι��
		Display();
		Output();
	}
 }

