
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : s133
//* File Name          : FeedBack.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2015.7.13
//* Description        : ͷ�ļ�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "BMS20.h"
//******************************************************************************
//* Function name:   ChagerCurtCutOff
//* Description:     ���ϴ���,�й��Ϻ󽵵ͳ�繦��
//* EntryParameter : void
//* ReturnValue    : ���ϵȼ���1 ��Ϊ0��2��Ϊ20%; 3��Ϊ50%
//******************************************************************************
unsigned char ChagerCurtCutOff(void)
{
    unsigned char ErrorLever=0;
   
    if(CutChaCurt0.word!=0)
    {
        ErrorLever=3;
        //state_group3.Bit.St_Power_Reduce = 1;//�����ʱ�־λ
    } 
    else if(CutChaCurt50.word!=0)
    {
        ErrorLever=2;
        //state_group3.Bit.St_Power_Reduce = 1;//�����ʱ�־λ
    } 
    /*else if(CutChaCurt70.word!=0)
    {
        ErrorLever=3;
        //state_group3.Bit.St_Power_Reduce = 1;//�����ʱ�־λ
    } */
    else
    {
        ErrorLever=0;
       // if((CutDisCurt0.word==0)&&(CutDisCurt50.word==0))
        //    state_group3.Bit.St_Power_Reduce = 0;//�����ʱ�־λ
    } 
    return ErrorLever; 
}
//******************************************************************************
//* Function name:   ChagerCurtCutDone
//* Description:     ���ϴ���,�й��Ϻ󽵵ͳ�繦��
//* EntryParameter : ���ϵȼ�,��������ĵ���ֵ,Mode=1or2(30s=1,5min=2);
//* ReturnValue    : ���ͺ�ĵ���ֵ
//*��ע��            CurrentValue����ǰSOC,Tem�²���ֵ;DischargeCurrentOn���ͺ��ֵ(CurrentValue*ϵ��)
//*��ע:             CurrentDisCurrent:��ǰ�ϱ�ֵ,���������(CurrentDisCurrent=BiggestDischargePower)
//******************************************************************************
float ChagerCurtCutDone(unsigned char ErrorLever,float CurrentValue,unsigned char Mode)
{
    float ChargeCurrentOn=0;//������������
    float CurrentChaCurrent=0;//���������
    unsigned char ReturnDone=0;
    
    if(Mode==1)
        CurrentChaCurrent = BiggestFeedbackCurt;//��ֵ���������,��ʾ��ǰ�Ĺ���ֵ
        //CurrentChaCurrent = BiggestFeedbackPower;//��ֵ���������,��ʾ��ǰ�Ĺ���ֵ
    else if(Mode==2)
        CurrentChaCurrent = BiggestFeedbackCurtContinuous;//��ֵ���������,��ʾ��ǰ�Ĺ���ֵ 
        //CurrentChaCurrent = BiggestFeedbackContinuous;//��ֵ���������,��ʾ��ǰ�Ĺ���ֵ 
           
    if(ErrorLever==3)
    {      
        ChargeCurrentOn=0;
        ReturnDone=1;
    }
    else if(ErrorLever==2)
    {      
        ChargeCurrentOn=0.5*CurrentValue;
        ReturnDone=1;
    }
    /*else if(ErrorLever==3)
    {      
        ChargeCurrentOn=0.7*CurrentValue;
    }*/ 
        
    if(CurrentChaCurrent<=ChargeCurrentOn)//����������ֵС�ڵ�ǰ���͹��ʺ��ֵ
    {
        
        CurrentChaCurrent=CurrentChaCurrent+POWERSPEED;
        if(CurrentChaCurrent>=ChargeCurrentOn)
        {              
            CurrentChaCurrent=ChargeCurrentOn;//�ŵ����Ϊ0.2,������ָ���ϸ�ѹ
        }
        return CurrentChaCurrent;
    }
    else 
    {
        CurrentChaCurrent=CurrentChaCurrent-POWERSPEED;
        if(CurrentChaCurrent<=ChargeCurrentOn)
        {              
            CurrentChaCurrent=ChargeCurrentOn;//�ŵ����Ϊ0.2,������ָ���ϸ�ѹ
        }
        return CurrentChaCurrent;
    }
}
//******************************************************************************
//* Function name:   PulseRechargePowerAdjust1
//* Description:     ����ƽ���¶ȵ�����������������������30s
//* EntryParameter : tem, soc ƽ���¶ȣ�SOCֵ
//* ReturnValue    : fc ���صĻ�������ֵ  
//******************************************************************************
float PulseRechargePowerAdjust1(void) 
{   
    static float current1=0; //���������ݴ�ֵ1
    static float current2=0; //���������ݴ�ֵ2;
    //static unsigned int leftTime=0;
    static float fc=0;//���صĻ�������ֵ
    float BigCharCurrent = 0; 
    static unsigned char FirstTime=0;
    static unsigned char changeFlag=0;
    unsigned char ReduceCurt=0;//�������ĵȼ�
    float BigChaPowerCutDown;//������������,���ڹ��ϻָ������Ա仯 
    float ReturnValue=0;
    
    YoungM_PulseRechargePowerAdjust();//����
    BigCharCurrent = BiggestFeedbackCurt_Model;
     
    /*if(Tavg>0+40)
    { 
        if((BigCharCurrent*g_highVoltageV1/1000)>=100)
            BigCharCurrent=100000/g_highVoltageV1; 
    } 
    else if(Tavg>=(-15+40))
    {
        if((BigCharCurrent*g_highVoltageV1/1000)>=20)
            BigCharCurrent=20000/g_highVoltageV1; 
    }*/
    
    fc=BigCharCurrent;//for test
    
    ReduceCurt=ChagerCurtCutOff();//get the fault level of reduce power fault.
    if(ReduceCurt!=0)//�й���
    {
        if(ReduceCurt==3)
            BigChaPowerCutDown=0;
        else if(ReduceCurt==2)
            BigChaPowerCutDown=0.5*BigCharCurrent;
        
        fc = BigChaPowerCutDown; 
        current1 = BigChaPowerCutDown; 
        FirstTime=1; 
        ReturnValue=ChagerCurtCutDone(ReduceCurt,BigCharCurrent,DISCURT30s);
        return ReturnValue;   
    }
	
    if(FirstTime==0) //��һ�ν���,ֻ����һ��
    {
        fc=BigCharCurrent; 
        current1 = BigCharCurrent; 
        FirstTime=1;         
    }
    else//�ٴν���
    {
        if(changeFlag==0)//ԭֵΪ0������current2Ϊ�ڶ��ν���ú���ʱ��ֵ
        {
            current2=BigCharCurrent;//���µĵ���ֵ
        }
        if(current1!=current2) //������߲��ȣ�˵����һ��ڶ��β���
        {
            changeFlag=1;//���ñ仯��־λ
            if(current1>current2) 
            {
                fc=current1-POWERSPEED;// ÿ20ms�仯0.5kw,7ms����һ�Σ�21ms����0.51KW
                current1=fc;
                if(current1<=current2)
                {
                    fc=current2;
                    changeFlag=0; 
                }
            }
            else if(current1<current2) 
            {
                fc=current1+POWERSPEED;// ÿ5ms�仯0.05kw
                current1=fc;
                if(current1>=current2) 
                {
                    
                    fc=current2;
                    changeFlag=0;
                }
            }
        } 
        else 
        {
            changeFlag=0; 
        }
    }
 
    return fc; 
}

//******************************************************************************
//* Function name:   ContinueRechargeCurt
//* Description:     ����ƽ���¶ȵ�����������������������5min
//* EntryParameter : tem, soc ƽ���¶ȣ�SOCֵ
//* ReturnValue    : fc ���صĻ�������ֵ  
//******************************************************************************
float ContinueRechargeCurt(void) 
{   
	static float current1=0; //���������ݴ�ֵ1
	static float current2=0; //���������ݴ�ֵ2;
//	static unsigned int leftTime=0;
	static float fc=0;//���صĻ�������ֵ
	float BigCharCurrent = 0;

	static unsigned char FirstTime=0;
	static unsigned char changeFlag=0;
//	static unsigned int ii=0;
//	static unsigned int jj=0;
//	static unsigned char TimeoutFlag1=0;
//	static unsigned char TimeoutFlag2=0;
//	static unsigned char CloseFlag=0;
    
    unsigned char ReduceCurt=0;//�������ĵȼ�
    float BigChaPowerCutDown;//������������,���ڹ��ϻָ������Ա仯    
        
    //BigCharCurrent = feedbacksustain_20161129_qingnian_custom(soc,tem);
    
    BigCharCurrent = BiggestFeedbackCurt_Model;
    
    if(BigCharCurrent>=SetCap)
    {
        BigCharCurrent=SetCap;
    }
       
    ReduceCurt=ChagerCurtCutOff();//�ж��Ƿ��н����ʵĹ���
    if(ReduceCurt!=0)//�й���
    {
        if(ReduceCurt==3)
            BigChaPowerCutDown=0;
        else if(ReduceCurt==2)
            BigChaPowerCutDown=0.5*BigCharCurrent;
        
        fc = BigChaPowerCutDown; 
        current1 = BigChaPowerCutDown; 
        FirstTime=1;
        BiggestFeedbackCurtContinuous=ChagerCurtCutDone(ReduceCurt,BigCharCurrent,DISCURT5min);
        return BiggestFeedbackCurtContinuous;   
    }

    if(FirstTime==0) //��һ�ν���,ֻ����һ��
    {
        fc=BigCharCurrent; 
        current1 = BigCharCurrent; 
        FirstTime=1;         
    }
    else//�ٴν���
    {
        if(changeFlag==0)//ԭֵΪ0������current2Ϊ�ڶ��ν���ú���ʱ��ֵ
        {
            current2=BigCharCurrent;//���µĵ���ֵ
      
        }
        
        if(current1!=current2) //������߲��ȣ�˵����һ��ڶ��β���
        {
            changeFlag=1;//���ñ仯��־λ
            if(current1>current2)
            {
                fc=current1-POWERSPEED;// ÿ20ms�仯0.5kw,7ms����һ�Σ�21ms����0.51KW
                current1=fc;
                if(current1<=current2)
                {
                    fc=current2;
                    changeFlag=0; 
                }
            }
            else if(current1<current2) 
            {
                fc=current1+POWERSPEED;// ÿ5ms�仯0.05kw
                current1=fc;
                if(current1>=current2) 
                {
                    
                    fc=current2;
                    changeFlag=0;
                }
            }
        } 
        else 
        {
            changeFlag=0; 
        }
    }
 
    return fc; 
}