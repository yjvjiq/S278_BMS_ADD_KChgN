//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : InnerCAN.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2016.1.12
//* Description        : ���ļ��������ڲ�CAN�����б�������
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "BMS20.h"
unsigned char  C0552_0=0;
unsigned char  C0552_1=0;
unsigned char BmsCtlStat0  = 0;               //�̵���״̬
STATE_PC552_GROUP1   state_pc552_group1;
STATE_PC552_GROUP2   state_pc552_group2;
FAULT_PC552_GROUP1   fault_pc552_group1;
FAULT_PC552_GROUP2   fault_pc552_group2;
FAULT_PC552_GROUP3   fault_pc552_group3;
FAULT_PC552_GROUP4   fault_pc552_group4;
CAN554BYTE0      Can554Byte0;
CAN554BYTE1      Can554Byte1;
CAN554BYTE2      Can554Byte2;
CAN554BYTE3      Can554Byte3;
BMS_FAN_CONTROL  BMS_Fan_Control;
//PC750_GROUP pc750_group;
float	g_highVoltageV4; // V1B
float	g_highVoltageV5; // V2B
float	g_highVoltageV6; // V3B
unsigned char InsRelayControl = 0;//��Ե���Ƽ̵�������
//*************************************************************************************
//*************************************************************************************
//*****************������BMS��PC����Ϣ�ӳ���*****************************************
//*************************************************************************************
//******************************************************************************
//* Function name:   bmsToPcInfo450
//* Description:     ���������źš�SOC�����ŵ繦�ʡ��²��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsToPcInfo450(void)
{
	struct can_msg mg;
	unsigned int buff;
	float rc = 0;
	static unsigned char life=0;
	float g_socValueSet=0;  //ʵ�ʷ��͸�����
	char tt=100;
	mg.RTR= FALSE;  
	mg.len = 8;
	mg.prty = 0;

	life++;
	mg.data[0] = life;   //�����ź�

	buff = (unsigned int)(g_socValue*100*2.5);
	mg.data[1] =(unsigned char)buff;// ��غɵ�״̬SOC �ֱ���0.4,��λ%

	rc = -BiggestFeedbackPower;
	buff = (unsigned int)(rc*100);	//��������������
	mg.data[2]= buff>>8;//���ֽ� 
	mg.data[3]= (unsigned char)buff;//���ֽ� 

	rc = BiggestDischargePower;
	buff = (unsigned int)(rc*100);	//���ŵ繦��
	mg.data[4]= buff>>8;//���ֽ� 
	mg.data[5]= (unsigned char)buff;//���ֽ� 

	mg.data[6] = g_highestTemperature - g_lowestTemperature;//������ �¶Ȳ�
	buff = (unsigned int)(PowerVOL*5);
	mg.data[7] =buff; ;//�ɵ��ϵͳ���룺0~3��о������ 4~7 BMS�����Ϊ��Դ��ѹ

	mg.id= 0x000c0450;
	while((!MSCAN2SendMsg(mg))&&(tt>0))
		tt--;
    //MSCAN2SendMsg(mg);
}
//******************************************************************************
//* Function name:   bmsToPcInfo451
//* Description:     ����ϵͳ������ϵͳ��ѹ�����������͵�ѹ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsToPcInfo451(void)
{
    struct can_msg mg;
    unsigned int buff;
    char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    buff = (unsigned int)((g_systemCurrent+400)*50);	
	  mg.data[0]= buff>>8;//����鵱ǰ�������ֽ� 
  	mg.data[1]= (unsigned char)buff;//����鵱ǰ�������ֽ� 

	  //buff = (unsigned int)(g_highVoltageV1*100);
	  buff = (unsigned int)(g_highVoltageV1*50);
	  mg.data[2]= buff>>8;//����鵱ǰ��ѹ���ֽ� 
	  mg.data[3]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ� 
	  
	  buff = (unsigned int)(g_highestCellVoltage*10000);
	  mg.data[4]= buff>>8;//��ص�����ߵ�ѹ���ֽ� 
	  mg.data[5]= (unsigned char)buff;//��ص�����ߵ�ѹ���ֽ�
	  
	  buff = (unsigned int)(g_lowestCellVoltage*10000);
	  mg.data[6]= buff>>8;//��ص�����͵�ѹ���ֽ�
	  mg.data[7]= (unsigned char)buff;//��ص�����͵�ѹ���ֽ�

	  mg.id= 0x000c0451; 
	  //while(!MSCAN2SendMsg(mg));
	  while((!MSCAN2SendMsg(mg))&&(tt>0))
        tt--;
}
//******************************************************************************
//* Function name:   bmsToPcInfo552
//* Description:     �̵����������󡢼̵���״̬�������������¶ȡ�����״̬
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsToPcInfo552(void)
{
    struct can_msg mg;
    char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= C0552_0; //����ģʽ��A�ڵļ��
    mg.data[1]= BmsCtlStat0 & 0xff;//BMS����״̬
    mg.data[2]= (g_highestTemperature-40)+48;//�������¶� 
	  mg.data[3]= (g_lowestTemperature-40)+48;//�������¶� 
	  
    mg.data[4]= g_caution_Flag_1; //����״̬
    mg.data[5]= g_caution_Flag_2;
    mg.data[6]= g_caution_Flag_3;
    mg.data[7]= g_caution_Flag_4;
    
	  mg.id= 0x000c0552; 
	  //while(!MSCAN2SendMsg(mg));
	  while((!MSCAN2SendMsg(mg))&&(tt>0))
        tt--;
}
//******************************************************************************
//* Function name:   bmsToPcInfo553
//* Description:     
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsToPcInfo553(void)
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char AH=0;
    char tt=100;
    
	mg.RTR= FALSE;  
	mg.len = 8;
	mg.prty = 0;

	AH = (unsigned char) g_realNominalCap;
	mg.data[0] = AH; // gpio

	AH = (unsigned char) Q2;
	mg.data[1]= AH;//

	buff = (unsigned int)(Can_g_socValue*100*2.5);
	mg.data[2] =(unsigned char)buff;// ��غɵ�״̬SOC �ֱ���0.4,��λ%

	buff = (unsigned int)(CC2VOL*5);
	mg.data[3] =buff ;// 

	//buff = (unsigned int)(g_highVoltageV2*100);
	buff = (unsigned int)(g_highVoltageV2*50);
	mg.data[4]= buff>>8;//����鵱ǰ��ѹ���ֽ� 
	mg.data[5]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ�

	//buff = (unsigned int)(g_highVoltageV3*100);
	buff = (unsigned int)(g_highVoltageV3*50);
	mg.data[6]= buff>>8;//����鵱ǰ��ѹ���ֽ� 
	mg.data[7]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ�


	mg.id= 0x000c0553; 
	//while(!MSCAN2SendMsg(mg));
	while((!MSCAN2SendMsg(mg))&&(tt>0))
		tt--;
	 
}
//******************************************************************************
//* Function name:   bmsToPcInfo554
//* Description:     BMS���ϼ�V2��V3��ѹ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsToPcInfo554(void)
{
    struct can_msg mg;
    char tt=100;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= Can554Byte0.byte; 
    mg.data[1]= Can554Byte1.byte; 
    mg.data[2]= Can554Byte2.byte;
    mg.data[3]= Can554Byte3.byte;
    
    mg.data[4]= (DCTem1+48);
    mg.data[5]= (DCTem2+48);
    
    mg.data[6]= 0;
    mg.data[7]= 0;
    
    mg.id= 0x000c0554; 
	  while((!MSCAN2SendMsg(mg))&&(tt>0))
        tt--;
}
//******************************************************************************
//* Function name:   bmsToSBMS��Ϣ
//* Description:     ����SBMS
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void bmsToSBMSMessage1(void)
{
	struct can_msg mg;
	char tt=100;

	mg.id= 0x000c0125;
	mg.RTR= FALSE;  
	mg.len = 8;
	mg.prty = 0;

	mg.data[0]= InsRelayControl;//��Ե���Ƽ̵����Ŀ��� 
	mg.data[1]= St_heatManage; 
	mg.data[2]= 0;
	mg.data[3]= 0;

	mg.data[4]= 0; 
	mg.data[5]= 0; 
	mg.data[6]= 0;
	mg.data[7]= 0;
	while((!MSCAN2SendMsg(mg))&&(tt>0))
		tt--;
}
//******************************************************************************
//* Function name:   bmsToPcInfo750
//* Description:     BMS���Ƶ�����
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
/*void bmsToPcInfo750(void)
{
    struct can_msg mg;
    unsigned int buff;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= pc750_group.byte; 
    mg.data[1]= 0; 
    mg.data[2]= 0;
	  mg.data[3]= 0;
	  
    mg.data[4]= 0; 
    mg.data[5]= 0; 
    mg.data[6]= 0;
	  mg.data[7]= 0;
 
    mg.id= 0x000c0750;
    while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--; 
	  //MSCAN2SendMsg(mg);
}  */
//**************************************************************
//**************************************************************
//*********************BMS TO BMU********************************
//**************************************************************
//******************************************************************************
//* Function name:   sendBMSIDtoBMU
//* Description:     ����������BMS ��ʶ��Ϣ��Ӳ���汾������汾��ͨѶЭ��汾�ȣ�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void sendBMSIDtoBMU(void)   //
{
    struct can_msg mg;
  	unsigned char i;
  	static unsigned char lifeSign =0;
  	
  	mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    lifeSign++;

  	mg.data[0]= DEVICE_SERIAL_NUMBER;
  	mg.data[1]= HARDWARE_VERSION|(SOFTWARE_VERSION<<4);
  	mg.data[2]= COMMUNICATION_VERSION;
  	mg.data[3]= lifeSign;
  	for(i=4;i<8;i++)
  		mg.data[i]=0;

  	mg.id = 0x98FF0100;
  	MSCAN2SendMsg(mg);

}

//******************************************************************************
//* Function name:   sendBMSSFVersionToPC
//* Description:     ��BMU�������������
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void sendBMSSFVersionToPC(void)
{
    struct can_msg mg;
	 
  	mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= BMS_SW_Version[0]; //BMS
    mg.data[1]= BMS_SW_Version[1];; //
    mg.data[2]= BMS_SW_Version[2];; //��Ŀ����
    mg.data[3]= BMS_SW_Version[3];; //��
    mg.data[4]= BMS_SW_Version[4];; //��
    mg.data[5]= BMS_SW_Version[5];; // �汾
    mg.data[6]= BMS_SW_Version[6];; // �汾��
  	mg.data[7]= BMS_SW_Version[7];; //�޸Ĵ���
  	
  	mg.id = 0x180b0c5a;
  	MSCAN2SendMsg(mg);
}
//******************************************************************************
//* Function name:   sendRealtimeToBMU
//* Description:     ��BMU����BMS��ǰʵʱʱ��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void sendRealtimeToBMU(void) //��BMU����BMS��ǰʵʱʱ��
{
    struct can_msg mg;
  	 
  	mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= CurrentTime[6]; //��
    mg.data[1]= CurrentTime[5]; //��
    mg.data[2]= CurrentTime[4]; //��
    mg.data[3]= CurrentTime[2]; //ʱ
    mg.data[4]= CurrentTime[1]; //��
    mg.data[5]= CurrentTime[0]; //�� 
    mg.data[6]= 0;
  	mg.data[7]= 0;
  	
  	mg.id = 0x98FF0200;
  	MSCAN2SendMsg(mg);

}

//**************************************************************
//**************************************************************
//*********************BMS TO PC ��ʷ����***********************
//**************************************************************
//******************************************************************************
//* Function name:   RecordSystem
//* Description:     ϵͳ��ѹ��������SOC��BMS����״̬
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordSystem(void)//
{
	struct can_msg mg;
	char tt=100;

	mg.RTR= FALSE;  
	mg.len = 8;
	mg.prty = 0;

	mg.data[0]= g_storageSysVariableOut[INDEX]>>8;
	mg.data[1]= g_storageSysVariableOut[INDEX];

	mg.data[2]= g_storageSysVariableOut[TOTAL_VOLTAGE]>>8;//����鵱ǰ��ѹ���ֽ� 
	mg.data[3]= g_storageSysVariableOut[TOTAL_VOLTAGE];//����鵱ǰ��ѹ���ֽ� 

	mg.data[4]= g_storageSysVariableOut[TOTAL_CURRENT]>>8;//����鵱ǰ�������ֽ� 
	mg.data[5]= g_storageSysVariableOut[TOTAL_CURRENT];//����鵱ǰ�������ֽ� 

	mg.data[6] = g_storageSysVariableOut[PARA_SOC_DISPLAY];// ��غɵ�״̬SOC �ֱ���0.4,��λ%

	mg.data[7]= g_storageSysVariableOut[SYS_CONTACTORS_STATE];//BMS����״̬


	mg.id = 0x18ff6000;
	while((!MSCAN2SendMsg(mg))&&(tt>0))
		tt--;
	  
}
//******************************************************************************
//* Function name:   RecordFaultTemperture
//* Description:     ����״̬���������¶ȡ�ƽ���¶�
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordFaultTemperture(void)//
{
    struct can_msg mg;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= 0x00;
    mg.data[1]= g_storageSysVariableOut[CAUTION_FLAG_1];
    mg.data[2]= g_storageSysVariableOut[CAUTION_FLAG_2];
    mg.data[3]= g_storageSysVariableOut[CAUTION_FLAG_3];
    mg.data[4]= g_storageSysVariableOut[CAUTION_FLAG_4];
    mg.data[5]= g_storageSysVariableOut[CELL_MAX_TEM];//�������¶� 
    mg.data[6]= g_storageSysVariableOut[CELL_MIN_TEM];//�������¶� 
    mg.data[7]= g_storageSysVariableOut[CELL_AVERAGE_TEM];//���ƽ���¶�
    
    mg.id = 0x18ff6001;
	  //MSCAN2SendMsg(mg);
    while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--;
}
//******************************************************************************
//* Function name:   RecordRealtime
//* Description:     ϵͳʱ��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordRealtime(void)//
{
    struct can_msg mg;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    mg.data[0]= g_storageSysVariableOut[SYS_REALTIME_SECOND];
    mg.data[1]= g_storageSysVariableOut[SYS_REALTIME_MINUTE];
    mg.data[2]= g_storageSysVariableOut[SYS_REALTIME_HOUR];
    mg.data[3]= g_storageSysVariableOut[SYS_REALTIME_WEEK];
    mg.data[4]= g_storageSysVariableOut[SYS_REALTIME_DAY];
    mg.data[5]= g_storageSysVariableOut[SYS_REALTIME_MONTH];// 
    mg.data[6]= g_storageSysVariableOut[SYS_REALTIME_YEAR];//
    mg.data[7]= 0x00;//

    mg.id = 0x18ff6002;
	  //MSCAN2SendMsg(mg);
	  while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--;
}
//******************************************************************************
//* Function name:   RecordExtremCellVoltage
//* Description:     ���������͵�ѹ��ƽ����ѹ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordExtremCellVoltage(void)//
{

    struct can_msg mg;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariableOut[CELL_MAX_VOLTAGE]>>8;
    mg.data[1]= g_storageSysVariableOut[CELL_MAX_VOLTAGE];
    mg.data[2]= g_storageSysVariableOut[CELL_MIN_VOLTAGE]>>8;
    mg.data[3]= g_storageSysVariableOut[CELL_MIN_VOLTAGE];
    mg.data[4]= g_storageSysVariableOut[CELL_AVERAGE_VOLTAGE]>>8;
    mg.data[5]= g_storageSysVariableOut[CELL_AVERAGE_VOLTAGE];
    mg.data[6]= 0x00;
    mg.data[7]= 0xC8;//
    
    mg.id = 0x18ff6003;
	  //MSCAN2SendMsg(mg);
	  while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--; 
}
//******************************************************************************
//* Function name:   RecordInsulation
//* Description:     ������Ե����ֵ��1·��ѹ��Ӧ��KֵBֵ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordInsulation(void)//
{

    struct can_msg mg;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariableOut[SYS_INSULATION_P]>>8;
    mg.data[1]= g_storageSysVariableOut[SYS_INSULATION_P];
    mg.data[2]= g_storageSysVariableOut[SYS_INSULATION_N]>>8;
    mg.data[3]= g_storageSysVariableOut[SYS_INSULATION_N];
    mg.data[4]= g_storageSysVariableOut[VOLT_K1]>>8;
    mg.data[5]= g_storageSysVariableOut[VOLT_K1];
    mg.data[6]= g_storageSysVariableOut[VOLT_B1]>>8;
    mg.data[7]= g_storageSysVariableOut[VOLT_B1];//
    
    mg.id = 0x18ff6004;
    while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--;
	  //MSCAN2SendMsg(mg); 
}
//******************************************************************************
//* Function name:   RecordVoltKB
//* Description:     2��3·��ѹ��Ӧ��KֵBֵ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordVoltKB(void) 
{    
    struct can_msg mg;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariableOut[VOLT_K2]>>8;
    mg.data[1]= g_storageSysVariableOut[VOLT_K2];
    mg.data[2]= g_storageSysVariableOut[VOLT_B2]>>8;
    mg.data[3]= g_storageSysVariableOut[VOLT_B2];
    mg.data[4]= g_storageSysVariableOut[VOLT_K3]>>8;
    mg.data[5]= g_storageSysVariableOut[VOLT_K3];
    mg.data[6]= g_storageSysVariableOut[VOLT_B3]>>8;
    mg.data[7]= g_storageSysVariableOut[VOLT_B3];//
    
    mg.id = 0x18ff6005;
	  //MSCAN2SendMsg(mg); 
    while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--;

}
//******************************************************************************
//* Function name:   RecordEndFlag
//* Description:     ���е����ѹ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordCellVoltage(unsigned char number)//
{

    struct can_msg mg;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    /*
    mg.data[0]= g_storageSysVariable[CELL_VOLTAGE_0+4*number]>>8;
    mg.data[1]= g_storageSysVariable[CELL_VOLTAGE_0+4*number];
    mg.data[2]= g_storageSysVariable[CELL_VOLTAGE_1+4*number]>>8;
    mg.data[3]= g_storageSysVariable[CELL_VOLTAGE_1+4*number];
    mg.data[4]= g_storageSysVariable[CELL_VOLTAGE_2+4*number]>>8;
    mg.data[5]= g_storageSysVariable[CELL_VOLTAGE_2+4*number];
    mg.data[6]= g_storageSysVariable[CELL_VOLTAGE_3+4*number]>>8;;
    mg.data[7]= g_storageSysVariable[CELL_VOLTAGE_3+4*number];//
    */
    mg.data[0]= g_storageSysVariableCellOut[CELL_VOLTAGE_0+4*number]>>8;
    mg.data[1]= g_storageSysVariableCellOut[CELL_VOLTAGE_0+4*number];
    mg.data[2]= g_storageSysVariableCellOut[CELL_VOLTAGE_1+4*number]>>8;
    mg.data[3]= g_storageSysVariableCellOut[CELL_VOLTAGE_1+4*number];
    mg.data[4]= g_storageSysVariableCellOut[CELL_VOLTAGE_2+4*number]>>8;
    mg.data[5]= g_storageSysVariableCellOut[CELL_VOLTAGE_2+4*number];
    mg.data[6]= g_storageSysVariableCellOut[CELL_VOLTAGE_3+4*number]>>8;;
    mg.data[7]= g_storageSysVariableCellOut[CELL_VOLTAGE_3+4*number];//
    
    mg.id = 0x18ff6100+number;
	  //MSCAN2SendMsg(mg); 
    while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--;
}
//******************************************************************************
//* Function name:   RecordEndFlag
//* Description:     ��ʷ��¼������ϱ���
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void RecordEndFlag(void)//��ʾ�������
{
    struct can_msg mg;
    unsigned char i=0;
    char tt=100;
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    for(i=0;i<8;i++)
        mg.data[i]= 0xEE;
    
    mg.id = 0x18ff0000;
	  //MSCAN2SendMsg(mg);
	  while((!MSCAN2SendMsg(mg))&&(tt>0))
	      tt--; 
}
//******************************************************************************
//* Function name:   ReadAHRecord
//* Description:     ��ȡ�洢��ʱ��(�����ŵ�)
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void ReadAHRecord(void)//
{

    struct can_msg mg; 
    char i=0,t=100; 
    unsigned long Buffer=0;
    unsigned int  Buffer1=0;
       
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    Buffer=chargeAH;
    //Buffer=0x00123456;
    for(i=2;i>=0;i--)
    {      
        mg.data[i]=Buffer;//[0]~[2]Ϊ�ߵ���
        Buffer=Buffer>>8;
    }
    
    Buffer=dischargeAH;
    //Buffer=0x00654321;
    for(i=5;i>=3;i--)
    {      
        mg.data[i]=Buffer;//[3]~[5]Ϊ�ߵ���
        Buffer=Buffer>>8;
    }
    
    Buffer1=Charge_Times;
    for(i=7;i>=6;i--)
    {      
        mg.data[i]=Buffer1;//[3]~[5]Ϊ�ߵ���
        Buffer1=Buffer1>>8;
    }
        
    mg.id = 0xC0AAA;    
	  while((MSCAN2SendMsg(mg)==FALSE)&&(t>0))
	      t--; 

}
//******************************************************************************
//* Function name:   InTestToPC
//* Description:     ��PC���ز���ģʽ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void InTestToPC(void)//
{

    struct can_msg mg; 
    char i=0,t=100; 
    unsigned long Buffer=0;
       
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]=0x55;
    mg.data[1]=0xaa;
     
    mg.id = 0xC01EE4F;    
	  while((MSCAN2SendMsg(mg)==FALSE)&&(t>0))
	      t--; 

}
//***********************************************************************
//************************************************************************
//*************************the end*************************************
//************************************************************************
