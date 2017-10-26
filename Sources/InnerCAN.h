//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : InnnerCAN.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : ���ļ��Ƕ�CAN���͵ı��Ľ���BIT����
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

///////////////0x450////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte flag0:1;       
		byte flag1:1;              
		byte initFinished:1;              
		byte flag3:1;
		byte BmsCtlStat:1;
		byte NegRelayState:1;
		byte PrechargeRelayStat:1;
		byte flag7:1;
	}Bit;
}CAN450BYTE0;    
extern CAN450BYTE0      Can450Byte0;

typedef union{
	unsigned char byte;
	struct {
		byte flag0:1;       
		byte flag1:1;              
		byte flag2:1;              
		byte BMSLowPowerOffRequest:1;
		byte flag4:1;
		byte ChgPowreOffReq:1;
		byte bcuSelfcheckCounter:2;
	}Bit;
}CAN450BYTE7;    
extern CAN450BYTE7      Can450Byte7;
/////////////0x554///////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte F0_systemUV2:1;       
		byte F1_cellUV2:1;              
		byte F2_systemOV2:1;              
		byte F3_cellOV2:1;
		byte F4_insulationLow2:1;
		byte F5_cellOT2:1;
		byte F6_cellUT2:1;
		byte F7_ChangerOCurt2:1;
	}Bit;
}CAN554BYTE0;    
extern CAN554BYTE0      Can554Byte0;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte F0_DisChaOCurt2:1;       
		byte F1_SOCLow2:1;              
		byte F2_CellUnbalance2:1;              
		byte F3_tempUnbalance2:1;
		byte flag4:1;
		byte flag5:2;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN554BYTE1;    
extern CAN554BYTE1      Can554Byte1;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte F0_systemUV1:1;       
		byte F1_cellUV1:1;              
		byte F2_systemOV1:1;              
		byte F3_cellOV1:1;
		byte F4_insulationLow1:1;
		byte F5_cellOT1:1;
		byte F6_cellUT1:1;
		byte F7_ChangerOCurt1:1;
	}Bit;
}CAN554BYTE2;    
extern CAN554BYTE2      Can554Byte2;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte F0_DisChaOCurt1:1;       
		byte F1_SOCLow1:1;              
		byte F2_CellUnbalance1:1;              
		byte F3_tempUnbalance1:1;
		byte F4_ChargeCurDirError:1;
		byte flag5:1;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN554BYTE3;    
extern CAN554BYTE3      Can554Byte3;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
	  byte flag0:1; 
		byte flag1:1;       
		byte flag2:1;              
		byte flag3:1;              
		byte flag4:1;
		byte flag5:1;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN554BYTE4;    
extern CAN554BYTE4      Can554Byte4;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
	  byte flag0:1; 
		byte flag1:1;       
		byte flag2:1;              
		byte flag3:1;
		byte flag4:1;
		byte flag5:1;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN554BYTE5;    
extern CAN554BYTE5      Can554Byte5;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte flag0:1;       
		byte flag1:1;              
		byte flag2:1;              
		byte flag3:1;
		byte flag4:1;
		byte flag5:1;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN554BYTE6;    
extern CAN554BYTE6      Can554Byte6;
//////////////////////////////////////////////////////////
typedef union{
	unsigned char byte;
	struct {
		byte flag0:1;       
		byte flag1:1;              
		byte flag2:1;              
		byte flag3:1;
		byte flag4:1;
		byte flag5:1;
		byte flag6:1;
		byte flag7:1;
	}Bit;
}CAN554BYTE7;    
extern CAN554BYTE7      Can554Byte7;
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//************** mscan.c ********************
extern unsigned char  C0552_0;
extern unsigned char  C0552_1;
/***********************��λ�����Ϻ�״̬START************************/

typedef union{
	unsigned char byte;
	struct {
	
	  byte St_CC_16A_PC:1;	                               //CC_16״̬
	  byte St_CC_32A_PC:1;                                 //CC_32״̬
	  byte St_CP_PC:1;                                     //CP״̬
	  byte St_ACC_PC:1;                                    //ACC״̬
	  byte St_ChargeIN_PC:1;                               //ChargeIN״̬
	  byte St_CC2_PC:1;                                    //CC2
	  byte Reserve_Bit25:1;	        		                  //Ԥ��
	  byte Reserve_Bit26:1;	        		                  //Ԥ��
	  
	}Bit;
}STATE_PC552_GROUP1;


typedef union{
	unsigned char byte;
	struct {
	
	  byte St_N_Relay_PC:1;	                          //�����̵���״̬
	  byte St_P_Relay_PC:1;                           //�����̵���״̬
	  byte St_H_Relay_PC:1;                           //���ȼ̵���״̬
	  byte St_Pre_Relay_PC:1;                         //Ԥ��̵���״̬
	  byte St_C_Relay_PC:1;                           //���̵���״̬
	  byte Reserve_Bit22:1;                           //Ԥ��
	  byte Reserve_Bit23:1;	        		              //Ԥ��
	  byte Reserve_Bit24:1;	        		              //Ԥ��
	  
	}Bit;
}STATE_PC552_GROUP2; 


typedef union{
	unsigned char byte;
	struct {
	
	  byte F3_Battery_Under_Voltage_PC:1;	             //�ܵ�ѹ����
	  byte F3_Cell_Under_Voltage_PC:1;                 //�����ѹ����
	  byte F3_Battery_Over_Voltage_PC:1;	             //�ܵ�ѹ����
	  byte F3_Cell_Over_Voltage_PC:1;                  //�����ѹ��	  
	  byte F3_Insulation_Low_PC:1;                     //��Ե��
	  byte F3_Over_Temprature_PC:1;                    //�¶ȹ���
	  byte F3_Under_Temprature_PC:1;                   //�¶ȹ���
	  byte F3_Charge_Over_Current_PC:1;                //������

	}Bit;
}FAULT_PC552_GROUP1;

typedef union{
	unsigned char byte;
	struct {
	
	  byte F3_Disharge_Over_Current_PC:1;                //�ŵ����
	  byte F3_SOC_Low_PC:1;                              //soc��
	  byte F3_Cell_Voltage_Unbalance_PC:1;	             //�����ѹ������
	  byte F3_Temprature_Unbalance_PC:1;	               //�¶Ȳ�����
	  byte F3_Communiction_OBC_PC:1;	                   //�복�س���ͨ�Ź���
	  byte Reserve_Bit0:1;                               //����
	  byte Reserve_Bit1:1;                               //����
	  byte Reserve_Bit2:1;                               //����

	}Bit;
}FAULT_PC552_GROUP2;


typedef union{
	unsigned char byte;
	struct {
	  byte F_MSD_Break_PC:1;                          //MSD��·����
	  byte F_N_Relay_Stick_PC:1;                      //�����̵���ճ������
	  byte F_N_Relay_Break_PC:1;                      //�����̵�����·����
	  byte F_Pre_Res_Break_PC:1;                      //Ԥ������·����
	  byte F_Pre_Relay_OR_P_Relay_Stick_PC:1;         //Ԥ��̵����������̵���ճ��
	  byte F_Pre_Relay_Break_PC:1;                    //Ԥ��̵�����·����
	  byte F_P_Relay_Break_PC:1;                      //�����̵�����·����
	  byte F_Pre_Charging_Failure_PC:1;               //Ԥ���ʧ�ܹ���	        		       
	}Bit;
}FAULT_PC552_GROUP3;   


typedef union{
	unsigned char byte;
	struct {
	
	  byte F_Inner_Communiction_PC:1;	                //�ڲ�ͨ�Ź���
	  byte F_Current_Sensor_PC:1;                     //��������������
	  byte F_Temperature_Sensor_PC:1;                 //�¶ȴ���������
	  byte F_Vlotage_Sensor_PC:1;                     //��ѹ����������
	  byte F_M95EEPROM_Failure_PC:1;                  //M95EEPROM����
    byte F_24C64EEPROM_Failure_PC:1;                //24C64EEPROM����
	  byte F_C_Relay_Stick_PC:1;                      //���̵���ճ������
	  byte F_C_Relay_Break_PC:1;                      //���̵�����·����
  		        		       
	}Bit;
}FAULT_PC552_GROUP4;

/*typedef union{
	unsigned char byte;
	struct {
	
	  byte LockAction:2;	    //������������������
	  byte ErrorStateDone:2;  //������ʧ��״̬���
    byte flag:4;  		        		       
	}Bit;
}PC750_GROUP;
*/
//////////**********��������ͨѶ***********/////////////////
typedef union{
	unsigned char byte;
	struct {
	
	  byte Fan1:1;	                               //���ȿ���1
	  byte Fan2:1;                                 //���ȿ���2
	  byte Fan3:1;                                 //���ȿ���3
	  byte Fan4:1;                                 //���ȿ���4
	  byte flag:4;                                 //Ԥ��
	}Bit;
}BMS_FAN_CONTROL;


//extern PC750_GROUP pc750_group;

extern STATE_PC552_GROUP1   state_pc552_group1;
extern STATE_PC552_GROUP2   state_pc552_group2;


extern FAULT_PC552_GROUP1   fault_pc552_group1;
extern FAULT_PC552_GROUP2   fault_pc552_group2;
extern FAULT_PC552_GROUP3   fault_pc552_group3;
extern FAULT_PC552_GROUP4   fault_pc552_group4;
extern BMS_FAN_CONTROL  BMS_Fan_Control;


extern unsigned char  BmsCtlStat0;

extern void bmsToPcInfo450(void);
extern void bmsToPcInfo451(void);
extern void bmsToPcInfo552(void);
extern void bmsToPcInfo553(void);
extern void bmsToPcInfo554(void);
extern void bmsToPcInfo750(void);
extern void bmsToSBMSMessage1(void);

extern void sendBMSIDtoBMU(void);
extern void sendRealtimeToBMU(void); //��BMU����BMS��ǰʵʱʱ��
extern void sendBMSSFVersionToPC(void); //��BMU����BMS��ǰʵʱʱ��


extern void RecordSystem(void);      //��ʷ����
extern void RecordFaultTemperture(void);
extern void RecordRealtime(void);
extern void RecordExtremCellVoltage(void);
extern void RecordInsulation(void);//
extern void RecordVoltKB(void);
extern void RecordCellVoltage(unsigned char number);
extern void RecordEndFlag(void);//��ʾ�������
extern void ReadAHRecord(void);
extern void InTestToPC(void);

extern float g_highVoltageV4; //�ܵ繭���̵�����ѹ
extern float g_highVoltageV5; //�����̵�����ѹ
extern float g_highVoltageV6;
extern unsigned char InsRelayControl;
//****************************************************
//*****************************************************8
//*******************************************************8