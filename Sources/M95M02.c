//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : m95m02.C
//* Author             : Kong XiangYu
//* Version            : V1.0.0
//* Start Date         : Mar. 14, 2015
//* Description        : �ó������оƬm95m02���ж�д����оƬ�洢ϵͳ����ʷ��¼��Ϣ�����800����¼
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h"

//**********************************************************************
//* Function name:   delayXusAtM95
//* Description:     m95m02��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delayXusAtM95(unsigned int x) 
{
    unsigned int i; 
    for (i=0;i<x;i++);
}
//**********************************************************************
//* Function name:   InitialAtM95M02
//* Description:     M95M02��ʼ��
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void InitialAtM95M02(void)
{
    SPI0CR1 &= 0xbf;    //��ֹSPI0ģʽ
    DDRS |= 0xe4;     //����CS SCK, SI,RES����Ϊ���,1110 0100 
    DDRS &= 0xef;    //SO����Ĭ��Ϊ����1110 1111 
 
    PTS |= 0x84;  // ����HOLDλΪ�ߵ�ƽ��оƬ��������ͣ״̬1000 0010
    delayXusAtM95(50);

}
//**********************************************************************
//* Function name:   cs_up
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void cs_up()
{
    PTS |= 0x04; //ƬѡΪ��
}   
//**********************************************************************
//* Function name:   cs_down
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void cs_down()
{
    PTS &= 0xfb;
}
//=============================================================================================
//"Clock Contrl .
//=============================================================================================
//**********************************************************************
//* Function name:   sck_up
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void sck_up()
{
    PTS |= 0x40;//0100 0000 PS6=1
}
//**********************************************************************
//* Function name:   sck_down
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void sck_down()
{
    PTS &= 0xbf;//1011 1111 PS6=0
}
//=============================================================================================
//"Data Out Contrl .
//=============================================================================================
//**********************************************************************
//* Function name:   si_up
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void si_up()
{
    PTS |= 0x20; //0010 0000 PS5=0 SI(��Ƭ��SPI���)
}
//**********************************************************************
//* Function name:   si_down
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void si_down()
{
    PTS &= 0xdf; // 1101 1111 PS5=1 SI����Ƭ��SPI���)
}

//**********************************************************************
//* Function name:   Byte_Out
//* Description:     "EXIT:SCK=1;CS=0; ��Ƭ����EEPROMдһλ�������أ������ָ���ʱ����.
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void Byte_Out(uchar bb)
{
    uchar flag;
	  uchar i;
	
	  for(i=0;i<8;i++)
	  {

    		flag=bb&0x80;		
    		if(flag==0x80)		
    		{
    			  si_up();
    		}
    		else
    		{
    			  si_down();
    		}
 
    		sck_up();		//"CLK������,������һλ������������EEPROM��������д��һλ
    
    		sck_down();		//"ʱ���½�
    
    		bb<<=1;
	  }
	
}
//**********************************************************************
//* Function name:   Byte_In
//* Description:     //"EXIT:SCK=1;CS=0; ��Ƭ����EEPROM���һλ.�½�����Ч����ROM��ʱ����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uchar Byte_In()
{
  	uchar x=0;
  	uchar i;
  	uchar flag;
  	for(i=0;i<8;i++)
  	{
    		x<<=1;	
    		flag= PTS & 0x10;		//��ȡSO������
    		if(flag==0x10)
    		{
    			x++;
    		}
    	
    		delayXusAtM95(15);
    		sck_up();
    	
    		sck_down();		//"CLK�½���,ʹ�����ͳ�һλ������
    		delayXusAtM95(15); 
    
    	
  	}
  	return(x);
}
//**********************************************************************
//* Function name:   M95_Write_Enable
//* Description:     M95дʹ��(WREN)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Write_Enable() 
{
    cs_down();
    delay_1us();//�����
   	
    Byte_Out(0x06);
    delay_1us(); //�����
    delayXusAtM95(5); 			
    cs_up();
   delay_1us();//�����
  
}
//**********************************************************************
//* Function name:   M95_Write_Disable
//* Description:     M95д��ʹ��(WRDI)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Write_Disable() 
{
    cs_down();	
    Byte_Out(0x04);
    delayXusAtM95(5); 			
    cs_up();
    delayXusAtM95(5);
}
//**********************************************************************
//* Function name:   M95_Read_Register
//* Description:     M95���Ĵ���״̬(RDSR)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uchar M95_Read_Register(void) 
{
    uchar bb;
    cs_down();
    delayXusAtM95(5);	
    Byte_Out(0x05);
  
    bb=Byte_In();
   			
    cs_up();

    return(bb); 
  
}
//**********************************************************************
//* Function name:   M95_Write_Register
//* Description:     M95д�Ĵ���״̬(WRSR)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Write_Register( uchar R_status) 
{
    M95_Write_Enable();//ʹ��д 
    //while(M95_Read_Register()&0x02);
    
    cs_down();	
    Byte_Out(0x01);
    Byte_Out(R_status);			
    cs_up();
    delayXusAtM95(50);
   
  
}
//**********************************************************************
//* Function name:   M95_Write_Addr
//* Description:     M95д24λ��ַ  //ע�⣺д������չ�ĵ�ַʱ�AA10=1��Ҳ����0x000004xx
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Write_Addr(ulong addr) 
{

    uchar i;
    ulong flag;    	
    for(i=0;i<16;i++)		
    {
    
        flag=addr&0x00800000;	
        if(flag==0x00800000)	
        {
        	  si_up();
        }
        else
        {
        	  si_down();
        }
      
        sck_up();		//"CLK������,������һλ������������
        delayXusAtM95(5);
        sck_down();		//"CLK�½�
       
        addr<<=1;
    }
    
    for(i=0;i<8;i++)		
    {
        
        flag=addr&0x00800000;	
        if(flag==0x00800000)	
        {
        	  si_up();
        }
        else
        {
        	  si_down();
        }
       
        sck_up();		//"CLK������,������һλ������������
       
        delayXusAtM95(5);
        sck_down();		//"CLK�½�
        addr<<=1;
    }

     
}

//**********************************************************************
//* Function name:   M95_Read_Memory
//* Description:     M95���ڴ棨READ)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Read_Memory(unsigned long ba,uchar *str,unsigned int count) 
{
    while(M95_Read_Register()&0x01);
    cs_down();
   
    delayXusAtM95(5);//�������ʱ	
    Byte_Out(0x03);
   
    M95_Write_Addr(ba);
 
    while(count!=0)                         //"read COUNT byte
    {
        *str = Byte_In();
        str++;
        count--;
        delayXusAtM95(10); //����ü�
    }

    cs_up();

  
}
//**********************************************************************
//* Function name:   M95_Write_Memory
//* Description:     M95д�ڴ�(WRITE)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
/////////////////////////////////
//void M95_Write_Memory(ulong ba,uchar *str,unsigned char count) 
void M95_Write_Memory(ulong ba,uchar *str,unsigned int count) 
{
   
    M95_Write_Enable(); //ʹ��д0x06 0b0000 0110
    while(M95_Read_Register()&0x01); 

    cs_down();
    delayXusAtM95(5);
    Byte_Out(0x02);
 
    M95_Write_Addr(ba); //
   
    while(count!=0)                         //"read COUNT byte
    {
        Byte_Out(*str);
       
        str++;
        count--;
       
    }
    
    //����ʱ//
    cs_up();
   
    while(M95_Read_Register()&0x01); //˵������д
  
}

//**********************************************************************
//* Function name:   M95_Read_IP_Memory
//* Description:     M95��Read Identification Page�ڴ棨RDID)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
//void M95_Read_IP_Memory(ulong ba,uchar *str,unsigned char count) 
void M95_Read_IP_Memory(ulong ba,uchar *str,unsigned int count) 
{
    cs_down();	
    Byte_Out(0x83);
    M95_Write_Addr(ba); //ע���ַ�ĸ�ʽ����ʮλ����Ϊ0
    while(count!=0)                         //"read COUNT byte
    {
        *str = Byte_In();
        str++;
        count--;
    }
    //����ʱ//
    cs_up();
  
}
//**********************************************************************
//* Function name:   M95_Write_IP_Memory
//* Description:     M95дRead Identification Page�ڴ�(WRID)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Write_IP_Memory(ulong ba,uchar *str,unsigned char count) 
{
    M95_Write_Enable(); //����Ҫʹ��д 
    cs_down();	
    Byte_Out(0x82);
    M95_Write_Addr(ba); //ע���ַ�ĸ�ʽ����ʮλ����Ϊ0
    while(count!=0)                         //"read COUNT byte
    {
        *str = Byte_In();
        str++;
        count--;
    }
    //����ʱ//
    cs_up();
  
}
//**********************************************************************
//* Function name:   M95_Read_Lock
//* Description:     M95��Read Lock Status��RDLS)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uchar M95_Read_Lock(ulong ba) 
{
    uchar bb;
    cs_down();	
    Byte_Out(0x83);
    M95_Write_Addr(ba); //ע���ַ�ĸ�ʽ����ʮλ����Ϊ1
    bb=Byte_In();
    cs_up();
    return(bb);
  
}
//**********************************************************************
//* Function name:   M95_Write_Lock
//* Description:     M95����IDֻ��(LID)
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void M95_Write_Lock( uchar R_status,ulong ba) 
{
    M95_Write_Enable();//ʹ��д 

    cs_down();	
    Byte_Out(0x82);
    M95_Write_Addr(ba);
    Byte_Out(R_status);	//ע�⣺xxxx xx1x		
    cs_up();

}

//**********************************************************************
//**************************** the end **********************************
//**********************************************************************
//**********************************************************************