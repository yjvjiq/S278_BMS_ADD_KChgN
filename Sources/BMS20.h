#ifndef _BMS20_H_
#define _BMS20_H_
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : C50ES_FP
//* File Name          : BMS.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2011,05,26
//* Description        : ���ļ���Ϊϵͳͷ�ļ����Ը���Ŀ��ϵͳ�������ж���Ͷ�����ȫ�ֱ�������ȫ������
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------ÿ����Ŀ����Ҫ�޸ĵĲ���---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

///////////////////////////////////ϵͳ����//////////////////////////////////

#define PROJECT_NAME                278     //��Ŀ����

#define StoreAHState                0       //�����Ƿ���Դ洢
#define BMU_NUMBER                  18      //BMU����
#define G_BMU_CIRCLE_FLAG           0x3FFFF //BMU����
#define CELL_NUM                    612     //��ؽ���
#define CELL_TEMP_NUM               51      //15*3+3*2 = 51, 18*3 = 54.
#define CELL_VOL_GROUP              304     //the number of cell voltage group, 612/3=304, 3 cells/group
#define CELL_TEMP_GROUP             17      //the number of cell temperature group, 51/3=17, 3 temps/group
//#define C                         150     //��籶��
#define SetCap                      150     //��籶��  Ϊ������C  C��������̫����  
#define EleBand_Max_Current			500
#define FastChg_Max_Current			240

#define DubugMode                   1       //����״̬������

//////////////////////////////////ֱ�����//////////////////////////////////
#define HIGHEST_ALLOWED_CHARGE_CV   3.65    //��������絥���ѹ CV
#define HIGHEST_ALLOWED_CHARGE_V    744.6   //����������ܵ�ѹ V   3.65*204=744.6
#define LOWEST_ALLOWED_CHARGE_V     673.2   //����������ܵ�ѹ V   3.3*204=673.2

#define HIGHEST_ALLOWED_CHARGE_T    57      //���������͹����¶�   57
#define LOWEST_ALLOWED_CHARGE_T     0       //����������¶�   0
#define LOWEST_ALLOWED_WORK_T       -30     //����������¶�   -30

#define CHARGE_CUTDOWN_CV1          3.55    //��һ�ν�����ѹ
#define CHARGE_CUTDOWN_CV2          3.60    //�ڶ��ν�����ѹ
#define HIGHEST_ALLOWED_CHARGE_A    300.0   //ֱ�������������� A
#define SYS_NOMINAL_AH              150     //ϵͳ����� AH
#define SYS_NOMINAL_V               652.8   //ϵͳ���ѹ V//3.2*204= 652.8
#define SYS_KWH                     97.9    //ϵͳ����  //150*652.8=97920WH
#define LOWEST_ALLOWED_DISCHARGE_V  510     //��С�������ܵ�ѹ V   2.5*204=510
#define CONSTANT_VOL_WORK           1       //��ѹ����
#define CONSTANT_CURT_WORK          2       //��������
#define CELL_TYPE                   3       //01Ǧ����;02������;03:�������;04:�������;
                                            //05:�����;06:��Ԫ;07:�ۺ���;08:�����;FFH:����
                                          
#define HIGHEST_VOL                 7344    //CML�������:����������  3.6*204*10
#define LOWEST_VOL                  6528    //CML�������:��С�������   3.2*204*10

#define POWER_SUPPLY 24	// when it's 24 means the BMS power supply is 24V.	
// below is the 24V power supply
#if(POWER_SUPPLY == 24)
#define CC2VOLHIGH                  7.5     //CC2������Χ��
#define CC2VOLLOW                   4.2     //CC2������Χ��
#else
#define CC2VOLHIGH                  7.5     //CC2������Χ��
#define CC2VOLLOW                   2.0     //CC2������Χ��
#endif

////////////////////////���Ȳ�������//////////////////////////////////////////////
#define HIGHEST_ALLOWED_HEAT_CHARGE_V       673.2   //���������ȳ���ܵ�ѹ V   3.3*204=673.2
#define HIGHEST_ALLOWED_HEAT_CHARGE_C       2.13    //���������ȳ����� A                       ??
#define START_ALLOWED_PreHEAT_CHARGE_TEM    0       //����Ԥ���ȷ�ֵ  �ϵ����С�ڴ�ֵ ֱ�ӽ������                               ??
#define STOP_ALLOWED_PreHEAT_CHARGE_TEM     5       //�ر�Ԥ���ȷ�ֵ                                  ??
#define START_ALLOWED_HEAT_CHARGE_TEM       10      //�������ȷ�ֵ �ϵ����С�ڴ�ֵ ����߳��߼��� 
#define STOP_ALLOWED_HEAT_CHARGE_TEM        15      //�رռ��ȷ�ֵ 
#define BACK_HEAT_TEM                       -1      //�رռ��ȷ�ֵ 
#define COOL_TYPE                           3       //1 ��Ȼ��ȴ 2��������ȴ 3��ˮ��
#define Chg_Socet_Max_Temperatue_lv1		85
#define Chg_Socet_Max_Temperatue_lv2		120
///////////////////////BMS��󱣻��ķ�ֵ///////////////////////
#define LOWEST_CELL_VOL     2.0		//�ŵ���С�����ѹ
#define HIGHEST_CELL_VOL    3.85	//��絥��������ѹ
#define HIGHEST_BATT_VOL    775		//����ܵ�ѹ3.8*204 = 775.2
#define HIGHEST_TEM         62		//����¶�62

//////////////////////BMS���ʱ仯�ٶ�//////////////////////////
#define POWERSPEED  0.5       //���ʵı仯�ٶ�

//////////////////////��ѹ�ȼ�:BMS��Ե�����������ֵ//////////////////////////
#define RESVALUE  0.2       //��������Ϊ0.2K

//////////////////////BMS�궨��ѹʱ�����õ�ѹ//////////////////////////
#define HIGHVOL_19    350       //����0x19ʱ��Ӧ�ĵ�ѹ
#define HIGHVOL_23    550      //����0x23ʱ��Ӧ�ĵ�ѹ

#include <hidef.h>      /* common defines and macros */
#include <string.h>
#include "derivative.h"      /* derivative-specific definitions */
//�ײ�ͷ�ļ�
#include  "mc9s12gpio.h"
#include  "74hc595.h"
#include  "adc.h"
#include  "bmn.h"
#include  "mscan.h"
#include  "RTI.h"
#include  "sd2405.h"
#include  "dflash.h"
#include  "ProcessBMU.h"
#include  "current.h"
#include  "SocAHintegral.h"
#include  "Record.h"
#include  "Storage.h"
#include  "M95M02.h"
#include  "Connect.h"
//Ӧ�ò�ͷ�ļ�
#include  "Machine.h"
#include  "VehicleCAN.h"
#include  "InnerCAN.h"
#include  "FastChargeCAN.h"
#include  "SOF.h"
#include  "feedBack.h"
#include  "SocAdjust.h"
#include  "Heat.h"
#include  "MachineControl.h"
//#include "BatteryOverVoltage1_types.h"
#include "Fault.h"
#include "24LC64.h"
#include "InitBMS.h"
#include "mc9s12Isr.h"
#include "ParameterSetting.h"
#include "Q1Q2calculate.h"
#include "Task.h"
#include "DC_ChangerProcess.h"
#include "AC_ChangerProcess.h"
#include "ads1015.h"
#include "lookup_Table_Tem_NEW20170204.h"

#include "Q1.h"
#include "CellTempUnbalance.h"
#include "YoungMan_LT.h"
#include "DCChrgTime.h"

//#include "Isolation.h"
//*******************************************
//*******************************************
#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong unsigned long

#define U8  uchar
#define U16 uint
#define U32 ulong

#define  HIGH 0
#define  LOW 1

#define ON	1
#define OFF	0

#define on	1
#define off	0

// ����ṹ
typedef struct _TASK_COMPONENTS
{
    uchar Run;                 // �������б�ǣ�0-�����У�1����
    uint Timer;                // ��ʱ��
    uint ItvTime;              // �������м��ʱ��
    void (*TaskHook)(void);    // Ҫ���е�������
} TASK_COMPONENTS;       // ������
/////////
//************************************************************************
//************************************************************************
// �����嵥
typedef enum _TASK_LIST
{
    TASK_CURRENT,
    TASK_SOC,
    TASK_BMU,            
    TASK_VOLTAGE,             
    TASK_INSULATION,             
    TASK_REPORT2PC,
    TASK_STAT_MACHINE,
    TASK_FAULT,
    TASK_RECORD,
    //TASK_DC_RECHARGE,
    TASKS_MAX      // �ܵĿɹ�����Ķ�ʱ������Ŀ                                     
} TASK_LIST;

////////
enum slaveNumber
{
    S0,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7
};

//*******************************************
//*******************************************
enum parameter_list
{
    PARA_SOC_VALUE,    //SOCʵ��ֵ
    //PARA_DISCHARGE_RATIO,//������
    PARA_POSRELAY_PSTATE,//�����̵���ճ������  relayPstate
    //PARA_SINGLE_CAPACITY,  //����������AH
    PARA_PRECHARGE_RELAY_PSTATE,//Ԥ��̵���ճ������
    PARA_WHOLE_CAPACITY,//�����������
    PARA_BMU_NUMBER,//  �������

    PARA_DIFFERENCE_SINGLE_V, // �����ѹ����ֵ
    PARA_DIFFERENCE_SINGLE_T, // ����¶Ⱦ���ֵ
    PARA_BALANCE_ON_VOLTAGE, //���⿪����ѹֵ
    PARA_BALANCE_OFF_VOLTAGE,//����رյ�ѹֵ
    PARA_O_SOC_VALUE,// soc��

    PARA_OO_SOC_VALUE,// soc����
    PARA_L_SOC_VALUE,//soc��
    PARA_LL_SOC_VALUE,//soc����
    PARA_O_WHOLE_VOLTAGE, // �ܵ�ѹֵ��
    PARA_OO_WHOLE_VOLTAGE, // �ܵ�ѹֵ����

    PARA_L_WHOLE_VOLTAGE, // �ܵ�ѹֵ��
    PARA_LL_WHOLE_VOLTAGE, // �ܵ�ѹֵ����
    PARA_O_CELL_VOLTAGE, // �����ѹ�ߣ���ѹֵ��
    PARA_OO_CELL_VOLTAGE, // �����ѹ����
    PARA_L_CELL_VOLTAGE,//  �����ѹ��(Ƿѹֵ)

    PARA_LL_CELL_VOLTAGE,//  �����ѹ����
    PARA_O_TEMPERATURE,//����
    PARA_OO_TEMPERATURE,//�¶ȼ���
    PARA_L_TEMPERATURE, // �¶ȵ�
    PARA_LL_TEMPERATURE, // �¶ȼ���

    PARA_O_CONSISTENCY_V, //��ѹһ���Բ�
    PARA_OO_CONSISTENCY_V,//��ѹһ���Լ���
    PARA_O_CONSISTENCY_T, //�¶�һ���Բ�
    PARA_OO_CONSISTENCY_T,//�¶�һ���Լ���
    PARA_CURRENT,//   ������ֵ

    PARA_HIGHEST_RECHARGE_VOLTAGE,//������߳��˵�ѹ
    PARA_HIGHEST_RECHARGE_CURRENT,//������������ 
    PARA_HIGHEST_RECHARGE_CURRENT_TIME,//������������ʱ��
    PARA_HIGHEST_DISCHARGE_CURRENT,//�������ŵ���� 
    PARA_HIGHEST_CHARGE_CURRENT_TIME,//����ŵ����������ʱ��

    PARA_L_ISOLATION_RESISTANCE, //  ��Ե�����ֵ
    PARA_LL_ISOLATION_RESISTANCE, //  ��Ե�������ֵ

    PARA_ENDFLAG
};
enum storage_list 
{
    INDEX = 0,
    TOTAL_VOLTAGE,			// 1
    TOTAL_CURRENT,			// 2
    PARA_SOC_DISPLAY,		// 3 SOCֵ
    SYS_CONTACTORS_STATE,	// 4 �����ܸ��̵���״̬

    PARA_ERROR_LEVER,		// 5 ���ϵȼ�ֵ  1:1������  2:2������
    CAUTION_FLAG_1,			// 6
    CAUTION_FLAG_2,			// 7
    CAUTION_FLAG_3,			// 8
    CAUTION_FLAG_4,			// 9

    SYS_REALTIME_SECOND,	// 10
    SYS_REALTIME_MINUTE,	// 11
    SYS_REALTIME_HOUR,		// 12
    SYS_REALTIME_WEEK,		// 13
    SYS_REALTIME_DAY,		// 14

    SYS_REALTIME_MONTH,		// 15
    SYS_REALTIME_YEAR,		// 16
    CELL_MAX_TEM,			// 17
    CELL_MIN_TEM,			// 18
    CELL_AVERAGE_TEM,		// 19

    CELL_MAX_VOLTAGE,		// 20
    CELL_MIN_VOLTAGE,		// 21
    CELL_AVERAGE_VOLTAGE,	// 22
    SYS_INSULATION_P,		// 23
    SYS_INSULATION_N,		// 24
    
    VOLT_K1,				// 25
    VOLT_B1,				// 26
    VOLT_K2,				// 27
    VOLT_B2,				// 28
    VOLT_K3,				// 29
    VOLT_B3,				// 30
    
    VERIFICATION			// 31 У��		
};

enum storage_cell_vol_list 
{  
    CELL_VOLTAGE_0,
    CELL_VOLTAGE_1,
    CELL_VOLTAGE_2,
    CELL_VOLTAGE_3,
    CELL_VOLTAGE_4,
    CELL_VOLTAGE_5,
    CELL_VOLTAGE_6,
    CELL_VOLTAGE_7,
    CELL_VOLTAGE_8,
    CELL_VOLTAGE_9,

    CELL_VOLTAGE_10,
    CELL_VOLTAGE_11,
    CELL_VOLTAGE_12,
    CELL_VOLTAGE_13,
    CELL_VOLTAGE_14,
    CELL_VOLTAGE_15,
    CELL_VOLTAGE_16,
    CELL_VOLTAGE_17,
    CELL_VOLTAGE_18,
    CELL_VOLTAGE_19,

    CELL_VOLTAGE_20,
    CELL_VOLTAGE_21,
    CELL_VOLTAGE_22,
    CELL_VOLTAGE_23,
    CELL_VOLTAGE_24,
    CELL_VOLTAGE_25,
    CELL_VOLTAGE_26,
    CELL_VOLTAGE_27,
    CELL_VOLTAGE_28,
    CELL_VOLTAGE_29,

    CELL_VOLTAGE_30,
    CELL_VOLTAGE_31,
    CELL_VOLTAGE_32,
    CELL_VOLTAGE_33,
    CELL_VOLTAGE_34,
    CELL_VOLTAGE_35,
    CELL_VOLTAGE_36
    
};

typedef struct {
	U16	cell_V[BMU_NUMBER][CELL_NUMBER];	// all BMU's cell voltage
	U8	cell_T[BMU_NUMBER][Tem_NUMBER];		// all BMU's cell temperature
	
	U16	cell_V_max[BMU_NUMBER];				// each BMU's cell max voltage
	U16	cell_V_min[BMU_NUMBER];				// each BMU's cell min voltage
	
	U8	cell_T_max[BMU_NUMBER];				// each BMU's cell max temperature
	U8	cell_T_min[BMU_NUMBER];				// each BMU's cell min temperature

	U8	cell_V_max_group_num[BMU_NUMBER];				// the max voltage in the BMU's LTC6802 number
	U8	cell_V_min_group_num[BMU_NUMBER];				// the min voltage in the BMU's LTC6802 number
	U8	cell_V_max_num[BMU_NUMBER];			// the max voltage in the BMU's LTC6802's cell number
	U8	cell_V_min_num[BMU_NUMBER];			// the min voltage in the BMU's LTC6802's cell number

	U8	cell_T_max_group_num[BMU_NUMBER];	// the max temperature in the BMU's LTC6802 number
	U8	cell_T_min_group_num[BMU_NUMBER];	// the min temperature in the BMU's LTC6802 number
	U8	cell_T_max_num[BMU_NUMBER];			// the max temperature in the BMU's LTC6802's cell number
	U8	cell_T_min_num[BMU_NUMBER];			// the min temperature in the BMU's LTC6802's cell number
} BMU_CELL_MSG;

typedef struct{
	U16 CellVoltageMax;
	U16 CellVoltageMin;
	
	U8	CellVoltageMaxPackNum;
	U8	CellVoltageMinPackNum;
	
	U8	CellVoltageMaxNum;
	U8	CellVoltageMinNum;
	
	U8	CellTempMax;
	U8	CellTempMin;

	U8	CellTempMaxPackNum;
	U8	CellTempMinPackNum;

	U8	CellTempMaxNum;
	U8	CellTempMinNum;
}BMS_CELL_MSG;


#define DEVICE_SERIAL_NUMBER 0x11    //	�������к� 1byte
#define HARDWARE_VERSION 0x01    //	Ӳ���汾 ��4λ��Ч   
#define SOFTWARE_VERSION 0x01   // 	����汾 ��4λ��Ч
#define COMMUNICATION_VERSION 0x01  // ͨѶЭ��汾 ��4λ��Ч

//#define ParamNUMBER 38
#define ENDFLAG 96

#define DISCHARGING 0 //�ŵ�
#define FASTRECHARGING 1  //���
#define RECHARGING 2  //���
#define TESTSTATE 4  //����ģʽ

//************ main.c ****************************************
extern unsigned char BMS_SW_Version[8];
extern unsigned char BMUOK;
extern unsigned char HeatFlag; //�ϵ�ǰ�¶ȱ�־λ

extern BMU_CELL_MSG			g_bmu_msg;
extern BMS_CELL_MSG			g_bms_msg;
extern U_BMS_STATUS_TYPE	g_bms_status;
extern U_BMS_FAULT_TYPE		g_bms_fault_msg;
extern U_TMS_BMS_MSG		g_TMS_BMS_msg;
extern U_BMS_TMS_MSG		g_BMS_TMS_msg;

 //***********************************************************************************
//*******************************the end**********************************************
//************************************************************************************
///////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////

#endif
