
#ifndef	__LCD_160160_H__
#define __LCD_160160_H__
#include "sys.h"

#define LCD_Back PAout(15)// 背光控制	

extern const u8 XZYE_8x9[];
extern const u8 WXYE_8x9[];
//字符定义:
extern const u16 Chr0_15x16[];			// 0
extern const u16 Chr1_15x16[];			// 1
extern const u16 Chr2_15x16[];			// 2
extern const u16 Chr3_15x16[];			// 3
extern const u16 Chr4_15x16[];			// 4
extern const u16 Chr5_15x16[];			// 5
extern const u16 Chr6_15x16[];			// 6
extern const u16 Chr7_15x16[];			// 7
extern const u16 Chr8_15x16[];			// 8
extern const u16 Chr9_15x16[];			// 9
extern const u16 ChrPoin_15x16[];		// .
extern const u16 ChrPer_15x16[];		// %
extern const u16 ChrC_15x16[];			//C
extern const u16 ChrG_15x16[];			//G
extern const u16 ChrO_15x16[];			//O
extern const u16 ChrK_15x16[];			//K
extern const u16 ChrE_15x16[];			//E
extern const u16 ChrS_15x16[];			//S
extern const u16 ChrD_15x16[];			//D

extern const u16 ChrM_15x16[];			//m
extern const u16 ChrA_15x16[];			//A
extern const u16 ChrV_15x16[];			//V

extern const u8 Chrm_14x14[];			//m
extern const u8 ChrA_14x14[];			//A
extern const u8 ChrV_14x14[];
extern const u8 ChrE_14x14[];
extern const u8 ChrS_14x14[];
extern const u8 ChrD_14x14[];



extern const u16 Chr0_16x32[];			//0
extern const u16 Chr1_16x32[];			// 1
extern const u16 Chr2_16x32[];
extern const u16 Chr3_16x32[];
extern const u16 Chr4_16x32[];
extern const u16 Chr5_16x32[];
extern const u16 Chr6_16x32[];
extern const u16 Chr7_16x32[];
extern const u16 Chr8_16x32[];
extern const u16 Chr9_16x32[];			//9
extern const u16 ChrPoin_16x32[];		//.
extern const u16 ChrEf_16x32[];			//-
extern const u16 ChrSP_16x32[];			// 
extern const u16 ChrTC_16x32[];			//℃
extern const u16 ChrV_16x32[];			//V
extern const u16 ChrPer_16x32[];		//%
extern const u16 Ji_16x32[];			//基
extern const u16 Ben_16x32[];			//本
extern const u16 She_16x32[];			//设
extern const u16 Zhi_16x32[];			//置
extern const u16 Fa_16x32[];			//阀
extern const u16 Wei_16x32[];			//位
extern const u16 Biao_16x32[];			//标
extern const u16 Ding_16x32[];			//定
extern const u16 Gao_16x32[];			//高
extern const u16 Ji1_16x32[];			//高
extern const u16 Hui_16x32[];			//恢
extern const u16 Fu_16x32[];			//复
extern const u16 Chu_16x32[];			//出
extern const u16 Chang_16x32[];			//厂
extern const u16 Shi_16x32[];			//是
extern const u16 Fou_16x32[];			//否
extern const u16 Kai_16x32[];			//开
extern const u16 Guan_16x32[];			//关
extern const u16 Dao_16x32[];			//到
extern const u16 Xian_16x32[];			//现
extern const u16 Chang1_16x32[];		//场
extern const u16 Sou_16x32[];			//手
extern const u16 Zhi1_16x32[];			//自
extern const u16 Dong_16x32[];			//动
extern const u16 Zhi2_16x32[];			//值

extern const u16 Zheng_16x32[];			//正
extern const u16 Fan_16x32[];			//反
extern const u16 Kui_16x32[];			//馈
extern const u16 Gu_16x32[];			//故
extern const u16 Zhang_16x32[];			//障
extern const u16 Zhen_16x32[];			//诊
extern const u16 Duan_16x32[];			//断

extern const u16 Da_16x32[];			//打
extern const u16 Bi_16x32[];			//闭
extern const u16 Jing_16x32[];			//静
extern const u16 Ting_16x32[];			//停
extern const u16 Wu_16x32[];			//无
extern const u16 Diu_16x32[];			//丢
extern const u16 Xin_16x32[];			//信
extern const u16 Hao_16x32[];			//号
extern const u16 Nei_16x32[];			//内
extern const u16 Wai_16x32[];			//外
extern const u16 Chao_16x32[];			//超
extern const u16 Wen_16x32[];			//温
extern const u16 Yuan_16x32[];			//远
extern const u16 Cheng_16x32[];			//程
extern const u16 Chuang_16x32[];		//场
extern const u16 Zhen1_16x32[];			//震
extern const u16 Dang_16x32[];			//荡
extern const u16 Du_16x32[];			//堵

extern const u16 Jie_16x32[];			//接
extern const u16 Xian1_16x32[];			//线
extern const u16 Zhi3_16x32[];			//止
extern const u16 Du1_16x32[];			//度

extern const u16 Jin_16x32[];			//禁
extern const u16 Di_16x32[];			//地
extern const u16 Zhong_16x32[];		    //中
extern const u16 Quan_16x32[];          //全

extern const u16 SP_16x16[];
extern const u16 LOCK_16x16[];
extern const u16 NET_16x16[];

//14x14液晶字模
//extern const u8 Chr0_14x14[];			// 0
extern const u8 Chr1_14x14[];			// 1
extern const u8 Chr2_14x14[];			// 2
//extern const u8 Chr3_14x14[];			// 3
//extern const u8 Chr4_14x14[];			// 4
//extern const u8 Chr5_14x14[];			// 5
//extern const u8 Chr6_14x14[];			// 6
//extern const u8 Chr7_14x14[];			// 7
//extern const u8 Chr8_14x14[];			// 8
//extern const u8 Chr9_14x14[];			// 9
extern const u8 SP_14x14[];
extern const u8 Ben_14x14[];
extern const u8 Di_14x14[];
extern const u8 Fang_14x14[];
extern const u8 Shi_14x14[];
extern const u8 An_14x14[];
extern const u8 Jian_14x14[];
extern const u8 Yuan_14x14[];
extern const u8 Cheng_14x14[];
extern const u8 Jin_14x14[];
extern const u8 Zhi_14x14[];
extern const u8 Cao_14x14[];
extern const u8 Zuo_14x14[];
extern const u8 Kai_14x14[];
extern const u8 Bi_14x14[];
extern const u8 Da_14x14[];
extern const u8 Guan_14x14[];
extern const u8 Bao_14x14[];
extern const u8 Chi_14x14[];
extern const u8 Zi_14x14[];
extern const u8 Dong_14x14[];
extern const u8 Hui_14x14[];
extern const u8 Fu_14x14[];
extern const u8 Mi_14x14[];
extern const u8 Ma_14x14[];
extern const u8 Ruan_14x14[];
extern const u8 Jian1_14x14[];
extern const u8 Ban_14x14[];
extern const u8 Yi_14x14[];
extern const u8 Biao_14x14[];
extern const u8 Ding_14x14[];
extern const u8 Wei_14x14[];
extern const u8 Wei1_14x14[];
extern const u8 Que_14x14[];
extern const u8 Ren_14x14[];
extern const u8 Kong_14x14[];
extern const u8 Kong1_14x14[];
extern const u8 Zhi1_14x14[];
extern const u8 Jing_14x14[];
extern const u8 Du_14x14[];
extern const u8 Xian_14x14[];
extern const u8 Chang_14x14[];
extern const u8 She_14x14[];
extern const u8 Zhi2_14x14[];
extern const u8 Diu_14x14[];
extern const u8 Xin_14x14[];
extern const u8 Ting_14x14[];
extern const u8 Xian1_14x14[];
extern const u8 Shi1_14x14[];
extern const u8 Xiang_14x14[];
extern const u8 Fan_14x14[];
extern const u8 Yu_14x14[];
extern const u8 Yan_14x14[];
extern const u8 Xuan_14x14[];
extern const u8 Ze_14x14[];
extern const u8 Zhong_14x14[];
extern const u8 Wen_14x14[];
extern const u8 Ying_14x14[];
extern const u8 Dian_14x14[];
extern const u8 Mo_14x14[];
extern const u8 Ni_14x14[];
extern const u8 Quan_14x14[];
extern const u8 Zheng_14x14[];
extern const u8 Hao_14x14[];
extern const u8 Dian1_14x14[];
extern const u8 Liu_14x14[];
extern const u8 Ya_14x14[];
extern const u8 Gei_14x14[];
extern const u8 Di1_14x14[];
extern const u8 Gao_14x14[];
extern const u8 Shu_14x14[];
extern const u8 Chu_14x14[];
extern const u8 Chu1_14x14[];
extern const u8 Xian2_14x14[];
extern const u8 Li_14x14[];
extern const u8 Ju_14x14[];
extern const u8 Liang_14x14[];
extern const u8 Xian3_14x14[];
extern const u8 Du1_14x14[];
extern const u8 Zhuan_14x14[];
extern const u8 Shi2_14x14[];
extern const u8 Shi3_14x14[];
extern const u8 Fou_14x14[];
extern const u8 Jian2_14x14[];
extern const u8 Jian3_14x14[];
extern const u8 Ce_14x14[];
extern const u8 Zi1_14x14[];
extern const u8 Sha_14x14[];
extern const u8 Che_14x14[];
extern const u8 Duan_14x14[];
extern const u8 Yun_14x14[];
extern const u8 xing_14x14[];
extern const u8 Chang1_14x14[];
extern const u8 Pan_14x14[];
extern const u8 Dao_14x14[];
extern const u8 You_14x14[];
extern const u8 Wu_14x14[];
extern const u8 Fu1_14x14[];
extern const u8 Zhu_14x14[];
extern const u8 Guo_14x14[];
extern const u8 Bao1_14x14[];
extern const u8 Jing1_14x14[];
extern const u8 Tong_14x14[];
extern const u8 Xun_14x14[];
extern const u8 Cuo_14x14[];
extern const u8 Wu1_14x14[];
extern const u8 tan_14x14[];
extern const u8 Zhi3_14x14[];
extern const u8 Que1_14x14[];
extern const u8 Xiang1_14x14[];
extern const u8 Yuan1_14x14[]; 

extern const u8 Sp_8x16[];
extern const u8 Num1_8x14[];
extern const u8 Num2_8x14[];
extern const u16  Full32x32[];			//满格(通用)
extern const u16 Sp32x32[];				//空格(通用)

extern const u16 Chr0_32x32[];			// 0
extern const u16 Chr1_32x32[];			// 1
extern const u16 Chr2_32x32[];			// 2
extern const u16 Chr3_32x32[];			// 3
extern const u16 Chr4_32x32[];			// 4
extern const u16 Chr5_32x32[];			// 5
extern const u16 Chr6_32x32[];			// 6
extern const u16 Chr7_32x32[];			// 7
extern const u16 Chr8_32x32[];			// 8
extern const u16 Chr9_32x32[];			// 9
extern const u16 ChrPoin_32x32[];		// .
extern const u16 ChrPer_32x32[];		// %
extern const u16 ChrV_32x32[];			// V
extern const u16 ChrTm_32x32[];			// T
extern const u16 ChrOm_32x32[];			// O
extern const u16 ChrYm_32x32[];			// Y
extern const u16 ChrIm_32x32[];			// I
extern const u16 ChrRm_32x32[];			// R
extern const u16 Shu32x32[];			//输
extern const u16 Ru32x32[];				//入
extern const u16 Mi32x32[];				//密
extern const u16 Ma32x32[];				//码
extern const u16 Qing32x32[];			//请
extern const u16 Que32x32[];			//确
extern const u16 Ding32x32[];			//定
extern const u16 WenHao32x32[];			//?
extern const u16 GanTan32x32[];			//!
extern const u16 She32x32[];			//设
extern const u16 Zhi32x32[];			//置
extern const u16 Geng32x32[];			//更
extern const u16 Guai32x32[];			//改
extern const u16 Xin32x32[];			//新
extern const u16 Sp_16x32[];

extern const u16 Kai_16x16[];
extern const u16 Guo_16x16[];
extern const u16 Li_16x16[];
extern const u16 Ju_16x16[];
extern const u16 Bi_16x16[];
extern const u16 Zhuan_16x16[];
extern const u16 Xiang_16x16[];
extern const u16 Cuo_16x16[];
extern const u16 Wu_16x16[];
extern const u16 Tong_16x16[];
extern const u16 Zai_16x16[];
extern const u16 Du_16x16[];
extern const u16 Fa_16x16[];
extern const u16 Dui_16x16[];
extern const u16 Xin_16x16[];
extern const u16 Hao_16x16[];
extern const u16 Nei_16x16[];
extern const u16 Bu_16x16[];
extern const u16 Chao_16x16[];
extern const u16 Wen_16x16[];
extern const u16 Dian_16x16[];
extern const u16 Ji_16x16[];
extern const u16 Re_16x16[];
extern const u16 Yuan_16x16[];
extern const u16 Que_16x16[];
extern const u16 Xiang1_16x16[];
extern const u16 SP1_16x16[];
    
extern u8 const Num_8x14[12][14];
extern u8 const Symbol_8x16[4][16];
extern u8 const string_code3[];
extern u8 BCD_buffer[6];								//BCD码存放区(BCD_buffer[0]为个位)

void display_char8x16(u16 y,u16 x,const u8 *dp,u16 f);
void display_char16x16(u16 y,u16 x,const u16 * dp,u16 f);		//适合X_15x16,X_15x16B点阵
void display_char16x32(u16 y,u16 x,const u16 * dp,u16 f);		//适合X_16x32,X_16x32B点阵
void display_char32x32(u16 y,u16 x,const u16 *dp,u16 f);		//适合X_32x32,X_32x32B点阵
void display_char12x12(u16 y,u16 x,const u16 *dp,u16 f);		//12X12汉字点阵
void display_char12x14(u16 y,u16 x,const u16 *dp,u16 f);		//12x14汉字点阵
void display_char14x14(u16 y,u16 x,const u8 *dp,u16 f);
//void display_Str16x32(u16 y,u16 x,u8 s,u16 f);
void display_char8x9(u16 y,u16 x,const u8 *dp,u16 f);
void dispayNum8x16(u16 y,u16 x,u8 bcd ,u16 f);
void dispayNum16x16(u16 y,u16 x,u8 bcd ,u16 f);
void dispayNum16x32(u16 y,u16 x,u8 bcd ,u16 f);
void dispayNum32x32(u16 y,u16 x,u8 bcd ,u16 f);
void display_Num_8x16(u16 y,u16 x,u32 num ,u16 f);
void display_4Num_8x16(u16 y,u16 x,u32 num ,u16 f);
void display_2Num_8x16(u16 y,u16 x,u32 num ,u16 f);
void LCD_Init(void);
void set_lcd_address(u16 y,u16 x);
void testLCD(void);
void LCDPort_Init(void);
void display_3pixels(u16 mono_data);
void hextobcd(u32 hexs);
u32 BCDtoDec(const u8 *bcd);

void LCD_Initx(u8 dir);
void transfer_command(u16 data1);
void display_Num_32x32_LZND(u16 y,u16 x,u32 num ,u16 f);
void dispayNum8x14(u16 y,u16 x,u8 bcd ,u16 f);
void dispay4Num8x14(u16 y,u16 x,u16 num ,u16 f);
void dispay3Num8x14(u16 y,u16 x,u16 num ,u16 f);
void display_char8x14(u16 y,u16 x,u8 bcd ,u16 f);
void displaySymbol_8x16(u16 y,u16 x,u8 index,u16 f);
void display1_char16x16(u16 y,u16 x,const u16 * dp,u16 f);		//适合X_15x16,X_15x16B点阵
#endif

