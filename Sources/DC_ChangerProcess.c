//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : �����ֽ�6123
//* File Name          : DC_ChangerProcess.c
//* Author             : ����ѩ
//* Version            : V1.0.0
//* Start Date         : 2016,11,30
//* Description        : �������
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"

unsigned char plug_DC_Connect=0;             //CC2�����ź�
static unsigned char sendi1=0;               //��ȫ�ֱ���
static unsigned char sendi2=0;               //��ȫ�ֱ���
static unsigned char sendi3=0;               //��ȫ�ֱ���

unsigned char	m_chmmode = CONSTANT_CURT_WORK;//ȫ�ֱ���
unsigned char fastendflag=0;                 //ȫ�ֱ���,��������־λ
float	m_askvoltage=0;                        //���ݵ�ذ��������
float	m_askcurrent=0;                        //ȫ�ֱ�����ֱ��������
/////////�����,BMSֹͣ����ԭ��////////////////
unsigned char  fastend1=0;                   //ȫ��
unsigned char  fastend2=0;
unsigned char  fastend3=0;
unsigned char  fastend4=0xf0;

/////////����ã�����BSM����ص�ǰ״̬////////////////
unsigned char  fasterror11 =0;
unsigned char  fasterror12 =0x10;            //�����磬0 b 00 01 00 00

unsigned char BROStopState = 0;              //�յ�CRO��,���ٷ�BRO
unsigned int chargeTimes=0;                  //������

//////////��ʱʱ���־///////////////////////////////
unsigned int CRMOverTimeBefore60s = 0;//�ղ���CRO����60s��ϵͳ������
unsigned int CRMOverTimeBefore = 0;//�ղ���CRO����
unsigned int CRMOverTime = 0;//CRM��ʱ����0xaa��ʱ
unsigned int CROOverTime = 0;//5sδ�յ�CRO����
unsigned int CROOverTime60s = 0;//CRO��ʱ����0xaa��ʱ
unsigned int CCSOverTime=0;                 //CCS��ʱʱ��
unsigned int CSTOverTime=0;                 //CST��ʱʱ��
unsigned int CSDOverTime=0;                 //CSD��ʱʱ��
unsigned char OverTimeState=0;               //��ʱ��־

unsigned char CC2ResState = 0; //0λ��״̬ 1����״̬ 2����״̬
float DCTem1=0;//DC1�¶Ȳɼ�
float DCTem2=0;//DC2�¶Ȳɼ�
float CC2VOL = 0;
float PowerVOL = 0;
//unsigned char DC_CC2Count = 0;
unsigned char BROErrorAA = 1;//BRO������0xaa
unsigned char DC_Vesion = 1;//DC���汾1���Ϲ���;2���¹���
unsigned char SelfState3 = 0;     //�Լ���������3��־
unsigned char BEMStop=0;

unsigned char DCStartState=0;//��俪ʼ��־:1�յ�CRM,2�յ�CHM

static unsigned char SetBSD = 0;//BSD�Ƿ��Ѿ�����
static unsigned char SetBST = 0;//BST�Ƿ��Ѿ�����
unsigned char ChargerStopState = 0;//���ʱ���û�г�������SOCͣ����99.2%
unsigned char PantNormalEndFlag=0;//�ܵ繭�����µ��־λ
unsigned char g_received_CST = 0;
unsigned char g_BST_send_ok = 0;

//******************************************************************************
//* Function name:   GetRequestCurrentDC
//* Description:     ���������������� C=120A
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
float GetRequestCurrentDC(void) 
{
    //float DCAmp;
    static unsigned char ChanceHappen1 = 0;//��ѹ����3.78�󲻻ָ�
    static unsigned char ChanceHappen2 = 0;//��ѹ����3.92�󲻻ָ�
    
    m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;
}
//**********************************************************************
//* Function name:   DCFaultDone
//* Description:     �������й��ϵĴ���
//* EntryParameter : None
//* ReturnValue    : 0:�޹���;1:�й���
//**********************************************************************
unsigned char DCFaultDone(void)
{
    static unsigned char ReturnValue = 0;//һ����ֵ,���ܸı�
    //////////////////////////////�ϸ�ѹ/////////////////////////////        
    if((Error_Group4.Bit.F6_Bat_Over_V>=2)//�ܵ�ѹ��ѹ2����3��
    ||(Error_Group2.Bit.F0_Cell_Over_V>=1))//�����ѹ1����2����3��
    
    {
        fastendflag = 1;
        fastend4 |= 0x04;//��ѹ�쳣
        OffState = 1;//�����µ�
        ReturnValue = 1;
    }

	if(Error_Group1.Bit.F4_Ch_Over_I==3) //������3��
    {
        fastendflag = 1;
        fastend4 |= 0x01;//����������
        OffState = 1;//�����µ�
        ReturnValue = 1;     
    } 

	if((Error_Group1.Bit.F6_Ins_Err==3)&&(g_BmsModeFlag == FASTRECHARGING))//��Ե3������
    {
        fastendflag = 1;
        fastend2 |= 0x01;//��Ե�쳣
        OffState = 1;//�����µ�
        ReturnValue = 1;    
    }

	if(Error_Group0.Bit.F2_Ele_Relay_Con==3)
    {
        fastendflag = 1;
        fastend2 |= 0x40;//�������¶ȹ���
        OffState = 1;//�����µ�
        ReturnValue = 1; 
    } 

	if(Error_Group2.Bit.F4_Temp_Over==3)//���ǳ���3���¶ȹ��߹���
    {
        fastendflag = 1;
        fastend3 |= 0x01;//����¶ȹ���
        OffState = 1;//�����µ�
        ReturnValue = 1;
    } 

	if(((g_caution_Flag_2 & 0x80) != 0)//����ͨ�Ź���
	||(Error_Group3.Bit.F0_Sub_Com_Err)//����ͨѶ����
	||(ACCha_Flag_BST)//���ʱ���������쳣
	||(Error_Group0.Bit.F0_Fire_Warning == 3)//����Ԥ��3�� 
//	||((CHMStep != 0) && (g_BmsModeFlag == RECHARGING) &&(
//	||(VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
//	||(VCU_ChgControl.Bit.downC_OK == 1)
//	||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
//	||(VCU_ParkBrake.Bit.Parking_Brake==0)
//	))
    ){
        fastendflag = 1;
        fastend3 |= 0x40;//��������
        OffState = 1;//�����µ�
        ReturnValue = 1;
    }

	if(Error_Group5.Bit.F6_Cell_Under_T == 3
	||g_bms_fault_msg.fault.Cell_T_Low_Flt == 3){
		fastendflag = 1;
	}
 
	return ReturnValue;   
	//////////////////////////////*�ϸ�ѹ*/////////////////////////////
}

//**********************************************************************
//* Function name:   TaskRechargeDC
//* Description:     ������̣���Լÿ10ms����һ�θú���
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void TaskRechargeDC(void)
{
    static unsigned char counter_250ms1=0;
    static unsigned char CHMStep1=0;
    
    static unsigned char counter_250ms2=0;
    static unsigned char CHMStep2=0;
    
    static unsigned char counter_250ms3=0;
    static unsigned char CHMStep3=0;
    
    static unsigned char counter_250ms4=0;
    static unsigned char CHMStep4_5=0;
    static unsigned char counter_50ms=0;
    
    static unsigned char counter_250ms5=0;
    static unsigned char CHMStep5=0;
    
    static unsigned char counter_250ms6=0;
    static unsigned char counter_250ms6_1=0;
    static unsigned char CHMStep6=0;
    
    static unsigned char counter_250ms7=0;
    
    static unsigned char DCFinish=0; //ֱ�����ʱֻ����Q1һ��
    static unsigned char timer1S=0;//�����ѹ�����,ֹͣ��������ж���ʱ
    static unsigned char timer2S=0;//��һ�ν�������ʱ
    static unsigned char timer3S=0;//�ڶ��ν�������ʱ
    static unsigned char ChanceCurt=0;//���ĩ��,�����仯�󲻻���,
    static unsigned char ChangeTimeState=0;//���������Խ���һ�μӷ�
    float curr=0;
    float curr1=0;
	static U16 counter_30s = 0;
    unsigned char ErrorState = 0;
	#define DECERASE_CURRENT_STEP	5 // decrease current step is 5A each time.
	#define DECERASE_CURRENT_STEP_2 2 // decrease current step is 2A each time.
	
    if((g_BmsModeFlag != FASTRECHARGING)&&(g_BmsModeFlag != RECHARGING))
        return;
    
//    if(g_highestTemperature>=(HIGHEST_ALLOWED_CHARGE_T+40))//40+57��40+0
//    {
//        fastend3|=0x01; //Temperature����ֹ���
//        fastendflag=1; //���ֹͣ����־  
//    }
    
    YoungMan_LT_FastChrgPowerAjust();
    curr =m_askcurrent_Model;
    ErrorState = DCFaultDone();
	if(ChanceCurt == 0)
	{
		curr1 = curr;
	}
	else
	{
		curr1 = m_askcurrent;
	}
	
	if(g_BmsModeFlag == FASTRECHARGING) 
	{
		////////////�����ѹ//////////
		if(((Tavg-40) <= 54) && ((Tavg-40) >= 0))
		{
		    m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;	//3.65*204 = 744.6
		}
        else	//if((Tavg-40) > 54)
        {
            m_askvoltage = 0;
        }
    }
    else if(g_BmsModeFlag == RECHARGING) 
    {
        m_askvoltage = HIGHEST_ALLOWED_CHARGE_V;
    }
     
    if((ErrorState==1)||(CutDCChaCurt0.word!=0)||(CutACChaCurt0.word!=0))//if fault occured.
    {
        curr1 = 0;
    }
    else 
    {
		if(g_BmsModeFlag == RECHARGING){
	        if(curr >= EleBand_Max_Current){
	            curr = EleBand_Max_Current;
				curr1 = curr;
	        }
		}
		else if(g_BmsModeFlag == FASTRECHARGING){
			if(curr >= FastChg_Max_Current){
				curr = FastChg_Max_Current;
				curr1 = curr;
			}
		}
		
        if((CutDCChaCurt50.word != 0) || (CutACChaCurt50.word != 0)){
            curr1 = 0.5 * curr;
        }
    }
    
	if(g_highestCellVoltage >= HIGHEST_ALLOWED_CHARGE_CV)  //3.65V = stop charge, charge finished.
	{
        timer1S++;
        if(timer1S >= 100)//10ms*100=1000ms
        {
            if(DCFinish == 0)
            {
				ChargerStopState = 1;
				SocRechargeAdjust(); //SOC adjust and Q1 adjust.
				StoreParameter();

				DCFinish = 1;
            }
			
			fastendflag = 1;
			fastend1 |= 0x10;//�����ѹ�ﵽĿ��ֵ��������ֹ���
			timer1S = 103; 

			curr1 = 0; //���������Ϊ0
			m_askvoltage = 0;//�����ѹ��Ϊ0
        }
    } 
	else if(g_highestCellVoltage >= CHARGE_CUTDOWN_CV2)//����Ϊ����������һ��
	{
		timer3S++;
		if(timer3S >= 100)//10*100 = 1000ms = 1s
		{
			timer3S = 0;
			if(curr1 > 0.1 * SetCap)
			{
				curr1 -= DECERASE_CURRENT_STEP_2; // decrease current step by step. 2A
			}
            else{
				//keep the current.
            }
		}
		ChanceCurt = 1;
	}
	else if(g_highestCellVoltage >= CHARGE_CUTDOWN_CV1)//����Ϊ����������һ��
    {
        timer2S++;
        if(timer2S >= 100) //10*100 = 1000ms = 1s
        {
            timer2S = 0;
            if(curr1 > 0.5 * SetCap){
                curr1 -= DECERASE_CURRENT_STEP; // decrease current step by step.5A
            }
            else{
				//keep the current.
            }
        }
		ChanceCurt = 1;
    }
	
    m_askcurrent = curr1;
	
    //////////////////////////////���ʱ���͹���///////////////////////////
	if(CHMStep)
	{
		if(CHMStep == 0x01)   //1801f456 00,��֡���ģ�����250ms�����ļ��10ms
		{
			counter_250ms1++;
			if(counter_250ms1 >= 25)
			{
				if(DCStartState == 2)//new GB-T start flag, reveiced CHM
				{
					cpuToCHMBHM();
					g_BST_send_ok = 0;
					CRMOverTimeBefore++;
					if(CRMOverTimeBefore >= 120)//���30s���ղ���CRM��ʱ 
					{
						OverTimeState=1;//��ʱ��־λ��1                
						CHMStep=0x07;
						BEMError1|=0x01;//�ղ���CRM��30s����BEM
						CRMOverTimeBefore = 0;
					}
				}
				
				if(DCStartState==1)////old GB-T start flag, reveiced CRM, detect whether 0xaa overtime.
				{
					CRMOverTime++;
					if(CRMOverTime>=20)//�ղ���0xaa5s���ϱ�
					{
						CHMStep=0x07;
						OverTimeState=1;
						BEMError1|=0x04;
						CRMOverTime = 0; 
					} 
				}
				
				CHMStep1=0;
				counter_250ms1=0;
    	      } 
			
			if(CHMStep1==0)
			{ 
				if(DCStartState==1)//����Ѿ����յ��ɹ��꿪ʼ
				{
					if(BRMStep == 0){ // if bms can send BRM, then continue send.
						BRMStep = 1; // if received CTS, clear it, set 1 when have sent BRM
						cpuToCHMBRM();
						counter_250ms1=0;
						sendi1++;
					}
					else if((BRMStep == 1)){// && (FlagBRMSend==1)
						sendi1++;
						if(sendi1==2)
						  cpuToCHMBRMDATA1();
						else if(sendi1==3)
						  cpuToCHMBRMDATA2();
						else if(sendi1==4)
						  cpuToCHMBRMDATA3();
						else if(sendi1==5)
						  cpuToCHMBRMDATA4();
						else if(sendi1==6)
						  cpuToCHMBRMDATA5();
						else if(sendi1==7)
						{
							cpuToCHMBRMDATA6();
							if(DC_Vesion==1)
							{
								sendi1=0;
								CHMStep1=1;
								FlagBRMSend = 0;
								BRMStep = 0;//接收到CRM后开始发送BRM以及下次发送BRM的时�?
							}
						}
						else if(sendi1==8)
						{
							cpuToCHMBRMDATA7();
							sendi1=0;        			          
							CHMStep1=1;
							FlagBRMSend = 0;
							BRMStep = 0;//接收到CRM后开始发送BRM以及下次发送BRM的时�? 
						}
					}
				}
			}
		}
		if(CHMStep==0x02)   //1801f456 aa
		{
			counter_250ms2++;
			if(counter_250ms2>=50)//500ms
			{
				CHMStep2=0;
				counter_250ms2=0;
			}
			if(CHMStep2==0)// && FlagBCPSend == 1
			{
				sendi2++;
				if(sendi2 == 1)
					cpuToCHMBCP();
				if(sendi2 == 2)
					cpuToCHMBCPDATA1();
				else if(sendi2 == 3)
				{
					sendi2=0;
					cpuToCHMBCPDATA2();
					CHMStep2=1;
				}
				sendi1=0;//�����һ�����
			}
		}
		if(CHMStep==0x03)  //1808f456 aa 250ms
		{            
			counter_250ms3++;
			if(CHMStep3==0)
			{
				CROOverTime++;
				if(SelfState3==0){
					cpuToCHMBRO();  //100956f4 aa
//					CloseNegControl=1;
				}
				if((CROOverTime>=20)||(CROOverTime60s>=240))//�յ�CRO,��û���յ�0xaa,5s��ʱ 
				{
					OverTimeState=1;//��ʱ��־λ��1                
					CHMStep=0x07;
					BEMError2|=0x04;
					CROOverTime = 0;
					CROOverTime60s = 0;
                }
                if(BROErrorAA==1)//���׮������������
                {
                    CHMStep=0x07;
                    BEMError4|=0x04;//��������                
                } 
                else if(0==BROErrorAA)
                {
                    CROOverTime60s++;
                    TurnOnChangerRelay = 1;
                }
                   
                CHMStep3=1;
			}
			
            if(counter_250ms3>=25)//ȷ����һ֡���ݷ�������������ʱС��250ms
            {
              
  			        counter_250ms3=0;
  			        CHMStep3=0;
  			        
            }
  		  }
		if((CHMStep==0x04)||(CHMStep==0x05)) //50ms period
		{
			CCSOverTime++;
			if(CCSOverTime>=100)//10ms*100=1S
			{
				OverTimeState=1;//��ʱ��־λ��1
				if(CHMStep<=0x06){                
					CHMStep=0x06;
				}
                BEMError3|=0x01;
                CCSOverTime = 0;    
            }
            if(counter_50ms>=5)//50ms 
            {			            
                cpuToCHMBCL();    // voltage request and current request
                counter_50ms=0;
            }
			counter_50ms++;
			counter_250ms4++;      //250ms
			if(counter_250ms4>=25)
			{
				CHMStep4_5=0;
				counter_250ms4=0;
			}
			if(CHMStep4_5==0)
			{
				sendi1++;
				if(sendi1 == 1){
					cpuToCHMBCS();
				}
				else if(sendi1 == 2){
					cpuToCHMBCSDATA1();
				}
				else if(sendi1 == 3){
					cpuToCHMBCSDATA2();
					sendi1 = 0;
					CHMStep4_5 = 1;
				}
			}
			if(CHMStep==0x05) 
			{
				counter_250ms5++;
				if(CHMStep5==0)
				{
					cpuToCHMBSM();
					CHMStep5=1;
                }
                if(counter_250ms5>=25)//ȷ����һ֡���ݷ�������������ʱС��250ms
                {
					counter_250ms5=0;
					CHMStep5=0;
                }
            }
		}	  
		if(((CHMStep==0x06)||(CHMStep==0x05))
			&&(
				(fastendflag == 1) 
				||(PantographOff == 1) 
				||(OffState == 1) 
				||(g_BmsModeFlag == RECHARGING && acc_Connect == 0)
				||(g_received_CST == 1)
				||(stateCode == 110 && ((VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
	            ||(VCU_ChgControl.Bit.downC_OK == 1)
	            ||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
	            ||(VCU_ParkBrake.Bit.Parking_Brake==0)))
			)
			){//10ms����һ��,��ʼ����ʱ��	
			//����յ������������ֹ���Ļ��ߵ����ѹ���ܵ�ѹ��������ֵ
			if(OverTimeState==1)
			{
				if(counter_250ms6_1%25==0)
				{
					counter_250ms6_1 = 0;
					cpuToCHMBEM();
				}
				counter_250ms6_1++;
			}
			if(SetBSD==0)//���BSD�Ѿ�����,��BST���ٷ���
			{
				cpuToCHMBST();
				g_BST_send_ok = 1;
				m_askcurrent=0;//�������Ϊ0
				CSTOverTime++;//10ms*200
				if(CSTOverTime>=500)//��ֹ�ղ���CST����5s��ʱ,10ms*500 
				{
					CHMStep=0x07;
					OverTimeState = 1;
					BEMError3|=0x02;
					CSTOverTime = 0;
				}
			}
			if(CHMStep==0x06)   
			{
				if(CHMStep6==0)
				{   
					cpuToCHMBSD();  // report BMS status: the highest and the lowest cell voltage, temperature and SOC.
					SetBSD = 1;
					CHMStep6=1;
				}
                counter_250ms6++;
				if(counter_250ms6>=25)
				{
					counter_250ms6=0;
					CHMStep6=0;			                       
				}
				
				CSDOverTime++;
				if(CSDOverTime>=1000)//���ղ�������ͳ�Ʊ���ʱ,��ʱ10s�µ�
				{
					CHMStep=0x07;
					OverTimeState = 1;
					BEMError4|=0x01;
					CSDOverTime = 0;     			          
				}    			      
			}
		}
		
		if(CHMStep==0x07) //�������� 
		{
			SelfState3=0;//之前收到CRO以后再也不发送了，流程如果重新来，需要清�?
			if(counter_250ms7 % 25==0)
			{
				if((OverTimeState==1)||(BROErrorAA==1))//����г�ʱ���ϲ��ϱ�,û�в��ϱ�
				{	                
					if(BEMStop==0)
					cpuToCHMBEM();
				}
				counter_250ms7 = 0;
			}
			counter_250ms7++;
			if(BEMStop == 1){
				counter_30s++;
				if(counter_30s>3000)//30S以内 3000*10ms=30S 
				{
					OffState = 1;//״̬����170��ת40�ı�־λ 
					Error_Group1.Bit.St_CHG_Allow = 1; //charge not allowed
					PantNormalEndFlag=1;//�ܵ繭�����µ�			  
					counter_30s=3002;
				}
			}
			else{
				OffState=1;//状态机�?70跳转180的标志位 
				Error_Group1.Bit.St_CHG_Allow=1; //charge not allowed
				PantNormalEndFlag=1;//受电弓正常下�?
			}
		}
	}/////////END��䷢����Ϣ
}
//******************************************************************************
//* Function name:   TaskDC
//* Description:     10ms�ж��е��øú���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void TaskDC(void)
{
    if((FASTRECHARGING==g_BmsModeFlag)||(RECHARGING==g_BmsModeFlag)) ////�����ܵ繭���
    {
        if(DCStartState == 0)//�����յ�֮�󣬲��ټ�ʱ
        {
			CRMOverTimeBefore60s++;
			if(FASTRECHARGING == g_BmsModeFlag){
				if(CRMOverTimeBefore60s >= 6000)//10ms*6000 == 60 000 ms = 60s = 1min
				{
					OverTimeState = 1;//��ʱ��־λ��1 			   
					CHMStep = 0x07;
					BEMError1 |= 0x01;//�ղ���CRM��30s����BEM
					CRMOverTimeBefore60s = 0;	   
				}
			}
			else if(RECHARGING == g_BmsModeFlag){
				if(CRMOverTimeBefore60s >= 60000)//10ms*60000 = 600 000ms = 600s = 10min
				{
					OverTimeState = 1;//��ʱ��־λ��1 			   
					CHMStep = 0x07;
					BEMError1 |= 0x01;//�ղ���CRM��30s����BEM
					CRMOverTimeBefore60s = 0;	   
				}
			}
        }
		
        if((stateCode==110)||(stateCode==170)|| (DCStartState != 0) ||(OverTimeState==1)){
            TaskRechargeDC();
        }
    }
}

//******************************************************************************
//* Function name:   GetDCTem
//* Description:     ���ֱ�������¶�
//* EntryParameter : None
//* ReturnValue    : None
	
	//	TurnOn_CC2();//12V   ��ѹ�л��ڳ�ʼ��BMS��
//******************************************************************************
void GetDCTem (void)
{
    float getAD1=0;//ADֵ1
    float getAD2=0;//ADֵ2
    unsigned int GetADVol1 = 0;
    unsigned int GetADVol2 = 0;
    float VolValue1=0;//ת����ĵ�ѹֵ1 �¶�1
    float VolValue2=0;//ת����ĵ�ѹֵ2 �¶�2
    float VolValue3=0;//ת����ĵ�ѹֵ1 CC2��ѹ
    float VolValue4=0;//ת����ĵ�ѹֵ2 ��Դ��ѹ
    
    float R1=0;//�¸�ͷ1��ֵ
    float R2=0;//�¸�ͷ2��ֵ  

    getAD1 = readADC_SingleEnded(0); //T1
    getAD2 = readADC_SingleEnded(3); //T2
    
    GetADVol1 = readADC_SingleEnded(1);//Vol of CC2
    GetADVol2 = readADC_SingleEnded(2); //Vol of ��Դ
    //EnableInterrupts; //���жϣ�����״̬������ 
    VolValue1=4.096*(float)getAD1/2048.0;     //(2048-0)/(4.096-0)=(AD1-0)/(ADX-0)
    VolValue2=4.096*(float)getAD2/2048.0;
    
    VolValue3=4.096*(float)GetADVol1/2048.0; //cc2
    VolValue4=4.096*(float)GetADVol2/2048.0; //��Դ��ѹ
    
    CC2VOL = (75.0+12.0)*VolValue3/12.0; //CC2��ѹ
    PowerVOL = (75.0+12.0)*VolValue4/12.0; //��Դ��ѹ
 
    R1=VolValue1/(5-VolValue1)*10;
    R2=VolValue2/(5-VolValue2)*10;
    
    DCTem1=lookup_Table_Tem_NEW20170204_custom(VolValue1);
    
    DCTem2=lookup_Table_Tem_NEW20170204_custom(VolValue2);
    
    
}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
