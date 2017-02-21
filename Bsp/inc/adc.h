#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
	 
#define ADC_CH_TEMP  	ADC_Channel_16 		//�¶ȴ�����ͨ��

//��ƽ��ֵ�Ĵ�������
#define	_adavtimes	1

//����������
#define _WD			0						//�¶�
#define _FK			1						//����
#define _SD			2						//��������
#define _IN			3						//������ѹ
#define _IT			4						//�ڲ��¶�AD
#define _IV			5						//�ڲ����յ�ѹ
#define _IT_C		6						//�ڲ��¶�(��)
//[0]~[3]:ADͨ��0~3(����RT��ѹֵWD��������ѹֵFK_IN��0~5V��ѹSD_ADC��
//40~20mAת����ѹIN_ADC_U)��ѹƽ��ֵ,[4]:�ڲ��¶ȴ������¶�ADֵ,
//[5]:�ڲ��Ĳ��յ�ѹ����ֵ(1.2V��׼��ѹ,�ο���ѹԼ3.3V),[6]:�ڲ��¶ȴ������¶�ֵ(��C)����10��
extern u32 temp_val[10];
extern u16 AvVal[10];					
extern u16 adcx;								//�ڲ��ο���ѹADֵ
extern u16 LRtime[10];							//��תʱ��
extern s16 CPUtemp;                             //CPU�¶�
//extern u16 RealTimeFKVal, RealTimeSDVal, RealTimeINVal;
   
u16  T_Get_Temp(void);  					//ȡ���¶�ֵ
void T_Adc_Init(void); 						//ADCͨ����ʼ��
u16  T_Get_Adc(u8 ch); 					//���ĳ��ͨ��ֵ  
u16  T_Get_Adc_Average(u8 ch,u8 times);	//�õ�ĳ��ͨ��10�β�����ƽ��ֵ 	  
void  Get_Adc_Average(void);				//�õ�0~3ͨ��10�β�����ƽ��ֵ 	  
#endif 

