//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : ����
//* File Name          : InitBMS.h
//* Author             : Kxy
//* Version            : V1.0.0
//* Start Date         : 2016,07,12
//* Description        : ���ļ���Ϊϵͳͷ�ļ����Ը���Ŀ��ϵͳ�������ж���Ͷ�����ȫ�ֱ�������ȫ������
//----------------------------------------------------------------------------------------------------
#define ParamNUMBER 38
extern unsigned char g_BmsModeFlag; //3:�����2:������ 1:�ŵ�
extern unsigned char OCVState;//0:OCVû�з���,1��OCV�Ѿ�����
extern void delay(unsigned int t); 
extern void parametersClean(void);
extern float g_defaultSysParameters[ParamNUMBER];
extern float g_sysPara[ParamNUMBER];

extern void InitialBMS(void);
extern void InitialSoc(void);
extern float ReadEndFlag(void);
extern void InitDefaultParam(void); 
extern void Interrupt_Priority_Set(void);