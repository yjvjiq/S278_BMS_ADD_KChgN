//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : gpio.C
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,06,02
//* Description        : �ó������9s12xep100��IO�ڽ��ж�������ã�ͬʱ�ж�ϵͳ�������ź�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h"

//**********************************************************************
//* Function name:   gpioInit
//* Description:     IO�ڳ�ʼ��
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void gpioInit(void) 
{
    DDRP |= 0xfe;

    DDRK &= 0x7f;  //�趨PK7Ϊ����   
    DDRA |= 0x01;  //portA0 ����Ϊ��� �� SW_POWER��    
    DDRA &= 0x3f;  //portA PA6��CP�� PA7��CC2��Ϊ����
    DDRS |= 0x03;  //PS0,PS1Ϊ�����
    DDRM |= 0x80;  //pm7 as output
    //DDRB |= 0x10;  //PB4 as WDI �ⲿ���Ź�����
    
    ATD0DIENL |= 0xe0;  //��AN05 AN06 AN07ģ�����������Ϊ ����ʹ��
    ATD0DIENH |= 0xe0;  //��AN13 AN14 AN15ģ�����������Ϊ ����ʹ��
    DDR1AD0 &= 0xbf;//PAD06 Ϊ���� IN1
    DDR0AD0 &= 0xdf;//PAD13 Ϊ����IN2
    DDR1AD0 &= 0xdf;//PAD05 Ϊ���� IN3
    DDR0AD0 &= 0x7f;//PAD15 Ϊ����IN+
    DDR1AD0 &= 0x7f;//PAD07 Ϊ���� IN-
    DDR0AD0 &= 0xbf;//PAD14 Ϊ����INh
    IRQCR &= 0xBF;
 
}
//**********************************************************************
//* Function name:   turnOnLED0
//* Description:     ����LED0�ĵ�
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOnLED0(void)      //����LED0�ĵ�
{
    PTP |= 0x02;     //���PP2=1;
}
//**********************************************************************
//* Function name:   turnOffLED0
//* Description:     Ϩ��LED0
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOffLED0(void) 
{
    PTP &= 0xfd;     //���PP2=0;
}
//**********************************************************************
//* Function name:   turnOnSW_Power
//* Description:     �պ�SW_POWER����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOnSW_Power(void)//����SW_POWER����
{

    PORTA |= 0x01;  //���PA0=1
}
//**********************************************************************
//* Function name:   turnOffSW_Power
//* Description:     �Ͽ�SW_POWER����
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOffSW_Power(void)//
{

    PORTA &= 0xfe;   //���PA1=0
}
//**********************************************************************
//* Function name:   turnOnDO1
//* Description:     �򿪿���DO1
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOnDO1(void) 
{
     PTS |= 0x02;  //�򿪿���DO1
}
//**********************************************************************
//* Function name:   turnOffDO1
//* Description:     �Ͽ�DO1
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOffDO1(void) 
{
     PTS &= 0xfd;  //�պϿ���DO1
}
//**********************************************************************
//* Function name:   turnOnDO2
//* Description:     �򿪿��� D02
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOnDO2(void) 
{
     PTS |= 0x01;  //�򿪿��� D02
}
//**********************************************************************
//* Function name:   turnOffDO2
//* Description:     �Ͽ�����DO2
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void turnOffDO2(void) 
{
     PTS &= 0xfe;  //�պϿ���DO2
}
//*******************************************
//************�����Ǹ�����������Ӧ�ķ���ֵ
//**********���з�������0��ʾ�����ϣ�����1��ʾ�Ͽ�************************
//**********************************************************************
//* Function name:   input1_state
//* Description:     ��練��
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char input1_state(void)    //IN1          ///test��練��
{
    unsigned char t;
    
    t=0;   
    if (PT1AD0 & 0x40) 
    {
        while(t<10) 
        {
            t++;
            if ((PT1AD0 & 0x40)==0) 
                return 0;         
            if(t>=10)
                return 1; 
        }
    }    
    return 0;        
}
//**********************************************************************
//* Function name:   input2_state
//* Description:     in2״̬
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char input2_state(void)    //IN2
{
    if (PT0AD0 & 0x20) 
        return 1;    
    return 0;        
}
//**********************************************************************
//* Function name:   input3_state
//* Description:     in3
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char input3_state(void)                 //////test INPUT2
{
    if (PT1AD0 & 0x20) 
      return 1;    
    return 0;
          
}
//**********************************************************************
//* Function name:   inputP_state
//* Description:     IN+  ��������
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char inputP_state(void)   //IN+         /////test ��������
{                        
    if (PT0AD0 & 0x80) 
        return 1;    
    return 0;        
}
//**********************************************************************
//* Function name:   inputN_state
//* Description:     IN- ��������
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char inputN_state(void)  //IN-
{
    if (PT1AD0 & 0x80) 
        return 1;    
    return 0;        
}
//**********************************************************************
//* Function name:   inputH_state
//* Description:     INh     �ߵ�ѹ����   
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char inputH_state(void)   //INh      ///////////////�ߵ�ѹ����   
{
    if (PT0AD0 & 0x40) 
        return 1;    
    return 0;        
}

unsigned char HV_Lock_Error_state(void){
	return inputH_state();
}
//**********************************************************************
//* Function name:   input4_state
//* Description:     IN4
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char input4_state(void)   //IN4
{
    if (PORTA & 0x02) 
        return 1;    
    return 0; 
            
}
//**********************************************************************
//* Function name:   input5_state
//* Description:     CC2 = IN5
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char input5_state(void)      //CC2 = IN5
{
    if (PORTA & 0x80) 
      return 1;    
    return 0; 
          
}
//**********************************************************************
//* Function name:   input6_state
//* Description:     IN6  = CP
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�����ϣ�����1��ʾ�Ͽ�
//**********************************************************************
unsigned char input6_state(void)      //PP0  = IN6   ///////test CP
{
    if (PTP & 0x01) 
        return 1;    
    return 0;           
}
//**********************************************************************
//* Function name:   inputAll_state
//* Description:     
//* EntryParameter : None
//* ReturnValue    : ����0��ʾ�޷�����1��ʾ�з���
//**********************************************************************
unsigned char inputAll_state(void) 
{
    if(!input1_state())
        return 0;
     
    if(!input2_state())
        return 0;
     
    if(!input3_state())
        return 0;
     
    if(!input4_state())
        return 0;
     
    if(!inputP_state())
        return 0;
     
    if(!inputN_state())
        return 0;
     
    if(!inputH_state())
        return 0;
          
    return 1;
}
