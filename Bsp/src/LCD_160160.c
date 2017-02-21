/* Test program for JLX16080，并行接口
驱动IC 是:UC1698(or compatible)
晶联讯电子：网址 http://www.jlxlcd.cn;

所有x,y参数，y以像素为单位，x以3个像素为一个单位
*/
#include "Def.h" 
#include "LCD_160160.H"
#include "LCD_32x32.H"
#include "LCD_15X16.H"
#include "LCD_8X16.H"
#include "LOGO.H"


#define cs1 PDout(11)// PD11,CS0,CS //CS0 片选 低电平片选
#define rs PDout(12)// PD12,RS,CD//寄存器选择信号 也叫“RS”或“D/C”,H:资料寄存器 0:指令寄存器
#define rd PDout(13)// PD13,WR1
#define wr PDout(14)// PD14,WR0	
#define reset PDout(15)//PD15,LCD_RST// LCD复位	

#define LCD_Back PAout(15)// 背光控制	

u8 BCD_buffer[6];								//BCD码存放区(BCD_buffer[0]为个位)

void hextobcd(u32 hexs);

/************************************************************
delay time


************************************************************/
void delay(u16 i)
{
	u16 j,k;
	for(j=0;j<i;j++)
	for(k=0;k<110;k++);
}
/************************************************************
transfer command to LCM

************************************************************/
void transfer_command(u16 data1)
{
	cs1=0;
	rs=0;
	rd=0;
	wr=0;
	data1=data1<<8;
	GPIOE->ODR &=(data1|0x00ff);
	GPIOE->ODR |=(data1&0xff00);
    //GPIOE->ODR = data1;
	rd=1;
	cs1=1;
	rd=0;						//WR1
}
/************************************************************
transfer data to LCM

************************************************************/
void transfer_data(u16 data1)
{
    cs1=0;						//cs
	rs=1;						//CD
	rd=0;						//WR1
	wr=0;						//WR,WR0
	data1=data1<<8;
	GPIOE->ODR &=(data1|0x00ff);
	GPIOE->ODR |=(data1&0xff00);
	rd=1;						//WR1
	cs1=1;
                                //cs
	rd=0;						//WR1
}

/************************************************************
transfer data to LCM

************************************************************/
void transfer_data16(u16 data1)
{
    u16 temp;
    if(ScrSel==1)
    {
        temp = 0;
        if(data1&0x001f)
        {
            temp = 0xf800;
        }
        if(data1&0x07e0)
        {
            temp |= 0x07e0;
        }
        if(data1&0xf800)
        {
            temp |= 0x001f;
        }
        data1 = temp;
    }
    transfer_data(data1>>8); //writting 3dots
    transfer_data(data1);
}
/********************************************
初始化PD11~PD15,PE8~PE15为输出口并使能这两个口的时钟

IO初始化

********************************************/
void LCDPort_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //使能PD,PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  //使能PA,PD,PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//此句为禁止JTAG口调试，用此语句也可使PA15作为普通IO
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		//如果不用SWD或JTAG调试时
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PD.11端口配置,CS(CS0)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				 //根据设定参数初始化GPIOD.11
	GPIO_SetBits(GPIOD,GPIO_Pin_11);						 //PD.11 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PD.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				 //根据设定参数初始化GPIOD.0
	GPIO_SetBits(GPIOD,GPIO_Pin_0);						 //PD.0 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PA.15 端口配置,RS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				 //根据设定参数初始化GPIOA.15
	GPIO_SetBits(GPIOA,GPIO_Pin_15);						 //A.15 输出高
//	GPIO_ResetBits(GPIOA,GPIO_Pin_15);						 //A.15 输出低

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD.12 端口配置,RS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				 //根据设定参数初始化GPIOC.9
	GPIO_SetBits(GPIOD,GPIO_Pin_12);						 //PD.12 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			 //端口配置,WR1(RD)
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_13);						 //输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 			 //端口配置,WR0(WR)
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_14);						 //输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 			 //端口配置,LCD_RST(RES)
	GPIO_Init(GPIOD, &GPIO_InitStructure); 				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_15);						 //输出高 

	//D0~D7    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//端口配置,D0
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_8);							//输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_9);							//输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_10);						//输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			 	//端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);				 	//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_11);						 //输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 			 	//端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure); 				 	//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_12);						 //输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 			 	//端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure); 				 	//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_13);						 //输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 			 	//端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure); 				 	//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_14);						 //输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 			 	//端口配置,D7
	GPIO_Init(GPIOE, &GPIO_InitStructure); 				 	//推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_15);						//输出高 
	
}

/************************************************************
initial LCD

************************************************************/
void LCD_Init()
{
	LCDPort_Init();
	
	reset=0; //低电平复位
	delay(500);
	reset=1; //复位完毕
	delay(5000);
	delay(5000);
	transfer_command(0xe2); //软件复位
	delay(500); //延时
	transfer_command(0x25); //设置温度补偿系数： TC1TC0 01=-0.05%
	transfer_command(0x2b); //internal pump:0x2b
    
	transfer_command(0xc4); //设置LCD 翻转方法
    if(ScrSel==1)
    {
        transfer_command(0xc2); //设置LCD 翻转方法
    }
    
	transfer_command(0xa3); //set line rate
	transfer_command(0xd1); //set color pattem
	transfer_command(0xd6); //设置为64 彩色*/
	transfer_command(0xeb); //设置偏压比: 1/12bias:0xeb
	transfer_command(0x81); //set Vbias potentiometer
	transfer_command(0x8c); //对比度设置08
	transfer_command(0xf1); //设置行数0xf1
	transfer_command(0x9f); //设置行数的数值为80 行:4f,160 行：9f
	transfer_command(0xad); //打开显示
/*
	 
	transfer_command(0xAE);//Set Display Disable
	 
	transfer_command(0x26);//Set Temperature Compensation // 01-0.05% best
	transfer_command(0xA2); //Set Line Rate
	transfer_command(0x2A);//Set Power Control (Interal VLCD;Panel loading definition>13nF) 0x2A
	transfer_command(0xE9); //Set LCD Bias ratio:1/10
	transfer_command(0x81); //Set gain and potentiometer Mode
	transfer_command(0xB8); //PM value:xx 0xB4(客户提供参数值)
	 
	transfer_command(0x89); //Set RAM Address Control
	transfer_command(0xC4); //Set LCD Maping Control (MY=1, MX=0)
	 
	transfer_command(0xDE); //Set COM Scan Function
	 
	transfer_command(0xC8); //Set N-Line Inversion
	transfer_command(0x18); //Set COM Scan Function
	 
	transfer_command(0xD5); //Set Color Mode (4K)
	transfer_command(0xD1); //Set Color Pattern (RGB)
	 
	transfer_command(0x84); //Set Partial Display Off
	 
	transfer_command(0xF4); //Set Windows Program Starting Column Address
	transfer_command(0x25); //
	 
	transfer_command(0xF6); //Set Windows Program Ending Column Address
	transfer_command(0x5A); //
	 
	transfer_command(0xF5); //Set Windows Program Starting Row Address
	transfer_command(0x00); //
	 
	transfer_command(0xF7); //Set Windows Program Ending Row Address
	transfer_command(0x9F); //
	 
	transfer_command(0xF8); //Set Windows Program Mode: Inside Mode
	 
	transfer_command(0xAD); //Set Display Enable
*/

}

/************************************************************

************************************************************/
/*写液晶显示的地址：y 是行，x 是列*/
void set_lcd_address(u16 y,u16 x)
{
	u16 row_msb,row_lsb,column,column_msb,column_lsb;
	row_lsb = y&0x0f;
	row_msb = ((y>>4)&0x0f);
	transfer_command(0x60+row_lsb);//行地址低4 位
	transfer_command(0x70+row_msb);//行地址高4 位
	column=x+37;
	column_lsb =column&0x0f;
	column_msb =((column>>4)&0x0f);
	transfer_command(0x00+column_lsb);///列地址低4 位
	transfer_command(0x10+column_msb);//列地址高4 位
}
/************************************************************

************************************************************/
void full_screen()
{
	u16 i,j,b;
	u8  tmpbit_h,tmpbit_l;
//	u16 row;
//	u16 col;
	u32 mono_data[5];
	u16 color_data;

	for(i = 0; i<160; i++)
		{
		//读入一行
		mono_data[0] = 0;
		for(b = 0; b < 20; b++)
			{
			mono_data[4-(b/4)]=(mono_data[4-(b/4)]<<=8)+LOGO_160x160[b + i *20];
			}
		//写一行
		set_lcd_address(i,0);
		tmpbit_l=0;
		for(j =0; j < 53; j ++)			//先写53 次*3 个点阵=前159 个点阵
			{
			if(mono_data[4]&0x80000000) //
				{color_data=0xf800;}
			else
				{color_data=0x0000;};
			if(mono_data[4]&0x40000000) //
				{color_data=color_data+0x07e0;}
			else;
			if(mono_data[4]&0x20000000) //
				{color_data=color_data+0x001f;}
			else;
				transfer_data(color_data>>8); //writting 3dots
				transfer_data(color_data);
			//mono_data=mono_data<<3;
			for(b = 0; b < 5; b++)
				{
				tmpbit_l=tmpbit_h;
				tmpbit_h =mono_data[b]>>29;
				mono_data[b]=(mono_data[b]<<3) | tmpbit_l;
				}
			}
		
		//写一行最后1个点
		if(mono_data[4]&0x80000000) //bit 1
			{color_data=0xf800;}
		else
			{color_data=0x0000;};
//		transfer_data(color_data>>8); //writting 3dots
//		transfer_data(color_data);
            transfer_data16(color_data);
		}

}
/************************************************************

************************************************************/
void clear_screen()
{
	u16 i,j;
	set_lcd_address(0,0);
	for(i=0;i<160;i++)
	{
		for(j=0;j<128;j++)
		{
		transfer_data(0x00);
		transfer_data(0x00);
		}
	}
}
/************************************************************

************************************************************/
/*全屏显示*/
void display_black()
{
	u16 i,j;
	for(i=0;i<160;i++)
	{
		for(j=0;j<128;j++)
		{
		transfer_data(0xff);
		transfer_data(0xff);
		}
	}
}
void display_snow1()
{
	u16 i,j,line=0;
	for(i=0;i<80;i++)
	{
		set_lcd_address(line,0);
		for(j=0;j<34;j++)
		{
			transfer_data(0xf8);
			transfer_data(0x1f);
			transfer_data(0x07);
			transfer_data(0xe0);
		}
		set_lcd_address(line+1,0);
		for(j=0;j<34;j++)
		{
			transfer_data(0x07);
			transfer_data(0xe0);
			transfer_data(0xf8);
			transfer_data(0x1f);
		}
		line+=2;
	}
}
void display_snow2()
{
	u16 i,j,line=0;
	for(i=0;i<80;i++)
	{
		set_lcd_address(line,0);
		for(j=0;j<34;j++)
		{
			transfer_data(0x07);
			transfer_data(0xe0);
			transfer_data(0xf8);
			transfer_data(0x1f);
		}
		set_lcd_address(line+1,0);
		for(j=0;j<34;j++)
		{
			transfer_data(0xf8);
			transfer_data(0x1f);
			transfer_data(0x07);
			transfer_data(0xe0);
		}
		line+=2;
	}
}
//===========================================================================
/*因为这款IC 原是用于彩色STN 液晶屏上的，每个像素点分为R\G\B(红绿蓝）三个点阵，
现在用的是单色屏，所以每3 个点阵还是要依照:
R4R3R2R1R0（第1 个点阵） G5G4G3G2G1G0（第二个点阵） B4B3B2B1B0（第三个点阵),
共16 位数据来表达*/
void display_3pixels(u16 mono_data)
{
	u16 color_data;
	if(mono_data&0x8000) //bit 15，12，9，6，3
		{color_data=0xf800;}
	else
		{color_data=0x0000;};
	if(mono_data&0x4000) //bit 14，11，8，5，2
		{color_data=color_data+0x07e0;}
	else;
	if(mono_data&0x2000) //bit 13，10，7，4，1
		{color_data=color_data+0x001f;}
	else;
//		transfer_data(color_data>>8); //writting 3dots
//		transfer_data(color_data);
        transfer_data16(color_data);
}
/*****************************************************************
写一个32x32 点阵的汉字或图片，
y 是行地址，x 是列地址，dp 是数据指针
*****************************************************************/
void display_char32x32(u16 y,u16 x,const u16 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 col;
	u32 mono_data;
	u16 color_data;

//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁
	
	for(i=0;i<32;i++)	//32行
	{
		col=x;
		row=y+i;
		set_lcd_address(row,col);
		mono_data=0x00000000+*dp;
		dp++;
        //zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
		/*mono_data=(mono_data<<=8)+*dp;
		dp++;
		mono_data=(mono_data<<=8)+*dp;
		dp++;*/
		if(f)
			{
			mono_data=mono_data ^ 0xffffffff;
			}
		for(j=0;j<10;j++) //10 次*3 个点阵=前30 个点阵
		{
			if(mono_data&0x80000000) //
				{color_data=0xf800;}
			else
				{color_data=0x0000;};
			if(mono_data&0x40000000) //
				{color_data=color_data+0x07e0;}
			else;
			if(mono_data&0x20000000) //
				{color_data=color_data+0x001f;}
			else;
				//		transfer_data(color_data>>8); //writting 3dots
                //		transfer_data(color_data);
                        transfer_data16(color_data);
			
			mono_data=mono_data<<3;
		}
		//最后两个点
		if(mono_data&0x80000000) //bit 1
			{color_data=0xf800;}
		else
			{color_data=0x0000;};
		if(mono_data&0x40000000) //bit 0
			{color_data=color_data+0x07e0;}
		else;
//		transfer_data(color_data>>8); //writting 3dots
//		transfer_data(color_data);
        transfer_data16(color_data);
		
		col+=10;
		
	}
}

///*****************************************************************
//显示最多四个字符的字符串(16x32 点阵的汉字或图片序列)
//y 是行地址，x 是列地址，s 是InfoDispStrSet里字串序号
//*****************************************************************/
//void display_Str16x32(u16 y,u16 x,u8 s,u16 f)
//{
//	if(InfoDispStrSet[s][0]) 
//	{
//		display_char16x32(y,x,InfoDispStrSet[s][0],f);
//		if(InfoDispStrSet[s][1])
//		{
//			display_char16x32(y,1*5+x,InfoDispStrSet[s][1],f);
//			if(InfoDispStrSet[s][2]) 
//			{
//				display_char16x32(y,2*5+x,InfoDispStrSet[s][2],f);
//				if(InfoDispStrSet[s][3]) 
//				{
//					display_char16x32(y,2*5+x,InfoDispStrSet[s][3],f);
//				}
//			}
//		}
//	}
//}
/*****************************************************************
写一个16x32 点阵的汉字或图片，
y 是行地址，x 是列地址，dp 是数据指针
*****************************************************************/
void display_char16x32(u16 y,u16 x,const u16 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 col;
	u16 mono_data;
	
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁
	
	for(i=0;i<32;i++)	//行
	{
		col=x;
		row=y+i;
		set_lcd_address(row,col);
		mono_data=0x0000+*dp;
		dp++;
        //zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
		if(f)
			{
			mono_data=mono_data ^ 0xffff;
			}
		for(j=0;j<5;j++) //5 次*3 个点阵=15 个点阵
		{
			display_3pixels(mono_data);
			mono_data=mono_data<<3;
		}
	}
}


/*写一个15x16 点阵的汉字或图片，因为此液晶驱动IC 是每3 列是一个地址，所以显示格式是15 列（3X5=15）*/
/*y 是行地址，x 是列地址，dp 是数据指针*/
/*****************************************************************
写一个16x16 点阵的汉字或图片，
(写一个15x16 点阵的汉字或图片，因为此液晶驱动IC 是每3 列是一个地址，所以显示格式是15 列（3X5=15）)

y 是行地址，x 是列地址，dp 是数据指针
*****************************************************************/
void display_char16x16(u16 y,u16 x,const u16 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 mono_data;
	
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁

	for(i=0;i<16;i++)
	{
		row=y;
		row=row+i;
		set_lcd_address(row,x);
		mono_data=0x0000+*dp;
		dp++;
//zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
		for(j=0;j<5;j++) //5 次*3 个点阵=15 个点阵
		{
			if(f)
				{
				display_3pixels(mono_data ^ 0xffff);
				}
			else
				{
				display_3pixels(mono_data);
				}
			mono_data=mono_data<<3;
		}
	}
}

void display1_char16x16(u16 y,u16 x,const u16 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 mono_data;
	
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁

	for(i=0;i<16;i++)
	{
		row=y;
		row=row+i;
		set_lcd_address(row,x);
		mono_data=0x0000+*dp;
		dp++;
//zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
		for(j=0;j<5;j++) //5 次*3 个点阵=15 个点阵
		{
			if(f)
				{
				display_3pixels(mono_data ^ 0xffff);
				}
			else
				{
				display_3pixels(mono_data);
				}
			mono_data=mono_data<<3;
		}
	}
}
/*****************************************************************
写一个14x14 点阵的汉字或图片，
y 是行地址，x 是列地址，dp 是数据指针
*****************************************************************/
void display_char14x14(u16 y,u16 x,const u8 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 col;
	u16 mono_data;
	
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁
	
	for(i=0;i<14;i++)	//行
	{
		col=x;
		row=y+i;
		set_lcd_address(row,col);
		mono_data=0x0000+*dp;
		dp++;
        //zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
		if(f)
			{
			mono_data=mono_data ^ 0xffff;
			}
		for(j=0;j<5;j++) //4次*3 个点阵=12个点阵
		{
			display_3pixels(mono_data);
			mono_data=mono_data<<3;
		}
	}
}

/*****************************************************************
写一个12x12 点阵的汉字或图片，
y 是行地址，x 是列地址，dp 是数据指针
*****************************************************************/
void display_char12x12(u16 y,u16 x,const u16 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 col;
	u16 mono_data;
	
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁
	
	for(i=0;i<12;i++)	//行
	{
		col=x;
		row=y+i;
		set_lcd_address(row,col);
		mono_data=0x0000+*dp;
		dp++;
//zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
        
		if(f)
			{
			mono_data=mono_data ^ 0xffff;
			}
		for(j=0;j<4;j++) //4次*3 个点阵=12个点阵
		{
			display_3pixels(mono_data);
			mono_data=mono_data<<3;
		}
	}
}

/*****************************************************************
写一个8x9 点阵的汉字或图片，
y 是行地址，x 是列地址，dp 是数据指针
*****************************************************************/
void display_char8x9(u16 y,u16 x,const u8 *dp,u16 f)
{
	u16 i,j;
	u16 row;
	u16 col;
	u16 mono_data;
	
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁
	
	for(i=0;i<8;i++)	//行
	{
		col=x;
		row=y+i;
		set_lcd_address(row,col);
		mono_data=0x0000+*dp;
		dp++;
//zhaoxw
        mono_data <<= 8;
		mono_data += *dp;
		dp++;
        
		if(f)
			{
			mono_data=mono_data ^ 0xffff;
			}
		for(j=0;j<3;j++) //4次*3 个点阵=12个点阵
		{
			display_3pixels(mono_data);
			mono_data=mono_data<<3;
		}
	}
}
/**********************************************************
写一个8x16 点阵的数字或图片，因为此液晶驱动IC 是每3 列是一个地址，所以显示格式是9 列（3X3=9）
y 是行地址，x 是列地址，dp 是数据指针
**********************************************************/
void display_char8x16(u16 y,u16 x,u8 const *dp ,u16 f)
{
	u16 i,j,row,mono_data;
	char char_address;
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁
	while(*dp)
	{
		char_address=*dp-32;
		for(i=0;i<16;i++)
		{
			row=y;
			row=row+i;
			set_lcd_address(row,x);
			mono_data=ascii_code_8x16[char_address][i];
			mono_data <<= 8;
			for(j=0;j<3;j++)// //3 次*3 个点阵=9 个点阵
			{
				if(f)
					{
					display_3pixels(mono_data ^ 0xffff);
					}
				else
					{
					display_3pixels(mono_data);
					}
				mono_data=mono_data<<3;
			}
		}
		dp++;
		x+=3;
	}
}

void display_char8x14(u16 y,u16 x,u8 bcd ,u16 f)
{
	u16 i,j,row,mono_data;
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁

    for(i=0;i<14;i++)
    {
        row=y;
        row=row+i;
        set_lcd_address(row,x);
        mono_data=Num_8x14[bcd][i];
        mono_data <<= 8;
        for(j=0;j<3;j++)// //3 次*3 个点阵=9 个点阵
        {
            if(f)
                {
                display_3pixels(mono_data ^ 0xffff);
                }
            else
                {
                display_3pixels(mono_data);
                }
            mono_data=mono_data<<3;
        }
    }	
}

void displaySymbol_8x16(u16 y,u16 x,u8 index,u16 f)
{
	u16 i,j,row,mono_data;
//	if(EnSelFlsh==0) f=0;									//使能反白显示闪烁//是否使能被选项闪烁

    for(i=0;i<16;i++)
    {
        row=y;
        row=row+i;
        set_lcd_address(row,x);
        mono_data=Symbol_8x16[index][i];
        mono_data <<= 8;
        for(j=0;j<3;j++)// //3 次*3 个点阵=9 个点阵
        {
            if(f)
                {
                display_3pixels(mono_data ^ 0xffff);
                }
            else
                {
                display_3pixels(mono_data);
                }
            mono_data=mono_data<<3;
        }
    }	
}
/**************************************************************

**************************************************************/
void dispayNum8x16(u16 y,u16 x,u8 bcd ,u16 f)
{
	switch(bcd & 0x0f)
		{
		case 0:
			display_char8x16(y,x,"0" ,f);
			break;
		case 1:
			display_char8x16(y,x,"1" ,f);
			break;
		case 2:
			display_char8x16(y,x,"2" ,f);
			break;
		case 3:
			display_char8x16(y,x,"3" ,f);
			break;
		case 4:
			display_char8x16(y,x,"4" ,f);
			break;
		case 5:
			display_char8x16(y,x,"5" ,f);
			break;
		case 6:
			display_char8x16(y,x,"6" ,f);
			break;
		case 7:
			display_char8x16(y,x,"7" ,f);
			break;
		case 8:
			display_char8x16(y,x,"8" ,f);
			break;
		case 9:
			display_char8x16(y,x,"9" ,f);
			break;
		default:
			display_char8x16(y,x," " ,f);
			break;
		}
}

void dispayNum8x14(u16 y,u16 x,u8 bcd ,u16 f)
{
    display_char8x14(y,x,bcd,f);
}

void dispay4Num8x14(u16 y,u16 x,u16 num ,u16 f)
{
    hextobcd(num);

	if(BCD_buffer[3])
	{
		display_char8x14(y,x,BCD_buffer[3],f);//
	}
	else
	{
		display_char8x14(y,x,10,f);//
	}
    
	if(BCD_buffer[3] || BCD_buffer[2])
	{
		display_char8x14(y,1*3+x,BCD_buffer[2],f);//
	}
	else
	{
		display_char8x14(y,1*3+x,10,f);//
	}
	if(BCD_buffer[3] || BCD_buffer[2]|| BCD_buffer[1])
	{
		display_char8x14(y,2*3+x,BCD_buffer[1],f);//
	}
	else
	{
		display_char8x14(y,2*3+x,10,f);//
	}
	display_char8x14(y,3*3+x,BCD_buffer[0],f);//-2(前移)
}

void dispay3Num8x14(u16 y,u16 x,u16 num ,u16 f)
{
    hextobcd(num);

	if(BCD_buffer[3])
	{
		display_char8x14(y,x,BCD_buffer[3],f);//
	}
	else
	{
		//display_char8x14(y,x,10,f);//
	}
    
	if(BCD_buffer[3] || BCD_buffer[2])
	{
		display_char8x14(y,1*3+x,BCD_buffer[2],f);//
	}
	else
	{
		display_char8x14(y,1*3+x,10,f);//
	}
	if(BCD_buffer[3] || BCD_buffer[2]|| BCD_buffer[1])
	{
		display_char8x14(y,2*3+x,BCD_buffer[1],f);//
	}
	else
	{
		display_char8x14(y,2*3+x,10,f);//
	}
	display_char8x14(y,3*3+x,BCD_buffer[0],f);//-2(前移)
}

/**************************************************************
显示一个0~9
**************************************************************/
void dispayNum16x16(u16 y,u16 x,u8 bcd ,u16 f)
{
	switch(bcd & 0x0f)
		{
		case 0:
			display_char16x16(y,x,Chr0_15x16 ,f);
			break;
		case 1:
			display_char16x16(y,x,Chr1_15x16 ,f);
			break;
		case 2:
			display_char16x16(y,x,Chr2_15x16 ,f);
			break;
		case 3:
			display_char16x16(y,x,Chr3_15x16 ,f);
			break;
		case 4:
			display_char16x16(y,x,Chr4_15x16 ,f);
			break;
		case 5:
			display_char16x16(y,x,Chr5_15x16 ,f);
			break;
		case 6:
			display_char16x16(y,x,Chr6_15x16 ,f);
			break;
		case 7:
			display_char16x16(y,x,Chr7_15x16 ,f);
			break;
		case 8:
			display_char16x16(y,x,Chr8_15x16 ,f);
			break;
		case 9:
			display_char16x16(y,x,Chr9_15x16 ,f);
			break;
		default:
//			display_char16x16(y,x,Sp32x32 ,f);
			break;
		}
}

/**************************************************************
显示一个0~9
**************************************************************/
void dispayNum32x32(u16 y,u16 x,u8 bcd ,u16 f)
{
	switch(bcd & 0x0f)
		{
		case 0:
			display_char32x32(y,x,Chr0_32x32,f);
			break;
		case 1:
			display_char32x32(y,x,Chr1_32x32 ,f);
			break;
		case 2:
			display_char32x32(y,x,Chr2_32x32 ,f);
			break;
		case 3:
			display_char32x32(y,x,Chr3_32x32 ,f);
			break;
		case 4:
			display_char32x32(y,x,Chr4_32x32 ,f);
			break;
		case 5:
			display_char32x32(y,x,Chr5_32x32 ,f);
			break;
		case 6:
			display_char32x32(y,x,Chr6_32x32 ,f);
			break;
		case 7:
			display_char32x32(y,x,Chr7_32x32 ,f);
			break;
		case 8:
			display_char32x32(y,x,Chr8_32x32 ,f);
			break;
		case 9:
			display_char32x32(y,x,Chr9_32x32 ,f);
			break;
		default:
//			display_char32x32(y,x,Sp32x32 ,f);
			break;
		}
}

/**************************************************************
显示一个0~9
**************************************************************/
void dispayNum16x32(u16 y,u16 x,u8 bcd ,u16 f)
{
	switch(bcd & 0x0f)
		{
		case 0:
			display_char16x32(y,x,Chr0_16x32,f);
			break;
		case 1:
			display_char16x32(y,x,Chr1_16x32 ,f);
			break;
		case 2:
			display_char16x32(y,x,Chr2_16x32 ,f);
			break;
		case 3:
			display_char16x32(y,x,Chr3_16x32 ,f);
			break;
		case 4:
			display_char16x32(y,x,Chr4_16x32 ,f);
			break;
		case 5:
			display_char16x32(y,x,Chr5_16x32 ,f);
			break;
		case 6:
			display_char16x32(y,x,Chr6_16x32 ,f);
			break;
		case 7:
			display_char16x32(y,x,Chr7_16x32 ,f);
			break;
		case 8:
			display_char16x32(y,x,Chr8_16x32 ,f);
			break;
		case 9:
			display_char16x32(y,x,Chr9_16x32 ,f);
			break;
		default:
//			display_char16x32(y,x,Sp32x32 ,f);
			break;
		}
}

/***************************************************
显示三位十进制数(0~100,32x32,格式:xxx%)(左边0不显示)

**************************************************/
void display_Num_32x32_LZND(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
	if(BCD_buffer[4])
	{
		dispayNum32x32(y,x,BCD_buffer[4],f);
		dispayNum32x32(y,1*10+x,BCD_buffer[3],f);
		dispayNum32x32(y,2*10+x,BCD_buffer[2],f);
		display_char16x32(y,3*10+x,ChrPoin_16x32,f);
		dispayNum32x32(y,4*10+x-6,BCD_buffer[1],f);
		display_char16x32(y,5*10+x-6,ChrPer_16x32,f);//%
	}
	else if((BCD_buffer[4]==0)&&(BCD_buffer[3]==0))
	{
		display_char32x32(y,0*10+x,Sp32x32,f);				    //	清痕迹
		dispayNum32x32(y,1*10+x,BCD_buffer[2],f);
		display_char16x32(y,2*10+x,ChrPoin_16x32,f);
		dispayNum32x32(y,3*10+x-6,BCD_buffer[1],f);
		display_char32x32(y,4*10+x-6,Sp32x32,f);				//	清痕迹
		display_char32x32(y,5*10+x-6,Sp32x32,f);				//	清痕迹
		display_char16x32(y,5*10+x-12,ChrPer_16x32,f);//%
	}
	else
	{
		display_char32x32(y,0*10+x,Sp32x32,f);				    //	清痕迹
		display_char32x32(y,4*10+x-6,Sp32x32,f);				//	清痕迹
		dispayNum32x32(y,0*10+x+6,BCD_buffer[3],f);
		dispayNum32x32(y,1*10+x+6,BCD_buffer[2],f);
		display_char16x32(y,2*10+x+6,ChrPoin_16x32,f);
		dispayNum32x32(y,3*10+x-6+6,BCD_buffer[1],f);
		display_char16x32(y,5*10+x-6,ChrPer_16x32,f);//%
	}
	
}

/***************************************************
显示三位十进制数(000~100,32x32,格式:xxx%)

**************************************************/
void display_Num_32x32(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
	dispayNum32x32(y,x,BCD_buffer[3],f);//
	dispayNum32x32(y,1*10+x,BCD_buffer[2],f);//
	dispayNum32x32(y,2*10+x,BCD_buffer[1],f);//
//	display_char32x32(y,3*10+x,ChrPoin_32x32,f);//
//	dispayNum32x32(y,4*10+x-2,BCD_buffer[0],f);//-2(前移)
	display_char32x32(y,3*10+x,ChrPer_32x32,f);//
}

/***************************************************
显示三位十进制数(000~100,16x32,格式:xxx%)

**************************************************/
void display_Num_16x32(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
	dispayNum16x32(y,x,BCD_buffer[3],f);//
	dispayNum16x32(y,1*5+x,BCD_buffer[2],f);//
	dispayNum16x32(y,2*5+x,BCD_buffer[1],f);//
//	display_char16x32(y,3*5+x,ChrPoin_16x32,f);//
//	dispayNum16x32(y,4*5+x-2,BCD_buffer[0],f);//-2(前移)
	display_char16x32(y,3*5+x,ChrPer_16x32,f);//
}

/***************************************************
显示四位十进制数(16x16,格式:xxx.x)

**************************************************/
void display_Num_16x16(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
	dispayNum16x16(y,x,BCD_buffer[3],f);//
	dispayNum16x16(y,1*5+x,BCD_buffer[2],f);//
	dispayNum16x16(y,2*5+x,BCD_buffer[1],f);//
	display_char16x16(y,3*5+x,ChrPoin_15x16,f);//
	dispayNum16x16(y,4*5+x-2,BCD_buffer[0],f);//-2(前移)
}

/***************************************************
显示四位十进制数(8x16,格式:xxx.x)

**************************************************/
void display_Num_8x16(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
	if(BCD_buffer[3])
	{
		dispayNum8x16(y,x,BCD_buffer[3],f);//
	}
	else
	{
		display_char8x16(y,x," ",f);//
	}
	if(BCD_buffer[3] || BCD_buffer[2])
	{
		dispayNum8x16(y,1*3+x,BCD_buffer[2],f);//
	}
	else
	{
		display_char8x16(y,1*3+x," ",f);//
	}
	dispayNum8x16(y,2*3+x,BCD_buffer[1],f);//
	display_char8x16(y,3*3+x,".",f);//
	dispayNum8x16(y,4*3+x-2,BCD_buffer[0],f);//-2(前移)
}
/***************************************************
显示四位十进制数(8x16,格式:xxxx)

**************************************************/
void display_4Num_8x16(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
    
//    dispayNum8x16(y,x,BCD_buffer[3],f);//
//	dispayNum8x16(y,1*3+x,BCD_buffer[2],f);//
//	dispayNum8x16(y,2*3+x,BCD_buffer[1],f);//
////	display_char8x16(y,3*3+x,".",f);//
//	dispayNum8x16(y,3*3+x,BCD_buffer[0],f);//-2(前移)
    
	if(BCD_buffer[3])
	{
		dispayNum8x16(y,x,BCD_buffer[3],f);//
	}
	else
	{
		display_char8x16(y,x," ",f);//
	}
    
	if(BCD_buffer[3] || BCD_buffer[2])
	{
		dispayNum8x16(y,1*3+x,BCD_buffer[2],f);//
	}
	else
	{
		display_char8x16(y,1*3+x," ",f);//
	}
	if(BCD_buffer[3] || BCD_buffer[2]|| BCD_buffer[1])
	{
		dispayNum8x16(y,2*3+x,BCD_buffer[1],f);//
	}
	else
	{
		display_char8x16(y,2*3+x," ",f);//
	}
	dispayNum8x16(y,3*3+x,BCD_buffer[0],f);//-2(前移)
}


/***************************************************
显示2位十进制数(8x16,格式:xxxx)

**************************************************/
void display_2Num_8x16(u16 y,u16 x,u32 num ,u16 f)
{
	hextobcd(num);
    display_char8x16(y,3+x,"   ",f);//
	dispayNum8x16(y,1*3+x,BCD_buffer[1],f);//
//	display_char8x16(y,3*3+x,".",f);//
	dispayNum8x16(y,2*3+x,BCD_buffer[0],f);//-2(前移)
    
	if(BCD_buffer[1])
	{
		dispayNum8x16(y,1*3+x,BCD_buffer[1],f);//
	}
	else
	{
		display_char8x16(y,1*3+x," ",f);//
	}
}

//============================================
//hex to bcd 转换程序
//最多支持6位BCD输出（溢出时得EEEEEE）
//使用方法：
//    ihexs = 59999;
//    hextobcd(ihexs);
//结果放于BCD_buffer[]
//============================================
void hextobcd(u32 hexs) 
{
u32 va;
u8 i;
    if (hexs <= 0xf423f)                             //最大数级999 999
    {
   	 va = 100000;                         		    //最大数级万位
   	 for   (i=5; i; i--) 
	 {
      		  BCD_buffer[i] = 0;               	  //目标数组清零
      		  while ((hexs>=va)&&(va>9)) 
		{
         	 	 hexs -= va;                     		//减除数
           		 BCD_buffer[i]++;            	   //商位加1
     		 }
       	 va /= 10;                          	//除数除10，指向低一位
   	 }
   	 BCD_buffer[0]=hexs;                    //最后个位数
    }
    else
    {
    	for (i=5;i;i--)						//hexs>最大数级0xf423f,则显示999999//EEEEEE
     	{
       	 BCD_buffer[i] = 0x09;                 //目标数组单元为
     	}
     	BCD_buffer[0] = 0x09;                 //目标数组低单元改为0x0e
    }
    	
}
/*********************************************************
function name: 4位BCD转换 十进制

fun :		
*********************************************************/
u32 BCDtoDec(const u8 *bcd)
{
	u32 dec;
	dec=bcd[3]*10*10*10+bcd[2]*10*10+bcd[1]*10+bcd[0];
/*	for(i=5;i=1;i--)
	{
		if(i>=2)
		{
			dec=(dec+bcd[i])*10;	
		}
		else
		{
			dec=dec+bcd[i];
		}
	}*/
	return dec;
}



//===============main program===================
void testLCD(void)
{
//	clear_screen();
	full_screen();
//	LCD_Back=0;
	
	
}









