//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : Record.c
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2014.5.7
//* Description        : ���ļ�ר��������ʷ��¼�����ֹ��ܣ�ÿ���Ӵ洢һ�������Լ��й��Ϸ���ʱ��ʱ�洢
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#include "derivative.h" /* include peripheral declarations */
#include "BMS20.h" 

//*************************************************************************************************
unsigned int g_storageSysVariableOut[STORE_NUMBER]= 
{
    0,  //��¼����
    0,  //ϵͳ�ܵ�ѹ
    0,  //ϵͳ�ܵ���
    1.0,  //SOC��ʾֵ
    0,   //�̵���״̬
    0.0,   //���ϵȼ�
    0,    //������Ϣ1
    0,   //������Ϣ2
    0,   //������Ϣ3
    0,   //������Ϣ4
    0,//second        10
    0,//minute////////11
    0,//hour
    0,//week
    0,//day
    0,//month
    0, //year
    0,//��ߵ����¶�
    0,//��͵����¶�
    0,//ƽ���¶�
    0, //��ߵ����ѹ
    0, //��͵����ѹ
    0, //ƽ����ѹ
    1000,//��Ե��ֵ��
    1000,//��Ե��ֵ��
    198,
    4000,
    198,
    4000,
    198,
    4000,
    0   //У��
};
unsigned char g_storageSysVariableT[36];
/*unsigned int g_storageSysVariableCell[200]= //�洢200�������ѹ��
{  
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0
};*/
/*unsigned int g_storageSysVariableCellOut[200]= //����200�������ѹ��
{  
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0
};*/

unsigned int g_storageSysVariableCell[36]= //�洢200�������ѹ��
{  
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0
};
unsigned int g_storageSysVariableCellOut[36]= //����200�������ѹ��
{  
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0
};

unsigned int g_storageSysVariable[120];
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//************************************************************************
//******************************************************************************
//* Function name:   ReadErrorIndex
//* Description:     ������ʷ����������
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
unsigned int ReadErrorIndex(void) 
{
    unsigned int counter=0;   
    unsigned int cc[1];
    
    Read24c64_Byte(ERROR_COUNT_ADDR,(unsigned char* )cc,2); //��ȡ��ǰ��ʷ��¼������  
    return ((unsigned int)cc[0]);   
}
//******************************************************************************
//* Function name:   storeErrorIndex
//* Description:     �洢��ʷ��¼������
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void storeErrorIndex(unsigned int index) 
{
    unsigned int counter[1];
    
    counter[0]= index;
    Write24c64_Byte(ERROR_COUNT_ADDR,(unsigned char *)&counter,2);//���¹��ϼ���� 
}
//******************************************************************************
//* Function name:   StoreSysVariable
//* Description:     �洢��ʷ��¼��Ϣ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void StoreSysVariable(void)  //�洢������Ϣ��SOCֵ����ַ��at45db161 �� 0x3ff ��ʼ
{
	unsigned char i=0,j=0; 
	unsigned int buffer1[STORE_NUMBER],buffer2_1[128],buffer2_2[72];//,test_1[128],test_2[72],test1[32]
	unsigned int check=0;   //У���

	g_errorCounter++;
	if(g_errorCounter>=500){  //����500����ѭ���洢
		g_errorCounter = 0;
	}
	
    storeErrorIndex(g_errorCounter);//���¹��ϼ����� 
	
    g_storageSysVariable[INDEX]= g_errorCounter;
    g_storageSysVariable[TOTAL_VOLTAGE]			= (unsigned int)g_highVoltageV1;   
    g_storageSysVariable[TOTAL_CURRENT]			= (unsigned int)((g_systemCurrent+400)*50);
    g_storageSysVariable[PARA_SOC_DISPLAY]		= (unsigned int)(g_socValue*100*2.5);
    g_storageSysVariable[SYS_CONTACTORS_STATE]	= BmsCtlStat0 & 0xff;//BMS����״̬ 
    
    //������Ϣ�ڹ��ϴ��������
    //ʵʱʱ��
    for(j=SYS_REALTIME_SECOND;j<SYS_REALTIME_YEAR+1;j++){
        g_storageSysVariable[j]= CurrentTime[j-10];
    }
	
	g_storageSysVariable[CELL_MAX_TEM]			= (unsigned int)((g_bms_msg.CellTempMax-40)+48);
	g_storageSysVariable[CELL_MIN_TEM]			= (unsigned int)((g_bms_msg.CellTempMin-40)+48);
	g_storageSysVariable[CELL_AVERAGE_TEM]		= (unsigned int)((g_averageTemperature-40)+48);
	g_storageSysVariable[CELL_MAX_VOLTAGE]		= (unsigned int)(g_bms_msg.CellVoltageMax);
	g_storageSysVariable[CELL_MIN_VOLTAGE]		= (unsigned int)(g_bms_msg.CellVoltageMin);
	g_storageSysVariable[CELL_AVERAGE_VOLTAGE]	= (unsigned int)(g_averageVoltage*10000); 
	g_storageSysVariable[SYS_INSULATION_P]		= Rp_Vpn_Value;
	g_storageSysVariable[SYS_INSULATION_N]		= Rn_Vpn_Value;
	
    //�����ѹֵ��BMU���ݴ�����
    //����AD45DB161
    //���´洢������Ϊ����256�������Էֳ������������档 Wrt_Flash��������ڲ�����unsigned char�͵ġ��ĳ�unsigned int�͵���ʱ�������
    //������ֿ�ҳ�������⡣�������ԣ�ÿ����¼ռһ��ҳ��528�ֽ�һҳ��      
    for(i=0;i<=35;i++) 
    {    
        buffer2_1[i] = g_storageSysVariableCell[i];//��¼���е����ѹ��ֵ
        check += buffer2_1[i];
    }
    M95_Write_Memory(256*(unsigned long)2*g_errorCounter,(unsigned char* )buffer2_1,36*2);//�ӵ�1ҳ��ʼ����ַ��0��ʼ,0~255,��18����ߵ����ѹ��18����͵����ѹ, 18*2bytes*2, max and min
    
    //for test
    /*
    DisableInterrupts;
    M95_Read_Memory(256*(unsigned long)2*g_errorCounter,(unsigned char* )test_1,256);//�ӵ�1ҳ��ʼ����ַ��0��ʼ,0~255,��128�������ѹ  
    EnableInterrupts; //���жϣ
    */
    //end of test
    
//    for(i=128;i<200;i++)
//    {
//		buffer2_2[i-128] = 0;   //fill 0 for unused area. just check calculate, project name S278.

//		check += buffer2_2[i-128];  
//    }
//      
//    M95_Write_Memory(256*(unsigned long)(2*g_errorCounter+1),buffer2_2,72*2);//�ӵ�1ҳ��ʼ����ַ��256��ʼ��256~399,������72����ѹ
    
    //for test
    /*
    DisableInterrupts;
    M95_Read_Memory(256*(unsigned long)(2*g_errorCounter+1),(unsigned char* )test_2,72*2);//�ӵ�1ҳ��ʼ����ַ��0��ʼ,0~255,��128�������ѹ  
    EnableInterrupts; //���ж�
    */
    //end of test
    
    //�ȴ���ҳ���ٴ�ǰ��ҳ����ΪУ�����ǰ��ҳ��
    for(i=0;i<=SYS_INSULATION_N;i++) 
    {
        buffer1[i]= g_storageSysVariable[i];//��¼64λ�����ֽ� 
        check += buffer1[i];
    }
    //***���ӱ���V1��V2��V3��K,Bֵ��������ΪBֵЩֵ��int���ͣ�����Ҫ�ò�����б���
   
	buffer1[VOLT_K1] =Vpn_K1;
	check += buffer1[VOLT_K1];
	
	buffer1[VOLT_B1] =(unsigned int)(32767-Vpn_B1);
	check += buffer1[VOLT_B1];
	
	buffer1[VOLT_K2] =Vpn_K2;
	check += buffer1[VOLT_K2];
	
	buffer1[VOLT_B2] =(unsigned int)(32767-Vpn_B2);
	check += buffer1[VOLT_B2];
	
	buffer1[VOLT_K3] =Vpn_K3;
	check += buffer1[VOLT_K3];
	
	buffer1[VOLT_B3] =(unsigned int)(32767-Vpn_B3);
	check += buffer1[VOLT_B3];
           
    buffer1[VERIFICATION] = check;
    
    M95_Write_Memory(256*(unsigned long)(2*g_errorCounter+1)+72*2,(unsigned char*)buffer1,STORE_NUMBER*2);//�ӵ�1ҳ��ʼ����ַ��400��ʼ��463�洢64����������
    
    //for test
    /*
    DisableInterrupts; //
        M95_Read_Memory(256*(unsigned long)(2*g_errorCounter+1)+72*2,test1,STORE_NUMBER*2);
    EnableInterrupts;
    */
    //for test
}
//******************************************************************************
//* Function name:   ReadOutErrorRecord
//* Description:     ������ʷ��¼��Ϣ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
 
unsigned char ReadOutErrorRecord(unsigned int counter) //   1:У�鲻ͨ��
{
	unsigned int i=0;
	unsigned int check=0;
	unsigned int verification=0;
	unsigned int buffer1[STORE_NUMBER],buffer2_1[128],buffer2_2[72];

	DisableInterrupts; //
        M95_Read_Memory(256*(unsigned long)2*counter,(unsigned char* )buffer2_1,256);//�ӵ�1ҳ��ʼ����ַ��0��ʼ,0~255,��128�������ѹ  
	EnableInterrupts; //���ж�

	for(i=0;i<36;i++) 
	{      
         check += buffer2_1[i];
    }


//    DisableInterrupts; //
//        M95_Read_Memory(256*((unsigned long)2*counter+1),(unsigned char* )buffer2_2,72*2);//�ӵ�1ҳ��ʼ����ַ��0��ʼ,0~255,��128�������ѹ  
//    EnableInterrupts; //���ж�  
//    for(i=0;i<72;i++) 
//    {      
//         check += buffer2_2[i];
//    }
    
    
    DisableInterrupts; //
        M95_Read_Memory((256*(unsigned long)(2*counter+1)+72*2),(unsigned char* )buffer1,STORE_NUMBER*2);
    EnableInterrupts; //���ж�
    
    for(i=0;i<=VOLT_B3;i++) 
    {      
        check += buffer1[i];
    }
    verification =  buffer1[VERIFICATION];

    if(verification == check)
    {
		for(i=0;i<STORE_NUMBER;i++) 
		    g_storageSysVariableOut[i] = buffer1[i]; //�������� 32
//		for(i=0;i<128;i++) 
//		    g_storageSysVariableCellOut[i]= buffer2_1[i];  //�����ѹ 
//		for(i=128;i<200;i++)
//		    g_storageSysVariableCellOut[i]= buffer2_2[i-128];  //�����ѹ     

//		for(i=128;i<200;i++){
//			g_storageSysVariableCellOut[i]= buffer2_2[i-128];  //�����ѹ	  
//		}

		for(i=0;i<36;i++){
			g_storageSysVariableCellOut[i]= buffer2_1[i];  //�����ѹ 
		}
		return 0;
    }
    else if(counter > g_errorCounter) 
    {
        for(i=0;i<STORE_NUMBER;i++) 
            g_storageSysVariableOut[i] = 0;
//        for(i=0;i<200;i++)
//            g_storageSysVariableCellOut[i]=0;   
        for(i=0;i<36;i++)
            g_storageSysVariableCellOut[i]=0;   
        return 0; 
    }
    return 1;                   
}
//******************************************************************************
//* Function name:   TaskRecordProcess
//* Description:     ��ʷ��¼����������̣���ȡ2000��
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void TaskRecordProcess(void)
{
	int i,j;

	if(!g_errorRecordRead_flag){ 
		return;
	}
	  
	DisableInterrupts; //�Ƿ��ȡ������Ϣ������ȡ������Ϣʱ�������ж�ֹͣ
	for(i=499;i>=0;i--) 
	{	    
		_FEED_COP();   //2s�ڲ�ι�ڹ�����ϵͳ��λ
		//PORTB_PB4 ^=1; //feed the hardware dog
		if(1==ReadOutErrorRecord(i))//���У�鲻ͨ���������¶�ȡ	 
		{
			ReadOutErrorRecord(i);
		}
		
		delay(1000);
		delay(1000);      
		RecordSystem();
		delay(1000);
		delay(1000);
		RecordFaultTemperture();
		delay(1000);
		delay(1000);
		RecordRealtime();
		delay(1000);
		delay(1000);
		RecordExtremCellVoltage();
		delay(1000);
		delay(1000);
		RecordInsulation();
		delay(1000);
		delay(1000);
		RecordVoltKB();
		delay(1000);
		delay(1000);
		for(j=0;j<50;j++) 
		{	        
			RecordCellVoltage(j);//0~4*49+3=199
			delay(1000);
			delay(1000);   //�˴�������ʱ�㹻
			delay(1000);
			delay(1000);
		}
    }
	
	delay(1000);
	delay(1000);
	RecordEndFlag();//end	 

	g_errorRecordRead_flag=0;	
	EnableInterrupts;	
}
//******************************************************************************
//* Function name:   parametersClean
//* Description:     ������������
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void parametersClean(void)
{
	unsigned int i,k;//,j

	for(i=0;i<BMU_NUMBER;i++){
		for(k=0;k<CELL_NUMBER;k++){
			g_bmu_msg.cell_V[i][k] = 0;
		}
		for(k=0;k<Tem_NUMBER;k++){
			g_bmu_msg.cell_T[i][k] = 0;
		}
	}
	
//	g_highestCellVoltage = 0;
//	g_lowestCellVoltage = 0;
	g_bms_msg.CellVoltageMax = 0;
	g_bms_msg.CellVoltageMin = 0;
	g_averageVoltage = 0; 

	for(i=0;i<STORE_NUMBER;i++){
        g_storageSysVariableOut[i] = 0;
	}

	VCU_ChgControl_2.Bit.rise_Eleband_No_OK = 1;
	VCU_ChgControl.Bit.rise_Eleband_Switch = 0;
	VCU_ChgControl.Bit.downC_OK = 1;
}
//********************************************************************************************
//***********************************the end*************************************************
//********************************************************************************************
//********************************************************************************************
