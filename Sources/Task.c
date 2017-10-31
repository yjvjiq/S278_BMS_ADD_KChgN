//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : Task.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.5.7
//* Description        : ���ļ�������ϵͳ�������񻮷ֲ��������й���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h" 



//**************************************************************************************
//* Variable definition                            
//**************************************************************************************/
//�����б�
static TASK_COMPONENTS TaskComps[] = 
{ 
    {0, 1,      1,      TaskCurrentDetect},         // ����������� 1ms���һ�ε���ֵ //0 ��־λ��1��
    {0, 700,    700,    TaskSocProcess},            // SOC����
    {0, 297,    300,    TaskBmuProcess},            // BMU���ݴ�������//��ǰ60ms
    {0, 17,     20,     TaskVoltage},               // �ܵ�ѹ����   .
//    {0, 297,	300,	TaskInsulation},            // ��Ե������� //�ô�ʱ����С600MS������̫С  80ms
	{0, 2097,	2100,	TaskInsulation},			// ��Ե������� //�ô�ʱ����С600MS������̫С  80ms
    {0, 7,     10,    TaskReport2PC},             // ���ͱ��ĵ���λ��
    {0, 5,      5,      TaskStatusMachine},         // ״̬������
    {0, 997,    1000,   TaskFaultProcess},          // 100ms���ϴ���
    //{0, 8,      8,    TaskRechargeDC},            // ֱ����� ���ܷſ�
    {0, 9,      10,     TaskRecordProcess},         // ��ʷ��¼����
	
};

//**************************************************************************************
//* FunctionName   : TaskRemarks()
//* Description    : �����־����
//* EntryParameter : None
//* ReturnValue    : None
//**************************************************************************************/
void TaskRemarks(void)
{
    uchar i;

    for (i=0; i<TASKS_MAX; i++)          // �������ʱ�䴦��
    {
         if (TaskComps[i].Timer)          // ʱ�䲻Ϊ0
        {
            TaskComps[i].Timer--;         // ��ȥһ������
            if (TaskComps[i].Timer == 0)       // ʱ�������
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;       // �ָ���ʱ��ֵ��������һ��
                 TaskComps[i].Run = 1;           // �����������
            }
        }
   }
}
//**************************************************************************************
//* FunctionName   : TaskProcess()
//* Description    : ������
//* EntryParameter : None
//* ReturnValue    : None
//**************************************************************************************/
void TaskProcess(void)
{
    uchar i;

    for (i=0; i<TASKS_MAX; i++)           // �������ʱ�䴦��
    {
        if (TaskComps[i].Run)           // ʱ�䲻Ϊ0
        {
             TaskComps[i].TaskHook();         // ��������
             TaskComps[i].Run = 0;          // ��־��0
        }
    }   
}
//************************************************************************
//* Function name:TaskCurrentDetect
//* Description:ͨ������ADͨ����ȡADֵ���پ�����ʽת���ɵ��������ݷ�Χȡ����һ������
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskCurrentDetect(void)//
{
    
    turnOnADC(); //��ADC�ж�
	  delay(10);  
	  if((Int_Flag&0x01)==0x01) //����32��ADֵ��ż���
        g_systemCurrent = CurrentCalculation();

}  
//************************************************************************
//* Function name:TaskSocProcess
//* Description:ÿ����㲢����SOC��ÿ���ӱ���һ����ʷ��¼
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskSocProcess(void)
{
    
    static float ahCharge1A=0;
    static float ahDischarge1A=0;
    static unsigned char DCfinish=0;

    unsigned char i=0;
    float ft=0;
	  	  	  
    I2CReadDate();  //��ȡϵͳʱ��

    if(CurrentTime[0]!=g_oldTime[0])   //ÿ���Ӽ���һ��SOC��ֵ
    {
        g_oldTime[0] = CurrentTime[0];//��
        SocIntegral();//������ֵõ���SOCֵ	   
        g_energyOfUsed = 0;	    		
        StoreSocRealvalue();//ÿ�뱣��SOCֵ
        //if(g_errorRecordRead_flag==0)
            //StoreSysVariable();//for test

    } //end of ÿ����
				
    if(CurrentTime[1]!=g_oldTime[1])// ÿ����SOCֵ����ϵͳ��������Ϊ�����ͨ��
    {
        g_oldTime[1] = CurrentTime[1];//��

        g_sysPara[PARA_SOC_VALUE] = g_socValue;

        if((g_BmsModeFlag == DISCHARGING)&&(StoreAHState==1))
        {
            ft=(StoreAHSOC-g_socValue)*SetCap;//�����ۻ���ŵ�AH����
            if(ft>1)
            {
                dischargeAH += (unsigned int)ft;
                StoreAHSOC=First_g_socValue;
            }
        }
        else if((g_BmsModeFlag == FASTRECHARGING)&&((StoreAHState==1)))
        {
            ft=(g_socValue-StoreAHSOC)*SetCap;//�����ۻ���ŵ�AH����
            if(ft>1)
            {
                chargeAH += (unsigned int)ft;
                StoreAHSOC=First_g_socValue;
            }
        }
			  
        //First_g_socValue=StoreAHSOC;      

        //����״̬����
        if(g_errorRecordRead_flag==0)
            StoreSysVariable();//ÿ���ӱ�����ϼ�¼��Ϣ
        if(StoreAHState==1)    	
            StoreChargeDischargeAH();//�����ۻ���ŵ�������	  
        sendBMSIDtoBMU(); //����BMS�汾ID�Ÿ�BMU
        sendRealtimeToBMU(); //����BMSϵͳʱ���BMU
    }
}
//**********************************************************************
//* Function name:    TaskBmuProcess
//* Description:      BMU�Լ죬�����ʱ���ղ�ȫBMU���ݣ�����ʾ�ڲ�ͨ�Ź���
//* EntryParameter : None
//* ReturnValue    : None
//**********************************************************************
void TaskBmuProcess(void) 
{
   static unsigned long t=0;
   
   unsigned char i=0,k=0x01,m=0x02;
   unsigned char ErrorState=0;
   float Value=0;
   unsigned long buff=0;
     
    if(!bmuProcess2()) //BMU�Լ�   	        
    {        
        t++;
        _FEED_COP();   //2s�ڲ�ι�ڹ�����ϵͳ��λ
    } 
    else 
    {            
        t=0;
        GetTavgProcess();
        //State_Box_Online=0x3f;        
    }
	
    if(t>=100) //��ʱ���ϱ���300ms*100=30s
    {
        if(t<=105)//5*300=1500ms
            return;
        else //t>105
        {          
            g_caution_Flag_3 |=0x01;    //to pc  
        }            
    }/////end of BMUͨ���ж�
    
    GetDCTem(); 
}

//***********************************************************************
//* Function name:   TaskReport2PC
//* Description:     ��BMS�������Ϣͨ���ڲ�����0x0c0450,0x0c0451,0x0c0452������λ�����
//*                  Ϊ��ֹ��֡�����Ա��ļ������
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskReport2PC(void) 
{
    static unsigned char ct=0;
	
    ct++;
	switch(ct){
		case 1:
			bmsToPcInfo450();
			break;
		case 2:
			bmsToPcInfo451();
			break;
		case 3:
			bmsToPcInfo552();
			break;
		case 4:
			bmsToPcInfo553();
			break;
		case 5:
			bmsToPcInfo554();
			break;
		case 6:
			bmsToSBMSMessage1(); //0x000c0125
			ct = 0;
			break;
		default:
			break;
	}
}
//***********************************************************************
//* Function name:   TaskGpioTest
//* Description:     Ӧ�ó�����뵽���Խ׶�ʱ,������A�ڵļ��
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskGpioTest(void) 
{
    static unsigned int TimerCP=0; 
    if((input2_state()==0)&&(input3_state()==1))   //16A 680R
        C0552_0 |= 0x01; 
    else
        C0552_0 &= 0xfe;
    
    if((input2_state()==0)&&(input3_state()==0))   //32A 220R
        C0552_0 |= 0x02; 
    else
        C0552_0 &= 0xfd;

    if(input6_state()==0)       //CP
    {
        C0552_0|=0x04;
        if(TimerCP>=500)      //10ms ��ʱ�� 2S
        {
            //TurnOn_CP();
            TimerCP=501;
        }
    }
    else 
    {
        C0552_0&=0xfb;
        //TurnOff_CP();
        TimerCP=0;
    }
    
    if(input4_state()==0)      //ACC
        C0552_0|=0x08; 
    else
        C0552_0&=0xf7;
    

    if(input6_state()==0)    //��練��     (�պϳ�練��ʱ,ACCҲ��)
        C0552_0|=0x10; 
    else
        C0552_0&=0xef;

    if(input5_state()==0)       //CC2
        C0552_0|=0x20; 
    else
        C0552_0&=0xdf;
    /*
    if(inputP_state()==0)       // ��������
        gpio_state|=0x20;  
    else
        gpio_state&=0xdf;

    if(inputN_state()==0)       // ��������
        gpio_state|=0x40;
    else
        gpio_state&=0xbf;

    if(inputH_state()==0)       ///INPUT1 �ߵ�ѹ����
        gpio_state|=0x80; 
    else
        gpio_state&=0x7f;  
    */
}
//***********************************************************************
//* Function name:   TaskStatusMachine
//* Description:     BMS����������������״̬�����������Ӧ����
//* EntryParameter : None
//* ReturnValue    : None
//************************************************************************
void TaskStatusMachine(void)//task period = 5ms.
{
    static unsigned char counter1_10ms=0;
    static unsigned char counter1_500ms=0;
    static unsigned char counter2_500ms=0;
    static unsigned char counter3_500ms=0;
    static unsigned char state30=0;
    static unsigned char state46=0;
    static unsigned char Delay14=0;
    static unsigned char TestDelay=0;//���뵽����ģʽ�µ�ʱ����ʱ
    static unsigned char dcMode,pcMode =0;//���״̬��ֵ��־λ
	
	SignalOnOffJudge(); 
	CarFaultDone();
	T_Ctrl_Process();

	
    switch(stateCode)
    {
        //////////////////****************�ϵ����********************///////////////// 
        case 11:
        case 81:
        case 141:
			tmr_p1=0;
			tmr_p2=0;
			tmr_p3=0;
            status_group1.Bit.St_BMS =3;
            state46 = 0;
            break;
        case 12:  //********************��ѹ�Լ�*************//////////////
        case 82:
        case 142:
            Error10S=0;
			
            HighVoltDetectPart1();//�г���MSD�븺��ճ�� �ܵ繭��MSD�ͳ�縺ճ��
			break;
        case 14:  //***********************�պ�����************/////////////
        case 84:
        case 144:
			if(stateCode == 84 || stateCode == 144){
    			//KChg_N_Switch(ON);
    			g_bms_sbms_ctrl_cmd.cmd_Kchg_N_ctrl = 1;
			}
			
			Kn_Switch(ON); // when in 14 state, it used for HV POWER, else turn on Kn to supply for DCDC in charge mode.
            
            Delay14++;
            if(Delay14 >= 4)
            {
				status_group3.Bit.St_N_Relay=1;
                Delay14=0;
            }
            break;
        case 17:   //***********************��ظ�ѹ���*****//////////////
        case 87:
        case 147:
            HighVoltDetectPart2();//������·������ճ��
            break;
        case 20:   //*********************�����̵����պ�*******////////////
        case 90:
        case 150:
			Kp_Switch(ON); // when in 20 state, it used for HV POWER, else turn on Kp to supply for DCDC in charge mode.
			
			if(stateCode == 90) 
            {
				KEleBand_P_Switch(ON);
            }
            else if(stateCode == 150) 
            {
				KFastChg_P_Switch(ON);
            }
			delay(25000); //20ms
			delay(25000); //20ms
            HighVoltDetectPart3();
            break;
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//////////////////****************��������********************///////////////// 
		//////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////      
        case 30:    //*********************�г�״̬***********/////////////
			turnOnSW_Power();
			
			if(state30==0) 
            {
                state30=1;
                pcMode=0; //��ֹ����״̬���ظ���ֵ���״̬����30��0
                dcMode=0; //��ֹ����״̬���ظ���ֵ���״̬����30��0
            }
    
            //YoungMan_LT_step();
            if(plug_AC_CP_Connect == 0)
            {
                BiggestDischargeCurt = BigDischargePowerAdjust();//SOF//30s
                BiggestDisCurtContinuous = BigDischargePowerContinuous();//SOF//5min
                BiggestFeedbackCurt = PulseRechargePowerAdjust1(); //�ƶ���������30s 
                BiggestFeedbackCurtContinuous = ContinueRechargeCurt(); //�ƶ���������5min         
            }
            else if((plug_AC_CP_Connect == 1)||(HighVolPowerOff == 1)||(VCU_Control.Bit.PowerOnOffReq == 2))//��⵽�ܵ繭
            {
                BiggestDischargeCurt =0;//SOF//30s
                BiggestDisCurtContinuous = 0;//SOF//5min
                BiggestFeedbackCurt = 0; //�ƶ���������30s 
                BiggestFeedbackCurtContinuous = 0; //�ƶ���������5min 
            }
            //if((plug_DC_Connect==1)||(OffState==1)||(HighVolPowerOff==1))//״̬���л��������µ���ϻ����й���
            {
                //state_group4.Bit.Request_Power_Off = 1;//BMS��ѹ�µ�����
            } 
			SocEndDischargeAdjust(); //�ŵ�ĩ��SOC����
//			CarFaultDone();//���̹��ϴ���,����Ϊѭ���ϱ�,��ֹ����ʱ����
			break;
			
        case 110:  //*********************������***********//////////////
            turnOnSW_Power();//��������� ��ֹ״̬����֮ǰ�������ϲ��˵�
            BiggestDischargeCurt =0;//SOF//30s
            BiggestDisCurtContinuous = 0;//SOF//5min
            BiggestFeedbackCurt = 0; //�ƶ���������30s 
            BiggestFeedbackCurtContinuous = 0; //�ƶ���������5min         
            if(pcMode==0) 
            {
                status_group3.Bit.St_Charge = 1; //�����
                pcMode=1;
            }
            
//            //state_group2.Bit.St_Charge_Achieve = 1;   //�����        
//            counter1_500ms++;
//            if(counter1_500ms>=70) //7*70=490ms
//            {
//                counter1_500ms=0;
//                Error_Group1.Bit.St_DisCHG_Allow=1;//�ŵ�����״̬λ������
//   
//            }
            
//			CarFaultDone();//���̹��ϴ���,����Ϊѭ���ϱ�,��ֹ����ʱ����
    
            break;
            
        case 170:   //********************����� *************///////////
            turnOnSW_Power();//��������� ��ֹ״̬����֮ǰ�������ϲ��˵�
            BiggestDischargeCurt =0;//SOF//30s
            BiggestDisCurtContinuous = 0;//SOF//5min
            BiggestFeedbackCurt = 0; //�ƶ���������30s 
            BiggestFeedbackCurtContinuous = 0; //�ƶ���������5min 
            
            if( dcMode==0) 
            {
                status_group3.Bit.St_Charge = 1; //�����
                dcMode=1;
            }
 
//            counter1_500ms++;
//            if(counter1_500ms>=70)
//            {
//				counter1_500ms=0;
//				//HeatAndChargeControl();
//				Error_Group1.Bit.St_DisCHG_Allow=1;//�ŵ�����״̬λ������
//            }
            
//			CarFaultDone();//���̹��ϴ���,����Ϊѭ���ϱ�,��ֹ����ʱ����

            break;
 
        //////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////
        //////////////////****************�µ����********************///////////////// 
        //////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////                         
        case 40:   //*****************�Ͽ������̵���***********////////
        case 120:
            status_group1.Bit.St_BMS =2;//��ѹ�Ͽ�
			
            if(stateCode == 40 && Error_Group6.Bit.F3_BMS_Protect != 1) 
            {
                Kp_Switch(OFF); //�Ͽ������̵���
            }
            else if(stateCode == 120 && Error_Group6.Bit.F3_BMS_Protect != 1)
            {
				KEleBand_P_Switch(OFF);
                status_group3.Bit.St_Charge = 2; //������
				Kp_Switch(OFF); // turn off the power supply for DCDC
                Kn_Switch(OFF);
            }
            bmsSelfcheckCounter=2;//û�й��ϣ��Լ������
			
            break;
        case 180:
            g_bms_sbms_ctrl_cmd.cmd_InsRelay_ctrl=0;//�رվ�Ե��⿪��
            TurnOffInsulation();//�رվ�Ե���Ŀ���
            status_group3.Bit.St_Charge = 2; //������
            status_group1.Bit.St_BMS =2;//��ѹ�Ͽ�
			KFastChg_P_Switch(OFF);
			
            bmsSelfcheckCounter=2;//û�й��ϣ��Լ������
            break;
             
        case 44:   //********************�Ͽ�����***************//////////      
        case 124:
        case 184:
            if((stateCode==44)||(stateCode==184)) //�ܵ繭����µ�ʱ���Ͽ���������ֹ�ٴ��ϵ��չ�
            {
				Kp_Switch(OFF);
			}
			
			if(stateCode == 44){
                Kn_Switch(OFF);
			}
			else{
			    KChg_N_Switch(OFF);
			}
            
            KEleBand_P_Switch(OFF);
            TurnOffNRelay=1;//�ϸ����̵�����ɱ�־
            break;  
            
        case 46:   //*****************��ѹ������************//////////////
        case 126:
        case 186:
            if(state46==0)
            {
                KHeat_Switch(OFF);//�رռ��ȼ̵��� 
                preChargeON=0;
                TurnOffNRelay=0;
                StoreSysVariable();
                StoreSocRealvalue();
                state46=1;
                SelfCheck = 0;//�Լ�����,��ֹ���ϵ����Լ�ʱ����������
            }
            
            bmsSelfcheckCounter=1;
			
			if(stateCode == 186 || stateCode == 126){
				Error_Group1.Bit.St_CHG_Allow = 1;	// not allowed.
				status_group3.Bit.St_Charge = 3;	// charge invalid.
			}
			if(stateCode == 46){
				Error_Group1.Bit.St_DisCHG_Allow = 1; // discharge not allowed.
			}
            break;
            
        case 47:  //******************BMS�ϵ�***************////////////////
        case 127:
        case 187:
			Kp_Switch(OFF); // turn off the power supply for DCDC in charging
			Kn_Switch(OFF);
			turnOffSW_Power();//close�ܵ�Դ����  
            bmsSelfcheckCounter=0;
            break;
        default:
            break;                        
    }
    
    stateCodeTransfer(); //�ڴ˵��÷�ֹ״̬û����״̬���Ѿ�����

}


//**************************************************************************************
//**************************************************************************************
//**************************************************************************************
//**************************************************************************************
