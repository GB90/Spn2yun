#include "Def.h"
#include "stmflash.h"
 

/*********************************************************
function name: 读取指定地址的半字(16位数据)

fun	:		faddr:读地址(此地址必须为2的倍数!!)
			返回值:对应数据.	
*********************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   


/*********************************************************
function name: 不检查的写入

fun	:		WriteAddr:起始地址
			pBuffer:数据指针
			NumToWrite:半字(16位)数   	
*********************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 


/*********************************************************
function name: 从指定地址开始写入指定长度的数据

fun	:		WriteAddr:起始地址(此地址必须为2的倍数!!)
			pBuffer:数据指针
			NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
*********************************************************/
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
    u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
    
    i = *pBuffer;
    STMFLASH_Read(WriteAddr,&secoff,1);			//是否需要写入 zhaoxw
    if(secoff == i)
    {
        return;
    }
   
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();						    //解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain;	//写地址偏移	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}
#endif


/*********************************************************
function name: 从指定地址开始读出指定长度的数据

fun	:		ReadAddr:起始地址
			pBuffer:数据指针
			NumToWrite:半字(16位)数
*********************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}


/*********************************************************
function name: 从指定地址开始写数据

fun	:		WriteAddr:起始地址
			WriteData:要写入的数据
*********************************************************/
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}

/*********************************************************
function name: 保持数据

fun	:		
*********************************************************/
void SaveDataToFlash(void)
{
    u16 i;
    MYU32 Wdata;
    
	DIS_INT;
	STMFLASH_Write(FLASH_SAVE_ADDR + 0,&isZero,1);			    //过零點
    
    Wdata.Myu32 = PosLmdU;
	STMFLASH_Write(FLASH_SAVE_ADDR + 2,&Wdata.Myu16.Myu16H,1);  //阀位上限
	STMFLASH_Write(FLASH_SAVE_ADDR + 4,&Wdata.Myu16.Myu16L,1);  //阀位下限
    
	STMFLASH_Write(FLASH_SAVE_ADDR + 6,&CInLmdU,1);			    //给定C上限
	STMFLASH_Write(FLASH_SAVE_ADDR + 8,&CInLmdD,1);			    //给定C下限
	STMFLASH_Write(FLASH_SAVE_ADDR + 10,&VInLmdU,1);			//给定V上限
	STMFLASH_Write(FLASH_SAVE_ADDR + 12,&VInLmdD,1);			//给定V下限
	STMFLASH_Write(FLASH_SAVE_ADDR + 14,&OutCLmdU,1);			//输出上限

	IWDG_feed();		//喂狗
	
	STMFLASH_Write(FLASH_SAVE_ADDR + 16,&OutCLmdD,1);			//输出下限
	STMFLASH_Write(FLASH_SAVE_ADDR + 18,&OutVLmdU,1);			//输出上限
	STMFLASH_Write(FLASH_SAVE_ADDR + 20,&OutVLmdD,1);			//输出下限
	STMFLASH_Write(FLASH_SAVE_ADDR + 22,&PosAccuracy,1);		//阀位精度
	STMFLASH_Write(FLASH_SAVE_ADDR + 24,&JogHolAutoSel,1);		//现场控制方式:	1点动,2保持,3自动
	STMFLASH_Write(FLASH_SAVE_ADDR + 26,&A_DInSel,1);			//输入方式:模拟量,开关量
	STMFLASH_Write(FLASH_SAVE_ADDR + 28,&NoSignalCtrSel,1);		//丢信处理方式
	STMFLASH_Write(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);			//远程0/本地(本地键盘操作)1/禁止>=2切换
	STMFLASH_Write(FLASH_SAVE_ADDR + 32,&ScrSel,1);				//显示方向
	STMFLASH_Write(FLASH_SAVE_ADDR + 34,&OutCVSel,1);			//输出电流或电压选择
	STMFLASH_Write(FLASH_SAVE_ADDR + 36,&PassWord,1);			//密码

	IWDG_feed();		//喂狗
	
	for(i=0; i<9; i++)
	{
		STMFLASH_Write(FLASH_SAVE_ADDR + 38 + i*2,&RelayType[i],1);		//通道设置
	}
	STMFLASH_Write(FLASH_SAVE_ADDR + 58,&Intermediate1,1);      //远程输入方式:	0模拟量,1开关量
	STMFLASH_Write(FLASH_SAVE_ADDR + 60,&Intermediate2,1); 		//丢信处理方式	1停止，2全开，3全闭
	STMFLASH_Write(FLASH_SAVE_ADDR + 62,&language,1); 			//语言选择		0中文 非0英文
	STMFLASH_Write(FLASH_SAVE_ADDR + 64,&electronicbrake,1);    //电子刹车开启标志	
	STMFLASH_Write(FLASH_SAVE_ADDR + 66,&SetCloseDir,1);        //关闭方向			1正   0反
	STMFLASH_Write(FLASH_SAVE_ADDR + 68,&Closeway,1); 			//Closeway, 非0 力矩   0 限位  判断到达闭位和开位的依据 力矩开关或限位开关
	STMFLASH_Write(FLASH_SAVE_ADDR + 70,&ESDset,1); 			//ESD设置，2开位，3闭位，1停止
	STMFLASH_Write(FLASH_SAVE_ADDR + 72,&TwoLine,1); 		 	//两线控制	  1禁止  2有信号开  3有信号关	  	
	STMFLASH_Write(FLASH_SAVE_ADDR + 74,&LockedRotorTime,1); 	//堵转时间
	STMFLASH_Write(FLASH_SAVE_ADDR + 76,&IntermittentRun,1);    //间断运行	(非0开间断运行，0关)
    STMFLASH_Write(FLASH_SAVE_ADDR + 78,&lock,1);
    
    Wdata.Myu32 = PosLmdD;
	STMFLASH_Write(FLASH_SAVE_ADDR + 80,&Wdata.Myu16.Myu16H,1);			
	STMFLASH_Write(FLASH_SAVE_ADDR + 82,&Wdata.Myu16.Myu16L,1);
    
    STMFLASH_Write(FLASH_SAVE_ADDR + 84,&Rs485Way,1);
    STMFLASH_Write(FLASH_SAVE_ADDR + 86,&mbaddr,1);
    STMFLASH_Write(FLASH_SAVE_ADDR + 88,&PhaseDir,1);
	IWDG_feed();		//喂狗
    
	i =0x5a;
	STMFLASH_Write(FLASH_SAVE_ADDR + 98,&i,1);//存储过标志

	EN_INT;
}

/*********************************************************
function name: 读数据

fun	:		
*********************************************************/
void GetDataFromFlash(void)
{
    u16 i;
    MYU32 Rdata;
    
	STMFLASH_Read(FLASH_SAVE_ADDR + 98,&i,1);//存储过标志
	if(i == 0x5a)
	{
		STMFLASH_Read(FLASH_SAVE_ADDR + 0,&isZero,1);		//自动标定过
//		STMFLASH_Read(FLASH_SAVE_ADDR + 2,&PosLmdU,1);			//阀位上限
//		STMFLASH_Read(FLASH_SAVE_ADDR + 4,&PosLmdD,1);			//阀位下限
            
        STMFLASH_Read(FLASH_SAVE_ADDR + 2,&Rdata.Myu16.Myu16H,1);			    //阀位上限
        STMFLASH_Read(FLASH_SAVE_ADDR + 4,&Rdata.Myu16.Myu16L,1);			    //阀位下限
        PosLmdU = Rdata.Myu32;
        
		STMFLASH_Read(FLASH_SAVE_ADDR + 6,&CInLmdU,1);			//给定C上限
		STMFLASH_Read(FLASH_SAVE_ADDR + 8,&CInLmdD,1);			//给定C下限
		STMFLASH_Read(FLASH_SAVE_ADDR + 10,&VInLmdU,1);			//给定V上限
		STMFLASH_Read(FLASH_SAVE_ADDR + 12,&VInLmdD,1);			//给定V下限
		STMFLASH_Read(FLASH_SAVE_ADDR + 14,&OutCLmdU,1);		//输出上限
		STMFLASH_Read(FLASH_SAVE_ADDR + 16,&OutCLmdD,1);		//输出下限
		STMFLASH_Read(FLASH_SAVE_ADDR + 18,&OutVLmdU,1);		//输出上限
		STMFLASH_Read(FLASH_SAVE_ADDR + 20,&OutVLmdD,1);		//输出下限
		STMFLASH_Read(FLASH_SAVE_ADDR + 22,&PosAccuracy,1);		//阀位精度
		STMFLASH_Read(FLASH_SAVE_ADDR + 24,&JogHolAutoSel,1);	//执行方式:点动,保持,自动
		STMFLASH_Read(FLASH_SAVE_ADDR + 26,&A_DInSel,1);		//输入方式:模拟量,开关量
		STMFLASH_Read(FLASH_SAVE_ADDR + 28,&NoSignalCtrSel,1);	//丢信处理方式
		STMFLASH_Read(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);		//远程0/本地(本地键盘操作)1/禁止>=2切换
		STMFLASH_Read(FLASH_SAVE_ADDR + 32,&ScrSel,1);			//显示方向
		STMFLASH_Read(FLASH_SAVE_ADDR + 34,&OutCVSel,1);		//输出电流或电压选择
		STMFLASH_Read(FLASH_SAVE_ADDR + 36,&PassWord,1);		//密码
		IWDG_feed();		//喂狗
		for(i=0; i<9; i++)
		{
			STMFLASH_Read(FLASH_SAVE_ADDR + 38 + i*2,&RelayType[i],1);		//通道设置
		}
		STMFLASH_Read(FLASH_SAVE_ADDR + 58,&Intermediate1,1); 			//远程输入方式:	0模拟量,1开关量
		STMFLASH_Read(FLASH_SAVE_ADDR + 60,&Intermediate2,1); 		//丢信处理方式	1停止，2全开，3全闭
		STMFLASH_Read(FLASH_SAVE_ADDR + 62,&language,1); 			//语言选择		0中文 非0英文
		STMFLASH_Read(FLASH_SAVE_ADDR + 64,&electronicbrake,1); 	//电子刹车开启标志	
		STMFLASH_Read(FLASH_SAVE_ADDR + 66,&SetCloseDir,1); 		//关闭方向		1正   0反
		STMFLASH_Read(FLASH_SAVE_ADDR + 68,&Closeway,1); 			//Closeway, 非0 力矩   0 限位  判断到达闭位和开位的依据 力矩开关或限位开关
		STMFLASH_Read(FLASH_SAVE_ADDR + 70,&ESDset,1); 				//ESD设置，2开位，3闭位，1停止
		STMFLASH_Read(FLASH_SAVE_ADDR + 72,&TwoLine,1); 		 	//两线控制	  1禁止  2有信号开  3有信号关	  	
		STMFLASH_Read(FLASH_SAVE_ADDR + 74,&LockedRotorTime,1); 	//堵转时间
		STMFLASH_Read(FLASH_SAVE_ADDR + 76,&IntermittentRun,1);		//间断运行	(非0开间断运行，0关)
        STMFLASH_Read(FLASH_SAVE_ADDR + 78,&lock,1);
        
        STMFLASH_Read(FLASH_SAVE_ADDR + 80,&Rdata.Myu16.Myu16H,1);			    //阀位上限
        STMFLASH_Read(FLASH_SAVE_ADDR + 82,&Rdata.Myu16.Myu16L,1);			    //阀位下限
        PosLmdD = Rdata.Myu32;
        
        STMFLASH_Read(FLASH_SAVE_ADDR + 84,&Rs485Way,1);
        STMFLASH_Read(FLASH_SAVE_ADDR + 86,&mbaddr,1);
        STMFLASH_Read(FLASH_SAVE_ADDR + 88,&PhaseDir,1);
	}
    else
    {
        //基本参数设定类
        //AutoSetOKF=0;				//未标定过
        isZero = 127;
        PosLmdU=3992;				//阀位上限		理论4095，实际会小于
        PosLmdD=38;				    //阀位下限
        CInLmdU=3707;			    //给定C上限
        CInLmdD=746;			    //给定C下限
        VInLmdU=4095;				//给定V上限
        VInLmdD=0;				    //给定V下限
        OutCLmdU=3375;				//输出上限
        OutCLmdD=669; 				//输出下限
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
        PassWord = 0000; 			//密码(默认为"0000"),限定小于等于9999
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
        SetCloseDir=1;			    //关闭方向	1正 0反
        Closeway=0;					//Closeway, 非0 力矩   0 限位  判断到达闭位和开位的依据 力矩开关或限位开关
        Rs485Way=0;                 //1：总线控制有效读写有效 0：总线控制无效读写有效
        ESDset=1;					//ESD设置，2开位，3闭位，1停止
        mbaddr = 1;
        TwoLine=1;					//两线控制	  1禁止  2有信号开  3有信号关	  
        LockedRotorTime=4;			//堵转时间		0- 10S
        IntermittentRun=0;			//间断运行	(非0开间断运行，0关)
        Intermediate1=25;           //中间位置1 0%~中间位置1
        Intermediate2=75;
		lock=0;                     //默认不上软件锁
        PhaseDir = 0;
        SaveDataToFlash();
    }

}



