//******************A301���������¶ȼ��*********************************
//***��ʼʱ�䣺2013��4��15��*********************************************
/****���ߣ�fencer*********************************************
 * FileName:        DS18B20.C
 * Dependencies:    
 * Processor:       Freescale MC2S12XEP100
 * Compiler:       	CodeWarriorV5.0 or higher
 * Linker:          MPLINK 03.70.00 or higher
 * Company:         DALASS 
 ********************************************************************** */
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "BMS20.h" 

#define uchar unsigned char
#define uint unsigned int
#define DQ_set    PORTA |= 0x20        //��������      
#define DQ_clr    PORTA &= 0xdf       //�������� 
/******************************************
����ʱ������Ե���12Mhz�ľ���
delay(0):��ʱ518us ���:518-2*256=6
delay(26):��ʱ7us   ��ԭ��д"5us"�Ǵ�ģ�
delay(98):��ʱ25us    ���:25-20=5
delay(178):��ʱ45us    ���:45-40=5
delay(820):��ʱ205us ���:205-200=5
delay(1620):��ʱ405us ���:405-400=5
*******************************************/
void delay18b20(unsigned int i) 
{
  int j,k;

  //for(j=i;j>0;j--)
  //    for(k=5;k>0;k--); 
  for(j=i;j>0;j--)
      for(k=5;k>0;k--); 
 
}

/*****************DS18B20******************/
void Init_Ds18b20(void)     //DS18B20��ʼ��send reset and initialization command
{
    /*
    uchar presence=1;
    
    while(presence)
    {  
        DDRE |= 0x01;
        delay18b20(5);
        DQ_set;                     //DQ��λ,��ҪҲ���С�
        delay18b20(10);                  //������ʱ
        DQ_clr;                    //��Ƭ����������
        delay18b20(750);                //��ȷ��ʱ��ά������480us
        DQ_set;
        DDRE &= 0xFE;                  //�ͷ����ߣ�������������
        delay18b20(80);
         
        if(PORTE&0x01)
            presence=1;
        else
            presence=0;
        delay18b20(650); //�˴���ʱ���㹻,ȷ������DS18B20������������
        delay18b20(650); 
    }
    */
    DDRA |= 0x20;

    DQ_clr; 
    //delay18b20(750);                //��ȷ��ʱ��ά������480us
    delay18b20(200);    //500us
    DQ_set;
    delay18b20(20);    //50us

}

uchar Read_One_Byte()       //��ȡһ���ֽڵ�����read a byte date
                            //������ʱ,�������ֽڵ������Чλ�ȴ������Ƴ�
{
    uchar i   = 0;
    uchar dat = 0;
    
    for(i=8;i>0;i--)
    {
       DDRA |= 0x20;
       delay18b20(1);
       DQ_clr;                  //���������ͣ�Ҫ��1us֮���ͷ�����
                               //��Ƭ��Ҫ�ڴ��½��غ��15us�ڶ����ݲŻ���Ч��
       delay18b20(1);                 //����ά����1us,��ʾ��ʱ��ʼ
       dat >>= 1;               //�ô������϶�����λ���ݣ����δӸ�λ�ƶ�����λ��
       DQ_set;                  //�ͷ����ߣ��˺�DS18B20���������,�����ݴ��䵽������
       delay18b20(3);
       DDRA &= 0xdf;
       delay18b20(10);                 //��ʱ7us,�˴������Ƽ��Ķ�ʱ��ͼ�������ѿ���������ʱ��ŵ���ʱ����15us�ڵ���󲿷�
       if(PORTA&0x20)                   //���������в���
       {
        dat |= 0x80;            //������Ϊ1,��DQΪ1,�ǾͰ�dat�����λ��1;��Ϊ0,�򲻽��д���,����Ϊ0
       }        
       delay18b20(30);               //����ʱ�����٣�ȷ����ʱ��ĳ���60us��
    }
    return (dat);
}

void Write_One_Byte(uchar dat)
{
    uchar i = 0;
    uchar temp;
    for(i=8;i>0;i--)
    {
        DDRA |= 0x20;
        delay18b20(1);
        DQ_clr;                        //��������
        delay18b20(1);                       //����ά����1us,��ʾдʱ��(����д0ʱ���д1ʱ��)��ʼ
        temp = dat&0x01;                 //���ֽڵ����λ��ʼ����
        if(temp==1)                              //ָ��dat�����λ���������,�������������ߺ��15us��,
            DQ_set;                              //��Ϊ15us��DS18B20������߲�����
        delay18b20(30);                     //������дʱ���������60us
        DQ_set;                        //д���,�����ͷ�����,
        dat >>= 1;
        delay18b20(3);
    }
}

int get_tmp()                   //��ȡ�¶�get the temperature
{
    uchar a,b;
    int  temp;
   
    //DisableInterrupts;
    Init_Ds18b20();                //��ʼ��
    delay18b20(30);
    Write_One_Byte(0xcc);          //����ROMָ��
    Write_One_Byte(0x44);          //�¶�ת��ָ��
    
    Init_Ds18b20();                 //��ʼ��
    delay18b20(30);
    Write_One_Byte(0xcc);          //����ROMָ��
    Write_One_Byte(0xbe);          //���ݴ���ָ��
    delay18b20(30);
    a = Read_One_Byte();           //��ȡ���ĵ�һ���ֽ�Ϊ�¶�LSB
    b = Read_One_Byte();           //��ȡ���ĵ�һ���ֽ�Ϊ�¶�MSB

    temp = b&0x0f;                      //�ȰѸ߰�λ��Ч���ݸ���temp
    temp <<= 8;                    //������8λ���ݴ�temp�Ͱ�λ�Ƶ��߰�λ
    temp = temp|a;                //���ֽںϳ�һ�����ͱ���
    if((b&0x0f)>=0x08)
    {
      temp=temp+1-4095;      
    }
    temp=temp*6.25;              //�õ���ʵʮ�����¶�ֵ
    
    //EnableInterrupts;            //��ΪDS18B20���Ծ�ȷ��0.0625��
                                //���Զ������ݵ����λ�������0.0625��                            
    return temp;
}