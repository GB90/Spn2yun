#include "Def.h"

#define	_LongKeyTime	10

u8 key=0;		
u8 KeyHoldStus=0;		//����ң�ذ����ж�ֵ	
u8 sta_old;		

//����ң�س�ʼ��
//����IO�Լ���ʱ��4�����벶��
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE); //ʹ��PORTBʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//TIM4 ʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PB9 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//�������� 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_9);	//��ʼ��GPIOB.9
/* 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PE0 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//���� 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOE,GPIO_Pin_0);	//��ʼ��GPIOE.0
*/					  
 	TIM_TimeBaseStructure.TIM_Period = 10000; //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx

  	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // ѡ������� IC4ӳ�䵽TI4��
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM4, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��

   	TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	

	TIM_ITConfig( TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE);//��������ж� ,����CC4IE�����ж�								 

	sta_old=0;
}

//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 	RmtSta=0;	  	  
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec=0;	//������յ�������	   		    
u8  RmTCnt=0;	//�������µĴ���	  
u8  KeyCnt=0;	//�������µĴ���	  
//��ʱ��2�жϷ������	 
void TIM4_IRQHandler(void)
{ 		    	 
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);//���������ʶ
				RmtSta&=0XF0;	//��ռ�����	
			}						 	   	
		}							    
	}
 	if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
	{	  
		if(RDATA)//�����ز���
		{

			TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���				
	    	TIM_SetCounter(TIM4,0);	   	//��ն�ʱ��ֵ
			RmtSta|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{			
  			 Dval=TIM_GetCapture4(TIM4);//��ȡCCR1Ҳ������CC1IF��־λ
			 TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC4P=0	����Ϊ�����ز���
 			
			if(RmtSta&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta&0X80)//���յ���������
				{
					
					if(Dval>300&&Dval<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec<<=1;	//����һλ.
						RmtRec|=0;	//���յ�0	   
					}else if(Dval>1400&&Dval<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec<<=1;	//����һλ.
						RmtRec|=1;	//���յ�1
					}else if(Dval>2200&&Dval<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
//						RmTCnt++; 		//������������1��
						if(RmTCnt<0xff)RmTCnt++; 		//������������1��
						RmtSta&=0XF0;	//��ռ�ʱ��		
					}
 				}
				else if(Dval>4200&&Dval<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta|=1<<7;	//��ǳɹ����յ���������
					RmTCnt=0;		//�����������������
				}						 
			}
			RmtSta&=~(1<<4);
		}				 		     	    					   
	}
 TIM_ClearFlag(TIM4,TIM_IT_Update|TIM_IT_CC4);	    
}

/***************************************************
����������
���keyֵ:
	 
key = 0LHS kkkk

S         :�հ���(�հ���ĳ��)
L         :������
H         :һֱ�ڰ���
kkkk      :��ֵ,Ϊ0000,û���κΰ�������
0000 xxxx :0LHS=0000���ſ�
***************************************************/
void Remote_Scan(void)
{        
	u8 sta=0;       
    u8 t1,t2,t3,t4;  
	if(RmtSta&(1<<6))					//�õ�һ��������������Ϣ��
	{ 
	    t1=RmtRec>>24;					//�õ���ַ��
	    t2=(RmtRec>>16)&0xff;			//�õ���ַ���� 
	    t3=(RmtRec>>8)&0xff;				//�õ�Key���� 
	    t4= RmtRec &0xff;					//�õ�Key
// 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)	//����ң��ʶ����(ID)����ַ 
 	    if((t1==(u8)~t2) && (t3 == (u8)~t4))//����ң����
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)
				{
				sta=t2;					//��ֵ��ȷ
				if(KeyHoldStus==0)
					{
					KeyHoldStus=1;
//					KeyCnt=RmTCnt=0;
					switch(t1&0xff)
						{
						case 0xb0:
						case K1v:
							key=0x11;
							break;		 
						case 0x30:
						case K2v:
							key=0x12;
							break;		 
						case 0xa8:
						case K3v:
							key=0x13;
							break;		 
						case 0x00:
						case K4v:
							key=0x14;
							break;		 
						case 0x18:
							key=0x15;
							break;		 
						}
					}
                    else
                    {
                        key|=0x20;				//һֱ�ڰ���
                    }
                    KeyCnt=RmTCnt;
                    if(KeyCnt>=_LongKeyTime)
                    {
                        key|=0x40;				//��������
                    }
	        	}
		}   
		if((sta==0)||((RmtSta&0X80)==0))		//�������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta&=~(1<<6);				    //������յ���Ч������ʶ
			RmTCnt=0;						    //�����������������
			RmtRec=0;
//			key=0;//��ֵ0
			key&=0x0f;//��ֵ
//            key|=0x80;                          //��������
			KeyHoldStus=0;
		}
	}  

}































