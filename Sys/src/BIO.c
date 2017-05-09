#include "Def.h"
#include "BIO.h"

u16 ChanlCnt;					//输出通道轮换设定
u16 ChSel[] = {9,8,7,6,5,4,3,2,1,0}; //10个输出口的输出通道号。即默认值 : MRTS ChS = 9;MD ChS = 8;SHUTRUN ChS = 7;OPENRUN ChS = 6;
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
u32 EncoderCurr;//编码器当前值
u16 PhaseDir = 0;

void Sample(void);
void Output(void);
void RelayRun(void);

void ReadAngle(void);
u16 GetMedianNum(u16 *bArray, u8 iFilterLen);
u16 EncoderToPer(void);
/***********************************
#define P_ BM1 		PAin(4)		//输入口，编码器
#define P_ BM2 		PAin(5)		//输入口，编码器
#define P_ BM3 		PAin(7)		//输入口，编码器
#define P_ OTL 		PEin(7)		//输入口，过热检测
#define P_ CTS 		PBin(0)		//输入口，闭力矩检测
#define P_ OTS 		PCin(4)		//输入口，开力矩检测
#define P_ ACLS		PCin(3)		//输入口，关辅助限位	
#define P_ AOLS		PCin(1)		//输入口，开辅助限位	
#define P_ IN_RCL	PCin(0)		//输入口，远程输入	
#define P_ IN_ESD	PCin(13)	//输入口，远程ESD	
#define P_ IN_BC	PEin(5)		//输入口，远程保持
#define P_ IN_SHUT 	PEin(4)		//输入口，远程关
#define P_ IN_OPEN 	PEin(3)		//输入口，远程开
//开关量输出通道定义:
#define Po10		PBout(15)	//输出口10， 正在运行/过矩输出
#define Po9		    PBout(14)	//输出口9， 中间位置输出
#define Po8 		PBout(13)	//输出口8， 关运行输出
#define Po7 		PBout(12)	//输出口7， 开运行输出	
#define Po6 		PBout(11)	//输出口6， 远程本地输出 
#define Po5		    PBout(10)	//输出口5， 闭力矩输出		
#define Po4		    PBout(1)	//输出口4， 开力矩输出
#define Po3 		PCout(5)	//输出口3， 关辅助限位输出	
#define Po2 		PAout(6)	//输出口2， 开辅助限位输出			
#define Po1		    PCout(2)	//输出口1， 故障报警输出 	
#define P_M_SW	    PCout(6)	//输出口， 电机驱动输出 
#define P_M_OS	    PCout(7)	//输出口， 电机正反转输出	
#define P_C_IO	    PEout(6)	//输出口， 电压/电流输出模式转换

*/
/*********************************************************
function name: IO口初始化	

fun	:		配置IO口输入输出
*********************************************************/
void IO_Init(void)
{
 
    GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //使能PA,B,C,D,E端口时钟

	//PA4,5,7 SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				 	//根据设定参数初始化GPIOx
	//GPIO_SetBits(GPIOA,GPIO_Pin_4);						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				 	//根据设定参数初始化GPIO
	//GPIO_SetBits(GPIOA,GPIO_Pin_5);						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	//GPIO_SetBits(GPIOA,GPIO_Pin_7); 						//输出高
    
	//PB0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	//GPIO_SetBits(GPIOB,GPIO_Pin_0); 						//输出高
	//PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_8); 						//输出高
	//PC0,1,3,4,13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_0); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_1); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_0); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 	//
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_4); 					 	//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 	//
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//根据设定参数初始化GPIO
	//GPIO_SetBits(GPIOC,GPIO_Pin_13); 					 	//输出高
	//PE1,2,3,4,5,7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_1); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_2); 
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_3); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_4); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_5); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 	//OTL
	GPIO_Init(GPIOE, &GPIO_InitStructure);				 	//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_7); 					 	//输出高
	//OUTPUT
	//PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOA,GPIO_Pin_6); 						//输出高
	//PB1,10,11,12,13,14,15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_1); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_10); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_11); 						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 	//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_12); 					 	//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 	//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_13); 					 	//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOB, &GPIO_InitStructure); 		   			//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_14);						//输出高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 		   		//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		    //
	GPIO_Init(GPIOB, &GPIO_InitStructure); 			  		//根据设定参数初始化GPIO
	//GPIO_ResetBits(GPIOB,GPIO_Pin_15);						
	//PC2,5,6,7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;	//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_2); 						//输出高
    GPIO_SetBits(GPIOC,GPIO_Pin_5); 						//输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_Init(GPIOC, &GPIO_InitStructure);				 	//根据设定参数初始化GPIO
	//GPIO_SetBits(GPIOC,GPIO_Pin_7); 					 	//输出高
	//PE6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOE,GPIO_Pin_6); 						//输出高

	//输出通道选择(默认)
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
	POStus.CV_IO=0;										//电流/电压输入控制
}
 
/*********************************************************
function name: POStus数据输出

fun	:		输出信号量
*********************************************************/
void Output(void)
{
//    POStus.CTSOUT = PIStus.CTS;                 //开关量，闭力矩输出
//    POStus.OTSOUT = PIStus.OTS;                 //开关量，开力矩输出
//    POStus.ACLSOUT = PIStus.ACLS;               //开关量，闭辅助限位输出
//    POStus.AOLSOUT = PIStus.AOLS;               //开关量，开辅助限位输出
//    POStus.RCL = PIStus.IN_RCL;                 //开关量，远程本地输出 RmLcDisSel
//    POStus.SHUTRUN = PIStus.IN_SHUT;            //开关量，关运行输出
//    POStus.OPENRUN = PIStus.IN_OPEN;            //开关量，开运行输出
    
    if(Closeway)                                //通过力矩开关判断是否到位
    {
        if(1 == CloseDirection)                      //正向
		{
            if((PIStus.CTS || (POStus.AL & SHUTBLOCK)) && Moto_REV_Chk)        //反转到极限? 
            {
                POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)                 //是否开启刹车功能
                {
                    Moto_STOP_Drv;                  //电机刹车，停止
                }
                else
                {
                    Moto_PARK_Drv;                  //电机停止
                }
            }

            if((PIStus.OTS || (POStus.AL & OPENBLOCK)) && Moto_FWD_Chk)         //正转到极限?
            {
                POStus.AL &= ~SHUTBLOCK;
                if(electronicbrake)                 //是否开启刹车功能
                {
                    Moto_STOP_Drv;                  //电机刹车，停止
                }
                else
                {
                    Moto_PARK_Drv;                  //电机停止
                }
            }
        }
        else
        {
            if((PIStus.CTS || (POStus.AL & SHUTBLOCK)) && Moto_FWD_Chk)        //反转到极限? 
            {
                POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)                 //是否开启刹车功能
                {
                    Moto_STOP_Drv;                  //电机刹车，停止
                }
                else
                {
                    Moto_PARK_Drv;                  //电机停止
                }
            }

            if((PIStus.OTS || (POStus.AL & OPENBLOCK)) && Moto_REV_Chk)         //正转到极限?
            {
                POStus.AL &= ~SHUTBLOCK;
                if(electronicbrake)                 //是否开启刹车功能
                {
                    Moto_STOP_Drv;                  //电机刹车，停止
                }
                else
                {
                    Moto_PARK_Drv;                  //电机停止
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
	else										//通过限位开关或阀位判断是否到位
	{
        if(1 == CloseDirection)                      //正向
		{
            if((PIStus.CTS || PIStus.ACLS || ((PIStus.FK_IN_Pers & 0x7fff) <= PosAccuracy) \
                || (POStus.AL & SHUTBLOCK) || (PIStus.FK_IN_Pers & 0x8000)) && Moto_REV_Chk)			//关到极限?
			{				
				POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)				//是否开启刹车功能
				{
					Moto_STOP_Drv;				//电机刹车，停止
				}
				else
				{
					Moto_PARK_Drv;				//电机停止
				}
			}
			else if((PIStus.OTS || PIStus.AOLS \
                || (POStus.AL & OPENBLOCK) || ((PIStus.FK_IN_Pers & 0x7fff) >= (10000 - PosAccuracy))) && Moto_FWD_Chk)		//开转到极限?
			{
                POStus.AL &= ~SHUTBLOCK;
				if(electronicbrake)				//是否开启刹车功能
				{
					Moto_STOP_Drv;				//电机刹车，停止
				}
				else
				{
					Moto_PARK_Drv;				//电机停止
				}
			}
		}
		else
		{
			if((PIStus.OTS || PIStus.AOLS || ((PIStus.FK_IN_Pers & 0x7fff) <= PosAccuracy) \
                || (POStus.AL & SHUTBLOCK) || (PIStus.FK_IN_Pers & 0x8000)) && Moto_FWD_Chk) 			//关到极限?
			{
				POStus.AL &= ~OPENBLOCK;
                if(electronicbrake)				//是否开启刹车功能
				{
					Moto_STOP_Drv;				//电机刹车，停止
				}
				else
				{
					Moto_PARK_Drv;				//电机停止
				}
			}
			else if((PIStus.CTS || PIStus.ACLS \
                 || (POStus.AL & OPENBLOCK) || ((PIStus.FK_IN_Pers & 0x7fff)>= (10000 - PosAccuracy))) && Moto_REV_Chk)		//开转到极限?
			{
				POStus.AL &= ~SHUTBLOCK;
                if(electronicbrake)				//是否开启刹车功能
				{
					Moto_STOP_Drv;				//电机刹车，停止
				}
				else
				{
					Moto_PARK_Drv;				//电机停止
				}
			}
		}
	}
    
    if(POStus.AL & (INTTHIGH | EXTTHIGH | ERRDIR | PHASELOST))    //报警停止
    {
        if(electronicbrake)				//是否开启刹车功能
        {
            Moto_STOP_Drv;				//电机刹车，停止
        }
        else
        {
            Moto_PARK_Drv;				//电机停止
        }
    }
        
	if(1 == CloseDirection)                              //正向
    {
        if(Moto_FWD_Chk)		                    //正转无报警状态
        {
            RelayByte.Byte.RelayNowOpen = 1;        //会被远程开关信号清零
        }
        else
        {
            RelayByte.Byte.RelayNowOpen = 0;
        }
        if(Moto_REV_Chk)			                //反转无报警状态
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
            {//到位信号反馈
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
        if(Moto_FWD_Chk)		                    //正转无报警状态
        {
            RelayByte.Byte.RelayNowShut = 1;
        }
        else
        {
            RelayByte.Byte.RelayNowShut = 0;
        }
        if(Moto_REV_Chk)			                //反转无报警状态
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
    
    //中间位置
    if(PIStus.FK_IN_Pers & 0x8000)//负数
    {
        RelayByte.Byte.RelayMid1 = 1;
    }
    else if(PIStus.FK_IN_Pers <  Intermediate1 * 100)			    //中间位置1 0%~中间位置1
    {
        RelayByte.Byte.RelayMid1 = 1;
    }
    else
    {
        RelayByte.Byte.RelayMid1 = 0;
    }
    
    if((PIStus.FK_IN_Pers & 0x7fff) >  Intermediate2 * 100)			//中间位置2 中间位置2~100%
    {
        RelayByte.Byte.RelayMid2= 1;
    }
    else
    {
        RelayByte.Byte.RelayMid2 = 0;
    }
        
    RelayByte.Byte.RelayLocal = RmLcDisSel;             //0远程 大于零非远程 1 就地 2 禁止
    
    if(POStus.AL)
    {
        RelayByte.Byte.RelayFault = 1;
    }
    else
    {
        RelayByte.Byte.RelayFault = 0;
    }
    
	if(OutCVSel)								        //输出电流或电压选择
	{
		POStus.CV_IO = 0;				
	}
	else
	{
		POStus.CV_IO = 1;		
	}			


	//PWM输出
	if(OutCVSel)
	{
		PWMDuty = 4095 - POStus.MA_OUT;
	}
	else
	{
		PWMDuty = 4095 - POStus.V_OUT;
	}
  	TIM_SetCompare3(TIM3, PWMDuty);							//输出PWM
	
	//LED显示
    //zhaoxw
	//if((PIStus.VMA_IN <= (CInLmdD/2))||(POStus.AL & 0xff== _NeiChaoWen)||(POStus.AL & 0xff== _WaiChaoWen)\
	//	||(POStus.AL & 0xff == _BiDu)||(POStus.AL & 0xff== _KaiDu))		//丢信号，内超温，外超温，闭堵，开堵
	
    /*if((PIStus.VMA_IN <= (CInLmdD/2))||((POStus.AL & 0xff) == _NeiChaoWen)||((POStus.AL & 0xff) == _WaiChaoWen)\
		||((POStus.AL & 0xff) == _BiDu)||((POStus.AL & 0xff) == _KaiDu))		//丢信号，内超温，外超温，闭堵，开堵
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
	else if(Moto_FWD_Chk)		    //正转无报警状态
	{
		LED2 = 1;
		LED3 = 0;
		POStus.OPENRUN=1;
	}
	else if(Moto_REV_Chk)			//反转无报警状态
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

        
	//可选择的十个输出通道输出
//	OutToCh(ChSel[0], POStus.MRTS);				//过力矩输出
//	OutToCh(ChSel[1], POStus.MD);				//中间位置 zhaoxw
//	OutToCh(ChSel[2], POStus.SHUTRUN);			//关运行输出
//	OutToCh(ChSel[3], POStus.OPENRUN);			//开运行输出
//	OutToCh(ChSel[4], !RmLcDisSel);				//远程,本地输出
//	OutToCh(ChSel[5], POStus.CTSOUT);			//闭力矩输出
//	OutToCh(ChSel[6], POStus.OTSOUT);			//开力矩输出
//	OutToCh(ChSel[7], POStus.ACLSOUT);			//关辅助限位输出	
//	OutToCh(ChSel[8], POStus.AOLSOUT);			//开辅助限位输出
//  OutToCh(ChSel[9], POStus.AL);               //故障报警
    
//	OutToCh(10, POStus.M_SW);					//电机正转输出
//	OutToCh(11, POStus.M_OS);					//电机反转输出
//	OutToCh(12, POStus.CV_IO);					//开关量， 电压/电流输出模式转换
//    if(RelayRunFlag)//
    RelayRun();
    
    if(1 == (POStus.M_SW & 0x00ff))
    {
        P_M_OS = 0;        //电机反转输出
        if(WaitMoto > 200)
        {
            P_M_SW = 1;
        }
    }
    else if(1 == (POStus.M_OS & 0x00ff))
    {
        P_M_SW = 0;        //电机反转输出
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
    
    P_C_IO = !(POStus.CV_IO & 0xff) && 1;       //开关量， 电压/电流输出模式转换 
}

/*********************************************************
function name: 逻辑模拟信号采样

fun	:		
*********************************************************/
void Sample(void)
{
//  u16 PK1,PK2;
//  u16 PI1,PI2;
    //模拟信号采样
    Get_Adc_Average();
    //逻辑信号采样
//  PIStus.BM1=0x01 & P_BM1;			//编码器
//  PIStus.BM2=0x01 & P_BM2;
//  PIStus.BM3=0x01 & P_BM3;

#ifndef __Debug__
    PIStus.CTS=0x01 & P_CTS;			//闭力矩检测
    PIStus.OTS=0x01 & P_OTS;			//开力矩检测
#else 
    PIStus.CTS=0x01 & !P_CTS;			//闭力矩检测
    PIStus.OTS=0x01 & !P_OTS;			//开力矩检测
#endif
    PIStus.OTL=0x01 & !P_OTL;			//过热检测
    PIStus.ACLS=0x01 & !P_ACLS;			//关辅助限位
    PIStus.AOLS=0x01 & !P_AOLS;			//开辅助限位
    PIStus.IN_RCL=0x01 & !P_IN_RCL;		//远程输入
    PIStus.IN_ESD=0x01 & !P_IN_ESD;		//远程ESD
    PIStus.IN_BC=0x01 & !P_IN_BC;		//远程保持
    //PIStus.IN_SHUT=0x01 & !P_IN_SHUT;	//远程关
    //PIStus.IN_OPEN=0x01 & !P_IN_OPEN;	//远程开
    
#if (POWER_MODE == 1)

    PIStus.PHASE = 0x01 & P_PHASE;		//电源相序
    PIStus.PHASE_LOST = 0x01 & !P_PHASELOST;		//电源缺相
    
    if(PhaseDir == PIStus.PHASE)                    //相序正序？
    {
        CloseDirection = SetCloseDir;
    }
    else
    {
        CloseDirection = !SetCloseDir;
    }
    
    //缺相检测
    if(PIStus.PHASE_LOST)
    {
        POStus.AL |= PHASELOST;			    //缺相
    }
    else if(POStus.AL & PHASELOST)
    {
        POStus.AL &= ~PHASELOST;
    }
    
 #endif
    
    //信号处理
    if(PIStus.CTS)						//闭力矩信号
    {
        if(1 == CloseDirection)
        {
            POStus.AL |= ERRCTS; 		//超下行程报警，全闭
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
            POStus.AL &= ~ERRCTS; 		//超下行程报警，全闭
        }
        else
        {
            POStus.AL &= ~ERROTS;
        }
    }

    if(PIStus.OTS) 						        //开力矩信号
    {
        if(1 == CloseDirection)
        {
            POStus.AL |= ERROTS;
        }
        else
        {
            POStus.AL |= ERRCTS; 				//超下行程报警，全闭
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
            POStus.AL &= ~ERRCTS; 				//超下行程报警，全闭
        }
    }
    
    //错误输入信号处理
    if(AvVal[_IT_C] > 800)					//超过70度报警
    {
        //POStus.AL |= INTTHIGH;			    //内超温
    }
    else if(POStus.AL & INTTHIGH)
    {
        POStus.AL &= ~INTTHIGH;
    }
    
    if(PIStus.OTL)
    {
        POStus.AL |= EXTTHIGH;			    //外超温
    }
    else if(POStus.AL & EXTTHIGH)
    {
        POStus.AL &= ~EXTTHIGH;
    }
        
    RelayByte.Byte.RelayCTor = POStus.AL & ERRCTS;
    RelayByte.Byte.RelayOTor = POStus.AL & ERROTS;
    RelayByte.Byte.RelayTor = RelayByte.Byte.RelayCTor | RelayByte.Byte.RelayOTor;
         
    if(OutCVSel)											//给定选择电流还是电压
    {
        PIStus.VMA_IN = AvVal[_SD];                         //电流
        if(PIStus.VMA_IN >= CInLmdD)
        {
            POStus.AL &= ~VMALOW;
            PIStus.VMA_IN_Pers = (PIStus.VMA_IN - CInLmdD)* 10000 / (CInLmdU - CInLmdD);//取得给定开度(%)
        }
        else                              //模拟量控制时候才报
        {												    //负值，
            PIStus.VMA_IN_Pers = 0; 						//取得给定开度值为0(%)
            //丢信号检查 
            if(PIStus.VMA_IN <= (CInLmdD >> 1) && (0 == A_DInSel))//&& (RmLcDisSel == 0))             //小于2mA切模拟
            {//如果选中模拟量，才会报丢信号，但是本地也会显示此报警，只不过不影响本地工作
                POStus.AL |= VMALOW;						//丢信号!
            }
            else
            {
                POStus.AL &= ~VMALOW;					    //清丢信号!
            }
        }
    }
    else
    {
        PIStus.VMA_IN = AvVal[_IN];						    //电压
        if(PIStus.VMA_IN >= VInLmdD)
        {
            PIStus.VMA_IN_Pers = (PIStus.VMA_IN - VInLmdD) * 10000 / (VInLmdU - VInLmdD);	//取得给定开度(%)
        }
    }

    PIStus.FK_IN = AvVal[_FK];								//取得反馈取得给定
    

    if(PIStus.FK_IN < 10)//电压够低，说明使用的是编码器
    {
        ReadAngle();
        PIStus.FK_IN_Pers = EncoderToPer();
    }
    else
    {
        if(SetCloseDir)
        {
            if(PIStus.FK_IN >= PosLmdD)							    //阀位下限
            {
                PIStus.FK_IN_Pers= (PIStus.FK_IN - PosLmdD)* 10000 / (PosLmdU - PosLmdD);	//取得反馈开度(%)
            }
            else
            {
                PIStus.FK_IN_Pers = ((PosLmdD - PIStus.FK_IN)* 10000 / (PosLmdU - PosLmdD)) | 0x8000;//取得负值反馈开度(%)
            }
        }
        else
        {
            if(PIStus.FK_IN <= PosLmdU)							    //阀位上限
            {
                PIStus.FK_IN_Pers= (PosLmdU - PIStus.FK_IN)* 10000 / (PosLmdU - PosLmdD);	//取得反馈开度(%)
            }
            else
            {
                PIStus.FK_IN_Pers = ((PIStus.FK_IN - PosLmdU)* 10000 / (PosLmdU - PosLmdD)) | 0x8000;//取得负值反馈开度(%)
            }
        }   
    }
    /***********************通讯处理****************************/
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
    if(MbStatus.MbOnline)//串口数据有变且在远程下起作用
    {
        if(Rs485Way)//启动了总线控制
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
        if(Rs485Way)//启动了总线控制
        {
            POStus.AL |= VMALOW;//丢信号
        }
    }
#endif        
    /***********************************************************/
    
    //显示逻辑信号用 测试用
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
u16 RelayType[9] = {0,1,2,3,5,6,9,10,7};       //保存继电器需要组态报警的序号

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
    RelayBit.Relay10 = POStus.AL && 1;      //固定
/*    
    Po1 = RelayBit.Relay10;     //監視
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
////编码器部分
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

//**********************中值滤波*****************************

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
//编码器过零点检测 只在行程设定时调用
/**************************************/
u8 IsEncoderZero(u8 Dir)
{
    static u8 Temp =0x01, flag = 0;
    //只能进入一次
    if(EncoderCurr <= ENCODER_ZERO)
    {
        if(Temp == 3)
        {
            Dir--;
        }
        if(1 != flag)
        {
            Temp <<= 0x01;//在零点
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
//    //        case 2:break;//零点右
//    //        case 3:break;//零点左
//            case 4:Dir++;break;//右到左
//            case 6:Dir--;break;//左到右
//            default:Dir = 127; break;
//        }
//    }
    return Dir;
}

/**************************************/
//编码器值转换为阀位百分比
/**************************************/
u16 EncoderToPer(void)
{
    s32 Len,Curr;
    u32 PosLmdUTemp,PosLmdDTemp,EncoderCurrTemp;
    u16 Per,Mid;
    s32 temp;

    if(isZero == 127)//不过零点
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

    if(1 != CloseDirection)//反作用
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
