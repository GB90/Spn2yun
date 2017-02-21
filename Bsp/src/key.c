#include "Def.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"


u16 keybuf0=0;								    
u16 keybuf1=0;								    
u16 keybuf2=0;								    

u8 KeyHoldStusA=0;		//���ⰴ���ж�ֵ

//��ֵ�Ĵ���
u16 KeyDn;								    
u16 KeyUp;								    

u16 KTCnt=0; 	//
u8 key_up=1;//�������ɿ���־

/*********************************************************
function name: ������ʼ������

fun	:	
*********************************************************/
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY1-->GPIOD.9, KEY2-->GPIOD.8, KEY3-->GPIOD.10, KEY4-->GPIOD.7  ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //ʹ��PD,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//GPIOD 7,8,9,10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD 7,8,9,10

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
    //	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    //	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
    //	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

	//��ֵ�Ĵ���
	KeyDn=0;									
	KeyUp=0;									


}


/*********************************************************
function name: ����������

fun	:		���ذ���ֵ  mode:0,��֧��������;1,֧��������;
			0��û���κΰ�������
			1��KEY1����
			2��KEY2����
			3��KEY3���� 
			4��KEY4����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
*********************************************************/
void KEY_Scan(u8 mode)
{
    if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
//		delay_ms(10);//ȥ���� 
		if(KTCnt<65535)KTCnt++;//zhaoxw �����壿
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
				key|=0x20;				//һֱ�ڰ���
			}
			if(KTCnt>=50)
			{
				key|=0x40;				//��������
			}
		}
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	{
		key_up=1; 	
        key|=0x80;				        //����̧��     
		KTCnt=0;
		KeyHoldStusA=0;
	}
}


