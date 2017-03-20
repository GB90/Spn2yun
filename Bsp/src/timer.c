#include "Def.h"
#include "timer.h"


u8 Fsh500mS;
u32 T10mS;
u16 PWMDuty;
u16 locktime;														//堵转计时
u16 skidstime;													//刹车时间

/*********************************************************
function name: 通用定时器中断初始化

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
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)				 //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  				//清除TIMx的中断待处理位:TIM 中断源 
        MbTimer++;
        if(MbTimer > MB_TIMEOUT)
        {
            MbTimer = MB_TIMEOUT;
        }
	}
}

///*********************************************************
//function name: 通用定时器中断初始化

//fun	:		这里时钟选择为APB1的2倍，而APB1为36M
//			arr：自动重装值。
//			psc：时钟预分频数
//			这里使用的是定时器3!
//*********************************************************/
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		    //时钟使能

//	TIM_TimeBaseStructure.TIM_Period = arr; 						//自动重装载周期值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//预分频值  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				    //根据指定的参数初始化TIMx
// 
//	TIM_ITConfig(  TIM3,TIM_IT_Update  |  TIM_IT_Trigger,  ENABLE  );	//使能更新触发TIM中断

//	//中断分组设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				//TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  			//先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  								//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM_Cmd(TIM3, ENABLE); 										 //使能TIMx外设
//							 
//}

//void TIM3_IRQHandler(void)   //TIM3中断
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)				 //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  				//清除TIMx的中断待处理位:TIM 中断源 
//		LED1=!LED1;
//        MbTimer++;
//        if(MbTimer > MB_TIMEOUT)
//        {
//            MbTimer = MB_TIMEOUT;
//        }
//	}
//}

/*********************************************************
function name: 通用定时器2中断初始化

fun	:		这里时钟选择为APB1的2倍，而APB1为36M
			arr：自动重装值。
			psc：时钟预分频数
			这里使用的是定时器3!
*********************************************************/
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;    
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA,PC端口时钟
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    //设置该引脚为复用输出功能,输出TIM2 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //CH1 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 100; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1 预装载使能
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器///
	TIM_SetCompare1(TIM2,LCDBACK);

    TIM_Cmd(TIM2, ENABLE);  //使能TIMx
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
function name: PWM输出初始化

fun	:		arr：自动重装值
			psc：时钟预分频数
*********************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 		//Timer3部分重映射  TIM3_CH3->PC8						//TIM3的CH3输出的PWM
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); 		//Timer3完全重映射  TIM3_CH3->PC8                      //TIM3的CH3输出的PWM
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
//*
	TIM_TimeBaseStructure.TIM_Period = arr; 						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		//选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 							//设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  					//根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  		//使能TIMx在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 						//使能TIMx在ARR上的预装载寄存器
	
 
	TIM_Cmd(TIM3, ENABLE); 									 //使能TIMx外设
//*/

/*
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能    
	RCC->APB2ENR|=1<<4; 	  //IO端口PC使能

GPIOC->CRL&=0XFF00FF00;//PB0,1,4,5输出
	GPIOC->CRL|=0X00BB00BB;//复用功能输出 	  
	GPIOC->ODR|= 0X33;//IO口上拉

	RCC->APB2ENR|=1<<0;        //开启AF辅助时钟 
	AFIO->MAPR&=0XFFFFF3FF;    //清除 MAPR 的[11:10]
// 	AFIO->MAPR|=2<<10;         //部分重映像,TIM3_CH2->PB5	
 	AFIO->MAPR|=2<<11;         //部分重映像,TIM3_CH3->PC8	

	TIM3->ARR=arr;//设定计数器自动重装值 
	TIM3->PSC=psc;//预分频器不分频
	
	TIM3->CCMR1|=6<<4;  	//CH1 PWM2模式		 CCR1
	TIM3->CCMR1|=1<<3; 		//CH1预装载使能
	TIM3->CCMR1|=6<<12; 	//CH2 PWM2模式		 
	TIM3->CCMR1|=1<<11; 	//CH2预装载使能
	TIM3->CCMR2|=6<<4;  	//CH3 PWM2模式		 
	TIM3->CCMR2|=1<<3; 		//CH3预装载使能
	TIM3->CCMR2|=6<<12;  	//CH4 PWM2模式		 
	TIM3->CCMR2|=1<<11; 	//CH4预装载使能	 

TIM3->CCER|=1<<0;   //OC1 输出使能
	TIM3->CCER|=1<<4;   //OC2 输出使能	
	TIM3->CCER|=1<<8;   //OC3 输出使能	
	TIM3->CCER|=1<<12;  //OC4 输出使能		   

//	TIM3->CR1=0x8000;   //ARPE使能 
	TIM3->CR1|=0x01;    //使能定时器3										  


	TIM3->CCR3 = 300;

	RCC->APB1ENR|=1<<1;//TIM3时钟使能 
	TIM3->CR1|=1<<4;//向下计数 
//	TIM3->CCMR1=0x6868;//通道1、2PWM模式1,预装载使能 
	TIM3->CCMR2=0x6868;//通道3、4PWM模式1,预装载使能 

	TIM3->CCR1=800; 
	TIM3->CCR2=400; 
	TIM3->CCR3=200; 
	TIM3->CCR4=400; 
	 	TIM3->ARR=1000;  //设定计数器自动重装值    
	TIM3->PSC=72;  //预分频器72,得到1Mhz的计数时钟 
	TIM3->CCER=0X1111;//打开四个通道，允许输出，有效电平为高 
	TIM3->CR1|=1<<7; 
	TIM3->EGR|=1; 
	TIM3->CR1|=0x01;    //使能定时器3 
*/




}


TIM_ICInitTypeDef  TIM5_ICInitStructure;


/*********************************************************
function name: 定时器5通道1输入捕获配置

fun	:		arr：自动重装值
			psc：时钟预分频数
*********************************************************/
void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
		 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器,1M的计数频率,1us加1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波

  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
   	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
   	TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	


	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
}

u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值
 
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); 
 
}
/*
//TIM4 CH1 PWM输出设置 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM_DeInit(TIM4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIMx外设
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOB外设时钟使能
	
 
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //CH1 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1 预装载使能
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器///
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx
	
	TIM_SetCompare3(TIM4,80);		   
	   										  
} 
*/


/*********************************************************
function name: TIM4 CH3 PWM输出设置 

fun	:		PWM输出初始化
			arr：自动重装值
			psc：时钟预分频数
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
	
//	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器///
	
	TIM_Cmd(TIM4,ENABLE);
}


