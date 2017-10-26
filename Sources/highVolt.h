/////////////////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////////////////
//BMN TxCAN
//#define  CANID_BJUID     0x18FF0100
//#define  CANID_BJUVPN    0x18FF0200
//#define  CANID_RPTOGND   0x18FF0300
//#define  CANID_RNTOGND   0x18FF0400

#define  CANID_BMNID     0x18FF0100
#define  CANID_BMNVPN    0x18FF0200
#define  CANID_RPNTOGND  0x18FF0300
#define  CANID_BMNLIFE   0x18FF0400

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


/////////////////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////////////////
extern void Task01_BMN_Init(void);
extern void Task02_BMN_GlobalVariables_Init(void);
extern void Task03_BMN_MCU_Init(void);
extern void Task04_BMN_FEED_COP(void);

extern void Task06_BMN_DATA_RECALL(void);
extern void Task07_BMN_DATA_SAVE(void);
extern void Task08_BMN_ReInit(void);
 
extern void Task09_BMN_GlobalVariables_Clear(void);



extern void Task11_Vpn_StartAD_Polling(void);
//extern void Task10_Vpn_StartAD_Polling(void);
extern void Task12_Vpn_Value_Calculate(void);
extern void Task13_Vpn_Value_Calibrate(void);
extern void Task14_R_Vp_GND_Value_Polling(void);
extern void Task15_R_Vn_GND_Value_Polling(void);
//extern void Task16_Rp_Rn_Value_Calculate(byte number);
extern void Task16_Rp_Rn_Value_Calculate();
extern void Task17_Rp_Rn_Value_Calculate_Filter(void);

extern void Task21_CAN_SendGouup1(void);
extern void Task22_CAN_SendGouup2(void);
extern void Task23_CAN_SendGouup3(void);

