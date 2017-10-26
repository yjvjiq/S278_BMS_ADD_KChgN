
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : Machine.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2015.7.13
//* Description        : ���ļ��Ƕ�CAN���͵ı��Ľ���BIT����
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
extern unsigned int tmr_p1;
extern unsigned int tmr_p2;
extern unsigned int tmr_p3;
extern unsigned int tmr_p4;
extern unsigned int tmr_p5;
extern unsigned int tmr_p6;
extern unsigned char bmsSelfcheckCounter;
extern unsigned char  prechargeFinished;    //Ԥ�����ɱ�־λ

extern unsigned char  acc_Connect;
extern unsigned char preChargeON;

extern unsigned char MSDError;                 //MSD����
extern unsigned char N_RelayConnetError;       //�����̵���ճ��
extern unsigned char N_RelayDisConError;       //�����̵�����·
extern unsigned char PreCha_RelayConError;     //Ԥ��̵���ճ��
extern unsigned char P_RelayConError;          //�����̵���ճ��
extern unsigned char PreCha_RelayDisConError;  //Ԥ��̵�����·
extern unsigned char P_RelayDisConError;       //�����̵�����·
extern unsigned char CCHG_RelayConError;  //�ܵ繭���̵���ճ��
extern unsigned char DCCHG_RelayConError;  //���̵���ճ��
extern unsigned char CCHG_RelayDisConError;  //�ܵ繭���̵�����·
extern unsigned char DCCHG_RelayDisConError;  //���̵�����·
extern unsigned char CHG_N_RelayConError;      //��縺ճ��
extern unsigned char CHG_N_RelayDisConError;      //��縺��·
extern unsigned char Cha_ResDisConError;       //Ԥ������·
extern unsigned char PreCha_Error;             //Ԥ���ʧ��
extern unsigned char SelfCheck;                //BMS�Լ���: 2,δͨ��;1ͨ��
extern unsigned char RelayErrorPowerOff;       //�̵����µ���� 
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
extern void VehiclePowerOffLogic(void);
extern void closeNegRelay(void);
extern void openNegRelay(void);
extern void closePosRelay(void);  
extern void openPosRelay(void);
extern void HighVoltDetectPart1(void);
extern void HighVoltDetectPart2(void);
extern void HighVoltDetectPart3(void);
//extern void PRelayConnectTest(void);
//extern void NRelayConnectTest(void);
//extern unsigned char GetTavg(unsigned char LowTem,unsigned char HighTem); 
 //extern unsigned char GetTavgProcess(unsigned char LowTem,unsigned char HighTem); 
extern void GetTavgProcess(void);
//**********************************************************
//***********************************************************
//***********************************************************
//**********************************************************
//***********************************************************
//***********************************************************