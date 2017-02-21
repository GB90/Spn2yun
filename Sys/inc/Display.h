#ifndef __SETUP_H
#define __SETUP_H	 

#include "sys.h"

//�ִ�����(��ӦInfoDispStrSet��һ�±�):
/*
#define _DiuXin			1					//���ź�
#define _XianFan		3					//�߷�
#define _ZhenDang		5					//��
#define _BiDu			6					//�ն�
#define _KaiDu			7					//����
#define _WaiChaoWen		8					//�ⳬ��
#define _NeiChaoWen		9					//�ڳ���

#define _Kaifa			10					//����
#define _BiFa			11					//�շ�
#define _TingZhi		12					//ֹͣ
#define _YuanCheng		13					//Զ��
#define _XianChang		14					//�ֳ�
#define _DaoWei			15					//��λ
#define _ZhongWei		16					//��λ
#define _QuanKai		17					//ȫ��
#define _QuanBi			18					//ȫ��
#define _KaiGuoJu		19					//��������
#define _BiGuoJu		20					//�չ�����
*/

//������ ˳��������ȼ��� ��->��
#define _ZhuanXiang		0					//ת�����
#define _KaiBiOn		1					//����ͬ��
#define _BiDu			2					//�ն�
#define _KaiDu			3					//����
#define _DiuXin			4					//���ź�
#define _KaiGuoJu		5					//��������
#define _BiGuoJu		6					//�չ�����
#define _NeiChaoWen		7					//�ڳ���
#define _WaiChaoWen		8					//�ⳬ��
#define _QueXiang		9					//ȱ��
//״̬��
#define _Kaifa			0					//����
#define _BiFa			1					//�շ�
#define _TingZhi		2					//ֹͣ
#define _DaoWei			3					//��λ
#define _ZhongWei		4					//��λ
#define _QuanKai		5					//ȫ��
#define _QuanBi			6					//ȫ��

//extern const u16 *InfoDispStrSet[22][5];	//����ĸ��ַ����ַ���(16x32 ����ĺ��ֻ�ͼƬ����)��

extern u16 AutoSetOKF;						//�Զ��궨��
extern u16 isZero;					    //�������Ƿ�����
extern u32 PosLmdU;							//��λ����
extern u32 PosLmdD;							//��λ����
extern u16 CInLmdU;							//��������
extern u16 CInLmdD;							//��������
extern u16 VInLmdU;							//��������
extern u16 VInLmdD;							//��������

extern u16 OutCLmdU;							//�������
extern u16 OutCLmdD;							//�������
extern u16 OutVLmdU;							//�������
extern u16 OutVLmdD;							//�������
extern u16 PosAccuracy;						//��λ����
extern u16 JogHolAutoSel;						//�ֳ����Ʒ�ʽ:	1�㶯,2����,3�Զ�

extern u16 A_DInSel;							//Զ�����뷽ʽ:	0ģ����,1������
extern u16 ScrSel ;							//��ʾ����
extern u16 NoSignalCtrSel;					//���Ŵ���ʽ	1ֹͣ��2ȫ����3ȫ��
extern u16 language;					//����ѡ��		0���� ��0Ӣ��
extern u16 electronicbrake;					//����ɲ��������־	
extern u16 CloseDirection;					//�رշ���			1��   0��
extern u16 SetCloseDir;					//�رշ���			1��   0��
extern u16 Closeway;						//Closeway, ��0 ����   0 ��λ  �жϵ����λ�Ϳ�λ������ ���ؿ��ػ���λ����
extern u16 Rs485Way;
extern u16 ESDset;						//ESD���ã�2��λ��3��λ��1ֹͣ
extern u16 mbaddr;						//ͨѶ��ַ
extern u16 TwoLine;						//���߿���	  1��ֹ  2���źſ�  3���źŹ�	  
extern u16 LockedRotorTime;				//��תʱ��
extern u16 IntermittentRun;				//�������	(��0��������У�0��)
extern u16 Intermediate1;			    //�м�λ��1 0%~�м�λ��1
extern u16 Intermediate2;			    //�м�λ��2 �м�λ��1~100%
extern u16 PassWord;						//����(Ĭ��Ϊ"0000"),�޶�С�ڵ���9999
extern u16 RmLcDisSel;						//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�

extern u16 OutCVSel;							//����������ѹѡ��

extern u16 EnSelFlsh;							//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
extern u16 DispParaSet_test;					//���Բ�����ʾѡ��

extern u16 OutCVSel;							//����������ѹѡ��
extern u8 DispStep;
extern u16 lock;						//Զ����״̬ 1������ 0������
extern u32 EncoderCurr;                 //��������ǰֵ
void Display(void);
void clear_screen(void);
void Display_Init(void);

#endif

