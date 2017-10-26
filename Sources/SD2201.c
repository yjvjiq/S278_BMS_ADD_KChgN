#include "SysDef.h"



U8  ReadTimeFlag;
SYSTEMTIME CurrentTimeSys;	
SUNRISET   SunTime;
//ϵͳʱ��
unsigned int CurrentData,CurrentWeek,CurrentTime;
char SDTimer[7];//={88,9,25,4,5,40,20};
//���ܣ�I2C��ʼ����
//���룺��
//�������
void Delay(unsigned int i)
{
   while(i--);
}

static void I2cStart(void)
{
   I2cSDA_POUT|=I2cSDA_B;
   I2cSCL_POUT|=I2cSCL_B;Delay(i2c_Delay);
   I2cSDA_POUT&=~I2cSDA_B;Delay(i2c_Delay);
   I2cSCL_POUT&=~I2cSCL_B;
}
//���ܣ�I2C��ֹ����
//���룺��
//�������
static void I2cStop(void)
{
   I2cSDA_POUT &= ~I2cSDA_B;
   I2cSCL_POUT |= I2cSCL_B;Delay(i2c_Delay);
   I2cSDA_POUT |= I2cSDA_B;Delay(i2c_Delay);
   I2cSCL_POUT &= ~I2cSCL_B;
}

//���ܣ�I2C����Ӧ��λ
//���룺��
//�������
static void I2cAck(void)
{
  I2cSDA_POUT &= ~I2cSDA_B;
  I2cSCL_POUT |= I2cSCL_B; Delay(i2c_Delay);
  I2cSCL_POUT &= ~I2cSCL_B; Delay(i2c_Delay);
  I2cSDA_POUT |= I2cSDA_B;
}

//���ܣ�I2C���ͷ�Ӧ��λ
//���룺��
//�������
static void I2cNack(void)
{
  I2cSDA_POUT |= I2cSDA_B;
  I2cSCL_POUT |= I2cSCL_B;Delay(i2c_Delay);
  I2cSCL_POUT &= ~I2cSCL_B;Delay(i2c_Delay);
  I2cSDA_POUT &= ~I2cSDA_B;
}

//���ܣ�I2C�ȴ�Ӧ��λ(����ֵ��Ӧ��λ����ȷΪ0)
//���룺��
//�����checkbyte = 1 right.
static char I2cWaitAck(void)
{
  char checkbyte;
  I2cSDA_PDIR &= ~I2cSDA_B;Delay(i2c_Delay);
  I2cSCL_POUT |= I2cSCL_B; Delay(i2c_Delay);
  checkbyte = I2cSDA_PIN & I2cSDA_B;
  I2cSCL_POUT &= ~I2cSCL_B; Delay(i2c_Delay);
  I2cSDA_PDIR |= I2cSDA_B;
  return checkbyte;
}
//���ܣ�����һ�ֽ�������SD2001��(����ֵ����������)
//���룺data: ����/������
//      order=1,H_L,��������;
//      order=0,L_H,��������.
//�������
static char SendSdByte(char data,char order)
{
  char i=1;
  if(order)
  {
     for(;i<=8;i++)
     {
        if(data&0x80)I2cSDA_POUT|=I2cSDA_B;
        else I2cSDA_POUT&=~I2cSDA_B;
        Delay(i2c_Delay);
        I2cSCL_POUT|=I2cSCL_B;
        data<<=1;
        Delay(i2c_Delay);
        I2cSCL_POUT&=~I2cSCL_B;
     }
  }
  else
  {
     for(;i<=8;i++)
     {
        if(data&0x01)I2cSDA_POUT|=I2cSDA_B;
        else I2cSDA_POUT&=~I2cSDA_B;
        Delay(i2c_Delay);
        I2cSCL_POUT|=I2cSCL_B;
        data>>=1;
        Delay(i2c_Delay);
        I2cSCL_POUT&=~I2cSCL_B;
     }
  }
  i=I2cWaitAck();
  return i;
}




//���ܣ���SD2001����һ�ֽ�����(L-H)��
//���룺��
//��������յ�����
static char RecvSdByte(void)
{
  char i,data=0;
  I2cSDA_PDIR&=~I2cSDA_B;
  for(i=1;i<=8;i++)
  {
    data>>=1;
    I2cSCL_POUT |= I2cSCL_B;
    Delay(i2c_Delay);
    if(I2cSDA_PIN&I2cSDA_B) data|=0x80;
    I2cSCL_POUT &=~ I2cSCL_B;
    Delay(i2c_Delay);
  }
  I2cSDA_PDIR|=I2cSDA_B;
  return(data);
}

void SixteenToTen( char * data)
{
     unsigned char i;
     for(i=0;i<7;i++)
     {
          if(i == 4)  
               *data &= 0X3F;     //
          *data = (*data/16)*10+(*data & 0x0F);
          data++;
     }    
}

void TenToSixteen(unsigned char * data)
{
     unsigned char i;
     for(i=0;i<7;i++)
     {
          
          *data = (*data/10)*16+(*data % 10);
          data++;
     }    
}

//��������ת�������ڸ�ʽ�ַ�
void DateToStr(SYSTEMTIME *Time)
{
     U8 Count = 0;
     Time->DateString[Count++] = Time->Year/10 + '0';
     Time->DateString[Count++] = Time->Year%10 + '0';
     Time->DateString[Count++] = '-';
     Time->DateString[Count++] = Time->Month/10 + '0';
     Time->DateString[Count++] = Time->Month%10 + '0';
     Time->DateString[Count++] = '-';
     Time->DateString[Count++] = Time->Day/10 + '0';
     Time->DateString[Count++] = Time->Day%10 + '0';
     Time->DateString[Count++] = '\0';                      
}
//ʱ������ת����ʱ���ʽ�ַ�
void TimeToStr(SYSTEMTIME *Time)
{
     Time->TimeString[0] = Time->Hour/10 + '0';
     Time->TimeString[1] = Time->Hour%10 + '0';
     Time->TimeString[2] = ':';
     Time->TimeString[3] = Time->Minute/10 + '0';
     Time->TimeString[4] = Time->Minute%10 + '0';
     Time->TimeString[5] = ':';
     Time->TimeString[6] = Time->Second/10 + '0';
     Time->TimeString[7] = Time->Second%10 + '0';
     // Time->TimeString[8] = ':';
     //Time->TimeString[9] = Time->HofSecond/10 + '0';
     //Time->TimeString[10] = Time->HofSecond%10 + '0';        
    // Time->TimeString[11] = '\0';
}
//���ܣ�����ǰʱ��(��ǰʱ��洢��ַ 7bytes)
//���룺7bytes ʱ��ṹ��������ʽ��y m d h m s w(byte)
//��������ĺ�� 7bytes ʱ��ṹ��������ʽ��y m d h m s w(byte)
void ReadSysTime(char *timeptr)
{
  char i;//,w
  I2cStart();
  SendSdByte(0x65,1);
  Delay(0);
  for(i=0;i<7;i++,timeptr++)
  {
    *timeptr=RecvSdByte();
   
          I2cAck();
  }
  I2cNack();
  I2cStop();
 // *timeptr=w;
}

void ReadSysTemTime (void )
{
     ReadSysTime(SDTimer);
     SixteenToTen(SDTimer);  
     
     CurrentTimeSys.Year =      (U8)SDTimer[0];
     CurrentTimeSys.Month =     (U8)SDTimer[1];
     CurrentTimeSys.Day =       (U8)SDTimer[2];
     CurrentTimeSys.Hour =      (U8)SDTimer[4];
     CurrentTimeSys.Minute =    (U8)SDTimer[5];
     CurrentTimeSys.Second =    (U8)SDTimer[6];
     CurrentTimeSys.Week =    (U8)SDTimer[3];
     CurrentTimeSys.TimeString[8] =0;           
     DateToStr(&CurrentTimeSys);
     TimeToStr(&CurrentTimeSys);            
}

//���ܣ�I2C��ʼ����
//���룺��
//�������
//����ʱ��(��ǰʱ��洢��ַ 7bytes)
//ʱ���ʽ��y m d h m s w (byte)
void SetSysTime(char *timeptr)
{
 
  char i,tmp;
  I2cStart();
  SendSdByte(0x64,1);
  Delay(0);
  for(i=0;i<7;i++)
  {
      if(i!=3)tmp=*timeptr++;
      else tmp=*(timeptr+3);
      SendSdByte(tmp,0);
  }
  I2cStop();
  
}

void InitRTC(unsigned char InitTime)
{
     if(InitTime)
     {          
          SDTimer [0] = HI(hld_reg[CurrentYearMonthAdr]);             
          SDTimer [1] = LOW(hld_reg[CurrentYearMonthAdr]);                     
          SDTimer [2] = HI(hld_reg[CurrentDayHourAdr]);              
          SDTimer [3] = LOW(hld_reg[CurrentDayHourAdr]);                    
          SDTimer [4] = HI(hld_reg[CurrentMinuteSecondAdr]);            
          SDTimer [5] = LOW(hld_reg[CurrentMinuteSecondAdr]);                   
          SDTimer [6] = LOW(hld_reg[CurrentWeekAdr]);
          TenToSixteen((unsigned char*)&SDTimer);
          SetSysTime(SDTimer);
     } 

      //
      //ʱ���жϳ�ʼ��
    TIMEINTDIR &= ~ INTSD2201;
    //P1IES &= ~ AD7799_RDY;        //�������ж�					
    P1IES ^= INTSD2201;       //�½����ж�
    P1IE |= INTSD2201;	    // Enable IRF intterupt                
}



//���ܣ�SD2001��ʼ��
//���룺��
//�������
void Sd2001Init(void)
{
      I2cSDA_PDIR |= I2cSDA_B;
      I2cSCL_PDIR |= I2cSCL_B;
      I2cStart();
      SendSdByte(0x60,1);  //����SD20001C״̬�Ĵ���_1���
      SendSdByte(0x02,0);  //24Сʱ�ƣ�int1 ���������,
      I2cStop();
      I2cStart();
      SendSdByte(0x62,1);  //����SD2200״̬�Ĵ���_2��
      SendSdByte(0x01,0);  //��TESTλ,�̶�Ƶ���ж����
      I2cStop();
      I2cStart();
      SendSdByte(0x68,1);  //����INT_1
      SendSdByte(0x01,0);  //���������
      I2cStop();
      
      InitRTC(0);
      
      
}

void TaskReadTime()
{
      unsigned int  tmp;      
      if(SysTemState & AD_Flag) 
      {             
            SysTemState &=~ AD_Flag;     
            if((hld_reg[LightSensorAdr] == 1)&&(hld_reg[LightContrAdr] == 1))
                  TaskAD();
      }      
      ReadSysTemTime();            
      //ʱ�����Ĵ���
      hld_reg[CurrentYearMonthAdr] = CurrentTimeSys.Year;
      hld_reg[CurrentYearMonthAdr] = ( hld_reg[CurrentYearMonthAdr] <<8)+CurrentTimeSys.Month;            
      hld_reg[CurrentDayHourAdr] = CurrentTimeSys.Day;
      hld_reg[CurrentDayHourAdr] = ( hld_reg[CurrentDayHourAdr] <<8)+CurrentTimeSys.Hour;            
      hld_reg[CurrentMinuteSecondAdr] = CurrentTimeSys.Minute;
      hld_reg[CurrentMinuteSecondAdr] = ( hld_reg[CurrentMinuteSecondAdr] <<8)+CurrentTimeSys.Second;            
      CurrentWeek = hld_reg[CurrentWeekAdr] =  CurrentTimeSys.Week;                         
      //�õ�ʱ����ʽ������
      tmp= CurrentTimeSys.Month;         //����
      tmp =(tmp << 8)+ CurrentTimeSys.Day;
      CurrentData = tmp;            
      tmp= CurrentTimeSys.Hour;         //ʱ��
      tmp =(tmp << 8)+ CurrentTimeSys.Minute;
      CurrentTime = tmp;                        
      //���¼����ճ������ʱ�� һ�����һ��
      if(SunTime.Day!=CurrentTimeSys.Day)
      {
            if(CurrentTimeSys.Year < 99)
            {
                  SunTime.Year = CurrentTimeSys.Year;
                  SunTime.Month = CurrentTimeSys.Month;
                  SunTime.Day =CurrentTimeSys.Day;
                  TestSurRiset();  
            }                  
      }      
}
////////////////////////////////////////////////////
