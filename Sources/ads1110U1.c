//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : ads1110U1.C
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���������IICͨ�ŷ�ʽ�� ��ADS1100,ADS1110��ͨ�ó���,����ϵͳ�ܵ�ѹV1�ļ���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "derivative.h" /* include peripheral declarations */

#define uch unsigned char
#define unint unsigned int

#define SCL1  PTP_PTP4
#define SDA1_DIR DDRP_DDRP5  //1--output  0---input
#define SDA1  PTP_PTP5
/////////////// below for test/////////
//#define SCL1  PTP_PTP2
//#define SDA1_DIR DDRP_DDRP3  //1--output  0---input
//#define SDA1  PTP_PTP3
///////////////test/////////
unsigned char TMR1L_U1,TMR1H_U1,TMR2_U1;
//**********************************************************************
//* Function name:   delay10usU1
//* Description:     ��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delay10usU1() 
{
    unint j;
    for(j=0;j<5;j++);
}
//**********************************************************************
//* Function name:   startU1
//* Description:     ��ʼ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void startU1 (void)       //��ʼ
{
    SDA1_DIR =1;
    asm("nop");
    SDA1=1;
    asm("nop");
    SCL1=1;
    delay10usU1();
    SDA1=0;
    asm("nop");  
    SCL1=0;    
    delay10usU1();
}
//**********************************************************************
//* Function name:   stopU1
//* Description:     ����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void stopU1(void)             //����
{ 
    SDA1_DIR =1;
    SDA1=0;
    asm("nop");
    SCL1=1;
    delay10usU1();
    SDA1=1;
    delay10usU1();
    SCL1=0;
    delay10usU1();       
}
//**********************************************************************
//* Function name:   checkU1
//* Description:     ���Ӧ���ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
uch checkU1(void)    //  ���Ӧ���ź�
{  
    uch ACK;
    
    SDA1_DIR =0;   //ע�⣬�˴�����任Ӧ����ʱ��Ϊ�͵�ʱ��
    SCL1=0;
    delay10usU1();
    SCL1=1;    
    delay10usU1();             
    asm("nop");
    asm("nop");
    if (SDA1 )
    {
        ACK = 1;
    }
    else
    {
        ACK = 0;
    } 
       
    SCL1=0;    
    asm("nop");    
    return(ACK);
}
//**********************************************************************
//* Function name:   sendBYTE_U1
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************      
void sendBYTE_U1(uch data)   //  ����һ���ֽ�
{
    uch bitcount=8;               //����8λ

    SDA1_DIR =1;
    do
    { 
        SCL1=0;              //��ʱ�Ӵ���4u���ڼ�д����         
        if((data&0x80)==0x80)   
        {
            SDA1=1;         //д 1
        }
        else  
        {
            SDA1=0;        //д 0
        }  
        delay10usU1();     
        SCL1=1;
        delay10usU1();
        data=data<<1;
        bitcount--;
    } while(bitcount);
    SCL1=0;
    SDA1=0;        //д 0
      
}   
//**********************************************************************
//* Function name:   reciveBYTE_U1
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uch reciveBYTE_U1(void)             //����һ���ֽ�
{
    uch temp1=0;
    uch bitcount1=8;

    
    SDA1_DIR =0;
    asm("nop");
    asm("nop");
    asm("nop");
    do
    { 
        SCL1=1;
        delay10usU1();  
        if(SDA1)                //�� 1
        { 
            temp1=temp1|0x01;    
        }
        else                     //�� 0
        {
            temp1=temp1&0xfe;
        }
        SCL1=0;              //��ʱ�Ӵ���4u���ڼ������
        delay10usU1();

        if(bitcount1-1)
        {
            temp1=temp1<<1;
        }
        bitcount1--;  
    }while(bitcount1);
    asm("nop");
    return(temp1);
}  
//**********************************************************************
//* Function name:   ackU1
//* Description:     �����������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
void ackU1(void)          //  �����������ź�
{ 
    SDA1_DIR =1;
    SDA1=0;
    SCL1=0;
    delay10usU1();
    SCL1=1;
    delay10usU1();
    SCL1=0;
    delay10usU1();  
    SDA1=1;
}
//**********************************************************************
//* Function name:   nackU1
//* Description:     ���Ͳ��������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************     
void nackU1(void)        //  ���Ͳ��������ź�
{ 
    SDA1_DIR =1;
    SDA1=1; 
    delay10usU1();  
    SCL1=1;
    delay10usU1();
    SCL1=0;
    SDA1=0; 
}  
//**********************************************************************
//* Function name:   wrtoIIC_U1
//* Description:     д������ַ��������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************   
void wrtoIIC_U1(uch data)     //д������ַ��������
{ 
    uch i;

    i =20;
    startU1();               //��ʼд
    sendBYTE_U1(0x90);         //д������ַ��д��       
    do
    {
      i--;
    }while(checkU1()&&(i>0));       //�ȴ�Ӧ��  
    //}while(checkU1());
     
    sendBYTE_U1(data);      //д������
    delay10usU1();
    
    i=20;
    do
    {
       i--;         
    }while(checkU1()&&(i>0)); 
    //}while(checkU1());         
    stopU1();            //����д    
             
}

//**********************************************************************
//* Function name:   resetIIC_U1
//* Description:     ��λ����
//* EntryParameter : None
//* ReturnValue    : None
//********************************************************************** 
/*
void resetIIC_U1(uch data) 
{
    startU1();               //��ʼд
    sendBYTE_U1(0x00);         //д������ַ��д��       
    do
    {
      ;
    }while(checkU1());       //�ȴ�Ӧ��   
     
    sendBYTE_U1(data);      //д������
    delay10usU1();
    do
    {
       ;        
    }while(checkU1());        
    stopU1();            //����д 

}    
*/
//**********************************************************************
//* Function name:   rdfromIIC_U1
//* Description:     ��ADS1100�ж�������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void rdfromIIC_U1(void)       //��ADS1100�ж�������
{   
    uch i;

    i =20;
    
    startU1();                  //��ʼ��
    sendBYTE_U1(0x91);           //д������ַ������
    do
    {
         i--;
    }while(checkU1()&&(i>0));    //�ȴ�Ӧ��
    //}while(checkU1());
               
    TMR1H_U1=reciveBYTE_U1();     // ����8λ
    ackU1();                  //������
    TMR1L_U1=reciveBYTE_U1();     //����8λ
    ackU1();                  //������
    TMR2_U1=reciveBYTE_U1();      //��������
    nackU1();
    stopU1();                  //������
   
} 
//**********************************************************************
//* Function name:   GetU1adValue
//* Description:     ��ȡ��Եͨ����ѹADֵ
//* EntryParameter : None
//* ReturnValue    : ADֵ
//**********************************************************************          
unsigned int GetU1adValue(void)
{   
    //unsigned char i;
    unsigned long buffer=0;
    unsigned int ad=0;
      
    wrtoIIC_U1(0x84);      
    /*
    for(i=0;i<8;i++) 
    {   
        rdfromIIC_U1();
        buffer += (((unsigned long)TMR1H_U1&0xffff)<<8) | (unsigned long)TMR1L_U1; 
    }
    ad = buffer>>3;              //�������8
    */
    rdfromIIC_U1();
    ad += (((unsigned long)TMR1H_U1&0xffff)<<8) | (unsigned long)TMR1L_U1;
    return ad;
      
}  
