
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//---------------------- Pride Power------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//* Project Name       : ����
//* File Name          : SocAdjust.c
//* Author             : judy
//* Version            : V1.0.0
//* Start Date         : 2016.2.14
//* Description        : ���ļ����ڵ����ڲ�ͬ�����SOCֵ
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#include "BMS20.h"
float g_sysPara[ParamNUMBER];
unsigned char g_bmubalance = 0; //0:��� 1:�ŵ�
unsigned long g_BMU_CIRCLE_FLAG =0; //��������BMU����
//******************************************************************************
//* Function name:   ParameterSetting
//* Description:     ͨ����λ������ϵͳ�����������޸�ĳ��ֵ
//* EntryParameter : None
//* ReturnValue    : None
//******************************************************************************
void ParameterSetting(void)//
{      
    unsigned char i=0;
    unsigned char SetTime[7];  
    float CanSOC=0; 
    switch(g_mboxID)//
    {            
        case 0x0c14ee00:
            sendRealtimeToBMU();
            break;
        case 0xC01ee18:
            sendBMSSFVersionToPC();
            break;
        case 0xc01ee14:         //�궨ϵͳʱ��
            //����ʱ��
		        for(i=0;i<7;i++)
		            SetTime[i]=*((&CAN2RXDSR0)+i);//��//��//ʱ//����//��//��//��
		        I2CWriteDate(SetTime);  //���ó�ʼʱ��
            break;
        case 0xC01ee15:
            g_sysPara[PARA_SOC_VALUE]= (float)(*((&CAN2RXDSR0)+2))/100;    // RSOC
            g_socValue = g_sysPara[PARA_SOC_VALUE];
            First_g_socValue = g_socValue;
            
            g_leftEnergy = g_socValue*(g_realNominalCap*3600);  //ʣ������
	          g_energyOfUsed = 0;                          //���ο�����������       
	          StoreParameter();               
	          StoreSocRealvalue();
            break;
         case 0xC01ef15:
                         
	          CanSOC =  (float)(*((&CAN2RXDSR0)+2))/100;
	          Can_g_socValue_Start=CanSOC;
	          CanSocIntegral();
	          StoreSocRealvalue();
            
            break;
        case 0xC01ee16:
            g_sysPara[PARA_WHOLE_CAPACITY]= *((&CAN2RXDSR0)+2);
	          g_realNominalCap =  g_sysPara[PARA_WHOLE_CAPACITY];//����������ʼ��
	          StoreParameter();
	          StoreQ1value();
            break;
        case 0xc01ee17:     //����BMU����
            g_sysPara[PARA_BMU_NUMBER]= *((&CAN2RXDSR0)+2);
            switch((unsigned int)g_sysPara[PARA_BMU_NUMBER])//����BMU�ĸ�����ȷ����Ӧ��������Ϣ���ܹ�֪���Ƿ����еĵ��������Ѿ�����һ��
            {
                case 1:
                  	g_BMU_CIRCLE_FLAG = 0x0000001;
                  	break;
                case 2:
                  	g_BMU_CIRCLE_FLAG = 0x0000011;
                  	break; 
                case 3:
                  	g_BMU_CIRCLE_FLAG = 0x0000111;
                  	break;  
                case 4:
                  	g_BMU_CIRCLE_FLAG = 0x0001111;
                  	break; 
                case 5:
                  	g_BMU_CIRCLE_FLAG = 0x0011111;
                  	break;
                case 6:
                  	g_BMU_CIRCLE_FLAG = 0x0111111;
                  	break;  
                case 7:
                  	g_BMU_CIRCLE_FLAG = 0x1111111;
                  	break;      
                default:
                  	break;
            }
            StoreParameter();
            //paraChangedFlag = 1;
            break;
        case 0xC01EE26://��ȡϵͳ�ڲ�����
            if(*((&CAN0RXDSR0)+0)==0x66)
                //paraReadFlag=1;
            break;            
        case 0xC01EE28:         //����ϵͳ����������Ϣfrom ��λ��
            g_sysPara[PARA_O_WHOLE_VOLTAGE]=  (*((&CAN2RXDSR0)+0)&0x00ff)<<8 | (*((&CAN2RXDSR0)+1)&0x00ff);//�ܵ�ѹ������ֵ
            g_sysPara[PARA_L_WHOLE_VOLTAGE]=  (*((&CAN2RXDSR0)+2)&0x00ff)<<8 | (*((&CAN2RXDSR0)+3)&0x00ff);//�ܵ�ѹ������ֵ
            g_sysPara[PARA_O_CELL_VOLTAGE] = *((&CAN2RXDSR0)+4)*0.01-1.75;//��ѹֵ
            g_sysPara[PARA_L_CELL_VOLTAGE] = *((&CAN2RXDSR0)+5)*0.01-1.75;//Ƿѹֵ
            g_sysPara[PARA_O_TEMPERATURE] =  *((&CAN2RXDSR0)+6)-40; //����ֵ
            g_sysPara[PARA_L_TEMPERATURE] =  *((&CAN2RXDSR0)+7)-40; //�¶ȹ���ֵ                    
            StoreParameter();
            //paraChangedFlag = 1;
            break;
        case 0xC01EE29:         //����ϵͳ����������Ϣfrom ��λ��
            g_sysPara[PARA_HIGHEST_RECHARGE_CURRENT] =  *((&CAN2RXDSR0)+0);//������������ֵ  
            g_sysPara[PARA_O_SOC_VALUE] =  *((&CAN2RXDSR0)+4); //SOC����
            g_sysPara[PARA_L_SOC_VALUE] =  *((&CAN2RXDSR0)+5); //SOC����  
            g_sysPara[PARA_L_ISOLATION_RESISTANCE] =  *((&CAN2RXDSR0)+6); //��Ե�������ֵ
            g_sysPara[PARA_O_CONSISTENCY_V] =  *((&CAN2RXDSR0)+7);//��ѹһ���Բ� 
            StoreParameter();
            //paraChangedFlag = 1;
            break;
        case 0xC01EE2a:         //����ϵͳ����������Ϣfrom ��λ��
            g_sysPara[PARA_BALANCE_ON_VOLTAGE] = *((&CAN2RXDSR0)+0)*0.01-1.75;//���⿪����ѹ
            g_sysPara[PARA_BALANCE_OFF_VOLTAGE] = *((&CAN2RXDSR0)+1)*0.01-1.75;//�����ֹ��ѹ
            g_sysPara[PARA_DIFFERENCE_SINGLE_V] =  *((&CAN2RXDSR0)+2);//����ѹ��
            StoreParameter();
            //paraChangedFlag = 1;
            break;
        case 0xC01EE2b:         //����ϵͳ����������Ϣfrom ��λ��
            break;                      
        default:
            break;                  
      }//end of switch      

}
