#ifndef __SETUP_H
#define __SETUP_H	 

#include "sys.h"

//字串定义(对应InfoDispStrSet第一下标):
/*
#define _DiuXin			1					//丢信号
#define _XianFan		3					//线反
#define _ZhenDang		5					//震荡
#define _BiDu			6					//闭堵
#define _KaiDu			7					//开堵
#define _WaiChaoWen		8					//外超温
#define _NeiChaoWen		9					//内超温

#define _Kaifa			10					//开阀
#define _BiFa			11					//闭阀
#define _TingZhi		12					//停止
#define _YuanCheng		13					//远程
#define _XianChang		14					//现场
#define _DaoWei			15					//到位
#define _ZhongWei		16					//中位
#define _QuanKai		17					//全开
#define _QuanBi			18					//全闭
#define _KaiGuoJu		19					//开过力矩
#define _BiGuoJu		20					//闭过力矩
*/

//报警类 顺序决定优先级别 低->高
#define _ZhuanXiang		0					//转向错误
#define _KaiBiOn		1					//开关同在
#define _BiDu			2					//闭堵
#define _KaiDu			3					//开堵
#define _DiuXin			4					//丢信号
#define _KaiGuoJu		5					//开过力矩
#define _BiGuoJu		6					//闭过力矩
#define _NeiChaoWen		7					//内超温
#define _WaiChaoWen		8					//外超温
#define _QueXiang		9					//缺相
//状态类
#define _Kaifa			0					//开阀
#define _BiFa			1					//闭阀
#define _TingZhi		2					//停止
#define _DaoWei			3					//到位
#define _ZhongWei		4					//中位
#define _QuanKai		5					//全开
#define _QuanBi			6					//全闭

//extern const u16 *InfoDispStrSet[22][5];	//最多四个字符的字符串(16x32 点阵的汉字或图片序列)库

extern u16 AutoSetOKF;						//自动标定过
extern u16 isZero;					    //编码器是否过零点
extern u32 PosLmdU;							//阀位上限
extern u32 PosLmdD;							//阀位下限
extern u16 CInLmdU;							//给定上限
extern u16 CInLmdD;							//给定下限
extern u16 VInLmdU;							//给定上限
extern u16 VInLmdD;							//给定下限

extern u16 OutCLmdU;							//输出上限
extern u16 OutCLmdD;							//输出下限
extern u16 OutVLmdU;							//输出上限
extern u16 OutVLmdD;							//输出下限
extern u16 PosAccuracy;						//阀位精度
extern u16 JogHolAutoSel;						//现场控制方式:	1点动,2保持,3自动

extern u16 A_DInSel;							//远程输入方式:	0模拟量,1开关量
extern u16 ScrSel ;							//显示方向
extern u16 NoSignalCtrSel;					//丢信处理方式	1停止，2全开，3全闭
extern u16 language;					//语言选择		0中文 非0英文
extern u16 electronicbrake;					//电子刹车开启标志	
extern u16 CloseDirection;					//关闭方向			1正   0反
extern u16 SetCloseDir;					//关闭方向			1正   0反
extern u16 Closeway;						//Closeway, 非0 力矩   0 限位  判断到达闭位和开位的依据 力矩开关或限位开关
extern u16 Rs485Way;
extern u16 ESDset;						//ESD设置，2开位，3闭位，1停止
extern u16 mbaddr;						//通讯地址
extern u16 TwoLine;						//两线控制	  1禁止  2有信号开  3有信号关	  
extern u16 LockedRotorTime;				//堵转时间
extern u16 IntermittentRun;				//间断运行	(非0开间断运行，0关)
extern u16 Intermediate1;			    //中间位置1 0%~中间位置1
extern u16 Intermediate2;			    //中间位置2 中间位置1~100%
extern u16 PassWord;						//密码(默认为"0000"),限定小于等于9999
extern u16 RmLcDisSel;						//远程0/本地(本地键盘操作)1/禁止>=2切换

extern u16 OutCVSel;							//输出电流或电压选择

extern u16 EnSelFlsh;							//使能反白显示闪烁//使能被选项闪烁
extern u16 DispParaSet_test;					//测试参数显示选择

extern u16 OutCVSel;							//输出电流或电压选择
extern u8 DispStep;
extern u16 lock;						//远程锁状态 1：上锁 0：解锁
extern u32 EncoderCurr;                 //编码器当前值
void Display(void);
void clear_screen(void);
void Display_Init(void);

#endif

