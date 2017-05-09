#include "Def.h"
#include <Math.h>
#define	_coloffset	    13		//�ӵڼ��п�ʼ��ʾ
#define 	line 		14		//��ʾ��
#define 	jump 		17		//�����ּ�������

//DispStep:
#define	DispStart						0
#define	DispWorkStusWait				1
#define	DispWorkStus					2
#define	DispEnioySetWait				3
#define	DispEnioySet					4
#define	DispSet1_1Wait					5
#define	DispSet1_1						6
#define	DispSet1_1_OK_Wait				7
#define	DispSet1_1_OK					8
#define	DispSet1_2Wait					9
#define	DispSet1_2						10
#define	DispSet1_3Wait					11
#define	DispSet1_3						12
#define	DispSet1_3Wait_PassWord			13
#define	DispSet1_4Wait					14
#define	DispSet1_4						15

#define	DispSet2_6_Wait					16
#define	DispSet2_6						17
#define	DispSet2_6_OK					18


//u32	StopDellTim=100;
u32	StopDellTim=10;
//u32	MoveMaxTim=500;
u32	MoveMaxTim=5000;
u32 WaitTim;
u32 WaitTimKai;
u32 WaitTimBi;

u16 lock;
u8 BaseSet1_STEP=0;
u8 StusView;							//״̬��ʾ��Ϣ(�ִ����)
u8 FreshTimes = 0;

const u16 *InfoDispStrSet[7][2]={				//(16x32 ����ĺ��ֻ�ͼƬ���п�)
	Kai_16x32,Fa_16x32, 			            //���� 1
	Bi_16x32,Fa_16x32,				            //�շ� 2
	Ting_16x32,Zhi3_16x32,	                    //ֹͣ 3
	Dao_16x32,Wei_16x32,			            //��λ 4
	Zhong_16x32,Wei_16x32,		                //��λ 5
	Quan_16x32,Kai_16x32,			            //ȫ�� 6	
	Quan_16x32,Bi_16x32,			            //ȫ�� 7
};

//�̵������������̬����ĿList
const u8 *RelayItem[13][5]={
	Bi_14x14,Wei_14x14,SP_14x14,SP_14x14,0,			            //0  �ص�λ
	Kai_14x14,Wei_14x14,SP_14x14,SP_14x14,0,					//1  ����λ
	Bi_14x14,Guo_14x14,Ju_14x14,SP_14x14,0,			            //2  �ع���
	Kai_14x14,Guo_14x14,Ju_14x14,SP_14x14,0,			        //3  ������
	Guo_14x14,Ju_14x14,SP_14x14,SP_14x14,0,		                //4  ����
	Bi_14x14,Yun_14x14,xing_14x14,Zhong_14x14,0,	            //5  �������У����ڹأ�
	Kai_14x14,Yun_14x14,xing_14x14,Zhong_14x14,0,	            //6  �������У����ڿ���
    Ben_14x14,Di_14x14,SP_14x14,SP_14x14,0,		                //7  ����
	ChrE_14x14,ChrS_14x14,ChrD_14x14,SP_14x14,0,		        //8  ESD�ź�
	Zhong_14x14,Jian2_14x14,Wei_14x14,Chr1_14x14,0, 	        //9  �м�λ��1
    Zhong_14x14,Jian2_14x14,Wei_14x14,Chr2_14x14,0, 	        //10 �м�λ��2
	Bao1_14x14,Jing1_14x14,SP_14x14,SP_14x14,0,				    //11 ����
    Dian1_14x14,Yuan1_14x14,Que1_14x14,Xiang1_14x14,0,				//12 ȱ��
};

//������ĿList
const u16 *ErrItem[10][5]={
	Zhuan_16x16,Xiang_16x16,Cuo_16x16,Wu_16x16,0,		//ת�����
	Kai_16x16,Bi_16x16,Tong_16x16,Zai_16x16,0,			//����ͬ��
	Bi_16x16,Fa_16x16,Du_16x16,Zhuan_16x16,0,			//�շ���ת
	Kai_16x16,Fa_16x16,Du_16x16,Zhuan_16x16,0,			//������ת
	Dui_16x16,Xin_16x16,Hao_16x16,SP1_16x16,0,		    //���ź�
	Kai_16x16,Guo_16x16,Li_16x16,Ju_16x16,0,			//��������
	Bi_16x16,Guo_16x16,Li_16x16,Ju_16x16,0,				//�չ�����
	Nei_16x16,Bu_16x16,Chao_16x16,Wen_16x16,0,		    //�ڲ�����
	Dian_16x16,Ji_16x16,Guo_16x16,Re_16x16,0,		    //�������
    Dian_16x16,Yuan_16x16,Que_16x16,Xiang1_16x16,0,		//��Դȱ��
};

u16 TmpSetVal;					
//���������趨��
u16 AutoSetOKF;					//�Զ��궨��
u16 isZero;					    //�������Ƿ�����
u32 PosLmdU;					//��λ����		4095
u32 PosLmdD;					//��λ����
u8  isZeroTemp = 127;
u16 PosAccuracy = 100;			//��λ����(Ĭ��0.50)
u16 JogHolAutoSel = 1;			//�ֳ����Ʒ�ʽ:1�㶯,2����,3�Զ�
u16 A_DInSel = 1;				//Զ�����뷽ʽ:0ģ����,1������,3�߿أ���
u16 NoSignalCtrSel = 1;			//���Ŵ���ʽ	1ֹͣ��2ȫ����3ȫ��
u16 ScrSel;						//��ʾ����		0����  ��0����
u16 language;					//����ѡ��		0���� ��0Ӣ��
//�߼������趨��
u16 electronicbrake;			//����ɲ��������־    0����  ��0 ����
u16 CloseDirection;				//�رշ���			1��   0��
u16 SetCloseDir;
u16 Closeway;					//Closeway, ��0 ����   0 ��λ  �жϵ����λ�Ϳ�λ������ ���ؿ��ػ���λ����
u16 Rs485Way=0;                     //1�����߿�����Ч��д��Ч 0�����߿�����Ч��д��Ч
u16 ESDset = 1;					    //ESD���ã���λ����λ����ֹ
u16 TwoLine = 1;					//���߿���	  1��ֹ  2���źſ�  3���źŹ�		
u16 LockedRotorTime;			//��תʱ��		0- 10S
u16 IntermittentRun;			//�������	(��0��������У�0��)
u16 Intermediate1;			    //�м�λ��1 0%~�м�λ��1
u16 Intermediate2;			    //�м�λ��2 �м�λ��1~100%
u16 PassWord;				    //����(Ĭ��Ϊ"0000"),�޶�С�ڵ���9999

u16 CInLmdU;				//����C����	3707					//0x0e78
u16 CInLmdD;				//����C����	746					    //0x02e8
u16 VInLmdU;				//����V����	4095
u16 VInLmdD;				//����V����	1

u16 OutCLmdU;			//�������		3375					//0x0d2f
u16 OutCLmdD;			//�������		669		2706			//0x029d
u16 OutVLmdU;				//�������		4095
u16 OutVLmdD;				//�������		0
u16 RmLcDisSel;					//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
u16 OutCVSel;					//����������ѹѡ��		=1
u16 OutCVSelflag;				//����������ѹѡ���־	

u16 LRtimeh=0010;				//��תʱ������
u16 LRtimel=0000;				//��תʱ������
//u16 Pos=0;						//��λ(����)
//���õ�����־
//u8 PosAccuracy_flag;				//��λ���ȵ��ڱ�־ zhaoxw ��Χ����
u16 PosAccuracy_flag;				//zhaoxw
u16 JogHolAutoSelflag;			//�ֳ����Ʒ�ʽ:1�㶯,2����,3�Զ� ��־λ
u16 A_DInSelflag;					//Զ�����뷽ʽ:ģ����,������
u16 NoSignalCtrSelflag;			//���ű�־
u16 ScrSelflag;					//��ʾ����		0����  ��0����
u16	languageflag;				//��Ӣ�Ĳ˵���ʾ��ʶ 
u16 electronicbrakeflag;			//����ɲ�������л���־
u16 CloseDirectionflag;			//�رշ���
u16 Closewayset;					//�رշ�ʽ
u16 Rs485Wayset;
u16 ESDsetflag;					//ESD���ã���λ����λ����ֹ
u16 TwoLineflag;					//���߿���
u16 LockedRotorTime_flag;		//��תʱ��
u16 mbaddr_flag;		        //ͨѶ��ַ
u16 mbaddr = 1;		                //ͨѶ��ַ
u16 IntermittentRunflag;			//�������
u16 Intermediate1flag;			//�м�λ��1
u16 Intermediate2flag;			//�м�λ��2
u16 StallTime;					//��תʱ��Stall time
//u8 ElecBrakEn;					//�����ƶ�ʹ��

u8 EOD;							//��������Effect of direction

//�������
u8 Testcloseorpen;				//���Կ��հ����Ƿ�����
u8 Testcorpflag;					//���Կ��հ����Ƿ�������ʶ

//u8 NoSignalCtr;					//���źſ��Ʒ����趨
//u8 AutoCalEn;					//�Զ��궨�趨Auto calibration
u8 MnOffPosSet;					//�㶯���趨

u16 InVLmdU;					//��������
u16 InVLmdD;					//��������
u16 TEMPCal;					//�¶�У׼
//u32 PassWord;					//����,�޶�С�ڵ���9999

u8 DispStep;		
u16	DispSetSel;		

u8	RolStus=1;		
u16 EnSelFlsh=0;				//ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
u16 DispParaSet_test=0;			//�Ƿ���ʾ�ڲ�����(������)

u8 ClosewayTemp;        //�ط���ʽ�ݴ� Ϊ�����г��趨����ʱʹ��λ������Ч
u8 ZeroStatus = 0;        //�����״̬��
void Display(void);
void DisplayTest(void);
void DisplayVer(void);
void displayMain(void);
void displaySetupMenu(void);

void dispMain(void);
void dispSetupMenu(void);
void dispSetMenu(u16 Sel);
void dispSetMenu1_1(void);
void dispSetMenu1_2(void);
void dispSetMenu1_3(void);
void dispSetMenu1_4(void);

void disp_Set1_1_x(u16 Sel);
void disp_Set1_2_x(u16 Sel);
void disp_Set1_3_x(u16 Sel);
void disp_Set1_4_x(u16 Sel);

void displayInterPara(u16 a,u16 b,u16 c,u16 d,u16 e,u16 f,u16 g,u16 h);
void BaseSet(void);					//�������ò���
void FeedBackSet(void);			//������Ʋ���
void AdvancedSet(void);			//�߼����ò���
void FaultDiagnosisSet(void);			//���������Ŀ����

void dispSetMenu1_2_6(void);
void disp_Set1_6_x(u16 Sel);
void disp_Set1_6_xEnglish(u16 Sel);

u8 GetMotoPos(void);	//ȡ�õ�λ��λ��

void displayEnglishSetupMenu(void);
void dispEnglishSetMenu(u16 Sel);
void disp_EnglishSet1_1_x(u16 Sel);
void TwoLineRun(void);
void IntermittentRundis(u8 Htarget ,u8 Ltarget);
u8 Trg(u8 trg);
void ClearMenu(void);
/*********************************************************
function name: ��ʾ������ʼ��
fun	:	��ʼ����ز���
*********************************************************/
void Display_Init(void)
{
	PosAccuracy_flag = PosAccuracy;		    //��ʼ������
	JogHolAutoSelflag = JogHolAutoSel;		
	A_DInSelflag = A_DInSel;				//Զ�̿��Ʒ�ʽ
	NoSignalCtrSelflag = NoSignalCtrSel;
	ScrSelflag = ScrSel;
	languageflag = language;
	electronicbrakeflag = electronicbrake;
	CloseDirectionflag = SetCloseDir;
	Closewayset = Closeway;
    Rs485Wayset = Rs485Way;
	ESDsetflag = ESDset;
    mbaddr_flag = mbaddr;
	TwoLineflag = TwoLine;
	LockedRotorTime_flag = LockedRotorTime;
	IntermittentRunflag = IntermittentRun;
    Intermediate1flag = Intermediate1;
    Intermediate2flag = Intermediate2;
	OutCVSelflag = OutCVSel;
}

/*********************************************************
function name: ��ʾ����
fun	:
*********************************************************/
void Display(void)
{
	DisplayVer();						//������ʾ
	displayMain();						//������������ʾ������
	if(language )
    {
		displayEnglishSetupMenu();		//Ӣ�����ò˵���ʾ
    }
	else
	{	
        displaySetupMenu();				//�������ò˵���ʾ������
        //DisplayTest();				//������ʾ
        ClearMenu();
    }
}

/*********************************************************
function name: �������Ľ������

fun	:	�������Ĳ˵���ʾ
*********************************************************/
void displaySetupMenu(void)
{
	dispSetupMenu();			//�������˵�==>1:��������,2:��������,3:�߼�����,4:�������.....
	dispSetMenu1_1();			//1:��������==>1:�Զ��궨,2:��λȷ��,3:��λȷ��,4:���ƾ���,5:�ֳ�����,6:Զ������,7:���Ŷ���,8:��ʾ����
	dispSetMenu1_2();			//2:��������==>1:�������,2:��������,3:��������,4:�������,5:�������,6:�������
	dispSetMenu1_3();			//3:�߼�����==>1:ESD����, 2:�رշ�ʽ,3:�رշ���,4:���߿���,5:��תʱ��,6:����ɲ��,7:�������,8:��������,9:�ָ�����,
	dispSetMenu1_4();			//4:�������==>1:��ʽ����,2:��������,3:Զ�̴�,4:Զ�̹ر�,5:Զ�̱���,6:Զ���Զ�,7:�ָ�����,
	dispSetMenu1_2_6();			//�������˵�==>2:��������==>6:�������(���ͨ���趨)
}

/*********************************************************
function name: ����Ӣ�Ľ������

fun	:����Ӣ�Ĳ˵���ʾ
*********************************************************/
void displayEnglishSetupMenu(void)
{
	dispSetupMenu();			//�������˵�==>1:��������,2:��������,3:�߼�����,4:�������.....
	dispSetMenu1_1();			//1:��������==>1:�Զ��궨,2:��λȷ��,3:��λȷ��,4:���ƾ���,5:�ֳ�����,6:Զ������,7:���Ŷ���,8:��ʾ����
	dispSetMenu1_2();			//2:��������==>1:�������,2:��������,3:��������,4:�������,5:�������,6:�������
	dispSetMenu1_3();			//3:�߼�����==>1:ESD����,2: �رշ�ʽ,3: �رշ���,4: ���߿���,5: ��תʱ��,6, ����ɲ��,7: �������,8: ��������,9: �ָ�����,
	dispSetMenu1_4();			//4:�������==>1: ��ʽ����,2: ��������,3: Զ�̴�,4: Զ�̹ر�,5: Զ�̱���,6: Զ���Զ�,//7: �ָ�����,
	dispSetMenu1_2_6();			//�������˵�==>2:��������==>6 �������(���ͨ���趨)
}

/*********************************************************
function name: ��������

fun	:
*********************************************************/
void DisplayVer(void)
{
#ifndef __Debug__
    u8 i;
#endif    
	switch(DispStep)
		{
		case	DispStart:
			//����������ʾ	
/*			clear_screen();
			testLCD();
			for(i=0;i<150;i++)
				{
				StartDelay_ms(5);
				while(TestDelay_ms());
				}
			clear_screen();
*/
#ifndef __Debug__
			/*for(i=0;i<200;i++)
			{
				display_char32x32(4*16,(200-i)/3+3,ChrTm_32x32,0);
				display_char32x32(4*16,i/20+3,ChrOm_32x32,0);
				display_char32x32(4*16,i/10+3,ChrYm_32x32,0);
				display_char32x32(4*16,i/5+3,ChrIm_32x32,0);
				IWDG_feed();		//ι��
				StartDelay_ms(5);
				Sample();
				while(TestDelay_ms());
			}*/
			clear_screen();
			display_char32x32(4*16,0*10+3,ChrTm_32x32,0);
			display_char32x32(4*16,1*10+3,ChrOm_32x32,0);
			display_char32x32(4*16,2*10+3,ChrYm_32x32,0);
			display_char32x32(4*16,3*10+3,ChrIm_32x32,0);
			display_char32x32(4*16,4*10+3,ChrRm_32x32,0);
//			testLCD();
			IWDG_feed();		//ι��
            
            for(i = 0; i < 20; i++)
			{
                IWDG_feed();		    //ι��
				StartDelay_ms(5);
				Sample();
				while(TestDelay_ms());
			}
            
			clear_screen();
 /*           
			display_char32x32(4*16,1*10,ChrV_32x32,0);
//			display_char32x32(4*16,1*10+3,Chr1_32x32,0);
//			display_char32x32(4*16,2*10+3,ChrPoin_32x32,0);
//			display_char32x32(4*16,2*10+6,Chr0_32x32,0);
			display_char32x32(4*16,2*10+3,Chr0_32x32,0);
			display_char32x32(4*16,3*10+3,Chr1_32x32,0);
*/          
            display_char8x16(64,15,"V:0.2.40b",0); 				//�汾��
            display_char8x16(80,15,"2017/05/09",0);			//����

			for(i=0;i<15;i++)
			{
                IWDG_feed();		    //ι��
				StartDelay_ms(5);
				Sample();
				while(TestDelay_ms());
			}
			clear_screen();

#endif
			DispStep = DispWorkStusWait;

			
			break;
		}
}
/*********************************************************
����ģʽ��ʾ

*********************************************************/
void DisplayTest(void)
{
	u8 i;
	u32 L,x1,x2,x3;
	
	
	if(DispParaSet_test==1)
		{
			//������,��ʾ�����ƽ
			x1=x2=x3=0;
			L=AvVal[7];
		//	L=0x17;
			for(i=0; i<4; i++)
				{
					x1=x1*10;
					if(L & 0x0800)
						{
							x1+=10;
						}
					L=L<<1;
				}
			for(i=0; i<4; i++)
				{
				x2=x2*10;
					if(L & 0x0800)
						{
							x2+=10;
						}
					L=L<<1;
				}
			for(i=0; i<4; i++)
				{
				x3=x3*10;
					if(L & 0x0800)
						{
							x3+=10;
						}
					L=L<<1;
				}
		}
	else	if(DispParaSet_test==2)
		{
			//������,��ʾ�����ƽ
			x1=x2=x3=0;
			L=AvVal[8];
		//	L=0x17;
			for(i=0; i<4; i++)
				{
					x1=x1*10;
					if(L & 0x0800)
						{
							x1+=10;
						}
					L=L<<1;
				}
			for(i=0; i<4; i++)
				{
				x2=x2*10;
					if(L & 0x0800)
						{
							x2+=10;
						}
					L=L<<1;
				}
			for(i=0; i<4; i++)
				{
				x3=x3*10;
					if(L & 0x0800)
						{
							x3+=10;
						}
					L=L<<1;
				}
		}
	

	if(K5Dn)		//����ң�� ����
	{
		ClsKDn;
		DispParaSet_test+=1;
		if(DispParaSet_test>=3)
		{
			DispParaSet_test=0;
			displayInterPara(AvVal[_WD],AvVal[_FK],AvVal[_SD],AvVal[_IN],AvVal[_IT_C],x1/10,x2/10,x3/10);
		}
	}
	if(DispParaSet_test)
	{
		displayInterPara(AvVal[_WD],AvVal[_FK],AvVal[_SD],AvVal[_IN],AvVal[_IT_C],x1/10,x2/10,x3/10);
	}


}

/*********************************************************
function name:��ת�жϺ���

fun :			�жϵ���Ƿ��ת
*********************************************************/
void  LockedRotor(void)
{
    static u16 temp1=0;
//    static u16 temp2=0;
    
    if((temp1 & 0xFFFC) == (PIStus.FK_IN_Pers& 0xFFFC))
    {
        //temp1=PIStus.FK_IN_Pers;
        if( locktime >= LockedRotorTime*10)
        {
            /*
            if(1 == CloseDirection)
            {
                if(Moto_FWD_Chk)
                {
                    POStus.AL |= OPENBLOCK;
                }
                else if(Moto_REV_Chk)
                {
                    POStus.AL |= SHUTBLOCK;
                }
                else
                {
                    POStus.AL &= ~OPENBLOCK;
                    POStus.AL &= ~SHUTBLOCK;
                }
            }
            else
            {
                if(Moto_FWD_Chk)
                {
                    POStus.AL |= SHUTBLOCK;
                }
                else if(Moto_REV_Chk)
                {
                    POStus.AL |= OPENBLOCK;
                }
                else
                {
                    POStus.AL &= ~OPENBLOCK;
                    POStus.AL &= ~SHUTBLOCK;
                }
            }
            */
            if(Moto_FWD_Chk) 
            {
                if(1 == CloseDirection)
                {
                    POStus.AL |= OPENBLOCK;
                }
                else
                {
                    POStus.AL |= SHUTBLOCK;
                }
            }
            else if(Moto_REV_Chk)
            {
				if(1 == CloseDirection)
				{
					POStus.AL |= SHUTBLOCK;
				}
				else
				{
					POStus.AL |= OPENBLOCK;
				}
			}
			else 
			{
                POStus.AL &= ~OPENBLOCK;
                POStus.AL &= ~SHUTBLOCK;
			}
		}
    }
    else
    {
        locktime = 0;
        temp1=PIStus.FK_IN_Pers;
        POStus.AL &= ~OPENBLOCK;
        POStus.AL &= ~SHUTBLOCK;
    }
    
// 	if( locktime==0)
// 	{
//		temp1=PIStus.FK_IN_Pers;
// 	}
//    else if( locktime >= LockedRotorTime*10)				//�����趨��ת�ж�ʱ��	
//	{
//		locktime = 0;
//        temp2=PIStus.FK_IN_Pers;
//		if(Moto_FWD_Chk) 
//		{
//			//if((temp1>=temp2-2)&&(temp1<=temp2+2))
//            if((temp1 & 0xFFFC) == (temp2 & 0xFFFC))
//			{
//                if(CloseDirection)
//                {
//                    POStus.AL |= OPENBLOCK;
//                }
//                else
//                {
//                    POStus.AL |= SHUTBLOCK;
//                }
//			}
//			else 
//			{
//                POStus.AL &= ~OPENBLOCK;
//                POStus.AL &= ~SHUTBLOCK;
//			}
//		}
//		else if(Moto_REV_Chk)
//		{
//			//if((temp2>=temp1-2)&&(temp2<=temp1+2))
//            if((temp1 & 0xFFFC) == (temp2 & 0xFFFC))
//			{
//				if(CloseDirection)
//				{
//					POStus.AL |= SHUTBLOCK;
//				}
//				else
//				{
//					POStus.AL |= OPENBLOCK;
//				}
//			}
//			else 
//			{
//                POStus.AL &= ~OPENBLOCK;
//                POStus.AL &= ~SHUTBLOCK;
//			}
//		}
//	}
}	

/*********************************************************
function name:�㶯���к���

fun :	�յ�����ת�źţ�ת���̶�����
*********************************************************/
void JogRun(void)
{
	static u8 stopflag=0;		//ɲ��״̬λ
	//static u32 runflag=0;		//���ת��λ��״̬
	if((KeyHoldStus) || (KeyHoldStusA))
	{
        if((key & 0x0f) == 4)		
        {
            if((GetMotoPos() < _MoreLdU))	//�㶯״̬ÿ��ת	50
            {
                Moto_FWD_Drv;			    //��ת
                                            //���ת���ˣ���Ҫɲ������λɲ��״̬��
                stopflag = 1;				//���ת���ˣ���Ҫɲ������λɲ��״̬��
            }            
        }
        else if(((key & 0x0f) == 3))
        {
            if((GetMotoPos() > _LessLdD) )	
            {
                Moto_REV_Drv;			    //��ת
                stopflag = 1;				//���ת���ˣ���Ҫɲ������λɲ��״̬��
            }                        
        }
        
        if(((key & 0x0f) == 0)&&stopflag)
        {

            Moto_STOP_Drv;			//���ɲ��
            stopflag = 0;
        }
	}
	else
	{
		if(electronicbrake)				//��������˵���ɱ������
		{
			Moto_STOP_Drv;
		}
		else								//û�п�������ɲ������
		{
			Moto_PARK_Drv;
		}
	}
}

/*********************************************************
function name:�������к���

fun :	�յ�����ת�źţ�����ת�����źŽ���
*********************************************************/
void KeepRun(void)
{
	if((KeyHoldStus) || (KeyHoldStusA))
	{
		if(K4Dn)
		{
            if((GetMotoPos() < _MoreLdU))
            {
                Moto_FWD_Drv;	//��ת
            }
		}
		else if(K3Dn)
		{
            if((GetMotoPos() > _LessLdD))
            {
                Moto_REV_Drv;   //��ת
            }
		}
	}
}

/*********************************************************
function name:�Զ����к���

fun :	�յ�����ת�źţ��̰��ź���㶯һ���������ź���ת������λ��
*********************************************************/
void AnimationrRun(void)
{
	static u8 stopflag=0;		//ɲ��״̬λ
//	static u32 runflag=0;		//���ת��λ��״̬
	if(((KeyHoldStus) )|| ((KeyHoldStusA)))
	{
		if(K4Dn)
		{
            if(GetMotoPos() < _MoreLdU) 
            {
                Moto_FWD_Drv;			//��ת
                stopflag = 1;				//���ת���ˣ���Ҫɲ������λɲ��״̬��
                if(Klong)
                {
                    if(GetMotoPos() < _MoreLdU) 
                    {
                        Moto_FWD_Drv;	//��ת	
                        stopflag = 0;
                    }
                }
            }
		}
		else if(K3Dn)
		{
			if(GetMotoPos() > _LessLdD) 
			{
				Moto_REV_Drv;				    //��ת
				stopflag = 1;					//���ת���ˣ���Ҫɲ������λɲ��״̬��
				if(Klong)						//�������Զ��յ�λ
				{
					if(GetMotoPos() > _LessLdD) 
					{
                        Moto_REV_Drv;			//��ת
                        stopflag = 0;				//���ת���ˣ���Ҫɲ������λɲ��״̬��
					}
				}
			}
		}
	}
	else if((!KeyHoldStusA) && (!KeyHoldStus) && stopflag)//zhaoxw
	{
        if(electronicbrake)				//��������˵���ɱ������
        {
            Moto_STOP_Drv;
        }
        else							//û�п�������ɲ������
        {
            Moto_PARK_Drv;
        }
	}
}

/*********************************************************
function name:�������к���

fun :			ѡ�񱾵�ʱ displayMain״ִ̬������
*********************************************************/
void  Localdisplay(void)
{
	//u8 static stc=0;//zhaoxw
//    static u8 stc=0;
	if(JogHolAutoSel == 1)				    //�㶯-----------------
	{
		JogRun();
	}
	else if(JogHolAutoSel== 2)				//����-----------------
	{
		KeepRun();
	}
	else if(JogHolAutoSel == 3)				//�Զ�-----------------
	{
		AnimationrRun();
	}
    
//    if(PIStus.IN_RCL==1&&stc)				//Զ�̰����л� ���� /Զ�̿���
//    {
//        RmLcDisSel = (RmLcDisSel ^ 0x01);
//        stc=0;
//    }
//    else if(PIStus.IN_RCL==0)
//    {
//        stc=1;
//    }
}
/*********************************************************
function name:Զ��ģ�������к���

fun :			ѡ��Զ��ģ��ʱ displayMain״ִ̬������
*********************************************************/
void RemoteAnalogdisplay(void)
{		
	//if(IntermittentRun==0)//ģ��������Ҫ�������
	{
		if((GetMotoPos() ==_InTarget))			//��λ��Ŀ��ֵ��
		{
            if(electronicbrake)				    //��������˵���ɱ������
			{
				Moto_STOP_Drv;
			}
			else								//û�п�������ɲ������
			{
				Moto_PARK_Drv;
			}
		}
		else if(GetMotoPos() ==  _LessTarget)		//С��Ŀ��ֵ����ת
		{
			Moto_FWD_Drv;
		}
		else if(GetMotoPos() == _MoreTarget)		//����Ŀ��ֵ��ת
		{
			Moto_REV_Drv;
		}
	}

	if(POStus.AL & VMALOW)//
	{
        //���Ŵ���
		if(NoSignalCtrSel==1)						//ֹͣ�����ֵ�ǰλ��
		{
			Moto_PARK_Drv;						
		}
		else if(NoSignalCtrSel == 2)					//ȫ��
		{  
            if(GetMotoPos() < _MoreLdU) 
            {
                Moto_FWD_Drv;		
            }
            else
            {
                Moto_PARK_Drv;
            }
		}
		else if(NoSignalCtrSel == 3)					//ȫ��
		{
			if(GetMotoPos() > _LessLdD) 
			{
				Moto_REV_Drv;			
			}
			else
			{
                Moto_PARK_Drv;
			}
		}
	}	
}
/*********************************************************
function name:Զ�̿��������к���

fun :			ѡ��Զ�̿�����ʱ displayMain״ִ̬������
*********************************************************/
void RemoteQquantitydisplay(void)
{
//	u8 static stc = 0;	//Զ��/����״̬��
	u8 static sta = 0 ;	//BC״̬��
	u8 static stic = 0;
    static u8 TimerFlag;
	 if(PIStus.IN_BC==1&&sta)					//Զ���Զ�����
	 {
		stic++;
		if(stic>=3)
		{
			stic =0;
		}
		sta=0;
	 }
	 else if(PIStus.IN_BC==0)
	 {
		sta=1;
	 }
//****************************************Զ�� /�����л�	 
//	 if(PIStus.IN_RCL==1&&stc)			
//	 {
//		RmLcDisSel = (RmLcDisSel ^ 0x01);
//		stc=0;
//	 }
//	 else if(PIStus.IN_RCL==0)
//	 {
//		stc=1;
//	 }
/************************************************************/      
    if(TwoLine == 1)			//��ֹ
    {
        POStus.AL &= ~ERRCMD;
        if(!P_IN_SHUT && !P_IN_OPEN)//����ͬ��(�����������߿���)
        {
            TimerFlag = 0;
            PIStus.IN_OPEN = 0x01;
            PIStus.IN_SHUT = 0x01;
            POStus.AL |= ERRCMD;
        }
        else if(!P_IN_SHUT || !P_IN_OPEN)//�󰴰�����Ч
        {
            if(0 == TimerFlag)
            {
                InputTimer = 25;
                TimerFlag = 1;
            }
        }
        else
        {
            TimerFlag = 0;
            PIStus.IN_OPEN = 0x00;
            PIStus.IN_SHUT = 0x00;
        }
        
        if(0 == InputTimer && TimerFlag)//200ms
        {
            PIStus.IN_SHUT = 0x00;
            PIStus.IN_OPEN = 0x00;
            
            if(!P_IN_SHUT)
            {
                PIStus.IN_SHUT = 0x01;
            }
            
            if(!P_IN_OPEN)
            {
                PIStus.IN_OPEN = 0x01;
            }
        }
    }
    else
    {
        if(TwoLine == 2)
        {
            if(!P_IN_OPEN)
            {
                PIStus.IN_OPEN = 0x01;
                PIStus.IN_SHUT = 0x00;
            }
            else if(!P_IN_SHUT)
            {
                PIStus.IN_SHUT = 0x01;
                PIStus.IN_OPEN = 0x00;
            }
            else
            {
                PIStus.IN_SHUT = 0x00;
                PIStus.IN_OPEN = 0x00;
            }
        }
        if(TwoLine == 3)
        {
            if(!P_IN_SHUT)
            {
                PIStus.IN_SHUT = 0x01;
                PIStus.IN_OPEN = 0x00;
            }
            else if(!P_IN_OPEN)
            {
                PIStus.IN_OPEN = 0x01;
                PIStus.IN_SHUT = 0x00;
            }
            else
            {
                PIStus.IN_SHUT = 0x00;
                PIStus.IN_OPEN = 0x00;
            }
        }
    }

/************************************************************/ 
    if(TwoLine == 2 || TwoLine == 3)
    {   
        if(TwoLine == 2)
        {
            if(PIStus.IN_OPEN)
            {
                if(GetMotoPos() <_MoreLdU) 
                {
                    Moto_FWD_Drv;
                }
                else
                {
                    Moto_PARK_Drv;//ֹͣ
                }
            }
            else if(PIStus.IN_SHUT)
            {
                if(GetMotoPos() >_LessLdD) 
                {
                    Moto_REV_Drv;//��ת
                }
                else
                {
                    Moto_PARK_Drv;//ֹͣ	
                }
            }
            else
            {
                Moto_PARK_Drv;
            }
        }
        else
        {
            if(PIStus.IN_SHUT)
            {
                if(GetMotoPos() >_LessLdD) 
                {
                    Moto_REV_Drv;//��ת
                }
                else
                {
                    Moto_PARK_Drv;//ֹͣ	
                }
            }
            else if(PIStus.IN_OPEN)
            {
                if(GetMotoPos() <_MoreLdU) 
                {
                    Moto_FWD_Drv;//��ת
                }
                else
                {
                    Moto_PARK_Drv;//ֹͣ
                }
            }
            else
            {
                Moto_PARK_Drv;
            }
        }
    }
	else
	{
//****************************************Զ�̰������շ�
		if(PIStus.IN_BC)								//Զ���Զ�
		{
			stic = 0;
			if(!PIStus.IN_OPEN && PIStus.IN_SHUT)	
			{
				stic = 1;
			}
			else if(PIStus.IN_OPEN && !PIStus.IN_SHUT)
			{
				stic = 2;
			}
            else if(PIStus.IN_OPEN && PIStus.IN_SHUT)
            {
                stic = 3;
            }
			if(stic == 1)
			{						//����λ
				if(GetMotoPos() <_MoreLdU  ) 
				{
					Moto_FWD_Drv;//��ת
				}
				else
				{
					Moto_PARK_Drv;//ֹͣ
				}
			}
			else if(stic == 2)
			{
									  //�յ�λ
				if(GetMotoPos() >_LessLdD) 
				{
					Moto_REV_Drv;//��ת
				}
				else
				{
					Moto_PARK_Drv;//ֹͣ	
				}
			}
            else if(stic == 3)
            {
                Moto_PARK_Drv;//ֹͣ
            }
		}
		else											//Զ�̵㶯
		{
			if(PIStus.IN_OPEN && !PIStus.IN_SHUT)						//Զ�̰�������
			{
				if(GetMotoPos() < _MoreLdU) 
				{
					Moto_FWD_Drv;
				}
				else
				{
					Moto_PARK_Drv;
				}
			}
			else  if(!PIStus.IN_OPEN && PIStus.IN_SHUT)				//Զ�̰����շ�		
			{
				if(GetMotoPos() > _LessLdD) 
				{
					Moto_REV_Drv;		
				}
				else
				{
                    Moto_PARK_Drv;
				}
			}
			else
			{
				Moto_PARK_Drv;
			}	
		}
	}

	
/*********************************** Զ�̰���BC���в���
	 if(stic==1&&!PIStus.IN_OPEN&&!PIStus.IN_SHUT&&!PIStus.IN_ESD)	
	{
					//����λ
		if(GetMotoPos() <_MoreLdU  ) 
		{
			Moto_FWD_Drv;//��ת
		}
		else
		{
			Moto_PARK_Drv;//ֹͣ
		} 	
	}
	else if(stic==2&&!PIStus.IN_OPEN&&!PIStus.IN_SHUT&&!PIStus.IN_ESD)
	{
				  //�յ�λ
		if(GetMotoPos() >_LessLdD) 
		{
			Moto_REV_Drv;//��ת
		}
		else
		{
			Moto_PARK_Drv;//ֹͣ	
		}
	}
*/
//*******************************************Զ��ESD����			
}
/*********************************************************
����������

*********************************************************/
void displayMain(void)
{
#if (SOFTLOCK == 1)
    static u8 lockflag;
#endif
    
    if(DispStep != 2)                                   //�ڲ˵��£���������ȡ�ٶȹ���
        delay_ms(1);
    switch(DispStep)
	{
		case	DispWorkStusWait:
				Moto_PARK_Drv;                          //����˵�ʱ�����ֹͣ
				if((KeyHoldStus==0) && (KeyHoldStusA==0))
				{
					ClsKlong;
					ClsKDn;
					DispStep=DispWorkStus;
                    FreshTimes = 0;
				}
				break;
		case	DispWorkStus:
				//���뷧λ(����)��ʾ
                dispMain();								//������������ʾ
				if((Klong)&&(KDn==K1))//�������ؼ�
				{
                    if(RmLcDisSel == 1)
                    {
//                        RelayRunFlag = 0;
                        Moto_PARK_Drv;
                        clear_screen();
                        DispSetSel=1;
                        if(language)		                //�жϵ�ǰ��ʾ����
                            dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
                        else
                            dispSetMenu(DispSetSel);		//��ʾ���Ĳ˵�
                        if(language)
                            dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
                        else
                            displaySetupMenu();				//��ʾ���Ĳ˵�
                        DispStep=DispEnioySetWait;
                    }
                    
#if (SOFTLOCK == 1)
                    if(RmLcDisSel == 0 && lockflag == 0)//Զ���³���SET��
                    {
                        //display_char16x32(7*16-2,7*5+1*8,Zhi3_16x32,8 & DispSetSel);
                        lockflag = 1;
                        if(KeyHoldStusA)
                        {
                            lock = 1;//����
                        }
                        if(KeyHoldStus)
                        {
                            lock = 0;
                        }
                        DIS_INT;
                        STMFLASH_Write(FLASH_SAVE_ADDR + 78,&lock,1);			//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
                        EN_INT;  
                    }
                    else
                    {
                        lockflag = 0;
                    }
#endif
                    
				} 
				if(K1Dn)								//K1?
				{
					ClsKDn;
					Moto_PARK_Drv;	                    //ֹͣ
				}
                
#if (SOFTLOCK == 1)                 
                if(lock == 0)
#endif
                {
                    if(K2Up)								//K2(enter)?
                    {
                        ClsKDn;
                        Moto_PARK_Drv;	                    //ֹͣ
                        RmLcDisSel = (RmLcDisSel ^ 0x01);
                        DIS_INT;
                        STMFLASH_Write(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);			//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
                        EN_INT;                    
                    }
                    else if((Klong)&&(KDn==K2))					//K2(enter)����?
                    {
                        ClsKlong;
                        ClsKDn;
                        RmTCnt=0;
                        KTCnt=0;
                        RmLcDisSel = (RmLcDisSel ^ 0x02);
                        DIS_INT;
                        STMFLASH_Write(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);			//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
                        EN_INT;
                    }
                }
                
//                if(PIStus.IN_RCL==1)		        //Զ�̿����������߱����籣�棩
//                {
//                    A_DInSel = 1;
//                    A_DInSelflag = 1;
//                }
//                else                               //Զ��ģ����
//                {
//                    A_DInSel = 0;
//                    A_DInSelflag = 0;
//                }
                    
                if(RmLcDisSel == 1)						//���� 
				{
					 Localdisplay();		            //����
				}
				else if(RmLcDisSel == 0)				//Զ�� �����������������
				{                    
                    if(A_DInSel)
					{
#if (MB_MODE == 0)
                        if(Rs485Way && MbStatus.MbOnline)//���������߿���
                        {
                            if(WMbData.sMbData.MbSwitch == 1)
                            {
                                if((GetMotoPos() < _MoreLdU))
                                {
                                    Moto_FWD_Drv;	//��ת
                                }
                            }
                            else if(WMbData.sMbData.MbSwitch == 2)
                            {
                                if((GetMotoPos() > _LessLdD))
                                {
                                    Moto_REV_Drv;   //��ת
                                }
                            }
                            else if(WMbData.sMbData.MbSwitch == 3)
                            {
                                if(electronicbrake)
                                {
                                    Moto_STOP_Drv;
                                }
                                else
                                {
                                    Moto_PARK_Drv;
                                }
                            }
                        }
                        else
#endif
                        {
                            RemoteQquantitydisplay();		//Զ�̿������к���
                        }
					}
					else
					{
						RemoteAnalogdisplay();          //Զ��ģ�����к���	
					}
				}												
				else									//��ֹ
				{
					Moto_PARK_Drv;
                }
                
                
                if(PIStus.IN_ESD)
                {
                    RelayByte.Byte.RelayESD = 1;
                    if(ESDset == 2)
                    {
                        //����λ
                        if(GetMotoPos() <_MoreLdU) 
                        {
                            Moto_FWD_Drv;//��ת
                        }
                        else
                        {
                            Moto_PARK_Drv;//ֹͣ
                        } 	
                    }
                    else if(ESDset == 3)
                    {
                    //�յ�λ
                        if(GetMotoPos() >_LessLdD) 
                        {
                            Moto_REV_Drv;//��ת
                        }
                        else
                        {
                            Moto_PARK_Drv;//ֹͣ	
                        }
                    }
                    if(ESDset==1)
                    {
                        Moto_PARK_Drv;		//ֹͣ	
                    }
                }
                else
                {
                    RelayByte.Byte.RelayESD = 0;
                }
                
                //4-20mA���
                if(OutCVSel)
                {
                    POStus.MA_OUT = PIStus.FK_IN_Pers * (OutCLmdU - OutCLmdD)/10000 + OutCLmdD;
                }
                else
                {
                    POStus.V_OUT = PIStus.FK_IN_Pers * (OutVLmdU - OutVLmdD)/10000 + OutVLmdD;
                }
                break;
		}
}

/*********************************************************
����������

1:��������,2:��������,3:�߼�����,4:�������.....

*********************************************************/
void dispSetupMenu(void)
{
    switch(DispStep)
	{
        case	DispEnioySetWait:
            Moto_PARK_Drv;
            if((KeyHoldStus==0) && (KeyHoldStusA==0))
            {
                Display_Init();
                ClsKlong;
                ClsKDn;
                DispStep=DispEnioySet;
            }
            break;
        case	DispEnioySet:
            if(K3Dn)									//+
            {
                ClsKDn;
                DispSetSel=DispSetSel<<1;
                if(DispSetSel>=0x10)
                {
                    DispSetSel=1;
                }
                if(language)		//�жϵ�ǰ��ʾ����
                    dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
                else
                    dispSetMenu(DispSetSel);				//��ʾ���Ĳ˵�
            }
            if(K4Dn)									//-
            {
                ClsKDn;
                DispSetSel=DispSetSel>>1;
                if(DispSetSel==0x0000)
                    {
                        DispSetSel=0x0008;
                    }
                if(language)		//�жϵ�ǰ��ʾ����
                    dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
                else
                    dispSetMenu(DispSetSel);				//��ʾ���Ĳ˵�					//
            }
            if(((Klong) && (KDn==K1)) || (K1Dn))			//�Ƿ񷵻��ϼ��˵�
            {
                ClsKDn;
                ClsKlong;
                DispStep=DispWorkStusWait; 
                clear_screen();
                DispSetSel=0;
                FreshTimes = 0;
                dispMain();
                Moto_PARK_Drv;
                //���浽FLash
                SaveDataToFlash();
                ModBusDataInit();               //��ʼ��WMbDataֵ
            }
            if(K2Dn)									//enter?
            {
                ClsKDn;
                switch(DispSetSel)					
                {
                    case 0x0001:	
                            DispStep=DispSet1_1Wait;
                            clear_screen();
                            DispSetSel=1;
                            if(language)
                                disp_EnglishSet1_1_x(DispSetSel);
                            else
                                disp_Set1_1_x(DispSetSel);
                    break;
                    
                    case 0x0002:
                            DispStep=DispSet1_2Wait;
                            clear_screen();
                            DispSetSel=1;
                            disp_Set1_2_x(DispSetSel);						
                    break;
                                    
                    case 0x0004:
                            DispStep=DispSet1_3Wait;
                    break;

                    case 0x0008:
                            DispStep=DispSet1_4Wait;
                            clear_screen();
                            DispSetSel=1;
                            disp_Set1_4_x(DispSetSel);			
                    break;
                    
                }
    /****************
                if(DispSetSel==0x0001)
                {
                    DispStep=DispSet1_1Wait;
                    clear_screen();
                    DispSetSel=1;
                    if(language)
                        disp_EnglishSet1_1_x(DispSetSel);
                    else
                        disp_Set1_1_x(DispSetSel);
                }
                else if(DispSetSel==0x0002)
                {
                    DispStep=DispSet1_2Wait;
                    clear_screen();
                    DispSetSel=1;
                    disp_Set1_2_x(DispSetSel);
                }
                else if(DispSetSel==0x0004)
                {
                    DispStep=DispSet1_3Wait;
    //						clear_screen();
    //						DispSetSel=1;
    //						disp_Set1_3_x(DispSetSel);
                }
                else if(DispSetSel==0x0008)
                {
                    DispStep=DispSet1_4Wait;
                    clear_screen();
                    DispSetSel=1;
                    disp_Set1_4_x(DispSetSel);
                }*/
            }
            break;
    }
}

/*********************************************************
����1����

1:�Զ��궨,2:��λȷ��,3:��λȷ��,4:���ƾ���,5:�ֳ�����,6:Զ������,7:���Ŷ���,8:��ʾ����,9����ѡ��

*********************************************************/
void dispSetMenu1_1(void)
{
//	PosAccuracy_flag=PosAccuracy;	//��ʼ������
	switch(DispStep)
		{
		case	DispSet1_1Wait:
			if((KeyHoldStus==0) && (KeyHoldStusA==0))
            {
                ClsKlong;
                ClsKDn;
                DispStep=DispSet1_1;
                EnSelFlsh=0;
            }
			break;
		case	DispSet1_1:
            
			if(language)
				disp_EnglishSet1_1_x(DispSetSel);	//��ʾӢ�Ĳ˵�
			else
				disp_Set1_1_x(DispSetSel);		//��ʾ���Ĳ˵�
            
			//�������ò���
			BaseSet();
            if(ZeroStatus)
            {
                isZeroTemp = IsEncoderZero(isZeroTemp);
            }
			if(K3Dn)									//+
            {
                ClsKDn;
                if(EnSelFlsh==0)
                {
                    DispSetSel=DispSetSel<<1;
                    if(DispSetSel>=0x0200)
                    {
                        DispSetSel=0x0100;
                    }
                }
            }
			if(K4Dn)									//-
            {
                ClsKDn;
                if(EnSelFlsh==0)
                {
                    DispSetSel=DispSetSel>>1;
                    if(DispSetSel==0x000)
                    {
                        DispSetSel=0x001;
                    }
                }
            }
			if(K2Dn)									//enter?
            {
                ClsKDn;
                clear_screen();
                EnSelFlsh=!EnSelFlsh;					//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
                Moto_PARK_Drv;							//�ͷŵ��
                BaseSet1_STEP =0;
                if(DispSetSel <= 0x004)
                {
                    ClosewayTemp = Closeway | 0x80;         //����ԭ�еĹط�ʽ
                    Closeway = 1;                           //�趨�г�ʱ��Ϊ���عأ��޵�����λ
#if (POWER_MODE == 1)                    
                    PhaseDir = PIStus.PHASE;                //ˢ������
#endif
                }
                if(DispSetSel == 0x001)
                {
                    isZeroTemp = 127;
                    ZeroStatus = 0;
                }
            }
            
//			if(((Klong) && (KDn==K1)) || (KDn==K1))		//�Ƿ񷵻��ϼ��˵�DispBaseSet1x
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//�Ƿ񷵻��ϼ��˵�,����ȡ���ڰ���˸
            {
                ClsKlong;
                ClsKDn;
                Moto_PARK_Drv;							//�ͷŵ��
                
                if(ClosewayTemp & 0x80)
                {
                    BaseSet1_STEP =0;//zhaoxw
                    Closeway = ClosewayTemp & 1;         //����ԭ�еĹط�ʽ
                    ClosewayTemp = 0;
                }
                if(DispSetSel == 0x001)
                {
                    isZeroTemp = 127;
                    ZeroStatus = 0;
                }

                if(EnSelFlsh)
                {
//					clear_screen();
                    
//                    if(BaseSet1_STEP || DispSetSel==0x02 || DispSetSel==0x04)
//                    {
//                        
//                    }
                    
                    DispStep=DispSet1_1Wait;
                    EnSelFlsh=0;
                    if(language)
                        disp_EnglishSet1_1_x(DispSetSel);	//��ʾӢ�Ĳ˵�
                    else
                        disp_Set1_1_x(DispSetSel);		//��ʾ���Ĳ˵�
                }
                else
                {
                    clear_screen();
                    DispSetSel=0x01;
                    if(language)							//�жϵ�ǰ��ʾ����
                        dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
                    else
                        dispSetMenu(DispSetSel);			//��ʾ���Ĳ˵�			
                    EnSelFlsh=0;
                    DispStep=DispEnioySetWait;
                }
            }
			break;
		}

}

/*********************************************************
����2����
(2:��������)
1:���ص����Ͷ�΢��,2:���ص����߶�΢��,3:λ�������Ͷ�΢��,4:λ�������߶�΢��,5:�������

*********************************************************/
void dispSetMenu1_2(void)
{
	switch(DispStep)
	{
		case	DispSet1_2Wait:
			if((KeyHoldStus==0) && (KeyHoldStusA==0))
			{
				ClsKlong;
				ClsKDn;
				DispStep=DispSet1_2;
				EnSelFlsh=0;
				TmpSetVal = 0;
			}
			break;
		case	DispSet1_2:
			disp_Set1_2_x(DispSetSel);
			//�������ò���
			FeedBackSet();
			if(K3Dn)									//+
			{
				ClsKDn;
				if(EnSelFlsh==0)
				{
					DispSetSel=DispSetSel<<1;
					if(DispSetSel>=0x0040)
					{
						DispSetSel=1;
					}
				}
			}
			if(K4Dn)									//-
			{
				ClsKDn;
				if(EnSelFlsh==0)
				{
					DispSetSel=DispSetSel>>1;
					if(DispSetSel==0x00)
					{
						DispSetSel=0x20;
					}
				}
			}
			if(K2Dn)									    //enter?
            {
                ClsKDn;
                if(DispSetSel == 0x20)
                {
                    DispStep = DispSet2_6_Wait;			    //�������ͨ������
                    EnSelFlsh=0;
                }
                else
                {
                    EnSelFlsh=!EnSelFlsh;					//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
                    
                    if((EnSelFlsh)&&(DispSetSel==0x10))		//����
                    {
                        if(OutCVSel)
                        {
                            TmpSetVal = OutCLmdU;
                        }
                        else
                        {
                            TmpSetVal = OutVLmdU;
                        }
                    }
                    if((EnSelFlsh)&&(DispSetSel==0x08)) 	//����
                    {
                        if(OutCVSel)
                        {
                            TmpSetVal = OutCLmdD;
                        }
                        else
                        {
                            TmpSetVal = OutVLmdD;
                        }
                    }
                }
            }
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//�Ƿ񷵻��ϼ��˵�,����ȡ���ڰ���˸
			{
				ClsKlong;
				ClsKDn;
				if(EnSelFlsh)
				{
					DispStep=DispSet1_2Wait;
					EnSelFlsh=0;
					disp_Set1_2_x(DispSetSel);
				}
				else
				{
					clear_screen();
					DispSetSel=0x02;
					if(language)		//�жϵ�ǰ��ʾ����
						dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
					else
						dispSetMenu(DispSetSel);			//��ʾ���Ĳ˵�		
					EnSelFlsh=0;
					DispStep=DispEnioySetWait;
				}
			}
		break;
	}
}

/*********************************************************
����1.3����
(3:�߼�����)
3:�߼�����==>1:ESD����,2: �رշ�ʽ,3: �رշ���,4: ���߿���,5: ��תʱ��,6, ����ɲ��,7: �������,8: ��������,9: �ָ�����
*********************************************************/
void dispSetMenu1_3(void)
{
	static u8 i;
	u8 a,b,c,d;
	switch(DispStep)
		{
		case	DispSet1_3Wait:
			if((KeyHoldStus==0) && (KeyHoldStusA==0))
			{
				ClsKlong;
				ClsKDn;
				DispStep=DispSet1_3Wait_PassWord;
				EnSelFlsh=0;
				clear_screen();
			}
			break;
		case	DispSet1_3Wait_PassWord:										//��������
			display_char32x32(32,1*10-3,Shu32x32,0);
			display_char32x32(32,2*10-3,Ru32x32,0);
			display_char32x32(32,3*10-3,Mi32x32,0);
			display_char32x32(32,4*10-3,Ma32x32,0);
			switch(i)					
			{
				case 0:	a=0;b=0,c=0,d=1;				
				break;

				case 1:	a=0;b=0,c=1,d=0;				
				break;

				case 2:	a=0;b=1,c=0,d=0;				
				break;

				case 3:	a=1;b=0,c=0,d=0;				
				break;
			}
			dispayNum32x32(64,1*10-3,BCD_buffer[3],a);
			dispayNum32x32(64,2*10-3,BCD_buffer[2],b);
			dispayNum32x32(64,3*10-3,BCD_buffer[1],c);
			dispayNum32x32(64,4*10-3,BCD_buffer[0],d);
			
			if(KeyHoldStus || KeyHoldStusA)
			{
				if(K4Dn)							//+
				{
					ClsKDn;
					BCD_buffer[i]++;
					if(BCD_buffer[i]>9)
					{
						BCD_buffer[i]=0;
					}
					
				}
				if(K3Dn)							//-
				{
					ClsKDn;
					BCD_buffer[i]--;
					if(BCD_buffer[i]==255)
					{
						BCD_buffer[i]=9;
					}
				}
			}	

			if(K2Dn||(Klong && (KDn==2)))									//���� enter?ȷ��
			{
				ClsKDn;
				if(Klong)
				{
					if(BCDtoDec(BCD_buffer) == PassWord)						//������
					{
						DispStep=DispSet1_3;
						DispSetSel=1;
						clear_screen();
						disp_Set1_3_x(DispSetSel);
					}
					else
					{	//������ʾ
						display_char14x14(106,0*5+16,Mi_14x14+28*0,0);
						display_char14x14(106,1*5+16,Ma_14x14+28*1,0);
						display_char14x14(106,2*5+16,Cuo_14x14+28*2,0);
						display_char14x14(106,3*5+16,Wu1_14x14+28*3,0);
						display_char14x14(106,4*5+16,tan_14x14+28*4,0);
					}
				}
				else
				{
					i++;
					if(i>=4)
					{
						i=0;
					}
				}
			}
//			if(((Klong) && (KDn==K1)) || (KDn==K1)) 	//�Ƿ񷵻��ϼ��˵�
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//�Ƿ񷵻��ϼ��˵�,����ȡ���ڰ���˸
			{
				ClsKlong;
				ClsKDn;
				clear_screen();
				DispSetSel=4;
				if(language)		//�жϵ�ǰ��ʾ����
					dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
				else
					dispSetMenu(DispSetSel);			//��ʾ���Ĳ˵�		
				EnSelFlsh=0;
				DispStep=DispEnioySetWait;
			}
			break;
		case	DispSet1_3:
			disp_Set1_3_x(DispSetSel);						//�߼��˵�����			
			//�߼����ò���
			AdvancedSet();		
			if(K3Dn)									//+
			{
				ClsKDn;
				if(EnSelFlsh==0)
				{
					DispSetSel=DispSetSel<<1;
					if(DispSetSel>=0x0800)
					{
						DispSetSel=0x0400;
					}
				}
			}
			if(K4Dn)									//-
			{
				ClsKDn;
				if(EnSelFlsh==0)
				{
					DispSetSel=DispSetSel>>1;
					if(DispSetSel==0x00)
					{
						DispSetSel=0x01;
					}
				}
			}
			if(K2Dn)									//enter?
			{
				ClsKDn;
				EnSelFlsh=!EnSelFlsh;					//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
				clear_screen();
			}
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//�Ƿ񷵻��ϼ��˵�,����ȡ���ڰ���˸
			{
				ClsKlong;
				ClsKDn;
				if(EnSelFlsh)
				{
					EnSelFlsh=0;
					clear_screen();
				}
				else
				{
					clear_screen();
					DispSetSel=0x04;
					if(language)		//�жϵ�ǰ��ʾ����
						dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
					else
						dispSetMenu(DispSetSel);			//��ʾ���Ĳ˵�		
					EnSelFlsh=0;
					DispStep=DispEnioySetWait;
				}
			}
		break;
	}
}

/*********************************************************
����1.4����
(4:�������)
4:�������==>1: ��ʽ����,2: ��������,3: Զ�̴�,4: Զ�̹ر�,5: Զ�̱���,6: Զ���Զ�,//7: �ָ�����,//8: ����汾

*********************************************************/
void dispSetMenu1_4(void)
{
	switch(DispStep)
	{
		case	DispSet1_4Wait:
				if((KeyHoldStus==0) && (KeyHoldStusA==0))
				{
					ClsKlong;
					ClsKDn;
					DispStep=DispSet1_4;
				}
				break;
		
		case	DispSet1_4:
				disp_Set1_4_x(DispSetSel);
					//����������ò���
				FaultDiagnosisSet();
				if(K3Dn)									//+
				{
					ClsKDn;
					if(EnSelFlsh==0)
					{
						DispSetSel=DispSetSel<<1;
						if(DispSetSel>=0x0100)
						{
							DispSetSel=0x80;
						}
					}
				}
				if(K4Dn)									//-
				{
					ClsKDn;
					if(EnSelFlsh==0)
					{
						DispSetSel=DispSetSel>>1;
						if(DispSetSel==0x00)
						{
							DispSetSel=0x01;
						}
					}
				}
				if(K2Dn)									//enter?
				{
					ClsKDn;
					EnSelFlsh=!EnSelFlsh;					//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
					clear_screen();
				}
				if(((Klong) && (KDn==K1)) || (K1Dn)) 		//�Ƿ񷵻��ϼ��˵�,����ȡ���ڰ���˸
				{
					ClsKlong;
					ClsKDn;
					if(EnSelFlsh)
					{
						DispStep=DispSet1_4Wait;
						EnSelFlsh=0;
						clear_screen();
//						disp_Set1_4_x(DispSetSel);
					}
					else
					{
						clear_screen();
						DispSetSel=0x08;
						if(language)		//�жϵ�ǰ��ʾ����
							dispEnglishSetMenu(DispSetSel);	//��ʾӢ�Ĳ˵�
						else
							dispSetMenu(DispSetSel);			//��ʾ���Ĳ˵�	
						
						EnSelFlsh=0;
						DispStep=DispEnioySetWait;
					}
				}
				break;
		}
}

/*******************************************************

���ͨ���趨

*******************************************************/
void dispSetMenu1_2_6(void)
{
	u16 n,m;
	switch(DispStep)
	{
		case DispSet2_6_Wait:
			if((KeyHoldStus==0) && (KeyHoldStusA==0))
			{
				ClsKlong;
				ClsKDn;
				DispStep = DispSet2_6;
				EnSelFlsh=0;
				TmpSetVal = RelayType[0];							//װ���һ������
				DispSetSel = 1;
				clear_screen();
				if(language)
				{
					disp_Set1_6_xEnglish(DispSetSel);
				}
				else
				{
				      disp_Set1_6_x(DispSetSel);
				}
			}
			break;
		case DispSet2_6:
			if(language)						//���ͨ���趨�˵���ʾ
			{
				disp_Set1_6_xEnglish(DispSetSel);
			}
			else
			{
			      disp_Set1_6_x(DispSetSel);
			}					
			if(K3Dn)									//+
			{
				ClsKDn;
				if(EnSelFlsh==0)
				{
					DispSetSel = DispSetSel <<1;
					if(DispSetSel >= 0x200)
					{
						DispSetSel=0x100;
					}
					m=1;
					for(n = 0; n<9; n++)
					{
							if(DispSetSel & m) TmpSetVal = RelayType[n]; //�ҳ���ѡ��Ĳ���-->TmpSetVal
							m = m<<1;
					}
				}
				else
				{
					TmpSetVal += 1;if(TmpSetVal > 12)TmpSetVal = 0;
				}
			}
			if(K4Dn)									//-
			{
				ClsKDn;
				if(EnSelFlsh==0)
				{
					DispSetSel=DispSetSel>>1;
					if(DispSetSel==0x00)
					{
						DispSetSel=0x001;
					}
					m=1;
					for(n = 0; n<9; n++)
					{
							if(DispSetSel & m) TmpSetVal = RelayType[n]; //�ҳ���ѡ��Ĳ���-->TmpSetVal
							m = m<<1;
					}
				}
				else
				{
					TmpSetVal -= 1;if(TmpSetVal > 250)TmpSetVal = 12;
				}
			}
			if(K2Dn)									//enter?
			{
				ClsKDn;
				
				m=1;
				for(n = 0; n<9; n++)
				{
						if(DispSetSel & m) RelayType[n] = TmpSetVal; //TmpSetVal-->��ѡ��Ĳ���
						m = m<<1;
				}
				EnSelFlsh=!EnSelFlsh;					//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
			}
			if(((Klong) && (KDn==K1)) || (K1Dn)) 		//�Ƿ񷵻��ϼ��˵�
			{
				ClsKlong;
				ClsKDn;
				if(EnSelFlsh)
				{
					EnSelFlsh=0;
				}
				else
				{
					DispStep=DispSet1_2Wait;
					EnSelFlsh=0;
					DispSetSel=0x20;
					clear_screen();
					disp_Set1_2_x(DispSetSel);
				}
			}
			break;
		}	
}


/*********************************************************
function name: ������������ʾ

fun	:����ѡ��
*********************************************************/
void dispMain(void)
{	
	u32 i;
	u32 l;	
	u16 PK,PK1,PK2,NewX;
    u8 ErrView;
    static u8 clr;

    PK=PIStus.FK_IN_Pers;
    //������Ϣ��ʾ
	if(POStus.AL)									//����?
	{                
        NewX = 13;
        if(POStus.AL & ERRDIR)
        {
            ErrView = _ZhuanXiang;
        }

        if(POStus.AL & ERRCMD)
        {
            ErrView = _KaiBiOn;
        }
        
        if(POStus.AL & SHUTBLOCK)
        {
            ErrView = _BiDu;
        }

        if(POStus.AL & OPENBLOCK)
        {
            ErrView = _KaiDu;
        }

        if(POStus.AL & VMALOW)
        {
            ErrView = _DiuXin;
        }

        if(POStus.AL & ERROTS)
        {
            ErrView = _KaiGuoJu;
        }

        if(POStus.AL & ERRCTS)
        {
            ErrView = _BiGuoJu;
        }
        if(POStus.AL & INTTHIGH)
        {
            ErrView = _NeiChaoWen;
        }
        if(POStus.AL & EXTTHIGH)
        {
            ErrView = _WaiChaoWen;
        }
        if(POStus.AL & PHASELOST)
        {
            ErrView = _QueXiang;
        }
	}
    else
    {
        NewX = 21;
        ErrView = 0;
    }
	
	{
        if((PIStus.FK_IN_Pers & 0x7fff) > (10000 - PosAccuracy))
        {
            if(Closeway == 0)
            {
                PK = 10000;
                StusView=_QuanKai;
                LED2 = 0;
                LED3 = 1;
            }
            else //��0~100%���� �����ڹ��عأ����ڱ���
            {
                if(PIStus.OTS || PIStus.CTS)				
                {
                    StusView=_QuanKai;
                    LED2 = 0; 
                    LED3 = 1;
                }
            }
        }
        else if((PIStus.FK_IN_Pers & 0x8000)||(PIStus.FK_IN_Pers & 0x7fff) < PosAccuracy)					//����Ϊ��ֵ
        {
            if(Closeway == 0)
            {
                PK = 0;
                StusView=_QuanBi;
                LED2 = 1;
                LED3 = 0;
            }
            else
            {
                if(PIStus.OTS || PIStus.CTS)				
                {
                    StusView=_QuanBi;
                    LED2 = 1; 
                    LED3 = 0;
                }
            }
        }
        else if(((PIStus.FK_IN_Pers & 0x7fff) <= (5000  + PosAccuracy)) && ((PIStus.FK_IN_Pers & 0x7fff)>= (5000 - PosAccuracy))) //�Ƿ��м�λ
        {
            StusView=_ZhongWei;
        }
        else if((0 == A_DInSel) && (GetMotoPos() == _InTarget)) 	//�Ƿ�λ
        {
            StusView=_DaoWei;
            LED2 = 1;
            LED3 = 1;
        }
        else
        {
            StusView=_TingZhi;
            LED2 = 1;
            LED3 = 1;
           
            if(Moto_FWD_Chk) 
            {
                if(1 == CloseDirection)
                {
                    StusView=_Kaifa;
                }
                else
                {
                    StusView=_BiFa;
                }
                LockedRotor();	
            }
            else if(Moto_REV_Chk)
            {
                if(1 == CloseDirection)
                {
                    StusView=_BiFa;
                }
                else
                {
                    StusView=_Kaifa;
                }
                LockedRotor();	//��ת�ж�
            }
            else if(Moto_STOP_Chk)								//ɲ��
            {
                StusView=_TingZhi;
            }
            else if(Moto_PARK_Chk)								//ֹͣ
            {
                StusView=_TingZhi;
            }
        }   
	}
       
    //���Ͻ���ʾ״̬��Ϣ				
    display_char16x32(22,12,InfoDispStrSet[StusView][0],0);
    display_char16x32(22,17,InfoDispStrSet[StusView][1],0);
    
	if(FreshTimes-- == 0)
    {                                
        FreshTimes = 15;
        
        //��λ�ٷ�ֵ��ʾ
        display_Num_32x32_LZND(57,0,PK& 0x7fff,4 &DispSetSel);

        if(PIStus.FK_IN_Pers & 0x8000)
        {
            //��ֵ����ʾ"-"
            if(BCD_buffer[4])
            {
                display_char16x32(30,5,ChrEf_16x32,4 & DispSetSel);
            }
            else if(BCD_buffer[3])
            {
                display_char16x32(30,5,ChrEf_16x32,4 & DispSetSel);
            }
            else if(BCD_buffer[2])
            {
                display_char16x32(30,5,ChrEf_16x32,4 & DispSetSel);
            }
            else if(BCD_buffer[1])
            {
                display_char16x32(30,5,ChrEf_16x32,4 & DispSetSel);
            }
            //��ֵ�������Ƚ�������ʾһ����λ
    //		for(l = 0; l < 8; l ++)
    //		{
    //			set_lcd_address(97 + l,3);
    //			display_3pixels(0xffff);
    //		}
            PIStus.FK_IN_Pers = 0;
        }
        else
        {
            display_char16x32(30,5,ChrSP_16x32,4 & DispSetSel);//ȥ������
        }
        
        //��ֵ����������ʾ8���ؿ�Ƚ�����
        for(l = 0; l < 8; l ++)
        {
            set_lcd_address(100 + l,3);
            for(i = 3; i < 50; i ++)	//һ�з�54���㣬ÿ����ռ��������(����������54����ʾ)
            {
                if(((i < ((PIStus.FK_IN_Pers & 0x7fff) * 0.47 /100) + 3) && (GetMotoPos() != _InTarget)&&(i>3)) || ((i < ((PIStus.FK_IN_Pers & 0x7fff) * 0.47 /100) + 3) && (GetMotoPos() == _InTarget)&&(i>3)))
                {
                    display_3pixels(0xffff);
                }
                else
                {
                    display_3pixels(0x0000);
                }
            }
        }
        

        //����ֵ��ʾ3���ؿ�Ƚ�����
        for(l = 0; l < 3; l ++)
        {
            set_lcd_address(110 + l,3);
            for(i = 3; i < 50; i ++)	//һ�з�54���㣬ÿ����ռ��������(����������54����ʾ)
            {
                if((i < (((PIStus.VMA_IN_Pers & 0x7fff) * 0.47 /100) +3))&&(i>3))
                {
                    display_3pixels(0xffff);
                }
                else
                {
                    display_3pixels(0x0000);
                }
            }
        }
    }
    
    //��ʾ���ű߿�
    for(l = 0; l < 1; l ++)
    {
        set_lcd_address(97 + l,3);
        for(i = 3; i < 50; i ++)			//һ�з�54���㣬ÿ����ռ��������(����������54����ʾ)
        {
            if(i < 50)
            {
                display_3pixels(0xffff);
            }
            else
            {
                display_3pixels(0x0000);
            }
        }
        set_lcd_address(114 + l,3);
        for(i = 3; i < 50; i ++)	//һ�з�54���㣬ÿ����ռ��������(����������54����ʾ)
        {
            if(i < 50)
            {
                display_3pixels(0xffff);
            }
            else
            {
                display_3pixels(0x0000);
            }
        }
    }
    //��ʾ���ű߿���������,��
    for(l = 0; l < 18; l ++)
    {
        set_lcd_address(97 + l,3);
        display_3pixels(0xFFFF);
        set_lcd_address(97 + l,50);
        display_3pixels(0xFFFF);
    }
    //��ʾ���ȷ��м������
    for(l = 0; l < 17; l ++)
    {
        set_lcd_address(97 + l,26);
        if(l & 0x1)
            display_3pixels(0x5B6D);
        else
            if(l)display_3pixels(0x0000);	
        
    }
    
	//���½���ʾ����ֵ%
//	hextobcd((PIStus.VMA_IN*1000)/186-30);		//ֱ�� ��ʾ4-20MA�����ź�	
//	hextobcd((PIStus.VMA_IN/183)*1000);		//ֱ�� ��ʾ4-20MA�����ź�	
/*	hextobcd(PIStus.VMA_IN_Pers);			 	 //�԰ٷֱȵķ�ʽ ��ʾ4-20MA�����ź�
	if((BCD_buffer[4]==0) )
	{
		display_char16x32(7*16,0*5+_coloffset-3,Sp32x32,2 & DispSetSel);
		dispayNum16x32(7*16,1*5+_coloffset-3,BCD_buffer[3],2 & DispSetSel);
		display_char16x32(7*16,2*5+_coloffset-3,ChrPoin_16x32,2 & DispSetSel);
		dispayNum16x32(7*16,3*5+_coloffset-6,BCD_buffer[2],2 & DispSetSel);		//ʮ��λ
//		dispayNum16x32(7*16,4*5+_coloffset-6,BCD_buffer[1],2 & DispSetSel);		//��ȷ���ٷ�λ
	}
	else
	{
		dispayNum16x32(7*16,0*5+_coloffset-3,BCD_buffer[4],2 & DispSetSel);
		dispayNum16x32(7*16,1*5+_coloffset-3,BCD_buffer[3],2 & DispSetSel);
		display_char16x32(7*16,2*5+_coloffset-3,ChrPoin_16x32,2 & DispSetSel);
		dispayNum16x32(7*16,3*5+_coloffset-6,BCD_buffer[2],2 & DispSetSel);		//ʮ��λ
//		dispayNum16x32(7*16,4*5+_coloffset-6,BCD_buffer[1],2 & DispSetSel);		//��ȷ���ٷ�λ
	}		*/
#if (DISTEMP == 1)
    //���Ͻ���ʾ�¶�
    if(CPUtemp < 0)
    {
        CPUtemp = CPUtemp*-1;
        display_char8x16(23,33,"-",2 & DispSetSel);
    }
    hextobcd(CPUtemp*10);
    dispayNum8x16(23,36,BCD_buffer[3],2 & DispSetSel);
    dispayNum8x16(23,39,BCD_buffer[2],2 & DispSetSel);
    displaySymbol_8x16(23,42,0,2 & DispSetSel);
    if(CPUtemp < 0)
    {
        CPUtemp = CPUtemp * (-1);
    }
#endif

/********************************������ʾ**************************************/    
	PK1=PIStus.VMA_IN_Pers/10;
	PK2=PIStus.VMA_IN_Pers/10;
	PK=(PK1+PK2)/2*10;
	hextobcd(PK);
    /**************************************************************************/
    if(BCD_buffer[4])
	{
        //1*16+5,29
		dispayNum8x16(39,     27,BCD_buffer[4],2 & DispSetSel);
		dispayNum8x16(39,     30,BCD_buffer[3],2 & DispSetSel);
		dispayNum8x16(39,     33,BCD_buffer[2],2 & DispSetSel);
		displaySymbol_8x16(39,36,3,2 & DispSetSel);
		dispayNum8x16(39,     39,BCD_buffer[1],2 & DispSetSel);
	}
	else if((BCD_buffer[3]==0) && (BCD_buffer[4]==0))
	{
		displaySymbol_8x16(39,27,2,2 & DispSetSel);
        displaySymbol_8x16(39,30,2,2 & DispSetSel);
		dispayNum8x16(39,     33,BCD_buffer[2],2 & DispSetSel);
		displaySymbol_8x16(39,36,3,2 & DispSetSel);
		dispayNum8x16(39,     39,BCD_buffer[1],2 & DispSetSel);
	}
	else
	{
		displaySymbol_8x16(39,27,2,2 & DispSetSel);	
        dispayNum8x16(39,     30,BCD_buffer[3],2 & DispSetSel);
		dispayNum8x16(39,     33,BCD_buffer[2],2 & DispSetSel);
		displaySymbol_8x16(39,36,3,2 & DispSetSel);
		dispayNum8x16(39,     39,BCD_buffer[1],2 & DispSetSel);
	}
    displaySymbol_8x16(40,    42,1,2 & DispSetSel);	
/******************************************************************************/
	
    //���½�
    if(NewX == 13)//����
    {
        if(clr != 1)
        {
            clr = 1;
            display_char16x32(120,21,Sp_16x32,8 & DispSetSel);//���֮ǰ��Ϣ
            display_char16x32(120,26,Sp_16x32,8 & DispSetSel);
        }
        display1_char16x16(125,25,ErrItem[ErrView][0],0);
        display1_char16x16(125,30,ErrItem[ErrView][1],0);
        display1_char16x16(125,35,ErrItem[ErrView][2],0);
        display1_char16x16(125,40,ErrItem[ErrView][3],0);
	}
    if(NewX == 21)
    {
        if(clr != 2)
        {
            clr = 2;
            display_char16x32(120,13,Sp_16x32,8 & DispSetSel);
            display_char16x32(120,18,Sp_16x32,8 & DispSetSel);
            display1_char16x16(125,25,SP1_16x16,0);
            display1_char16x16(125,30,SP1_16x16,0);
            display1_char16x16(125,35,SP1_16x16,0);
            display1_char16x16(125,40,SP1_16x16,0);
        }
    }
    
//    if(clr == 1)//�л�ʱ�������Ӧλ�����ݣ���ֹ��˸
//    {
//        clr = 0;
//        display_char16x32(120,13,Sp_16x32,8 & DispSetSel);
//		display_char16x32(120,18,Sp_16x32,8 & DispSetSel);
//    }
	if(RmLcDisSel==0)		//Զ��
	{
		display_char16x32(120,NewX,Yuan_16x32,8 & DispSetSel);
		display_char16x32(120,NewX + 5,Cheng_16x32,8 & DispSetSel);
	}
	else if(RmLcDisSel==1)	//����
	{
		display_char16x32(120,NewX,Ben_16x32,8 & DispSetSel);
		display_char16x32(120,NewX + 5,Di_16x32,8 & DispSetSel);
	}
	else					//��ֹ
	{
		display_char16x32(120,NewX,Jin_16x32,8 & DispSetSel);
		display_char16x32(120,NewX + 5,Zhi3_16x32,8 & DispSetSel);
	}
    
#if (POWER_MODE == 1 && POWER_MODE_DISP == 1)//������ʾ����
    if(1 == CloseDirection)
    {
        display_char14x14(24,35,Zheng_14x14,0);
    }
    else
    {
        display_char14x14(24,35,Fan_14x14,0);
    }
#endif
    
#if (SOFTLOCK == 1)
    if(lock)//����
    {
        display_char16x16(120,6,LOCK_16x16,0);
    }
    else
    {
        display_char16x16(120,6,SP_16x16,0);
    }
#endif
#if ((MB_MODE < 2)&&(DISTEMP == 1))
    if(MbStatus.MbOnline)
    {
        display_char16x16(22,24,NET_16x16,0);
    }
    else
    {
        display_char16x16(22,24,SP_16x16,0);
    }
#elif ((MB_MODE < 2)&&(DISTEMP == 0))
    if(MbStatus.MbOnline)
    {
        display_char16x16(22,39,NET_16x16,0);
    }
    else
    {
        display_char16x16(22,39,SP_16x16,0);
    }
#endif
	//��Բ��ʾ
/*	w=w+1;
	if(w>100)w=0;
	if(w<=(PIStus.FK_IN_Pers/100))
		{
//		display_char8x16(JinDuTab[w][1],JinDuTab[(w/8)*8][0],"*",0);
//		display_char8x16(JinDuTab[w][1],JinDuTab[w][0]," ",0);
		display_char8x16(JinDuTab[w][1],JinDuTab[(w/1)*1][0]," ",0);
		}
	else
		{
//		display_char8x16(JinDuTab[w][1],JinDuTab[(w/8)*8][0]," ",0);
//		display_char8x16(JinDuTab[w][1],JinDuTab[(w/1)*1][0]," ",0);
		display_char8x16(JinDuTab[w][1],JinDuTab[(w/1)*1][0]," ",1);
		}
*/
}

/*********************************************************
function name: ��ʾ�������˵�
fun	:
*********************************************************/
void dispSetMenu(u16 Sel)
{
	display_char16x32(16,2*5,Chr1_16x32,0x01 & Sel);
	display_char16x32(16,3*5,ChrPoin_16x32,0x01 & Sel);
	display_char16x32(16,4*5,Ji_16x32,0x01 & Sel);
	display_char16x32(16,5*5,Ben_16x32,0x01 & Sel);
	display_char16x32(16,6*5,She_16x32,0x01 & Sel);
	display_char16x32(16,7*5,Zhi_16x32,0x01 & Sel);
	
	display_char16x32(48,2*5,Chr2_16x32,0x02 & Sel);
	display_char16x32(48,3*5,ChrPoin_16x32,0x02 & Sel);
	display_char16x32(48,4*5,Fan_16x32,0x02 & Sel);
	display_char16x32(48,5*5,Kui_16x32,0x02 & Sel);
	display_char16x32(48,6*5,She_16x32,0x02 & Sel);
	display_char16x32(48,7*5,Zhi_16x32,0x02 & Sel);
	
	display_char16x32(80,2*5,Chr3_16x32,0x04 & Sel);
	display_char16x32(80,3*5,ChrPoin_16x32,0x04 & Sel);
	display_char16x32(80,4*5,Gao_16x32,0x04 & Sel);
	display_char16x32(80,5*5,Ji1_16x32,0x04 & Sel);
	display_char16x32(80,6*5,She_16x32,0x04 & Sel);
	display_char16x32(80,7*5,Zhi_16x32,0x04 & Sel);
	
	display_char16x32(112,2*5,Chr4_16x32,0x08 & Sel);
	display_char16x32(112,3*5,ChrPoin_16x32,0x08 & Sel);
	display_char16x32(112,4*5,Gu_16x32,0x08 & Sel);
	display_char16x32(112,5*5,Zhang_16x32,0x08 & Sel);
	display_char16x32(112,6*5,Zhen_16x32,0x08 & Sel);
	display_char16x32(112,7*5,Duan_16x32,0x08 & Sel);

}

/*********************************************************
function name: ��ʾӢ�����˵�
fun	:		��ʾӢ�����˵�
*********************************************************/
void dispEnglishSetMenu(u16 Sel)
{

	u16 sl;
//	clear_screen();
	if(Fsh500mS && EnSelFlsh)														//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
    {
        sl=0;				
    }
	else
	{	
        sl=Sel;
    }

    // ��ʾ1.Basic setup
    display_char8x16(2*15+2,9,"1.BasicSet",0x01 & sl);// 

    // ��ʾ2.Feedback set
    display_char8x16(3*15+4,9,"2.FeedbSet",0x02 & sl);// 

    // ��ʾ3.advanced	setup
    display_char8x16(4*15+6,9,"3.AdvancSet",0x04 & sl);// 

    // ��ʾ4.Fault diagnosis		FDD-- ���ϼ�����
    display_char8x16(5*15+8,9,"4.FDD",0x08 & sl);// 
	
}


/*********************************************************
function name: 14x14���һ��
fun	:		����14x14��С������������		x :Ҫ�����һ��
*********************************************************/
void ClearLine_14x14(u16 x)
{
	u8 z ;
	for(z=0;z<10;z++)
	{
		display_char14x14(x,z*5+_coloffset-3,SP_14x14,0);
	}
}

/*********************************************************
function name:16x16���һ��
fun	:		����16x16��С������������		x :Ҫ�����һ��
*********************************************************/
void ClearLine_16x16(u16 x)
{
	u8 z ;
	for(z=0;z<10;z++)
	{
		display_char16x16(x,z*5+_coloffset-3,SP_16x16,0);
	}
}


/*********************************************************
function name: ���Ļ������ý���

fun	:1:�Զ��궨,2:��λȷ��,3:��λȷ��,4:���ƾ���,5:�ֳ�����,6:Զ������,7:���Ŷ���,8:��ʾ����,9����ѡ��
*********************************************************/
void disp_Set1_1_x(u16 Sel)
{
    u16 Temp;
    
    if(Fsh500mS && EnSelFlsh)						//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		Sel=0;				//
	}
		//������ᴦ��
	if((Sel==0x40)||(Sel==0x100))
	{
		RolStus=1;
	}
	if((Sel==0x001)||(Sel==0x020))
	{
		RolStus=2;
	}
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&((DispSetSel==0x80) ||(DispSetSel==0x100)))
	{
	}
	else
	{	
		if(RolStus == 1)
		{
            display_char8x9(jump+line*4+8,3,XZYE_8x9,0);
			display_char8x9(jump+line*3+6,3,WXYE_8x9,0);
            
            // 7�ֳ�����
			display_char14x14(jump+line*1+2,0*5+_coloffset-3,Xian_14x14,0x40 & Sel);
			display_char14x14(jump+line*1+2,1*5+_coloffset-3,Chang_14x14,0x40 & Sel);
			display_char14x14(jump+line*1+2,2*5+_coloffset-3,Kong_14x14,0x40 & Sel);
			display_char14x14(jump+line*1+2,3*5+_coloffset-3,Zhi1_14x14,0x40 & Sel);				
			if(JogHolAutoSelflag==1)
			{
				display_char14x14(jump+line*1+2,5*5+_coloffset-6,Dian_14x14,0x40 & Sel);
				display_char14x14(jump+line*1+2,6*5+_coloffset-6,Dong_14x14,0x40 & Sel);
			}
			else if(JogHolAutoSelflag==2)
			{
				display_char14x14(jump+line*1+2,5*5+_coloffset-6,Bao_14x14,0x40 & Sel);
				display_char14x14(jump+line*1+2,6*5+_coloffset-6,Chi_14x14,0x40 & Sel);
			}
			else if(JogHolAutoSelflag==3)
			{
				display_char14x14(jump+line*1+2,5*5+_coloffset-6,Zi_14x14,0x40 & Sel);
				display_char14x14(jump+line*1+2,6*5+_coloffset-6,Dong_14x14,0x40 & Sel);
			}
            
			// 8���Ŷ���
			//ClearLine_14x14(jump+line*2+2);
			display_char14x14(jump+line*2+2,0*5+_coloffset-3,Diu_14x14,0x80 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset-3,Xin_14x14,0x80 & Sel);
			display_char14x14(jump+line*2+2,2*5+_coloffset-3,Dong_14x14,0x80 & Sel);
			display_char14x14(jump+line*2+2,3*5+_coloffset-3,Zuo_14x14,0x80 & Sel);
			if(NoSignalCtrSelflag==1)
			{
				display_char14x14(jump+line*2+2,5*5+_coloffset-6,Ting_14x14,0x80 & Sel);
				display_char14x14(jump+line*2+2,6*5+_coloffset-6,Zhi_14x14,0x80 & Sel);
			}
			else if(NoSignalCtrSelflag==2)
			{
				display_char14x14(jump+line*2+2,5*5+_coloffset-6,Quan_14x14,0x80 & Sel);
				display_char14x14(jump+line*2+2,6*5+_coloffset-6,Kai_14x14,0x80 & Sel);
			}
			else if(NoSignalCtrSelflag==3)
			{
				display_char14x14(jump+line*2+2,5*5+_coloffset-6,Quan_14x14,0x80 & Sel);
				display_char14x14(jump+line*2+2,6*5+_coloffset-6,Bi_14x14,0x80 & Sel);
			}
			
			// 9��ʾ����	
			//ClearLine_14x14(jump+line*3+4);
			display_char14x14(jump+line*3+4,0*5+_coloffset-3,Xian1_14x14,0x100 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset-3,Shi1_14x14,0x100 & Sel);
			display_char14x14(jump+line*3+4,2*5+_coloffset-3,Fang_14x14,0x100 & Sel);
			display_char14x14(jump+line*3+4,3*5+_coloffset-3,Xiang_14x14,0x100 & Sel);
			if(ScrSelflag==0)
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset-6,Zheng_14x14,0x100 & Sel);
				display_char14x14(jump+line*3+4,6*5+_coloffset-6,Xiang_14x14,0x100 & Sel);
			}
			else
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset-6,Fan_14x14,0x100 & Sel);
				display_char14x14(jump+line*3+4,6*5+_coloffset-6,Xiang_14x14,0x100 & Sel);
			}
			
			// 10����ѡ��			
			//ClearLine_14x14(jump+line*4+6);
			display_char14x14(jump+line*4+6,0*5+_coloffset-3,Yu_14x14,0x200 & Sel);
			display_char14x14(jump+line*4+6,1*5+_coloffset-3,Yan_14x14,0x200 & Sel);
			display_char14x14(jump+line*4+6,2*5+_coloffset-3,Xuan_14x14,0x200 & Sel);
			display_char14x14(jump+line*4+6,3*5+_coloffset-3,Ze_14x14,0x200 & Sel);
			if(languageflag)
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset-6,Ying_14x14,0x200 & Sel);
				display_char14x14(jump+line*4+6,6*5+_coloffset-6,Wen_14x14,0x200 & Sel);
			}
			else
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset-6,Zhong_14x14,0x200 & Sel);
				display_char14x14(jump+line*4+6,6*5+_coloffset-6,Wen_14x14,0x200 & Sel);
			}	
			//ClearLine_14x14(jump+line*1);	
			//ClearLine_14x14(jump+line*5+8);
			//ClearLine_14x14(jump+line*6+10);
		}	
		else
		{
            if(AvVal[_FK] < 10)
            {
                Temp = EncoderCurr;
            }
            else
            {
                Temp = AvVal[_FK];
            }
            display_char8x9(jump+line*3+6,3,XZYE_8x9,0);
			display_char8x9(jump+line*4+8,3,WXYE_8x9,0);
			// 1�Զ��궨
			//ClearLine_14x14(jump+line*1);
			display_char14x14(jump+line*1,0*5+_coloffset-3,Zi_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset-3,Dong_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,2*5+_coloffset-3,Biao_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,3*5+_coloffset-3,Ding_14x14,0x01 & Sel);	
            if((DispSetSel==0x01) && (EnSelFlsh))
			{
				dispay4Num8x14(jump+line*1,5*5+_coloffset-3,Temp,0x02 & Sel);	//����ֵ
			}
			else
			{
                display_char8x14(jump+line*1,5*5+_coloffset-3+0*3,10,0);//
                display_char8x14(jump+line*1,5*5+_coloffset-3+1*3,10,0);//
                display_char8x14(jump+line*1,5*5+_coloffset-3+2*3,10,0);//
                display_char8x14(jump+line*1,5*5+_coloffset-3+3*3,10,0);//
			}

			// 2��λȷ��
			display_char14x14(jump+line*2+2,0*5+_coloffset-3,Kai_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset-3,Wei_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,2*5+_coloffset-3,Que_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,3*5+_coloffset-3,Ren_14x14,0x02 & Sel);	
			if((DispSetSel==0x02) && (EnSelFlsh))
			{
				dispay4Num8x14(jump+line*2+2,5*5+_coloffset-3,Temp,0x02 & Sel);	//����ֵ
			}
			else
			{
				dispay4Num8x14(jump+line*2+2,5*5+_coloffset-3,PosLmdU,0x02 & Sel);		//��λ��ֵ
			}
			// 3��λȷ��
			display_char14x14(jump+line*3+4,0*5+_coloffset-3,Guan_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset-3,Wei_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,2*5+_coloffset-3,Que_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,3*5+_coloffset-3,Ren_14x14,0x04 & Sel);
			if((DispSetSel==0x04) && (EnSelFlsh))
			{
				dispay4Num8x14(jump+line*3+4,5*5+_coloffset-3,Temp,0x04 & Sel);			//����ֵ
			}
			else
			{
				
				dispay4Num8x14(jump+line*3+4,5*5+_coloffset-3,PosLmdD,0x04 & Sel);			//��λֵ
			}
           
			// 4�رշ�ʽ
			display_char14x14(jump+line*4+6,0*5+_coloffset-3,Guan_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,1*5+_coloffset-3,Bi_14x14,0x08 & Sel);	
			display_char14x14(jump+line*4+6,2*5+_coloffset-3,Fang_14x14,0x08 & Sel);	
			display_char14x14(jump+line*4+6,3*5+_coloffset-3,Shi_14x14,0x08 & Sel);
			if(Closewayset)
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset-3,Li_14x14,0x08 & Sel);
				display_char14x14(jump+line*4+6,6*5+_coloffset-3,Ju_14x14,0x08 & Sel);
			}
			else
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset-3,Xian2_14x14,0x08 & Sel);
				display_char14x14(jump+line*4+6,6*5+_coloffset-3,Wei_14x14,0x08 & Sel);
			}

			// 5�رշ���
			display_char14x14(jump+line*5+8,0*5+_coloffset-3,Guan_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,1*5+_coloffset-3,Bi_14x14,0x10 & Sel);	
			display_char14x14(jump+line*5+8,2*5+_coloffset-3,Fang_14x14,0x10 & Sel);	
			display_char14x14(jump+line*5+8,3*5+_coloffset-3,Xiang_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,6*5+_coloffset-3,Xiang_14x14,0x10 & Sel);
			if(CloseDirectionflag )
			{
				display_char14x14(jump+line*5+8,5*5+_coloffset-3,Zheng_14x14,0x10 & Sel);
			}
			else
			{
				display_char14x14(jump+line*5+8,5*5+_coloffset-3,Fan_14x14,0x10 & Sel);
			}
			
			// 6Զ�̿���
			//ClearLine_14x14(jump+line*6+10);
			display_char14x14(jump+line*6+10,0*5+_coloffset-3,Yuan_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,1*5+_coloffset-3,Cheng_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,2*5+_coloffset-3,Kong_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,3*5+_coloffset-3,Zhi1_14x14,0x20 & Sel);
			if(A_DInSelflag==0)
			{
				display_char14x14(jump+line*6+10,5*5+_coloffset-3,Mo_14x14,0x20 & Sel);
				display_char14x14(jump+line*6+10,6*5+_coloffset-3,Ni_14x14,0x20 & Sel);
			}
			else if(A_DInSelflag==1)
			{
				display_char14x14(jump+line*6+10,5*5+_coloffset-3,Kai_14x14,0x20 & Sel);
				display_char14x14(jump+line*6+10,6*5+_coloffset-3,Guan_14x14,0x20 & Sel);
			}
            else
			{
				display_char14x14(jump+line*6+10,5*5+_coloffset-3,Xian3_14x14,0x20 & Sel);
				display_char14x14(jump+line*6+10,6*5+_coloffset-3,Kong_14x14,0x20 & Sel);
			}
		}
	}
}

/*********************************************************
function name: Ӣ�Ļ������ý���

fun	:1:�Զ��궨,2:��λȷ��,3:��λȷ��,4:���ƾ���,5:�ֳ�����,6:Զ������,7:���Ŷ���,8:��ʾ����,9����ѡ��
*********************************************************/
void disp_EnglishSet1_1_x(u16 Sel)
{
    if(Fsh500mS && EnSelFlsh)						//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		Sel=0;				
	}
	//������ᴦ��

    if(Sel==0x100)
	{
		RolStus=1;
	}
	if(Sel==0x001)
	{
		RolStus=2;
	}
    
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&((DispSetSel==0x80) ||(DispSetSel==0x100)))
	{
	}
	else
	{
		if(RolStus == 1)
		{
			// 1�Զ��궨
			ClearLine_16x16(0*15+2);
			display_char8x16(0*15+2,9,"ATG",0x01 & Sel);
			// 2��λȷ��
			display_char8x16(1*15+3,9,"OLCF",0x02 & Sel);
			if((DispSetSel==0x02) && (EnSelFlsh))
			{
					display_4Num_8x16(1*15+3,27,AvVal[_FK],0x02 & Sel);
			}
			else
			{
					display_4Num_8x16(1*15+3,27,PosLmdU,0x02 & Sel);
			}
			
			// 3��λȷ��
			display_char8x16(2*15+4,9,"CLCF",0x04 & Sel);
			if((DispSetSel==0x04) && (EnSelFlsh))
			{
				display_4Num_8x16(2*15+4,27,AvVal[_FK],0x04 & Sel);
			}
			else
			{
				display_4Num_8x16(2*15+4,27,PosLmdD,0x04 & Sel);
			}
			
			// 4���ƾ���
			display_char8x16(3*15+5,9,"NPC",0x08 & Sel);
			display_Num_8x16(3*15+5,27,PosAccuracy/10,0x08 & Sel);
			
			// 5�ֳ�����
			display_char8x16(4*15+6,9,"PRSC",0x10 & Sel);
			if(JogHolAutoSel==0)
			{
				display_char8x16(4*15+6,27,"jog",0x10 & Sel);	//�㶯
			}
			else if(JogHolAutoSel==1)
			{
				display_char8x16(4*15+6,27,"KBO",0x10 & Sel);	//����
			}
			else
			{
				display_char8x16(4*15+6,27,"ACD",0x10 & Sel);	//�Զ�
			}
			
			// 6Զ�̿���
			display_char8x16(5*15+7,9,"LRM",0x20 & Sel);
			if(A_DInSelflag)
			{
				display_char8x16(5*15+7,27,"ADR",0x20 & Sel);	//ģ��
			}
			else
			{
				display_char8x16(5*15+7,27,"SRS",0x20 & Sel);	//����
			}
			
			// 7���Ŷ���
			display_char8x16(6*15+8,9,"CLP",0x40 & Sel);
			if(NoSignalCtrSel==0)
			{
				display_char8x16(6*15+8,27,"DSP",0x40 & Sel);	//ֹͣ
			}
			else if(NoSignalCtrSel==1)
			{
				display_char8x16(6*15+8,27,"FOT",0x40 & Sel);	//ȫ��
			}
			else
			{
				display_char8x16(6*15+8,27,"FCT",0x40 & Sel);	//ȫ��
			}
			
			// 8��ʾ����
			display_char8x16(7*15+9,9,"DDS",0x80 & Sel);
			
			//����ϴζ������ʾ
//				display_char16x16(128,6*5+_coloffset-3,Sp32x32,0x00 & Sel);
			if(ScrSelflag==0)
				{
				display_char8x16(7*15+9,27,"R.S.+",0x80 & Sel);	//��
				}
			else
				{
				display_char8x16(7*15+9,27,"B.S.+",0x80 & Sel);	//��
				}
			
				// 9����ѡ��
			display_char8x16(8*15+10,9,"OSL",0x100 & Sel);	
			//����ϴε���ʾ�ۼ�
			display_char8x16(8*15+10,34-6,"   ",0x00 & Sel);
			if(languageflag)
				{
					display_char8x16(8*15+10,27,"en",0x100 & Sel);	//����
				}
			else
				{
					display_char8x16(8*15+10,27,"cn",0x100 & Sel);	//����
				}	
			//������һ��
			display_char8x16(9*15+10,4,"                   ",0x00 & Sel);
		}
		else
		{
			//�����0��
			display_char8x16(0*15+2,4,"                    ",0x00 & Sel);
			// 1�Զ��궨
			ClearLine_16x16(1*15+2);
			display_char8x16(1*15+2,9,"ATG",0x01 & Sel);
			
			// 2��λȷ��
			display_char8x16(2*15+3,9,"OLCF",0x02 & Sel);
			if((DispSetSel==0x02) && (EnSelFlsh))
			{
				display_4Num_8x16(2*15+3,27,AvVal[_FK],0x02 & Sel);
			}
			else
			{
				display_4Num_8x16(2*15+3,27,PosLmdU,0x02 & Sel);
			}
		
			// 3��λȷ��
			display_char8x16(3*15+4,9,"CLCF",0x04 & Sel);
			if((DispSetSel==0x04) && (EnSelFlsh))
			{
				display_4Num_8x16(3*15+4,27,AvVal[_FK],0x04 & Sel);
			}
			else
			{
				display_4Num_8x16(3*15+4,27,PosLmdD,0x04 & Sel);
			}
				
			// 4���ƾ���
			display_char8x16(4*15+5,9,"NPC",0x08 & Sel);
			display_Num_8x16(4*15+5,27,PosAccuracy/10,0x08 & Sel);
				
			// 5�ֳ�����
			display_char8x16(5*15+6,9,"PRSC",0x10 & Sel);
			if(JogHolAutoSel==0)
			{
				display_char8x16(5*15+6,27,"jog",0x10 & Sel);	//�㶯
			}
			else if(JogHolAutoSel==1)
			{
				display_char8x16(5*15+6,27,"KBO",0x10 & Sel);	//����
			}
			else
			{
				display_char8x16(5*15+6,27,"ACD",0x10 & Sel);	//�Զ�
			}
				
			// 6Զ�̿���ģʽ
			display_char8x16(6*15+7,9,"LRM",0x20 & Sel);
			if(A_DInSelflag)
			{
				display_char8x16(6*15+7,27,"ADR",0x20 & Sel);	//ģ��
			}
			else
			{
				display_char8x16(6*15+7,27,"SRS",0x20 & Sel);	//����
			}
			
			// 7���Ŷ���
			display_char8x16(7*15+8,9,"CLP",0x40 & Sel);
			if(NoSignalCtrSel==0)
			{
				display_char8x16(7*15+8,27,"DSP",0x40 & Sel);	//ֹͣ
			}
			else if(NoSignalCtrSel==1)
			{
                display_char8x16(7*15+8,27,"FOT",0x40 & Sel);	//ȫ��
			}
			else
			{
				display_char8x16(7*15+8,27,"FCT",0x40 & Sel);	//ȫ��
			}
			
			// 8��ʾ����
			display_char8x16(8*15+9,9,"DDS",0x80 & Sel);
			
			//����ϴζ������ʾ
//				display_char16x16(128,6*5+_coloffset-3,Sp32x32,0x00 & Sel);//
			if(ScrSelflag==0)
			{
				display_char8x16(8*15+9,27,"R.S.+",0x80 & Sel);	//��
			}
			else
			{
                display_char8x16(8*15+9,27,"B.S.+",0x80 & Sel);	//��
			}
			
				// 9����ѡ��
			display_char8x16(9*15+10,9,"OSL",0x100 & Sel);	
			if(languageflag)
			{
				display_char8x16(9*15+10,27,"en",0x100 & Sel);	//Ӣ��
			}
			else
			{
				display_char8x16(9*15+10,27,"cn",0x100 & Sel);	//����
			}	
		}
	}
}

/*********************************************************
�������ý���

Sel		:ѡ�񷴰���ʾ��Ŀ,1:���ص����Ͷ�΢��,2:���ص����߶�΢��,3:λ�������Ͷ�΢��,4:λ�������߶�΢��,5:�������,

*********************************************************/
void disp_Set1_2_x(u16 Sel)
{
    if(Fsh500mS && EnSelFlsh)														//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		Sel=0;				//
	}
	display_char8x9(jump+line*3+16,3,XZYE_8x9,0);
	// 1 �ź�(����)
	display_char14x14(jump+line*1,0*5+_coloffset-6,Xin_14x14,0x01 & Sel);
	display_char14x14(jump+line*1,1*5+_coloffset-6,Hao_14x14,0x01 & Sel);	
	display_char14x14(jump+line*1,2*5+_coloffset-6,Xuan_14x14,0x01 & Sel);
	display_char14x14(jump+line*1,3*5+_coloffset-6,Ze_14x14,0x01 & Sel);
	display_char14x14(jump+line*1,4*5+_coloffset-3,Dian1_14x14,0x01 & Sel);		
	if(OutCVSelflag)
	{		
		display_char14x14(jump+line*1,5*5+_coloffset-3,Liu_14x14,0x01 & Sel);	
//		display_char14x14(jump+line*1,6*5+_coloffset-3,Chrm_14x14,0x01 & Sel);
//		display_char14x14(jump+line*1,7*5+_coloffset-3,ChrA_14x14,0x01 & Sel);
        display_char8x16(jump+line*1,6*5+_coloffset-3,"mA",0x01 & Sel);
	}
	else
	{		
		display_char14x14(jump+line*1,5*5+_coloffset-3,Ya_14x14,0x01 & Sel);	
//		display_char14x14(jump+line*1,6*5+_coloffset-3,ChrV_14x14,0x01 & Sel);
//		display_char14x14(jump+line*1,7*5+_coloffset-3,SP_14x14,0);
        display_char8x16(jump+line*1,6*5+_coloffset-3,"V ",0x01 & Sel);
	}
	// 2 //��������4mA/��������0V
	display_char14x14(jump+line*2+2,0*5+_coloffset-6,Gei_14x14,0x02 & Sel);
	display_char14x14(jump+line*2+2,1*5+_coloffset-6,Ding_14x14,0x02 & Sel);	
	display_char14x14(jump+line*2+2,2*5+_coloffset-6,Di1_14x14,0x02 & Sel);	
	display_char14x14(jump+line*2+2,3*5+_coloffset-6,Xin_14x14,0x02 & Sel);	
/*
	if((EnSelFlsh) && (DispSetSel==0x02))
		{
		if(OutCVSel)
			{
			display_Num_8x16(48,4*5+_coloffset, PIStus.VMA_IN * 12000/AvVal[_IV]/150, 0x02 & Sel);			//������ĵ�ѹ����ֵ(V)=PIStus.VMA_IN * 1200/AvVal[_IV]����������ֵ(mA)=��ѹ����ֵ*1000/150ŷ 
			}
		else
			{
			display_Num_8x16(48,4*5+_coloffset, PIStus.VMA_IN * 1.2 * (51 + 22) * 10/AvVal[_IV]/22, 0x02 & Sel);	//������ĵ�ѹ����ֵ(V)(AD�ڴ�)=PIStus.VMA_IN * 1200/AvVal[_IV]��������ĵ�ѹ����ֵ(V)(����ڴ�)=PIStus.VMA_IN * 1.2 * (51 + 22)/AvVal[_IV]/22��
			}																											//51,22Ϊ��ѹ����ֵ��*10����õ�һλС��
		}
	else
		{
		if(OutCVSel)
			{
			display_Num_8x16(48,4*5+_coloffset, CInLmdD * 12000/AvVal[_IV]/150, 0x02 & Sel);				//4mA�趨ֵ
			}
		else
			{
			display_Num_8x16(48,4*5+_coloffset, VInLmdD * 12000/AvVal[_IV]/150, 0x02 & Sel);					//0V�趨ֵ
			}																										
		}
*/	
	if((EnSelFlsh) && (DispSetSel==0x02))
	{
		if(OutCVSelflag)
		{
			if(PIStus.VMA_IN > CInLmdD)
            {
                display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, 40 + (PIStus.VMA_IN - CInLmdD) * 160 /(CInLmdU - CInLmdD), 0x02 & Sel);
            } 
            else
            {
                display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, 40 - (CInLmdD - PIStus.VMA_IN) * 160 /(CInLmdU - CInLmdD), 0x02 & Sel);
            }    
		}
		else
		{
			if(PIStus.VMA_IN > CInLmdD)
            {
                display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, (PIStus.VMA_IN - VInLmdD) * 100 /(VInLmdU - VInLmdD), 0x02 & Sel);
            } 
            else
            {
                display_char8x16(jump+line*2+1,3*5+_coloffset-3,"-",0x02 & Sel);//-
                display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, (VInLmdD - PIStus.VMA_IN) * 100 /(VInLmdU - VInLmdD), 0x02 & Sel);
            }  
        }																											
	}
	else
	{
		if(OutCVSelflag)
		{
			display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, 40, 0x02 & Sel);							//4mA�����Ӧ��PWMռ�ձ�
		}
		else
		{
			display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, 0, 0x02 & Sel);								//0V�����Ӧ��PWMռ�ձ�
		}																										
	}


	// 3 //��������20mA/��������10V
	display_char14x14(jump+line*3+4,0*5+_coloffset-6,Gei_14x14,0x04 & Sel);
	display_char14x14(jump+line*3+4,1*5+_coloffset-6,Ding_14x14,0x04 & Sel);	
	display_char14x14(jump+line*3+4,2*5+_coloffset-6,Gao_14x14,0x04 & Sel);	
	display_char14x14(jump+line*3+4,3*5+_coloffset-6,Xin_14x14,0x04 & Sel);	
/*
	if((EnSelFlsh) && (DispSetSel==0x04))
		{
		if(OutCVSel)
			{
			display_Num_8x16(64,4*5+_coloffset, PIStus.VMA_IN * 12000/AvVal[_IV]/150, 0x04 & Sel);						//������ĵ�ѹ����ֵ(V)=PIStus.VMA_IN * 1200/AvVal[_IV]����������ֵ(mA)=��ѹ����ֵ*1000/150ŷ 
			}
		else
			{
			display_Num_8x16(64,4*5+_coloffset, PIStus.VMA_IN * 1.2 * (51 + 22) * 10/AvVal[_IV]/22, 0x04 & Sel);		//������ĵ�ѹ����ֵ(V)(AD�ڴ�)=PIStus.VMA_IN * 1200/AvVal[_IV]��������ĵ�ѹ����ֵ(V)(����ڴ�)=PIStus.VMA_IN * 1.2 * (51 + 22)/AvVal[_IV]/22��
			}																											//51,22Ϊ��ѹ����ֵ��*10����õ�һλС��
		}
	else
		{
		if(OutCVSel)
			{
			display_Num_8x16(64,4*5+_coloffset, CInLmdU * 12000/AvVal[_IV]/150, 0x04 & Sel);							//20mA�趨ֵ
			}
		else
			{
			display_Num_8x16(64,4*5+_coloffset, VInLmdU * 12000/AvVal[_IV]/150, 0x04 & Sel);							//10V�趨ֵ
			}																											
		}
*/		
	
	if((EnSelFlsh) && (DispSetSel==0x04))
	{
		if(OutCVSelflag)
		{
			display_Num_8x16(jump+line*3+3,4*5+_coloffset-3, 40 + (PIStus.VMA_IN - CInLmdD) * 160 /(CInLmdU - CInLmdD), 0x04 & Sel);								
		}
		else
		{
			display_Num_8x16(jump+line*3+3,4*5+_coloffset-3, (PIStus.VMA_IN - VInLmdD) * 100 /(VInLmdU - VInLmdD), 0x04 & Sel);							
		}																											
	}
	else
	{
		if(OutCVSelflag)
		{
            display_Num_8x16(jump+line*3+3,4*5+_coloffset-3, 200, 0x04 & Sel);										//20mA�����Ӧ��PWMռ�ձ�
		}
		else
		{
			display_Num_8x16(jump+line*3+3,4*5+_coloffset-3, 100, 0x04 & Sel);										//10V�����Ӧ��PWMռ�ձ�
		}																											
	}
	
	// 4 �������4mA/��������0V
	display_char14x14(jump+line*4+6,0*5+_coloffset-6,Shu_14x14,0x08 & Sel);
	display_char14x14(jump+line*4+6,1*5+_coloffset-6,Chu_14x14,0x08 & Sel);	
	display_char14x14(jump+line*4+6,2*5+_coloffset-6,Di1_14x14,0x08 & Sel);	
	display_char14x14(jump+line*4+6,3*5+_coloffset-6,Xin_14x14,0x08 & Sel);	
	if((EnSelFlsh) && (DispSetSel==0x08))
    {
		if(OutCVSelflag)
        {
            display_Num_8x16(jump+line*4+5,4*5+_coloffset-3, (POStus.MA_OUT - OutCLmdD) * 200 /(OutCLmdU - OutCLmdD), 0x08 & Sel);									//
        }
		else
        {
            display_Num_8x16(jump+line*4+5,4*5+_coloffset-3, (POStus.V_OUT - OutVLmdD) * 100 /(OutVLmdU - OutVLmdD), 0x08 & Sel);									//
        }
    }
	else
    {
		if(OutCVSelflag)
		{
			display_Num_8x16(jump+line*4+5,4*5+_coloffset-3, 40, 0x08 & Sel);							//4mA�����Ӧ��PWMռ�ձ�
		}
		else
		{
			display_Num_8x16(jump+line*4+5,4*5+_coloffset-3, 0, 0x08 & Sel);								//0V�����Ӧ��PWMռ�ձ�
        }																											
    }

	
	// 5 �������20mA/10V
	display_char14x14(jump+line*5+8,0*5+_coloffset-6,Shu_14x14,0x10 & Sel);
	display_char14x14(jump+line*5+8,1*5+_coloffset-6,Chu_14x14,0x10 & Sel);	
	display_char14x14(jump+line*5+8,2*5+_coloffset-6,Gao_14x14,0x10 & Sel);	
	display_char14x14(jump+line*5+8,3*5+_coloffset-6,Xin_14x14,0x10 & Sel);	
	if((EnSelFlsh) && (DispSetSel==0x10))
	{
		if(OutCVSelflag)
		{
			display_Num_8x16(jump+line*5+7,4*5+_coloffset-3, (POStus.MA_OUT - OutCLmdD) * 200 /(OutCLmdU - OutCLmdD), 0x10 & Sel);									//
		}
		else
		{
			display_Num_8x16(jump+line*5+7,4*5+_coloffset-3, (POStus.V_OUT - OutVLmdD) * 100 /(OutVLmdU - OutVLmdD), 0x10 & Sel);									//
		}																											
	}
	else 
	{
		if(OutCVSelflag)
		{
			display_Num_8x16(jump+line*5+7,4*5+_coloffset-3, 200, 0x10 & Sel);										//20mA�����Ӧ��PWMռ�ձ�
		}
		else
		{
			display_Num_8x16(jump+line*5+7,4*5+_coloffset-3, 100, 0x10 & Sel);										//10V�����Ӧ��PWMռ�ձ�
		}																											
	}
	// 6 �������
	display_char14x14(jump+line*6+10,0*5+_coloffset-6,Chu1_14x14,0x20 & Sel);
	display_char14x14(jump+line*6+10,1*5+_coloffset-6,Dian_14x14,0x20 & Sel);	
	display_char14x14(jump+line*6+10,2*5+_coloffset-6,Shu_14x14,0x20 & Sel);	
	display_char14x14(jump+line*6+10,3*5+_coloffset-6,Chu_14x14,0x20 & Sel);	
}

/*********************************************************
�߼����ý���

��ʾ=1:����ɲ��,2:�رշ���,3: �رշ�ʽ,4: ESD����5:���߿���,6:��תʱ��, ,7: �������,8: ��������,9: �ָ�����

*********************************************************/
void disp_Set1_3_x(u16 Sel)
{
#define	_coloffset_1_3_x	15
#define 		saze	14    

	if(Fsh500mS && EnSelFlsh)								//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		Sel=0;				
	}
	//������ᴦ��
    if((Sel==0x40)||(Sel==0x400))
	{
		RolStus=1;
	}
	if((Sel==0x001)||(Sel==0x020))
	{
		RolStus=2;
	}
    
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&((DispSetSel==0x200) ||(DispSetSel==0x400)))
	{//ˢ��
	}
	else
	{	
		if(RolStus==1)
		{
			display_char8x9(jump+line*4+8,3,XZYE_8x9,0);
			display_char8x9(jump+line*3+6,3,WXYE_8x9,0);	
            
            // 7���ƾ���
			display_char14x14(jump+line*1,0*5+_coloffset-3,Kong_14x14,0x40 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset-3,Zhi1_14x14,0x40 & Sel);
			display_char14x14(jump+line*1,2*5+_coloffset-3,Jing_14x14,0x40 & Sel);
			display_char14x14(jump+line*1,3*5+_coloffset-3,Du_14x14,0x40 & Sel);		
			
			display_Num_8x16(jump+line*1-3,4*5+_coloffset,PosAccuracy_flag/10,0x40 & Sel);
            
//			// 7�������
//			display_char14x14(jump+line*1,0*5+_coloffset-3,Jian2_14x14,0x40 & Sel);
//			display_char14x14(jump+line*1,1*5+_coloffset-3,Duan_14x14,0x40 & Sel);	
//			display_char14x14(jump+line*1,2*5+_coloffset-3,Yun_14x14,0x40 & Sel);	
//			display_char14x14(jump+line*1,3*5+_coloffset-3,xing_14x14,0x40 & Sel);
//			if(IntermittentRunflag)
//			{
//				display_char14x14(jump+line*1,5*5+_coloffset-3,Shi3_14x14,0x40 & Sel);
//			}
//			else
//			{
//				display_char14x14(jump+line*1,5*5+_coloffset-3,Fou_14x14,0x40 & Sel);
//			}
			
            // 8�м�λ��1
			display_char14x14(jump+line*2,0*5+_coloffset-3,Zhong_14x14,0x80 & Sel);
			display_char14x14(jump+line*2,1*5+_coloffset-3,Jian2_14x14,0x80 & Sel);	
			display_char14x14(jump+line*2,2*5+_coloffset-3,Wei_14x14,0x80 & Sel);	
			display_char14x14(jump+line*2,3*5+_coloffset-3,Zhi2_14x14,0x80 & Sel);
            dispayNum8x14(jump+line*2,4*5+_coloffset-3,1,0x80 & Sel);
			{
				dispay3Num8x14(jump+line*2,4*5+_coloffset-1,Intermediate1flag,0x80 & Sel);	//����ֵ
                display_char8x14(jump+line*2,6*5+_coloffset,11,0x80 & Sel);
            }
            
            // 9�м�λ��2	
			display_char14x14(jump+line*3,0*5+_coloffset-3,Zhong_14x14,0x100 & Sel);
			display_char14x14(jump+line*3,1*5+_coloffset-3,Jian2_14x14,0x100 & Sel);	
			display_char14x14(jump+line*3,2*5+_coloffset-3,Wei_14x14,0x100 & Sel);	
			display_char14x14(jump+line*3,3*5+_coloffset-3,Zhi2_14x14,0x100 & Sel);
            dispayNum8x14(jump+line*3,4*5+_coloffset-3,2,0x100 & Sel);
			{
				dispay3Num8x14(jump+line*3,4*5+_coloffset-1,Intermediate2flag,0x100 & Sel);	//����ֵ
                display_char8x14(jump+line*3,6*5+_coloffset,11,0x100 & Sel);
            }
            
			// 10��������	
			display_char14x14(jump+line*4,0*5+_coloffset-3,	Mi_14x14,0x200 & Sel);
			display_char14x14(jump+line*4,1*5+_coloffset-3,Ma_14x14,0x200 & Sel);	
			display_char14x14(jump+line*4,2*5+_coloffset-3,She_14x14,0x200 & Sel);	
			display_char14x14(jump+line*4,3*5+_coloffset-3,Zhi2_14x14,0x200 & Sel);
			
			// 11�ָ�����
			display_char14x14(jump+line*5,0*5+_coloffset-3,	Hui_14x14,0x400 & Sel);
			display_char14x14(jump+line*5,1*5+_coloffset-3,Fu_14x14,0x400 & Sel);	
			display_char14x14(jump+line*5,2*5+_coloffset-3,Chu_14x14,0x400 & Sel);	
			display_char14x14(jump+line*5,3*5+_coloffset-3,Chang1_14x14,0x400 & Sel);	
		}
		else
		{
			display_char8x9(jump+line*3+6,3,XZYE_8x9,0);
			display_char8x9(jump+line*4+8,3,WXYE_8x9,0);
			// 1����ɲ��
			display_char14x14(jump+line*1,0*5+_coloffset-3,Dian1_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset-3,Zi1_14x14,0x01 & Sel);	
			display_char14x14(jump+line*1,2*5+_coloffset-3,Sha_14x14,0x01 & Sel);	
			display_char14x14(jump+line*1,3*5+_coloffset-3,Che_14x14,0x01 & Sel);
			if(electronicbrakeflag)
			{
				display_char14x14(jump+line*1,5*5+_coloffset-3,Kai_14x14,0x01 & Sel);
			}
			else
			{
				display_char14x14(jump+line*1,5*5+_coloffset-3,Guan_14x14,0x01 & Sel);
			}
			// 2��תʱ��
			//ClearLine_14x14(jump+line*2+2);
			display_char14x14(jump+line*2+2,0*5+_coloffset-3,Du1_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset-3,Zhuan_14x14,0x02 & Sel);	
			display_char14x14(jump+line*2+2,2*5+_coloffset-3,Shi2_14x14,0x02 & Sel);	
			display_char14x14(jump+line*2+2,3*5+_coloffset-3,Jian2_14x14,0x02 & Sel);

			dispay3Num8x14(jump+line*2+2,3*5+_coloffset+4, LockedRotorTime_flag,0x02 & Sel);

            // 3ͨѶ����
			display_char14x14(jump+line*3+4,0*5+_coloffset-3,Tong_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset-3,Xun_14x14,0x04 & Sel);	
			display_char14x14(jump+line*3+4,2*5+_coloffset-3,Kong_14x14,0x04 & Sel);	
			display_char14x14(jump+line*3+4,3*5+_coloffset-3,Zhi1_14x14,0x04 & Sel);		
            
            if(Rs485Wayset)
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset-3,Shi3_14x14,0x04 & Sel);
			}
			else
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset-3,Fou_14x14,0x04 & Sel);
			}
            
            // 4ͨѶ��ַ
			display_char14x14(jump+line*4+6,0*5+_coloffset-3,Tong_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,1*5+_coloffset-3,Xun_14x14,0x08 & Sel);		
			display_char14x14(jump+line*4+6,2*5+_coloffset-3,Di_14x14,0x08 & Sel);	
			display_char14x14(jump+line*4+6,3*5+_coloffset-3,Zhi3_14x14,0x08 & Sel);

			dispay3Num8x14(jump+line*4+6,3*5+_coloffset+4, mbaddr_flag,0x08 & Sel);
            
			// 5	ESD����	
			display_char14x14(jump+line*5+8,0*5+_coloffset-3,ChrE_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,1*5+_coloffset-3,ChrS_14x14,0x10 & Sel);	
			display_char14x14(jump+line*5+8,2*5+_coloffset-3,ChrD_14x14,0x10 & Sel);
            display_char14x14(jump+line*5+8,3*5+_coloffset-3,SP_14x14,0x10 & Sel);            
			switch(ESDsetflag)					//ESD
			{
				case 1:	
					display_char14x14(jump+line*5+8,5*5+_coloffset-3,Ting_14x14,0x10 & Sel);
					display_char14x14(jump+line*5+8,6*5+_coloffset-3,Zhi_14x14,0x10& Sel);
				break;
				case 2:	
					display_char14x14(jump+line*5+8,5*5+_coloffset-3,Kai_14x14,0x10 & Sel);
					display_char14x14(jump+line*5+8,6*5+_coloffset-3,Wei_14x14,0x10 & Sel);
				break;		
				case 3:	
					display_char14x14(jump+line*5+8,5*5+_coloffset-3,Bi_14x14,0x10 & Sel);
					display_char14x14(jump+line*5+8,6*5+_coloffset-3,Wei_14x14,0x10 & Sel);
				break;
			}
			// 6���߿���
			display_char14x14(jump+line*6+10,0*5+_coloffset-3,Liang_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,1*5+_coloffset-3,Xian3_14x14,0x20 & Sel);	
			display_char14x14(jump+line*6+10,2*5+_coloffset-3,Kong_14x14,0x20 & Sel);	
			display_char14x14(jump+line*6+10,3*5+_coloffset-3,Zhi1_14x14,0x20 & Sel);	
			switch(TwoLineflag)					//���߿��ƣ�
			{
				case 1:	
					display_char14x14(jump+line*6+10,5*5+_coloffset-3,Jin_14x14,0x20 & Sel);	//��ֹ
					display_char14x14(jump+line*6+10,6*5+_coloffset-3,Zhi_14x14,0x20 & Sel);
				break;
				
				case 2:	
					display_char14x14(jump+line*6+10,5*5+_coloffset-3,Xin_14x14,0x20 & Sel);	
					display_char14x14(jump+line*6+10,6*5+_coloffset-3,Kai_14x14,0x20 & Sel);//�ſ�
				break;
								
				case 3:	
					display_char14x14(jump+line*6+10,5*5+_coloffset-3,Xin_14x14,0x20 & Sel);	//��ֹ
					display_char14x14(jump+line*6+10,6*5+_coloffset-3,Guan_14x14,0x20 & Sel);//�Ź�
				break;
			}
		}
	}	
}
/*********************************************************
function name: ������Ͻ���	14X14��С����

fun	:��ʾ=1: ��ʽ����,2: ���ؼ��,3: Զ�̴�,4: Զ�̹ر�,5: Զ�̱���,6: Զ���Զ�,7:�ָ�����,8:����汾
*********************************************************/
void disp_Set1_4_x(u16 Sel)
{
#define	_coloffset_1_4	13

	if(Fsh500mS && EnSelFlsh)									//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		Sel=0;				
	}
	//������ᴦ�� 
    if((Sel==0x40)||(Sel==0x80))
	{
		RolStus=1;
	}
	if((Sel==0x001)||(Sel==0x020))
	{
		RolStus=2;
	}
    
	if((EnSelFlsh)&&(DispStep==DispSet1_4)&&((DispSetSel==0x40) ||(DispSetSel==0x80)))
	{
	}
	else
	{
		if(RolStus==1)
		{	
			display_char8x9(jump+line*4+8,3,XZYE_8x9,0);
			display_char8x9(jump+line*3+6,3,WXYE_8x9,0);
			// 7�ָ�����
			//ClearLine_14x14(jump+line*1);	
			//ClearLine_14x14(jump+line*2+2);	
			display_char14x14(jump+line*2+2,0*5+_coloffset_1_4-3,Hui_14x14,0x40 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset_1_4-3,Fu_14x14,0x40 & Sel);
			display_char14x14(jump+line*2+2,2*5+_coloffset_1_4-3,Mi_14x14,0x40 & Sel);
			display_char14x14(jump+line*2+2,3*5+_coloffset_1_4-3,Ma_14x14,0x40 & Sel);	
			
			//8����汾��
			//ClearLine_14x14(jump+line*3+4);	
			display_char14x14(jump+line*3+4,0*5+_coloffset_1_4-3,Ruan_14x14,0x80 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset_1_4-3,Jian1_14x14,0x80 & Sel);
			display_char14x14(jump+line*3+4,2*5+_coloffset_1_4-3,Ban_14x14,0x80 & Sel);
			display_char14x14(jump+line*3+4,3*5+_coloffset_1_4-3,Ben_14x14,0x80 & Sel);
			ClearLine_14x14(jump+line*4+6);	
			ClearLine_14x14(jump+line*5+8);	
			ClearLine_14x14(jump+line*6+10);		
		}
		else
		{
			display_char8x9(jump+line*3+6,3,XZYE_8x9,0);
			display_char8x9(jump+line*4+8,3,WXYE_8x9,0);
			// 1��ʽ����
			display_char14x14(jump+line*1,0*5+_coloffset_1_4-3,Fang_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset_1_4-3,Shi_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,2*5+_coloffset_1_4-3,An_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,3*5+_coloffset_1_4-3,Jian_14x14,0x01 & Sel);				
			if(RmLcDisSel==1)					//����
			{
				display_char14x14(jump+line*1,5*5+_coloffset_1_4-3,Ben_14x14,0);
				display_char14x14(jump+line*1,6*5+_coloffset_1_4-3,Di_14x14,0);
			}
			else if(!RmLcDisSel)					//Զ��
			{
				display_char14x14(jump+line*1,5*5+_coloffset_1_4-3,Yuan_14x14,0);
				display_char14x14(jump+line*1,6*5+_coloffset_1_4-3,Cheng_14x14,0);
			}
			else									//��ֹ
			{
				display_char14x14(jump+line*1,5*5+_coloffset_1_4-3,Jin_14x14,0);
				display_char14x14(jump+line*1,6*5+_coloffset_1_4-3,Zhi_14x14,0);
			}
			
			// 2 ���ؼ��
			display_char14x14(jump+line*2+2,0*5+_coloffset_1_4-3,Li_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset_1_4-3,Ju_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,2*5+_coloffset_1_4-3,Jian3_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,3*5+_coloffset_1_4-3,Ce_14x14,0x02 & Sel);	
			if(1 == CloseDirection)
			{
				if(PIStus.OTS)
				{
					display_char14x14(jump+line*2+2,5*5+_coloffset_1_4-3,Kai_14x14,0);
				}
				else if(PIStus.CTS)
				{
					display_char14x14(jump+line*2+2,5*5+_coloffset_1_4-3,Bi_14x14,0);
				}
				else 
				{
					display_char14x14(jump+line*2+2,5*5+_coloffset_1_4-3,Wu_14x14,0);
				}
			}
			else
			{
				if(PIStus.CTS)
				{
					display_char14x14(jump+line*2+2,5*5+_coloffset_1_4-3,Kai_14x14,0);
				}
				else if(PIStus.OTS)
				{
					display_char14x14(jump+line*2+2,5*5+_coloffset_1_4-3,Bi_14x14,0);
				}
				else 
				{
					display_char14x14(jump+line*2+2,5*5+_coloffset_1_4-3,Wu_14x14,0);
				}
			}
			// 3Զ�̴�
			display_char14x14(jump+line*3+4,0*5+_coloffset_1_4-3,Yuan_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset_1_4-3,Cheng_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,2*5+_coloffset_1_4-3,Da_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,3*5+_coloffset_1_4-3,Kai_14x14,0x04 & Sel);	
			if(!P_IN_OPEN)		//��Զ�̿��ź�
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset_1_4-3,You_14x14,0);
			}
			else
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset_1_4-3,Wu_14x14,0);
			}
			// 4Զ�̹ر�
			display_char14x14(jump+line*4+6,0*5+_coloffset_1_4-3,Yuan_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,1*5+_coloffset_1_4-3,Cheng_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,2*5+_coloffset_1_4-3,Guan_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,3*5+_coloffset_1_4-3,Bi_14x14,0x08 & Sel);	
			if(!P_IN_SHUT)	//��Զ�̹��ź�
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset_1_4-3,You_14x14,0);
			}
			else
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset_1_4-3,Wu_14x14,0);
			}
			// 5Զ�̱���
			display_char14x14(jump+line*5+8,0*5+_coloffset_1_4-3,Yuan_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,1*5+_coloffset_1_4-3,Cheng_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,2*5+_coloffset_1_4-3,Bao_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,3*5+_coloffset_1_4-3,Chi_14x14,0x10 & Sel);		
			if(PIStus.IN_BC)	//��Զ�̱����ź�
			{
				display_char14x14(jump+line*5+8,5*5+_coloffset_1_4-3,You_14x14,0);
			}
			else
			{
				display_char14x14(jump+line*5+8,5*5+_coloffset_1_4-3,Wu_14x14,0);
			}
			// 6Զ��ģ��
			display_char14x14(jump+line*6+10,0*5+_coloffset_1_4-3,Yuan_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,1*5+_coloffset_1_4-3,Cheng_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,2*5+_coloffset_1_4-3,Mo_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,3*5+_coloffset_1_4-3,Ni_14x14,0x20 & Sel);		
			
//          if((PIStus.VMA_IN > (CInLmdD/2)&&(PIStus.VMA_IN<0x0e78)))	//��Զ�̱��Զ��ź� (4-20AMģ���޺�)
//			{
//				display_char14x14(jump+line*6+10,5*5+_coloffset_1_4-3,You_14x14,0);
//			}
//			else
//			{
//				display_char14x14(jump+line*6+10,5*5+_coloffset_1_4-3,Wu_14x14,0);
//			}
            
            if(OutCVSelflag)
			{
                display_Num_8x16(jump+line*6+10,4*5+_coloffset_1_4-3, 40 + PIStus.VMA_IN_Pers * 0.016,0);
                display_char8x16(jump+line*6+10,7*5+_coloffset_1_4-4,"mA",0);
			}
            else
			{
                display_Num_8x16(jump+line*6+10,4*5+_coloffset_1_4-3, PIStus.VMA_IN_Pers * 0.01,0);
//                display_char14x14(jump+line*6+10,7*5+_coloffset_1_4-3,ChrV_14x14,0);
//                display_char14x14(jump+line*6+10,8*5+_coloffset_1_4-3,SP_14x14,0);
                display_char8x16(jump+line*6+10,7*5+_coloffset_1_4-4,"V ",0);
			}	
		}
	}
}


/*********************************************************
function name: ��ʾ�ڲ����� displayInternalParameters
fun	:
*********************************************************/
void displayInterPara(u16 a,u16 b,u16 c,u16 d,u16 e,u16 f,u16 g,u16 h)
{	
	if(DispParaSet_test==1)
	{
		display_4Num_8x16(0,0,a,DispParaSet_test);				
		display_4Num_8x16(0,14,b,DispParaSet_test); 			
		display_4Num_8x16(0,28,c,DispParaSet_test); 			
		display_4Num_8x16(0,42,d,DispParaSet_test); 			
		display_4Num_8x16(144,0,e,DispParaSet_test);				
		display_4Num_8x16(144,14,f,DispParaSet_test);				
		display_4Num_8x16(144,28,g,DispParaSet_test);				
		display_4Num_8x16(144,42,h,DispParaSet_test);				
	}
	else if(DispParaSet_test==2)
	{
		display_4Num_8x16(0,0,a,0);				
		display_4Num_8x16(0,14,b,0); 			
		display_4Num_8x16(0,28,c,0); 			
		display_4Num_8x16(0,42,d,0); 			
		display_4Num_8x16(144,0,e,0);				
		display_4Num_8x16(144,14,f,0);				
		display_4Num_8x16(144,28,g,0);				
		display_4Num_8x16(144,42,h,0);				
	}
	else
	{
		display_char8x16(0,0,"     ",0);				
		display_char8x16(0,14,"     ",0); 			
		display_char8x16(0,28,"     ",0); 			
		display_char8x16(0,42,"      ",0); 			
		display_char8x16(144,0,"     ",0);				
		display_char8x16(144,14,"     ",0);				
		display_char8x16(144,28,"     ",0);				
		display_char8x16(144,42,"     ",0);				
	}
		
}
///*********************************************************
//function name: ʮ���ź����ͨ�����ý���

//fun	:��ʾ=1:MRTS CH,2: MD CH,3:SHUTRUN CH,4: OPENRUN CH,5: RCL CH,6, CTSOUT CH,7: OTSOUT CH,
//8: ACLSOUT CH,9: AOLSOUT CH,10:AL CH CH = 1~10
//*********************************************************/
//void disp_Set1_6_x(u16 Sel)
//{
//#define	_coloffset_1_6	16
//#define 		saze	14

//    u16 sl;

//	if(Fsh500mS && EnSelFlsh)														//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
//	{
//		sl=0;				
//	}
//	else
//		sl=Sel;
//	//������ᴦ��
//    if((Sel==0x40)||(Sel==0x200))
//	{
//		RolStus=1;
//	}
//	if((Sel==0x001)||(Sel==0x020))
//	{
//		RolStus=2;
//	}

//	if(RolStus == 1)
//	{
//		display_char8x9(jump+line*4+8,3,XZYE_8x9,0);
//		display_char8x9(jump+line*3+6,3,WXYE_8x9,0);
//		// 7������
//        //����7
//		//ClearLine_14x14(jump+line*1);
//		//ClearLine_14x14(jump+line*2+2);
//		display_char14x14(jump+line*2+2,0*5+_coloffset_1_6-3,Kai_14x14,0x40 & sl);
//		display_char14x14(jump+line*2+2,1*5+_coloffset_1_6-3,Li_14x14,0x40 & sl);
//		display_char14x14(jump+line*2+2,2*5+_coloffset_1_6-3,Ju_14x14,0x40 & sl);
//		if((Sel & 0x40) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*2+1,5*5+_coloffset_1_6-3,TmpSetVal,0x40 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*2+1,5*5+_coloffset_1_6-3,ChSel[6],0x40 & Sel);
//		}
//		// 8 �ظ���
//        //����8
//		//ClearLine_14x14(jump+line*3+4);
//		display_char14x14(jump+line*3+4,0*5+_coloffset_1_6-3,Guan_14x14,0x80 & sl);
//		display_char14x14(jump+line*3+4,1*5+_coloffset_1_6-3,Fu1_14x14,0x80 & sl);
//		display_char14x14(jump+line*3+4,2*5+_coloffset_1_6-3,Zhu_14x14,0x80 & sl);
//		if((Sel & 0x80) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*3+3,5*5+_coloffset_1_6-3,TmpSetVal,0x80 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*3+3,5*5+_coloffset_1_6-3,ChSel[7],0x80 & Sel);
//		}
//		// 9	������
//        //����9
//		//ClearLine_14x14(jump+line*4+6);
//		display_char14x14(jump+line*4+6,0*5+_coloffset_1_6-3,Kai_14x14,0x100 & sl);
//		display_char14x14(jump+line*4+6,1*5+_coloffset_1_6-3,Fu1_14x14,0x100 & sl);
//		display_char14x14(jump+line*4+6,2*5+_coloffset_1_6-3,Zhu_14x14,0x100 & sl);
//		if((Sel & 0x100) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*4+5,5*5+_coloffset_1_6-3,TmpSetVal,0x100 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*4+5,5*5+_coloffset_1_6-3,ChSel[8],0x100 & Sel);
//		}
//		// 10 ����
//        //����10
//		//ClearLine_14x14(jump+line*5+8);
//		display_char14x14(jump+line*5+8,0*5+_coloffset_1_6-3,BaoJing_14x14+28*0,0x200 & sl);
//		display_char14x14(jump+line*5+8,1*5+_coloffset_1_6-3,BaoJing_14x14+28*1,0x200 & sl);
//		if((Sel & 0x200) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*5+7,5*5+_coloffset_1_6-3,TmpSetVal,0x200 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*5+7,5*5+_coloffset_1_6-3,ChSel[9],0x200 & Sel);
//		}
//		//ClearLine_14x14(jump+line*6+10);
//		//ClearLine_14x14(jump+line*6+8);
//		//ClearLine_14x14(jump+line*7+10);
//	}
//	else
//	{
//		display_char8x9(jump+line*3+6,3,XZYE_8x9,0);
//		display_char8x9(jump+line*4+8,3,WXYE_8x9,0);
//		// 1,�������		
//        //����1
//		//ClearLine_14x14(jump+line*1);
//		display_char14x14(jump+line*1,0*5+_coloffset_1_6-3,YunXingGuoJu_14x14+28*0,0x01 & sl);
//		display_char14x14(jump+line*1,1*5+_coloffset_1_6-3,YunXingGuoJu_14x14+28*1,0x01 & sl);
//		display_char14x14(jump+line*1,2*5+_coloffset_1_6-3,YunXingGuoJu_14x14+28*2,0x01 & sl);
//		display_char14x14(jump+line*1,3*5+_coloffset_1_6-3,YunXingGuoJu_14x14+28*3,0x01 & sl);
//		if(Sel & 1)
//		{
//			dispayNum8x16(jump+line*1-1,5*5+_coloffset_1_6-3,TmpSetVal,(0x01 & sl));
//		}
//		else
//		{
//			dispayNum8x16(jump+line*1-1,5*5+_coloffset_1_6-3,ChSel[0],0x01 & Sel);
//		}
//		// 2 �м�λ��
//        //����2
//		//ClearLine_14x14(jump+line*2+2);
//		display_char14x14(jump+line*2+2,0*5+_coloffset_1_6-3,Zhong_14x14,0x02 & sl);
//		display_char14x14(jump+line*2+2,1*5+_coloffset_1_6-3,Jian2_14x14,0x02 & sl);
//		display_char14x14(jump+line*2+2,2*5+_coloffset_1_6-3,Wei_14x14,0x02 & sl);
//		display_char14x14(jump+line*2+2,3*5+_coloffset_1_6-3,Zhi2_14x14,0x02 & sl);
//		if(Sel & 2)
//		{
//			dispayNum8x16(jump+line*2+1,5*5+_coloffset_1_6-3,TmpSetVal,(0x02 & sl));
//		}
//		else
//		{
//			dispayNum8x16(jump+line*2+1,5*5+_coloffset_1_6-3,ChSel[1],0x02 & Sel);
//		}
//		// 3 ������
//        //����3
//		//ClearLine_14x14(jump+line*3+4);
//		display_char14x14(jump+line*3+4,0*5+_coloffset_1_6-3,Guan_14x14,0x04 & sl);
//		display_char14x14(jump+line*3+4,1*5+_coloffset_1_6-3,Yun_14x14,0x04 & sl);
//		display_char14x14(jump+line*3+4,2*5+_coloffset_1_6-3,xing_14x14,0x04 & sl);
//		if(Sel & 4)
//		{
//			dispayNum8x16(jump+line*3+3,5*5+_coloffset_1_6-3,TmpSetVal,0x04 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*3+3,5*5+_coloffset_1_6-3,ChSel[2],0x04 & Sel);
//		}
//		// 4 ������
//        //����4
//		//ClearLine_14x14(jump+line*4+6);
//		display_char14x14(jump+line*4+6,0*5+_coloffset_1_6-3,Kai_14x14,0x08 & sl);
//		display_char14x14(jump+line*4+6,1*5+_coloffset_1_6-3,Yun_14x14,0x08 & sl);
//		display_char14x14(jump+line*4+6,2*5+_coloffset_1_6-3,xing_14x14,0x08 & sl);
//		if((Sel & 8) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*4+5,5*5+_coloffset_1_6-3,TmpSetVal,0x08 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*4+5,5*5+_coloffset_1_6-3,ChSel[3],0x08 & Sel);
//		}
//		// 5 Զ��/ ����
//        //����5
//		//ClearLine_14x14(jump+line*5+8);
//		display_char14x14(jump+line*5+8,0*5+_coloffset_1_6-3,Yuan_14x14,0x10 & sl);
//		display_char14x14(jump+line*5+8,1*5+_coloffset_1_6-3,Cheng_14x14,0x10 & sl);
//		display_char14x14(jump+line*5+8,2*5+_coloffset_1_6-3,Ben_14x14,0x10 & sl);
//		display_char14x14(jump+line*5+8,3*5+_coloffset_1_6-3,Di_14x14,0x10 & sl);
//		if((Sel & 16) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*5+7,5*5+_coloffset_1_6-3,TmpSetVal,0x10 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*5+7,5*5+_coloffset_1_6-3,ChSel[4],0x10 & Sel);
//		}
//		// 6 ������
//        //����6
//		//ClearLine_14x14(jump+line*6+10);
//		display_char14x14(jump+line*6+10,0*5+_coloffset_1_6-3,Guan_14x14,0x20 & sl);
//		display_char14x14(jump+line*6+10,1*5+_coloffset_1_6-3,Li_14x14,0x20 & sl);
//		display_char14x14(jump+line*6+10,2*5+_coloffset_1_6-3,Ju_14x14,0x20 & sl);
//		if((Sel & 32) && EnSelFlsh)
//		{
//			dispayNum8x16(jump+line*6+9,5*5+_coloffset_1_6-3,TmpSetVal,0x20 & sl);
//		}
//		else
//		{
//			dispayNum8x16(jump+line*6+9,5*5+_coloffset_1_6-3,ChSel[5],0x20 & Sel);
//		}
//	}
//}

/*********************************************************
function name: ʮ���ź����ͨ�����ý���

fun	:��ʾ=1:MRTS CH,2: MD CH,3:SHUTRUN CH,4: OPENRUN CH,5: RCL CH,6, CTSOUT CH,7: OTSOUT CH,
8: ACLSOUT CH,9: AOLSOUT CH,10:AL CH CH = 1~10
*********************************************************/
void disp_Set1_6_x(u16 Sel)
{
#define	_coloffset_1_6	16
#define 		saze	14

    u16 sl;

	if(Fsh500mS && EnSelFlsh)														//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		sl=0;				
	}
	else
		sl=Sel;
	//������ᴦ��
    if((Sel==0x40)||(Sel==0x200))
	{
		RolStus=1;
	}
	if((Sel==0x001)||(Sel==0x020))
	{
		RolStus=2;
	}

	if(RolStus == 1)
	{
		display_char8x9(jump+line*4+8,3,XZYE_8x9,0);
		display_char8x9(jump+line*3+6,3,WXYE_8x9,0);
		
        //����7
		display_char14x14(jump+line*1,0*5+_coloffset_1_6-3,Chu1_14x14,0x40 & sl);
		display_char14x14(jump+line*1,1*5+_coloffset_1_6-3,Dian_14x14,0x40 & sl);
        dispayNum8x14(jump+line*1,    2*5+_coloffset_1_6-3,7,0x40 & sl);
		if((Sel & 0x40) && EnSelFlsh)
		{
            display_char14x14(jump+line*1,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x40 & sl);
            display_char14x14(jump+line*1,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x40 & sl);
            display_char14x14(jump+line*1,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x40 & sl);
            display_char14x14(jump+line*1,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x40 & sl);

		}
		else
		{
            display_char14x14(jump+line*1,4*5+_coloffset-3,RelayItem[RelayType[6]][0],0x40 & Sel);
            display_char14x14(jump+line*1,5*5+_coloffset-3,RelayItem[RelayType[6]][1],0x40 & Sel);
            display_char14x14(jump+line*1,6*5+_coloffset-3,RelayItem[RelayType[6]][2],0x40 & Sel);
            display_char14x14(jump+line*1,7*5+_coloffset-3,RelayItem[RelayType[6]][3],0x40 & Sel);
		}
        
        //����8
		display_char14x14(jump+line*2,0*5+_coloffset_1_6-3,Chu1_14x14,0x80 & sl);
		display_char14x14(jump+line*2,1*5+_coloffset_1_6-3,Dian_14x14,0x80 & sl);
        dispayNum8x14(jump+line*2,    2*5+_coloffset_1_6-3,8,0x80 & sl);
        if((Sel & 0x80) && EnSelFlsh)
		{
            display_char14x14(jump+line*2,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x80 & sl);
            display_char14x14(jump+line*2,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x80 & sl);
            display_char14x14(jump+line*2,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x80 & sl);
            display_char14x14(jump+line*2,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x80 & sl);

		}
		else
		{
            display_char14x14(jump+line*2,4*5+_coloffset-3,RelayItem[RelayType[7]][0],0x80 & Sel);
            display_char14x14(jump+line*2,5*5+_coloffset-3,RelayItem[RelayType[7]][1],0x80 & Sel);
            display_char14x14(jump+line*2,6*5+_coloffset-3,RelayItem[RelayType[7]][2],0x80 & Sel);
            display_char14x14(jump+line*2,7*5+_coloffset-3,RelayItem[RelayType[7]][3],0x80 & Sel);
		}
        
        //����9
		display_char14x14(jump+line*3,0*5+_coloffset_1_6-3,Chu1_14x14,0x100 & sl);
		display_char14x14(jump+line*3,1*5+_coloffset_1_6-3,Dian_14x14,0x100 & sl);
        dispayNum8x14(jump+line*3,    2*5+_coloffset_1_6-3,9,0x100 & sl);
        if((Sel & 0x100) && EnSelFlsh)
		{
            display_char14x14(jump+line*3,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x100 & sl);
            display_char14x14(jump+line*3,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x100 & sl);
            display_char14x14(jump+line*3,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x100 & sl);
            display_char14x14(jump+line*3,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x100 & sl);

		}
		else
		{
            display_char14x14(jump+line*3,4*5+_coloffset-3,RelayItem[RelayType[8]][0],0x100 & Sel);
            display_char14x14(jump+line*3,5*5+_coloffset-3,RelayItem[RelayType[8]][1],0x100 & Sel);
            display_char14x14(jump+line*3,6*5+_coloffset-3,RelayItem[RelayType[8]][2],0x100 & Sel);
            display_char14x14(jump+line*3,7*5+_coloffset-3,RelayItem[RelayType[8]][3],0x100 & Sel);
		}
        
        //�������� �̶�����������̬
//      display_char14x14(jump+line*4,0*5+_coloffset_1_6-3,Bao1_14x14,0x200 & sl);
//		display_char14x14(jump+line*4,1*5+_coloffset_1_6-3,Jing1_14x14,0x200 & sl);
//		display_char14x14(jump+line*4,2*5+_coloffset_1_6-3,Chu1_14x14,0x200 & sl);
//		display_char14x14(jump+line*4,3*5+_coloffset_1_6-3,Dian_14x14,0x200 & sl);
//		if((Sel & 0x200) && EnSelFlsh)
//		{
//            display_char14x14(jump+line*4,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x200 & sl);
//            display_char14x14(jump+line*4,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x200 & sl);
//            display_char14x14(jump+line*4,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x200 & sl);
//            display_char14x14(jump+line*4,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x200 & sl);

//		}
//		else
//		{
//            display_char14x14(jump+line*4,4*5+_coloffset-3,RelayItem[RelayType[9]][0],0x200 & Sel);
//            display_char14x14(jump+line*4,5*5+_coloffset-3,RelayItem[RelayType[9]][1],0x200 & Sel);
//            display_char14x14(jump+line*4,6*5+_coloffset-3,RelayItem[RelayType[9]][2],0x200 & Sel);
//            display_char14x14(jump+line*4,7*5+_coloffset-3,RelayItem[RelayType[9]][3],0x200 & Sel);
//		}
	}
	else
	{
		display_char8x9(jump+line*3+6,3,XZYE_8x9,0);
		display_char8x9(jump+line*4+8,3,WXYE_8x9,0);
        
		//1,�������		
        //����1
		display_char14x14(jump+line*1,0*5+_coloffset_1_6-3,Chu1_14x14,0x01 & sl);
		display_char14x14(jump+line*1,1*5+_coloffset_1_6-3,Dian_14x14,0x01 & sl);
        dispayNum8x14(jump+line*1,    2*5+_coloffset_1_6-3,1,0x01 & sl);
        if((Sel & 1) && EnSelFlsh)
		{
            display_char14x14(jump+line*1,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x01 & sl);
            display_char14x14(jump+line*1,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x01 & sl);
            display_char14x14(jump+line*1,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x01 & sl);
            display_char14x14(jump+line*1,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x01 & sl);

		}
		else
		{
            display_char14x14(jump+line*1,4*5+_coloffset-3,RelayItem[RelayType[0]][0],0x01 & Sel);
            display_char14x14(jump+line*1,5*5+_coloffset-3,RelayItem[RelayType[0]][1],0x01 & Sel);
            display_char14x14(jump+line*1,6*5+_coloffset-3,RelayItem[RelayType[0]][2],0x01 & Sel);
            display_char14x14(jump+line*1,7*5+_coloffset-3,RelayItem[RelayType[0]][3],0x01 & Sel);
		}
        
		// 2 �м�λ��
        //����2
		display_char14x14(jump+line*2,0*5+_coloffset_1_6-3,Chu1_14x14,0x02 & sl);
		display_char14x14(jump+line*2,1*5+_coloffset_1_6-3,Dian_14x14,0x02 & sl);
        dispayNum8x14(jump+line*2,    2*5+_coloffset_1_6-3,2,0x02 & sl);
        if((Sel & 0x02) && EnSelFlsh)
		{
            display_char14x14(jump+line*2,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x02 & sl);
            display_char14x14(jump+line*2,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x02 & sl);
            display_char14x14(jump+line*2,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x02 & sl);
            display_char14x14(jump+line*2,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x02 & sl);

		}
		else
		{
            display_char14x14(jump+line*2,4*5+_coloffset-3,RelayItem[RelayType[1]][0],0x02 & Sel);
            display_char14x14(jump+line*2,5*5+_coloffset-3,RelayItem[RelayType[1]][1],0x02 & Sel);
            display_char14x14(jump+line*2,6*5+_coloffset-3,RelayItem[RelayType[1]][2],0x02 & Sel);
            display_char14x14(jump+line*2,7*5+_coloffset-3,RelayItem[RelayType[1]][3],0x02 & Sel);
		}
        
		// 3 ������
        //����3
		display_char14x14(jump+line*3,0*5+_coloffset_1_6-3,Chu1_14x14,0x04 & sl);
		display_char14x14(jump+line*3,1*5+_coloffset_1_6-3,Dian_14x14,0x04 & sl);
        dispayNum8x14(jump+line*3,    2*5+_coloffset_1_6-3,3,0x04 & sl);
        if((Sel & 0x04) && EnSelFlsh)
		{
            display_char14x14(jump+line*3,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x04 & sl);
            display_char14x14(jump+line*3,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x04 & sl);
            display_char14x14(jump+line*3,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x04 & sl);
            display_char14x14(jump+line*3,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x04 & sl);

		}
		else
		{
            display_char14x14(jump+line*3,4*5+_coloffset-3,RelayItem[RelayType[2]][0],0x04 & Sel);
            display_char14x14(jump+line*3,5*5+_coloffset-3,RelayItem[RelayType[2]][1],0x04 & Sel);
            display_char14x14(jump+line*3,6*5+_coloffset-3,RelayItem[RelayType[2]][2],0x04 & Sel);
            display_char14x14(jump+line*3,7*5+_coloffset-3,RelayItem[RelayType[2]][3],0x04 & Sel);
		}
        
		// 4 ������
        //����4
		display_char14x14(jump+line*4,0*5+_coloffset_1_6-3,Chu1_14x14,0x08 & sl);
		display_char14x14(jump+line*4,1*5+_coloffset_1_6-3,Dian_14x14,0x08 & sl);
        dispayNum8x14(jump+line*4,    2*5+_coloffset_1_6-3,4,0x08 & sl);
        if((Sel & 0x08) && EnSelFlsh)
		{
            display_char14x14(jump+line*4,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x08 & sl);
            display_char14x14(jump+line*4,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x08 & sl);
            display_char14x14(jump+line*4,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x08 & sl);
            display_char14x14(jump+line*4,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x08 & sl);

		}
		else
		{
            display_char14x14(jump+line*4,4*5+_coloffset-3,RelayItem[RelayType[3]][0],0x08 & Sel);
            display_char14x14(jump+line*4,5*5+_coloffset-3,RelayItem[RelayType[3]][1],0x08 & Sel);
            display_char14x14(jump+line*4,6*5+_coloffset-3,RelayItem[RelayType[3]][2],0x08 & Sel);
            display_char14x14(jump+line*4,7*5+_coloffset-3,RelayItem[RelayType[3]][3],0x08 & Sel);
		}
        
		// 5 Զ��/ ����
        //����5
		display_char14x14(jump+line*5,0*5+_coloffset_1_6-3,Chu1_14x14,0x10 & sl);
		display_char14x14(jump+line*5,1*5+_coloffset_1_6-3,Dian_14x14,0x10 & sl);
        dispayNum8x14(jump+line*5,    2*5+_coloffset_1_6-3,5,0x10 & sl);
        if((Sel & 0x10) && EnSelFlsh)
		{
            display_char14x14(jump+line*5,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x10 & sl);
            display_char14x14(jump+line*5,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x10 & sl);
            display_char14x14(jump+line*5,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x10 & sl);
            display_char14x14(jump+line*5,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x10 & sl);

		}
		else
		{
            display_char14x14(jump+line*5,4*5+_coloffset-3,RelayItem[RelayType[4]][0],0x10 & Sel);
            display_char14x14(jump+line*5,5*5+_coloffset-3,RelayItem[RelayType[4]][1],0x10 & Sel);
            display_char14x14(jump+line*5,6*5+_coloffset-3,RelayItem[RelayType[4]][2],0x10 & Sel);
            display_char14x14(jump+line*5,7*5+_coloffset-3,RelayItem[RelayType[4]][3],0x10 & Sel);
		}
        
		// 6 ������
        //����6
		display_char14x14(jump+line*6,0*5+_coloffset_1_6-3,Chu1_14x14,0x20 & sl);
		display_char14x14(jump+line*6,1*5+_coloffset_1_6-3,Dian_14x14,0x20 & sl);
        dispayNum8x14(jump+line*6,    2*5+_coloffset_1_6-3,6,0x20 & sl);
        if((Sel & 0x20) && EnSelFlsh)
		{
            display_char14x14(jump+line*6,4*5+_coloffset-3,RelayItem[TmpSetVal][0],0x20 & sl);
            display_char14x14(jump+line*6,5*5+_coloffset-3,RelayItem[TmpSetVal][1],0x20 & sl);
            display_char14x14(jump+line*6,6*5+_coloffset-3,RelayItem[TmpSetVal][2],0x20 & sl);
            display_char14x14(jump+line*6,7*5+_coloffset-3,RelayItem[TmpSetVal][3],0x20 & sl);
		}
		else
		{
            display_char14x14(jump+line*6,4*5+_coloffset-3,RelayItem[RelayType[5]][0],0x20 & Sel);
            display_char14x14(jump+line*6,5*5+_coloffset-3,RelayItem[RelayType[5]][1],0x20 & Sel);
            display_char14x14(jump+line*6,6*5+_coloffset-3,RelayItem[RelayType[5]][2],0x20 & Sel);
            display_char14x14(jump+line*6,7*5+_coloffset-3,RelayItem[RelayType[5]][3],0x20 & Sel);
		}
	}
}
/*********************************************************
function name: ʮ���ź����ͨ������Ӣ�Ľ���

fun	:��ʾ=1:MRTS CH,2: MD CH,3:SHUTRUN CH,4: OPENRUN CH,5: RCL CH,6, CTSOUT CH,7: OTSOUT CH,
8: ACLSOUT CH,9: AOLSOUT CH,10:AL CH CH = 1~10
*********************************************************/
void disp_Set1_6_xEnglish(u16 Sel)
{
	u16 sl;
	if(Fsh500mS && EnSelFlsh)														//�Ƿ�ʹ�ܷ�����ʾ��˸//�Ƿ�ʹ�ܱ�ѡ����˸
	{
		sl=0;				//
	}
	else
		sl=Sel;
	// 1
	display_char8x16(0*15+2,19,"MRTS",0x01 & sl); 
	if(Sel & 1)
	{
		dispayNum8x16(0*15+2,32,TmpSetVal,(0x01 & sl));
	}
	else
	{
		dispayNum8x16(0*15+2,32,ChSel[0],0x01 & Sel);
	}
	// 2
	display_char8x16(1*15+2,19,"MD",0x02 & sl);
	if(Sel & 2)
	{
		dispayNum8x16(1*15+2,32,TmpSetVal,(0x02 & sl));
	}
	else
	{
		dispayNum8x16(1*15+2,32,ChSel[1],0x02 & Sel);//ChSel
	}
	// 3
	display_char8x16(2*15+2,19,"SHUTRUN",0x04 & sl);
	if(Sel & 4)
	{
		dispayNum8x16(2*15+2,41,TmpSetVal,0x04 & sl);
	}
	else
	{
		dispayNum8x16(2*15+2,41,ChSel[2],0x04 & Sel);//ChSel
	}
	// 4
	display_char8x16(3*15+2,19,"OPENRUN",0x08 & sl);
	if((Sel & 8) && EnSelFlsh)
	{
		dispayNum8x16(3*15+2,41,TmpSetVal,0x08 & sl);
	}
	else
	{
		dispayNum8x16(3*15+2,41,ChSel[3],0x08 & Sel);//ChSel
	}
	// 5
	display_char8x16(4*15+2,19,"RCL",0x10 & sl);
	if((Sel & 16) && EnSelFlsh)
	{
		dispayNum8x16(4*15+2,41,TmpSetVal,0x10 & sl);
	}
	else
	{
		dispayNum8x16(4*15+2,41,ChSel[4],0x10 & Sel);//ChSel
	}
	// 6
	display_char8x16(5*15+2,19,"CTSOUT",0x20 & sl);
	if((Sel & 32) && EnSelFlsh)
	{
		dispayNum8x16(5*15+2,41,TmpSetVal,0x20 & sl);
	}
	else
	{
		dispayNum8x16(5*15+2,41,ChSel[5],0x20 & Sel);//ChSel
	}
	// 7
	display_char8x16(6*15+2,19,"OTSOUT",0x40 & sl);
	if((Sel & 64) && EnSelFlsh)
	{
		dispayNum8x16(6*15+2,41,TmpSetVal,0x40 & sl);
	}
	else
	{
		dispayNum8x16(6*15+2,41,ChSel[6],0x40 & Sel);//ChSel
	}
	// 8
	display_char8x16(7*15+2,19,"ACLSOUT",0x80 & sl);
	if((Sel & 128) && EnSelFlsh)
	{
		dispayNum8x16(7*15+2,41,TmpSetVal,0x80 & sl);
	}
	else
	{
		dispayNum8x16(7*15+2,41,ChSel[7],0x80 & Sel);//ChSel
	}
	// 9
	display_char8x16(8*15+2,19,"AOLSOUT",0x100 & sl);
	if((Sel & 256) && EnSelFlsh)
	{
		dispayNum8x16(8*15+2,41,TmpSetVal,0x100 & sl);
	}
	else
	{
		dispayNum8x16(8*15+2,41,ChSel[8],0x100 & Sel);//ChSel
	}
	// 10//�����10��
	display_char8x16(9*15+2,19,"AL ",0x200 & sl);
	if((Sel & 512) && EnSelFlsh)
	{
		dispayNum8x16(9*15+2,32,TmpSetVal,0x200 & sl);
	}
	else
	{
		dispayNum8x16(9*15+2,32,ChSel[9],0x200 & Sel);//ChSel
	}	

}

/*********************************************************
function name: �Զ��궨
fun :		
*********************************************************/
void AutoStartEndSet(void)
{
    switch(BaseSet1_STEP)
	{
		case	0:
			if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x01))
			{    
                WaitTim = T10mS + MoveMaxTim;									//�趨��תʱ��
				BaseSet1_STEP=1;
			}
			break;
		case	1:																//��ת
			Moto_REV_Drv;
			if(PIStus.ACLS || PIStus.CTS || T10mS >= WaitTim)		            //����ʼλ��ʱ?
			{
				PosLmdD=AvVal[_FK];												//ȡ����
                if(PosLmdD < 10)//��ѹ���ͣ�˵��ʹ�õ��Ǳ�����
                {
                    PosLmdD = EncoderCurr;
                }
                ZeroStatus = 1;                                                 //��ʼ�������    
				Moto_STOP_Drv;													//ȥɲ��
				WaitTim = T10mS + StopDellTim;									//�趨ɲ��ʱ��
				BaseSet1_STEP=2;
			}
			break;
		case	2:																//ɲ��
			if(T10mS>=WaitTim)													//ɲ��ʱ�䵽
			{
				//Moto_REV_Drv;
                //Moto_FWD_Drv;
				WaitTim = T10mS + MoveMaxTim;									//�趨��תʱ��
				BaseSet1_STEP=3;
			}
			break;
		case	3:													            //��ת
			Moto_FWD_Drv;
			if(PIStus.AOLS || PIStus.OTS || T10mS >= WaitTim)					//��δλ��ʱ?
			{
				PosLmdU=AvVal[_FK];												//ȡ����
                if(PosLmdU < 10)//��ѹ���ͣ�˵��ʹ�õ��Ǳ�����
                {
                    PosLmdU = EncoderCurr;
                }
                ZeroStatus = 3;
				Moto_STOP_Drv;													//ȥɲ��
				WaitTim = T10mS + StopDellTim;									//�趨ɲ��ʱ��
				BaseSet1_STEP=4;
			}
			break;
		case	4:																//ɲ��
//			if(T10mS>=WaitTim)													//ɲ��ʱ�䵽
			{
				Moto_PARK_Drv;													//�ͷŵ��
				AutoSetOKF=1;
				EnSelFlsh=0;													//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
                BaseSet1_STEP=0;												//�궨����
                Closeway = ClosewayTemp & 1;         //����ԭ�еĹط�ʽ
                ClosewayTemp = 0;
                ZeroStatus = 0;
                isZero = isZeroTemp;
                isZeroTemp = 127;
            }
			break;
		}
}

/*********************************************************
function name: ��λȷ��
fun :		
*********************************************************/
void OpenSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x02))
	{ 
        if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn)									//+
			{
				ClsKDn;
				Moto_FWD_Drv;//��ת
				
			}
			if(K3Dn)									//-
			{
				ClsKDn;
				Moto_REV_Drv;//��ת
				
			}
			WaitTim = T10mS + StopDellTim;									//�趨ɲ��ʱ��
		}
		else
		{
			Moto_STOP_Drv;
			if(T10mS>=WaitTim)
			{
//				EnSelFlsh=0;												//�궨����
				Moto_PARK_Drv;												//�ͷŵ��
			}
		}
   
		if(K2Dn)									//enter?
		{
			ClsKDn;
			PosLmdU=AvVal[_FK];
            if(PosLmdU < 10)//��ѹ���ͣ�˵��ʹ�õ��Ǳ�����
            {
                PosLmdU = EncoderCurr;
            }
            if(ZeroStatus != 1)
            {
                ZeroStatus += 1;
                if(ZeroStatus >= 3)
                {
                    ZeroStatus = 0;
                    isZero = isZeroTemp;
                    isZeroTemp = 127;
                }
            }
			EnSelFlsh=0;					//ʹ�ܷ�����ʾ��˸//ʹ�ܱ�ѡ����˸
            Closeway = ClosewayTemp & 1;         //����ԭ�еĹط�ʽ
            ClosewayTemp = 0;
        }
	}
}

/*********************************************************
function name: ��λȷ��
fun :		
*********************************************************/
void CloseSet(void)
{
    if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x04))
    {     
        if(KeyHoldStus || KeyHoldStusA)
        {
            if(K4Dn)									//+
            {
                ClsKDn;
                Moto_FWD_Drv;//��ת
                
            }
            if(K3Dn)									//-
            {
                ClsKDn;
                Moto_REV_Drv;//��ת
                
            }
            WaitTim = T10mS + StopDellTim;			//�趨ɲ��ʱ��
        }
        else
        {
            Moto_STOP_Drv;
            if(T10mS>=WaitTim)
            {
//				EnSelFlsh=0;								//�궨����
                Moto_PARK_Drv;						//�ͷŵ��
            }
        }
                
        if(K2Dn)										//enter?
        {
            ClsKDn;
            PosLmdD=AvVal[_FK];
            if(PosLmdD < 10)//��ѹ���ͣ�˵��ʹ�õ��Ǳ�����
            {
                PosLmdD = EncoderCurr;
            }
            if(ZeroStatus != 2)//���������ɣ�
            {
                ZeroStatus += 2;
                if(ZeroStatus >= 3)
                {
                    ZeroStatus = 0;
                    isZero = isZeroTemp;
                    isZeroTemp = 127;
                }
            }
            EnSelFlsh=0;								//�궨����
            Closeway = ClosewayTemp & 1;         //����ԭ�еĹط�ʽ
            ClosewayTemp = 0;
        }
    }
}

/*********************************************************
function name: �رշ���		
fun :		
*********************************************************/
void CloseDirectionSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x10))
	{
		if(K3Dn)														//+
		{
			ClsKDn;
			CloseDirectionflag=!CloseDirectionflag;
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			CloseDirectionflag=!CloseDirectionflag;
		}

		if(K2Dn)									//enter?
		{
			ClsKDn;
			if(CloseDirectionflag == 0)
			{
				SetCloseDir = 0;			//����
			}
			else
			{
				SetCloseDir = 1;			//����
			}
			EnSelFlsh=0;					//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			CloseDirectionflag = SetCloseDir;
			EnSelFlsh=0;
		}
	}	
}

/*********************************************************
function name: �رշ�ʽ
fun :		
*********************************************************/
void CloseWaySet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x08))
	{
		if(K3Dn)														//+
		{
			ClsKDn;
			Closewayset=!Closewayset;
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			Closewayset=!Closewayset;
		}

		if(K2Dn)									//enter?
		{
			ClsKDn;

            Closeway=Closewayset;

			EnSelFlsh=0;					//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			Closewayset = Closeway;
			EnSelFlsh=0;					//����
		}
	}	
}

/*********************************************************
function name: ���ƾ����趨
fun :		
*********************************************************/
void PrecisionSet(void)
{
		if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x40))
		{
			if(KeyHoldStus || KeyHoldStusA)
			{
				if(K4Dn||(Klong && (KDn==4)))									//+
				{
					ClsKDn;
					PosAccuracy_flag+=10;
					if(PosAccuracy_flag >=990) PosAccuracy_flag =990;
				}
				if(K3Dn||(Klong && (KDn==3)))									//-
				{
					ClsKDn;
					if(PosAccuracy_flag > 50)									//��߾���0.5
					{
						PosAccuracy_flag -=5;
					}
					else
					{
						PosAccuracy_flag =50;
					}
				}
			}
			else
			{
				ClsKlong;
			}	
			if(K2Dn)									//enter?
			{
				ClsKDn;
				PosAccuracy=PosAccuracy_flag;					//ȷ�ϸ��ľ���
				EnSelFlsh=0;							//����
			}
			else if(K1Dn)
			{
				ClsKDn;
				PosAccuracy_flag=PosAccuracy;
				EnSelFlsh=0;					//����
			}
		}
}

/*********************************************************
function name: �ֳ�����:ѡ��㶯�����֡��Զ�ģʽ
fun :		
*********************************************************/
void WorkModSet(void)
{
	
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x40))
	{
		if(K3Dn)									//+
		{
			ClsKDn;
			JogHolAutoSelflag+=1;
			if(JogHolAutoSelflag>=4)
			{
				JogHolAutoSelflag=1;
			}
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			JogHolAutoSelflag-=1;
			if(JogHolAutoSelflag==0)
			{
				JogHolAutoSelflag=3;
			}
		}
		if(K2Dn)									//enter?
		{
			ClsKDn;
			switch(JogHolAutoSelflag)					//����ģʽѡ��
			{
				case 1:	JogHolAutoSel=1;		//�㶯
				break;
				
				case 2:	JogHolAutoSel=2;		//����
				break;
								
				case 3:	JogHolAutoSel=3;		//�Զ�
				break;
			}
			EnSelFlsh=0;							//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			JogHolAutoSelflag=JogHolAutoSel;
			EnSelFlsh=0;					//����
		}
	}
}


/*********************************************************
function name: Զ������(����ģ������������)
fun :		
*********************************************************/
void SingleSel(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x20))
	{
		if(K3Dn)									//+
		{
			ClsKDn;
			A_DInSelflag=!A_DInSelflag;
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			A_DInSelflag=!A_DInSelflag;
		}
	
		if(K2Dn)									//enter?
		{
			ClsKDn;
			if(A_DInSelflag==1)
			{
				A_DInSel=1;		//Զ�����뿪����
			}
			else
			{
				A_DInSel=0;		//Զ������Ϊģ����
			}
			EnSelFlsh=0;					//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			A_DInSelflag=A_DInSel;
			EnSelFlsh=0;					//����
		}
	}
}

/*********************************************************
function name: ���Ŷ�������
fun :		
*********************************************************/
void NoSingleSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x80))
	{
		if(K3Dn)									//+
		{
			ClsKDn;
			NoSignalCtrSelflag+=1;
			if(NoSignalCtrSelflag>3)
				NoSignalCtrSelflag=1;
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			NoSignalCtrSelflag-=1;
			if(NoSignalCtrSelflag == 0)
				NoSignalCtrSelflag=3;
		}
		if(K2Dn)									//enter?
		{
			ClsKDn;
            NoSignalCtrSel = NoSignalCtrSelflag;
			EnSelFlsh=0;					//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			NoSignalCtrSelflag=NoSignalCtrSel;
			EnSelFlsh=0;					//����
		}
	}
}

/*********************************************************
function name: ��ʾ��������
fun :		
*********************************************************/
void LCDDirectionSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x100))
	{
		if(K3Dn)									//+
		{
			ClsKDn;
			ScrSelflag=!ScrSelflag;
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			ScrSelflag=!ScrSelflag;
		}	
		if(K2Dn)									//enter?
		{
			ClsKDn;
			if(ScrSelflag==0)
			{
				ScrSel=0;					//������ʾ
                transfer_command(0xc4);     //����LCD ��ת����
			}
			else
			{
				ScrSel=1;					//������ʾ
                transfer_command(0xc2);     //����LCD ��ת����
			}
            clear_screen();
			EnSelFlsh=0;					//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			ScrSelflag=ScrSel;
			EnSelFlsh=0;					//����
		}
	}
}

/*********************************************************
function name: ����ѡ������
fun :		
*********************************************************/
void LCDLanguage(void)
{
		if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x200))
		{
			if(K3Dn)									//+
			{
				ClsKDn;
				languageflag=!languageflag;
			}
			if(K4Dn)									//-
			{
				ClsKDn;
				languageflag=!languageflag;
			}
	
			if(K2Dn)									//enter?
			{
				ClsKDn;
				if(languageflag)
				{
//					language=1;					//�˰汾��ʱ����Ӣ�Ĳ˵�
					language=0;
				}
				else 
				{
					language=0;
				}
				EnSelFlsh=0;					//����
				clear_screen();
			}
			else if(K1Dn)
			{
				ClsKDn;
				languageflag=language;
				EnSelFlsh=0;					//����
			}
		}
}

/*********************************************************
function name: �������ò���
fun	:����ѡ��

*********************************************************/
void BaseSet(void)
{
    AutoStartEndSet();
    OpenSet();
    CloseSet();
    CloseWaySet();			//�رշ�ʽ
	CloseDirectionSet();	//�رշ���
    SingleSel();
    WorkModSet();
    NoSingleSet();
    LCDDirectionSet();
    LCDLanguage();
}

/*********************************************************
function name: ����ź�ѡ�񣬵�ѹ�����
fun :		
*********************************************************/
void OutCVSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_2)&&(DispSetSel==0x01))
	{
		if(K3Dn)														//+
		{
			ClsKDn;
			OutCVSelflag = !OutCVSelflag;
		}
		if(K4Dn)														//-
		{
			ClsKDn;
			OutCVSelflag = !OutCVSelflag;
		}
	
		if(K2Dn)														//enter?
		{
			ClsKDn;
			OutCVSel = OutCVSelflag;	
			EnSelFlsh=0;												//����
		}
		else if(K1Dn)					//�Ƿ񷵻��ϼ��˵�
		{
			ClsKDn;
			OutCVSelflag=OutCVSel;		//�ָ���ֵ	
			EnSelFlsh=0;					//����
		}
	}
}

/*********************************************************
function name: ��������
fun :		
*********************************************************/
void InSSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_2)&&(DispSetSel==0x02))
	{
		if(K2Dn)														//enter?
		{
			ClsKDn;
			EnSelFlsh=0;												//����
			if(OutCVSel)
			{
				CInLmdD = PIStus.VMA_IN;
			}
			else
			{
				VInLmdD = PIStus.VMA_IN;
			}
		}
	}
}

/*********************************************************
function name: ��������
fun :		
*********************************************************/
void InESet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_2)&&(DispSetSel==0x04))
	{
		if(K3Dn)														//+
		{
			ClsKDn;
		}
		if(K4Dn)														//-
		{
			ClsKDn;
		}
	
		if(K2Dn)														//enter?
		{
			ClsKDn;
			EnSelFlsh=0;												//����
			if(OutCVSel)
			{
				CInLmdU = PIStus.VMA_IN;
			}
			else
			{
				VInLmdU = PIStus.VMA_IN;
			}
		}
	}
	
}

/*********************************************************
function name: �������
fun :		
*********************************************************/
void OutSSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_2)&&(DispSetSel==0x08))
	{
		if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn||(Klong && (KDn==4)))									//+
			{
				ClsKDn;
				if(Klong)
				{
					TmpSetVal+=1;
					if(TmpSetVal >=4095) TmpSetVal =4095;
				}
				else
				{
					TmpSetVal+=2;
				}
				if(TmpSetVal >=4095) TmpSetVal =4095;
			}
			if(K3Dn||(Klong && (KDn==3)))									//-
			{
				ClsKDn;
				if(Klong)
				{
					if(TmpSetVal>=1)TmpSetVal-=1;
					if(TmpSetVal <= 0) TmpSetVal =0;
				}
				else
				{
                    if(TmpSetVal>=2)
					{
						TmpSetVal-=2;
					}
					else
					{
						TmpSetVal=0;
					}
				}
			}
		}
		if(K2Dn)														//enter?
		{
			ClsKDn;
			EnSelFlsh=0;												//����
			if(OutCVSel)
			{
				OutCLmdD= TmpSetVal;
			}
			else
			{
				OutVLmdD= TmpSetVal;
			}
			TmpSetVal = 0;
		}
		if(OutCVSel)
		{
			POStus.MA_OUT = TmpSetVal;
		}
		else
		{
			POStus.V_OUT = TmpSetVal;
		}
	}
}

/*********************************************************
function name: �������
fun :		
*********************************************************/
void OutESet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_2)&&(DispSetSel==0x10))
	{

		if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn||(Klong && (KDn==4)))									//+
			{
				ClsKDn;
				if(Klong)
				{
					TmpSetVal+=1;
					if(TmpSetVal >=4095) TmpSetVal =4095;
				}
				else
				{
					TmpSetVal+=2;
				}
				if(TmpSetVal >=4095) TmpSetVal =4095;
			}
			if(K3Dn||(Klong && (KDn==3)))									//-
			{
				ClsKDn;
				if(Klong)
				{
					if(TmpSetVal>=1)TmpSetVal-=1;
					if(TmpSetVal <=1) TmpSetVal =0;
				}
				else
				{
					if(TmpSetVal>=2)
					{
						TmpSetVal-=2;
					}
					else
					{
						TmpSetVal=0;
					}
				}
			}
		}

		if(K2Dn)														//enter?
		{
			ClsKDn;
			EnSelFlsh=0;												//����
			if(OutCVSel)
			{
				OutCLmdU = TmpSetVal;
			}
			else
			{
				OutVLmdU = TmpSetVal;
			}
			TmpSetVal = 0;
		}
		if(OutCVSel)
		{
			POStus.MA_OUT = TmpSetVal;
		}
		else
		{
			POStus.V_OUT = TmpSetVal;
		}
	}
	
}

/*********************************************************
�������ò���
fun	:����ѡ��

*********************************************************/
void FeedBackSet(void)
{
	OutCVSet();										//����������ѹѡ��
	InSSet();										//��������
	InESet();										//��������
	OutSSet();										//�������
	OutESet();										//�������
}


/*********************************************************
function name: ESD����
fun :		ESD����״̬������λ���յ�λ��ֹͣ
*********************************************************/
void EsdSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x10))
	{
		if(K3Dn)									//+
		{
			ClsKDn;
			ESDsetflag+=1;
			if(ESDsetflag>=4)
				ESDsetflag=1;			
				      //����״̬
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			ESDsetflag-=1;
			if(ESDsetflag <=0)
				ESDsetflag=3;
			
		}
		if(K2Dn)								//enter?
		{
			ClsKDn;
            ESDset = ESDsetflag;
			EnSelFlsh=0;						//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			ESDsetflag=ESDset;
			EnSelFlsh=0;	    				//����
		}
	}
}

/*********************************************************
function name: ͨѶ����
fun :		
*********************************************************/
void Rs485Set(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x04))
	{
		if(K3Dn)														//+
		{
			ClsKDn;
			Rs485Wayset=!Rs485Wayset;
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			Rs485Wayset=!Rs485Wayset;
		}

		if(K2Dn)									//enter?
		{
			ClsKDn;

            Rs485Way=Rs485Wayset;

			EnSelFlsh=0;					//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			Rs485Wayset = Rs485Way;
			EnSelFlsh=0;					//����
		}
	}	
}

/*********************************************************
function name: ͨѶ����
fun :		
*********************************************************/
void MbAddrSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x08))
	{
		if(KeyHoldStus || KeyHoldStusA)
		{
            if(K4Dn||(Klong && (KDn==4)))									//+
			{
				ClsKDn;
				if(Klong)
				{
					mbaddr_flag++;
				}
				else
				{
					mbaddr_flag++;
				}
				if(mbaddr_flag > 255) mbaddr_flag = 1;
			}
			if(K3Dn||(Klong && (KDn==3)))									//-
			{
				ClsKDn;
				if(Klong)
				{
					mbaddr_flag--;
				}
				else
				{
					mbaddr_flag--;
				}
				if(mbaddr_flag < 1) mbaddr_flag = 255;
			}
		}
		if(K2Dn)									//enter?
		{
			ClsKDn;
			mbaddr = mbaddr_flag;					//
			EnSelFlsh=0;							//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			mbaddr_flag = mbaddr;
			EnSelFlsh=0;					         //����
		}
	}	
}

/*********************************************************
function name: ���߿���		
fun :		
*********************************************************/
void TwoLineSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x20))
	{
		if(K3Dn)														//+
		{
			ClsKDn;
			TwoLineflag++;
			if(TwoLineflag>=4)
			{
				TwoLineflag=1;
			}
		}
		if(K4Dn)									//-
		{
			ClsKDn;
			TwoLineflag--;
			if(TwoLineflag<=0)
			{
				TwoLineflag=3;
			}
		}
		if(K2Dn)									//enter?
		{
			ClsKDn;
            TwoLine = TwoLineflag;
			EnSelFlsh=0;							//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			TwoLineflag=TwoLine;
			EnSelFlsh=0;					//����
		}
	}	
}

/*********************************************************
function name: ��תʱ��		
fun :		
*********************************************************/
void LockedRotorTimeSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x02))
	{
		if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn)									//+
			{
				ClsKDn;
				LockedRotorTime_flag+=1;
				if(LockedRotorTime_flag > 10)
					LockedRotorTime_flag = 0;
			}
			if(K3Dn)									//-
			{
				ClsKDn;
				LockedRotorTime_flag -=1;
				if(LockedRotorTime_flag > 20)//����10������ֵ
				{
					LockedRotorTime_flag = 10;
				}
			}
		}
		if(K2Dn)									//enter?
		{
			ClsKDn;
			LockedRotorTime=LockedRotorTime_flag;					//ȷ�ϸ��Ķ�תʱ��
			EnSelFlsh=0;							//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			LockedRotorTime_flag=LockedRotorTime;
			EnSelFlsh=0;					//����
		}
	}
}

/*********************************************************
function name: ����ɲ��
fun :		������
*********************************************************/
void ElectronicBrakeSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x01))
	{
		if(K3Dn)										//+
		{
			ClsKDn;
			electronicbrakeflag=!electronicbrakeflag;
		}
		if(K4Dn)										//-
		{
			ClsKDn;
			electronicbrakeflag=!electronicbrakeflag;
		}

		if(K2Dn)										//enter?
		{
			ClsKDn;
			if(electronicbrakeflag)
			{
				electronicbrake=1;
			}
			else
			{
				electronicbrake=0;
			}
			EnSelFlsh=0;								//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			electronicbrakeflag=electronicbrake;
			EnSelFlsh=0;					//����
		}
	}		
}

/*********************************************************
function name: �������		
fun :		
*********************************************************/
void IntermittentRunSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x40))
	{
		if(K3Dn)										//+
		{
			ClsKDn;
			IntermittentRunflag=!IntermittentRunflag;
		}
		if(K4Dn)										//-
		{
			ClsKDn;
			IntermittentRunflag=!IntermittentRunflag;
		}

		if(K2Dn)										//enter?
		{
			ClsKDn;
			if(IntermittentRunflag)
			{
				IntermittentRun=1;					//��
			}
			else
			{
				IntermittentRun=0;					//��
			}
			EnSelFlsh=0;								//����
		}
		else if(K1Dn)
		{
			ClsKDn;
			IntermittentRunflag=IntermittentRun;
			EnSelFlsh=0;					//����
		}
	}	
}

/*********************************************************
function name: ������к���		
fun :		
*********************************************************/
void IntermittentRundis(u8 Htarget ,u8 Ltarget)
{
	static u8 times=0;
//	static u16  target=0;
	if(Fsh500mS)
	{
		times++;			
	}
    //2016��9��13��13:38:50 zhaoxw target ��û��ʹ��
//	if(times==18)					//500mSx18 =4S
//	{
//		target=GetMotoPos();		//��ȡһ�η�λֵ
//	}

	if(((times>=10)&&(times<=18))||(GetMotoPos()== Htarget)||(GetMotoPos()== Ltarget))
	{
		if(times==18)
		{
			times=0;			
		}
		if(electronicbrake)				   //��������˵���ɱ������ break;
		{
			Moto_STOP_Drv;
		}
		else								//û�п�������ɲ������
		{
			Moto_PARK_Drv;
		}
	}
//	else if((GetMotoPos() <  target+680)&&(GetMotoPos() <Htarget))				//С��Ŀ��ֵ����ת
	else if( (((PIStus.VMA_IN_Pers )/10)*10  + PosAccuracy)&&(GetMotoPos() <Htarget))			//С��Ŀ��ֵ����ת
	{
		Moto_FWD_Drv;
		/*if(1 == CloseDirection)
		{
			StusView=_Kaifa;
		}
		else
		{
			StusView=_BiFa;
		}*/
	}
	else if( (((PIStus.VMA_IN_Pers)/10)*10  + PosAccuracy)&&(GetMotoPos() >Ltarget))			//����Ŀ��ֵ��ת
	{
		Moto_REV_Drv;
		/*if(1 == CloseDirection)
		{
			StusView=_BiFa;
		}
		else
		{
			StusView=_Kaifa;
		}*/
	}	
}
/*********************************************************
function name: �м�λ��1
fun :		
*********************************************************/
void Intermediate1Set(void)
{
    if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x80))
	{
        if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn||(Klong && (KDn==4)))									//+
			{
				ClsKDn;
				if(Klong)
				{
					Intermediate1flag+=2;
				}
				else
				{
					Intermediate1flag+=1;
				}
				if(Intermediate1flag > 45) Intermediate1flag = 1;
			}
			if(K3Dn||(Klong && (KDn==3)))									//-
			{
				ClsKDn;
				if(Klong)
				{
					Intermediate1flag-=2;
				}
				else
				{
					Intermediate1flag-=1;
				}
				if(Intermediate1flag == 0) Intermediate1flag = 45;
			}
		}
		if(K2Dn)														//enter?
		{
			ClsKDn;
            Intermediate1 = Intermediate1flag;
			EnSelFlsh=0;
		}
		else if(K1Dn)
		{
			ClsKDn;
            Intermediate1flag = Intermediate1;
			EnSelFlsh=0;					//����
		}
	}	
}

/*********************************************************
function name: �м�λ��2
fun :		
*********************************************************/
void Intermediate2Set(void)
{  
    if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x100))
	{
        if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn||(Klong && (KDn==4)))									//+
			{
				ClsKDn;
				if(Klong)
				{
					Intermediate2flag+=2;
				}
				else
				{
					Intermediate2flag+=1;
				}
				if(Intermediate2flag > 99) Intermediate2flag = 55;
			}
			if(K3Dn||(Klong && (KDn==3)))									//-
			{
				ClsKDn;
				if(Klong)
				{
					Intermediate2flag-=2;
				}
				else
				{
					Intermediate2flag-=1;
				}
				if(Intermediate2flag < 55) Intermediate2flag = 99;
			}
		}
		if(K2Dn)														//enter?
		{
			ClsKDn;
            Intermediate2 = Intermediate2flag;
			EnSelFlsh=0;
		}
		else if(K1Dn)
		{
			ClsKDn;
            Intermediate2flag = Intermediate2;
			EnSelFlsh=0;					//����
		}
	}	
}

/*********************************************************
function name: ��������
fun :		
*********************************************************/
void PasswordSet(void)		//�����趨
{
	static u8 i=0;
	u8 a,b,c,d;
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x200))
	{
		display_char32x32(32,1*10-3,Geng32x32,0);
		display_char32x32(32,2*10-3,Xin32x32,0);
		display_char32x32(32,3*10-3,Mi32x32,0);
		display_char32x32(32,4*10-3,Ma32x32,0);
//		hextobcd(TmpSetVal);
		switch(i)					
		{
			case 0:	a=0;b=0,c=0,d=1;				
			break;

			case 1:	a=0;b=0,c=1,d=0;				
			break;

			case 2:	a=0;b=1,c=0,d=0;				
			break;

			case 3:	a=1;b=0,c=0,d=0;				
			break;
		}
		dispayNum32x32(64,1*10-3,BCD_buffer[3],a);
		dispayNum32x32(64,2*10-3,BCD_buffer[2],b);
		dispayNum32x32(64,3*10-3,BCD_buffer[1],c);
		dispayNum32x32(64,4*10-3,BCD_buffer[0],d);
		if(KeyHoldStus || KeyHoldStusA)
		{
			if(K4Dn)							//+
			{
				ClsKDn;
				BCD_buffer[i]++;
				if(BCD_buffer[i]>9)
				{
					BCD_buffer[i]=0;
				}
				
			}
			if(K3Dn)							//-
			{
				ClsKDn;
				BCD_buffer[i]--;
				if(BCD_buffer[i]==255)
				{
					BCD_buffer[i]=9;
				}
			}
		}	
		if(K2Dn||(Klong && (KDn==2)))									//enter?
		{
			ClsKDn;
			if(Klong)
			{
				EnSelFlsh=0;											//����
				PassWord =BCDtoDec(BCD_buffer);						//��������OK
				clear_screen();		
				TmpSetVal = 0;
			}
			else
			{	
				i++;
				if(i>=4)
				{
					i=0;
				}
			}
		}
	}
	
}

/*********************************************************
function name: �ָ�����
fun :		
*********************************************************/
void DefaultSet(void)		//�ָ�����
{
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&(DispSetSel==0x400))
		{
		display_char32x32(64,10,Que32x32,1);
		display_char32x32(64,20,Ding32x32,1);
//		display_char32x32(64,25,Sp32x32,1);
		display_char32x32(64,30,WenHao32x32,1);
		if(K2Dn)
		{
			ClsKDn;
			//���������趨��
            //AutoSetOKF=0;				//δ�궨��
            isZero = 127;
            PosLmdU=3992;				//��λ����		����4095��ʵ�ʻ�С��
            PosLmdD=38;				    //��λ����
            CInLmdU=3707;			    //����C����
            CInLmdD=746;			    //����C����
            VInLmdU=4095;				//����V����
            VInLmdD=0;				    //����V����
            OutCLmdU=4095;				//�������
            OutCLmdD=0; 				//�������
            OutVLmdU=4095;				//�������
            OutVLmdD=0; 				//�������
            PosAccuracy=100;			//��λ����(Ĭ��1.00)
            JogHolAutoSel=1;			//�ֳ����Ʒ�ʽ:	1�㶯,2����,3�Զ�
            A_DInSel=1;					//Զ�����뷽ʽ:	0ģ����,1������
            NoSignalCtrSel=1;			//���Ŵ���ʽ	1ֹͣ��2ȫ����3ȫ��
            RmLcDisSel =1;				//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
            ScrSel =0;					//��ʾ����
            transfer_command(0xc4);     //����LCD ��ת���� �ָ�����Ĭ�ϲ���ʱ����ʾ����
            OutCVSel=1;					//����������ѹѡ��
            PassWord = 0; 			    //����(Ĭ��Ϊ"0000"),�޶�С�ڵ���9999
            //ChSel[i]                  //ͨ������
            RelayType[0] = 0;           //����̵�����Ҫ��̬���������
            RelayType[1] = 1;
            RelayType[2] = 2;
            RelayType[3] = 3;
            RelayType[4] = 5;
            RelayType[5] = 6;
            RelayType[6] = 9;
            RelayType[7] = 10;
            RelayType[8] = 7;
            //A_DInSel=0;				//Զ�����뷽ʽ:	0ģ����,1������
            //NoSignalCtrSel=1;			//���Ŵ���ʽ	1ֹͣ��2ȫ����3ȫ��
            language=0;					//����ѡ��		0���� ��0Ӣ��	

            //�߼������趨��
            electronicbrake=0;			//����ɲ��������־    0����  ��0 ����
            //CloseDirection = 1;			//�رշ���	1�� 0��
            SetCloseDir = 1;
            Closeway=0;					//Closeway, ��0 ����   0 ��λ  �жϵ����λ�Ϳ�λ������ ���ؿ��ػ���λ����
            Rs485Way=0;                 //1�����߿�����Ч��д��Ч 0�����߿�����Ч��д��Ч
            ESDset=1;					//ESD���ã�2��λ��3��λ��1ֹͣ
            mbaddr = 1;
            TwoLine=1;					//���߿���	  1��ֹ  2���źſ�  3���źŹ�	  
            LockedRotorTime=4;			//��תʱ��		0- 10S
            IntermittentRun=0;			//�������	(��0��������У�0��)
            Intermediate1=25;			//�м�λ��1 0%~�м�λ��1
            Intermediate2=75;
            lock=0;                     //Ĭ�ϲ��������
			EnSelFlsh =0;
			clear_screen();
			STMFLASH_Write(FLASH_SAVE_ADDR + 98,0,1);	//���´洢
            Display_Init();             //ˢ�²˵�
		}
		if(K1Dn || K3Dn || K4Dn)
		{
			ClsKDn;
			EnSelFlsh =0;
			clear_screen();
		}
	}
}

/*********************************************************
�߼����ò���
fun :����ѡ��

*********************************************************/
void AdvancedSet(void)
{
    ElectronicBrakeSet();
    LockedRotorTimeSet();
    
#if (MB_MODE < 2)    
    Rs485Set();
#else    
    Rs485Wayset = 0;
#endif
    
    MbAddrSet();
	EsdSet();				//ESD����  
	TwoLineSet();
    
	PrecisionSet();
	//IntermittentRunSet();
    Intermediate1Set();
    Intermediate2Set();
	PasswordSet();			//��������
	DefaultSet();				//�ָ���������
}

/*********************************************************
ȡ�÷���λ��
���:
	0 (_LessLdD):����Ч����֮��
	1(_LessTarget):��Ŀ����λ
	2(_InTarget):��Ŀ��֮��
	3(_MoreTarget):��Ŀ����λ
	4(_MoreLdU):����Ч����֮��
*********************************************************/
u8 GetMotoPos(void)
{
	u8 PosTemp = _InTarget;
    u16 temp;
    //if(CloseDirection)
	{
		//if(PIStus.CTS || PIStus.ACLS || (PIStus.FK_IN_Pers & 0x8000))
        if(PIStus.FK_IN_Pers & 0x8000)
		{
			PosTemp = _LessLdD;
		}
		//else if(PIStus.OTS|| PIStus.AOLS || ((PIStus.FK_IN_Pers & 0x7fff) > 10000))
        else if(PIStus.FK_IN_Pers > 10000)
		{
			PosTemp = _MoreLdU;
		}
		//	else if((PIStus.FK_IN_Pers <= (5000  + PosAccuracy)) && (PIStus.FK_IN_Pers	>= (5000 - PosAccuracy)))
		//		{
		//		return _InMid;
		//		}
		if((PIStus.FK_IN_Pers & 0x8000))
        {       
            if(PIStus.VMA_IN_Pers < PosAccuracy)
            {
                temp = PosAccuracy - PIStus.VMA_IN_Pers;
            }
            
            if((PIStus.FK_IN_Pers >= (0x8000 + temp)))
            {
                PosTemp = _LessTarget;
            }
            else if((PIStus.FK_IN_Pers & 0x7fff) > (PIStus.VMA_IN_Pers + PosAccuracy))
            {
                PosTemp = _MoreTarget;
            }
            else
            {
                PosTemp = _InTarget;
            }
        }
        else
        {
            if(PIStus.VMA_IN_Pers > PosAccuracy)
            {
                temp = PIStus.VMA_IN_Pers - PosAccuracy;
            }                
              
            if((PIStus.FK_IN_Pers <= (PIStus.VMA_IN_Pers  + PosAccuracy)) && (PIStus.FK_IN_Pers >= temp))
            {
                PosTemp = _InTarget;
            }
            else if((PIStus.FK_IN_Pers > (PIStus.VMA_IN_Pers + PosAccuracy)))
            {
                PosTemp = _MoreTarget;
            }
            else 
            {
                if(PIStus.VMA_IN_Pers < PosAccuracy)
                {
                    PosTemp = _InTarget;
                }
                else
                {
                    if((PIStus.FK_IN_Pers < (PIStus.VMA_IN_Pers - PosAccuracy)))
                    {
                        PosTemp = _LessTarget;
                    }
                }
            }
        }
	}
    return PosTemp;
//	else
//	{
//		if(PIStus.CTS || PIStus.ACLS || (PIStus.FK_IN_Pers & 0x8000))
//		{
//			return _MoreLdU;
//		}
//		else if(PIStus.OTS|| PIStus.AOLS || ((PIStus.FK_IN_Pers & 0x7fff) > 10000))
//		{
//			return _LessLdD;
//		}
//		else if((PIStus.FK_IN_Pers <= (PIStus.VMA_IN_Pers   + PosAccuracy)) && ((PIStus.FK_IN_Pers&0x7fff)	>= (PIStus.VMA_IN_Pers  - PosAccuracy)))
//		{
//			return _InTarget;
//		}
//		else if((PIStus.FK_IN_Pers&0x7fff) > (PIStus.VMA_IN_Pers   + PosAccuracy))
//		{
//			return _LessTarget;
//		}
//		else if((PIStus.FK_IN_Pers&0x7fff) < (PIStus.VMA_IN_Pers - PosAccuracy))
//		{
//			return _MoreTarget;
//		}
//	}
    //return 0;//zhaoxw
}


/*********************************************************
function name:���԰���
fun :	����Զ�̱���ѡ�񰴼��Ƿ�����������˲˵���ʾ�����н�����ʾһ��
*********************************************************/
void Testkey(void)
{
	//�˹����� void disp_Set1_4_x(u16 Sel)	�����������
}

/*********************************************************
function name:���ؿ��ز���	������  ������   ��λ
fun :
*********************************************************/
void TorqueTest(void)
{
	//�˹����� void disp_Set1_4_x(u16 Sel)	�����������
}

/*********************************************************
function name:����Զ�̴�
fun :
*********************************************************/
void Testopen(void)
{
//�˹����� void disp_Set1_4_x(u16 Sel)	�����������
}

/*********************************************************
function name:����Զ�̹ر�
fun :
*********************************************************/
void Testclose(void)
{
//�˹����� void disp_Set1_4_x(u16 Sel)	�����������
}

/*********************************************************
function name:����Զ�̱���
fun :
*********************************************************/
void Testkeep(void)
{
//�˹����� void disp_Set1_4_x(u16 Sel)	�����������
}

/*********************************************************
function name:����Զ���Զ�
fun :
*********************************************************/
void Testautomatic(void)
{
//�˹����� void disp_Set1_4_x(u16 Sel)	�����������	
}

/*********************************************************
function name:�ָ�����
fun :
*********************************************************/
void Recoverypassword(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_4)&&(DispSetSel==0x40))
	{
		display_char32x32(64,10,Que32x32,1);
		display_char32x32(64,21,Ding32x32,1);
//		display_char32x32(64,32,Sp32x32,1);
		display_char32x32(64,32,WenHao32x32,1);
		if(KeyHoldStus || KeyHoldStusA)
		{
			if(K2Dn)
			{
				ClsKDn;
				PassWord = 0000; 			//����(Ĭ��Ϊ"0000"),�޶�С�ڵ���9999		
				EnSelFlsh =0;
				clear_screen();
			}
		}
	}
}

/*********************************************************
function name:����汾
fun :			�鿴����汾��
*********************************************************/
void SftwareVersion(void)
{
	#define	_coloffset_1_B	15
	if((EnSelFlsh)&&(DispStep==DispSet1_4)&&(DispSetSel==0x80))
	{
		display_char14x14(jump+line*3,0*5+_coloffset_1_B,Ruan_14x14,0);
		display_char14x14(jump+line*3,1*5+_coloffset_1_B,Jian1_14x14,0);
		display_char14x14(jump+line*3,2*5+_coloffset_1_B,Ban_14x14,0);
		display_char14x14(jump+line*3,3*5+_coloffset_1_B,Ben_14x14,0);

        display_char8x16(jump+line*4,15,"V:0.2.40b",0); 				//�汾��
        display_char8x16(jump+line*5,15,"2017/05/09",0);			//����

		if(KeyHoldStus || KeyHoldStusA)
		{
			if(K2Dn)
			{
				ClsKDn;	
				EnSelFlsh =0;
				clear_screen();
			}
		}
	}
}

/*********************************************************
function name:���������Ŀ����
fun :����ѡ��

*********************************************************/
void FaultDiagnosisSet(void)
{
	Testkey();    				  //����Զ�̰���
	TorqueTest();		 	//���ؼ��
	Testopen();				 //����Զ�̴�
	Testclose();				 //����Զ�̹ر�
	Testkeep();				 //����Զ�̱���
	Testautomatic(); 			 //����Զ���Զ�
	Recoverypassword();		//����ָ�
	SftwareVersion();			//����汾
}

u8 Trg(u8 trg)
{ 
    static u8 Cont;
    unsigned char ReadData;
    ReadData = (trg&0xff)^0xff;
    trg = ReadData & (ReadData ^ Cont);
    Cont = ReadData;
    return trg;
}

void ClearMenu(void)
{ 
    if(Trg(RolStus) && (DispStep > DispWorkStus))
    {
        //clear_screen();
        ClearLine_14x14(jump+line*0);
        ClearLine_14x14(jump+line*1);
        ClearLine_14x14(jump+line*2);
        ClearLine_14x14(jump+line*3);
        ClearLine_14x14(jump+line*4);
        ClearLine_14x14(jump+line*5);
        ClearLine_14x14(jump+line*6); 
        ClearLine_14x14(jump+line*7); 
        ClearLine_14x14(jump+line*8);
        ClearLine_14x14(jump+line*9);
    }
}
