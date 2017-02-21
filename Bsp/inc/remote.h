#ifndef __RED_H
#define __RED_H 
#include "sys.h"   

  
#define RDATA PBin(9)	 //�������������

#define	K1		1							//���ؼ�
#define	K2		2							//ȷ�ϼ�
#define	K3		3							//+��
#define	K4		4							//-��
#define	K1Dn	(key&0x1f)==0x11			//���ؼ�
#define	K2Dn	(key&0x1f)==0x12			//ȷ�ϼ�
#define	K3Dn	(key&0x1f)==0x13			//+��
#define	K4Dn	(key&0x1f)==0x14			//-��
#define	K5Dn	(key&0x1f)==0x15			//���Լ�

#define	K1Up	(key&0x9f)==0x91			//���ؼ�
#define	K2Up	(key&0x9f)==0x92			//ȷ�ϼ�
#define	K3Up	(key&0x9f)==0x93			//+��
#define	K4Up	(key&0x9f)==0x94			//-��

#define	KDn		(key&0x0f)					//������
#define	Klong	(key&0x40)				    //������
#define	ClsKDn		key&=0xef				//�����ֵ
#define	ClsKlong	key&=0xbf				//���������ֵ		


//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0x44
#define REMOTE_ID 0x44      		   
//����ѡ�õ�ң��������
//#define K1v 0xAF      		   
//#define K2v 0x0F      		   
//#define K3v 0xD7      		   
//#define K4v 0xB7      		   

#define K1v 0x50      		   
#define K2v 0xF0      		   
#define K3v 0x28      		   
#define K4v 0x48      		   

#define RKup key&0x20      		   
#define RKdn key&0x0f      		   
#define RKLdn key&0x4f      		   

extern u8 RmTCnt;		//�������µĴ���	  
extern u8 key;		
extern u8 KeyHoldStus;	//����ң�ذ����ж�ֵ	
//extern u8 KeyHoldStus;		

void Remote_Init(void);    //���⴫��������ͷ���ų�ʼ��
void Remote_Scan(void);	     

#endif















