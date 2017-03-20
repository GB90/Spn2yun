#include "Def.h"
#include "timer.h"


u8 Fsh500mS;
u32 T10mS;
u16 PWMDuty;
u16 locktime;														//��ת��ʱ
u16 skidstime;													//ɲ��ʱ��

/*********************************************************
function name: ͨ�ö�ʱ���жϳ�ʼ��

fun	:	
*********************************************************/
void TIM1_Int_Init(u16 arr,u16 psc)  
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Period = arr;
  TIM_TimeBaseStructure.TIM_Prescaler =(psc-1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  
  TIM_ITConfig( TIM1, TIM_IT_Update,ENABLE);  

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);   
  
  TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)				 //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  				//���TIMx���жϴ�����λ:TIM �ж�Դ 
        MbTimer++;
        if(MbTimer > MB_TIMEOUT)
        {
            MbTimer = MB_TIMEOUT;
        }
	}
}

///*********************************************************
//function name: ͨ�ö�ʱ���жϳ�ʼ��

//fun	:		����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//			arr���Զ���װֵ��
//			psc��ʱ��Ԥ��Ƶ��
//			����ʹ�õ��Ƕ�ʱ��3!
//*********************************************************/
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		    //ʱ��ʹ��

//	TIM_TimeBaseStructure.TIM_Period = arr; 						//�Զ���װ������ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//Ԥ��Ƶֵ  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				    //����ָ���Ĳ�����ʼ��TIMx
// 
//	TIM_ITConfig(  TIM3,TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE  );	//ʹ�ܸ��´���TIM�ж�

//	//�жϷ�������
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				//TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  			//��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  								//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM_Cmd(TIM3, ENABLE); 										 //ʹ��TIMx����
//							 
//}

//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)				 //���ָ����TIM�жϷ������:TIM �ж�Դ 
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  				//���TIMx���жϴ�����λ:TIM �ж�Դ 
//		LED1=!LED1;
//        MbTimer++;
//        if(MbTimer > MB_TIMEOUT)
//        {
//            MbTimer = MB_TIMEOUT;
//        }
//	}
//}

/*********************************************************
function name: ͨ�ö�ʱ��2�жϳ�ʼ��

fun	:		����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
			arr���Զ���װֵ��
			psc��ʱ��Ԥ��Ƶ��
			����ʹ�õ��Ƕ�ʱ��3!
*********************************************************/
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;    
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA,PC�˿�ʱ��
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    //���ø�����Ϊ�����������,���TIM2 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //CH1 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 100; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 �͵�ƽ��Ч 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1 Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���///
	TIM_SetCompare1(TIM2,LCDBACK);

    TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
void TIM2_IRQHandler(void)   
{
    static u8 numms = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
        
        if(WaitMoto++ > 200)
        {   
            WaitMoto = 210;
        }
        if(numms++ > 10)
        {
            numms = 0;
            T10mS+=1;
            InputTimer --;
            if(InputTimer > 10000)
            {
                InputTimer = 0;
            }
            if(T10mS & 0x00000020)
            {
                skidstime++;
                Fsh500mS=1;		
                locktime++;
                if(locktime>LockedRotorTime*10)
                {
                    locktime=0;
                }
            }
            else
            {
                Fsh500mS=0;
            }
        }        
	}
}



/*********************************************************
function name: PWM�����ʼ��

fun	:		arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
*********************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 		//Timer3������ӳ��  TIM3_CH3->PC8						//TIM3��CH3�����PWM
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); 		//Timer3��ȫ��ӳ��  TIM3_CH3->PC8                      //TIM3��CH3�����PWM
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
//*
	TIM_TimeBaseStructure.TIM_Period = arr; 						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 							//���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  					//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  		//ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 						//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
 
	TIM_Cmd(TIM3, ENABLE); 									 //ʹ��TIMx����
//*/

/*
	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��    
	RCC->APB2ENR|=1<<4; 	  //IO�˿�PCʹ��

GPIOC->CRL&=0XFF00FF00;//PB0,1,4,5���
	GPIOC->CRL|=0X00BB00BB;//���ù������ 	  
	GPIOC->ODR|= 0X33;//IO������

	RCC->APB2ENR|=1<<0;        //����AF����ʱ�� 
	AFIO->MAPR&=0XFFFFF3FF;    //��� MAPR ��[11:10]
// 	AFIO->MAPR|=2<<10;         //������ӳ��,TIM3_CH2->PB5	
 	AFIO->MAPR|=2<<11;         //������ӳ��,TIM3_CH3->PC8	

	TIM3->ARR=arr;//�趨�������Զ���װֵ 
	TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ
	
	TIM3->CCMR1|=6<<4;  	//CH1 PWM2ģʽ		 CCR1
	TIM3->CCMR1|=1<<3; 		//CH1Ԥװ��ʹ��
	TIM3->CCMR1|=6<<12; 	//CH2 PWM2ģʽ		 
	TIM3->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��
	TIM3->CCMR2|=6<<4;  	//CH3 PWM2ģʽ		 
	TIM3->CCMR2|=1<<3; 		//CH3Ԥװ��ʹ��
	TIM3->CCMR2|=6<<12;  	//CH4 PWM2ģʽ		 
	TIM3->CCMR2|=1<<11; 	//CH4Ԥװ��ʹ��	 

TIM3->CCER|=1<<0;   //OC1 ���ʹ��
	TIM3->CCER|=1<<4;   //OC2 ���ʹ��	
	TIM3->CCER|=1<<8;   //OC3 ���ʹ��	
	TIM3->CCER|=1<<12;  //OC4 ���ʹ��		   

//	TIM3->CR1=0x8000;   //ARPEʹ�� 
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3										  


	TIM3->CCR3 = 300;

	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ�� 
	TIM3->CR1|=1<<4;//���¼��� 
//	TIM3->CCMR1=0x6868;//ͨ��1��2PWMģʽ1,Ԥװ��ʹ�� 
	TIM3->CCMR2=0x6868;//ͨ��3��4PWMģʽ1,Ԥװ��ʹ�� 

	TIM3->CCR1=800; 
	TIM3->CCR2=400; 
	TIM3->CCR3=200; 
	TIM3->CCR4=400; 
	 	TIM3->ARR=1000;  //�趨�������Զ���װֵ    
	TIM3->PSC=72;  //Ԥ��Ƶ��72,�õ�1Mhz�ļ���ʱ�� 
	TIM3->CCER=0X1111;//���ĸ�ͨ���������������Ч��ƽΪ�� 
	TIM3->CR1|=1<<7; 
	TIM3->EGR|=1; 
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3 
*/




}


TIM_ICInitTypeDef  TIM5_ICInitStructure;


/*********************************************************
function name: ��ʱ��5ͨ��1���벶������

fun	:		arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
*********************************************************/
void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
		 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�

  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
   	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
   	TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	


	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
}

u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
 
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); 
 
}
/*
//TIM4 CH1 PWM������� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_DeInit(TIM4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIMx����
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOB����ʱ��ʹ��
	
 
   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���ù������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //CH1 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 �͵�ƽ��Ч 
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1 Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���///
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
	
	TIM_SetCompare3(TIM4,80);		   
	   										  
} 
*/


/*********************************************************
function name: TIM4 CH3 PWM������� 

fun	:		PWM�����ʼ��
			arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
*********************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_DeInit(TIM4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	TIM_TimeBaseInitStructure.TIM_Period=arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0x00;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
//	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���///
	
	TIM_Cmd(TIM4,ENABLE);
}


