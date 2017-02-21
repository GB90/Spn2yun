#include "myiic.h"
#include "delay.h"

 
//初始化IIC
void IIC_Init(void)
{					     
#if(BoardType==TP201)	//如果是TP201板:
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE ); 
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	//PB6,PB7 输出高
#else	//如果是最小板:
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE ); 
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);	//PB6,PB7 输出高
#endif

}
//产生IIC起始信号
void IIC_Start(void)
{
//	SDA_OUT();     //sda线输出
	SDA_H;	  	  
	SCL_H;
	delay_us(4);
 	SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SCL_L;//钳住I2C总线，准备发送或接收数据 
 /*   
    SDA_H;
	SCL_H;
	delay_us(4);
	if(!SDA_read)
        return ;	//SDA?????????,??
	SDA_L;
	delay_us(4);
	if(SDA_read) 
        return ;	//SDA??????????,??
	SDA_L;
	//I2C_delay();
	//return TRUE;
    */
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
//	SDA_OUT();//sda线输出
	SCL_L;
	SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SCL_H; 
	SDA_H;//发送I2C总线结束信号
	delay_us(4);	
/*
	SCL_L;
	delay_us(4);
	SDA_L;
	delay_us(4);
	SCL_H;
	delay_us(4);
	SDA_H;
	delay_us(4);  
    */
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
//	SDA_IN();      //SDA设置为输入  
	SDA_H;delay_us(1);	   
	SCL_H;delay_us(1);	 
	while(SDA_read)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	SCL_L;
//	SDA_OUT();
	SDA_L;
	delay_us(2);
	SCL_H;
	delay_us(2);
	SCL_L;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	SCL_L;
//	SDA_OUT();
	SDA_H;
	delay_us(2);
	SCL_H;
	delay_us(2);
	SCL_L;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
//	SDA_OUT(); 	    
    SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			SDA_H;
		else
			SDA_L;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		SCL_H;
		delay_us(2); 
		SCL_L;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SCL_L; 
        delay_us(2);
		SCL_H;
        receive<<=1;
        if(SDA_read)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























