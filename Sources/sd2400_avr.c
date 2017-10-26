//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : sd2400_avr.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : �ó������ʵʱʱ��оƬSD2400����ʱ������úͶ�ȡ
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */

#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong unsigned long int
//����sda��λ����
#define sda_in	    DDRJ &= 0xbf   	   //������������
#define sda_out	    DDRJ |= 0x40       //�����������
#define sda_setb    PTJ |= 0x40        //��������      
#define sda_clr     PTJ &= 0xbf        //��������      
#define sda_r       (PTJ & 0x40)       //���ݶ�ȡ  
//ʱ��scl��λ����
#define scl_in	    DDRJ &= 0x7f       //ʱ����������
#define scl_out	    DDRJ |= 0x80       //ʱ���������
#define scl_setb    PTJ |= 0x80        //ʱ������      
#define scl_clr     PTJ &= 0x7f        //ʱ������      
#define scl_r       (PTJ & 0x80)       //ʱ�Ӷ�ȡ 
//===============
#define true  1
#define false 0
//===============
unsigned char CurrentTime[7];//

void WriteTimeOn(void);
void WriteTimeOff(void);
//====================================================================
/*��ʱ�ӳ���*/
/*
void delay_ms(uint time)    	       //11.0592M,time=1��ʾ��ʱ1ms
{
    uint i;
    for(;time>0;time--)
    for(i=0;i<1571;i++);
    asm("nop");
    asm("nop");
    asm("nop");
}
*/
//**********************************************************************
//* Function name:   delay_1us
//* Description:     ��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delay_1us(void)        	       //11.0592M, 1us��ʱ����
{
    asm("nop");
}
//**********************************************************************
//* Function name:   delay_us
//* Description:     ��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delay_us(uint time)     	       //11.0592M, Nus��ʱ����
{
    uint i;
    for (i=0;i<time;i++)
    asm("nop");
}  
//=====================================================================  
//**********************************************************************
//* Function name:   I2CStart
//* Description:     ����SD2400��I2C����
//* EntryParameter : None
//* ReturnValue    : true�� false
//**********************************************************************
uchar I2CStart(void)
{
    sda_out;
  	scl_out;
  	delay_1us();
      	sda_setb;	
  	scl_setb;	
  	delay_us(5);
  	sda_in;
  	delay_us(5);
  	if(!sda_r)return false;		//SDA��Ϊ�͵�ƽ������æ,�˳�
  	sda_out;
  	delay_1us();
  	sda_clr;	
  	delay_us(10);
  	sda_in;    	
  	delay_us(10);
  	while(sda_r)return false;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
  	scl_clr;	
  	delay_us(5);
  	return true;
}
//**********************************************************************
//* Function name:   I2CStop
//* Description:     �ر�SD2400��I2C����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void I2CStop(void)
{
    scl_out;
  	sda_out;
  	delay_1us();
  	sda_clr;
  	delay_1us();
  	scl_clr;
  	delay_us(5);
  	scl_setb;
  	delay_us(2);
  	sda_setb;
}
//**********************************************************************
//* Function name:   I2CAck
//* Description:     ���� ACK
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void I2CAck(void)
{	
    scl_out;
  	sda_out;
  	delay_1us();
  	sda_clr;	
  	delay_1us();
  	scl_clr;	
  	delay_us(5);
  	scl_setb;	
  	delay_us(5);
  	scl_clr;	
}
//**********************************************************************
//* Function name:   I2CNoAck
//* Description:     ����NO ACK
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void I2CNoAck(void)
{	
    scl_out;
  	sda_out;
  	delay_1us();
  	sda_setb;	
  	delay_1us();
  	scl_clr;	
  	delay_us(5);
  	scl_setb;	
  	delay_us(5);
  	scl_clr;	
}
//**********************************************************************
//* Function name:   I2CWaitAck
//* Description:     ��ȡACK�ź�
//* EntryParameter : None
//* ReturnValue    : ����Ϊ:1=��ACK,0=��ACK
//**********************************************************************
uchar I2CWaitAck(void) 			 //����Ϊ:1=��ACK,0=��ACK
{
  	uchar errtime=255;
  	
  	scl_out;
  	sda_in;
  	delay_1us();
  	scl_clr;	
  	delay_us(5);
  	scl_setb;	
  	delay_1us();
  	sda_in;
  	delay_1us();
  	while(sda_r)	
  	{
    		errtime--;
    		delay_1us();
    		if(!errtime)
    		{
        		delay_us(10);
        		scl_clr;
        		return false;
        }
  	}
  	scl_clr;	
  	return true;
}
//**********************************************************************
//* Function name:   I2CSendByte
//* Description:     MCU��SD2400����һ���ֽ�//���ݴӸ�λ����λ
//* EntryParameter : �����ֽ�
//* ReturnValue    : None
//**********************************************************************              
void I2CSendByte(unsigned char demand)		 //���ݴӸ�λ����λ
{
  	uchar bd=8; 
  	                                                       
  	sda_out;
  	scl_out;
		while(bd--)
		{
  			scl_clr;  
  			delay_1us();
  			if(demand&0x80)
  			{
  			    sda_setb;
  			}
  			else
  			{
  			    sda_clr;
  			}
  			demand<<=1;
  			delay_us(3);
  			scl_setb;
  			delay_us(3);
		}
		scl_clr;	      
}
//**********************************************************************
//* Function name:   I2CReceiveByte
//* Description:     MCU��SD2400����һ�ֽ�//���ݴӸ�λ����λ//
//* EntryParameter : None
//* ReturnValue    : һ���ֽ�
//**********************************************************************
uchar I2CReceiveByte(void) 		//���ݴӸ�λ����λ//
{
  	uchar bl=8;
  	uchar ddata=0;
  	
  	scl_out;
  	delay_1us();
  	delay_1us();
  	sda_in;			 	//c51��ΪSDA=1;�˿��л�Ϊ����״̬����
  	delay_1us();
  	while(bl--)
  	{
        ddata<<=1;              //���ݴӸ�λ��ʼ��ȡ
    		delay_1us();
    		scl_clr;
    		delay_us(5);	        //�Ӹ�λ��ʼ ddata|=SDA;ddata<<=1
    		scl_setb;
    		delay_us(5);	
    		if(sda_r)
    		{
    			  ddata|=0x01;
    		}  		  		
  	}
  	scl_clr;
  	return ddata;
}
//**********************************************************************
//* Function name:   I2CReadDate
//* Description:     ��SD2400ʵʱ���ݼĴ���
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void I2CReadDate(void)
{
  	unsigned char n;
  	
  	I2CStart();
  	I2CSendByte(0x65); 
  	I2CWaitAck();
  	for(n=0;n<7;n++)
  	{
    		CurrentTime[n]=I2CReceiveByte()&0x7f;
    		if (n!=6)        	 //���һ�����ݲ�Ӧ��
    		{
    			  I2CAck();
    		}
  	}
  	I2CNoAck();
  	delay_1us();
  	I2CStop();
}
//**********************************************************************
//* Function name:   WriteTimeOn
//* Description:     дSD2400�������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void WriteTimeOn(void)
{		
  	I2CStart();
  	I2CSendByte(0x64);      
  	I2CWaitAck();   
    I2CSendByte(0x10);		//����д��ַ10H      
  	I2CWaitAck();	
  	I2CSendByte(0x80);		//��WRTC1=1      
  	I2CWaitAck();
  	I2CStop(); 
  	
  	I2CStart();
  	I2CSendByte(0x64);      
  	I2CWaitAck();   
    I2CSendByte(0x0F);		//����д��ַ0FH      
  	I2CWaitAck();	
  	I2CSendByte(0x84);		//��WRTC2,WRTC3=1      
  	I2CWaitAck();
  	I2CStop(); 
	
}
//**********************************************************************
//* Function name:   I2CWriteDate
//* Description:     дSD2400ʵʱ���ݼĴ���
//* EntryParameter : ���ݵ�ַָ��
//* ReturnValue    : None
//**********************************************************************
void I2CWriteDate(unsigned char *pSecDa)
{		

    WriteTimeOn();

  	I2CStart();
  	I2CSendByte(0x64);      
  	I2CWaitAck();   
    I2CSendByte(0x00);		//����д��ʼ��ַ      
  	I2CWaitAck();	
  	I2CSendByte(*pSecDa);		// second     
  	I2CWaitAck();	
  	I2CSendByte(*(pSecDa+1));		//minute      
  	I2CWaitAck();	
  	I2CSendByte(0x80|*(pSecDa+2));		//hour ,��ʮ��Сʱ��     
  	I2CWaitAck();	
  	I2CSendByte(*(pSecDa+3));		//week      
  	I2CWaitAck();	
  	I2CSendByte(*(pSecDa+4));		//day      
  	I2CWaitAck();	
  	I2CSendByte(*(pSecDa+5));		//month      
  	I2CWaitAck();	
  	I2CSendByte(*(pSecDa+6));		//year      
  	I2CWaitAck();	
  	I2CStop();

  	I2CStart();
  	I2CSendByte(0x64);      
  	I2CWaitAck();   
    I2CSendByte(0x12);		//����д��ʼ��ַ      
  	I2CWaitAck();		
    I2CSendByte(0x00);		//�������ֵ����Ĵ���      
  	I2CWaitAck();        
  	I2CStop();  
  	
  	WriteTimeOff();      
}
//**********************************************************************
//* Function name:   WriteTimeOff
//* Description:     дSD2400��ֹ����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void WriteTimeOff(void)
{		
  	I2CStart();
  	I2CSendByte(0x64);
  	I2CWaitAck();   
    I2CSendByte(0x0F);		//����д��ַ0FH      
  	I2CWaitAck();	
  	I2CSendByte(0x0) ;		//��WRTC2,WRTC3=0      
  	I2CWaitAck();
  	I2CSendByte(0x0) ;		//��WRTC1=0(10H��ַ)      
  	I2CWaitAck();
  	I2CStop(); 
}

