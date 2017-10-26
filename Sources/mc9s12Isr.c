//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : mc9s12Isr.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���ļ�����ϵͳ���е��ж��ӳ��򣬰���RTI�жϡ�ADC�жϡ�CAN�����жϣ�3·��
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"
//*******************************************
//*******************************************
float ac[12];
float ax[12];
//
struct can_msg *msg;
unsigned long g_mboxID;
unsigned char g_mboxData[1][8];// CAN2���յ��ı������ݣ�ÿ������8���ֽ�

unsigned char CHMStep=0; //��䲽��

unsigned char Int_Flag =0;// 0x01:AD�ж� 0x02:can0�ж� 

unsigned char g_errorRecordRead_flag;//������Ϣ��ȡ��־
unsigned char stateCode=0; //״̬������

unsigned char TestState=0;//״̬�����뵽����״̬

long Priority = 6;

unsigned char DC_Start=0;//���յ�0xaa��,��ʼ�պϸ����̵���

unsigned char BootState = 0;//boot����

unsigned char TurnOnChangerRelay = 1;//�պϳ��̵���

unsigned int DC_ChargeTime=0; //�ۼƳ��ʱ��

unsigned char FlagBRMSend = 0; //BMS��֡���ı�־λ,0������,1����
unsigned char BRMStep = 0; //BRM�ѷ��ͱ�־λ,0δ���ͻ�����;1�ѷ���

unsigned char FlagBCPSend = 0;//����BCP��,�յ��������ص���Ϣ,��ʼ�������ݶ�
unsigned char BCPStep = 0;    //BCP�еĲ���0:����BCP��ͷ;1:����BCP�����ݶ�

unsigned char FlagBCSSend = 0;//����BCS��,�յ��������ص���Ϣ,��ʼ�������ݶ�
unsigned char BCSStep = 0;    //BCS�еĲ���0:����BCS��ͷ;1:����BCP�����ݶ�

float g_SBMS_current1 = 0;	//the channal 1 HV current received from SBMS.
float g_SBMS_current2 = 0;	//the channal 2 HV current received from SBMS.

unsigned char InsRelayState = 0;    //SBMS���Ե���Ƽ̵���״̬
 	
static unsigned char CEMflag=0;

//******************************************************************************
//* Function name:   SendMes
//* Description:     ��������CAN����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void SendMes(void)
{
    
    static unsigned char PIT_1000ms_Counter0=7;       //1000ms��ʱ������ 
    static unsigned char PIT_70ms_Counter0=0;       //70ms��ʱ������
    static unsigned char PIT_100ms_Counter0=1;       //100ms��ʱ������0
    static unsigned char PIT_100ms_Counter1=3;       //100ms��ʱ������1
    static unsigned char PIT_100ms_Counter2=2;       //100ms��ʱ������2
    static unsigned char PIT_10ms_Counter3=8;         //100ms��ʱ������3 ���Ͳ��Ա���
    static unsigned char PIT_10ms_Counter4=11;         //100ms��ʱ������4�����ܵ繭����
    static unsigned int PIT_300ms_Counter1=0;       //1000ms��ʱ������
    static unsigned int PIT_500ms_Counter=5;       //500ms��ʱ������
    static unsigned char CellNum=0,TemNum=0; 
    static unsigned int ChargeTimeSum=6;//�ۼƳ��ʱ��++
    //static unsigned char CellOver=0;//��ѹ���ݷ������־λ
    //static unsigned char TempOver=1;//�¶����ݷ������־λ 
         
    PIT_1000ms_Counter0++;//1000ms
    PIT_100ms_Counter0++;//100ms 
    PIT_100ms_Counter1++;//100ms 
    PIT_100ms_Counter2++;//100ms 
    PIT_10ms_Counter3++;
    
    if(g_BmsModeFlag == FASTRECHARGING)//���ģʽ 
        PIT_500ms_Counter++; //500ms
    
    if(g_BmsModeFlag == FASTRECHARGING )//���ģʽ 
    {
        ChargeTimeSum++;
        if(ChargeTimeSum>=6006)//1min=60000ms  �����ۼƳ��ʱ��
        {
            DC_ChargeTime++;
            ChargeTimeSum=6;
        }
    }
   
    ///////////////////////1000ms��500ms///////////////////
    if(PIT_500ms_Counter == 55) 
    {
        if(g_BmsModeFlag == FASTRECHARGING )//���ģʽ 
        {
            BMS_To_VCU_CHGMsg1();           //500ms 0x1828D0D2;
            PIT_500ms_Counter=5;
        }
    }
    else if(PIT_1000ms_Counter0 == 106)      //1000ms
    {
        BMS_To_VCU_BasicMsg0();//0x1825D0D2
    }
    else if(PIT_1000ms_Counter0 == 107)      //1000ms
    {
        BMS_To_VCU_BasicMsg1();//0x1826D0D2
    }
    else if(PIT_1000ms_Counter0 == 108)      //1000ms
    {
        BMS_To_VCU_BasicMsg2();//0x1827D0D2
    }
	else if(PIT_1000ms_Counter0 == 109){
		BMS_TMS_msg();
        PIT_1000ms_Counter0=7;
	}
    ///////////////////////100ms/////////////////////////// 
    if(PIT_10ms_Counter3 == 28)
    {  
        if(DubugMode==1) 
        {
            bmsToPcTestCar(); /////////////////////
            bmsToPcTestCar1(); //////////////////////////
            PIT_10ms_Counter3=8;
        }
    }
    if((g_BmsModeFlag == RECHARGING)&&(PROJECT_NAME == 6843))
    {
        PIT_10ms_Counter4++;
        if(PIT_10ms_Counter4>=21) 
        {
            BMS_VCU_WIFI();       //ת������WiFi����Ϣ�ӿ��CAN������CAN
            PIT_10ms_Counter4=11;//0x0c0217A7
        }
    }
    if(PIT_100ms_Counter0==10)
        BMS_To_VCU_BatteryMsg1();//0x1422D0D2
    
    else if(PIT_100ms_Counter0==11)
    {
        BMS_To_VCU_BatteryMsg2();//0x1423D0D2
        PIT_100ms_Counter0=1;
    }
    
    if(PIT_100ms_Counter2==11)
    {
        BMS_To_VCU_BatteryMsg3(); //0x1424D0D2
    }
    else if(PIT_100ms_Counter2==12) 
    {
        BMS_To_VCU_BatteryMsg8(); //0x0C1ED0D2
    }
    else if(PIT_100ms_Counter2==13)
    {
        BMS_To_VCU_BatteryMsg6();//0x1881D0D2
        PIT_100ms_Counter2=2;
    }
    
    if(PIT_100ms_Counter1==12) 
    {
        //if(1 == VCU_CellRequest.Bit.PackMessageReq)
            BMS_To_VCU_BatteryMsg4(); //0x1813D0D2; 
    }
    else if(PIT_100ms_Counter1==13) 
    {
        //if(1 == VCU_CellRequest.Bit.PackMessageReq )
            BMS_To_VCU_BatteryMsg5();//0x1814D0D2; 
    }
    else if(PIT_100ms_Counter1==14)
    {
        BMS_To_VCU_BatteryMsg7();//0x0C1FD0D2; 
        PIT_100ms_Counter1 = 3;
    }
  
    
    ///////////////////////���õ�֡����ms///////////////////////////
    //if(1 == VCU_CellRequest.Bit.CellVolReq )     //0x1c15D0D2
    
        PIT_70ms_Counter0++; //200ms
        if(PIT_70ms_Counter0 > 20)  //200ms��һ֡
        {
            BMS_To_VCU_BatCellVolData();
//            CellNum++;
//            if(CellNum>=CELL_VOL_GROUP) 
//            {
//                CellNum=0;
//            }
            
            PIT_70ms_Counter0 = 0;
        } 
    //if(1 == VCU_CellRequest.Bit.CellTempReq )  //0x1c64D0D2
    
        PIT_300ms_Counter1++;//ms
        if(PIT_300ms_Counter1>=20)//200ms��һ��
        {
            BMS_To_VCU_BatCellTempData();
//            TemNum++;
//            if(TemNum>=CELL_TEMP_GROUP) //-1����Ϊ��0��ʼ
//            {
//                TemNum=0;
//            }

            PIT_300ms_Counter1 = 0;
        } 
}
//******************************************************************************
//* Function name:   PIT0_ISR
//* Description:     10ms��ʱ�ж��ӳ���,�ڴ���Ҫ���͸�������0x650����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
#pragma CODE_SEG NON_BANKED
interrupt void PIT0_ISR(void)   //10ms�ж�һ��
{   
    while (PITTF&0x01){ 
        PITTF |= 0x01;	//set 1 to clean timeout flag.
    }
	
    SendMes();
    
    if(BMUOK==1){
        TaskDC();//call the DC charge process.
    }
}
#pragma CODE_SEG DEFAULT
//******************************************************************************
//* Function name:   RTI_ISR
//* Description:     1ms��ʱ�ж��ӳ���,��������һЩʱ���־λ�Լ�����ʹ�õ������л���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
#pragma CODE_SEG NON_BANKED
interrupt void RTI_ISR(void)      //1ms�ж�һ��
{ 

    //turnOnLED0(); 	     // for debug 
    TaskRemarks();  //******
    
    if(tmr_p1<=600){     //����״̬����ʱ        
        if((stateCode==12)||(stateCode==82)||(stateCode==142)){
            tmr_p1++;
        }
//		else{
//			tmr_p1 = 0;
//		}
    }
	
    if(tmr_p2<=600){
        if((stateCode==17)||(stateCode==87)||(stateCode==147)){
            tmr_p2++;
        }
//		else{
//			tmr_p2 = 0;
//		}
    }
	
    if(tmr_p3 <= 600){
        if((stateCode ==20)||(stateCode==90)||(stateCode==150)){
            tmr_p3++;
        }
//		else{
//			tmr_p3 = 0;
//		}
    }
	
    if(tmr_p4<600){
        if((stateCode ==13)||(stateCode==83)||(stateCode==143)){
            tmr_p4++;
        }
		else{
			tmr_p4 = 0;
		}
    }
	
//	if(tmr_p5<600)
//		if((stateCode ==46)||(stateCode==126)||(stateCode==186))  
//			tmr_p5++;
//	if(tmr_p6<600)        
//		if((stateCode ==40)||(stateCode==120)||(stateCode==180))
//			tmr_p6++;
    
    if(Error10S < 15000){
        if((HighVolPowerOff==1)&&(stateCode ==30)//�г��¸�ѹ����
        ||((PantographOff==1)&&(stateCode ==110)))//�ܵ繭����¸�ѹ����
        {
            Error10S++;
            if(Error10S>=10000){
                E10SOverFlag=1;//10S��ʱ��ı�־λ    
            }
        }
        else 
        {
            Error10S=0;
            E10SOverFlag=0;
        }
    }
	
	if((HighVolPowerOff==1) && (E10SOverFlag == 1)){
		Error_30s_delay_cnt++;
		if(Error_30s_delay_cnt >= 30000){
			Error_30s_delay_cnt = 30000;
		}
	}
	
    if((E10SOverFlag == 1)&&(Error20S<21000))//10s��ʱ���20S����ʱ
    {
        Error20S++;            
    }
	
	if(Error5S<10000){
		if((OffState==1)&&(stateCode ==170)){	//����¸�ѹ����
			Error5S++;
		}
	}
	
    if((g_systemCurrent > -500)&&(g_systemCurrent < 500))  //��ֹ�������
    {
		g_energyOfUsed = g_energyOfUsed+ g_systemCurrent*0.0011+0.0000042;// // 1m�����һ��
    }
	
	//turnOffLED0(); 	     // for debug 
    // clear RTIF bit 
    CRGFLG = 0x80; 
}
#pragma CODE_SEG DEFAULT

//******************************************************************************
//* Function name:   ADC0_ISR
//* Description:     ADC�ж��ӳ���,��ȡ2��ADͨ���ĵ���  DHAB/S24
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
#pragma CODE_SEG NON_BANKED
interrupt void ADC0_ISR(void) 
{	
	  static unsigned char ADcount=0;   
	  //ac[ADcount]=(((float)ATD0DR1-ADCLC)*5.0/4096-2.5)*250;      //CUR_AD���ڴ����ͨ��  CUR_AD1  //S24
      //ax[ADcount]=(((float)ATD0DR0-ADCLX)*5.0/4096-2.5)*37.45;    //X_AD  //����Сͨ�� CUR_AD2 ����
	  ac[ADcount]=(((float)ATD0DR1-ADCLC) * 5.0 / 4096 - 2.5) * 370;    //CUR_AD���ڴ����ͨ��  CUR_AD1, S133, G = 2.7mV/A
	  ax[ADcount]=(((float)ATD0DR0-ADCLX) * 5.0 / 4096 - 2.5) * 37.45;  //X_AD,����Сͨ�� CUR_AD2 ����, S133, G = 26.7mV/A
	  ATD0STAT0_SCF=1;	//���������ɱ�־	                                                                                  
	  ADcount++;	
	 
	  if(ADcount>=12)
	  {
		    ADcount =0;
	 	    Int_Flag |= 0x01;
		    turnOffADC();
	  }	
}
#pragma CODE_SEG DEFAULT

//******************************************************************************
//* Function name:   CAN0_RECEIVE_ISR
//* Description:     CAN0�����ж��ӳ���,�ڸ���Ŀ�����ڽ��ս������������ı���0x6d0
//*                  //���� /�ⲿCAN / 500bd  /��׼֡��ʽ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
#pragma CODE_SEG NON_BANKED
interrupt void CAN0_RECEIVE_ISR(void)   //���� /�ⲿCAN / 500Hz
{
   unsigned char i,msgData[8];
   unsigned long msgCan0RxID;
   unsigned long pp=0;
   unsigned long pp1=0;
   unsigned int buffer = 0;
   unsigned int VCUSpeed = 0;
   
   unsigned int VolLow=0; ///////////�ܵ繭���
   unsigned int VolHigh=0;

   DisableInterrupts;   
   if(!(CAN0RFLG&0x01)) // Checks for received messages
        return ;
   //msgCan0RxID = (unsigned int)(CAN0RXIDR0<<3) |       //standard
   //         (unsigned char)(CAN0RXIDR1>>5); 
   
   pp = (unsigned long)CAN0RXIDR0<<15;  
   pp1 = (unsigned long)((CAN0RXIDR1>>5)&0x07)<<15;                              

   msgCan0RxID = ((unsigned long)pp<<6) | //expanded
              (pp1<<3) |
              ((unsigned long)(CAN0RXIDR1&0x07)<<15)|
              ((unsigned long)CAN0RXIDR2<<7) | 
              ((unsigned long)CAN0RXIDR3>>1);
   
    for(i=0;i<8;i++){ 
        msgData[i] = *((&CAN0RXDSR0)+i);
    }
	
    switch(msgCan0RxID) 
    {
        case 0x1801D2D0://
			VCUOverTime = 0;//����
			VCU_CellRequest.byte = msgData[0];
			break;
            
        case 0x1802D2D0:
			VCUOverTime = 0;//����      
			VCU_Control.byte = msgData[1];
			break;
			
        case 0x0CFFEBEF:  
            VCUOverTime = 0;//����
            VCU_Request.byte = msgData[2];
            break;
            
        case 0x0C0117A7://�ܵ繭����WiFi      /////////����ܵ繭���������CAN�ڴ˽��ձ���
            ACCOverTime = 0;//����
			VCU_ChgControl.byte = msgData[3];
            break;
			
		case 0x18010027: // used in Eleband Charge, CarWiFi.
			ACCOverTime = 0;//����
			VCU_ChgControl_2.byte = msgData[2];
			break;
			
		case 0x18FEF117:
            buffer = msgData[2];
            buffer = buffer<<8;
            VCUSpeed = buffer+msgData[1];
            VehicleSpeed = VCUSpeed*0.00390625;
            break;
			
        case 0x18FFDD17:		//0x18FEF117:
			VCU_ParkBrakeOverTime = 0;
            VCU_ParkBrake.byte = msgData[0];
			break;
			
		    ///////////// ����CAN�����ܵ繭���� ///////////////
		case 0x1826f456:
        //if((fChg2bmsbyte[0]==0x01)&&(fChg2bmsbyte[1]==0x01)&&(fChg2bmsbyte[2]==0))
        //{
            DCStartState=2;//�¹��꿪ʼ�ı�־ 
            DC_Vesion = 2; //�¹���
            if(CHMStep<=0x01)
                CHMStep=0x01;
        //}
            break;
			
        case 0x1801f456:
            CRMOverTimeBefore60s = 0;
            if( msgData[0]==0x00)
            {              
                DCStartState=1;//�Ϲ��꿪ʼ�ı�־                 
                CRMOverTimeBefore = 0;
                if(CHMStep<=0x01)                
                    CHMStep=0x01;
            }
            else if( msgData[0]==0xaa)
            {
                CRMOverTime=0;
                DC_Start = 1;
                if(CHMStep<=0x02)
                    CHMStep=0x02;
                //TurnOn_INA2K();
                //SelfState2=1;
            }            
            break;
            
        case 0x1cecf456:
			if((msgData[0]==0x11)&&(msgData[5]==0x00)&&(msgData[6]==0x02)&&((msgData[7]==0x00))){
				FlagBRMSend = 1; //��־λ��1,��ʼ���Ͷ�֡��֤,���Ҳ�����BRM
			}
			
            if((msgData[0]==0x11)&&(msgData[5]==0x00)&&(msgData[6]==0x06)&&((msgData[7]==0x00))){
                 FlagBCPSend = 1;
            }
			
            if((msgData[0]==0x11)&&(msgData[5]==0x00)&&(msgData[6]==0x11)&&((msgData[7]==0x00))){
                 FlagBCSSend = 1;
            }
			
//			if((fChg2bmsbyte[0]==0x13)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x02)&&((fChg2bmsbyte[7]==0x00)))
//				BRMStep = 0;//����־λ����,���Դ��·���BRM
//			if((fChg2bmsbyte[0]==0x13)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x06)&&((fChg2bmsbyte[7]==0x00)))
//				BCPStep = 0;//����־λ����,���Դ��·���BRM
//			if((fChg2bmsbyte[0]==0x13)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x11)&&((fChg2bmsbyte[7]==0x00)))
//				BCSStep = 0;//����־λ����,���Դ��·���BRM
            break;
                
        case 0x1807f456:    
            //CHMStep=0x03;
            break;  
        case 0x1808f456:
            VolHigh = msgData[1];
            VolHigh = VolHigh<<8;
            VolHigh = (VolHigh+msgData[0]);

            VolLow = msgData[3];
            VolLow = VolLow<<8;
            VolLow = (VolLow+msgData[2]);
            if((VolLow>HIGHEST_VOL)||(VolHigh<LOWEST_VOL))
            {
                BROErrorAA=1;//������������            
            } 
            else
            {
                BROErrorAA=0;//����������
            }
            if(CHMStep<=0x03)   
                CHMStep=0x03;
            break;  
        case 0x100af456:
            CROOverTime = 0;//�յ�CRO������
            if( msgData[0]==0xaa)
            {
                if(CHMStep<=0x04)
                    CHMStep=0x04;
                SelfState3=1;
                CROOverTime60s = 0;//�յ�0xaa������           
            } 
            break; 
        case 0x1812f456:
            CCSOverTime=0;
                        
            //DC_ChargeTime = msgData[5];
            //DC_ChargeTime = VolHigh<<8;
            //DC_ChargeTime = (VolHigh+msgData[4]);  
            if(CHMStep<=0x05)
                CHMStep=0x05;
            break;
        case 0x101af456:
            m_askcurrent=0;//�������Ϊ0 
            if(((msgData[0])&(0x40)) == 0)
            {
                fastend1 = 0x40;
            }
            CSTOverTime=0;
            if(CHMStep<=0x06)    
                CHMStep=0x06;
            break; 
			
        case 0x181df456:
            CSDOverTime=0;
			
            if(CHMStep<=0x07)     
                CHMStep=0x07;
            break;
        case 0x081ff456:
            BEMStop = 1;
            if(CHMStep<=0x07)
                CHMStep=0x07;
            break;
            ///////////// ����CAN�����ܵ繭���� ///////////////
		case 0X0CFF13F4:
		{
			for(i=0;i<8;i++){ 
				g_TMS_BMS_msg.data[i] = msgData[i];
			}
		}
		break;
		default:
            break;
    }
    CAN0RFLG = 0x01;   
    EnableInterrupts; 
}
#pragma CODE_SEG DEFAULT
//******************************************************************************
//* Function name:   CAN1_RECEIVE_ISR
//* Description:     CAN1�����ж��ӳ���,���ڿ��ͨ��  250bd /��չ֡ģʽ                , charge CAN
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
#pragma CODE_SEG NON_BANKED
interrupt void CAN1_RECEIVE_ISR(void)  //���
{
    unsigned long can1ReceiveID;
    unsigned char i,fChg2bmsbyte[8];
    unsigned long pp=0;
    unsigned long pp1=0;
    unsigned int VolLow=0;
    unsigned int VolHigh=0;
	
    if(!(CAN1RFLG&0x01))
    {
        CAN1RFLG = 0x01;     
        EnableInterrupts;
        return;
    }
    
    pp = (unsigned long)CAN1RXIDR0<<15;
    pp1 = (unsigned long)((CAN1RXIDR1>>5)&0x07)<<15; 
    
    can1ReceiveID =((unsigned long)pp<<6) | //expanded
              (pp1<<3) |
              ((unsigned long)(CAN1RXIDR1&0x07)<<15)|
              ((unsigned long)CAN1RXIDR2<<7) | 
              ((unsigned long)CAN1RXIDR3>>1);
    for(i=0;i<8;i++) 
        fChg2bmsbyte[i] = *((&CAN1RXDSR0)+i);
       
    switch(can1ReceiveID)
    {
		case 0x0C0217A7://�ܵ繭����wife      /////////����ܵ繭����߿��CAN�ڴ˽��ձ���
			VCU_ChgControl.byte = fChg2bmsbyte[3];
//			WifeLife=fChg2bmsbyte[7];
			ACCOverTime = 0;//����
			break;
        case 0x1826f456: //CHM
//			if((fChg2bmsbyte[0]==0x01)&&(fChg2bmsbyte[1]==0x01)&&(fChg2bmsbyte[2]==0))
//			{
				DCStartState=2;//�¹��꿪ʼ�ı�־ 
				DC_Vesion = 2; //�¹���
				if(CHMStep<=0x01)
					CHMStep=0x01;
//			}
            break;
        case 0x1801f456:  //CRM
            BEMStop=0;
            CRMOverTimeBefore60s = 0;
            if( fChg2bmsbyte[0]==0x00)
            {              
                DCStartState=1;//�Ϲ��꿪ʼ�ı�־
                //if(CHMStep==0x07) 
                //    ReCRMFlag=1;
                CRMOverTimeBefore = 0;
                if((CHMStep<=0x01)||((CHMStep==0x07)&&(OffState==0)))//���������ʱ�������¿�ʼ���                
                    CHMStep=0x01;
            }
            else if( fChg2bmsbyte[0]==0xaa)
            {
                CRMOverTime=0;
                DC_Start = 1;
                if(CHMStep<=0x02)
                    CHMStep=0x02;
                //TurnOn_INA2K();
                //SelfState2=1;
            }            
            break;
            
        case 0x1cecf456:
           if((fChg2bmsbyte[0]==0x11)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x02)&&((fChg2bmsbyte[7]==0x00)))
               FlagBRMSend = 1; //��־λ��1,��ʼ���Ͷ�֡��֤,���Ҳ�����BRM
                //if((fChg2bmsbyte[0]==0x13)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x02)&&((fChg2bmsbyte[7]==0x00)))
                     //BRMStep = 0;//����־λ����,���Դ��·���BRM
                     
            if((fChg2bmsbyte[0]==0x11)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x06)&&((fChg2bmsbyte[7]==0x00)))
                 FlagBCPSend = 1;
            //if((fChg2bmsbyte[0]==0x13)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x06)&&((fChg2bmsbyte[7]==0x00)))
                 //BCPStep = 0;//����־λ����,���Դ��·���BCP
            
            if((fChg2bmsbyte[0]==0x11)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x11)&&((fChg2bmsbyte[7]==0x00)))
                 FlagBCSSend = 1;
            
            //if((fChg2bmsbyte[0]==0x13)&&(fChg2bmsbyte[5]==0x00)&&(fChg2bmsbyte[6]==0x11)&&((fChg2bmsbyte[7]==0x00)))
                 //BCSStep = 0;//����־λ����,���Դ��·���BRM
                 
            break;
                
        case 0x1807f456:    
            //CHMStep=0x03;
            break;  
        case 0x1808f456:
            VolHigh = fChg2bmsbyte[1];
            VolHigh = VolHigh<<8;
            VolHigh = (VolHigh+fChg2bmsbyte[0]);

            VolLow = fChg2bmsbyte[3];
            VolLow = VolLow<<8;
            VolLow = (VolLow+fChg2bmsbyte[2]);
            if((VolLow>HIGHEST_VOL)||(VolHigh<LOWEST_VOL))
            {
                BROErrorAA=1;//������������            
            } 
            else
            {
                BROErrorAA=0;//����������
            }
            if(CHMStep<=0x03)   
                CHMStep=0x03;
            break;  
        case 0x100af456:
            CROOverTime = 0;//�յ�CRO������
            if( fChg2bmsbyte[0]==0xaa)
            {
                if(CHMStep<=0x04)
                    CHMStep=0x04;
                SelfState3=1;
                CROOverTime60s = 0;//�յ�0xaa������           
            } 
            break; 
        case 0x1812f456:
            
            CCSOverTime=0;
            CEMflag=0;///////��ֹ���׮�ҷ�CEM            
            DC_ChargeTime = fChg2bmsbyte[5];
            DC_ChargeTime = VolHigh<<8;
            DC_ChargeTime = (VolHigh+fChg2bmsbyte[4]);  
            if(CHMStep<=0x05)
                CHMStep=0x05;
            break;
        case 0x101af456:
            g_received_CST = 1;
            m_askcurrent = 0;//�������Ϊ0 
            if(((fChg2bmsbyte[0])&(0x40)) == 0)
            {
                fastend1 = 0x40;
            }
            CSTOverTime=0;
            if(CHMStep<=0x06)    
                CHMStep=0x06;
            break; 
                  
        case 0x181df456:
            CSDOverTime=0;
                         
            if(CHMStep<=0x07)     
                CHMStep=0x07;
            break;
        case 0x081ff456:
            BEMStop = 1;
            CEMflag++;
            if(CEMflag>=3) 
            {
                CEMflag=0;
                if(CHMStep<=0x07)     
                    CHMStep=0x07;
            }
            break;
      default:
            break;
    }//end of swtich 
    
    // Clear RXF flag (buffer ready to be read)
    CAN1RFLG = 0x01;        
}

#pragma CODE_SEG DEFAULT
//******************************************************************************
//* Function name:   CAN2_RECEIVE_ISR
//* Description:     CAN2�����ж��ӳ���,������BMU����λ�������ڲ�����ͨ��
//*                  �ڲ�/ 250Bd /��չ֡ 
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
#pragma CODE_SEG NON_BANKED
interrupt void CAN2_RECEIVE_ISR(void)   //�ڲ�  BMU   250Hz
{
    unsigned char i,j;
    unsigned int temper;
    unsigned long pp=0;
    unsigned long pp1=0;
    unsigned long DBuffer[1];
    unsigned int DBuffer1[1];
    unsigned int buffer = 0;
    unsigned int sbms_current1_t = 0;
	unsigned int sbms_current2_t = 0;
    unsigned int V4 = 0;
    unsigned int V5 = 0;
    unsigned int V6 = 0;
    unsigned int HeatCurrent = 0; 
//	unsigned long int DBuffer[1];
//	Checks for received messages
    static unsigned char FireSave=0;
    static unsigned char saveCounter=0; 
    
    if(!(CAN2RFLG&0x01))
    {
        CAN2RFLG = 0x01;     
        EnableInterrupts;
        return;
    }  
    
    pp = (unsigned long)CAN2RXIDR0<<15;  
    pp1 = (unsigned long)((CAN2RXIDR1>>5)&0x07)<<15;                              
    g_mboxID = ((unsigned long)pp<<6) | //expanded
               (pp1<<3) |
               ((unsigned long)(CAN2RXIDR1&0x07)<<15)|
               ((unsigned long)CAN2RXIDR2<<7) | 
               ((unsigned long)CAN2RXIDR3>>1);
   
    for(i=0;i<8;i++){
        g_mboxData[0][i]= *((&CAN2RXDSR0)+i);
    }
	
    switch(g_mboxID)//
    {
        case 0x000c0123://SBMS��Ϣ1
        
            SBMSOverTime=0;//��ʱ��ʱ��־λ��0
            sbms_current1_t = (((unsigned int)g_mboxData[0][0]&0x00ff)<<8) | g_mboxData[0][1];
            g_SBMS_current1 = sbms_current1_t*0.02 - 400;//���ȵ���
            V4 = (((unsigned int)g_mboxData[0][2]&0x00ff)<<8) | g_mboxData[0][3];
            g_highVoltageV4 = V4*0.02;
            V5 = (((unsigned int)g_mboxData[0][4]&0x00ff)<<8) | g_mboxData[0][5];
            g_highVoltageV5 = V5*0.02;
            V6 = (((unsigned int)g_mboxData[0][6]&0x00ff)<<8) | g_mboxData[0][7];
            g_highVoltageV6 = V6*0.02;
            
            break;
        case 0x000c0124:  //�����ϸ澯��֤��Ŀ����������Ҫ�ſ�
            SBMSOverTime=0;//��ʱ��ʱ��־λ��0
            FireSave = g_mboxData[0][0];//����Ԥ����Ϣ
            if(FireSave>=1) 
            {
                saveCounter++;
                if(saveCounter>=5) 
                {
                    saveCounter=6;
                    Error_Group0.Bit.F0_Fire_Warning=g_mboxData[0][0];//����Ԥ����Ϣ
                }
            } 
            else{
                saveCounter=0;
            }
			
            InsRelayState = g_mboxData[0][1];   //��Ե���Ƽ̵���״̬
			sbms_current2_t = (((unsigned int)g_mboxData[0][2]&0x00ff)<<8) | g_mboxData[0][3];
			g_SBMS_current2 = sbms_current2_t * 0.02 - 400;
            break;
        case 0xC01EE3F:
            if((g_mboxData[0][0]==0x55)&&(g_mboxData[0][1]==0xAA))
            {
                TestState=1;//���Ա�־λ��1,�������״̬��177
                g_BmsModeFlag = TESTSTATE;//����ģʽ
            }
            InTestToPC();
            break;
        case 0xC01EE27: //���µ����
            if(g_mboxData[0][2]==0x55)//�����̵����պ�
            {
				//�պ�����    
				TurnOn_INZK();//���������Ӵ���
//				state_group1.Bit.St_P_Relay=1;//to vcu
            }              
            if(g_mboxData[0][2]==0xaa)  
            { 
                TurnOff_INZK();//�Ͽ������Ӵ���
//				state_group1.Bit.St_P_Relay=1;//to vcu  
            }
            bmsToPcInfo552(); //���ͼ̵���״̬����ɣ�BMS���ֵ�ǰ״̬  
            break;
        case 0xC01EE3a: //Ԥ��
            if(g_mboxData[0][2]==0X55) 
            {
				TurnOn_INBK();//����Ԥ��Ӵ���
            }
            if(g_mboxData[0][2]==0xAA)  
            { 
                TurnOff_INBK();//�Ͽ�Ԥ��Ӵ���
            }        
            break;
        case 0xC01EE3b://����
            if(g_mboxData[0][2]==0x55) 
            {   
        	      TurnOn_INHK();//���ϼ��ȽӴ��� 
            }
            if(g_mboxData[0][2]==0xAA)  
            { 
                TurnOff_INHK();//�Ͽ����ȽӴ���
            } 
            break;
        case 0xC01EE3d: //���
            if(g_mboxData[0][2]==0x55) 
            {   
				TurnOn_INA2K();//���ϳ��Ӵ���
            }
            if(g_mboxData[0][2]==0xAA)  
            { 
                TurnOff_INA2K();//�Ͽ����Ӵ���
            } 
            break;
        case 0xC01EE3e: //�����̵�������0xC01EE3a         
            if(g_mboxData[0][2]==0x55) 
            {
                //�պ��ܸ�    
				TurnOn_INFK();//���ϸ����Ӵ���
//				state_group1.Bit.St_N_Relay=1;//to vcu 
            }
            if(g_mboxData[0][2]==0xaa)  
            { 
                TurnOff_INFK();//�Ͽ������Ӵ���
//				state_group1.Bit.St_N_Relay=0;//to vcu
            }
            break;
        case 0xC0777: //�����ŵ�����
            DBuffer[0]=0;
            DBuffer1[0]=0;
            DisableInterrupts;
            Write24c64_Byte(AH_CHARGE_ADDR,(unsigned char* )DBuffer,4);
            Write24c64_Byte(AH_DISCHARGE_ADDR,(unsigned char* )DBuffer,4);
            Write24c64_Byte(ChangerTime_ADDR,(unsigned char* )DBuffer1,2);
            DisableInterrupts;
            break;
       case 0xC0888:
            ReadChargeDischargeAH();//��ȡ����
            //StoreChargeTime();
            ReadAHRecord();//��������0xC0AAA
            break;
        case 0xC0999:
            //StoreChargeTime();
            break;
        case 0x18FF0A00:         
            BMNVPNflag.Bit.flag0=1;
            if(g_mboxData[0][0]==25) 
            { 
                g_mboxData[0][0]=0;
                BMNVPNflag.Bit.flag4=1;
            }
            if(g_mboxData[0][0]==35)
            { 
                g_mboxData[0][0]=0;
                BMNVPNflag.Bit.flag6=1;
            } 
            Task13_Vpn_Value_Calibrate();       //У׼�ܵ�ѹ       
            break;
        case  0x18FF0A01:
            Vpn_K1 = (((unsigned int)g_mboxData[0][0]&0x00ff)<<8) | g_mboxData[0][1];
            Vpn_B1 = (((unsigned int)g_mboxData[0][2]&0x00ff)<<8) | g_mboxData[0][3];
            KB_DATA_SAVE();
            break;
        case  0x18FF0A02:
            Vpn_K2 = (((unsigned int)g_mboxData[0][0]&0x00ff)<<8) | g_mboxData[0][1];
            Vpn_B2 = (((unsigned int)g_mboxData[0][2]&0x00ff)<<8) | g_mboxData[0][3];
            KB_DATA_SAVE();
            break;
        case  0x18FF0A03:
            Vpn_K3 = (((unsigned int)g_mboxData[0][0]&0x00ff)<<8) | g_mboxData[0][1];
            Vpn_B3 = (((unsigned int)g_mboxData[0][2]&0x00ff)<<8) | g_mboxData[0][3];
            KB_DATA_SAVE();
            break;
        
        case 0x18ff6200:  //�յ���ȡ������Ϣ������
            g_errorRecordRead_flag=1;  
            break;
        case 0x18ff6201: //������й�����Ϣ
            g_errorCounter=0;
            temper = g_errorCounter;
            Write24c64_Byte(ERROR_COUNT_ADDR,(unsigned char *)&temper,2);//���¹��ϼ����� 
            temper = 0;
            Read24c64_Byte(ERROR_COUNT_ADDR,(unsigned char *)&temper,2);
            g_errorCounter = temper;
            break;
        
        case 0x0a0218fb://����BOOT����
            DFlash_Erase_Sector(0x0000); //�����Ȳ���
            DataBuffer[0] = 0xaaaa;
            DFlash_Write_Word(0x0000);//д��boot��־0xaaaa,��boot������������־����ִ��bootloader���س��򣬷�֮����ִ��Ӧ�ó���                 
            COPCTL = 0x07;        //enable watchdog
            ARMCOP = 0x00;        //feed 0 to watch dog for reset 
            BootState = 1;
            break;
        case 0x0c14ee00:    //��ȡʱ��
        case 0xc01ee14:     //����ʱ��
        case 0xC01ee15:     //SOC����
        case 0xC01ef15:     //������ʾSOC
        case 0xC01ee16:     //ʣ������
        case 0xC01ee17:     //BMU����
        case 0xC01ee18:     //����汾��
        case 0xC01EE28:     //����ϵͳ����������Ϣfrom ��λ��
        case 0xC01EE29:     //����ϵͳ����������Ϣfrom ��λ��
        case 0xC01EE2a:     //����ϵͳ����������Ϣfrom ��λ��
        case 0xC01EE2b:     //����ϵͳ����������Ϣfrom ��λ��
        case 0xC01EE26:
            ParameterSetting();
            break;
        default:
			Int_Flag |= 0x08;
			BMU_Processure();
			break;  
    }//end of switch      	 		
    // Clear RXF flag (buffer ready to be read)
    CAN2RFLG = 0x01;       
}
#pragma CODE_SEG DEFAULT

//*******************************************
//*******************************************
