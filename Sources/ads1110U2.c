//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : ads1110U2.C
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���������IICͨ�ŷ�ʽ�� ��ADS1100,ADS1110��ͨ�ó���,����ϵͳ�ܵ�ѹV2�ļ���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */

#define uch unsigned char
#define unint unsigned int

#define SCL2  PTP_PTP2
#define SDA2_DIR DDRP_DDRP3  //1--output  0---input
#define SDA2  PTP_PTP3

///// below for test
//#define SCL2  PTP_PTP4
//#define SDA2_DIR DDRP_DDRP5  //1--output  0---input
//#define SDA2  PTP_PTP5

unsigned char TMR1L_U2,TMR1H_U2,TMR2_U2;
//**********************************************************************
//* Function name:   delay10us
//* Description:     ��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delay10us() 
{
    unint j;
    for(j=0;j<5;j++);
}
//**********************************************************************
//* Function name:   start
//* Description:     ��ʼ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void start (void)       //��ʼ
{
    SDA2_DIR =1;
    asm("nop");
    SDA2=1;
    asm("nop");
    SCL2=1;
    delay10us();
    SDA2=0;
    asm("nop");  
    SCL2=0;    
    delay10us();
}
//**********************************************************************
//* Function name:   stop
//* Description:     ����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void stop(void)             //����
{ 
    
    
    SDA2_DIR =1;
    asm("nop");
    SDA2=0;
    
    SCL2=1;
    delay10us();
    SDA2=1;
    delay10us();
    SCL2=0;
    delay10us(); 
          
}
//**********************************************************************
//* Function name:   check
//* Description:     ���Ӧ���ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
uch check(void)    //  ���Ӧ���ź�
{  
    uch ACK;
    
    SDA2_DIR =0;   //ע�⣬�˴�����任Ӧ����ʱ��Ϊ�͵�ʱ��
    SCL2=0;
    delay10us();
    SCL2=1;    
    delay10us();             
    asm("nop");
    asm("nop");
    if (SDA2 )
    {
        ACK = 1;
    }
    else
    {
        ACK = 0;
    }    
    SCL2=0;    
    asm("nop");    
    return(ACK);
}
//**********************************************************************
//* Function name:   sendBYTE
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************     
void sendBYTE(uch data)   //  ����һ���ֽ�
{
    uch bitcount=8;               //����8λ

    SDA2_DIR =1;
    do
    { 
        SCL2=0;              //��ʱ�Ӵ���4u���ڼ�д����         
        if((data&0x80)==0x80)   
        {
            SDA2=1;         //д 1
        }
        else  
        {
            SDA2=0;        //д 0
        }  
        delay10us();     
        SCL2=1;
        delay10us();
        data=data<<1;
        bitcount--;
    } while(bitcount);
    SCL2=0;
      
}   
//**********************************************************************
//* Function name:   reciveBYTE
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uch reciveBYTE(void)             //����һ���ֽ�
{
    uch temp1=0;
    uch bitcount1=8;

    SDA2_DIR =0;
    asm("nop");
    asm("nop");
    asm("nop");
    do
    { 
        SCL2=0;              //��ʱ�Ӵ���4u���ڼ������
        delay10us();
        SCL2=1;
        delay10us();  
        if(SDA2)                //�� 1
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
    SCL2=0;
    asm("nop");
    return(temp1);
}  
//**********************************************************************
//* Function name:   ack
//* Description:     �����������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
void ack(void)          //  �����������ź�
{ 
    SDA2_DIR =1; 
    SDA2=0;  
    SCL2=0;
    delay10us();
    SCL2=1;
    delay10us();
    SCL2=0;
    delay10us();
}
//**********************************************************************
//* Function name:   nack
//* Description:     ���Ͳ��������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************     
void nack(void)        //  ���Ͳ��������ź�
{ 
    
    SDA2_DIR =1;
    SDA2=1; 
    asm("nop"); 
    SCL2=1;
    delay10us();
    SDA2=0; 
    SCL2=0;
    delay10us();
    
}  
//**********************************************************************
//* Function name:   wrtoIIC
//* Description:     д������ַ��������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************   
void wrtoIIC(uch data)     //д������ַ��������
{ 
    uch i;

    i =20;
    start();               //��ʼд
    sendBYTE(0x90);         //д������ַ��д��       
    do
    {
      i--;
    }while(check()&&(i>0));        //�ȴ�Ӧ��   
     
    sendBYTE(data);      //д������
    delay10us();
    
    i=20;
    do
    {
       i--;        
    }while(check()&&(i>0));        
    stop();            //����д    
             
}

//**********************************************************************
//* Function name:   resetIIC
//* Description:     ��λ����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
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
//* Function name:   rdfromIIC
//* Description:     ��ADS1100�ж�������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void rdfromIIC(void)       //��ADS1100�ж�������
{   
    uch i;

    i =20;
    start();                  //��ʼ��
    sendBYTE(0x91);           //д������ַ������
    do
    {
        i-- ;
    //}while(check());      //�ȴ�Ӧ�� 
    }while(check()&&(i>0));            
    TMR1H_U2=reciveBYTE();     // ����8λ
    ack();                  //������
    TMR1L_U2=reciveBYTE();     //����8λ
    ack();                  //������
    TMR2_U2=reciveBYTE();      //��������
    nack();
  
    stop();                  //������
   
} 
//**********************************************************************
//* Function name:   GetU2adValue
//* Description:     ��ȡ��Եͨ����ѹADֵ
//* EntryParameter : None
//* ReturnValue    : ADֵ
//**********************************************************************      
unsigned int GetU2adValue(void)
{   
    //unsigned char i;
    unsigned long buffer=0;
    unsigned int ad=0;

    wrtoIIC(0x84);
    /*           
    for(i=0;i<8;i++) 
    {      
        rdfromIIC(); 
        buffer += (((unsigned long)TMR1H_U2&0xffff)<<8) | (unsigned long)TMR1L_U2; 
    }
    ad = buffer>>3;              //�������8
    */
    rdfromIIC(); 
    ad += (((unsigned long)TMR1H_U2&0xffff)<<8) | (unsigned long)TMR1L_U2; 
    return ad;
      
}  
