
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : S133
//* File Name          : DC_ChangerProcess.h
//* Author             : Judy
//* Version            : V1.0.0
//* Start Date         : 2015.7.13
//* Description        : ���ļ��Ƕ�CAN���͵ı��Ľ���BIT����
//----------------------------------------------------------------------------------------------------

extern unsigned char  plug_DC_Connect;

extern unsigned char	m_chmmode;
extern unsigned char fastendflag;

extern float	m_askvoltage;
extern float	m_askcurrent;






extern unsigned char  fastend1;
extern unsigned char  fastend2;
extern unsigned char  fastend3;
extern unsigned char  fastend4;
extern unsigned char  fasterror11;
extern unsigned char  fasterror12;



extern unsigned char BROStopState;     //BRO���յ�CRO��ֹͣ����

extern unsigned int  chargeTimes;

extern unsigned int CRMOverTimeBefore60s;//�ղ���CRO����60s��ϵͳ������
extern unsigned int CRMOverTimeBefore;//���ղ���CRM��ʱ
extern unsigned int CRMOverTime;//CRM��ʱ
extern unsigned int CROOverTime; //5sδ�յ�CRO 
extern unsigned int CROOverTime60s;//60sCRO��ʱ����0xaa��ʱ
extern unsigned int CCSOverTime;      //��䳬ʱ�ж�
extern unsigned int CSTOverTime;
extern unsigned int CSDOverTime;      //CSD��ʱʱ��
extern unsigned char OverTimeState;

extern unsigned char CC2ResState;
extern float DCTem1;
extern float DCTem2;
extern float CC2VOL;
extern float PowerVOL;
//extern unsigned char DC_CC2Count;

extern unsigned char BROErrorAA;//BRO������0xaa
extern unsigned char DC_Vesion;//DC���汾1���Ϲ���;2���¹���

extern unsigned char DCStartState;
extern unsigned char SelfState3;     //�Լ���������3��־
extern unsigned char BEMStop;
extern unsigned char ChargerStopState;
extern unsigned char PantNormalEndFlag;//�ܵ繭�����µ��־λ
extern unsigned char g_received_CST;
extern unsigned char g_BST_send_ok;

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
extern float GetRequestCurrentDC(void);
extern void TaskRechargeDC(void);
extern void GetDCTem (void);
extern void TaskDC(void);

//***************************************************************************
