//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : ads1110insulation.C
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���������IICͨ�ŷ�ʽ�� 4��PIC16F877,��ADS1100,ADS1110��ͨ�ó���,���ھ�Ե��ֵ���㹦��
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h"

#define uch unsigned char
#define unint unsigned int

#define SCL4  PTP_PTP6
#define SDA4_DIR DDRP_DDRP7  //1--output  0---input
#define SDA4  PTP_PTP7

unsigned char TMR1L_iso,TMR1H_iso,TMR2_iso;

//**********************************************************************
//* Function name:   delay10usU4  
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delay10usU4() 
{
    unint j;
    for(j=0;j<5;j++);
}
//**********************************************************************
//* Function name:   startU4  
//* Description:     ��ʼ�ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void startU4 (void)       //��ʼ
{
    SDA4_DIR =1;
    asm("nop");
    SDA4=1;
    asm("nop");
    SCL4=1;
    delay10usU4();
    SDA4=0;
    asm("nop");  
    SCL4=0;    
    delay10usU4();
}
//**********************************************************************
//* Function name:   stopU4
//* Description:     �����ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void stopU4(void)             //����
{ 
    SDA4_DIR =1;
    SDA4=0;
    asm("nop");
    SCL4=1;
    delay10usU4();
    SDA4=1;
    delay10usU4();
    SCL4=0;
    delay10usU4();       
}
//**********************************************************************
//* Function name:   checkU4
//* Description:     ���Ӧ���ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
uch checkU4(void)    //  ���Ӧ���ź�
{  
    uch ACK;
    
    SDA4_DIR =0;   //ע�⣬�˴�����任Ӧ����ʱ��Ϊ�͵�ʱ��
    SCL4=0;
    delay10usU4();
    SCL4=1;    
    delay10usU4();             
    asm("nop");
    asm("nop");
    if (SDA4 )
    {
        ACK = 1;
    }
    else
    {
        ACK = 0;
    }    
    SCL4=0;    
    asm("nop");    
    return(ACK);
}
//**********************************************************************
//* Function name:   sendBYTE_U4
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************       
void sendBYTE_U4(uch data)   //  ����һ���ֽ�
{
    uch bitcount=8;               //����8λ

    SDA4_DIR =1;
    do
    { 
        SCL4=0;              //��ʱ�Ӵ���4u���ڼ�д����         
        if((data&0x80)==0x80)   
        {
            SDA4=1;         //д 1
        }
        else  
        {
            SDA4=0;        //д 0
        }  
        delay10usU4();     
        SCL4=1;
        delay10usU4();
        data=data<<1;
        bitcount--;
    } while(bitcount);
    SCL4=0;
      
}   
//**********************************************************************
//* Function name:   reciveBYTE_U4 
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uch reciveBYTE_U4(void)             //����һ���ֽ�
{
    uch temp1=0;
    uch bitcount1=8;

    SDA4_DIR =0;
    asm("nop");
    asm("nop");
    asm("nop");
    do
    { 
        SCL4=0;              //��ʱ�Ӵ���4u���ڼ������
        delay10usU4();
        SCL4=1;
        delay10usU4();  
        if(SDA4)                //�� 1
        { 
            temp1=temp1|0x01;    
        }
        else                     //�� 0
        {
            temp1=temp1&0xfe;
        }

        if(bitcount1-1)
        {
            temp1=temp1<<1; 
        }
        bitcount1--;  
    }while(bitcount1);
    SCL4=0;
    asm("nop");
    return(temp1);
}  
//**********************************************************************
//* Function name:   ackU4 
//* Description:     �����������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
void ackU4(void)          //  �����������ź�
{ 
    SDA4_DIR =1;
    SDA4=0;
    SCL4=0;
    delay10usU4();
    SCL4=1;
    delay10usU4();
    SCL4=0;
    delay10usU4();
}
//**********************************************************************
//* Function name:   nackU4
//* Description:     ���Ͳ��������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************        
void nackU4(void)        //  ���Ͳ��������ź�
{ 
    SDA4_DIR =1;
    SDA4=1; 
    delay10usU4();  
    SCL4=1;
    delay10usU4();
    SCL4=0;
}  
//**********************************************************************
//* Function name:   wrtoIIC_U4
//* Description:     д������ַ��������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************  
void wrtoIIC_U4(uch data)     //д������ַ��������
{ 
    uch i;
    
    i=20;
    startU4();               //��ʼд
    sendBYTE_U4(0x90);         //д������ַ��д��       
    do
    {
      i--;
    }while(checkU4()&&(i>0));       //�ȴ�Ӧ��   
     
    sendBYTE_U4(data);      //д������
    delay10usU4();
    i=20;
    do
    {
       i--;        
    }while(checkU4()&&(i>0));       
    stopU4();            //����д    
             
}
/*
void resetIIC(uch data) 
{
    start();               //��ʼд
    sendBYTE(0x00);         //д������ַ��д��       
    do
    {
      ;
    }while(check());       //�ȴ�Ӧ��   
     
    sendBYTE(data);      //д������
    delay10us();
    do
    {
       ;        
    }while(check());        
    stop();            //����д 

}
*/
//**********************************************************************
//* Function name:   rdfromIIC_U4
//* Description:     ��ADS1100�ж�������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void rdfromIIC_U4(void)       //��ADS1100�ж�������
{   
    uch i;
    
    i=20;
    startU4();                  //��ʼ��
    sendBYTE_U4(0x91);           //д������ַ������
    do
    {
        i--;
    }while(checkU4()&&(i>0));     //�ȴ�Ӧ��           
    TMR1H_iso=reciveBYTE_U4();     // ����8λ
    ackU4();                  //������
    TMR1L_iso=reciveBYTE_U4();     //����8λ
    ackU4();                  //������
    TMR2_iso=reciveBYTE_U4();      //��������
    nackU4();
    stopU4();                  //������
   
} 
//**********************************************************************
//* Function name:   GetInsulateVoltAd 
//* Description:     ��ȡ��Եͨ����ѹADֵ
//* EntryParameter : None
//* ReturnValue    : ADֵ
//**********************************************************************     
unsigned int GetInsulateVoltAd(void)
{   
    unsigned char i;
    unsigned long buffer=0;
    unsigned int ad=0;

    wrtoIIC_U4(0x8c); 
              
    for(i=0;i<8;i++) 
    {      
        rdfromIIC_U4();   
        buffer += (((unsigned long)TMR1H_iso&0xffff)<<8) | (unsigned long)TMR1L_iso; 
    }
    ad = buffer>>3;
    return ad;
      
}  
