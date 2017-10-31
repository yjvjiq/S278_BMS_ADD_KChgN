//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : MachineControl.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : ���ļ�����״̬ת���ж�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h" 
unsigned char HighVolPowerOff = 0;//�г�ģʽ�ϸ�ѹ����
unsigned char OffState=0;//���ģʽ�ϸ�ѹ����
unsigned char PantographOff=0;//�ܵ繭ģʽ�ϸ�ѹ����

unsigned char ChangerINError = 2; //����ChanerIN�ź�ͻȻû��:0:��ChanerIN�ź�;1:��ChangerIN�ź�;2����ȷ��
unsigned char State177End=0;//��ͣ��177״̬,
unsigned char PowerOffError=0;//�г�ģʽ�µ����

unsigned int Error10S=0; //�г����ܵ繭�����µ�10S��ʱ
unsigned int Error5S=0;  //����µ�5S��ʱ
unsigned char E10SOverFlag=0; //10S��ʱ��ɱ�־λ
unsigned int Error20S=0; //�г����ܵ繭�����µ�10S��ʱ���20S��ʱ
unsigned int Error_30s_delay_cnt = 0;
unsigned char TurnOffNRelay=0;//�ϸ��̵�����ɱ�־λ

//******************************************************************************
//* Function name:   stateCodeTransfer
//* Description:     ��ϵͳ�����ڼ�ʱ�̼�⵱ǰ�źţ���û�ź��ˣ���Ҫ�µ�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void stateCodeTransfer(void) 
{
    static unsigned int Delay30=0;
    static unsigned int CDelay30=0;
    static unsigned int DCDelay30=0;
    static unsigned int DisChangerDelay=0;
    static unsigned int Delay110=0;
    static unsigned int Delay170=0;
    static unsigned int Delay126=0;
    static unsigned int Delay186=0;
    static unsigned int Delay30_81=0;
	static U16 DelayfPowerOffCnt = 0;
    static U16 charge_disable_cnt_t = 0;
	static U16 fast_charge_end_cnt_t = 0;
	        
//    if(State177End==1)//��177״̬�½�����State177End��1
//    {
//        stateCode=179;//����ģʽ
//        return;
//    }
//    else if(TestState==1)
//    {
//        stateCode=177;//�رռ̵���״̬
//        return;
//    }
    /////////////////�г�//////////////////////
    if(g_BmsModeFlag == DISCHARGING)
    {
		Error_Group1.Bit.St_CHG_Allow = 1;	// charge not allowed.
        if(stateCode == 11)
        {   
            stateCode = 12;
        }
        else if(stateCode == 12)
        {
            if((MSDError)||(N_RelayConnetError)||(HighVolPowerOff)||(acc_Connect == 0)
            ||(VCU_Request.Bit.Finish_PreChg)
            ||(g_bms_fault_msg.fault.HLVol_Lock_Alram == 1)
            ||(VCU_ChgControl.Bit.rise_Eleband_Switch)
//            ||(VCU_ChgControl.Bit.downC_OK == 0)
            ||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 0)
            ){
                stateCode = 46;//MSD��·||����ճ��||��Ҫ�µ�Ĺ���||ON=0||����Ԥ�������
            }
            else if((VCU_Control.Bit.PowerOnOffReq == 1)&&(bmsSelfcheckCounter==1)){
                stateCode = 14;
            }
        }
        else if(stateCode == 14)
        {
            if((acc_Connect == 0)||(PowerOffError == 1)||(VCU_Request.Bit.Finish_PreChg == 1))
                stateCode = 44;//||����Ԥ�������
            else if(status_group3.Bit.St_N_Relay == 1)
                stateCode = 17;
        }
        else if(stateCode == 17)
        {
          
           if((N_RelayDisConError)||(P_RelayConError)||(acc_Connect==0)
           ||(HighVolPowerOff==1)||(VCU_Request.Bit.Finish_PreChg))
                stateCode = 44; //�����̵�����·||����ճ��||��Ҫ�µ�Ĺ���||ON=0||����Ԥ�������
           else if(bmsSelfcheckCounter==2)
                stateCode = 20;
        }
        else if(stateCode == 20)
        {
            if((P_RelayDisConError==1)||(HighVolPowerOff == 1)||(acc_Connect == 0)||(VCU_Control.Bit.PowerOnOffReq == 2))
            {
				if(VCU_Control.Bit.PowerOnOffReq == 2){ // power off command exist 2s, then turn off the contactor.
					DelayfPowerOffCnt++;
					if(DelayfPowerOffCnt >= 50){ // delay 2s
						stateCode =40;
						DelayfPowerOffCnt = 0;
					}
				}
				else{
					stateCode = 40;//������·||��Ҫ�µ�Ĺ���||ON=0
					DelayfPowerOffCnt = 0;
				}
            }
            else if((bmsSelfcheckCounter==3)&&(status_group3.Bit.St_P_Relay)&&(VCU_Request.Bit.Finish_PreChg))
                stateCode = 30;//�Լ������=3&&�����̵����պ�&&����Ԥ������
        } 
        else if(stateCode == 30)
        {
            if(((VCU_Control.Bit.PowerOnOffReq != 2)&&(E10SOverFlag == 1)&&(Error_30s_delay_cnt >= 30000))
				||((VCU_Control.Bit.PowerOnOffReq == 2)&&(E10SOverFlag == 1))//fault occured, and has delay 10s already, then received the power off command.
				||(Delay30 >= 400)
            )
            {
                 stateCode = 40;
                 Delay30 = 0;
            }
            else if((acc_Connect==0)||(VCU_Control.Bit.PowerOnOffReq == 2))//ACC disappear, 5ms*400=2S
            {
                Delay30++;
            }
            else 
            {
				//do nothing here, keep previous status.
            } 
              
        } 
        else if(stateCode == 40)//�Ͽ������̵���
        {
            if(bmsSelfcheckCounter == 2)
                stateCode = 44; //�Լ������==2&&�����̵����ѶϿ�
        } 
        else if(stateCode == 44)
        { 
            if(TurnOffNRelay)
                stateCode = 46;
        }
        else if((stateCode == 46))
        {
            /////////////////////46 start/////////////////////////
            if((HighVolPowerOff) || (acc_Connect==0) || (RelayErrorPowerOff))
            {
                if(bmsSelfcheckCounter==1)//�޹���
                    stateCode = 47;
            }
            else if(((VCU_ChgControl.Bit.rise_Eleband_Switch)
//				||(VCU_ChgControl.Bit.downC_OK == 0)
				||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 0))
				&&(bmsSelfcheckCounter==1))
            {
                //��������&&������λ&&�Լ������==1
                InitialSoc();
				g_BmsModeFlag = RECHARGING;//�����ܵ繭���ģʽ
				status_group4.Bit.Mode_BMS_Work = 2;//BMS��ǰ����״̬=���
				status_group3.Bit.St_CHG_Mode=2;//���ģʽ����AC���
				status_group3.Bit.St_Charge = 1;//���ڳ�� 
				Error_Group1.Bit.St_CHG_Allow = 0; //charge allowed
                stateCode = 81;
                bmsSelfcheckCounter = 0;            
            }
        }
    }
    ///////////////�ܵ繭��� //////////////////
    else if(g_BmsModeFlag == RECHARGING)
    {
		Error_Group1.Bit.St_DisCHG_Allow = 1;	// discharge not allowed.
        if(stateCode == 81)
        {   
            if((VCU_ChgControl.Bit.rise_Eleband_Switch)
//				&&(VCU_ChgControl.Bit.downC_OK == 0)
				&&(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 0)
				&&(VCU_ParkBrake.Bit.Parking_Brake)){
                stateCode = 82;
				}
        }
        else if(stateCode == 82)
        {
			if(g_bms_msg.CellVoltageMax >= (U16)(HIGHEST_ALLOWED_CHARGE_CV * 10000)
				|| g_bms_msg.CellTempMax >= (U16)(HIGHEST_ALLOWED_CHARGE_T + 40)
				|| g_bms_msg.CellTempMin <= (LOWEST_ALLOWED_WORK_T + 40)
				||(g_bms_fault_msg.fault.HLVol_Lock_Alram == 1)){
				stateCode = 126;
			}
            else if((MSDError==1)||(CCHG_RelayConError == 1)||(PantographOff)
            ||(VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
            ||(VCU_ChgControl.Bit.downC_OK == 1)
            ||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
            ||(VCU_ParkBrake.Bit.Parking_Brake==0))
                stateCode=126; //MSD��·||��縺ճ��||��Ҫ�µ����||||��������==0||������λ==0||פ���ź�==0
            else if((bmsSelfcheckCounter==1) && (DC_Start == 1))
                stateCode=84; //�Լ������==1&&������λ&&��������==1&&פ���ź�==1
        }
        else if(stateCode == 84)
        {
            if((VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
				||(VCU_ChgControl.Bit.downC_OK == 1)
				||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
				||(VCU_ParkBrake.Bit.Parking_Brake==0)||(PantographOff)){
                stateCode = 124;//��������==0||������λ==0||פ���ź�==0||��Ҫ�µ����
				}
            else{// if((BmsCtlStat0&0x08)!=0)//������λ&&��������
                stateCode = 87;//��縺�ѱպ�
            }
        }
        else if(stateCode == 87)
        {
            if((CHG_N_RelayDisConError)||(CCHG_RelayConError)||(PantographOff)
             ||(VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
             ||(VCU_ChgControl.Bit.downC_OK == 1)
             ||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
             ||(VCU_ParkBrake.Bit.Parking_Brake==0)){
                stateCode = 124;//��ѹ���2�й���||���µ�Ĺ���||��������==0||������λ==0������WIFI)||פ���ź�
			}
            else if(bmsSelfcheckCounter==2) {
                stateCode = 90;//�Լ������==2&&��������==1&&������λ==1������WIFI)
            }
        }
        else if(stateCode == 90)
        {
            if((VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
				||(VCU_ChgControl.Bit.downC_OK == 1)
				||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
				||(VCU_ParkBrake.Bit.Parking_Brake==0)
            	||(PantographOff)||(CCHG_RelayDisConError)){
                stateCode = 120; //��ѹ���3�й���||���µ�Ĺ���||��������==0||������λ==0������WIFI)||פ���ź� 
            }
            else if(bmsSelfcheckCounter==3){
                stateCode = 110;//�Լ������==3&&��������==1&&������λ==1&&��翪��==1
            }
        }
        else if(stateCode == 110)
        {
            if((slowRechargeFinished == 1)
			||((PantographOff == 1)&&(E10SOverFlag == 1)&&(g_systemCurrent > -5))//��ʱ10S�µ�
			||((PantographOff == 1)&&(E10SOverFlag == 1)&&(Error20S>=20000)&&(g_systemCurrent <= -5))//������ʱ10S&&��������5A���ٳ���20S
            ){
                stateCode = 120;//�������ɻ������µ�Ĺ��ϻ��߼�⽵������==0||������λ==0||��翪��==0    
            }
            else if((VCU_ChgControl.Bit.rise_Eleband_Switch == 0)
            ||(VCU_ChgControl.Bit.downC_OK == 1)
            ||(VCU_ChgControl_2.Bit.rise_Eleband_No_OK == 1)
            ||(VCU_ParkBrake.Bit.Parking_Brake==0) 
			||((fastendflag == 1) && (PantographOff != 1)) // charge finished and no fault.
			)
            {
                CDelay30++;
				if(CDelay30 >= 400){	//����ź���     5ms * 400 = 2.0S
					CDelay30 = 0;
					stateCode = 120;
				}
            }
        } 
        else if(stateCode == 120)//�Ͽ��ܵ繭�̵���
        {
            if(bmsSelfcheckCounter == 2)//�Լ������=2
                stateCode = 124;
        } 
        else if(stateCode == 124)
        { 
            if(TurnOffNRelay)//ȷ����縺�̵����ѶϿ�
                stateCode = 126;
        } 
        else if((stateCode == 126))
        {
            if(((bmsSelfcheckCounter==1)&&(g_highVoltageV3<200))||(acc_Connect==0)||(OffState)||(PantographOff))//�޹���
			{
                stateCode = 127;
			}
		} 
    }
    ///////////////////////////���/////////////////////////////////
    else if(g_BmsModeFlag == FASTRECHARGING)
    {
		Error_Group1.Bit.St_DisCHG_Allow = 1;	// discharge not allowed.
        if(stateCode == 141)
        {
            g_bms_sbms_ctrl_cmd.cmd_InsRelay_ctrl = 1;//���ʱ�ɾ�Ե���Ƹ�SBMS
            if((status_group3.Bit.St_N_Relay == 0)&&(status_group3.Bit.St_P_Relay == 0)
            &&(CC2VOL<=CC2VOLHIGH)&&(CC2VOL>=CC2VOLLOW)) //CC2��ѹ�ڷ�Χ��
                stateCode = 142;
        }
        else if(stateCode == 142)
        {
			if(g_bms_msg.CellVoltageMax >= (U16)(HIGHEST_ALLOWED_CHARGE_CV * 10000)
				|| g_bms_msg.CellTempMax >= (U16)(HIGHEST_ALLOWED_CHARGE_T + 40)
				|| g_bms_msg.CellTempMin <= (LOWEST_ALLOWED_WORK_T + 40)
				||(g_bms_fault_msg.fault.HLVol_Lock_Alram == 1)
				||(DCTem1 >= Chg_Socet_Max_Temperatue_lv1)
				||(DCTem2 >= Chg_Socet_Max_Temperatue_lv1)
				){
				stateCode = 186;
				
				if((DCTem1 >= Chg_Socet_Max_Temperatue_lv2)
				||(DCTem2 >= Chg_Socet_Max_Temperatue_lv2)){
					fastendflag = 1;
					fastend2 |= 0x40;//�������¶ȹ���
					OffState = 1;//�����µ�
					Error_Group0.Bit.F2_Ele_Relay_Con = 3;//����CAN��ֵ     
				}
				else if((DCTem1 >= Chg_Socet_Max_Temperatue_lv1)
				||(DCTem2 >= Chg_Socet_Max_Temperatue_lv1)){
					fastendflag = 1;
					fastend2 |= 0x40;//�������¶ȹ���
					OffState = 1;//�����µ�
					Error_Group0.Bit.F2_Ele_Relay_Con = 2;//����CAN��ֵ     
				}
				
			}
            else if((MSDError)||(DCCHG_RelayConError == 1)||(plug_DC_Connect == 0)||(OffState))
                stateCode = 186;//��ѹ���1�й���||���µ�Ĺ���||CC2==0 ||ChargeIN==0
            else if((plug_DC_Connect == 1)&&(bmsSelfcheckCounter==1) && (DC_Start == 1))
                stateCode = 144;
        }
        else if(stateCode == 144)
        {
            if((plug_DC_Connect == 0)||(OffState == 1))
                stateCode = 184; //���µ�Ĺ���||CC2==0 ||ChargeIN==0
            else if(plug_DC_Connect == 1)//&&((BmsCtlStat0&0x08)!=0)
                stateCode = 147;//�����̵����Ѿ��պ�&&CC2==1&&ChargeIN==1
        }
        else if(stateCode == 147)
        {
           if((CHG_N_RelayDisConError==1)||(DCCHG_RelayConError==1)||(plug_DC_Connect == 0)||(OffState))
                stateCode = 184;//��ѹ���2�й���||���µ�Ĺ���||CC2==0 ||ChargeIN==0
           else if((plug_DC_Connect == 1)&&(bmsSelfcheckCounter==2))
                stateCode = 150;//�Լ������==2&&CC2==1&&ChargeIN==1
        }
        else if (stateCode == 150)
        {                                      
            if((plug_DC_Connect == 0)||(DCCHG_RelayDisConError==1)||(OffState))
                stateCode = 180;//��ѹ���3�й���||���µ�Ĺ���||CC2==0 ||ChargeIN==0
            else if((bmsSelfcheckCounter==3)&&(plug_DC_Connect == 1)&&(status_group2.Bit.St_CHG_Connect == 2))
                stateCode = 170;
        }
        else if(stateCode == 170)
        {
            if(((OffState)&&(Error5S >= 5000))
				||(DCDelay30 >= 400))
            { 
                 stateCode = 180;
                 DCDelay30 = 0; 
            }  
            else if(plug_DC_Connect == 0)//CC2��    5ms*300=2.5S
            {                
                DCDelay30++; 
            }
			else if(fastendflag == 1){
				fast_charge_end_cnt_t++;
				if(fast_charge_end_cnt_t >= 1000){
					fast_charge_end_cnt_t = 0;
					stateCode = 180;
				}
			}
        } 
        else if(stateCode == 180)//�Ͽ����̵���
        {
            if(bmsSelfcheckCounter == 2)
                stateCode = 184;
        } 
        else if(stateCode == 184)
        { 
            if(TurnOffNRelay)//ȷ�����Ͽ�
                stateCode = 186;
        } 
        else if((stateCode == 186))
        {
			if(bmsSelfcheckCounter == 1){
                stateCode = 187;
			}
        } 
    } 
}
//*************************************************************************
//**************************the end *********************************************
//************************************************************************************
//******************************************************************************************
