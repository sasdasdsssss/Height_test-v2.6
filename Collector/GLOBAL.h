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
#define WM_DRAW_T						WM_USER + 13 //画图消息


#define ABS_(x)                  ((x)>0?(x):-(x))			//  求绝对值的函数

#define ONE_FRAME_LEN					1046

#define	FRONT_							0				// 前雷达
#define LEFT							1				// 左雷达
#define RIGHT							2				// 右雷达
#define CONTROL							4				// 中控
#define SYBSD_L							 6				//商用车左侧
#define SYBSD_R							 7				//商用车右侧

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
	float   Vy;					// (相对纵向速度)				
	float	Rx;					// (横向位置）
	float	Ry;					// (纵向位置）				
	float	Vx;					// (相对横向速度）
	float	Obj_XPos_Stdev;		// (纵向位置标准差)
	float	Obj_YPos_Stdev;		// (横向位置标准差)	
	float	Obj_ObstacleProb;	// (物体成为障碍物的估计概率
	float	Ly_a;				// (相对纵向加速度				
	float	Obj_XVelRel_Stdev;	// (相对纵向速度的标准差估计值 
	float	ProbOfExist;		// (估计物体存在的概率)		    
	uint8	Msg_CheckSum2;		// (CheckSum2
	uint8	nature;				// (对象的估计类型)	
	uint8	Msg_AliveCounter1;	// (活动计数器)					
	uint8	Obj_MeasFlag;		// (预测还是探测值)	
	uint8	Msg_CheckSum1; 		// (CheckSum1						//	
	uint8	Obj_UpdateFlag;		// (新建目标还是已存在目标）
	uint8	Obj_ValidFlag;		// (数据有效性标志)		
	uint8	State;				// (运动类型)
	uint8	Msg_AliveCounter2;	// (活动计数器)			
}TargetI;


typedef struct
{
	uint16_t ID;			  /// 目标ID
	int16_t Ry;				  /// 纵向距离               Factor:0.1｜Offset:0∣Range:0~200.0m
	int16_t Rx;				  /// 横向距离               Factor:0.1｜Offset:0∣Range:-100.0~100.0m
	int16_t Vy;				  /// 纵向相对速度           Factor:0.1｜Offset:0∣Range:-100.0~100.0m/s
	int16_t Vx;				  /// 横向相对速度           Factor:0.1｜Offset:0∣Range:-100.0~100.0m/s
	int16_t Pv;				  /// 目标俯仰角度           Factor:0.1｜Offset:0∣Range:-10.0~10.0°

	int16_t Ph;				  /// 水平方位角             Factor:0.1 ｜Offset:    0 ∣Range:-10.0~10.0°
	int16_t Lx_a;			  /// 横向相对加速度         Factor:0.01｜Offset:    0 ∣Range:-10.00~10.00m/s^2
	int16_t Ly_a;			  /// 纵向相对加速度         Factor:0.01｜Offset:    0 ∣Range:-10.00~10.00m/s^2
	uint8_t UpdateFlag;		  /// 0x0: new object in the cycle;  0x1: object existed in previous cycle

	uint16_t TrackingNum;
	uint8_t RCS;			  ///  RCS值            Factor:1   ｜Offset:    0 ∣Range:0~250
	uint8_t width;			  /// 目标尺寸(宽度)        Factor:0.2 ｜Offset:    0 ∣Range:0-51m
	uint8_t nature;			  /// 目标种类            0x0:未知;0x1:车；0x2:人：0x3~0x7预留
	uint8_t State;			  /// 目标运动状态      0x0:目标静止；0x1:目标运动（相对自车远离）；0x2:目标运动（相对自车靠近）
	uint8_t ProbOfExist;	  /// 目标置信度          Factor:1   ｜Offset:0 ∣ Range:0~100
	uint8_t MeasFlag;		  /*  预测目标还是真实目标          0：表示测量值，1：表示预测值 */
	int16_t V;
}TargetOut1_Type;    /* 53F的  52A的和这个缺少了一个measflag */


typedef struct
{
	int16_t CarSpeed;
	int16_t WheelAngle;
	int16_t CarAccelerat;
}CarInfo_Type;

typedef struct
{
	float  point_r;  //距离
	float  point_v;  //速度
	float  angle;    //角度
	float  amp;      //能量
	float angle_f;                //俯仰角
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
	uint8_t StartFlag;  //  0x00：无  0x01:下线标定高度角  0x02：自动标定高度角  0x03: 下线标定水平角  0x04 : 自动标定水平角  0x10 : 下线标定所有角 0x11:自动标定所有角  0xff 结束标定
	uint8_t ErrorState; //  错误标志   1:等待    2:错误1   3:en错误2  ......
	uint8_t Percentage; //  0~100
	int16_t phase;			//
	int16_t phase_height;	//
	uint8_t tarR;			/* 目标纵距离 */
	int8_t  tarL;			/* 目标横距离 */
	uint8_t tarH;			/* 目标高度 */
	uint8_t sensorH;		/* 安装高度 */
}Calibration_Type;

typedef struct
{
	int16_t car_state;			// 车档位信息
	int16_t Mode;               // 扫频模式
	int16_t SpeedMode;          // 高低速模式
	int16_t thetaerror;         // 水平校正角
	int16_t thetaerrorHigh;     // 高度校正角
	int16_t SelfV;              // 本车速度
	int16_t est_self_speed;		// 估计的本车车速
	int16_t SteerAngle;         // 方向盘转角
	int16_t leftBarLength;      // 左护栏距离
	int16_t rightBarLength;     // 右护栏距离
	Calibration_Type CalibrationInfo;   // 标定参数
	uint8_t BlindFlag;                      // 遮挡标志   0：未遮挡               1：遮挡
	uint8_t micro_doppler_flag;				// 微多普勒标志
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

//////报警信息
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


UINT CameraThread(LPVOID lpParam);		//摄像头线程
UINT My_Thread(LPVOID lpParam);			//网口数据处理线程
UINT CameraDisp(LPVOID lpParam);		//摄像头线程

extern void DSP_mode(/*in*/int16_t* datain,		 // 输入极值点信息头指针
	uint16_t dataNum,			     // 极值点的数目
	uint16_t* p_SweepInfoIn,	     // 雷达扫频参数
	int16_t* dataout_CAN,			     // 输出目标    输出到CAN的目标
	uint16_t* p_doutlen_CAN);

//UINT CameraThread(LPVOID lpParam);		//摄像头线程
#endif