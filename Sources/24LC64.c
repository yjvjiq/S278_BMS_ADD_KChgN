//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : 24LC64.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.12.8
//* Description        : �ó������24LC64���ж�д����24c64����ַΪ0x1fff ,Ҳ����8192*8bit,8192�ֽ�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "derivative.h" /* include peripheral declarations */

//=============================================================================================
//#define i2c_Delay 100
//#define Delay_5ms 1550
#define i2c_Delay 50
#define Delay_5ms 700
#define EEPROM 0xA0

#define SCL_24C64_DIR  DDRH_DDRH0  //1--output  0---input
#define SCL_24C64      PTH_PTH0
#define SDA_24C64_DIR  DDRH_DDRH2  //1--output  0---input
#define SDA_24C64      PTH_PTH2

unsigned char E2ACK_OK;
//**********************************************************************
//* Function name:   DelayTime
//* Description:     ��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void DelayTime(unsigned int value)
{
    while(value--);
}
//**********************************************************************
//* Function name:   Start
//* Description:     �ṩi2c���߹���ʱ���е���ʼλ(SCLΪ��ʱSDA��һ���½���,�������߶�Ϊ��)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void Start(void)
{
	  SDA_24C64_DIR=1;
	  SCL_24C64_DIR=1;
	  asm("nop");
  	SDA_24C64=1;
    SCL_24C64=1;             //SCL and SDA set to inputs, signals go high due to pull-up resistors
    DelayTime(i2c_Delay);
    SDA_24C64=0;             //Set to output, data low so SDA = 0
    DelayTime(i2c_Delay);
    SCL_24C64=0;             //Set to output, data low so SCL = 0
    DelayTime(i2c_Delay);
}
//**********************************************************************
//* Function name:   Stop
//* Description:     �ṩi2c���߹���ʱ���е�ֹͣλ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void Stop(void)
{
	  SDA_24C64_DIR=1;
	  asm("nop");
	  SDA_24C64=0;                        //Set to output, data low so SDA = 0
    DelayTime(i2c_Delay);
    SCL_24C64=1;                        //Set to input, SCL = 1 due to pull-up
    DelayTime(i2c_Delay);
    SDA_24C64=1;                       //Set to input, SDA = 1 due to pull-up
    DelayTime(i2c_Delay);
}
//**********************************************************************
//* Function name:   Clock
//* Description:     ʱ��
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void Clock(void)
{
  	SCL_24C64=1;                              //Set to input, SCL = 1 due to pull-up
  	DelayTime(i2c_Delay);
  	SCL_24C64=0;                                //Set to output, data low so SCL = 0
  	DelayTime(i2c_Delay);

}
//**********************************************************************
//* Function name:   NAck
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void NAck(void)
{
 	  SDA_24C64_DIR=1;
	  asm("nop");
 	  SDA_24C64=1;                          //Set to input, SDA = 1 due to pull-up
  	Clock();
    SDA_24C64_DIR=1;
    asm("nop");
  	SDA_24C64=1;                               //Set to input, SDA = 1 due to pull-up
}
//**********************************************************************
//* Function name:   Ack
//* Description:     ack from 24c64 to MCU
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void Ack(void)   //ack from 24c64 to MCU
{
	  SDA_24C64_DIR=0;
	  asm("nop");
	  if(SDA_24C64)                 //"�ߵ�ƽ
  	{
	      E2ACK_OK=0;
  	}
	  else
	  {
	      E2ACK_OK=0x55;
	  }
    Clock();

}
//**********************************************************************
//* Function name:   MCUAck
//* Description:     ack from MCU TO 24C64
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void MCUAck()   //ack from MCU TO 24C64
{
    SDA_24C64_DIR=1;
    SDA_24C64=0;                        //Set to output, data low so SDA = 0
    Clock();

}
//**********************************************************************
//* Function name:   OutByte
//* Description:     ��24c64д��һ���ֽ�
//* EntryParameter : һ���ֽ�
//* ReturnValue    : None
//**********************************************************************
void OutByte(unsigned char dd)
{
	  unsigned char i;
	  
	  SDA_24C64_DIR=1;
    asm("nop");
	  for(i=8;i>0;i--)
	  {
		    
		    if(dd & 0x80)
	    	{
			      SDA_24C64=1;        //Set to input, SDA = 1 due to pull-up
		    }
	    	else
		    {
			      SDA_24C64=0;           //Set to output, data low so SDA = 0
		    }
		    dd <<= 1;
		    Clock();
	  }
}
//**********************************************************************
//* Function name:   InByte
//* Description:     ��24c64����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : һ���ֽ�
//**********************************************************************
unsigned char InByte(void)
{
	  unsigned char i,dd;
	  
	  dd=0;
	  
	  SDA_24C64_DIR=0;
	  asm("nop");
	  asm("nop");
	  asm("nop");
	  asm("nop");  //�˴���Ҫһ����ʱ
	  for(i=8;i>0;i--)
	  {    		

    	  dd <<= 1;
    		if(SDA_24C64)           //�� 1
        { 
            dd=dd|0x01;    
        }
        else                     //�� 0
        {
            dd=dd&0xfe;
        }
        Clock();		
  	}
	  return dd;
}
//**********************************************************************
//* Function name:   Read24c64_Byte
//* Description:     ��24c64��ȡ����ֽ�
//* EntryParameter : ������ַ���ֽڴ��ָ�룬�ֽ���
//* ReturnValue    : None
//**********************************************************************
void Read24c64_Byte (unsigned int wOffset,unsigned char *str,unsigned char count)
{
    Start ();
    OutByte (EEPROM);                       //"write ADDR(R/W=W)
    Ack();
    OutByte (wOffset/256);                	//"write ADDR
    Ack ();
    OutByte(wOffset%256);
    Ack();
        
    Start ();
    OutByte (EEPROM | 1);                   //"write CONTRL BYTE(R/W=R)
    Ack();
        
    while(count!=1)                         //"read COUNT-1 byte
    {
        *str = InByte();
        MCUAck();
        str++;
        count--;
    }
    *str = InByte();                       //"read the last byte
    NAck();
        
    Stop();
}
//**********************************************************************
//* Function name:   Write24c64_Byte
//* Description:     ��24c64д�����ֽ�
//* EntryParameter : ������ַ���ֽڴ��ָ�룬�ֽ���
//* ReturnValue    : None
//**********************************************************************
void Write24c64_Byte (unsigned int wOffset,unsigned char *str,unsigned char count)
{
    Start ();
    OutByte(EEPROM);
    Ack();
    OutByte(wOffset/256);
    Ack();
    OutByte(wOffset%256);   
    Ack ();
    
    for(;count!=0;count--) 
    {
        OutByte (*str);
        Ack();
        str++;
    }
    Stop();
        
    DelayTime(Delay_5ms);
}
//**********************************************************************
//* Function name:   WriteEEPROM_Long
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
/*void WriteEEPROM_Long(uint wOffset,ulong *Addr)
{
        unsigned char *strAddr;
        strAddr = (unsigned char *)0x00200;
        
        *strAddr++ = (unsigned char)*Addr;
        *strAddr++ = (unsigned char)(*Addr>>2);
        *strAddr++ = (unsigned char)(*Addr>>4);
        *strAddr++ = (unsigned char)(*Addr>>6);
        WriteEEPROM_Byte(wOffset,strAddr,4);
}*/

//=============================================================================================
//=============================================================================================
//=============================================================================================
//===================================the end ====================================================
//=============================================================================================
//=============================================================================================
