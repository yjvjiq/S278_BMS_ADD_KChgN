//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : PHEV.DJ
//* File Name          : Machine.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2011.6.7
//* Description        : ���ļ�������Ŀ��״̬�����Դ���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h" 

//******************************************************************************
//******************************************************************************
//******************************************************************************

unsigned int tmr_p1 =0;
unsigned int tmr_p2 =0;
unsigned int tmr_p3 =0;
unsigned int tmr_p4 =0;
unsigned int tmr_p5 =0;
unsigned int tmr_p6 =0;

unsigned char prechargeFinished =0;      //Ԥ�����ɱ�־λ
unsigned char preChargeON =0;
unsigned char bmsSelfcheckCounter=0;
unsigned char acc_Connect=0;
unsigned char MSDError=0;                 //MSD����
unsigned char N_RelayConnetError=0;       //�����̵���ճ��
unsigned char N_RelayDisConError=0;       //�����̵�����·
unsigned char PreCha_RelayConError=0;     //Ԥ��̵���ճ��
unsigned char P_RelayConError=0;          //�����̵���ճ��
unsigned char PreCha_RelayDisConError=0;  //Ԥ��̵�����·
unsigned char P_RelayDisConError=0;  //�����̵�����·
unsigned char CCHG_RelayDisConError=0;  //�ܵ繭���̵�����·
unsigned char CCHG_RelayConError=0;  //�ܵ繭���̵���ճ�� OK

unsigned char CHG_N_RelayConError=0;  //��縺�̵���ճ��  OK
unsigned char CHG_N_RelayDisConError=0;  //��縺�̵�����·  OK

unsigned char DCCHG_RelayConError=0;  //���̵���ճ��
unsigned char DCCHG_RelayDisConError=0;  //���̵�����·
unsigned char Cha_ResDisConError=0;       //Ԥ������·
unsigned char PreCha_Error=0;             //Ԥ���ʧ��
unsigned char SelfCheck = 0;              //BMS�Լ���: 2,δͨ��;1ͨ��(Ԥ��̵����Ƿ�ɹ��Ͽ�)
unsigned char RelayErrorPowerOff = 0;     //�̵����µ����


//******************************************************************************
//* Function name:   VehiclePowerOffLogic
//* Description:     BMS�������ϣ��µ�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void VehiclePowerOffLogic(void)
{
    static unsigned char OffFlag=0;
    static unsigned char count=0;
    
    TurnOff_INZK();//�Ͽ������Ӵ��� 
    turnOffSW_Power();//�رյ�Դ���س��׶ϵ�
}
//******************************************************************************
//* Function name:   closeNegRelay
//* Description:     �պ������̵���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void closeNegRelay(void) 
{
    TurnOn_INFK();                 //to pc
    delay(25000);                  //19ms
    delay(25000);                  //19ms
}
//******************************************************************************
//* Function name:   openNegRelay
//* Description:     �Ͽ������̵���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void openNegRelay(void) 
{
    TurnOff_INFK();//�Ͽ������̵���
    //state_group1.Bit.St_N_Relay=0;//to vcu
    delay(25000); //19ms
    delay(25000); //19ms
    status_group3.Bit.St_N_Relay=0;  

}
//******************************************************************************
//* Function name:   closePosRelay
//* Description:     �պ������̵���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void closePosRelay(void) 
{
    TurnOn_INZK();//�պ������̵���
    delay(25000); //20ms
    delay(25000); //20ms
    status_group3.Bit.St_P_Relay=1;//to vcu
}
//******************************************************************************
//* Function name:   openPosRelay
//* Description:     �Ͽ������̵���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void openPosRelay(void) 
{
    TurnOff_INZK();//�Ͽ������̵���
    delay(25000); //20ms
    //TurnOff_INBK();//�Ͽ�Ԥ���̵���
    //delay(25000); //20ms
    status_group3.Bit.St_P_Relay=0;//to vcu
}

//******************************************************************************
//* Function name:   GetTavg
//* Description:     ��ŵ繦�ʲο����¶�ȡֵ
//* EntryParameter : ����¶ȣ�����¶�
//* ReturnValue    : Tavg
//******************************************************************************
/*unsigned char GetTavg(unsigned char LowTem,unsigned char HighTem)  
{
    unsigned char Tavg1;

    if((LowTem>10+40)&&(HighTem>=55+40))
        Tavg1=HighTem;
    
    else if((LowTem>10+40)&&(HighTem<55+40)) //��������Ҫ�ſ�
        Tavg1=g_averageTemperature;
    
    else if(LowTem<=10+40) 
        Tavg1=LowTem;
    
    return Tavg1;
} */
//******************************************************************************
//* Function name:   GetTavgProcess
//* Description:     ��ŵ繦�ʲο����¶�ȡֵ
//* EntryParameter : ����¶ȣ�����¶�
//* ReturnValue    : Tavg
//******************************************************************************
void GetTavgProcess(void)  
{
    static unsigned char timer1,timer2; 
    if(g_highestTemperature>45+40)//��������¶ȴ���45�ȳ���2S����ôƽ���¶�Ϊ����¶�
    {
        timer2=0;
        timer1++;
        if(timer1>7)//300ms*7=2100ms
        {
            Tavg=g_highestTemperature;
            timer1=0;
        }
    } 
    else// if(g_highestTemperature<=45+40)//��������¶�С��45�ȳ���2S����ôƽ���¶�Ϊ����¶�
    {     
        timer1=0;
        timer2++;
        if(timer2>7) 
        {
            Tavg=g_lowestTemperature;
            timer2=0;
        } 
    }
}
//******************************************************************************
//* Function name:   HighVoltDetectPart1
//* Description:     ����state�����ϵ粽��1��״̬������12/17or46/126
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void HighVoltDetectPart1(void) 
{
    static unsigned char tt=0;
    static unsigned char pp=0;
    static unsigned char pc=0;
	static U16 s_detect_part1_cnt_t = 0;
	static U8 hv_lock_error_cnt = 0;

	s_detect_part1_cnt_t++; // record how many times run HighVoltDetectPart1 to cover the fault detect.
	
    //**************���MSD******************************//////////////////////// 
    if(g_highVoltageV1 < 100) 
    {
        tt++;
        if(tt >= 12)//60ms�����жϳ���
        {
            //to vcu
            RelayErrorPowerOff = 1;//�̵����µ����
            MSDError = 1;
            g_caution_Flag_4 |= 0x01; //to PC
            tt=13;
        }
    }
    else
    {
        tt=0;
    }

	// hv lock error detect, 1 means error.
	if(HV_Lock_Error_state() == 1){
		hv_lock_error_cnt++;
		if(hv_lock_error_cnt >= 12){
			Error_Group6.Bit.F1_HLVol_Lock = 1;
			g_bms_fault_msg.fault.HLVol_Lock_Alram = 1;
			status_group3.Bit.Fault_Level = 3;
			g_bms_status.status.Fault_Level = 3;
			
			status_group4.Bit.St_BMS_System = 1;
			g_bms_fault_msg.fault.BMS_Sys_Flt_Sts = 1;

			hv_lock_error_cnt = 21;
		}
	}
	else{
		hv_lock_error_cnt = 0;
	}
    
    //********��������̵���ճ�� *****************//////////////////////////////////
    if(g_BmsModeFlag == DISCHARGING)
    {
        if(g_highVoltageV2 > 200)
        {
            pp++;
            if(pp>=12)//60ms
            {
                /////////////////����ճ��///////////////////
                RelayErrorPowerOff = 1;//�̵����µ����
                BmsCtlStat0 |= 0x01;//�����̵���״̬�պ�
                g_caution_Flag_4 |= 0x02; //to PC
                Error_Group3.Bit.F4_N_Con_Err = 1;//error to VCU
                N_RelayConnetError = 1;
                pp = 13;
                /////////////////����ճ��///////////////////
            }
        }
        else
        {
             pp=0;
        }
    }
	
    //********����縺�̵���ճ�� *****************//////////////////////////////////
    if((g_BmsModeFlag == RECHARGING)||(g_BmsModeFlag == FASTRECHARGING))
    {
        if(g_highVoltageV4 >= 200)
        {
            pc++;
            if(pc>=20)//100ms
            {
                /////////////////��縺��ճ��///////////////////
                RelayErrorPowerOff = 1;//�̵����µ����
                Error_Group1.Bit.F2_DCChg_Neg_Relay_Con = 1;//ֱ����縺ճ������λҲ��Ϊ�ܵ繭��
                CHG_N_RelayConError = 1;
				pc=21;
                /////////////////��縺��ճ��///////////////////
            }
        } 
        else
        {
             pc=0;
        }
    }
    
    if((MSDError == 0)
		&& (g_bms_fault_msg.fault.HLVol_Lock_Alram == 0)
		&& (s_detect_part1_cnt_t >= 25))//all BMS mode need detect.
    {
        if(((N_RelayConnetError==0)&&(stateCode == 12))//�г�
          ||((CHG_N_RelayConError==0)&&(stateCode == 82))//�ܵ繭
          ||((CHG_N_RelayConError==0)&&(stateCode == 142))
          )
        {
            bmsSelfcheckCounter=1; //����״̬ 
        } 
    }
}
//******************************************************************************
//* Function name:   HighVoltDetectPart2
//* Description:     ��ѹ��⣺״̬������17/87�����̵�����· ��Ԥ������·����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void HighVoltDetectPart2(void)//Ԥ��̵����Ѿ��պ� 
{    
    static unsigned char NDisConnect_tt=0;
    static unsigned char PConnect_tt=0;
    static unsigned char CCHGConnect_tt=0;
    static unsigned char DCCHGConnect_tt=0;
    static unsigned char CHGDisConnect_tt=0;
	static U16 s_detect_part2_cnt_t = 0;

    if((tmr_p2 <= 100)) //��ʱ20ms���ߴ���500ms,�������� 	//||(tmr_p2 >= 600)
	{
		s_detect_part2_cnt_t = 0;
		return;
    }
	else{
		s_detect_part2_cnt_t++;
	}

    if(g_BmsModeFlag == DISCHARGING)//�г�
    {
		/////������·/////// 
        if(g_highVoltageV2 < 200)
        {
            NDisConnect_tt++;
            if (NDisConnect_tt>=12)//�˲���ʱ60ms����ѹ�Ƿ��ܼ�ʱ�仯��
            {
                RelayErrorPowerOff = 1;//�̵����µ����
                g_caution_Flag_4 |=0x04; //to PC
                BmsCtlStat0 &=0xfe;//�����Ӵ���״̬�Ͽ�
                N_RelayDisConError = 1;  //�����̵�����·
                NDisConnect_tt = 13;
            }
        }
        else   
        {
            NDisConnect_tt=0;  
        }
        
        /////����ճ��//////
        if(g_highVoltageV3 > 200) 
        {
            PConnect_tt++;
            if (PConnect_tt>=12)//�˲���ʱ60ms����ѹ�Ƿ��ܼ�ʱ�仯��
            {
                RelayErrorPowerOff = 1;//�̵����µ����
                g_caution_Flag_4 |= 0x10; //to PC
                BmsCtlStat0 |= 0x02;//�����̵���״̬�պ�
                Error_Group3.Bit.F2_P_Con_Err = 1;//error to VCU
                P_RelayConError = 1;//����ճ������
                PConnect_tt = 13;
            }
        }
        else   
        {
            PConnect_tt=0;  
        }
    }  
    
    if((g_BmsModeFlag == RECHARGING)||(g_BmsModeFlag == FASTRECHARGING))//������ 
    {
		/////��縺�̵�����·///////��綼��
		if(g_highVoltageV4 < 200)
		{
			CHGDisConnect_tt++;
			if (CHGDisConnect_tt>=20)//�˲���ʱ100ms����ѹ�Ƿ��ܼ�ʱ�仯��
			{
				RelayErrorPowerOff = 1;//�̵����µ����
				CHG_N_RelayDisConError = 1;//��縺��·����
				g_caution_Flag_4 |= (1 << 5); // precharge relay off error.
				CHGDisConnect_tt = 23;
            }
        }
        else
        {
            CHGDisConnect_tt=0;
        }
        
        if(g_BmsModeFlag == RECHARGING)
        {
            /////�ܵ�����̵���ճ��///////
            if(g_highVoltageV6 > 200)  
            {
                CCHGConnect_tt++;
                if (CCHGConnect_tt>=20)//�˲���ʱ100ms����ѹ�Ƿ��ܼ�ʱ�仯��
                {
                    RelayErrorPowerOff = 1;//�̵����µ����
                    g_caution_Flag_2 |=0x20; //to PC
                    Error_Group1.Bit.F3_Ele_Relay_Con = 1;//error to VCU
                    status_group2.Bit.St_Ele_Relay = 2;//�ܵ繭�̵�������
                    CCHG_RelayConError = 1;//�ܵ繭���̵���ճ��
                    CCHGConnect_tt = 23;
                }
            }
            else   
            {
                CCHGConnect_tt=0;  
            }
        }
        else if(g_BmsModeFlag == FASTRECHARGING)
        {
			/////���̵���ճ��///////
			if(g_highVoltageV5>200)  
			{
				DCCHGConnect_tt++;
				if (DCCHGConnect_tt>=20)//�˲���ʱ60ms����ѹ�Ƿ��ܼ�ʱ�仯��
				{
					RelayErrorPowerOff = 1;//�̵����µ����
					g_caution_Flag_3 |=0x80; //to PC
					Error_Group3.Bit.F5_DC_Con_Err = 1;//error to VCU
					DCCHG_RelayConError = 1;//���̵���ճ��
					DCCHGConnect_tt = 23;
                }
            }
            else   
            {
                DCCHGConnect_tt=0;  
            }
        }
    }

	if(s_detect_part2_cnt_t >= 25)
    {
        if(((N_RelayDisConError==0)&&(P_RelayConError==0)&&(stateCode == 17))//������·&&����ճ�� 
        ||((CHG_N_RelayDisConError==0)&&(CCHG_RelayConError==0)&&(stateCode == 87))//��縺��·&& �ܵ繭���ճ��
        ||((CHG_N_RelayDisConError==0)&&(DCCHG_RelayConError==0)&&(stateCode == 147)))//��縺��·&& ���ճ��
        {
            bmsSelfcheckCounter=2; //����״̬ 
        }
    }
}
//******************************************************************************
//* Function name:   HighVoltDetectPart3
//* Description:     ����state�����ϵ粽��3��״̬������20/90
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void HighVoltDetectPart3(void) 
{      
    static unsigned char PDisConnect_tt=0;
    static unsigned char CCHGDisConnect_tt=0;
    static unsigned char DCHGDisConnect_tt=0;
    static U16 s_detect_part3_cnt_t = 0;
	
    if(tmr_p3 <= 100)  //��ʱ100ms
	{
		s_detect_part3_cnt_t = 0;
		return;
    }
	else{
		s_detect_part3_cnt_t++;
	}
    
    ///////////////�����̵�����·///////
    if((g_highVoltageV3 < 200)&&(stateCode == 20))
    {
        PDisConnect_tt ++;
        if (PDisConnect_tt>=12)//�˲���ʱ60ms����ѹ�Ƿ��ܼ�ʱ�仯��
        {
            RelayErrorPowerOff = 1;//�̵����µ����
            g_caution_Flag_4 |=0x40; //to PC
            BmsCtlStat0 &=0xfd;//�����̵���״̬�Ͽ�
            P_RelayDisConError = 1;//������·����
            PDisConnect_tt = 13;
        }
    }
    else   
    {
        PDisConnect_tt=0;  
    }
	
    /////////////�ܵ�����̵�����·///////
    if((g_highVoltageV6 < 200) && (stateCode == 90))
    {
        CCHGDisConnect_tt ++;
        if (CCHGDisConnect_tt>=20)//�˲���ʱ100ms����ѹ�Ƿ��ܼ�ʱ�仯��
        {
            RelayErrorPowerOff = 1;//�̵����µ����
            g_caution_Flag_2 |=0x40; //to PC
            CCHG_RelayDisConError = 1;//�ܵ繭�̵�����·����
            status_group2.Bit.St_Ele_Relay = 1;//�ܵ繭�̵���δ����
            CCHGDisConnect_tt = 23;
        }
    }
    else   
    {
        CCHGDisConnect_tt=0;  
    }
	
	/////////////���̵�����·///////
	if((g_highVoltageV5 < 200) && (stateCode == 150))
	{
	    DCHGDisConnect_tt++;
	    if (DCHGDisConnect_tt >= 20)//�˲���ʱ100ms����ѹ�Ƿ��ܼ�ʱ�仯��
	    {
	        RelayErrorPowerOff = 1;//�̵����µ����
	        g_caution_Flag_3 |= 0x40; //to PC
	        DCCHG_RelayDisConError = 1;//���̵�����·����
	        DCHGDisConnect_tt = 20;
	    }
	}
	else   
	{
	    DCHGDisConnect_tt=0;
	}
		
	if(s_detect_part3_cnt_t >= 25) {
        if(((P_RelayDisConError==0)&&(stateCode == 20))
        ||((CCHG_RelayDisConError==0)&&(stateCode == 90))
        ||((DCCHG_RelayDisConError==0)&&(stateCode == 150)))
        {
            bmsSelfcheckCounter = 3; //����״̬
            status_group1.Bit.St_BMS = 0;//��ѹ���� 
        }  
    } 
}  
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
