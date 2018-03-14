//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : 
//* File Name          : main.C
//* Author             : 孙丽雪
//* Software Version   : B00F.1704.010
//* Start Date         : 2017年4月6日                                   
//* Description        : BMS主板程序
//* Processor:         : Freescale MC2S12XEP100
//* Compiler:          : CodeWarriorV5.0 or higher
//* Details:           : S277(6130)5个BMU板，单体总个数180，电芯类型为磷酸铁锂电池，240AH 138.2度电
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
 
#include "BMS20.h"

/* global variables definitions */
//BMS Software Version define
unsigned char BMS_SW_Version[8]={0x0b,0x01,0x16,0x11,0x09,0x00,0x01,0x01};//b116.0830.011 //s278 project
unsigned char BMUOK=0;
unsigned char HeatFlag; //上电前温度标志位
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
void main(void) 
{
	IVBR = 0x7f;  //向量表重新定位因为之前的空间被BOOTLOADER程序占用了  
	
	CLKSEL = 0x80;//PLLCLK = 1 选则PLL作为时钟  bus clock = pll clock/2 ,,16M晶振，BUSCLOCK = 16M
	while((CLKSEL & 0x80) == 0);
	
	COPCTL |=0x07; //看门狗初始化，分频时钟为2的24次方; 大约2秒钟
	
	InitialHc595(); //Relay初始化 
	gpioInit(); //IO port initialize
	ADCInitial(); //ADC初始化
	ads1015_init();
	read_adc_config();
	MSCANInit();  //CAN初始化
	PIT0_Init();
	RTIInit();  //设置定时器间隔时间
	InitialAtM95M02();//M95M02 Flash初始化
	DFlash_Init(); //Data Flash初始化
	Task02_BMN_GlobalVariables_Init();
	Task11_Vpn_StartAD_Polling(); //计算总电压值；此处不能删除，用于激活ADS1110
	
	delay(65000);  //19ms
	delay(65000); //cant delete
	
	InitialBMS();   //BCU初始化，包括参数，SOC，时间的初始化
	YoungMan_LT_initialize();//初始化CC2
	
	turnOnLED0(); 	     // for debug
		
	Interrupt_Priority_Set();
	EnableInterrupts; //开中断，接收状态机编码
	TurnOnCan2Rx(); //打开内部中断，接收BMU数据
	
	BMU_initial();//////BMU自检
	GetDCTem();/////此处调用是充电初始时检测插座温度异常和CC2电压用
	bmsModeAwake();//系统模式判断
    DCChrgTime_initialize();// charge left time calculate initialize.
	
    if(g_highestTemperature>(45+40)){
        Tavg=g_highestTemperature;
    }
    else{
        Tavg=g_lowestTemperature;
    }
	
    if(Tavg<=40)        //小于0度
    {
        HeatFlag=1;
    }
    else if (Tavg<=50)  //0-10度
    {
        HeatFlag=2;
    }
    else                //大于10度
        HeatFlag=3;
    
	BMUOK = 1;
	//****************************************************************               
	InitialSoc();   //SOC，初始容量，剩余容量初始化，必须要在BMU数据都收到后才读取，否则温度不对，额定容量计算会错       
	SocOCVAdjust(30); //SOC OCV修正120min
    
    //*******************************************************************
    for(;;)
    {
		_FEED_COP();   //2s内不喂狗，则系统复位
		TaskProcess(); 
    }
	/* please make sure that you never leave main */
}


