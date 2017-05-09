#include "Def.h"
#include "BIO.h"

u16 ChanlCnt;					//���ͨ���ֻ��趨
u16 ChSel[] = {9,8,7,6,5,4,3,2,1,0}; //10������ڵ����ͨ���š���Ĭ��ֵ : MRTS ChS = 9;MD ChS = 8;SHUTRUN ChS = 7;OPENRUN ChS = 6;
							    //RCL ChS = 5;CTSOUT ChS = 4;OTSOUT ChS = 3;ACLSOUT ChS = 2;AOLSOUT ChS = 1;AL ChS = 0;

u16 MRTSChS = 9;
u16 MDChS = 8;
u16 SHUTRUNChS = 7;
u16 OPENRUNChS = 6;
u16 RCLChS = 5;
u16 CTSOUTChS = 4;
u16 OTSOUTChS = 3;
u16 ACLSOUTChS = 2;
u16 AOLSOUTChS = 1;
u16 ALChS = 0;
//u8  RelayRunFlag = 1;
u16 InputTimer;
u16 WaitMoto = 0;
u32 EncoderCurr;//��������ǰֵ
u16 PhaseDir = 0;

void Sample(void);
void Output(void);
void RelayRun(void);

void ReadAngle(void);
u16 GetMedianNum(u16 *bArray, u8 iFilterLen);
u16 EncoderToPer(void);
/***********************************
#define P_ BM1 		PAin(4)		//����ڣ�������
#define P_ BM2 		PAin(5)		//����ڣ�������
#define P_ BM3 		PAin(7)		//����ڣ�������
#define P_ OTL 		PEin(7)		//����ڣ����ȼ��
#define P_ CTS 		PBin(0)		//����ڣ������ؼ��
#define P_ OTS 		PCin(4)		//����ڣ������ؼ��
#define P_ ACLS		PCin(3)		//����ڣ��ظ�����λ	
#define P_ AOLS		PCin(1)		//����ڣ���������λ	
#define P_ IN_RCL	PCin(0)		//����ڣ�Զ������	
#define P_ IN_ESD	PCin(13)	//����ڣ�Զ��ESD	
#define P_ IN_BC	PEin(5)		//����ڣ�Զ�̱���
#define P_ IN_SHUT 	PEin(4)		//����ڣ�Զ�̹�
#define P_ IN_OPEN 	PEin(3)		//����ڣ�Զ�̿�
//���������ͨ������:
#define Po10		PBout(15)	//�����10�� ��������/�������
#define Po9		    PBout(14)	//�����9�� �м�λ�����
#define Po8 		PBout(13)	//�����8�� ���������
#define Po7 		PBout(12)	//�����7�� ���������	
#define Po6 		PBout(11)	//�����6�� Զ�̱������ 
#define Po5		    PBout(10)	//�����5�� ���������		
#define Po4		    PBout(1)	//�����4�� ���������
#define Po3 		PCout(5)	//�����3�� �ظ�����λ���	
#define Po2 		PAout(6)	//�����2�� ��������λ���			
#define Po1		    PCout(2)	//�����1�� ���ϱ������ 	
#define P_M_SW	    PCout(6)	//����ڣ� ���������� 
#define P_M_OS	    PCout(7)	//����ڣ� �������ת���	
#define P_C_IO	    PEout(6)	//����ڣ� ��ѹ/�������ģʽת��

*/
/*********************************************************
function name: IO�ڳ�ʼ��	

fun	:		����IO���������
*********************************************************/
void IO_Init(void)
{
 
    GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //ʹ��PA,B,C,D,E�˿�ʱ��

	//PA4,5,7 SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				 	//�����趨������ʼ��GPIOx
	//GPIO_SetBits(GPIOA,GPIO_Pin_4);						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				 	//�����趨������ʼ��GPIO
	//GPIO_SetBits(GPIOA,GPIO_Pin_5);						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				//�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	//GPIO_SetBits(GPIOA,GPIO_Pin_7); 						//�����
    
	//PB0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	//GPIO_SetBits(GPIOB,GPIO_Pin_0); 						//�����
	//PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_8); 						//�����
	//PC0,1,3,4,13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_0); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_1); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_0); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 	//
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_4); 					 	//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 	//
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//�����趨������ʼ��GPIO
	//GPIO_SetBits(GPIOC,GPIO_Pin_13); 					 	//�����
	//PE1,2,3,4,5,7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_1); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_2); 
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_3); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_4); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_5); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 	//OTL
	GPIO_Init(GPIOE, &GPIO_InitStructure);				 	//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_7); 					 	//�����
	//OUTPUT
	//PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOA,GPIO_Pin_6); 						//�����
	//PB1,10,11,12,13,14,15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_1); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_10); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_11); 						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 	//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_12); 					 	//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 	//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_13); 					 	//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure); 		   			//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_14);						//�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 		   		//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		    //
	GPIO_Init(GPIOB, &GPIO_InitStructure); 			  		//�����趨������ʼ��GPIO
	//GPIO_ResetBits(GPIOB,GPIO_Pin_15);						
	//PC2,5,6,7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;	//�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_2); 						//�����
    GPIO_SetBits(GPIOC,GPIO_Pin_5); 						//�����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//�����趨������ʼ��GPIO
	//GPIO_SetBits(GPIOC,GPIO_Pin_7); 					 	//�����
	//PE6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_6); 						//�����

	//���ͨ��ѡ��(Ĭ��)
	MRTSChS = 9;
	MDChS = 8;
	SHUTRUNChS = 7;
	OPENRUNChS = 6;
	RCLChS = 5;
	CTSOUTChS = 4;
	OTSOUTChS = 3;
	ACLSOUTChS = 2;
	AOLSOUTChS = 1;
	ALChS = 0;
	POStus.MD=0;
	POStus.SHUTRUN=0;
	POStus.OPENRUN=0;
	POStus.RCL=0;
	POStus.CTSOUT=0;
	POStus.OTSOUT=0;
	POStus.ACLSOUT=0;
	POStus.AOLSOUT=0;
	POStus.AL=0;
	POStus.M_SW=0;
	POStus.M_OS=0;
	POStus.CV_IO=0;										//����/��ѹ�������
}
 
/*********************************************************
function name: POStus�������

fun	:		����ź���
*********************************************************/
void Output(void)
{
//    POStus.CTSOUT = PIStus.CTS;                 //�����������������
//    POStus.OTSOUT = PIStus.OTS;                 //�����������������
//    POStus.ACLSOUT = PIStus.ACLS;               //���������ո�����λ���
//    POStus.AOLSOUT = PIStus.AOLS;               //����������������λ���
//    POStus.RCL = PIStus.IN_RCL;                 //��������Զ�̱������ RmLcDisSel
//    POStus.SHUTRUN = PIStus.IN_SHUT;            //�����������������
//    POStus.OPENRUN = PIStus.IN_OPEN;            //�����������������
    
    if(Closeway)                                //ͨ�����ؿ����ж��Ƿ�λ
    {
        if(1 == CloseDirection)                      //����
		{
            if((PIStus.CTS || (POStus.AL & SHUTBLOCK)) && Moto_REV_Chk)        //��ת������? 
            {
                POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)                 //�Ƿ���ɲ������
                {
                    Moto_STOP_Drv;                  //���ɲ����ֹͣ
                }
                else
                {
                    Moto_PARK_Drv;                  //���ֹͣ
                }
            }

            if((PIStus.OTS || (POStus.AL & OPENBLOCK)) && Moto_FWD_Chk)         //��ת������?
            {
                POStus.AL &= ~SHUTBLOCK;
                if(electronicbrake)                 //�Ƿ���ɲ������
                {
                    Moto_STOP_Drv;                  //���ɲ����ֹͣ
                }
                else
                {
                    Moto_PARK_Drv;                  //���ֹͣ
                }
            }
        }
        else
        {
            if((PIStus.CTS || (POStus.AL & SHUTBLOCK)) && Moto_FWD_Chk)        //��ת������? 
            {
                POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)                 //�Ƿ���ɲ������
                {
                    Moto_STOP_Drv;                  //���ɲ����ֹͣ
                }
                else
                {
                    Moto_PARK_Drv;                  //���ֹͣ
                }
            }

            if((PIStus.OTS || (POStus.AL & OPENBLOCK)) && Moto_REV_Chk)         //��ת������?
            {
                POStus.AL &= ~SHUTBLOCK;
                if(electronicbrake)                 //�Ƿ���ɲ������
                {
                    Moto_STOP_Drv;                  //���ɲ����ֹͣ
                }
                else
                {
                    Moto_PARK_Drv;                  //���ֹͣ
                }
            }
        }
            
        if(PIStus.CTS)
        {           
            RelayByte.Byte.RelayShut = 1;
        }
        else
        {
            RelayByte.Byte.RelayShut = 0;
        }

        if(PIStus.OTS)
        {
            RelayByte.Byte.RelayOpen = 1;
        }
        else
        {
            RelayByte.Byte.RelayOpen = 0;
        }
	}
	else										//ͨ����λ���ػ�λ�ж��Ƿ�λ
	{
        if(1 == CloseDirection)                      //����
		{
            if((PIStus.CTS || PIStus.ACLS || ((PIStus.FK_IN_Pers & 0x7fff) <= PosAccuracy) \
                || (POStus.AL & SHUTBLOCK) || (PIStus.FK_IN_Pers & 0x8000)) && Moto_REV_Chk)			//�ص�����?
			{				
				POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)				//�Ƿ���ɲ������
				{
					Moto_STOP_Drv;				//���ɲ����ֹͣ
				}
				else
				{
					Moto_PARK_Drv;				//���ֹͣ
				}
			}
			else if((PIStus.OTS || PIStus.AOLS \
                || (POStus.AL & OPENBLOCK) || ((PIStus.FK_IN_Pers & 0x7fff) >= (10000 - PosAccuracy))) && Moto_FWD_Chk)		//��ת������?
			{
                POStus.AL &= ~SHUTBLOCK;
				if(electronicbrake)				//�Ƿ���ɲ������
				{
					Moto_STOP_Drv;				//���ɲ����ֹͣ
				}
				else
				{
					Moto_PARK_Drv;				//���ֹͣ
				}
			}
		}
		else
		{
			if((PIStus.OTS || PIStus.AOLS || ((PIStus.FK_IN_Pers & 0x7fff) <= PosAccuracy) \
                || (POStus.AL & SHUTBLOCK) || (PIStus.FK_IN_Pers & 0x8000)) && Moto_FWD_Chk) 			//�ص�����?
			{
				POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)				//�Ƿ���ɲ������
				{
					Moto_STOP_Drv;				//���ɲ����ֹͣ
				}
				else
				{
					Moto_PARK_Drv;				//���ֹͣ
				}
			}
			else if((PIStus.CTS || PIStus.ACLS \
                 || (POStus.AL & OPENBLOCK) || ((PIStus.FK_IN_Pers & 0x7fff)>= (10000 - PosAccuracy))) && Moto_REV_Chk)		//��ת������?
			{
				POStus.AL &= ~SHUTBLOCK;
                if(electronicbrake)				//�Ƿ���ɲ������
				{
					Moto_STOP_Drv;				//���ɲ����ֹͣ
				}
				else
				{
					Moto_PARK_Drv;				//���ֹͣ
				}
			}
		}
	}
    
    if(POStus.AL & (INTTHIGH | EXTTHIGH | ERRDIR | PHASELOST))    //����ֹͣ
    {
        if(electronicbrake)				//�Ƿ���ɲ������
        {
            Moto_STOP_Drv;				//���ɲ����ֹͣ
        }
        else
        {
            Moto_PARK_Drv;				//���ֹͣ
        }
    }
        
	if(1 == CloseDirection)                              //����
    {
        if(Moto_FWD_Chk)		                    //��ת�ޱ���״̬
        {
            RelayByte.Byte.RelayNowOpen = 1;        //�ᱻԶ�̿����ź�����
        }
        else
        {
            RelayByte.Byte.RelayNowOpen = 0;
        }
        if(Moto_REV_Chk)			                //��ת�ޱ���״̬
        {
            RelayByte.Byte.RelayNowShut = 1;
        }
        else
        {
            RelayByte.Byte.RelayNowShut = 0;
        }
        
        if(!Closeway)
        {
            if(PIStus.AOLS || (PIStus.FK_IN_Pers & 0x7fff) > (10000 - PosAccuracy))
            {//��λ�źŷ���
                RelayByte.Byte.RelayShut = 0;
                RelayByte.Byte.RelayOpen = 1;
            }
            else if(PIStus.ACLS || (PIStus.FK_IN_Pers & 0x7fff) < PosAccuracy || (PIStus.FK_IN_Pers & 0x8000))
            {
                RelayByte.Byte.RelayShut = 1;
                RelayByte.Byte.RelayOpen = 0;
            }
            else
            {
                RelayByte.Byte.RelayShut = 0;
                RelayByte.Byte.RelayOpen = 0;
            }
        }
    }
    else
    {
        if(Moto_FWD_Chk)		                    //��ת�ޱ���״̬
        {
            RelayByte.Byte.RelayNowShut = 1;
        }
        else
        {
            RelayByte.Byte.RelayNowShut = 0;
        }
        if(Moto_REV_Chk)			                //��ת�ޱ���״̬
        {
            RelayByte.Byte.RelayNowOpen = 1;
        }
        else
        {
            RelayByte.Byte.RelayNowOpen = 0;
        }
        if(!Closeway)
        {
            if(PIStus.ACLS || (PIStus.FK_IN_Pers & 0x7fff) > (10000 - PosAccuracy))
            {
                RelayByte.Byte.RelayShut = 0;
                RelayByte.Byte.RelayOpen = 1;
            }
            else if(PIStus.AOLS || (PIStus.FK_IN_Pers & 0x7fff) < PosAccuracy || (PIStus.FK_IN_Pers & 0x8000))
            {
                RelayByte.Byte.RelayShut = 1;
                RelayByte.Byte.RelayOpen = 0;
            }
            else
            {
                RelayByte.Byte.RelayShut = 0;
                RelayByte.Byte.RelayOpen = 0;
            }
        }
    }
    
    //�м�λ��
    if(PIStus.FK_IN_Pers & 0x8000)//����
    {
        RelayByte.Byte.RelayMid1 = 1;
    }
    else if(PIStus.FK_IN_Pers <  Intermediate1 * 100)			    //�м�λ��1 0%~�м�λ��1
    {
        RelayByte.Byte.RelayMid1 = 1;
    }
    else
    {
        RelayByte.Byte.RelayMid1 = 0;
    }
    
    if((PIStus.FK_IN_Pers & 0x7fff) >  Intermediate2 * 100)			//�м�λ��2 �м�λ��2~100%
    {
        RelayByte.Byte.RelayMid2= 1;
    }
    else
    {
        RelayByte.Byte.RelayMid2 = 0;
    }
        
    RelayByte.Byte.RelayLocal = RmLcDisSel;             //0Զ�� �������Զ�� 1 �͵� 2 ��ֹ
    
    if(POStus.AL)
    {
        RelayByte.Byte.RelayFault = 1;
    }
    else
    {
        RelayByte.Byte.RelayFault = 0;
    }
    
	if(OutCVSel)								        //����������ѹѡ��
	{
		POStus.CV_IO = 0;				
	}
	else
	{
		POStus.CV_IO = 1;		
	}			


	//PWM���
	if(OutCVSel)
	{
		PWMDuty = 4095 - POStus.MA_OUT;
	}
	else
	{
		PWMDuty = 4095 - POStus.V_OUT;
	}
  	TIM_SetCompare3(TIM3, PWMDuty);							//���PWM
	
	//LED��ʾ
    //zhaoxw
	//if((PIStus.VMA_IN <= (CInLmdD/2))||(POStus.AL & 0xff== _NeiChaoWen)||(POStus.AL & 0xff== _WaiChaoWen)\
	//	||(POStus.AL & 0xff == _BiDu)||(POStus.AL & 0xff== _KaiDu))		//���źţ��ڳ��£��ⳬ�£��ն£�����
	
    /*if((PIStus.VMA_IN <= (CInLmdD/2))||((POStus.AL & 0xff) == _NeiChaoWen)||((POStus.AL & 0xff) == _WaiChaoWen)\
		||((POStus.AL & 0xff) == _BiDu)||((POStus.AL & 0xff) == _KaiDu))		//���źţ��ڳ��£��ⳬ�£��ն£�����
    {
		if(Fsh500mS)
		{
			LED2 = 0;
			LED3 = 1;
		}
		else
		{
			LED2 = 1;
			LED3 = 0;
		}
	}
	else if(((POStus.AL & 0xff) ==_QuanKai)||((POStus.AL & 0xff) ==_QuanBi)||((POStus.AL & 0xff) ==_ZhongWei)\
		||((POStus.AL & 0xff) ==_DaoWei)||PIStus.OTS||PIStus.CTS)
	{
		LED2 = 0;
		LED3 = 0;	
	}	
	else if(Moto_FWD_Chk)		    //��ת�ޱ���״̬
	{
		LED2 = 1;
		LED3 = 0;
		POStus.OPENRUN=1;
	}
	else if(Moto_REV_Chk)			//��ת�ޱ���״̬
	{
		LED2 = 0;
		LED3 = 1;
		POStus.SHUTRUN=1;
	}
	else 
	{
		LED2 = 1;
		LED3 = 1;
	}
    */

        
	//��ѡ���ʮ�����ͨ�����
//	OutToCh(ChSel[0], POStus.MRTS);				//���������
//	OutToCh(ChSel[1], POStus.MD);				//�м�λ�� zhaoxw
//	OutToCh(ChSel[2], POStus.SHUTRUN);			//���������
//	OutToCh(ChSel[3], POStus.OPENRUN);			//���������
//	OutToCh(ChSel[4], !RmLcDisSel);				//Զ��,�������
//	OutToCh(ChSel[5], POStus.CTSOUT);			//���������
//	OutToCh(ChSel[6], POStus.OTSOUT);			//���������
//	OutToCh(ChSel[7], POStus.ACLSOUT);			//�ظ�����λ���	
//	OutToCh(ChSel[8], POStus.AOLSOUT);			//��������λ���
//  OutToCh(ChSel[9], POStus.AL);               //���ϱ���
    
//	OutToCh(10, POStus.M_SW);					//�����ת���
//	OutToCh(11, POStus.M_OS);					//�����ת���
//	OutToCh(12, POStus.CV_IO);					//�������� ��ѹ/�������ģʽת��
//    if(RelayRunFlag)//
    RelayRun();
    
    if(1 == (POStus.M_SW & 0x00ff))
    {
        P_M_OS = 0;        //�����ת���
        if(WaitMoto > 200)
        {
            P_M_SW = 1;
        }
    }
    else if(1 == (POStus.M_OS & 0x00ff))
    {
        P_M_SW = 0;        //�����ת���
        if(WaitMoto > 200)
        {
            P_M_OS = 1;
        }
    }
    else
    {
        P_M_OS = 0;
        P_M_SW = 0;
        WaitMoto = 0;
    }
    
    P_C_IO = !(POStus.CV_IO & 0xff) && 1;       //�������� ��ѹ/�������ģʽת�� 
}

/*********************************************************
function name: �߼�ģ���źŲ���

fun	:		
*********************************************************/
void Sample(void)
{
//  u16 PK1,PK2;
//  u16 PI1,PI2;
    //ģ���źŲ���
    Get_Adc_Average();
    //�߼��źŲ���
//  PIStus.BM1=0x01 & P_BM1;			//������
//  PIStus.BM2=0x01 & P_BM2;
//  PIStus.BM3=0x01 & P_BM3;

#ifndef __Debug__
    PIStus.CTS=0x01 & P_CTS;			//�����ؼ��
    PIStus.OTS=0x01 & P_OTS;			//�����ؼ��
#else 
    PIStus.CTS=0x01 & !P_CTS;			//�����ؼ��
    PIStus.OTS=0x01 & !P_OTS;			//�����ؼ��
#endif
    PIStus.OTL=0x01 & !P_OTL;			//���ȼ��
    PIStus.ACLS=0x01 & !P_ACLS;			//�ظ�����λ
    PIStus.AOLS=0x01 & !P_AOLS;			//��������λ
    PIStus.IN_RCL=0x01 & !P_IN_RCL;		//Զ������
    PIStus.IN_ESD=0x01 & !P_IN_ESD;		//Զ��ESD
    PIStus.IN_BC=0x01 & !P_IN_BC;		//Զ�̱���
    //PIStus.IN_SHUT=0x01 & !P_IN_SHUT;	//Զ�̹�
    //PIStus.IN_OPEN=0x01 & !P_IN_OPEN;	//Զ�̿�
    
#if (POWER_MODE == 1)

    PIStus.PHASE = 0x01 & P_PHASE;		//��Դ����
    PIStus.PHASE_LOST = 0x01 & !P_PHASELOST;		//��Դȱ��
    
    if(PhaseDir == PIStus.PHASE)                    //��������
    {
        CloseDirection = SetCloseDir;
    }
    else
    {
        CloseDirection = !SetCloseDir;
    }
    
    //ȱ����
    if(PIStus.PHASE_LOST)
    {
        POStus.AL |= PHASELOST;			    //ȱ��
    }
    else if(POStus.AL & PHASELOST)
    {
        POStus.AL &= ~PHASELOST;
    }
    
 #endif
    
    //�źŴ���
    if(PIStus.CTS)						//�������ź�
    {
        if(1 == CloseDirection)
        {
            POStus.AL |= ERRCTS; 		//�����г̱�����ȫ��
        }
        else
        {
            POStus.AL |= ERROTS;
        }
    }
    else
    {
        if(1 == CloseDirection)
        {
            POStus.AL &= ~ERRCTS; 		//�����г̱�����ȫ��
        }
        else
        {
            POStus.AL &= ~ERROTS;
        }
    }

    if(PIStus.OTS) 						        //�������ź�
    {
        if(1 == CloseDirection)
        {
            POStus.AL |= ERROTS;
        }
        else
        {
            POStus.AL |= ERRCTS; 				//�����г̱�����ȫ��
        }
    }
    else
    {
        if(1 == CloseDirection)
        {
            POStus.AL &= ~ERROTS;
        }
        else
        {
            POStus.AL &= ~ERRCTS; 				//�����г̱�����ȫ��
        }
    }
    
    //���������źŴ���
    if(AvVal[_IT_C] > 800)					//����70�ȱ���
    {
        //POStus.AL |= INTTHIGH;			    //�ڳ���
    }
    else if(POStus.AL & INTTHIGH)
    {
        POStus.AL &= ~INTTHIGH;
    }
    
    if(PIStus.OTL)
    {
        POStus.AL |= EXTTHIGH;			    //�ⳬ��
    }
    else if(POStus.AL & EXTTHIGH)
    {
        POStus.AL &= ~EXTTHIGH;
    }
        
    RelayByte.Byte.RelayCTor = POStus.AL & ERRCTS;
    RelayByte.Byte.RelayOTor = POStus.AL & ERROTS;
    RelayByte.Byte.RelayTor = RelayByte.Byte.RelayCTor | RelayByte.Byte.RelayOTor;
         
    if(OutCVSel)											//����ѡ��������ǵ�ѹ
    {
        PIStus.VMA_IN = AvVal[_SD];                         //����
        if(PIStus.VMA_IN >= CInLmdD)
        {
            POStus.AL &= ~VMALOW;
            PIStus.VMA_IN_Pers = (PIStus.VMA_IN - CInLmdD)* 10000 / (CInLmdU - CInLmdD);//ȡ�ø�������(%)
        }
        else                              //ģ��������ʱ��ű�
        {												    //��ֵ��
            PIStus.VMA_IN_Pers = 0; 						//ȡ�ø�������ֵΪ0(%)
            //���źż�� 
            if(PIStus.VMA_IN <= (CInLmdD >> 1) && (0 == A_DInSel))//&& (RmLcDisSel == 0))             //С��2mA��ģ��
            {//���ѡ��ģ�������Żᱨ���źţ����Ǳ���Ҳ����ʾ�˱�����ֻ������Ӱ�챾�ع���
                POStus.AL |= VMALOW;						//���ź�!
            }
            else
            {
                POStus.AL &= ~VMALOW;					    //�嶪�ź�!
            }
        }
    }
    else
    {
        PIStus.VMA_IN = AvVal[_IN];						    //��ѹ
        if(PIStus.VMA_IN >= VInLmdD)
        {
            PIStus.VMA_IN_Pers = (PIStus.VMA_IN - VInLmdD) * 10000 / (VInLmdU - VInLmdD);	//ȡ�ø�������(%)
        }
    }

    PIStus.FK_IN = AvVal[_FK];								//ȡ�÷���ȡ�ø���
    

    if(PIStus.FK_IN < 10)//��ѹ���ͣ�˵��ʹ�õ��Ǳ�����
    {
        ReadAngle();
        PIStus.FK_IN_Pers = EncoderToPer();
    }
    else
    {
        if(SetCloseDir)
        {
            if(PIStus.FK_IN >= PosLmdD)							    //��λ����
            {
                PIStus.FK_IN_Pers= (PIStus.FK_IN - PosLmdD)* 10000 / (PosLmdU - PosLmdD);	//ȡ�÷�������(%)
            }
            else
            {
                PIStus.FK_IN_Pers = ((PosLmdD - PIStus.FK_IN)* 10000 / (PosLmdU - PosLmdD)) | 0x8000;//ȡ�ø�ֵ��������(%)
            }
        }
        else
        {
            if(PIStus.FK_IN <= PosLmdU)							    //��λ����
            {
                PIStus.FK_IN_Pers= (PosLmdU - PIStus.FK_IN)* 10000 / (PosLmdU - PosLmdD);	//ȡ�÷�������(%)
            }
            else
            {
                PIStus.FK_IN_Pers = ((PIStus.FK_IN - PosLmdU)* 10000 / (PosLmdU - PosLmdD)) | 0x8000;//ȡ�ø�ֵ��������(%)
            }
        }   
    }
    /***********************ͨѶ����****************************/
    if(Moto_REV_Chk)
    {
        RMbData.sMbData.MbSwitch = 1;
    }
    else if(Moto_FWD_Chk)
    {
        RMbData.sMbData.MbSwitch = 2;
    }
    else
    {
        RMbData.sMbData.MbSwitch = 3;
    }
    RMbData.sMbData.MbVavlePer = PIStus.FK_IN_Pers;
    RMbData.sMbData.MbCloseDir = SetCloseDir;
    RMbData.sMbData.MbCloseMode = Closeway;
    RMbData.sMbData.MbEsdMode = CPUtemp;
    RMbData.sMbData.MbLow = ESDset;
    RMbData.sMbData.MbRemotMode = A_DInSel;
    RMbData.sMbData.MbRelay[0] = RelayType[0];
    RMbData.sMbData.MbRelay[1] = RelayType[1];
    RMbData.sMbData.MbRelay[2] = RelayType[2];
    RMbData.sMbData.MbRelay[3] = RelayType[3];
    RMbData.sMbData.MbRelay[4] = RelayType[4];
    RMbData.sMbData.MbRelay[5] = RelayType[5];
    RMbData.sMbData.MbRelay[6] = RelayType[6];
    RMbData.sMbData.MbRelay[7] = RelayType[7];
    RMbData.sMbData.MbRelay[8] = RelayType[8];
    RMbData.sMbData.MbCPUTemp = CPUtemp;
    RMbData.sMbData.MbVmaPer = PIStus.VMA_IN_Pers;
    RMbData.sMbData.MbSN = 123;
    RMbData.sMbData.MbTimer = uTimer.UTimer;
    RMbData.sMbData.MbVer = 456;
    RMbData.sMbData.MbAl = POStus.AL;
    
#if (MB_MODE == 0)
    if(MbStatus.MbOnline)//���������б�����Զ����������
    {
        if(Rs485Way)//���������߿���
        {
            PIStus.VMA_IN_Pers = WMbData.sMbData.MbVavlePer;
        }
        POStus.AL &= ~VMALOW;
        if(MbStatus.MbInData)
        {
            SetCloseDir = WMbData.sMbData.MbCloseDir;
            Closeway       = WMbData.sMbData.MbCloseMode;
            ESDset         = WMbData.sMbData.MbEsdMode;
            NoSignalCtrSel = WMbData.sMbData.MbLow;
            A_DInSel       = WMbData.sMbData.MbRemotMode;
            RelayType[0] = WMbData.sMbData.MbRelay[0];
            RelayType[1] = WMbData.sMbData.MbRelay[1];
            RelayType[2] = WMbData.sMbData.MbRelay[2];
            RelayType[3] = WMbData.sMbData.MbRelay[3];
            RelayType[4] = WMbData.sMbData.MbRelay[4];
            RelayType[5] = WMbData.sMbData.MbRelay[5];
            RelayType[6] = WMbData.sMbData.MbRelay[6];
            RelayType[7] = WMbData.sMbData.MbRelay[7];
            RelayType[8] = WMbData.sMbData.MbRelay[8];
            SaveDataToFlash();
            MbStatus.MbInData = 0;
        }
    }
    else
    {
        if(Rs485Way)//���������߿���
        {
            POStus.AL |= VMALOW;//���ź�
        }
    }
#endif        
    /***********************************************************/
    
    //��ʾ�߼��ź��� ������
    /*
    AvVal[7]=(PIStus.BM1<<12) + (PIStus.BM2<<11) + (PIStus.BM3<<10) + (PIStus.OTL<<9) + 
             (PIStus.CTS<<8) + (PIStus.OTS<<7) + (PIStus.ACLS<<6) + (PIStus.AOLS<<5) + 
             (PIStus.IN_RCL<<4) + (PIStus.IN_ESD<<3) + (PIStus.IN_BC<<2) + (PIStus.IN_SHUT<<1) + (PIStus.IN_OPEN);
    
    AvVal[8]=(((POStus.MRTS & 0xff)&&1)<<11) + (((POStus.MD& 0xff)&&1)<<10) + (((POStus.SHUTRUN& 0xff)&&1)<<9) + 
             (((POStus.OPENRUN& 0xff)&&1)<<8) + (((POStus.RCL& 0xff)&&1)<<7) + (((POStus.CTSOUT& 0xff)&&1)<<6) + 
             (((POStus.OTSOUT& 0xff)&&1)<<5) + (((POStus.ACLSOUT& 0xff)&&1)<<4) + (((POStus.AOLSOUT & 0xff)&&1)<<3)+ 
             (((POStus.AL & 0xff)&&1)<<2) + (((POStus.M_SW& 0xff)&&1)<<1) + ((POStus.M_OS & 0xff)&&1);
    */
}

/*******************************************/
u16 RelayType[9] = {0,1,2,3,5,6,9,10,7};       //����̵�����Ҫ��̬���������

RELAY_BYTE RelayByte;
RELAY_BIT RelayBit;

void RelayRun(void)
{
    RelayBit.Relay1 = RelayByte.ByteArray[RelayType[0]] && 1;
    RelayBit.Relay2 = RelayByte.ByteArray[RelayType[1]] && 1;
    RelayBit.Relay3 = RelayByte.ByteArray[RelayType[2]] && 1;
    RelayBit.Relay4 = RelayByte.ByteArray[RelayType[3]] && 1;
    RelayBit.Relay5 = RelayByte.ByteArray[RelayType[4]] && 1;
    RelayBit.Relay6 = RelayByte.ByteArray[RelayType[5]] && 1;
    RelayBit.Relay7 = RelayByte.ByteArray[RelayType[6]] && 1;
    RelayBit.Relay8 = RelayByte.ByteArray[RelayType[7]] && 1;
    RelayBit.Relay9 = RelayByte.ByteArray[RelayType[8]] && 1;
    //RelayBit.Relay10 = RelayByte.ByteArray[RelayType[9]] && 1;
    RelayBit.Relay10 = POStus.AL && 1;      //�̶�
/*    
    Po1 = RelayBit.Relay10;     //�Oҕ
    Po2 = !RelayBit.Relay1;     //OUT1
    Po3 = !RelayBit.Relay2;     //OUT2
    Po4 = !RelayBit.Relay3;     //OUT3
    Po5 = !RelayBit.Relay4;     //OUT4
    Po6 = !RelayBit.Relay5;     //OUT5
    Po7 = !RelayBit.Relay6;     //OUT6
    Po8 = !RelayBit.Relay7;     //OUT7
    Po9 = !RelayBit.Relay8;     //OUT8
    Po10 = !RelayBit.Relay9;    //OUT9
*/ 
/*
    P_OTSOUT = RelayBit.Relay10;      //OUT0
    P_ACLSOUT = !RelayBit.Relay1;     //OUT1
    P_AOLSOUT = !RelayBit.Relay2;     //OUT2	
    P_AL = !RelayBit.Relay3;          //OUT3
    */
    
    P_AL = RelayBit.Relay10;           //AL
    P_AOLSOUT = !RelayBit.Relay1;     //OUT0
    P_ACLSOUT = !RelayBit.Relay2;     //OUT1
    P_OTSOUT = !RelayBit.Relay3;      //OUT2	
    P_CTSOUT = !RelayBit.Relay4;      //OUT3
}


///**************************************/
////����������
///**************************************/
void   Delay(u16 Time)
{
    u16 i = 0;
    for (i = 0; i < Time; i++)
    {
        __NOP();
        IWDG_feed();
    }
}

void  WriteByte(u8  Write_Value)
{
     u8 i = 0;
    
     DIO_OUT();
     for (i = 0; i < 8; i++)
     {
        SCLK_SET();
        Delay(15);
        if (Write_Value & 0x80)
        {
            DIO_SET();
        }
        else
        {
            DIO_CLR();
        }
        SCLK_CLR();
        Write_Value <<= 1;
        Delay(20);
     }
}
u8  ReadByte(void)
{
    u8 i = 0;
    u8 Read_Value = 0;
    
    DIO_IN();
    for (i = 0; i < 8; i++)
    {
       SCLK_SET();
       Delay(15);
       Read_Value <<= 1;
       SCLK_CLR();
       if (READ_DIO())
       {
          Read_Value |= 1; 
       }
       else
       {
          Read_Value &= 0xFE;
       }
       Delay(20);
    }
    return Read_Value;
}

void ReadAngle(void)
{
    u16  Angle_Value = 0; 
    u16  Check_Angle_Value = 0;
    static u16 FwCode[5];
    static u8 cnt;
    
    CS_CLR();
    Delay(5);
    WriteByte(0xaa); //1
    WriteByte(0xff); //2
    Delay(10);
    Angle_Value = ReadByte(); //3
    Angle_Value <<= 8;
    Angle_Value |= ReadByte(); //4 
    Check_Angle_Value = Angle_Value;
    CS_SET();
    
    if(Check_Angle_Value & 0x0001)
    {
        Angle_Value = (Angle_Value >> 4);
        FwCode[cnt++] = Angle_Value;
        if(cnt >= 5)
        {
            EncoderCurr = GetMedianNum(FwCode,5);
            //EncoderCurr = Angle_Value;
            cnt = 0;
        }
    }
}

//**********************��ֵ�˲�*****************************

u16 GetMedianNum(u16 *bArray, u8 iFilterLen)  
{  
    u8 i, j;
    u16 temp;
	for (j = 0; j < iFilterLen - 1; j++)  
	{  
		 for (i = 0; i < iFilterLen - j; i++)   
		{   
			 if (bArray[i] > bArray[i + 1])  
			 {     
				temp = bArray[i];   
				bArray[i] = bArray[i + 1];   
				bArray[i + 1] = temp;  
			 }  
		} 
	}
	return bArray[(iFilterLen - 1) / 2]; 
}  
/**************************************/
//������������� ֻ���г��趨ʱ����
/**************************************/
u8 IsEncoderZero(u8 Dir)
{
    static u8 Temp =0x01, flag = 0;
    //ֻ�ܽ���һ��
    if(EncoderCurr <= ENCODER_ZERO)
    {
        if(Temp == 3)
        {
            Dir--;
        }
        if(1 != flag)
        {
            Temp <<= 0x01;//�����
            flag = 1;
        }
    }
    else if(EncoderCurr >= (ENCODER_MAX - ENCODER_ZERO))
    {
        if(Temp == 2)
        {
            Dir++;//
        }
        if(2 != flag)
        {
            Temp += 0x02;
            flag = 2;
        }
    }
    else
    {
        Temp = 1;
        flag = 0;
    }
//    if(Temp>3)
//    {
//        switch(Temp)
//        {
//    //        case 2:break;//�����
//    //        case 3:break;//�����
//            case 4:Dir++;break;//�ҵ���
//            case 6:Dir--;break;//����
//            default:Dir = 127; break;
//        }
//    }
    return Dir;
}

/**************************************/
//������ֵת��Ϊ��λ�ٷֱ�
/**************************************/
u16 EncoderToPer(void)
{
    s32 Len,Curr;
    u32 PosLmdUTemp,PosLmdDTemp,EncoderCurrTemp;
    u16 Per,Mid;
    s32 temp;

    if(isZero == 127)//�������
    {
        PosLmdDTemp = PosLmdD;
        PosLmdUTemp = PosLmdU;
        EncoderCurrTemp = EncoderCurr;
        if(PosLmdU > PosLmdD)
        {
            PosLmdUTemp = PosLmdD;
            PosLmdDTemp = PosLmdU;
        }
        
        Len = PosLmdUTemp - PosLmdDTemp;
        
        Mid = Len / 2 + PosLmdDTemp;
        if(Mid > ENCODER_MAX / 2)
        {Mid -=ENCODER_MAX / 2;}
        else
        {Mid +=ENCODER_MAX / 2;}
        Curr = EncoderCurrTemp - PosLmdDTemp;
        if(Mid > ENCODER_MAX / 2)
        {
            if(EncoderCurrTemp > Mid)
            {
                Curr = ENCODER_MAX - EncoderCurrTemp + PosLmdDTemp;
                Curr = Curr*(-1);
            }
            else if(EncoderCurrTemp < PosLmdDTemp)
            {
                Curr = PosLmdDTemp - EncoderCurrTemp;
                Curr = Curr*(-1);
            }
        }
        else
        {
            if(EncoderCurrTemp < Mid)
            {
                Curr = ENCODER_MAX - PosLmdUTemp + EncoderCurrTemp;
                Curr = Curr + Len;
            }
            else if(EncoderCurrTemp > PosLmdUTemp)
            {
                Curr = EncoderCurrTemp - PosLmdUTemp;
                Curr = Curr + Len;
            }
        }
        if(PosLmdD < PosLmdU)
        {
            Curr = Len - Curr;
        }
    }
    else
    {
        PosLmdDTemp = PosLmdD;
        PosLmdUTemp = PosLmdU;
        EncoderCurrTemp = EncoderCurr;
        if(PosLmdD < PosLmdU)
        {
            PosLmdUTemp = PosLmdD;
            PosLmdDTemp = PosLmdU;
        }
        Len = ENCODER_MAX - (PosLmdDTemp - PosLmdUTemp);
        Mid = Len / 2;
        if(Mid > PosLmdUTemp)
        {
            Mid += PosLmdDTemp - ENCODER_MAX / 2;
        }
        else
        {
            Mid = PosLmdUTemp - Mid + ENCODER_MAX / 2;
        }

        if(EncoderCurr <= PosLmdUTemp)
        {
            Curr = Len - (PosLmdUTemp - EncoderCurrTemp);
        }
        else if(EncoderCurrTemp > PosLmdDTemp)
        {
            Curr = EncoderCurrTemp - PosLmdDTemp;
        }
        else
        {
            if(EncoderCurrTemp < Mid)
            {
                Curr = Len + EncoderCurrTemp - PosLmdUTemp;
            }
            else
            {
                Curr = EncoderCurrTemp - PosLmdDTemp;
            }
        }
        if(PosLmdD < PosLmdU)
        {
            Curr = Len - Curr;
        }
    }
    
    temp = (Curr * 10000 / Len);

    if(1 != CloseDirection)//������
    {
        temp = 10000 - temp;
    }
    
    if(temp >= 0)
    {
        Per = temp;
    }
    else
    {
        temp = temp * (-1);
        Per = 0x8000 | temp;
    }
    return Per;
}
