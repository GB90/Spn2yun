#include "sys.h"
#include "delay.h"
#include "usart.h"	  

MB_STATUS    MbStatus;
MB_unRICV    MbRicv;
MB_strSEND03 MbstrSEND03;
MB_strSEND06 MbstrSEND06;
MBFSM        MbFsm = R_CRC;
UMBDATA      RMbData,WMbData;
u8 Len;
u8 SendBuf[50];
u32 MbTimer;

void ModBusRun(void);
static void  ModBusFuc(void);
static void  ModBusRp(void);
static void  ModBusFuc03(void);
#if (MB_MODE == 0)
static void  ModBusFuc06(void);
#endif


static const u8 CRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40
};

static const u8 CRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
0x41, 0x81, 0x80, 0x40
};

/*********************************************************
function name: ��ʼ��IO ����1 

fun	:		bound:���õĲ�����
*********************************************************/
void uart_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1

	//USART1_485_CC   PA.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				  //PA.8�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RS485_TX_EN=0;		   					                   //Ĭ��Ϊ����ģʽ//485ģʽ����.0,����;1,����.
	
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

    //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

static void USART1_SendArray(u8 *Data, u8 len)
{
    u8 i;
    RS485_TX_EN = 1;
    delay_ms(10);
    for(i = 0; i < len; i++)
    {
        USART_SendData(USART1, Data[i]);
        while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
    }
    RS485_TX_EN = 0;
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
    static u8 Res[30],len;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res[len++] =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������

        MbRicv.DataRicv[len - 1] = Res[len - 1];
 
        if(len >= 8)
        {
            len = 0;
            if(Res[0] == mbaddr)//if(Res[0] == MB_ADDR)
            {
                MbStatus.MbRicved = 1;
                MbStatus.MbOnline = 1;
                MbTimer = 0;
            }
        }
        else
        {
            if(Res[0] != mbaddr)//if(Res[0] != MB_ADDR)
            {
                len = 0;
            }
        }
//        switch(Fsm)
//        {
//            case 0:
//                if(Res[0] == MB_ADDR)
//                {
//                    Fsm = 1;
//                    MbRicv.DataRicv[0] = Res[0];
//                }
//                else
//                {
//                    len = 0;
//                }
//                break;
//            case 1:
//                MbRicv.DataRicv[len - 1] = Res[len - 1];
//                if(len > 7)
//                {
//                    len = 0;
//                    //MbStatus.MbRicv = 1;
//                    Fsm = 0;
//                    Res[0] = 0;
//                }
//                break;
//        }
    }
} 


u16 Crc16(u8 *frame, u16 len )
{
    u8 ucCRCHi = 0xFF;
    u8 ucCRCLo = 0xFF;
    u8 *temp;
    u32 iIndex;
    
    temp = frame;
    while( len-- )
    {
        iIndex = ucCRCLo ^ *temp++;
        ucCRCLo = ( u8 )( ucCRCHi ^ CRCHi[iIndex] );
        ucCRCHi = CRCLo[iIndex];
    }

    return ( u16 )( ucCRCHi << 8 | ucCRCLo );
}

void ModBusRun(void)
{
    WB  WbTemp;
    /*****************����Ƿ����*****************/
#if (MB_LOST == 1)
    if(MbTimer == MB_TIMEOUT)
    {
        MbStatus.MbOnline = 0;
    }
#endif
    /**********************************************/
#if (MB_MODE < 2)   
    if(MbStatus.MbRicved)//���յ�һ֡
#else
    if(0)//���յ�һ֡
#endif
    {
        switch((u8)MbFsm)
        {
            case R_CRC:
                MbStatus.MbErr = 0;//����
                WbTemp.Word = Crc16(MbRicv.DataRicv,6);
                if(WbTemp.Byte.ByteH == MbRicv.MbstrRicv.CrcHi && WbTemp.Byte.ByteL == MbRicv.MbstrRicv.CrcLo)
                {
                    MbFsm = R_FUC;
                }
                else
                {
                    MbStatus.MbErr = 1;//У�����
                    MbFsm = R_RESPOND;
                }
                break;
            case R_FUC:
                if( MbRicv.MbstrRicv.FucCode == 0x03 ||\
                    MbRicv.MbstrRicv.FucCode == 0x06 )
                {
                    ModBusFuc();
                }
                else
                {
                    MbStatus.MbErr = 1;//��֧�ֵĹ����� 
                }
                MbFsm = R_RESPOND;
                break;
            case R_RESPOND:
                ModBusRp();
                MbFsm = R_END;
                break;
            case R_END:
                MbFsm = R_CRC;
                MbStatus.MbRicved = 0;
#if (MB_LOST == 1)
                MbStatus.MbOnline = 0;
#endif
                break;
            default:break;
        }
    }
}


void  ModBusDataInit(void)
{
    WMbData.sMbData.MbCloseDir = CloseDirection;
    WMbData.sMbData.MbCloseMode = Closeway;
    WMbData.sMbData.MbEsdMode = ESDset;
    WMbData.sMbData.MbLow = NoSignalCtrSel;
    WMbData.sMbData.MbRemotMode = A_DInSel;
    
    WMbData.sMbData.MbRelay[0] = (RelayType[0]);
    WMbData.sMbData.MbRelay[1] = (RelayType[1]);
    WMbData.sMbData.MbRelay[2] = (RelayType[2]);
    WMbData.sMbData.MbRelay[3] = (RelayType[3]);
    WMbData.sMbData.MbRelay[4] = (RelayType[4]);
    WMbData.sMbData.MbRelay[5] = (RelayType[5]);
    WMbData.sMbData.MbRelay[6] = (RelayType[6]);
    WMbData.sMbData.MbRelay[7] = (RelayType[7]);
    WMbData.sMbData.MbRelay[8] = (RelayType[8]);
}
static void  ModBusFuc(void)
{
    switch(MbRicv.MbstrRicv.FucCode)
    {
#if (MB_MODE < 2)
        case 0x03://��
            ModBusFuc03();
            break;
#endif
#if (MB_MODE == 0)
        case 0x06://д
            ModBusFuc06();
            MbStatus.MbInData = 1;//���Ѹ�������
        break;
#endif
        default:
            MbStatus.MbErr = 1;     //�Ƿ��Ĺ�����
        break;
    }
}

#if (MB_MODE < 2)
static void  ModBusFuc03(void)//��
{
    u8 i,len;
    WB  WbTemp0,WbTemp1;
    
    WbTemp0.Byte.ByteH = MbRicv.MbstrRicv.NumHi;
    WbTemp0.Byte.ByteL = MbRicv.MbstrRicv.NumLo;
    len = WbTemp0.Word;

    WbTemp0.Byte.ByteH = MbRicv.MbstrRicv.LocaHi;
    WbTemp0.Byte.ByteL = MbRicv.MbstrRicv.LocaLo;

    if(len == 0)
    {
        MbStatus.MbErr = 3;//��������
    }
                
    for(i = 0; i < len; i++)
    {
        WbTemp1.Word = RMbData.MbDataArray[WbTemp0.Word + i];
        
        MbstrSEND03.Data[i*2]     = WbTemp1.Byte.ByteH;
        MbstrSEND03.Data[i*2 + 1] = WbTemp1.Byte.ByteL;
        if(WbTemp0.Word + i > 21)
        {
            MbStatus.MbErr = 2;     //�Ƿ������ݵ�ַ
            break;
        }
    }
    len = len * 2;
    MbstrSEND03.Addr = MbRicv.MbstrRicv.Addr;
    MbstrSEND03.FucCode = MbRicv.MbstrRicv.FucCode;
    MbstrSEND03.Num = len;
    Len = len + 5;
    
    SendBuf[0] = MbstrSEND03.Addr;
    SendBuf[1] = MbstrSEND03.FucCode;
    SendBuf[2] = MbstrSEND03.Num;
    for(i = 0; i < MbstrSEND03.Num; i++)
    {
        SendBuf[3 + i] = MbstrSEND03.Data[i];
    }

    WbTemp1.Word = Crc16(SendBuf,len + 3);
    MbstrSEND03.CrcLo = WbTemp1.Byte.ByteL;
    MbstrSEND03.CrcHi = WbTemp1.Byte.ByteH;

    SendBuf[3 + MbstrSEND03.Num] = MbstrSEND03.CrcLo;
    SendBuf[4 + MbstrSEND03.Num] = MbstrSEND03.CrcHi;
}
#endif
#if (MB_MODE == 0)
static void  ModBusFuc06(void)//д
{
    WB  WbTemp;
     
    WbTemp.Byte.ByteH = MbRicv.MbstrRicv.LocaHi;
    WbTemp.Byte.ByteL = MbRicv.MbstrRicv.LocaLo;
    
    switch(WbTemp.Word)
    {      
        case 0x0000://������ 1��2��3ͣ
            WMbData.sMbData.MbSwitch =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0001://��λ�ٷֱ�
            WbTemp.Byte.ByteH = MbRicv.MbstrRicv.NumHi;
            WbTemp.Byte.ByteL = MbRicv.MbstrRicv.NumLo;
            WMbData.sMbData.MbVavlePer = WbTemp.Word;
            break;
        case 0x0002://�ط���
            WMbData.sMbData.MbCloseDir =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0003://��ģʽ
            WMbData.sMbData.MbCloseMode =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0004://ESD����
            WMbData.sMbData.MbEsdMode =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0005://���ź�
            WMbData.sMbData.MbLow =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0006://Զ��ģʽ
            WMbData.sMbData.MbRemotMode =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0007:
            WMbData.sMbData.MbRelay[0] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0008:
            WMbData.sMbData.MbRelay[1] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x0009:
            WMbData.sMbData.MbRelay[2] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x000A:
            WMbData.sMbData.MbRelay[3] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x000B:
            WMbData.sMbData.MbRelay[4] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x000C:
            WMbData.sMbData.MbRelay[5] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x000D:
            WMbData.sMbData.MbRelay[6] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x000E:
            WMbData.sMbData.MbRelay[7] =  MbRicv.MbstrRicv.NumLo;
            break;
        case 0x000F:
            WMbData.sMbData.MbRelay[8] =  MbRicv.MbstrRicv.NumLo;
            break;
        default:
            MbStatus.MbErr = 2;     //�Ƿ������ݵ�ַ
            break;
    }
    MbstrSEND06.Addr = MbRicv.MbstrRicv.Addr;
    MbstrSEND06.FucCode = MbRicv.MbstrRicv.FucCode;
    MbstrSEND06.LocaHi = MbRicv.MbstrRicv.LocaHi;
    MbstrSEND06.LocaLo = MbRicv.MbstrRicv.LocaLo;
    MbstrSEND06.DataHi = MbRicv.MbstrRicv.NumHi;
    MbstrSEND06.DataLo = MbRicv.MbstrRicv.NumLo;
    
    SendBuf[0] = MbstrSEND06.Addr;
    SendBuf[1] = MbstrSEND06.FucCode;
    SendBuf[2] = MbstrSEND06.LocaHi;
    SendBuf[3] = MbstrSEND06.LocaLo;
    SendBuf[4] = MbstrSEND06.DataHi;
    SendBuf[5] = MbstrSEND06.DataLo;

    WbTemp.Word = Crc16(SendBuf,6);
    MbstrSEND06.CrcLo = WbTemp.Byte.ByteL;
    MbstrSEND06.CrcHi = WbTemp.Byte.ByteH;
    
    SendBuf[6] = MbstrSEND06.CrcLo;
    SendBuf[7] = MbstrSEND06.CrcHi;    
    
    Len = 8;
}
#endif

static void  ModBusRp(void)
{
    WB WbTemp;
    
    if(MbStatus.MbErr)
    {
        SendBuf[1] |=0x80;
        SendBuf[2] = MbStatus.MbErr;
        
        WbTemp.Word = Crc16(SendBuf,3);
        SendBuf[3] = WbTemp.Byte.ByteL;
        SendBuf[4] = WbTemp.Byte.ByteH;
        Len = 5;
    }

    USART1_SendArray(SendBuf,Len);
    Len = 0;
}
