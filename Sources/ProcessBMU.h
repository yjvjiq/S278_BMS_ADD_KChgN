//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : ProcessBMU.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2015.7.13
//* Description        : ���ļ���BMU������ͷ�ļ�
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
#ifndef _PROCESSBMU_H_
#define _PROCESSBMU_H_

#define LTC6802_NUM			5   // ÿ��BMU��6802�ĸ���
#define CELL_NUMBER         36  //ÿ��6802��Ӧ�ĵ������
#define Tem_NUMBER          3   //ÿ��6802��Ӧ���¶ȸ���
#define SLAVE_NUMBER_BEIQI  8   //����Ҫ��ĴӰ����
///////////////////////////////////////////////////////////////////////////////////////////
//************ ProcessBMU.c ****************************************
extern float g_highestCellVoltage;
extern float g_lowestCellVoltage;
extern float g_averageVoltage; //ƽ�������ѹ
extern float g_systemVoltage;      //ϵͳ��ѹ=�����ѹ�ۼ��ܵ�ѹ
extern unsigned char g_highestTemperature;
extern unsigned char g_lowestTemperature;
extern unsigned char g_averageTemperature;
extern unsigned char Tavg;

extern unsigned char g_bmu2_number_v[BMU_NUMBER];
extern unsigned char g_cell_number_v[BMU_NUMBER][5];
extern unsigned char g_bmu2_number_t[BMU_NUMBER];
extern unsigned char g_cell_number_t[BMU_NUMBER][5];

extern unsigned long g_circleFlag; //һ��ѭ����ɵı�־
extern unsigned long g_configFlag;//�յ�������Ϣ��־�������ж�BMU������6805�����͵������

extern void BMU_initial(void);
extern void BMU_Processure(void);
extern unsigned char bmuProcess2(void);
//************************************************************************
//************************************************************************
//*************************the end****************************************
//************************************************************************
//************************************************************************
#endif