//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : bmn.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���������ڼ����ܵ�ѹv1,v2,v3�;�Ե��ֵRp,Rn,���������вɼ���ѹ��Ե��ȫ�ֱ���
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////
// Defines
typedef union{
	unsigned char byte;
	struct {
		byte flag0:1;       //CANRx�궨�ж�
		byte flag1:1;       //60s �˲���־ 60s= Filter_N*6
		byte flag2:1;       //δ��
		byte flag3:1;       //100V ���ϱ궨
		byte flag4:1;       //200V �궨
		byte flag5:1;       //200V �궨
		byte flag6:1;       //400V �궨
		byte flag7:1;       //400V �궨
	}Bit;
}BMNVPNFLAG ;    //BMN VPN�궨�����־   

/////////////////////////////////////////////////////////////////////////////////////////

#define  CANID_BMNVPN    0x18FF0212
#define  CANID_RPNTOGND  0x18FF0312

#define  CANID_RPTOGND   0x18FF0700
#define  CANID_RNTOGND   0x18FF0800

//BJUID
#define BMN_Part_SerialNum   0x22        //�������к�
#define BMN_HW_Version       0x02        //Ӳ���汾
#define BMN_SW_Version       0x23        //����汾
#define BMN_CAN_Pro_Version  0x08        //ͨѶЭ��汾


//BMSID
//BMS->BJU
//#define  CANID_BMSID   0x18FF0100
//PC->BJU
//#define  CANID_BMNCC1  0x18FF025A   //����PC����ģʽ   Vpn250V
#define  CANID_BMNCC1  0x18FF0A00   //����PC����ģʽ   Vpn250V
#define  CANID_BMNCC2  0x18FF0800   //��Ե���ʹ�ܻ��ֹ��1=ʹ�ܣ�0=��ֹ
/////////////////////////////////////////////////////////////////////////////////////////
// Variables
/////////////////////////////////////////////////////////////////////////////////////////
extern BMNVPNFLAG      BMNVPNflag;
extern unsigned int Vpn_K1;              //
extern  int Vpn_B1;                       //
extern unsigned int Vpn_K2;              //
extern  int Vpn_B2;                       //
extern unsigned int Vpn_K3;              //
extern  int Vpn_B3;                       //

extern float V51_Voltage_CAL_Value;                 //V5���˵�ѹֵ������1�պϣ�����2�Ͽ���
extern float V52_Voltage_CAL_Value;                  //V5���˵�ѹֵ������1�Ͽ�������2�պϣ�
//extern long Rp_Vpn_Value;
//extern long Rn_Vpn_Value;
extern float Rp_Vpn_Value;
extern float Rn_Vpn_Value;

extern unsigned int   V51_Voltage_AD_Value; 
extern unsigned int  V52_Voltage_AD_Value;
extern float	g_highVoltageV1; //��ص�ѹ
extern float g_highVoltageV2; //Ԥ���̵�����Ԥ�����֮��ĵ�ѹ
extern float g_highVoltageV3;//Ԥ���̵�����һ�ߵĵ�ѹ

extern unsigned char flagV51;
extern unsigned char flagV52;
extern unsigned char ads1110Worked;

extern unsigned int Vpn1_Voltage_AD_Value;        //����ܵ�ѹ1����ֵ     0.01V  100~500V   100.00~500.00
//extern unsigned int Vpn1_Voltage_CAL_Value;       //����ܵ�ѹ1����ֵ     0.01V  100~500V   100.00~500.00
extern long Vpn1_Voltage_CAL_Value;       //����ܵ�ѹ1����ֵ     0.01V  100~500V   100.00~500.00
extern unsigned int Vpn2_Voltage_AD_Value;        //����ܵ�ѹ2����ֵ     0.01V  100~500V   100.00~500.00
//extern unsigned int Vpn2_Voltage_CAL_Value;       //����ܵ�ѹ2����ֵ     0.01V  100~500V   100.00~500.00
extern long Vpn2_Voltage_CAL_Value;       //����ܵ�ѹ2����ֵ     0.01V  100~500V   100.00~500.00
extern unsigned int Vpn3_Voltage_AD_Value;        //����ܵ�ѹ3����ֵ     0.01V  100~500V   100.00~500.00
//extern unsigned int Vpn3_Voltage_CAL_Value;       //����ܵ�ѹ3����ֵ     0.01V  100~500V   100.00~500.00
extern long Vpn3_Voltage_CAL_Value;       //����ܵ�ѹ3����ֵ     0.01V  100~500V   100.00~500.00
/////////////////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////////////////

//extern void Task01_BMN_Init(void);
extern void Task02_BMN_GlobalVariables_Init(void);
//extern void Task03_BMN_MCU_Init(void);
extern void Task04_BMN_FEED_COP(void);

//extern void Task06_BMN_DATA_RECALL(void);
//extern void Task07_BMN_DATA_SAVE(void);
extern void Task08_BMN_ReInit(void);
 
extern void Task09_BMN_GlobalVariables_Clear(void);

extern void Task11_Vpn_StartAD_Polling(void);
extern void Task13_Vpn_Value_Calibrate(void);
extern void Task14_R_Vp_GND_Value_Polling(void);
extern void Task15_R_Vn_GND_Value_Polling(void);
extern void Task17_Rp_Rn_Value_Calculate_Filter(void);

extern void Task21_CAN_SendGouup1(void);
extern void Task22_CAN_SendGouup2(void);

extern void TaskVoltage(void);
extern void TaskInsulation(void);

extern unsigned int GetU1adValue(void);
extern unsigned int GetU2adValue(void);
extern unsigned int GetU3adValue(void);
extern unsigned int GetInsulateVoltAd(void);
extern void TurnOffInsulation(void);
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

