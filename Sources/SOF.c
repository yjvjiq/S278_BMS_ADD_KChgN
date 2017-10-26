
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : �����ֽ�
//* File Name          : SOF.c
//* Author             : ����ѩ
//* Version            : V1.0.0
//* Start Date         : 2016.11.30
//* Description        : ���ļ����ڲ�����SOF��ǰֵ
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h" 

//******************************************************************************
//* Function name:   DischagerCurtCutOff
//* Description:     ���ϴ���,�й��Ϻ󽵵ͷŵ繦��
//* EntryParameter : void
//* ReturnValue    : ���ϵȼ���1 ��Ϊ0��2��Ϊ50%; 3��Ϊ70%
//******************************************************************************
unsigned char DischagerCurtCutOff(void)
{
    unsigned char ErrorLever=0;

    if(CutDisCurt0.word!=0)
    {
        ErrorLever=3;
    } 
    else if(CutDisCurt50.word!=0)
    {
        ErrorLever=2;
    }
    /*else if(CutDisCurt70.word!=0)
    {
        ErrorLever=3;
    }*/ 
    else 
    {
        ErrorLever=0;
    }  
    return ErrorLever; 
    

}
//******************************************************************************
//* Function name:   DischagerCurtCutDone
//* Description:     ���ϴ���,�й��Ϻ󽵵ͷŵ繦��
//* EntryParameter : ���ϵȼ�,��������ĵ���ֵ,Mode=1or2(30s=1,5min=2);
//* ReturnValue    : ���ͺ�ĵ���ֵ
//*��ע��            CurrentValue����ǰSOC,Tem�²���ֵ;DischargeCurrentOn���ͺ��ֵ(CurrentValue*ϵ��)
//*��ע:             CurrentDisCurrent:��ǰ�ϱ�ֵ,���������(CurrentDisCurrent=BiggestDischargePower)
//******************************************************************************
float DischagerCurtCutDone(unsigned char ErrorLever,float CurrentValue,unsigned char Mode)
{
    float DischargeCurrentOn=0;//������������
    float CurrentDisCurrent=0;//���������
    unsigned char ReturnDone=0;
    
    if(Mode==1)
        CurrentDisCurrent = BiggestDischargeCurt;//��ֵ���������,��ʾ��ǰ�Ĺ���ֵ
    else if(Mode==2)
        CurrentDisCurrent = BiggestDisCurtContinuous;//��ֵ���������,��ʾ��ǰ�Ĺ���ֵ 
           
    if(ErrorLever==3)
    {      
        DischargeCurrentOn=0;
        ReturnDone=1;
    }
    else if(ErrorLever==2)
    {      
        DischargeCurrentOn=0.5*CurrentValue;
        ReturnDone=1;
    }
    /*else if(ErrorLever==3)
    {      
        DischargeCurrentOn=0.7*CurrentValue;
        
    }*/     
    if(CurrentDisCurrent<=DischargeCurrentOn)//����������ֵС�ڵ�ǰ���͹��ʺ��ֵ
    {
        CurrentDisCurrent=CurrentDisCurrent+POWERSPEED;
        if(CurrentDisCurrent>=DischargeCurrentOn)
        {              
            CurrentDisCurrent=DischargeCurrentOn;//�ŵ����Ϊ0.2,������ָ���ϸ�ѹ
        }
        return CurrentDisCurrent;
    }
    else 
    {
        CurrentDisCurrent=CurrentDisCurrent-POWERSPEED;
        if(CurrentDisCurrent<=DischargeCurrentOn)
        {              
            CurrentDisCurrent=DischargeCurrentOn;//�ŵ����Ϊ0.2,������ָ���ϸ�ѹ
        }
        return CurrentDisCurrent;
    }
}
//******************************************************************************
//* Function name:   BigDischargePowerAdjust
//* Description:     New SOF 
//* EntryParameter : �¶ȣ�SOC����ǰ��������ʻ����ֵ
//* ReturnValue    : bigPowerCurrent��ǰ�������ŵ繦�ʻ����
//******************************************************************************
float BigDischargePowerAdjust()
{
    static float power1=0; //�ŵ繦���ݴ�ֵ1
    static float power2=0; //�ŵ繦���ݴ�ֵ2
    static unsigned char counter=0;//2�λ������ʼ���
    static unsigned char changeFlag=0;//�������Ա仯ʱ��־λ
    static unsigned char FirstOn=0;
    unsigned char ReduceCurt=0;//���ϵȼ�1,2,3�ķ���ֵ
    float BigDisPower=0; //�������ŵ繦�ʵ��м��������ֹ���õ������ж�ֱ�ӷ���
    float BigDisPowerCutDown=0;//��ͬ������,�������͵�����
    float ReturnValue=0;
    static float pc=0;//�������ŵ繦��/���� ����ֵ
    
    BigDisPower=BiggestDischargeCurt;
    
    YoungMa_BigDischargePowerAdjust();
    BigDisPower=BiggestDischargeCurt_Model;
    ReduceCurt=DischagerCurtCutOff();//�ж��Ƿ��н����ʵĹ���
    if(ReduceCurt!=0)//�й���
    {
        if(ReduceCurt==3)
            BigDisPowerCutDown=0;
        else if(ReduceCurt==2)
            BigDisPowerCutDown=0.5*BigDisPower;
        //else if(ReduceCurt==3)
        //    BigDisPowerCutDown=0.7*BigDisPower;
        pc = BigDisPowerCutDown; 
        power1 = BigDisPowerCutDown; 
        FirstOn=1; 
        ReturnValue=DischagerCurtCutDone(ReduceCurt,BigDisPower,DISCURT30s);
        return ReturnValue; 
    }
    if(FirstOn==0) //��һ�ν���
    {
        pc=BigDisPower; 
        power1 = BigDisPower; 
        FirstOn=1;         
    } 
    else//�ٴν���
    {
        if(changeFlag==0)//ԭֵΪ0������power2Ϊ�ڶ��ν���ú���ʱ��ֵ
            power2=BigDisPower;
     
        if(power1!=power2) //������߲��ȣ�˵����һ��ڶ��β���
        {
            changeFlag=1;//���ñ仯��־λ
            if(power1>power2) 
            {
                pc=power1-POWERSPEED;// ÿ20ms�仯0.5kw,7ms����һ�Σ�21ms����0.51KW
                power1=pc;
                if(power1<=power2)
                {
                    
                    pc=power2;
                    changeFlag=0; 
                }
            }
            else if(power1<power2) 
            {
                pc=power1+POWERSPEED;// ÿ5ms�仯0.05kw
                power1=pc;
                if(power1>=power2) 
                {
                    
                    pc=power2;
                    changeFlag=0;
                }
            }
        } 
        else 
        {
            changeFlag=0; 
        }
    
    } 
 
    return pc; 
    
}
//******************************************************************************
//* Function name:   BigDischargePowerContinuous
//* Description:     New SOF 
//* EntryParameter : �¶ȣ�SOC����ǰ��������ʻ����ֵ
//* ReturnValue    : bigPowerCurrent��ǰ�������ŵ繦�ʻ����
//******************************************************************************
float BigDischargePowerContinuous()
{
    static float power1=0; //�ŵ繦���ݴ�ֵ1
    static float power2=0; //�ŵ繦���ݴ�ֵ2
    static unsigned char counter=0;//2�λ������ʼ���
    static unsigned char changeFlag=0;//�������Ա仯ʱ��־λ
    static unsigned char FirstOn=0;
    unsigned char ReduceCurt=0;
    float BigDisPower=0; //�������ŵ繦�ʵ��м��������ֹ���õ������ж�ֱ�ӷ���
    float BigDisPowerCutDown=0;
    static float pc=0;//�������ŵ繦��/���� ����ֵ
    
   
    BigDisPower=BiggestDischargeCurt_Model;
    if(BigDisPower>=SetCap)
    {
        BigDisPower=SetCap;
    }
    ReduceCurt = DischagerCurtCutOff();
    if(ReduceCurt!=0)//�й���
    {
        if(ReduceCurt==3)
            BigDisPowerCutDown=0;
        else if(ReduceCurt==2)
            BigDisPowerCutDown=0.5*BigDisPower;
        //else if(ReduceCurt==3)
        //    BigDisPowerCutDown=0.7*BigDisPower;
        pc = BigDisPowerCutDown;//���ϻָ���,���Ա仯���� 
        power1 = BigDisPowerCutDown; //���ϻָ���,���Ա仯���� 
        FirstOn=1; 
        BiggestDisCurtContinuous=DischagerCurtCutDone(ReduceCurt,BigDisPower,DISCURT5min);
        return BiggestDisCurtContinuous; 
    }
    if(FirstOn==0) //��һ�ν���
    {
        pc=BigDisPower; 
        power1 = BigDisPower; 
        FirstOn=1;         
    } 
    else//�ٴν���
    {
        if(changeFlag==0)//ԭֵΪ0������power2Ϊ�ڶ��ν���ú���ʱ��ֵ
            power2=BigDisPower;
     
        if(power1!=power2) //������߲��ȣ�˵����һ��ڶ��β���
        {
            changeFlag=1;//���ñ仯��־λ
            if(power1>power2) 
            {
                pc=power1-POWERSPEED;// ÿ20ms�仯0.5kw,7ms����һ�Σ�21ms����0.51KW
                power1=pc;
                if(power1<=power2)
                {
                    
                    pc=power2;
                    changeFlag=0; 
                }
            }
            else if(power1<power2) 
            {
                pc=power1+POWERSPEED;// ÿ5ms�仯0.05kw
                power1=pc;
                if(power1>=power2) 
                {
                    
                    pc=power2;
                    changeFlag=0;
                }
            }
        } 
        else 
        {
            changeFlag=0; 
        }
    } 
    return pc; 
}
//***********************************************************************************************
//************************************************************************************************
//************************************the end*****************************************************
//************************************************************************************************
//***********************************************************************************************
//************************************************************************************************