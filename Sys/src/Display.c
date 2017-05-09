#include "Def.h"
#include <Math.h>
#define	_coloffset	    13		//从第几列开始显示
#define 	line 		14		//显示行
#define 	jump 		17		//横向字间跳格宽度

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
u8 StusView;							//状态显示信息(字串序号)
u8 FreshTimes = 0;

const u16 *InfoDispStrSet[7][2]={				//(16x32 点阵的汉字或图片序列库)
	Kai_16x32,Fa_16x32, 			            //开阀 1
	Bi_16x32,Fa_16x32,				            //闭阀 2
	Ting_16x32,Zhi3_16x32,	                    //停止 3
	Dao_16x32,Wei_16x32,			            //到位 4
	Zhong_16x32,Wei_16x32,		                //中位 5
	Quan_16x32,Kai_16x32,			            //全开 6	
	Quan_16x32,Bi_16x32,			            //全闭 7
};

//继电器触点可以组态的项目List
const u8 *RelayItem[13][5]={
	Bi_14x14,Wei_14x14,SP_14x14,SP_14x14,0,			            //0  关到位
	Kai_14x14,Wei_14x14,SP_14x14,SP_14x14,0,					//1  开到位
	Bi_14x14,Guo_14x14,Ju_14x14,SP_14x14,0,			            //2  关过矩
	Kai_14x14,Guo_14x14,Ju_14x14,SP_14x14,0,			        //3  开过矩
	Guo_14x14,Ju_14x14,SP_14x14,SP_14x14,0,		                //4  过距
	Bi_14x14,Yun_14x14,xing_14x14,Zhong_14x14,0,	            //5  关运行中（正在关）
	Kai_14x14,Yun_14x14,xing_14x14,Zhong_14x14,0,	            //6  开运行中（正在开）
    Ben_14x14,Di_14x14,SP_14x14,SP_14x14,0,		                //7  本地
	ChrE_14x14,ChrS_14x14,ChrD_14x14,SP_14x14,0,		        //8  ESD信号
	Zhong_14x14,Jian2_14x14,Wei_14x14,Chr1_14x14,0, 	        //9  中间位置1
    Zhong_14x14,Jian2_14x14,Wei_14x14,Chr2_14x14,0, 	        //10 中间位置2
	Bao1_14x14,Jing1_14x14,SP_14x14,SP_14x14,0,				    //11 报警
    Dian1_14x14,Yuan1_14x14,Que1_14x14,Xiang1_14x14,0,				//12 缺相
};

//报警项目List
const u16 *ErrItem[10][5]={
	Zhuan_16x16,Xiang_16x16,Cuo_16x16,Wu_16x16,0,		//转向错误
	Kai_16x16,Bi_16x16,Tong_16x16,Zai_16x16,0,			//开闭同在
	Bi_16x16,Fa_16x16,Du_16x16,Zhuan_16x16,0,			//闭阀堵转
	Kai_16x16,Fa_16x16,Du_16x16,Zhuan_16x16,0,			//开阀堵转
	Dui_16x16,Xin_16x16,Hao_16x16,SP1_16x16,0,		    //丢信号
	Kai_16x16,Guo_16x16,Li_16x16,Ju_16x16,0,			//开过力矩
	Bi_16x16,Guo_16x16,Li_16x16,Ju_16x16,0,				//闭过力矩
	Nei_16x16,Bu_16x16,Chao_16x16,Wen_16x16,0,		    //内部超温
	Dian_16x16,Ji_16x16,Guo_16x16,Re_16x16,0,		    //电机过热
    Dian_16x16,Yuan_16x16,Que_16x16,Xiang1_16x16,0,		//电源缺相
};

u16 TmpSetVal;					
//基本参数设定类
u16 AutoSetOKF;					//自动标定过
u16 isZero;					    //编码器是否过零点
u32 PosLmdU;					//阀位上限		4095
u32 PosLmdD;					//阀位下限
u8  isZeroTemp = 127;
u16 PosAccuracy = 100;			//阀位精度(默认0.50)
u16 JogHolAutoSel = 1;			//现场控制方式:1点动,2保持,3自动
u16 A_DInSel = 1;				//远程输入方式:0模拟量,1开关量,3线控（）
u16 NoSignalCtrSel = 1;			//丢信处理方式	1停止，2全开，3全闭
u16 ScrSel;						//显示方向		0正向  非0反向
u16 language;					//语言选择		0中文 非0英文
//高级参数设定类
u16 electronicbrake;			//电子刹车开启标志    0不开  非0 开启
u16 CloseDirection;				//关闭方向			1正   0反
u16 SetCloseDir;
u16 Closeway;					//Closeway, 非0 力矩   0 限位  判断到达闭位和开位的依据 力矩开关或限位开关
u16 Rs485Way=0;                     //1：总线控制有效读写有效 0：总线控制无效读写有效
u16 ESDset = 1;					    //ESD设置，开位，闭位，禁止
u16 TwoLine = 1;					//两线控制	  1禁止  2有信号开  3有信号关		
u16 LockedRotorTime;			//堵转时间		0- 10S
u16 IntermittentRun;			//间断运行	(非0开间断运行，0关)
u16 Intermediate1;			    //中间位置1 0%~中间位置1
u16 Intermediate2;			    //中间位置2 中间位置1~100%
u16 PassWord;				    //密码(默认为"0000"),限定小于等于9999

u16 CInLmdU;				//给定C上限	3707					//0x0e78
u16 CInLmdD;				//给定C下限	746					    //0x02e8
u16 VInLmdU;				//给定V上限	4095
u16 VInLmdD;				//给定V下限	1

u16 OutCLmdU;			//输出上限		3375					//0x0d2f
u16 OutCLmdD;			//输出下限		669		2706			//0x029d
u16 OutVLmdU;				//输出上限		4095
u16 OutVLmdD;				//输出下限		0
u16 RmLcDisSel;					//远程0/本地(本地键盘操作)1/禁止>=2切换
u16 OutCVSel;					//输出电流或电压选择		=1
u16 OutCVSelflag;				//输出电流或电压选择标志	

u16 LRtimeh=0010;				//堵转时间上限
u16 LRtimel=0000;				//堵转时间下限
//u16 Pos=0;						//阀位(开度)
//设置调整标志
//u8 PosAccuracy_flag;				//阀位精度调节标志 zhaoxw 范围不够
u16 PosAccuracy_flag;				//zhaoxw
u16 JogHolAutoSelflag;			//现场控制方式:1点动,2保持,3自动 标志位
u16 A_DInSelflag;					//远程输入方式:模拟量,开关量
u16 NoSignalCtrSelflag;			//丢信标志
u16 ScrSelflag;					//显示方向		0正向  非0反向
u16	languageflag;				//中英文菜单显示标识 
u16 electronicbrakeflag;			//电子刹车开关切换标志
u16 CloseDirectionflag;			//关闭反向
u16 Closewayset;					//关闭方式
u16 Rs485Wayset;
u16 ESDsetflag;					//ESD设置，开位，闭位，禁止
u16 TwoLineflag;					//两线控制
u16 LockedRotorTime_flag;		//堵转时间
u16 mbaddr_flag;		        //通讯地址
u16 mbaddr = 1;		                //通讯地址
u16 IntermittentRunflag;			//间断运行
u16 Intermediate1flag;			//中间位置1
u16 Intermediate2flag;			//中间位置2
u16 StallTime;					//堵转时间Stall time
//u8 ElecBrakEn;					//电子制动使能

u8 EOD;							//正反作用Effect of direction

//故障诊断
u8 Testcloseorpen;				//测试开闭按键是否正常
u8 Testcorpflag;					//测试开闭按键是否正常标识

//u8 NoSignalCtr;					//无信号控制方法设定
//u8 AutoCalEn;					//自动标定设定Auto calibration
u8 MnOffPosSet;					//点动关设定

u16 InVLmdU;					//输入上限
u16 InVLmdD;					//输入下限
u16 TEMPCal;					//温度校准
//u32 PassWord;					//密码,限定小于等于9999

u8 DispStep;		
u16	DispSetSel;		

u8	RolStus=1;		
u16 EnSelFlsh=0;				//使能反白显示闪烁//是否使能被选项闪烁
u16 DispParaSet_test=0;			//是否显示内部参数(测试用)

u8 ClosewayTemp;        //关阀方式暂存 为了在行程设定动作时使限位数据无效
u8 ZeroStatus = 0;        //过零点状态机
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
void BaseSet(void);					//基本设置操作
void FeedBackSet(void);			//反馈设计操作
void AdvancedSet(void);			//高级设置操作
void FaultDiagnosisSet(void);			//故障诊断项目操作

void dispSetMenu1_2_6(void);
void disp_Set1_6_x(u16 Sel);
void disp_Set1_6_xEnglish(u16 Sel);

u8 GetMotoPos(void);	//取得电位器位置

void displayEnglishSetupMenu(void);
void dispEnglishSetMenu(u16 Sel);
void disp_EnglishSet1_1_x(u16 Sel);
void TwoLineRun(void);
void IntermittentRundis(u8 Htarget ,u8 Ltarget);
u8 Trg(u8 trg);
void ClearMenu(void);
/*********************************************************
function name: 显示函数初始化
fun	:	初始化相关参数
*********************************************************/
void Display_Init(void)
{
	PosAccuracy_flag = PosAccuracy;		    //初始化精度
	JogHolAutoSelflag = JogHolAutoSel;		
	A_DInSelflag = A_DInSel;				//远程控制方式
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
function name: 显示处理
fun	:
*********************************************************/
void Display(void)
{
	DisplayVer();						//开机显示
	displayMain();						//主工作画面显示及操作
	if(language )
    {
		displayEnglishSetupMenu();		//英文设置菜单显示
    }
	else
	{	
        displaySetupMenu();				//中文设置菜单显示及操作
        //DisplayTest();				//测试显示
        ClearMenu();
    }
}

/*********************************************************
function name: 设置中文界面操作

fun	:	操作中文菜单显示
*********************************************************/
void displaySetupMenu(void)
{
	dispSetupMenu();			//设置主菜单==>1:基本设置,2:反馈设置,3:高级设置,4:故障诊断.....
	dispSetMenu1_1();			//1:基本设置==>1:自动标定,2:开位确认,3:关位确认,4:控制精度,5:现场控制,6:远程设置,7:丢信动作,8:显示方向
	dispSetMenu1_2();			//2:反馈设置==>1:输出设置,2:给定低信,3:给定高信,4:输出低信,5:输出高信,6:触点输出
	dispSetMenu1_3();			//3:高级设置==>1:ESD设置, 2:关闭方式,3:关闭方向,4:两线控制,5:堵转时间,6:电子刹车,7:间断运行,8:密码设置,9:恢复出厂,
	dispSetMenu1_4();			//4:故障诊断==>1:方式按键,2:操作按键,3:远程打开,4:远程关闭,5:远程保持,6:远程自动,7:恢复出厂,
	dispSetMenu1_2_6();			//设置主菜单==>2:反馈设置==>6:触点输出(输出通道设定)
}

/*********************************************************
function name: 设置英文界面操作

fun	:操作英文菜单显示
*********************************************************/
void displayEnglishSetupMenu(void)
{
	dispSetupMenu();			//设置主菜单==>1:基本设置,2:反馈设置,3:高级设置,4:故障诊断.....
	dispSetMenu1_1();			//1:基本设置==>1:自动标定,2:开位确认,3:关位确认,4:控制精度,5:现场控制,6:远程设置,7:丢信动作,8:显示方向
	dispSetMenu1_2();			//2:反馈设置==>1:输出设置,2:给定低信,3:给定高信,4:输出低信,5:输出高信,6:触点输出
	dispSetMenu1_3();			//3:高级设置==>1:ESD设置,2: 关闭方式,3: 关闭方向,4: 两线控制,5: 堵转时间,6, 电子刹车,7: 间断运行,8: 密码设置,9: 恢复出厂,
	dispSetMenu1_4();			//4:故障诊断==>1: 方式按键,2: 操作按键,3: 远程打开,4: 远程关闭,5: 远程保持,6: 远程自动,//7: 恢复出厂,
	dispSetMenu1_2_6();			//设置主菜单==>2:反馈设置==>6 触点输出(输出通道设定)
}

/*********************************************************
function name: 开机界面

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
			//开机动画显示	
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
				IWDG_feed();		//喂狗
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
			IWDG_feed();		//喂狗
            
            for(i = 0; i < 20; i++)
			{
                IWDG_feed();		    //喂狗
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
            display_char8x16(64,15,"V:0.2.40b",0); 				//版本号
            display_char8x16(80,15,"2017/05/09",0);			//日期

			for(i=0;i<15;i++)
			{
                IWDG_feed();		    //喂狗
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
调试模式显示

*********************************************************/
void DisplayTest(void)
{
	u8 i;
	u32 L,x1,x2,x3;
	
	
	if(DispParaSet_test==1)
		{
			//测试用,显示输入电平
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
			//测试用,显示输出电平
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
	

	if(K5Dn)		//测试遥控 按键
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
function name:堵转判断函数

fun :			判断电机是否堵转
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
//    else if( locktime >= LockedRotorTime*10)				//到达设定堵转判断时间	
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
function name:点动运行函数

fun :	收到正反转信号，转动固定幅度
*********************************************************/
void JogRun(void)
{
	static u8 stopflag=0;		//刹车状态位
	//static u32 runflag=0;		//电机转动位置状态
	if((KeyHoldStus) || (KeyHoldStusA))
	{
        if((key & 0x0f) == 4)		
        {
            if((GetMotoPos() < _MoreLdU))	//点动状态每次转	50
            {
                Moto_FWD_Drv;			    //正转
                                            //电机转动了，需要刹车，置位刹车状态机
                stopflag = 1;				//电机转动了，需要刹车，置位刹车状态机
            }            
        }
        else if(((key & 0x0f) == 3))
        {
            if((GetMotoPos() > _LessLdD) )	
            {
                Moto_REV_Drv;			    //反转
                stopflag = 1;				//电机转动了，需要刹车，置位刹车状态机
            }                        
        }
        
        if(((key & 0x0f) == 0)&&stopflag)
        {

            Moto_STOP_Drv;			//电机刹车
            stopflag = 0;
        }
	}
	else
	{
		if(electronicbrake)				//如果开启了电子杀车功能
		{
			Moto_STOP_Drv;
		}
		else								//没有开启电子刹车功能
		{
			Moto_PARK_Drv;
		}
	}
}

/*********************************************************
function name:保持运行函数

fun :	收到正反转信号，保持转动至信号结束
*********************************************************/
void KeepRun(void)
{
	if((KeyHoldStus) || (KeyHoldStusA))
	{
		if(K4Dn)
		{
            if((GetMotoPos() < _MoreLdU))
            {
                Moto_FWD_Drv;	//正转
            }
		}
		else if(K3Dn)
		{
            if((GetMotoPos() > _LessLdD))
            {
                Moto_REV_Drv;   //反转
            }
		}
	}
}

/*********************************************************
function name:自动运行函数

fun :	收到正反转信号，短按信号与点动一样，长按信号运转到极限位置
*********************************************************/
void AnimationrRun(void)
{
	static u8 stopflag=0;		//刹车状态位
//	static u32 runflag=0;		//电机转动位置状态
	if(((KeyHoldStus) )|| ((KeyHoldStusA)))
	{
		if(K4Dn)
		{
            if(GetMotoPos() < _MoreLdU) 
            {
                Moto_FWD_Drv;			//正转
                stopflag = 1;				//电机转动了，需要刹车，置位刹车状态机
                if(Klong)
                {
                    if(GetMotoPos() < _MoreLdU) 
                    {
                        Moto_FWD_Drv;	//正转	
                        stopflag = 0;
                    }
                }
            }
		}
		else if(K3Dn)
		{
			if(GetMotoPos() > _LessLdD) 
			{
				Moto_REV_Drv;				    //反转
				stopflag = 1;					//电机转动了，需要刹车，置位刹车状态机
				if(Klong)						//长按，自动闭到位
				{
					if(GetMotoPos() > _LessLdD) 
					{
                        Moto_REV_Drv;			//反转
                        stopflag = 0;				//电机转动了，需要刹车，置位刹车状态机
					}
				}
			}
		}
	}
	else if((!KeyHoldStusA) && (!KeyHoldStus) && stopflag)//zhaoxw
	{
        if(electronicbrake)				//如果开启了电子杀车功能
        {
            Moto_STOP_Drv;
        }
        else							//没有开启电子刹车功能
        {
            Moto_PARK_Drv;
        }
	}
}

/*********************************************************
function name:本地运行函数

fun :			选择本地时 displayMain状态执行命令
*********************************************************/
void  Localdisplay(void)
{
	//u8 static stc=0;//zhaoxw
//    static u8 stc=0;
	if(JogHolAutoSel == 1)				    //点动-----------------
	{
		JogRun();
	}
	else if(JogHolAutoSel== 2)				//保持-----------------
	{
		KeepRun();
	}
	else if(JogHolAutoSel == 3)				//自动-----------------
	{
		AnimationrRun();
	}
    
//    if(PIStus.IN_RCL==1&&stc)				//远程按键切换 本地 /远程控制
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
function name:远程模拟量运行函数

fun :			选择远程模拟时 displayMain状态执行命令
*********************************************************/
void RemoteAnalogdisplay(void)
{		
	//if(IntermittentRun==0)//模拟量不需要间断运行
	{
		if((GetMotoPos() ==_InTarget))			//阀位在目标值上
		{
            if(electronicbrake)				    //如果开启了电子杀车功能
			{
				Moto_STOP_Drv;
			}
			else								//没有开启电子刹车功能
			{
				Moto_PARK_Drv;
			}
		}
		else if(GetMotoPos() ==  _LessTarget)		//小于目标值，正转
		{
			Moto_FWD_Drv;
		}
		else if(GetMotoPos() == _MoreTarget)		//大于目标值反转
		{
			Moto_REV_Drv;
		}
	}

	if(POStus.AL & VMALOW)//
	{
        //丢信处理
		if(NoSignalCtrSel==1)						//停止，保持当前位置
		{
			Moto_PARK_Drv;						
		}
		else if(NoSignalCtrSel == 2)					//全开
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
		else if(NoSignalCtrSel == 3)					//全闭
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
function name:远程开关量运行函数

fun :			选择远程开关量时 displayMain状态执行命令
*********************************************************/
void RemoteQquantitydisplay(void)
{
//	u8 static stc = 0;	//远程/本地状态机
	u8 static sta = 0 ;	//BC状态机
	u8 static stic = 0;
    static u8 TimerFlag;
	 if(PIStus.IN_BC==1&&sta)					//远程自动按键
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
//****************************************远程 /本地切换	 
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
    if(TwoLine == 1)			//禁止
    {
        POStus.AL &= ~ERRCMD;
        if(!P_IN_SHUT && !P_IN_OPEN)//开关同在(不适用在两线控制)
        {
            TimerFlag = 0;
            PIStus.IN_OPEN = 0x01;
            PIStus.IN_SHUT = 0x01;
            POStus.AL |= ERRCMD;
        }
        else if(!P_IN_SHUT || !P_IN_OPEN)//后按按键无效
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
                    Moto_PARK_Drv;//停止
                }
            }
            else if(PIStus.IN_SHUT)
            {
                if(GetMotoPos() >_LessLdD) 
                {
                    Moto_REV_Drv;//反转
                }
                else
                {
                    Moto_PARK_Drv;//停止	
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
                    Moto_REV_Drv;//反转
                }
                else
                {
                    Moto_PARK_Drv;//停止	
                }
            }
            else if(PIStus.IN_OPEN)
            {
                if(GetMotoPos() <_MoreLdU) 
                {
                    Moto_FWD_Drv;//正转
                }
                else
                {
                    Moto_PARK_Drv;//停止
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
//****************************************远程按键开闭阀
		if(PIStus.IN_BC)								//远程自动
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
			{						//开到位
				if(GetMotoPos() <_MoreLdU  ) 
				{
					Moto_FWD_Drv;//正转
				}
				else
				{
					Moto_PARK_Drv;//停止
				}
			}
			else if(stic == 2)
			{
									  //闭到位
				if(GetMotoPos() >_LessLdD) 
				{
					Moto_REV_Drv;//反转
				}
				else
				{
					Moto_PARK_Drv;//停止	
				}
			}
            else if(stic == 3)
            {
                Moto_PARK_Drv;//停止
            }
		}
		else											//远程点动
		{
			if(PIStus.IN_OPEN && !PIStus.IN_SHUT)						//远程按键开阀
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
			else  if(!PIStus.IN_OPEN && PIStus.IN_SHUT)				//远程按键闭阀		
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

	
/*********************************** 远程按键BC运行部分
	 if(stic==1&&!PIStus.IN_OPEN&&!PIStus.IN_SHUT&&!PIStus.IN_ESD)	
	{
					//开到位
		if(GetMotoPos() <_MoreLdU  ) 
		{
			Moto_FWD_Drv;//正转
		}
		else
		{
			Moto_PARK_Drv;//停止
		} 	
	}
	else if(stic==2&&!PIStus.IN_OPEN&&!PIStus.IN_SHUT&&!PIStus.IN_ESD)
	{
				  //闭到位
		if(GetMotoPos() >_LessLdD) 
		{
			Moto_REV_Drv;//反转
		}
		else
		{
			Moto_PARK_Drv;//停止	
		}
	}
*/
//*******************************************远程ESD按键			
}
/*********************************************************
工作主界面

*********************************************************/
void displayMain(void)
{
#if (SOFTLOCK == 1)
    static u8 lockflag;
#endif
    
    if(DispStep != 2)                                   //在菜单下，编码器读取速度过快
        delay_ms(1);
    switch(DispStep)
	{
		case	DispWorkStusWait:
				Moto_PARK_Drv;                          //进入菜单时，电机停止
				if((KeyHoldStus==0) && (KeyHoldStusA==0))
				{
					ClsKlong;
					ClsKDn;
					DispStep=DispWorkStus;
                    FreshTimes = 0;
				}
				break;
		case	DispWorkStus:
				//进入阀位(工作)显示
                dispMain();								//工作主界面显示
				if((Klong)&&(KDn==K1))//长按返回键
				{
                    if(RmLcDisSel == 1)
                    {
//                        RelayRunFlag = 0;
                        Moto_PARK_Drv;
                        clear_screen();
                        DispSetSel=1;
                        if(language)		                //判断当前显示语言
                            dispEnglishSetMenu(DispSetSel);	//显示英文菜单
                        else
                            dispSetMenu(DispSetSel);		//显示中文菜单
                        if(language)
                            dispEnglishSetMenu(DispSetSel);	//显示英文菜单
                        else
                            displaySetupMenu();				//显示中文菜单
                        DispStep=DispEnioySetWait;
                    }
                    
#if (SOFTLOCK == 1)
                    if(RmLcDisSel == 0 && lockflag == 0)//远程下长按SET键
                    {
                        //display_char16x32(7*16-2,7*5+1*8,Zhi3_16x32,8 & DispSetSel);
                        lockflag = 1;
                        if(KeyHoldStusA)
                        {
                            lock = 1;//上锁
                        }
                        if(KeyHoldStus)
                        {
                            lock = 0;
                        }
                        DIS_INT;
                        STMFLASH_Write(FLASH_SAVE_ADDR + 78,&lock,1);			//远程0/本地(本地键盘操作)1/禁止>=2切换
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
					Moto_PARK_Drv;	                    //停止
				}
                
#if (SOFTLOCK == 1)                 
                if(lock == 0)
#endif
                {
                    if(K2Up)								//K2(enter)?
                    {
                        ClsKDn;
                        Moto_PARK_Drv;	                    //停止
                        RmLcDisSel = (RmLcDisSel ^ 0x01);
                        DIS_INT;
                        STMFLASH_Write(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);			//远程0/本地(本地键盘操作)1/禁止>=2切换
                        EN_INT;                    
                    }
                    else if((Klong)&&(KDn==K2))					//K2(enter)长按?
                    {
                        ClsKlong;
                        ClsKDn;
                        RmTCnt=0;
                        KTCnt=0;
                        RmLcDisSel = (RmLcDisSel ^ 0x02);
                        DIS_INT;
                        STMFLASH_Write(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);			//远程0/本地(本地键盘操作)1/禁止>=2切换
                        EN_INT;
                    }
                }
                
//                if(PIStus.IN_RCL==1)		        //远程开关量（不具备掉电保存）
//                {
//                    A_DInSel = 1;
//                    A_DInSelflag = 1;
//                }
//                else                               //远程模拟量
//                {
//                    A_DInSel = 0;
//                    A_DInSelflag = 0;
//                }
                    
                if(RmLcDisSel == 1)						//本地 
				{
					 Localdisplay();		            //本地
				}
				else if(RmLcDisSel == 0)				//远程 这里可以做报警限制
				{                    
                    if(A_DInSel)
					{
#if (MB_MODE == 0)
                        if(Rs485Way && MbStatus.MbOnline)//启动了总线控制
                        {
                            if(WMbData.sMbData.MbSwitch == 1)
                            {
                                if((GetMotoPos() < _MoreLdU))
                                {
                                    Moto_FWD_Drv;	//正转
                                }
                            }
                            else if(WMbData.sMbData.MbSwitch == 2)
                            {
                                if((GetMotoPos() > _LessLdD))
                                {
                                    Moto_REV_Drv;   //反转
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
                            RemoteQquantitydisplay();		//远程开关运行函数
                        }
					}
					else
					{
						RemoteAnalogdisplay();          //远程模拟运行函数	
					}
				}												
				else									//禁止
				{
					Moto_PARK_Drv;
                }
                
                
                if(PIStus.IN_ESD)
                {
                    RelayByte.Byte.RelayESD = 1;
                    if(ESDset == 2)
                    {
                        //开到位
                        if(GetMotoPos() <_MoreLdU) 
                        {
                            Moto_FWD_Drv;//正转
                        }
                        else
                        {
                            Moto_PARK_Drv;//停止
                        } 	
                    }
                    else if(ESDset == 3)
                    {
                    //闭到位
                        if(GetMotoPos() >_LessLdD) 
                        {
                            Moto_REV_Drv;//反转
                        }
                        else
                        {
                            Moto_PARK_Drv;//停止	
                        }
                    }
                    if(ESDset==1)
                    {
                        Moto_PARK_Drv;		//停止	
                    }
                }
                else
                {
                    RelayByte.Byte.RelayESD = 0;
                }
                
                //4-20mA输出
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
设置主界面

1:基本设置,2:反馈设置,3:高级设置,4:故障诊断.....

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
                if(language)		//判断当前显示语言
                    dispEnglishSetMenu(DispSetSel);	//显示英文菜单
                else
                    dispSetMenu(DispSetSel);				//显示中文菜单
            }
            if(K4Dn)									//-
            {
                ClsKDn;
                DispSetSel=DispSetSel>>1;
                if(DispSetSel==0x0000)
                    {
                        DispSetSel=0x0008;
                    }
                if(language)		//判断当前显示语言
                    dispEnglishSetMenu(DispSetSel);	//显示英文菜单
                else
                    dispSetMenu(DispSetSel);				//显示中文菜单					//
            }
            if(((Klong) && (KDn==K1)) || (K1Dn))			//是否返回上级菜单
            {
                ClsKDn;
                ClsKlong;
                DispStep=DispWorkStusWait; 
                clear_screen();
                DispSetSel=0;
                FreshTimes = 0;
                dispMain();
                Moto_PARK_Drv;
                //保存到FLash
                SaveDataToFlash();
                ModBusDataInit();               //初始化WMbData值
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
设置1界面

1:自动标定,2:开位确认,3:关位确认,4:控制精度,5:现场控制,6:远程设置,7:丢信动作,8:显示方向,9语言选择

*********************************************************/
void dispSetMenu1_1(void)
{
//	PosAccuracy_flag=PosAccuracy;	//初始化精度
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
				disp_EnglishSet1_1_x(DispSetSel);	//显示英文菜单
			else
				disp_Set1_1_x(DispSetSel);		//显示中文菜单
            
			//基本设置操作
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
                EnSelFlsh=!EnSelFlsh;					//使能反白显示闪烁//使能被选项闪烁
                Moto_PARK_Drv;							//释放电机
                BaseSet1_STEP =0;
                if(DispSetSel <= 0x004)
                {
                    ClosewayTemp = Closeway | 0x80;         //保存原有的关方式
                    Closeway = 1;                           //设定行程时改为力矩关，无电子限位
#if (POWER_MODE == 1)                    
                    PhaseDir = PIStus.PHASE;                //刷新相序
#endif
                }
                if(DispSetSel == 0x001)
                {
                    isZeroTemp = 127;
                    ZeroStatus = 0;
                }
            }
            
//			if(((Klong) && (KDn==K1)) || (KDn==K1))		//是否返回上级菜单DispBaseSet1x
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//是否返回上级菜单,或者取消黑白闪烁
            {
                ClsKlong;
                ClsKDn;
                Moto_PARK_Drv;							//释放电机
                
                if(ClosewayTemp & 0x80)
                {
                    BaseSet1_STEP =0;//zhaoxw
                    Closeway = ClosewayTemp & 1;         //保存原有的关方式
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
                        disp_EnglishSet1_1_x(DispSetSel);	//显示英文菜单
                    else
                        disp_Set1_1_x(DispSetSel);		//显示中文菜单
                }
                else
                {
                    clear_screen();
                    DispSetSel=0x01;
                    if(language)							//判断当前显示语言
                        dispEnglishSetMenu(DispSetSel);	//显示英文菜单
                    else
                        dispSetMenu(DispSetSel);			//显示中文菜单			
                    EnSelFlsh=0;
                    DispStep=DispEnioySetWait;
                }
            }
			break;
		}

}

/*********************************************************
设置2界面
(2:反馈设置)
1:阀控电流低端微调,2:阀控电流高端微调,3:位反电流低端微调,4:位反电流高端微调,5:触点输出

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
			//反馈设置操作
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
                    DispStep = DispSet2_6_Wait;			    //进入输出通道设置
                    EnSelFlsh=0;
                }
                else
                {
                    EnSelFlsh=!EnSelFlsh;					//使能反白显示闪烁//使能被选项闪烁
                    
                    if((EnSelFlsh)&&(DispSetSel==0x10))		//高信
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
                    if((EnSelFlsh)&&(DispSetSel==0x08)) 	//低信
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
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//是否返回上级菜单,或者取消黑白闪烁
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
					if(language)		//判断当前显示语言
						dispEnglishSetMenu(DispSetSel);	//显示英文菜单
					else
						dispSetMenu(DispSetSel);			//显示中文菜单		
					EnSelFlsh=0;
					DispStep=DispEnioySetWait;
				}
			}
		break;
	}
}

/*********************************************************
设置1.3界面
(3:高级设置)
3:高级设置==>1:ESD设置,2: 关闭方式,3: 关闭方向,4: 两线控制,5: 堵转时间,6, 电子刹车,7: 间断运行,8: 密码设置,9: 恢复出厂
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
		case	DispSet1_3Wait_PassWord:										//检验密码
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

			if(K2Dn||(Klong && (KDn==2)))									//长按 enter?确认
			{
				ClsKDn;
				if(Klong)
				{
					if(BCDtoDec(BCD_buffer) == PassWord)						//验密码
					{
						DispStep=DispSet1_3;
						DispSetSel=1;
						clear_screen();
						disp_Set1_3_x(DispSetSel);
					}
					else
					{	//错误提示
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
//			if(((Klong) && (KDn==K1)) || (KDn==K1)) 	//是否返回上级菜单
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//是否返回上级菜单,或者取消黑白闪烁
			{
				ClsKlong;
				ClsKDn;
				clear_screen();
				DispSetSel=4;
				if(language)		//判断当前显示语言
					dispEnglishSetMenu(DispSetSel);	//显示英文菜单
				else
					dispSetMenu(DispSetSel);			//显示中文菜单		
				EnSelFlsh=0;
				DispStep=DispEnioySetWait;
			}
			break;
		case	DispSet1_3:
			disp_Set1_3_x(DispSetSel);						//高级菜单处理			
			//高级设置操作
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
				EnSelFlsh=!EnSelFlsh;					//使能反白显示闪烁//使能被选项闪烁
				clear_screen();
			}
			if(((Klong) && (KDn==K1)) || (K1Dn)) 	//是否返回上级菜单,或者取消黑白闪烁
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
					if(language)		//判断当前显示语言
						dispEnglishSetMenu(DispSetSel);	//显示英文菜单
					else
						dispSetMenu(DispSetSel);			//显示中文菜单		
					EnSelFlsh=0;
					DispStep=DispEnioySetWait;
				}
			}
		break;
	}
}

/*********************************************************
设置1.4界面
(4:故障诊断)
4:故障诊断==>1: 方式按键,2: 操作按键,3: 远程打开,4: 远程关闭,5: 远程保持,6: 远程自动,//7: 恢复密码,//8: 软件版本

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
					//故障诊断设置操作
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
					EnSelFlsh=!EnSelFlsh;					//使能反白显示闪烁//使能被选项闪烁
					clear_screen();
				}
				if(((Klong) && (KDn==K1)) || (K1Dn)) 		//是否返回上级菜单,或者取消黑白闪烁
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
						if(language)		//判断当前显示语言
							dispEnglishSetMenu(DispSetSel);	//显示英文菜单
						else
							dispSetMenu(DispSetSel);			//显示中文菜单	
						
						EnSelFlsh=0;
						DispStep=DispEnioySetWait;
					}
				}
				break;
		}
}

/*******************************************************

输出通道设定

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
				TmpSetVal = RelayType[0];							//装入第一个参数
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
			if(language)						//输出通道设定菜单显示
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
							if(DispSetSel & m) TmpSetVal = RelayType[n]; //找出被选择的参数-->TmpSetVal
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
							if(DispSetSel & m) TmpSetVal = RelayType[n]; //找出被选择的参数-->TmpSetVal
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
						if(DispSetSel & m) RelayType[n] = TmpSetVal; //TmpSetVal-->被选择的参数
						m = m<<1;
				}
				EnSelFlsh=!EnSelFlsh;					//使能反白显示闪烁//使能被选项闪烁
			}
			if(((Klong) && (KDn==K1)) || (K1Dn)) 		//是否返回上级菜单
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
function name: 工作主界面显示

fun	:功能选择
*********************************************************/
void dispMain(void)
{	
	u32 i;
	u32 l;	
	u16 PK,PK1,PK2,NewX;
    u8 ErrView;
    static u8 clr;

    PK=PIStus.FK_IN_Pers;
    //报警信息显示
	if(POStus.AL)									//故障?
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
            else //在0~100%以内 不属于过矩关，属于报警
            {
                if(PIStus.OTS || PIStus.CTS)				
                {
                    StusView=_QuanKai;
                    LED2 = 0; 
                    LED3 = 1;
                }
            }
        }
        else if((PIStus.FK_IN_Pers & 0x8000)||(PIStus.FK_IN_Pers & 0x7fff) < PosAccuracy)					//反馈为负值
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
        else if(((PIStus.FK_IN_Pers & 0x7fff) <= (5000  + PosAccuracy)) && ((PIStus.FK_IN_Pers & 0x7fff)>= (5000 - PosAccuracy))) //是否中间位
        {
            StusView=_ZhongWei;
        }
        else if((0 == A_DInSel) && (GetMotoPos() == _InTarget)) 	//是否到位
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
                LockedRotor();	//堵转判断
            }
            else if(Moto_STOP_Chk)								//刹车
            {
                StusView=_TingZhi;
            }
            else if(Moto_PARK_Chk)								//停止
            {
                StusView=_TingZhi;
            }
        }   
	}
       
    //左上角显示状态信息				
    display_char16x32(22,12,InfoDispStrSet[StusView][0],0);
    display_char16x32(22,17,InfoDispStrSet[StusView][1],0);
    
	if(FreshTimes-- == 0)
    {                                
        FreshTimes = 15;
        
        //阀位百分值显示
        display_Num_32x32_LZND(57,0,PK& 0x7fff,4 &DispSetSel);

        if(PIStus.FK_IN_Pers & 0x8000)
        {
            //负值，显示"-"
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
            //负值，阀开度进度条显示一个单位
    //		for(l = 0; l < 8; l ++)
    //		{
    //			set_lcd_address(97 + l,3);
    //			display_3pixels(0xffff);
    //		}
            PIStus.FK_IN_Pers = 0;
        }
        else
        {
            display_char16x32(30,5,ChrSP_16x32,4 & DispSetSel);//去掉负号
        }
        
        //正值，阀开度显示8象素宽度进度条
        for(l = 0; l < 8; l ++)
        {
            set_lcd_address(100 + l,3);
            for(i = 3; i < 50; i ++)	//一行分54个点，每个点占三个象素(即开度最多分54段显示)
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
        

        //给定值显示3象素宽度进度条
        for(l = 0; l < 3; l ++)
        {
            set_lcd_address(110 + l,3);
            for(i = 3; i < 50; i ++)	//一行分54个点，每个点占三个象素(即开度最多分54段显示)
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
    
    //显示阀门边框
    for(l = 0; l < 1; l ++)
    {
        set_lcd_address(97 + l,3);
        for(i = 3; i < 50; i ++)			//一行分54个点，每个点占三个象素(即开度最多分54段显示)
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
        for(i = 3; i < 50; i ++)	//一行分54个点，每个点占三个象素(即开度最多分54段显示)
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
    //显示阀门边框左右两边,及
    for(l = 0; l < 18; l ++)
    {
        set_lcd_address(97 + l,3);
        display_3pixels(0xFFFF);
        set_lcd_address(97 + l,50);
        display_3pixels(0xFFFF);
    }
    //显示进度阀中间的虚线
    for(l = 0; l < 17; l ++)
    {
        set_lcd_address(97 + l,26);
        if(l & 0x1)
            display_3pixels(0x5B6D);
        else
            if(l)display_3pixels(0x0000);	
        
    }
    
	//左下角显示给定值%
//	hextobcd((PIStus.VMA_IN*1000)/186-30);		//直接 显示4-20MA输入信号	
//	hextobcd((PIStus.VMA_IN/183)*1000);		//直接 显示4-20MA输入信号	
/*	hextobcd(PIStus.VMA_IN_Pers);			 	 //以百分比的方式 显示4-20MA输入信号
	if((BCD_buffer[4]==0) )
	{
		display_char16x32(7*16,0*5+_coloffset-3,Sp32x32,2 & DispSetSel);
		dispayNum16x32(7*16,1*5+_coloffset-3,BCD_buffer[3],2 & DispSetSel);
		display_char16x32(7*16,2*5+_coloffset-3,ChrPoin_16x32,2 & DispSetSel);
		dispayNum16x32(7*16,3*5+_coloffset-6,BCD_buffer[2],2 & DispSetSel);		//十分位
//		dispayNum16x32(7*16,4*5+_coloffset-6,BCD_buffer[1],2 & DispSetSel);		//精确到百分位
	}
	else
	{
		dispayNum16x32(7*16,0*5+_coloffset-3,BCD_buffer[4],2 & DispSetSel);
		dispayNum16x32(7*16,1*5+_coloffset-3,BCD_buffer[3],2 & DispSetSel);
		display_char16x32(7*16,2*5+_coloffset-3,ChrPoin_16x32,2 & DispSetSel);
		dispayNum16x32(7*16,3*5+_coloffset-6,BCD_buffer[2],2 & DispSetSel);		//十分位
//		dispayNum16x32(7*16,4*5+_coloffset-6,BCD_buffer[1],2 & DispSetSel);		//精确到百分位
	}		*/
#if (DISTEMP == 1)
    //右上角显示温度
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

/********************************反馈显示**************************************/    
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
	
    //右下角
    if(NewX == 13)//报警
    {
        if(clr != 1)
        {
            clr = 1;
            display_char16x32(120,21,Sp_16x32,8 & DispSetSel);//清楚之前信息
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
    
//    if(clr == 1)//切换时才清除相应位置内容，防止闪烁
//    {
//        clr = 0;
//        display_char16x32(120,13,Sp_16x32,8 & DispSetSel);
//		display_char16x32(120,18,Sp_16x32,8 & DispSetSel);
//    }
	if(RmLcDisSel==0)		//远程
	{
		display_char16x32(120,NewX,Yuan_16x32,8 & DispSetSel);
		display_char16x32(120,NewX + 5,Cheng_16x32,8 & DispSetSel);
	}
	else if(RmLcDisSel==1)	//本地
	{
		display_char16x32(120,NewX,Ben_16x32,8 & DispSetSel);
		display_char16x32(120,NewX + 5,Di_16x32,8 & DispSetSel);
	}
	else					//禁止
	{
		display_char16x32(120,NewX,Jin_16x32,8 & DispSetSel);
		display_char16x32(120,NewX + 5,Zhi3_16x32,8 & DispSetSel);
	}
    
#if (POWER_MODE == 1 && POWER_MODE_DISP == 1)//测试显示内容
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
    if(lock)//上锁
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
	//外圆显示
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
function name: 显示中文主菜单
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
function name: 显示英文主菜单
fun	:		显示英文主菜单
*********************************************************/
void dispEnglishSetMenu(u16 Sel)
{

	u16 sl;
//	clear_screen();
	if(Fsh500mS && EnSelFlsh)														//是否使能反白显示闪烁//是否使能被选项闪烁
    {
        sl=0;				
    }
	else
	{	
        sl=Sel;
    }

    // 显示1.Basic setup
    display_char8x16(2*15+2,9,"1.BasicSet",0x01 & sl);// 

    // 显示2.Feedback set
    display_char8x16(3*15+4,9,"2.FeedbSet",0x02 & sl);// 

    // 显示3.advanced	setup
    display_char8x16(4*15+6,9,"3.AdvancSet",0x04 & sl);// 

    // 显示4.Fault diagnosis		FDD-- 故障检测诊断
    display_char8x16(5*15+8,9,"4.FDD",0x08 & sl);// 
	
}


/*********************************************************
function name: 14x14清除一行
fun	:		用于14x14大小字体的整行清除		x :要清除哪一行
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
function name:16x16清除一行
fun	:		用于16x16大小字体的整行清除		x :要清除哪一行
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
function name: 中文基本设置界面

fun	:1:自动标定,2:开位确认,3:关位确认,4:控制精度,5:现场控制,6:远程设置,7:丢信动作,8:显示方向,9语言选择
*********************************************************/
void disp_Set1_1_x(u16 Sel)
{
    u16 Temp;
    
    if(Fsh500mS && EnSelFlsh)						//是否使能反白显示闪烁//是否使能被选项闪烁
	{
		Sel=0;				//
	}
		//画面卷轴处理
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
            
            // 7现场控制
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
            
			// 8丢信动作
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
			
			// 9显示方向	
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
			
			// 10语言选择			
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
			// 1自动标定
			//ClearLine_14x14(jump+line*1);
			display_char14x14(jump+line*1,0*5+_coloffset-3,Zi_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset-3,Dong_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,2*5+_coloffset-3,Biao_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,3*5+_coloffset-3,Ding_14x14,0x01 & Sel);	
            if((DispSetSel==0x01) && (EnSelFlsh))
			{
				dispay4Num8x14(jump+line*1,5*5+_coloffset-3,Temp,0x02 & Sel);	//调节值
			}
			else
			{
                display_char8x14(jump+line*1,5*5+_coloffset-3+0*3,10,0);//
                display_char8x14(jump+line*1,5*5+_coloffset-3+1*3,10,0);//
                display_char8x14(jump+line*1,5*5+_coloffset-3+2*3,10,0);//
                display_char8x14(jump+line*1,5*5+_coloffset-3+3*3,10,0);//
			}

			// 2开位确认
			display_char14x14(jump+line*2+2,0*5+_coloffset-3,Kai_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset-3,Wei_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,2*5+_coloffset-3,Que_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,3*5+_coloffset-3,Ren_14x14,0x02 & Sel);	
			if((DispSetSel==0x02) && (EnSelFlsh))
			{
				dispay4Num8x14(jump+line*2+2,5*5+_coloffset-3,Temp,0x02 & Sel);	//调节值
			}
			else
			{
				dispay4Num8x14(jump+line*2+2,5*5+_coloffset-3,PosLmdU,0x02 & Sel);		//开位最值
			}
			// 3关位确认
			display_char14x14(jump+line*3+4,0*5+_coloffset-3,Guan_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset-3,Wei_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,2*5+_coloffset-3,Que_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,3*5+_coloffset-3,Ren_14x14,0x04 & Sel);
			if((DispSetSel==0x04) && (EnSelFlsh))
			{
				dispay4Num8x14(jump+line*3+4,5*5+_coloffset-3,Temp,0x04 & Sel);			//调节值
			}
			else
			{
				
				dispay4Num8x14(jump+line*3+4,5*5+_coloffset-3,PosLmdD,0x04 & Sel);			//闭位值
			}
           
			// 4关闭方式
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

			// 5关闭方向
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
			
			// 6远程控制
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
function name: 英文基本设置界面

fun	:1:自动标定,2:开位确认,3:关位确认,4:控制精度,5:现场控制,6:远程设置,7:丢信动作,8:显示方向,9语言选择
*********************************************************/
void disp_EnglishSet1_1_x(u16 Sel)
{
    if(Fsh500mS && EnSelFlsh)						//是否使能反白显示闪烁//是否使能被选项闪烁
	{
		Sel=0;				
	}
	//画面卷轴处理

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
			// 1自动标定
			ClearLine_16x16(0*15+2);
			display_char8x16(0*15+2,9,"ATG",0x01 & Sel);
			// 2开位确认
			display_char8x16(1*15+3,9,"OLCF",0x02 & Sel);
			if((DispSetSel==0x02) && (EnSelFlsh))
			{
					display_4Num_8x16(1*15+3,27,AvVal[_FK],0x02 & Sel);
			}
			else
			{
					display_4Num_8x16(1*15+3,27,PosLmdU,0x02 & Sel);
			}
			
			// 3关位确认
			display_char8x16(2*15+4,9,"CLCF",0x04 & Sel);
			if((DispSetSel==0x04) && (EnSelFlsh))
			{
				display_4Num_8x16(2*15+4,27,AvVal[_FK],0x04 & Sel);
			}
			else
			{
				display_4Num_8x16(2*15+4,27,PosLmdD,0x04 & Sel);
			}
			
			// 4控制精度
			display_char8x16(3*15+5,9,"NPC",0x08 & Sel);
			display_Num_8x16(3*15+5,27,PosAccuracy/10,0x08 & Sel);
			
			// 5现场控制
			display_char8x16(4*15+6,9,"PRSC",0x10 & Sel);
			if(JogHolAutoSel==0)
			{
				display_char8x16(4*15+6,27,"jog",0x10 & Sel);	//点动
			}
			else if(JogHolAutoSel==1)
			{
				display_char8x16(4*15+6,27,"KBO",0x10 & Sel);	//保持
			}
			else
			{
				display_char8x16(4*15+6,27,"ACD",0x10 & Sel);	//自动
			}
			
			// 6远程控制
			display_char8x16(5*15+7,9,"LRM",0x20 & Sel);
			if(A_DInSelflag)
			{
				display_char8x16(5*15+7,27,"ADR",0x20 & Sel);	//模拟
			}
			else
			{
				display_char8x16(5*15+7,27,"SRS",0x20 & Sel);	//开关
			}
			
			// 7丢信动作
			display_char8x16(6*15+8,9,"CLP",0x40 & Sel);
			if(NoSignalCtrSel==0)
			{
				display_char8x16(6*15+8,27,"DSP",0x40 & Sel);	//停止
			}
			else if(NoSignalCtrSel==1)
			{
				display_char8x16(6*15+8,27,"FOT",0x40 & Sel);	//全开
			}
			else
			{
				display_char8x16(6*15+8,27,"FCT",0x40 & Sel);	//全闭
			}
			
			// 8显示方向
			display_char8x16(7*15+9,9,"DDS",0x80 & Sel);
			
			//清除上次多余的显示
//				display_char16x16(128,6*5+_coloffset-3,Sp32x32,0x00 & Sel);
			if(ScrSelflag==0)
				{
				display_char8x16(7*15+9,27,"R.S.+",0x80 & Sel);	//正
				}
			else
				{
				display_char8x16(7*15+9,27,"B.S.+",0x80 & Sel);	//反
				}
			
				// 9语言选择
			display_char8x16(8*15+10,9,"OSL",0x100 & Sel);	
			//清除上次的显示痕迹
			display_char8x16(8*15+10,34-6,"   ",0x00 & Sel);
			if(languageflag)
				{
					display_char8x16(8*15+10,27,"en",0x100 & Sel);	//中文
				}
			else
				{
					display_char8x16(8*15+10,27,"cn",0x100 & Sel);	//中文
				}	
			//清除最后一行
			display_char8x16(9*15+10,4,"                   ",0x00 & Sel);
		}
		else
		{
			//清除第0行
			display_char8x16(0*15+2,4,"                    ",0x00 & Sel);
			// 1自动标定
			ClearLine_16x16(1*15+2);
			display_char8x16(1*15+2,9,"ATG",0x01 & Sel);
			
			// 2开位确认
			display_char8x16(2*15+3,9,"OLCF",0x02 & Sel);
			if((DispSetSel==0x02) && (EnSelFlsh))
			{
				display_4Num_8x16(2*15+3,27,AvVal[_FK],0x02 & Sel);
			}
			else
			{
				display_4Num_8x16(2*15+3,27,PosLmdU,0x02 & Sel);
			}
		
			// 3关位确认
			display_char8x16(3*15+4,9,"CLCF",0x04 & Sel);
			if((DispSetSel==0x04) && (EnSelFlsh))
			{
				display_4Num_8x16(3*15+4,27,AvVal[_FK],0x04 & Sel);
			}
			else
			{
				display_4Num_8x16(3*15+4,27,PosLmdD,0x04 & Sel);
			}
				
			// 4控制精度
			display_char8x16(4*15+5,9,"NPC",0x08 & Sel);
			display_Num_8x16(4*15+5,27,PosAccuracy/10,0x08 & Sel);
				
			// 5现场控制
			display_char8x16(5*15+6,9,"PRSC",0x10 & Sel);
			if(JogHolAutoSel==0)
			{
				display_char8x16(5*15+6,27,"jog",0x10 & Sel);	//点动
			}
			else if(JogHolAutoSel==1)
			{
				display_char8x16(5*15+6,27,"KBO",0x10 & Sel);	//保持
			}
			else
			{
				display_char8x16(5*15+6,27,"ACD",0x10 & Sel);	//自动
			}
				
			// 6远程控制模式
			display_char8x16(6*15+7,9,"LRM",0x20 & Sel);
			if(A_DInSelflag)
			{
				display_char8x16(6*15+7,27,"ADR",0x20 & Sel);	//模拟
			}
			else
			{
				display_char8x16(6*15+7,27,"SRS",0x20 & Sel);	//开关
			}
			
			// 7丢信动作
			display_char8x16(7*15+8,9,"CLP",0x40 & Sel);
			if(NoSignalCtrSel==0)
			{
				display_char8x16(7*15+8,27,"DSP",0x40 & Sel);	//停止
			}
			else if(NoSignalCtrSel==1)
			{
                display_char8x16(7*15+8,27,"FOT",0x40 & Sel);	//全开
			}
			else
			{
				display_char8x16(7*15+8,27,"FCT",0x40 & Sel);	//全闭
			}
			
			// 8显示方向
			display_char8x16(8*15+9,9,"DDS",0x80 & Sel);
			
			//清除上次多余的显示
//				display_char16x16(128,6*5+_coloffset-3,Sp32x32,0x00 & Sel);//
			if(ScrSelflag==0)
			{
				display_char8x16(8*15+9,27,"R.S.+",0x80 & Sel);	//正
			}
			else
			{
                display_char8x16(8*15+9,27,"B.S.+",0x80 & Sel);	//反
			}
			
				// 9语言选择
			display_char8x16(9*15+10,9,"OSL",0x100 & Sel);	
			if(languageflag)
			{
				display_char8x16(9*15+10,27,"en",0x100 & Sel);	//英文
			}
			else
			{
				display_char8x16(9*15+10,27,"cn",0x100 & Sel);	//中文
			}	
		}
	}
}

/*********************************************************
反馈设置界面

Sel		:选择反白显示项目,1:阀控电流低端微调,2:阀控电流高端微调,3:位反电流低端微调,4:位反电流高端微调,5:触点输出,

*********************************************************/
void disp_Set1_2_x(u16 Sel)
{
    if(Fsh500mS && EnSelFlsh)														//是否使能反白显示闪烁//是否使能被选项闪烁
	{
		Sel=0;				//
	}
	display_char8x9(jump+line*3+16,3,XZYE_8x9,0);
	// 1 信号(类型)
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
	// 2 //给定低信4mA/给定低信0V
	display_char14x14(jump+line*2+2,0*5+_coloffset-6,Gei_14x14,0x02 & Sel);
	display_char14x14(jump+line*2+2,1*5+_coloffset-6,Ding_14x14,0x02 & Sel);	
	display_char14x14(jump+line*2+2,2*5+_coloffset-6,Di1_14x14,0x02 & Sel);	
	display_char14x14(jump+line*2+2,3*5+_coloffset-6,Xin_14x14,0x02 & Sel);	
/*
	if((EnSelFlsh) && (DispSetSel==0x02))
		{
		if(OutCVSel)
			{
			display_Num_8x16(48,4*5+_coloffset, PIStus.VMA_IN * 12000/AvVal[_IV]/150, 0x02 & Sel);			//补偿后的电压测量值(V)=PIStus.VMA_IN * 1200/AvVal[_IV]。电流测量值(mA)=电压测量值*1000/150欧 
			}
		else
			{
			display_Num_8x16(48,4*5+_coloffset, PIStus.VMA_IN * 1.2 * (51 + 22) * 10/AvVal[_IV]/22, 0x02 & Sel);	//补偿后的电压测量值(V)(AD口处)=PIStus.VMA_IN * 1200/AvVal[_IV]。补偿后的电压测量值(V)(输入口处)=PIStus.VMA_IN * 1.2 * (51 + 22)/AvVal[_IV]/22。
			}																											//51,22为分压电阻值。*10是想得到一位小数
		}
	else
		{
		if(OutCVSel)
			{
			display_Num_8x16(48,4*5+_coloffset, CInLmdD * 12000/AvVal[_IV]/150, 0x02 & Sel);				//4mA设定值
			}
		else
			{
			display_Num_8x16(48,4*5+_coloffset, VInLmdD * 12000/AvVal[_IV]/150, 0x02 & Sel);					//0V设定值
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
			display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, 40, 0x02 & Sel);							//4mA输出对应的PWM占空比
		}
		else
		{
			display_Num_8x16(jump+line*2+1,4*5+_coloffset-3, 0, 0x02 & Sel);								//0V输出对应的PWM占空比
		}																										
	}


	// 3 //给定高信20mA/给定高信10V
	display_char14x14(jump+line*3+4,0*5+_coloffset-6,Gei_14x14,0x04 & Sel);
	display_char14x14(jump+line*3+4,1*5+_coloffset-6,Ding_14x14,0x04 & Sel);	
	display_char14x14(jump+line*3+4,2*5+_coloffset-6,Gao_14x14,0x04 & Sel);	
	display_char14x14(jump+line*3+4,3*5+_coloffset-6,Xin_14x14,0x04 & Sel);	
/*
	if((EnSelFlsh) && (DispSetSel==0x04))
		{
		if(OutCVSel)
			{
			display_Num_8x16(64,4*5+_coloffset, PIStus.VMA_IN * 12000/AvVal[_IV]/150, 0x04 & Sel);						//补偿后的电压测量值(V)=PIStus.VMA_IN * 1200/AvVal[_IV]。电流测量值(mA)=电压测量值*1000/150欧 
			}
		else
			{
			display_Num_8x16(64,4*5+_coloffset, PIStus.VMA_IN * 1.2 * (51 + 22) * 10/AvVal[_IV]/22, 0x04 & Sel);		//补偿后的电压测量值(V)(AD口处)=PIStus.VMA_IN * 1200/AvVal[_IV]。补偿后的电压测量值(V)(输入口处)=PIStus.VMA_IN * 1.2 * (51 + 22)/AvVal[_IV]/22。
			}																											//51,22为分压电阻值。*10是想得到一位小数
		}
	else
		{
		if(OutCVSel)
			{
			display_Num_8x16(64,4*5+_coloffset, CInLmdU * 12000/AvVal[_IV]/150, 0x04 & Sel);							//20mA设定值
			}
		else
			{
			display_Num_8x16(64,4*5+_coloffset, VInLmdU * 12000/AvVal[_IV]/150, 0x04 & Sel);							//10V设定值
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
            display_Num_8x16(jump+line*3+3,4*5+_coloffset-3, 200, 0x04 & Sel);										//20mA输出对应的PWM占空比
		}
		else
		{
			display_Num_8x16(jump+line*3+3,4*5+_coloffset-3, 100, 0x04 & Sel);										//10V输出对应的PWM占空比
		}																											
	}
	
	// 4 输出低信4mA/给定低信0V
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
			display_Num_8x16(jump+line*4+5,4*5+_coloffset-3, 40, 0x08 & Sel);							//4mA输出对应的PWM占空比
		}
		else
		{
			display_Num_8x16(jump+line*4+5,4*5+_coloffset-3, 0, 0x08 & Sel);								//0V输出对应的PWM占空比
        }																											
    }

	
	// 5 输出高信20mA/10V
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
			display_Num_8x16(jump+line*5+7,4*5+_coloffset-3, 200, 0x10 & Sel);										//20mA输出对应的PWM占空比
		}
		else
		{
			display_Num_8x16(jump+line*5+7,4*5+_coloffset-3, 100, 0x10 & Sel);										//10V输出对应的PWM占空比
		}																											
	}
	// 6 触点输出
	display_char14x14(jump+line*6+10,0*5+_coloffset-6,Chu1_14x14,0x20 & Sel);
	display_char14x14(jump+line*6+10,1*5+_coloffset-6,Dian_14x14,0x20 & Sel);	
	display_char14x14(jump+line*6+10,2*5+_coloffset-6,Shu_14x14,0x20 & Sel);	
	display_char14x14(jump+line*6+10,3*5+_coloffset-6,Chu_14x14,0x20 & Sel);	
}

/*********************************************************
高级设置界面

显示=1:电子刹车,2:关闭方向,3: 关闭方式,4: ESD设置5:两线控制,6:堵转时间, ,7: 间断运行,8: 密码设置,9: 恢复出厂

*********************************************************/
void disp_Set1_3_x(u16 Sel)
{
#define	_coloffset_1_3_x	15
#define 		saze	14    

	if(Fsh500mS && EnSelFlsh)								//是否使能反白显示闪烁//是否使能被选项闪烁
	{
		Sel=0;				
	}
	//画面卷轴处理
    if((Sel==0x40)||(Sel==0x400))
	{
		RolStus=1;
	}
	if((Sel==0x001)||(Sel==0x020))
	{
		RolStus=2;
	}
    
	if((EnSelFlsh)&&(DispStep==DispSet1_3)&&((DispSetSel==0x200) ||(DispSetSel==0x400)))
	{//刷屏
	}
	else
	{	
		if(RolStus==1)
		{
			display_char8x9(jump+line*4+8,3,XZYE_8x9,0);
			display_char8x9(jump+line*3+6,3,WXYE_8x9,0);	
            
            // 7控制精度
			display_char14x14(jump+line*1,0*5+_coloffset-3,Kong_14x14,0x40 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset-3,Zhi1_14x14,0x40 & Sel);
			display_char14x14(jump+line*1,2*5+_coloffset-3,Jing_14x14,0x40 & Sel);
			display_char14x14(jump+line*1,3*5+_coloffset-3,Du_14x14,0x40 & Sel);		
			
			display_Num_8x16(jump+line*1-3,4*5+_coloffset,PosAccuracy_flag/10,0x40 & Sel);
            
//			// 7间断运行
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
			
            // 8中间位置1
			display_char14x14(jump+line*2,0*5+_coloffset-3,Zhong_14x14,0x80 & Sel);
			display_char14x14(jump+line*2,1*5+_coloffset-3,Jian2_14x14,0x80 & Sel);	
			display_char14x14(jump+line*2,2*5+_coloffset-3,Wei_14x14,0x80 & Sel);	
			display_char14x14(jump+line*2,3*5+_coloffset-3,Zhi2_14x14,0x80 & Sel);
            dispayNum8x14(jump+line*2,4*5+_coloffset-3,1,0x80 & Sel);
			{
				dispay3Num8x14(jump+line*2,4*5+_coloffset-1,Intermediate1flag,0x80 & Sel);	//调节值
                display_char8x14(jump+line*2,6*5+_coloffset,11,0x80 & Sel);
            }
            
            // 9中间位置2	
			display_char14x14(jump+line*3,0*5+_coloffset-3,Zhong_14x14,0x100 & Sel);
			display_char14x14(jump+line*3,1*5+_coloffset-3,Jian2_14x14,0x100 & Sel);	
			display_char14x14(jump+line*3,2*5+_coloffset-3,Wei_14x14,0x100 & Sel);	
			display_char14x14(jump+line*3,3*5+_coloffset-3,Zhi2_14x14,0x100 & Sel);
            dispayNum8x14(jump+line*3,4*5+_coloffset-3,2,0x100 & Sel);
			{
				dispay3Num8x14(jump+line*3,4*5+_coloffset-1,Intermediate2flag,0x100 & Sel);	//调节值
                display_char8x14(jump+line*3,6*5+_coloffset,11,0x100 & Sel);
            }
            
			// 10密码设置	
			display_char14x14(jump+line*4,0*5+_coloffset-3,	Mi_14x14,0x200 & Sel);
			display_char14x14(jump+line*4,1*5+_coloffset-3,Ma_14x14,0x200 & Sel);	
			display_char14x14(jump+line*4,2*5+_coloffset-3,She_14x14,0x200 & Sel);	
			display_char14x14(jump+line*4,3*5+_coloffset-3,Zhi2_14x14,0x200 & Sel);
			
			// 11恢复出厂
			display_char14x14(jump+line*5,0*5+_coloffset-3,	Hui_14x14,0x400 & Sel);
			display_char14x14(jump+line*5,1*5+_coloffset-3,Fu_14x14,0x400 & Sel);	
			display_char14x14(jump+line*5,2*5+_coloffset-3,Chu_14x14,0x400 & Sel);	
			display_char14x14(jump+line*5,3*5+_coloffset-3,Chang1_14x14,0x400 & Sel);	
		}
		else
		{
			display_char8x9(jump+line*3+6,3,XZYE_8x9,0);
			display_char8x9(jump+line*4+8,3,WXYE_8x9,0);
			// 1电子刹车
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
			// 2堵转时间
			//ClearLine_14x14(jump+line*2+2);
			display_char14x14(jump+line*2+2,0*5+_coloffset-3,Du1_14x14,0x02 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset-3,Zhuan_14x14,0x02 & Sel);	
			display_char14x14(jump+line*2+2,2*5+_coloffset-3,Shi2_14x14,0x02 & Sel);	
			display_char14x14(jump+line*2+2,3*5+_coloffset-3,Jian2_14x14,0x02 & Sel);

			dispay3Num8x14(jump+line*2+2,3*5+_coloffset+4, LockedRotorTime_flag,0x02 & Sel);

            // 3通讯控制
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
            
            // 4通讯地址
			display_char14x14(jump+line*4+6,0*5+_coloffset-3,Tong_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,1*5+_coloffset-3,Xun_14x14,0x08 & Sel);		
			display_char14x14(jump+line*4+6,2*5+_coloffset-3,Di_14x14,0x08 & Sel);	
			display_char14x14(jump+line*4+6,3*5+_coloffset-3,Zhi3_14x14,0x08 & Sel);

			dispay3Num8x14(jump+line*4+6,3*5+_coloffset+4, mbaddr_flag,0x08 & Sel);
            
			// 5	ESD设置	
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
			// 6两线控制
			display_char14x14(jump+line*6+10,0*5+_coloffset-3,Liang_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,1*5+_coloffset-3,Xian3_14x14,0x20 & Sel);	
			display_char14x14(jump+line*6+10,2*5+_coloffset-3,Kong_14x14,0x20 & Sel);	
			display_char14x14(jump+line*6+10,3*5+_coloffset-3,Zhi1_14x14,0x20 & Sel);	
			switch(TwoLineflag)					//两线控制？
			{
				case 1:	
					display_char14x14(jump+line*6+10,5*5+_coloffset-3,Jin_14x14,0x20 & Sel);	//禁止
					display_char14x14(jump+line*6+10,6*5+_coloffset-3,Zhi_14x14,0x20 & Sel);
				break;
				
				case 2:	
					display_char14x14(jump+line*6+10,5*5+_coloffset-3,Xin_14x14,0x20 & Sel);	
					display_char14x14(jump+line*6+10,6*5+_coloffset-3,Kai_14x14,0x20 & Sel);//信开
				break;
								
				case 3:	
					display_char14x14(jump+line*6+10,5*5+_coloffset-3,Xin_14x14,0x20 & Sel);	//禁止
					display_char14x14(jump+line*6+10,6*5+_coloffset-3,Guan_14x14,0x20 & Sel);//信关
				break;
			}
		}
	}	
}
/*********************************************************
function name: 故障诊断界面	14X14大小点阵

fun	:显示=1: 方式按键,2: 力矩检测,3: 远程打开,4: 远程关闭,5: 远程保持,6: 远程自动,7:恢复密码,8:软件版本
*********************************************************/
void disp_Set1_4_x(u16 Sel)
{
#define	_coloffset_1_4	13

	if(Fsh500mS && EnSelFlsh)									//是否使能反白显示闪烁//是否使能被选项闪烁
	{
		Sel=0;				
	}
	//画面卷轴处理 
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
			// 7恢复密码
			//ClearLine_14x14(jump+line*1);	
			//ClearLine_14x14(jump+line*2+2);	
			display_char14x14(jump+line*2+2,0*5+_coloffset_1_4-3,Hui_14x14,0x40 & Sel);
			display_char14x14(jump+line*2+2,1*5+_coloffset_1_4-3,Fu_14x14,0x40 & Sel);
			display_char14x14(jump+line*2+2,2*5+_coloffset_1_4-3,Mi_14x14,0x40 & Sel);
			display_char14x14(jump+line*2+2,3*5+_coloffset_1_4-3,Ma_14x14,0x40 & Sel);	
			
			//8软件版本号
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
			// 1方式按键
			display_char14x14(jump+line*1,0*5+_coloffset_1_4-3,Fang_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,1*5+_coloffset_1_4-3,Shi_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,2*5+_coloffset_1_4-3,An_14x14,0x01 & Sel);
			display_char14x14(jump+line*1,3*5+_coloffset_1_4-3,Jian_14x14,0x01 & Sel);				
			if(RmLcDisSel==1)					//本地
			{
				display_char14x14(jump+line*1,5*5+_coloffset_1_4-3,Ben_14x14,0);
				display_char14x14(jump+line*1,6*5+_coloffset_1_4-3,Di_14x14,0);
			}
			else if(!RmLcDisSel)					//远程
			{
				display_char14x14(jump+line*1,5*5+_coloffset_1_4-3,Yuan_14x14,0);
				display_char14x14(jump+line*1,6*5+_coloffset_1_4-3,Cheng_14x14,0);
			}
			else									//禁止
			{
				display_char14x14(jump+line*1,5*5+_coloffset_1_4-3,Jin_14x14,0);
				display_char14x14(jump+line*1,6*5+_coloffset_1_4-3,Zhi_14x14,0);
			}
			
			// 2 力矩检测
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
			// 3远程打开
			display_char14x14(jump+line*3+4,0*5+_coloffset_1_4-3,Yuan_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,1*5+_coloffset_1_4-3,Cheng_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,2*5+_coloffset_1_4-3,Da_14x14,0x04 & Sel);
			display_char14x14(jump+line*3+4,3*5+_coloffset_1_4-3,Kai_14x14,0x04 & Sel);	
			if(!P_IN_OPEN)		//有远程开信号
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset_1_4-3,You_14x14,0);
			}
			else
			{
				display_char14x14(jump+line*3+4,5*5+_coloffset_1_4-3,Wu_14x14,0);
			}
			// 4远程关闭
			display_char14x14(jump+line*4+6,0*5+_coloffset_1_4-3,Yuan_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,1*5+_coloffset_1_4-3,Cheng_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,2*5+_coloffset_1_4-3,Guan_14x14,0x08 & Sel);
			display_char14x14(jump+line*4+6,3*5+_coloffset_1_4-3,Bi_14x14,0x08 & Sel);	
			if(!P_IN_SHUT)	//有远程关信号
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset_1_4-3,You_14x14,0);
			}
			else
			{
				display_char14x14(jump+line*4+6,5*5+_coloffset_1_4-3,Wu_14x14,0);
			}
			// 5远程保持
			display_char14x14(jump+line*5+8,0*5+_coloffset_1_4-3,Yuan_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,1*5+_coloffset_1_4-3,Cheng_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,2*5+_coloffset_1_4-3,Bao_14x14,0x10 & Sel);
			display_char14x14(jump+line*5+8,3*5+_coloffset_1_4-3,Chi_14x14,0x10 & Sel);		
			if(PIStus.IN_BC)	//有远程保持信号
			{
				display_char14x14(jump+line*5+8,5*5+_coloffset_1_4-3,You_14x14,0);
			}
			else
			{
				display_char14x14(jump+line*5+8,5*5+_coloffset_1_4-3,Wu_14x14,0);
			}
			// 6远程模拟
			display_char14x14(jump+line*6+10,0*5+_coloffset_1_4-3,Yuan_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,1*5+_coloffset_1_4-3,Cheng_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,2*5+_coloffset_1_4-3,Mo_14x14,0x20 & Sel);
			display_char14x14(jump+line*6+10,3*5+_coloffset_1_4-3,Ni_14x14,0x20 & Sel);		
			
//          if((PIStus.VMA_IN > (CInLmdD/2)&&(PIStus.VMA_IN<0x0e78)))	//有远程保自动信号 (4-20AM模拟限号)
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
function name: 显示内部参数 displayInternalParameters
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
//function name: 十种信号输出通道设置界面

//fun	:显示=1:MRTS CH,2: MD CH,3:SHUTRUN CH,4: OPENRUN CH,5: RCL CH,6, CTSOUT CH,7: OTSOUT CH,
//8: ACLSOUT CH,9: AOLSOUT CH,10:AL CH CH = 1~10
//*********************************************************/
//void disp_Set1_6_x(u16 Sel)
//{
//#define	_coloffset_1_6	16
//#define 		saze	14

//    u16 sl;

//	if(Fsh500mS && EnSelFlsh)														//是否使能反白显示闪烁//是否使能被选项闪烁
//	{
//		sl=0;				
//	}
//	else
//		sl=Sel;
//	//画面卷轴处理
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
//		// 7开力矩
//        //触点7
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
//		// 8 关辅助
//        //触点8
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
//		// 9	开辅助
//        //触点9
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
//		// 10 报警
//        //触点10
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
//		// 1,过矩输出		
//        //触点1
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
//		// 2 中间位置
//        //触点2
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
//		// 3 关运行
//        //触点3
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
//		// 4 开运行
//        //触点4
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
//		// 5 远程/ 本地
//        //触点5
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
//		// 6 关力矩
//        //触点6
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
function name: 十种信号输出通道设置界面

fun	:显示=1:MRTS CH,2: MD CH,3:SHUTRUN CH,4: OPENRUN CH,5: RCL CH,6, CTSOUT CH,7: OTSOUT CH,
8: ACLSOUT CH,9: AOLSOUT CH,10:AL CH CH = 1~10
*********************************************************/
void disp_Set1_6_x(u16 Sel)
{
#define	_coloffset_1_6	16
#define 		saze	14

    u16 sl;

	if(Fsh500mS && EnSelFlsh)														//是否使能反白显示闪烁//是否使能被选项闪烁
	{
		sl=0;				
	}
	else
		sl=Sel;
	//画面卷轴处理
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
		
        //触点7
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
        
        //触点8
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
        
        //触点9
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
        
        //报警触点 固定，不参与组态
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
        
		//1,过矩输出		
        //触点1
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
        
		// 2 中间位置
        //触点2
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
        
		// 3 关运行
        //触点3
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
        
		// 4 开运行
        //触点4
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
        
		// 5 远程/ 本地
        //触点5
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
        
		// 6 关力矩
        //触点6
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
function name: 十种信号输出通道设置英文界面

fun	:显示=1:MRTS CH,2: MD CH,3:SHUTRUN CH,4: OPENRUN CH,5: RCL CH,6, CTSOUT CH,7: OTSOUT CH,
8: ACLSOUT CH,9: AOLSOUT CH,10:AL CH CH = 1~10
*********************************************************/
void disp_Set1_6_xEnglish(u16 Sel)
{
	u16 sl;
	if(Fsh500mS && EnSelFlsh)														//是否使能反白显示闪烁//是否使能被选项闪烁
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
	// 10//清除第10行
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
function name: 自动标定
fun :		
*********************************************************/
void AutoStartEndSet(void)
{
    switch(BaseSet1_STEP)
	{
		case	0:
			if((EnSelFlsh)&&(DispStep==DispSet1_1)&&(DispSetSel==0x01))
			{    
                WaitTim = T10mS + MoveMaxTim;									//设定反转时间
				BaseSet1_STEP=1;
			}
			break;
		case	1:																//反转
			Moto_REV_Drv;
			if(PIStus.ACLS || PIStus.CTS || T10mS >= WaitTim)		            //到开始位或超时?
			{
				PosLmdD=AvVal[_FK];												//取数据
                if(PosLmdD < 10)//电压够低，说明使用的是编码器
                {
                    PosLmdD = EncoderCurr;
                }
                ZeroStatus = 1;                                                 //开始过零点检测    
				Moto_STOP_Drv;													//去刹车
				WaitTim = T10mS + StopDellTim;									//设定刹车时间
				BaseSet1_STEP=2;
			}
			break;
		case	2:																//刹车
			if(T10mS>=WaitTim)													//刹车时间到
			{
				//Moto_REV_Drv;
                //Moto_FWD_Drv;
				WaitTim = T10mS + MoveMaxTim;									//设定正转时间
				BaseSet1_STEP=3;
			}
			break;
		case	3:													            //正转
			Moto_FWD_Drv;
			if(PIStus.AOLS || PIStus.OTS || T10mS >= WaitTim)					//到未位或超时?
			{
				PosLmdU=AvVal[_FK];												//取数据
                if(PosLmdU < 10)//电压够低，说明使用的是编码器
                {
                    PosLmdU = EncoderCurr;
                }
                ZeroStatus = 3;
				Moto_STOP_Drv;													//去刹车
				WaitTim = T10mS + StopDellTim;									//设定刹车时间
				BaseSet1_STEP=4;
			}
			break;
		case	4:																//刹车
//			if(T10mS>=WaitTim)													//刹车时间到
			{
				Moto_PARK_Drv;													//释放电机
				AutoSetOKF=1;
				EnSelFlsh=0;													//使能反白显示闪烁//使能被选项闪烁
                BaseSet1_STEP=0;												//标定结束
                Closeway = ClosewayTemp & 1;         //保存原有的关方式
                ClosewayTemp = 0;
                ZeroStatus = 0;
                isZero = isZeroTemp;
                isZeroTemp = 127;
            }
			break;
		}
}

/*********************************************************
function name: 开位确定
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
				Moto_FWD_Drv;//正转
				
			}
			if(K3Dn)									//-
			{
				ClsKDn;
				Moto_REV_Drv;//反转
				
			}
			WaitTim = T10mS + StopDellTim;									//设定刹车时间
		}
		else
		{
			Moto_STOP_Drv;
			if(T10mS>=WaitTim)
			{
//				EnSelFlsh=0;												//标定结束
				Moto_PARK_Drv;												//释放电机
			}
		}
   
		if(K2Dn)									//enter?
		{
			ClsKDn;
			PosLmdU=AvVal[_FK];
            if(PosLmdU < 10)//电压够低，说明使用的是编码器
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
			EnSelFlsh=0;					//使能反白显示闪烁//使能被选项闪烁
            Closeway = ClosewayTemp & 1;         //保存原有的关方式
            ClosewayTemp = 0;
        }
	}
}

/*********************************************************
function name: 关位确定
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
                Moto_FWD_Drv;//正转
                
            }
            if(K3Dn)									//-
            {
                ClsKDn;
                Moto_REV_Drv;//反转
                
            }
            WaitTim = T10mS + StopDellTim;			//设定刹车时间
        }
        else
        {
            Moto_STOP_Drv;
            if(T10mS>=WaitTim)
            {
//				EnSelFlsh=0;								//标定结束
                Moto_PARK_Drv;						//释放电机
            }
        }
                
        if(K2Dn)										//enter?
        {
            ClsKDn;
            PosLmdD=AvVal[_FK];
            if(PosLmdD < 10)//电压够低，说明使用的是编码器
            {
                PosLmdD = EncoderCurr;
            }
            if(ZeroStatus != 2)//过零点检测完成？
            {
                ZeroStatus += 2;
                if(ZeroStatus >= 3)
                {
                    ZeroStatus = 0;
                    isZero = isZeroTemp;
                    isZeroTemp = 127;
                }
            }
            EnSelFlsh=0;								//标定结束
            Closeway = ClosewayTemp & 1;         //保存原有的关方式
            ClosewayTemp = 0;
        }
    }
}

/*********************************************************
function name: 关闭方向		
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
				SetCloseDir = 0;			//反向
			}
			else
			{
				SetCloseDir = 1;			//正向
			}
			EnSelFlsh=0;					//结束
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
function name: 关闭方式
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

			EnSelFlsh=0;					//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			Closewayset = Closeway;
			EnSelFlsh=0;					//结束
		}
	}	
}

/*********************************************************
function name: 控制精度设定
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
					if(PosAccuracy_flag > 50)									//最高精度0.5
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
				PosAccuracy=PosAccuracy_flag;					//确认更改精度
				EnSelFlsh=0;							//结束
			}
			else if(K1Dn)
			{
				ClsKDn;
				PosAccuracy_flag=PosAccuracy;
				EnSelFlsh=0;					//结束
			}
		}
}

/*********************************************************
function name: 现场控制:选择点动、保持、自动模式
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
			switch(JogHolAutoSelflag)					//控制模式选择
			{
				case 1:	JogHolAutoSel=1;		//点动
				break;
				
				case 2:	JogHolAutoSel=2;		//保持
				break;
								
				case 3:	JogHolAutoSel=3;		//自动
				break;
			}
			EnSelFlsh=0;							//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			JogHolAutoSelflag=JogHolAutoSel;
			EnSelFlsh=0;					//结束
		}
	}
}


/*********************************************************
function name: 远程设置(输入模拟量或数字量)
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
				A_DInSel=1;		//远程输入开关量
			}
			else
			{
				A_DInSel=0;		//远程输入为模拟量
			}
			EnSelFlsh=0;					//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			A_DInSelflag=A_DInSel;
			EnSelFlsh=0;					//结束
		}
	}
}

/*********************************************************
function name: 丢信动作设置
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
			EnSelFlsh=0;					//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			NoSignalCtrSelflag=NoSignalCtrSel;
			EnSelFlsh=0;					//结束
		}
	}
}

/*********************************************************
function name: 显示方向设置
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
				ScrSel=0;					//正向显示
                transfer_command(0xc4);     //设置LCD 翻转方法
			}
			else
			{
				ScrSel=1;					//反向显示
                transfer_command(0xc2);     //设置LCD 翻转方法
			}
            clear_screen();
			EnSelFlsh=0;					//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			ScrSelflag=ScrSel;
			EnSelFlsh=0;					//结束
		}
	}
}

/*********************************************************
function name: 语言选择设置
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
//					language=1;					//此版本暂时不设英文菜单
					language=0;
				}
				else 
				{
					language=0;
				}
				EnSelFlsh=0;					//结束
				clear_screen();
			}
			else if(K1Dn)
			{
				ClsKDn;
				languageflag=language;
				EnSelFlsh=0;					//结束
			}
		}
}

/*********************************************************
function name: 基本设置操作
fun	:功能选择

*********************************************************/
void BaseSet(void)
{
    AutoStartEndSet();
    OpenSet();
    CloseSet();
    CloseWaySet();			//关闭方式
	CloseDirectionSet();	//关闭方向
    SingleSel();
    WorkModSet();
    NoSingleSet();
    LCDDirectionSet();
    LCDLanguage();
}

/*********************************************************
function name: 输出信号选择，电压或电流
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
			EnSelFlsh=0;												//结束
		}
		else if(K1Dn)					//是否返回上级菜单
		{
			ClsKDn;
			OutCVSelflag=OutCVSel;		//恢复旧值	
			EnSelFlsh=0;					//结束
		}
	}
}

/*********************************************************
function name: 给定低信
fun :		
*********************************************************/
void InSSet(void)
{
	if((EnSelFlsh)&&(DispStep==DispSet1_2)&&(DispSetSel==0x02))
	{
		if(K2Dn)														//enter?
		{
			ClsKDn;
			EnSelFlsh=0;												//结束
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
function name: 给定高信
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
			EnSelFlsh=0;												//结束
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
function name: 输出低信
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
			EnSelFlsh=0;												//结束
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
function name: 输出高信
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
			EnSelFlsh=0;												//结束
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
反馈设置操作
fun	:功能选择

*********************************************************/
void FeedBackSet(void)
{
	OutCVSet();										//输出入电流电压选择
	InSSet();										//给定低信
	InESet();										//给定高信
	OutSSet();										//输出低信
	OutESet();										//输出高信
}


/*********************************************************
function name: ESD设置
fun :		ESD三种状态，开到位，闭到位，停止
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
				      //自由状态
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
			EnSelFlsh=0;						//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			ESDsetflag=ESDset;
			EnSelFlsh=0;	    				//结束
		}
	}
}

/*********************************************************
function name: 通讯控制
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

			EnSelFlsh=0;					//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			Rs485Wayset = Rs485Way;
			EnSelFlsh=0;					//结束
		}
	}	
}

/*********************************************************
function name: 通讯控制
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
			EnSelFlsh=0;							//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			mbaddr_flag = mbaddr;
			EnSelFlsh=0;					         //结束
		}
	}	
}

/*********************************************************
function name: 两线控制		
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
			EnSelFlsh=0;							//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			TwoLineflag=TwoLine;
			EnSelFlsh=0;					//结束
		}
	}	
}

/*********************************************************
function name: 堵转时间		
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
				if(LockedRotorTime_flag > 20)//大于10的随意值
				{
					LockedRotorTime_flag = 10;
				}
			}
		}
		if(K2Dn)									//enter?
		{
			ClsKDn;
			LockedRotorTime=LockedRotorTime_flag;					//确认更改堵转时间
			EnSelFlsh=0;							//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			LockedRotorTime_flag=LockedRotorTime;
			EnSelFlsh=0;					//结束
		}
	}
}

/*********************************************************
function name: 电子刹车
fun :		开，关
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
			EnSelFlsh=0;								//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			electronicbrakeflag=electronicbrake;
			EnSelFlsh=0;					//结束
		}
	}		
}

/*********************************************************
function name: 间断运行		
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
				IntermittentRun=1;					//是
			}
			else
			{
				IntermittentRun=0;					//否
			}
			EnSelFlsh=0;								//结束
		}
		else if(K1Dn)
		{
			ClsKDn;
			IntermittentRunflag=IntermittentRun;
			EnSelFlsh=0;					//结束
		}
	}	
}

/*********************************************************
function name: 间断运行函数		
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
    //2016年9月13日13:38:50 zhaoxw target 并没有使用
//	if(times==18)					//500mSx18 =4S
//	{
//		target=GetMotoPos();		//读取一次阀位值
//	}

	if(((times>=10)&&(times<=18))||(GetMotoPos()== Htarget)||(GetMotoPos()== Ltarget))
	{
		if(times==18)
		{
			times=0;			
		}
		if(electronicbrake)				   //如果开启了电子杀车功能 break;
		{
			Moto_STOP_Drv;
		}
		else								//没有开启电子刹车功能
		{
			Moto_PARK_Drv;
		}
	}
//	else if((GetMotoPos() <  target+680)&&(GetMotoPos() <Htarget))				//小于目标值，正转
	else if( (((PIStus.VMA_IN_Pers )/10)*10  + PosAccuracy)&&(GetMotoPos() <Htarget))			//小于目标值，正转
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
	else if( (((PIStus.VMA_IN_Pers)/10)*10  + PosAccuracy)&&(GetMotoPos() >Ltarget))			//大于目标值反转
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
function name: 中间位置1
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
			EnSelFlsh=0;					//结束
		}
	}	
}

/*********************************************************
function name: 中间位置2
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
			EnSelFlsh=0;					//结束
		}
	}	
}

/*********************************************************
function name: 密码设置
fun :		
*********************************************************/
void PasswordSet(void)		//密码设定
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
				EnSelFlsh=0;											//结束
				PassWord =BCDtoDec(BCD_buffer);						//密码设置OK
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
function name: 恢复出厂
fun :		
*********************************************************/
void DefaultSet(void)		//恢复出厂
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
			//基本参数设定类
            //AutoSetOKF=0;				//未标定过
            isZero = 127;
            PosLmdU=3992;				//阀位上限		理论4095，实际会小于
            PosLmdD=38;				    //阀位下限
            CInLmdU=3707;			    //给定C上限
            CInLmdD=746;			    //给定C下限
            VInLmdU=4095;				//给定V上限
            VInLmdD=0;				    //给定V下限
            OutCLmdU=4095;				//输出上限
            OutCLmdD=0; 				//输出下限
            OutVLmdU=4095;				//输出上限
            OutVLmdD=0; 				//输出下限
            PosAccuracy=100;			//阀位精度(默认1.00)
            JogHolAutoSel=1;			//现场控制方式:	1点动,2保持,3自动
            A_DInSel=1;					//远程输入方式:	0模拟量,1开关量
            NoSignalCtrSel=1;			//丢信处理方式	1停止，2全开，3全闭
            RmLcDisSel =1;				//远程0/本地(本地键盘操作)1/禁止>=2切换
            ScrSel =0;					//显示方向
            transfer_command(0xc4);     //设置LCD 翻转方法 恢复出厂默认参数时候显示问题
            OutCVSel=1;					//输出电流或电压选择
            PassWord = 0; 			    //密码(默认为"0000"),限定小于等于9999
            //ChSel[i]                  //通道设置
            RelayType[0] = 0;           //保存继电器需要组态报警的序号
            RelayType[1] = 1;
            RelayType[2] = 2;
            RelayType[3] = 3;
            RelayType[4] = 5;
            RelayType[5] = 6;
            RelayType[6] = 9;
            RelayType[7] = 10;
            RelayType[8] = 7;
            //A_DInSel=0;				//远程输入方式:	0模拟量,1开关量
            //NoSignalCtrSel=1;			//丢信处理方式	1停止，2全开，3全闭
            language=0;					//语言选择		0中文 非0英文	

            //高级参数设定类
            electronicbrake=0;			//电子刹车开启标志    0不开  非0 开启
            //CloseDirection = 1;			//关闭方向	1正 0反
            SetCloseDir = 1;
            Closeway=0;					//Closeway, 非0 力矩   0 限位  判断到达闭位和开位的依据 力矩开关或限位开关
            Rs485Way=0;                 //1：总线控制有效读写有效 0：总线控制无效读写有效
            ESDset=1;					//ESD设置，2开位，3闭位，1停止
            mbaddr = 1;
            TwoLine=1;					//两线控制	  1禁止  2有信号开  3有信号关	  
            LockedRotorTime=4;			//堵转时间		0- 10S
            IntermittentRun=0;			//间断运行	(非0开间断运行，0关)
            Intermediate1=25;			//中间位置1 0%~中间位置1
            Intermediate2=75;
            lock=0;                     //默认不上软件锁
			EnSelFlsh =0;
			clear_screen();
			STMFLASH_Write(FLASH_SAVE_ADDR + 98,0,1);	//重新存储
            Display_Init();             //刷新菜单
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
高级设置操作
fun :功能选择

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
	EsdSet();				//ESD设置  
	TwoLineSet();
    
	PrecisionSet();
	//IntermittentRunSet();
    Intermediate1Set();
    Intermediate2Set();
	PasswordSet();			//密码设置
	DefaultSet();				//恢复出厂设置
}

/*********************************************************
取得阀门位置
结果:
	0 (_LessLdD):在有效下限之外
	1(_LessTarget):在目标下位
	2(_InTarget):在目标之内
	3(_MoreTarget):在目标上位
	4(_MoreLdU):在有效上限之外
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
function name:测试按键
fun :	测试远程本地选择按键是否正常，进入此菜单显示，运行界面显示一致
*********************************************************/
void Testkey(void)
{
	//此功能在 void disp_Set1_4_x(u16 Sel)	函数里已完成
}

/*********************************************************
function name:力矩开关测试	开力矩  闭力矩   空位
fun :
*********************************************************/
void TorqueTest(void)
{
	//此功能在 void disp_Set1_4_x(u16 Sel)	函数里已完成
}

/*********************************************************
function name:测试远程打开
fun :
*********************************************************/
void Testopen(void)
{
//此功能在 void disp_Set1_4_x(u16 Sel)	函数里已完成
}

/*********************************************************
function name:测试远程关闭
fun :
*********************************************************/
void Testclose(void)
{
//此功能在 void disp_Set1_4_x(u16 Sel)	函数里已完成
}

/*********************************************************
function name:测试远程保持
fun :
*********************************************************/
void Testkeep(void)
{
//此功能在 void disp_Set1_4_x(u16 Sel)	函数里已完成
}

/*********************************************************
function name:测试远程自动
fun :
*********************************************************/
void Testautomatic(void)
{
//此功能在 void disp_Set1_4_x(u16 Sel)	函数里已完成	
}

/*********************************************************
function name:恢复密码
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
				PassWord = 0000; 			//密码(默认为"0000"),限定小于等于9999		
				EnSelFlsh =0;
				clear_screen();
			}
		}
	}
}

/*********************************************************
function name:软件版本
fun :			查看软件版本号
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

        display_char8x16(jump+line*4,15,"V:0.2.40b",0); 				//版本号
        display_char8x16(jump+line*5,15,"2017/05/09",0);			//日期

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
function name:故障诊断项目操作
fun :功能选择

*********************************************************/
void FaultDiagnosisSet(void)
{
	Testkey();    				  //测试远程按键
	TorqueTest();		 	//力矩检测
	Testopen();				 //测试远程打开
	Testclose();				 //测试远程关闭
	Testkeep();				 //测试远程保持
	Testautomatic(); 			 //测试远程自动
	Recoverypassword();		//密码恢复
	SftwareVersion();			//软件版本
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
