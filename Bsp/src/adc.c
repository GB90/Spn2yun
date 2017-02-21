#include "Def.h"
#include "adc.h"

u32 temp_val[10];
u32 temp_val1[10];
u16 AvVal[10];
u8 adch=0;
u8 adctimes=0;
u16 LRtime[10];			//堵转时间
s16 CPUtemp;            //CPU温度
u16 adcx;
//u16 RealTimeFKVal, RealTimeSDVal, RealTimeINVal;
/*********************************************************
function name: ADC初始化函数
fun	:		初始化ADC
			这里仅以规则通道为例，默认将开启通道0~3
*********************************************************/
void T_Adc_Init(void)  //ADC通道初始化
{
	u8	i;
	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   								//72M/6=12,ADC最大时间不能超过14M
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   								//72M/8=9,ADC最大时间不能超过14M

	//PA0/1/2/3 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

   	ADC_DeInit(ADC1);  												//将外设 ADC1 的全部寄存器重设为缺省值
	
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);									//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

	ADC_TempSensorVrefintCmd(ENABLE);
	
 
	ADC_Cmd(ADC1, ENABLE);											//使能指定的ADC1

	ADC_ResetCalibration(ADC1);										//重置指定的ADC1的校准寄存器

     while(ADC_GetResetCalibrationStatus(ADC1));							//获取ADC1重置校准寄存器的状态,设置状态则等待

	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));								//获取指定ADC1的校准程序,设置状态则等待

	for(i=0;i<11;i++)
	{
		temp_val[i]=0;
		AvVal[i]=0;
	}
}

/*********************************************************
function name: ADC采样

fun	:		采样每个通道的数据
*********************************************************/
u16 T_Get_Adc(u8 ch)   
{
 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);							//使能指定的ADC1的软件转换启动功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));					//等待转换结束
	return ADC_GetConversionValue(ADC1);								//返回最近一次ADC1规则组的转换结果
}


/*********************************************************
function name: ADC采样芯片温度

fun	:		得到ADC采样内部温度传感器的值，取10次,然后平均
			T（℃）（℃）={（V25-Vsense）/Avg_Slope}+25=(1.43-ad*3.3/4096)/0.0043
			V25=1.43,Avg_Slope=4.3mV/C
*********************************************************/
u16 T_Get_Temp(void)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<10;t++)
	{
		temp_val+=T_Get_Adc(ADC_Channel_16);						  //TampSensor
		delay_ms(5);
	}
	return temp_val/10;
}


/*********************************************************
function name: ADC采样芯片温度

fun	:		 获取通道ch的转换值
			取times次,然后平均
*********************************************************/
u16 T_Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=T_Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	   
/********************************************************
function name: 获得所有模拟量===>AvVal[x]    //[0]~[3]:AD通道0~3(板上RT电压值WD，反馈电压值FK_IN，0~5V电压SD_ADC，
			40~20mA转换电压IN_ADC_U)电压平均值,[4]:内部温度传感器温度值扩大10倍,[5]:内部的参照电压测量值(参考电压约3.3V)
fun	:输出:	AvVal[0]:板上RT电压值WD电压平均值
			AvVal[1]:反馈电压值FK_IN电压平均值
			AvVal[2]: 0~5V电压SD_ADC电压平均值
			AvVal[3]: 40~20mA转换电压IN_ADC_U电压平均值
			AvVal[4]: 内部温度传感器电压平均值
			AvVal[5]: 内部的1.2V参照电压测量值(参考电压约3.3V)
			AvVal[6]: 内部温度传感器温度平均值(扩大了10倍)
			AvVal[7]: test Input
			AvVal[8]: test Out
			AvVal[9]: -
*******************************************************/
void Get_Adc_Average(void)
{
	u8 i;	
	
	if(adch==_IT)
	{
		temp_val[adch]+=T_Get_Adc(ADC_Channel_16);						//内部温度传感器对应电压的AD值
	}
	else if(adch==_IV)
	{
		adcx=T_Get_Adc(ADC_Channel_17); 									//内部的参照电压测量值(参考电压约3.3V)
		temp_val[adch]+=adcx;												
	}
	else
	{
			temp_val[adch]+=T_Get_Adc(adch);
			temp_val1[adch]=T_Get_Adc(adch);
			if((temp_val1[adch]-temp_val[adch])<5)			//限幅滤波法
			{
				temp_val[adch]=temp_val1[adch];
			}
	}
	
	adctimes++;
    
	if(adctimes >= _adavtimes)
	{
		adctimes=0;
        //AdcTemp = 0;
//		AvVal[adcx]=temp_val[adcx]/_adavtimes;
//		if(adch==_IT)
//		{
//			AvVal[_IT_C] = (1430-(1200.0*AvVal[_IT]/adcx))/4.5 + 250;		//内部温度值扩大10倍-->AvVal[_IT_C]
//            //AvVal[_IT_C]=(0x6E2- ADC_ConvertedValue_Bak) / 0x05 + 25;		//内部温度值扩大10倍-->AvVal[_IT_C]
//		}
        AvVal[adch]=temp_val[adch]/_adavtimes;
		if(adch==_IT)
		{
			//AvVal[_IT_C]=(1430-(1200*AvVal[_IT]/adcx))*10/4.5 + 250;
            //CPUtemp = (1430-(3300*AvVal[_IT]/adcx))*10/4.5 + 250;   
            CPUtemp=(0x6E2- AvVal[_IT]) / 0.517 + 250;
		}
		adch++;
		if(adch>=_IT_C)
		{
			adch=0;
			//清零
			for(i=0;i<_IT_C;i++)
			{
				temp_val[i]=0;
			}
		}
	}
}		
