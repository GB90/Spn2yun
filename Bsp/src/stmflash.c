#include "Def.h"
#include "stmflash.h"
 

/*********************************************************
function name: ��ȡָ����ַ�İ���(16λ����)

fun	:		faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
			����ֵ:��Ӧ����.	
*********************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   


/*********************************************************
function name: ������д��

fun	:		WriteAddr:��ʼ��ַ
			pBuffer:����ָ��
			NumToWrite:����(16λ)��   	
*********************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 


/*********************************************************
function name: ��ָ����ַ��ʼд��ָ�����ȵ�����

fun	:		WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
			pBuffer:����ָ��
			NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
*********************************************************/
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
    u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
    
    i = *pBuffer;
    STMFLASH_Read(WriteAddr,&secoff,1);			//�Ƿ���Ҫд�� zhaoxw
    if(secoff == i)
    {
        return;
    }
   
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						    //����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif


/*********************************************************
function name: ��ָ����ַ��ʼ����ָ�����ȵ�����

fun	:		ReadAddr:��ʼ��ַ
			pBuffer:����ָ��
			NumToWrite:����(16λ)��
*********************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}


/*********************************************************
function name: ��ָ����ַ��ʼд����

fun	:		WriteAddr:��ʼ��ַ
			WriteData:Ҫд�������
*********************************************************/
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}

/*********************************************************
function name: ��������

fun	:		
*********************************************************/
void SaveDataToFlash(void)
{
    u16 i;
    MYU32 Wdata;
    
	DIS_INT;
	STMFLASH_Write(FLASH_SAVE_ADDR + 0,&isZero,1);			    //�����c
    
    Wdata.Myu32 = PosLmdU;
	STMFLASH_Write(FLASH_SAVE_ADDR + 2,&Wdata.Myu16.Myu16H,1);  //��λ����
	STMFLASH_Write(FLASH_SAVE_ADDR + 4,&Wdata.Myu16.Myu16L,1);  //��λ����
    
	STMFLASH_Write(FLASH_SAVE_ADDR + 6,&CInLmdU,1);			    //����C����
	STMFLASH_Write(FLASH_SAVE_ADDR + 8,&CInLmdD,1);			    //����C����
	STMFLASH_Write(FLASH_SAVE_ADDR + 10,&VInLmdU,1);			//����V����
	STMFLASH_Write(FLASH_SAVE_ADDR + 12,&VInLmdD,1);			//����V����
	STMFLASH_Write(FLASH_SAVE_ADDR + 14,&OutCLmdU,1);			//�������

	IWDG_feed();		//ι��
	
	STMFLASH_Write(FLASH_SAVE_ADDR + 16,&OutCLmdD,1);			//�������
	STMFLASH_Write(FLASH_SAVE_ADDR + 18,&OutVLmdU,1);			//�������
	STMFLASH_Write(FLASH_SAVE_ADDR + 20,&OutVLmdD,1);			//�������
	STMFLASH_Write(FLASH_SAVE_ADDR + 22,&PosAccuracy,1);		//��λ����
	STMFLASH_Write(FLASH_SAVE_ADDR + 24,&JogHolAutoSel,1);		//�ֳ����Ʒ�ʽ:	1�㶯,2����,3�Զ�
	STMFLASH_Write(FLASH_SAVE_ADDR + 26,&A_DInSel,1);			//���뷽ʽ:ģ����,������
	STMFLASH_Write(FLASH_SAVE_ADDR + 28,&NoSignalCtrSel,1);		//���Ŵ���ʽ
	STMFLASH_Write(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);			//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
	STMFLASH_Write(FLASH_SAVE_ADDR + 32,&ScrSel,1);				//��ʾ����
	STMFLASH_Write(FLASH_SAVE_ADDR + 34,&OutCVSel,1);			//����������ѹѡ��
	STMFLASH_Write(FLASH_SAVE_ADDR + 36,&PassWord,1);			//����

	IWDG_feed();		//ι��
	
	for(i=0; i<9; i++)
	{
		STMFLASH_Write(FLASH_SAVE_ADDR + 38 + i*2,&RelayType[i],1);		//ͨ������
	}
	STMFLASH_Write(FLASH_SAVE_ADDR + 58,&Intermediate1,1);      //Զ�����뷽ʽ:	0ģ����,1������
	STMFLASH_Write(FLASH_SAVE_ADDR + 60,&Intermediate2,1); 		//���Ŵ���ʽ	1ֹͣ��2ȫ����3ȫ��
	STMFLASH_Write(FLASH_SAVE_ADDR + 62,&language,1); 			//����ѡ��		0���� ��0Ӣ��
	STMFLASH_Write(FLASH_SAVE_ADDR + 64,&electronicbrake,1);    //����ɲ��������־	
	STMFLASH_Write(FLASH_SAVE_ADDR + 66,&SetCloseDir,1);        //�رշ���			1��   0��
	STMFLASH_Write(FLASH_SAVE_ADDR + 68,&Closeway,1); 			//Closeway, ��0 ����   0 ��λ  �жϵ����λ�Ϳ�λ������ ���ؿ��ػ���λ����
	STMFLASH_Write(FLASH_SAVE_ADDR + 70,&ESDset,1); 			//ESD���ã�2��λ��3��λ��1ֹͣ
	STMFLASH_Write(FLASH_SAVE_ADDR + 72,&TwoLine,1); 		 	//���߿���	  1��ֹ  2���źſ�  3���źŹ�	  	
	STMFLASH_Write(FLASH_SAVE_ADDR + 74,&LockedRotorTime,1); 	//��תʱ��
	STMFLASH_Write(FLASH_SAVE_ADDR + 76,&IntermittentRun,1);    //�������	(��0��������У�0��)
    STMFLASH_Write(FLASH_SAVE_ADDR + 78,&lock,1);
    
    Wdata.Myu32 = PosLmdD;
	STMFLASH_Write(FLASH_SAVE_ADDR + 80,&Wdata.Myu16.Myu16H,1);			
	STMFLASH_Write(FLASH_SAVE_ADDR + 82,&Wdata.Myu16.Myu16L,1);
    
    STMFLASH_Write(FLASH_SAVE_ADDR + 84,&Rs485Way,1);
    STMFLASH_Write(FLASH_SAVE_ADDR + 86,&mbaddr,1);
    STMFLASH_Write(FLASH_SAVE_ADDR + 88,&PhaseDir,1);
	IWDG_feed();		//ι��
    
	i =0x5a;
	STMFLASH_Write(FLASH_SAVE_ADDR + 98,&i,1);//�洢����־

	EN_INT;
}

/*********************************************************
function name: ������

fun	:		
*********************************************************/
void GetDataFromFlash(void)
{
    u16 i;
    MYU32 Rdata;
    
	STMFLASH_Read(FLASH_SAVE_ADDR + 98,&i,1);//�洢����־
	if(i == 0x5a)
	{
		STMFLASH_Read(FLASH_SAVE_ADDR + 0,&isZero,1);		//�Զ��궨��
//		STMFLASH_Read(FLASH_SAVE_ADDR + 2,&PosLmdU,1);			//��λ����
//		STMFLASH_Read(FLASH_SAVE_ADDR + 4,&PosLmdD,1);			//��λ����
            
        STMFLASH_Read(FLASH_SAVE_ADDR + 2,&Rdata.Myu16.Myu16H,1);			    //��λ����
        STMFLASH_Read(FLASH_SAVE_ADDR + 4,&Rdata.Myu16.Myu16L,1);			    //��λ����
        PosLmdU = Rdata.Myu32;
        
		STMFLASH_Read(FLASH_SAVE_ADDR + 6,&CInLmdU,1);			//����C����
		STMFLASH_Read(FLASH_SAVE_ADDR + 8,&CInLmdD,1);			//����C����
		STMFLASH_Read(FLASH_SAVE_ADDR + 10,&VInLmdU,1);			//����V����
		STMFLASH_Read(FLASH_SAVE_ADDR + 12,&VInLmdD,1);			//����V����
		STMFLASH_Read(FLASH_SAVE_ADDR + 14,&OutCLmdU,1);		//�������
		STMFLASH_Read(FLASH_SAVE_ADDR + 16,&OutCLmdD,1);		//�������
		STMFLASH_Read(FLASH_SAVE_ADDR + 18,&OutVLmdU,1);		//�������
		STMFLASH_Read(FLASH_SAVE_ADDR + 20,&OutVLmdD,1);		//�������
		STMFLASH_Read(FLASH_SAVE_ADDR + 22,&PosAccuracy,1);		//��λ����
		STMFLASH_Read(FLASH_SAVE_ADDR + 24,&JogHolAutoSel,1);	//ִ�з�ʽ:�㶯,����,�Զ�
		STMFLASH_Read(FLASH_SAVE_ADDR + 26,&A_DInSel,1);		//���뷽ʽ:ģ����,������
		STMFLASH_Read(FLASH_SAVE_ADDR + 28,&NoSignalCtrSel,1);	//���Ŵ���ʽ
		STMFLASH_Read(FLASH_SAVE_ADDR + 30,&RmLcDisSel,1);		//Զ��0/����(���ؼ��̲���)1/��ֹ>=2�л�
		STMFLASH_Read(FLASH_SAVE_ADDR + 32,&ScrSel,1);			//��ʾ����
		STMFLASH_Read(FLASH_SAVE_ADDR + 34,&OutCVSel,1);		//����������ѹѡ��
		STMFLASH_Read(FLASH_SAVE_ADDR + 36,&PassWord,1);		//����
		IWDG_feed();		//ι��
		for(i=0; i<9; i++)
		{
			STMFLASH_Read(FLASH_SAVE_ADDR + 38 + i*2,&RelayType[i],1);		//ͨ������
		}
		STMFLASH_Read(FLASH_SAVE_ADDR + 58,&Intermediate1,1); 			//Զ�����뷽ʽ:	0ģ����,1������
		STMFLASH_Read(FLASH_SAVE_ADDR + 60,&Intermediate2,1); 		//���Ŵ���ʽ	1ֹͣ��2ȫ����3ȫ��
		STMFLASH_Read(FLASH_SAVE_ADDR + 62,&language,1); 			//����ѡ��		0���� ��0Ӣ��
		STMFLASH_Read(FLASH_SAVE_ADDR + 64,&electronicbrake,1); 	//����ɲ��������־	
		STMFLASH_Read(FLASH_SAVE_ADDR + 66,&SetCloseDir,1); 		//�رշ���		1��   0��
		STMFLASH_Read(FLASH_SAVE_ADDR + 68,&Closeway,1); 			//Closeway, ��0 ����   0 ��λ  �жϵ����λ�Ϳ�λ������ ���ؿ��ػ���λ����
		STMFLASH_Read(FLASH_SAVE_ADDR + 70,&ESDset,1); 				//ESD���ã�2��λ��3��λ��1ֹͣ
		STMFLASH_Read(FLASH_SAVE_ADDR + 72,&TwoLine,1); 		 	//���߿���	  1��ֹ  2���źſ�  3���źŹ�	  	
		STMFLASH_Read(FLASH_SAVE_ADDR + 74,&LockedRotorTime,1); 	//��תʱ��
		STMFLASH_Read(FLASH_SAVE_ADDR + 76,&IntermittentRun,1);		//�������	(��0��������У�0��)
        STMFLASH_Read(FLASH_SAVE_ADDR + 78,&lock,1);
        
        STMFLASH_Read(FLASH_SAVE_ADDR + 80,&Rdata.Myu16.Myu16H,1);			    //��λ����
        STMFLASH_Read(FLASH_SAVE_ADDR + 82,&Rdata.Myu16.Myu16L,1);			    //��λ����
        PosLmdD = Rdata.Myu32;
        
        STMFLASH_Read(FLASH_SAVE_ADDR + 84,&Rs485Way,1);
        STMFLASH_Read(FLASH_SAVE_ADDR + 86,&mbaddr,1);
        STMFLASH_Read(FLASH_SAVE_ADDR + 88,&PhaseDir,1);
	}
    else
    {
        //���������趨��
        //AutoSetOKF=0;				//δ�궨��
        isZero = 127;
        PosLmdU=3992;				//��λ����		����4095��ʵ�ʻ�С��
        PosLmdD=38;				    //��λ����
        CInLmdU=3707;			    //����C����
        CInLmdD=746;			    //����C����
        VInLmdU=4095;				//����V����
        VInLmdD=0;				    //����V����
        OutCLmdU=3375;				//�������
        OutCLmdD=669; 				//�������
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
        PassWord = 0000; 			//����(Ĭ��Ϊ"0000"),�޶�С�ڵ���9999
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
        SetCloseDir=1;			    //�رշ���	1�� 0��
        Closeway=0;					//Closeway, ��0 ����   0 ��λ  �жϵ����λ�Ϳ�λ������ ���ؿ��ػ���λ����
        Rs485Way=0;                 //1�����߿�����Ч��д��Ч 0�����߿�����Ч��д��Ч
        ESDset=1;					//ESD���ã�2��λ��3��λ��1ֹͣ
        mbaddr = 1;
        TwoLine=1;					//���߿���	  1��ֹ  2���źſ�  3���źŹ�	  
        LockedRotorTime=4;			//��תʱ��		0- 10S
        IntermittentRun=0;			//�������	(��0��������У�0��)
        Intermediate1=25;           //�м�λ��1 0%~�м�λ��1
        Intermediate2=75;
		lock=0;                     //Ĭ�ϲ��������
        PhaseDir = 0;
        SaveDataToFlash();
    }

}



