#ifndef _GLOBAL
#define _GLOBAL



#define DEVID_X 14
#define DEVID_Y 18


#define NUM_FRE1	256
#define NUM_FRE2	256
#define NUM_FRE3	128

#define DISTANCE_MAX  50.0
#define DISTANCE_MIN  0.0

#define CROSS_L  -8
#define CROSS_R  8

#define FPS 100

#define DATA_DEEP 300

#define  MAX_RECV_FRAME_NUM  16

#define CAMERA_NUM						1

#define WM_DATA_DEAL					WM_USER + 11
#define WM_MCU_DATA_STORE				WM_USER + 12
#define WM_DRAW_T						WM_USER + 13 //��ͼ��Ϣ


#define ABS_(x)                  ((x)>0?(x):-(x))			//  �����ֵ�ĺ���

#define ONE_FRAME_LEN					1046

#define	FRONT_							0				// ǰ�״�
#define LEFT							1				// ���״�
#define RIGHT							2				// ���״�
#define CONTROL							4				// �п�
#define SYBSD_L							 6				//���ó����
#define SYBSD_R							 7				//���ó��Ҳ�

#define DSIP_TARGETNUM 512
#define CAN_TARGETNUM 32
#define DSIP_TARGETNUM_LIST 32


#define CHART_DRAW 

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef float f32;
typedef double f64;

typedef  short				 int16_t;
typedef  unsigned short		uint16_t;
typedef  int				 int32_t;
typedef  unsigned int		uint32_t;
typedef  unsigned long long	uint64_t;

typedef struct
{
	float	ID0;
	float   Vy;					// (��������ٶ�)				
	float	Rx;					// (����λ�ã�
	float	Ry;					// (����λ�ã�				
	float	Vx;					// (��Ժ����ٶȣ�
	float	Obj_XPos_Stdev;		// (����λ�ñ�׼��)
	float	Obj_YPos_Stdev;		// (����λ�ñ�׼��)	
	float	Obj_ObstacleProb;	// (�����Ϊ�ϰ���Ĺ��Ƹ���
	float	Ly_a;				// (���������ٶ�				
	float	Obj_XVelRel_Stdev;	// (��������ٶȵı�׼�����ֵ 
	float	ProbOfExist;		// (����������ڵĸ���)		    
	uint8	Msg_CheckSum2;		// (CheckSum2
	uint8	nature;				// (����Ĺ�������)	
	uint8	Msg_AliveCounter1;	// (�������)					
	uint8	Obj_MeasFlag;		// (Ԥ�⻹��̽��ֵ)	
	uint8	Msg_CheckSum1; 		// (CheckSum1						//	
	uint8	Obj_UpdateFlag;		// (�½�Ŀ�껹���Ѵ���Ŀ�꣩
	uint8	Obj_ValidFlag;		// (������Ч�Ա�־)		
	uint8	State;				// (�˶�����)
	uint8	Msg_AliveCounter2;	// (�������)			
}TargetI;


typedef struct
{
	uint16_t ID;			  /// Ŀ��ID
	int16_t Ry;				  /// �������               Factor:0.1��Offset:0�ORange:0~200.0m
	int16_t Rx;				  /// �������               Factor:0.1��Offset:0�ORange:-100.0~100.0m
	int16_t Vy;				  /// ��������ٶ�           Factor:0.1��Offset:0�ORange:-100.0~100.0m/s
	int16_t Vx;				  /// ��������ٶ�           Factor:0.1��Offset:0�ORange:-100.0~100.0m/s
	int16_t Pv;				  /// Ŀ�긩���Ƕ�           Factor:0.1��Offset:0�ORange:-10.0~10.0��

	int16_t Ph;				  /// ˮƽ��λ��             Factor:0.1 ��Offset:    0 �ORange:-10.0~10.0��
	int16_t Lx_a;			  /// ������Լ��ٶ�         Factor:0.01��Offset:    0 �ORange:-10.00~10.00m/s^2
	int16_t Ly_a;			  /// ������Լ��ٶ�         Factor:0.01��Offset:    0 �ORange:-10.00~10.00m/s^2
	uint8_t UpdateFlag;		  /// 0x0: new object in the cycle;  0x1: object existed in previous cycle

	uint16_t TrackingNum;
	uint8_t RCS;			  ///  RCSֵ            Factor:1   ��Offset:    0 �ORange:0~250
	uint8_t width;			  /// Ŀ��ߴ�(���)        Factor:0.2 ��Offset:    0 �ORange:0-51m
	uint8_t nature;			  /// Ŀ������            0x0:δ֪;0x1:����0x2:�ˣ�0x3~0x7Ԥ��
	uint8_t State;			  /// Ŀ���˶�״̬      0x0:Ŀ�꾲ֹ��0x1:Ŀ���˶�������Գ�Զ�룩��0x2:Ŀ���˶�������Գ�������
	uint8_t ProbOfExist;	  /// Ŀ�����Ŷ�          Factor:1   ��Offset:0 �O Range:0~100
	uint8_t MeasFlag;		  /*  Ԥ��Ŀ�껹����ʵĿ��          0����ʾ����ֵ��1����ʾԤ��ֵ */
	int16_t V;
}TargetOut1_Type;    /* 53F��  52A�ĺ����ȱ����һ��measflag */


typedef struct
{
	int16_t CarSpeed;
	int16_t WheelAngle;
	int16_t CarAccelerat;
}CarInfo_Type;

typedef struct
{
	float  point_r;  //����
	float  point_v;  //�ٶ�
	float  angle;    //�Ƕ�
	float  amp;      //����
	float angle_f;                //������
	float  SNR;
}OutPeaks_Type;

typedef struct
{
	int64_t  Distance : 11;
	int64_t  Velocity : 11;
	int64_t  Angle : 11;
	int64_t  CrossDistance : 11;
	int64_t  AccOrSpeedcross : 8;
	uint64_t Id : 12;
}TargetCanCell;

typedef struct
{
	uint8_t StartFlag;  //  0x00����  0x01:���߱궨�߶Ƚ�  0x02���Զ��궨�߶Ƚ�  0x03: ���߱궨ˮƽ��  0x04 : �Զ��궨ˮƽ��  0x10 : ���߱궨���н� 0x11:�Զ��궨���н�  0xff �����궨
	uint8_t ErrorState; //  �����־   1:�ȴ�    2:����1   3:en����2  ......
	uint8_t Percentage; //  0~100
	int16_t phase;			//
	int16_t phase_height;	//
	uint8_t tarR;			/* Ŀ���ݾ��� */
	int8_t  tarL;			/* Ŀ������ */
	uint8_t tarH;			/* Ŀ��߶� */
	uint8_t sensorH;		/* ��װ�߶� */
}Calibration_Type;

typedef struct
{
	int16_t car_state;			// ����λ��Ϣ
	int16_t Mode;               // ɨƵģʽ
	int16_t SpeedMode;          // �ߵ���ģʽ
	int16_t thetaerror;         // ˮƽУ����
	int16_t thetaerrorHigh;     // �߶�У����
	int16_t SelfV;              // �����ٶ�
	int16_t est_self_speed;		// ���Ƶı�������
	int16_t SteerAngle;         // ������ת��
	int16_t leftBarLength;      // ��������
	int16_t rightBarLength;     // �һ�������
	Calibration_Type CalibrationInfo;   // �궨����
	uint8_t BlindFlag;                      // �ڵ���־   0��δ�ڵ�               1���ڵ�
	uint8_t micro_doppler_flag;				// ΢�����ձ�־
}SweepInfo1_Type;


typedef struct
{
	unsigned short ID;
	unsigned char Data[8];
}CAN_Data_Type;

typedef struct
{
	short Speed;
	short WheelCircle;
	unsigned char gear : 2;
	unsigned char leftLampState : 1;
	unsigned char rightLampState : 1;
	unsigned char driveDoorState : 1;
	unsigned char passengerDoorState : 1;
	unsigned char leftDoorState : 1;
	unsigned char rightDoorState : 1;
	unsigned char ACCState : 1;
	unsigned char brakeState : 1;
	unsigned char handBrakeState : 1;
	unsigned char frontLampState : 1;
	unsigned char lockState : 2;
	unsigned char frontAlarm : 2;
	unsigned char LeftLCAAlarm : 2;
	unsigned char LeftDOWAlarm : 2;
	unsigned char LeftRCWAlarm : 2;
	unsigned char LeftRCTAAlarm : 2;
	unsigned char RightLCAAlarm : 2;
	unsigned char RightDOWAlarm : 2;
	unsigned char RightRCWAlarm : 2;
	unsigned char RightRCTAAlarm : 2;

}CarState_Type;

//////������Ϣ
typedef struct {
	unsigned char RecieveCircleState : 1;
	unsigned char StateFreState : 1;
	unsigned char CardIDState : 1;
	unsigned char SendPowerState : 1;
	unsigned char FRTempState : 1;
	unsigned char RadarShelterState : 1;
	unsigned char flagnull1 : 1;
	unsigned char flagnull2 : 1;
}Bits;

typedef struct {
	char LCA_Alarm_Grade;
	char LCA_Alarm_Type;
	char DOW_Alarm_Grade;
	char RCW_Alarm_Grade;
	char RCTA_Alarm_Grade;
	Bits RadarState;
}Alarm_Type;


UINT CameraThread(LPVOID lpParam);		//����ͷ�߳�
UINT My_Thread(LPVOID lpParam);			//�������ݴ����߳�
UINT CameraDisp(LPVOID lpParam);		//����ͷ�߳�

extern void DSP_mode(/*in*/int16_t* datain,		 // ���뼫ֵ����Ϣͷָ��
	uint16_t dataNum,			     // ��ֵ�����Ŀ
	uint16_t* p_SweepInfoIn,	     // �״�ɨƵ����
	int16_t* dataout_CAN,			     // ���Ŀ��    �����CAN��Ŀ��
	uint16_t* p_doutlen_CAN);

//UINT CameraThread(LPVOID lpParam);		//����ͷ�߳�
#endif