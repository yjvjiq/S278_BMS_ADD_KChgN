//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : ads1110U3.C
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���������IICͨ�ŷ�ʽ�� ��ADS1100,ADS1110��ͨ�ó���,����ϵͳ�ܵ�ѹV3�ļ���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */

#define uch unsigned char
#define unint unsigned int

#define SCL3  PTM_PTM7
#define SDA3_DIR DDRM_DDRM6  //1--output  0---input
#define SDA3  PTM_PTM6

///// below for test
//#define SCL3  PTP_PTP2
//#define SDA3_DIR DDRP_DDRP3  //1--output  0---input
//#define SDA3  PTP_PTP3

unsigned char TMR1L_U3,TMR1H_U3,TMR2_U3;
//**********************************************************************
//* Function name:   delay10usU3
//* Description:     ��ʱ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void delay10usU3() 
{
    unint j;
    for(j=0;j<5;j++);
}
//**********************************************************************
//* Function name:   startU3
//* Description:     ��ʼ
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void startU3 (void)       //��ʼ
{
    SDA3_DIR =1;
    asm("nop");
    SDA3=1;
    asm("nop");
    SCL3=1;
    delay10usU3();
    SDA3=0;
    asm("nop");  
    SCL3=0;    
    delay10usU3();
}
//**********************************************************************
//* Function name:   stopU3
//* Description:     ����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void stopU3(void)             //����
{ 
    SDA3_DIR =1;
    SDA3=0;
    asm("nop");
    SCL3=1;
    delay10usU3();
    SDA3=1;
    delay10usU3();
    SCL3=0;
    delay10usU3();       
}
//**********************************************************************
//* Function name:   checkU3
//* Description:     ���Ӧ���ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
uch checkU3(void)    //  ���Ӧ���ź�
{  
    uch ACK;
    
    SDA3_DIR =0;   //ע�⣬�˴�����任Ӧ����ʱ��Ϊ�͵�ʱ��
    SCL3=0;
    delay10usU3();
    SCL3=1;    
    delay10usU3();             
    asm("nop");
    asm("nop");
    if (SDA3 )
    {
        ACK = 1;
    }
    else
    {
        ACK = 0;
    } 
       
    SCL3=0;    
    asm("nop");    
    return(ACK);
}
//**********************************************************************
//* Function name:   sendBYTE_U3
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************          
void sendBYTE_U3(uch data)   //  ����һ���ֽ�
{
    uch bitcount=8;               //����8λ

    SDA3_DIR =1;
    do
    { 
        SCL3=0;              //��ʱ�Ӵ���4u���ڼ�д����         
        if((data&0x80)==0x80)   
        {
            SDA3=1;         //д 1
        }
        else  
        {
            SDA3=0;        //д 0
        }  
        delay10usU3();     
        SCL3=1;
        delay10usU3();
        data=data<<1;
        bitcount--;
    } while(bitcount);
    SCL3=0;      
}   
//**********************************************************************
//* Function name:   reciveBYTE_U3
//* Description:     ����һ���ֽ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
uch reciveBYTE_U3(void)             //����һ���ֽ�
{
    uch temp1=0;
    uch bitcount1=8;

    SDA3_DIR =0;
    asm("nop");
    asm("nop");
    asm("nop");
    do
    { 
        SCL3=0;              //��ʱ�Ӵ���4u���ڼ������
        delay10usU3();
        SCL3=1;
        delay10usU3();  
        if(SDA3)                //�� 1
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
    SCL3=0;
    asm("nop");
    return(temp1);
}  
//**********************************************************************
//* Function name:   ackU3
//* Description:     �����������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************    
void ackU3(void)          //  �����������ź�
{ 
    SDA3_DIR =1;
    SDA3=0;
    SCL3=0;
    delay10usU3();
    SCL3=1;
    delay10usU3();
    SCL3=0;
    delay10usU3();
}
//**********************************************************************
//* Function name:   nackU3
//* Description:     ���Ͳ��������ź�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************     
void nackU3(void)        //  ���Ͳ��������ź�
{ 
    SDA3_DIR =1;
    SDA3=1; 
    delay10usU3();  
    SCL3=1;
    delay10usU3();
    SCL3=0;
}  
//**********************************************************************
//* Function name:   wrtoIIC_U3
//* Description:     д������ַ��������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************  
void wrtoIIC_U3(uch data)     //д������ַ��������
{ 
    uch i;

    i =20;
    
    startU3();               //��ʼд
    sendBYTE_U3(0x90);         //д������ַ��д��       
    do
    {
      i--;
    }while((checkU3()&&(i>0)));       //�ȴ�Ӧ��   
    
    sendBYTE_U3(data);      //д������
    delay10usU3();
    i =20;
    do
    {
       i--;        
    }while((checkU3()&&(i>0)));       
    stopU3();            //����д    
            
}
//**********************************************************************
//* Function name:   rdfromIIC_U3
//* Description:     ��ADS1100�ж�������
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void rdfromIIC_U3(void)       //��ADS1100�ж�������
{   
    uch i;

    i =20;  
    startU3();                  //��ʼ��
    sendBYTE_U3(0x91);           //д������ַ������
    do
    {
         i--;
    }while((checkU3()&&(i>0)));      //�ȴ�Ӧ��
            
    TMR1H_U3=reciveBYTE_U3();     // ����8λ
    ackU3();                  //������
    TMR1L_U3=reciveBYTE_U3();     //����8λ
    ackU3();                  //������
    TMR2_U3=reciveBYTE_U3();      //��������
   
    nackU3();
    stopU3();                  //������
   
} 
//**********************************************************************
//* Function name:   GetU3adValue
//* Description:     ��ȡ��Եͨ����ѹADֵ
//* EntryParameter : None
//* ReturnValue    : ADֵ
//**********************************************************************      
unsigned int GetU3adValue(void)
{   
    //unsigned char i;
    //unsigned long buffer=0;
    unsigned int ad=0;

    wrtoIIC_U3(0x84);        
    /*
    for(i=0;i<8;i++) 
    {   
        rdfromIIC_U3();
        buffer += (((unsigned long)TMR1H_U3&0xffff)<<8) | (unsigned long)TMR1L_U3; 
    }
    ad = buffer>>3;              //�������8
    */
    rdfromIIC_U3();
    ad += (((unsigned long)TMR1H_U3&0xffff)<<8) | (unsigned long)TMR1L_U3; 
    return ad;
      
}  

////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////END///////////////////////////////