#include "Def.h"
#include "adc.h"

u32 temp_val[10];
u32 temp_val1[10];
u16 AvVal[10];
u8 adch=0;
u8 adctimes=0;
u16 LRtime[10];			//��תʱ��
s16 CPUtemp;            //CPU�¶�
u16 adcx;
//u16 RealTimeFKVal, RealTimeSDVal, RealTimeINVal;
/*********************************************************
function name: ADC��ʼ������
fun	:		��ʼ��ADC
			������Թ���ͨ��Ϊ����Ĭ�Ͻ�����ͨ��0~3
*********************************************************/
void T_Adc_Init(void)  //ADCͨ����ʼ��
{
	u8	i;
	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   								//72M/6=12,ADC���ʱ�䲻�ܳ���14M
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   								//72M/8=9,ADC���ʱ�䲻�ܳ���14M

	//PA0/1/2/3 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

   	ADC_DeInit(ADC1);  												//������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);									//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	ADC_TempSensorVrefintCmd(ENABLE);
	
 
	ADC_Cmd(ADC1, ENABLE);											//ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1);										//����ָ����ADC1��У׼�Ĵ���

     while(ADC_GetResetCalibrationStatus(ADC1));							//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�

	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));								//��ȡָ��ADC1��У׼����,����״̬��ȴ�

	for(i=0;i<11;i++)
	{
		temp_val[i]=0;
		AvVal[i]=0;
	}
}

/*********************************************************
function name: ADC����

fun	:		����ÿ��ͨ��������
*********************************************************/
u16 T_Get_Adc(u8 ch)   
{
 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��3,�������˳��ֵΪ1,����ʱ��Ϊ239.5����	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);							//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));					//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);								//�������һ��ADC1�������ת�����
}


/*********************************************************
function name: ADC����оƬ�¶�

fun	:		�õ�ADC�����ڲ��¶ȴ�������ֵ��ȡ10��,Ȼ��ƽ��
			T���棩���棩={��V25-Vsense��/Avg_Slope}+25=(1.43-ad*3.3/4096)/0.0043
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
function name: ADC����оƬ�¶�

fun	:		 ��ȡͨ��ch��ת��ֵ
			ȡtimes��,Ȼ��ƽ��
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
function name: �������ģ����===>AvVal[x]    //[0]~[3]:ADͨ��0~3(����RT��ѹֵWD��������ѹֵFK_IN��0~5V��ѹSD_ADC��
			40~20mAת����ѹIN_ADC_U)��ѹƽ��ֵ,[4]:�ڲ��¶ȴ������¶�ֵ����10��,[5]:�ڲ��Ĳ��յ�ѹ����ֵ(�ο���ѹԼ3.3V)
fun	:���:	AvVal[0]:����RT��ѹֵWD��ѹƽ��ֵ
			AvVal[1]:������ѹֵFK_IN��ѹƽ��ֵ
			AvVal[2]: 0~5V��ѹSD_ADC��ѹƽ��ֵ
			AvVal[3]: 40~20mAת����ѹIN_ADC_U��ѹƽ��ֵ
			AvVal[4]: �ڲ��¶ȴ�������ѹƽ��ֵ
			AvVal[5]: �ڲ���1.2V���յ�ѹ����ֵ(�ο���ѹԼ3.3V)
			AvVal[6]: �ڲ��¶ȴ������¶�ƽ��ֵ(������10��)
			AvVal[7]: test Input
			AvVal[8]: test Out
			AvVal[9]: -
*******************************************************/
void Get_Adc_Average(void)
{
	u8 i;	
	
	if(adch==_IT)
	{
		temp_val[adch]+=T_Get_Adc(ADC_Channel_16);						//�ڲ��¶ȴ�������Ӧ��ѹ��ADֵ
	}
	else if(adch==_IV)
	{
		adcx=T_Get_Adc(ADC_Channel_17); 									//�ڲ��Ĳ��յ�ѹ����ֵ(�ο���ѹԼ3.3V)
		temp_val[adch]+=adcx;												
	}
	else
	{
			temp_val[adch]+=T_Get_Adc(adch);
			temp_val1[adch]=T_Get_Adc(adch);
			if((temp_val1[adch]-temp_val[adch])<5)			//�޷��˲���
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
//			AvVal[_IT_C] = (1430-(1200.0*AvVal[_IT]/adcx))/4.5 + 250;		//�ڲ��¶�ֵ����10��-->AvVal[_IT_C]
//            //AvVal[_IT_C]=(0x6E2- ADC_ConvertedValue_Bak) / 0x05 + 25;		//�ڲ��¶�ֵ����10��-->AvVal[_IT_C]
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
			//����
			for(i=0;i<_IT_C;i++)
			{
				temp_val[i]=0;
			}
		}
	}
}		
