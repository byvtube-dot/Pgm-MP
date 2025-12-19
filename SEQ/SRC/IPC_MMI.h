#pragma once
#define _USE_MATH_DEFINES
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <stdio.h>
#include <shlwapi.h>
#include <time.h>
#include <mmsystem.h>
#include <cassert>
#include <queue>
#include <math.h>

using namespace std;

#define MAX_MOTOR						50
#define MAX_INDEX_MOTOR					50
#define MAX_INDEX_CYLINDER				10
#define MAX_INDEX_VACUUM				10
#define MAX_INDEX_CONVEYOR				10
#define MAX_IO_CHANNEL					50
#define MAX_DATA_PARAM					1000
#define MAX_ERR_WR						10
#define MAX_SCOPE_ACTUATOR				10
#define MAX_SCOPE_CHAR					256
#define MAX_SCOPE_DATA					10000
#define MAX_MSG_TITLE_CHAR				200
#define MAX_MSG_NOTICE					4
#define MAX_MSG_BUTTON_CHAR				10
#define MAX_UPDATE_MMI					100
#define MAX_VIDATA_COUNT				100
#define MAX_VIRESULT_COUNT				10000
#define MAX_BARCODE_RESULT_CHAR			128
#define MAX_VIUNIT_COUNT				50
#define MAX_VIUNIT_CHAR					128

namespace IPC_MMI
{
	enum
	{
		PRESSURE_POSITIVE	= 1,
		PRESSURE_NEGATIVE	= 2,
	};

	enum
	{
		MAX_SEND_SCAN_COORD	= 500,
		MAX_UNIT			= 10,
		MAX_FIDUCIAL		= 4,
		MAX_STEP			= 50,
	};

#pragma pack(push, 1)
	typedef struct _MtControl
	{
		int		nMtNo;
		int		nCmd;
		int		nCmdIndexNo;
		int		nDir;
		double	dPulse;
	}MtCmd;

	typedef struct _MtTable
	{
		int		nMtNo;
		double	dPos[MAX_INDEX_MOTOR];
		double	dVel[MAX_INDEX_MOTOR];
		double	dAcc[MAX_INDEX_MOTOR];
	}MtTable;

	typedef struct _MtStatus
	{
		int		nMtNo;
		BOOL	bServoOn;
		BOOL	bAlarm;
		BOOL	bDriving;
		BOOL	bPaused;
		BOOL	bHome;
		BOOL    bHoming;
		BOOL	bCw;
		BOOL	bCCw;
		BOOL	bOrg;
		BOOL	bBrakeOn;
		int		nCurIndex;
		double	dRealCnt;
	}MtStatus;

	typedef struct _ReadIO
	{
		WORD inCH[MAX_IO_CHANNEL];
		WORD outCH[MAX_IO_CHANNEL];
	}ReadIO;

	typedef struct _WriteIO
	{
		int  nIONo;
		BOOL bOn;
	}WriteIO;

	typedef struct _IntData
	{
		int nStart;
		int nEnd;
		int nVal[MAX_DATA_PARAM];
	}IntData;

	typedef struct _DoubleData
	{
		int nStart;
		int nEnd;
		double dVal[MAX_DATA_PARAM];
	}DoubleData;

	typedef struct _ErrWar
	{
		int err[MAX_ERR_WR];
		int wr[MAX_ERR_WR];
	}ErrWr;

	typedef struct _VacControl
	{
		INT32	actuatorNo;
		INT32	indexNo;
		INT32	setOn;			// (1:ON),(0:OFF)
		INT32	setOff;			// (1:ON),(0:OFF)
		INT32	setBlow;		// (1:ON),(0:OFF)
	}VacCmd;

	typedef struct _VacState
	{
		INT32	iHighNo;		// Display when defined
		INT32	iLowNo;			// Display when defined
		INT32	oOnNo;			// Display when defined
		INT32	oOffNo;			// Display when defined
		INT32	oBlowNo;		// Display when defined
		INT32	pressureType;	// (1:+),(2:-)
		INT32	highOn;			// (1:ON),(0:OFF)
		INT32	lowOn;			// (1:ON),(0:OFF)
		INT32	blowOn;			// (1:ON),(0:OFF)
		INT32	outOn;			// (1:ON),(0:OFF)
		INT32	outOff;			// (1:ON),(0:OFF)
		INT32	outBlow;		// (1:ON),(0:OFF)
		double	value;
		double	elapsedTimeOn;
		double	elapsedTimeOff;
	}VacState;

	typedef struct _VacTable
	{
		INT32	actuatorNo;
		double	valueHighOn[MAX_INDEX_VACUUM];
		double	valueHighOff[MAX_INDEX_VACUUM];
		double	valueLowOn[MAX_INDEX_VACUUM];
		double	valueLowOff[MAX_INDEX_VACUUM];
		double	valueBlowOn[MAX_INDEX_VACUUM];
		INT32	delayTimeOn[MAX_INDEX_VACUUM];
		INT32	delayTimeOff[MAX_INDEX_VACUUM];
		INT32	delayTimeBlow[MAX_INDEX_VACUUM];
	}VacTable;

	/** FDC VACUUM DATA **/
	typedef struct _VacFdcData
	{
		bool	use;
		INT32	collectStart;
		INT32	dataSend;			// COMM_REQ, COMM_COMP

		double	min;
		double	max;
		double	avg;
	}VacFdcData;

	typedef struct _VacFdcInfo
	{
		INT32	actuatorNo;
		VacFdcData data[100];
	}VacFdcInfo;
	/** FDC VACUUM DATA **/

	typedef struct _CylControl
	{
		INT32	actuatorNo;
		INT32	indexNo;
		INT32	setA;			// (1:ON),(0:OFF)
		INT32	setB;			// (1:ON),(0:OFF)
	}CylCmd;

	typedef struct _CylState
	{
		INT32	diA;			// Display when defined
		INT32	diB;			// Display when defined
		INT32	doA;			// Display when defined
		INT32	doB;			// Display when defined
		INT32	iDir;			// (1:FWD,UP,OPEN,CW), (2:BWD,DOWN,CLOSE,CCW)
		INT32	oDir;			// (1:FWD,UP,OPEN,CW), (2:BWD,DOWN,CLOSE,CCW) 
		double	elapsedTimeA;
		double	elapsedTimeB;
	}CylState;

	typedef struct _CylTable
	{
		INT32	actuatorNo;
		INT32	delayTimeA[MAX_INDEX_CYLINDER];
		INT32	delayTimeB[MAX_INDEX_CYLINDER];
	}CylTable;

	typedef struct _ScopeData
	{
		INT32	totalCount;
		INT32	intervalTime;	//msec
		char	actuatorName[MAX_SCOPE_ACTUATOR][MAX_SCOPE_CHAR];
		double	mesureValue[MAX_SCOPE_ACTUATOR][MAX_SCOPE_DATA];
	}ScopeData;

	typedef struct _MsgNotice
	{
		char    strMsgTtile[MAX_MSG_TITLE_CHAR];
		char    strBtn[MAX_MSG_NOTICE][MAX_MSG_BUTTON_CHAR];
	}MsgNotice;

	typedef struct _MotorSetData
	{
		int setMtNo;
		int setIdxNo;
		double setPos;
		double setAcc;
		double setSpeed;
	}MotorSetData;

	typedef struct _MtUpdateMmi
	{
		MotorSetData setData[MAX_UPDATE_MMI];
	}MtUpdateMmi;

	typedef struct _PkgSetData
	{
		int setPkgNo;
		double setValue;
	}PkgSetData;

	typedef struct _PkgUpdateMmi
	{
		PkgSetData setData[MAX_UPDATE_MMI];
	}PkgUpdateMmi;

	typedef struct _PrsViData
	{
		int viNo;
		int recvCnt;
		int result[MAX_VIDATA_COUNT]; // 0: Good, 1~ : Reject
		XYT data[MAX_VIDATA_COUNT];
	}PrsViData;

	//typedef struct _QcViData
	//{
	//	int viNo;
	//	int recvCnt;
	//	int result[MAX_VIRESULT_COUNT];
	//}QcViData;

	// Only MultiPacking
	typedef struct _QcViData
	{
		int viNo;
		int recvCnt;
		int result[MAX_VIRESULT_COUNT];	// 0: Fail, 1 : Good
		double LabelCoordX;	// Label Barcode X,Y좌표
		double LabelCoordY;	// Label Barcode X,Y좌표
		double LabelBlankPixel; // Lable 테두리 여백의 감지된 PIXEL
	}QcViData;

	typedef struct _BarcodeResult
	{
		int		barCodeNo;
		char	str[MAX_BARCODE_RESULT_CHAR];
	}BarcodeResult;

	typedef struct _BcdViData
	{
		int viNo;
		int recvCnt;
		int result[MAX_VIUNIT_COUNT];
		XYT data[MAX_VIUNIT_COUNT];
		char barcode[MAX_VIUNIT_COUNT][MAX_VIUNIT_CHAR];
	}BcdViData;
#pragma pack(pop)
};

#define MMI_BUFFER_SIZE					1024 * 1000
#define MAX_LAYER_CNT					5
#define MAX_UNIT_CNT					10
#define MAX_LABEL_PRS_CNT				10

namespace IPC_MMI
{
	enum CMD
	{
		// Common : 0 ~ 99
		CMD_NOTHING						= 0,
		CMD_RD_IO						= 1,
		CMD_WR_IO						= 2,
		CMD_RD_PKG						= 3,
		CMD_WR_PKG						= 4,
		CMD_RD_NDM						= 5,
		CMD_WR_NDM						= 6,
		CMD_RD_DDM						= 7,
		CMD_WR_DDM						= 8,
		CMD_RD_USE						= 9,
		CMD_WR_USE						= 10,

		CMD_RD_MT_STATUS				= 11,
		CMD_RD_MT_TABLE					= 12,
		CMD_WR_MT_TABLE					= 13,
		CMD_WR_MT_CMD					= 14,
		CMD_RD_ERR_WR					= 15,
		//CMD_RD_VAC						= 16,
		//CMD_WR_VAC						= 17,
		CMD_RD_MSGNOTICE				= 18,

		CMD_RD_SYS						= 20,
		CMD_WR_SYS						= 21,
		CMD_RD_RCP						= 22,
		CMD_WR_RCP						= 23,

		CMD_RD_MT_UPDATE_MMI			= 24,
		CMD_RD_PKG_UPDATE_MMI			= 25,
		CMD_RD_RCP_UPDATE_MMI			= 26,

		CMD_RD_VAC_STATE				= 27,	// NEW
		CMD_RD_VAC_TABLE				= 28,	// NEW
		CMD_WR_VAC_TABLE				= 29,	// NEW
		CMD_WR_VAC_CONTROL				= 30,	// NEW

		CMD_RD_CYL_STATE				= 31,	// NEW
		CMD_RD_CYL_TABLE				= 32,	// NEW
		CMD_WR_CYL_TABLE				= 33,	// NEW
		CMD_WR_CYL_CONTROL				= 34,	// NEW

		CMD_RD_SEQ_VERSION              = 39,	// 25.11.12
		CMD_WR_FIRE_DETECT_STATE		= 40,	// 25.05.30

		CMD_RD_VAC_FDC					= 41,	/** FDC VACUUM DATA **/
		CMD_WR_VAC_FDC					= 42,	/** FDC VACUUM DATA **/
		// User : 100 ~
		//CMD_RD_REFERENCE_MAP			= 100,
		//CMD_WR_REFERENCE_MAP			= 101,

		CMD_WR_PRS_RESULT				= 100,
		CMD_WR_QC_RESULT				= 101,
		CMD_WR_VISION_BARCODE			= 102,
		CMD_WR_BCD_RESULT				= 103,	// 25.06.02

		CMD_WR_PRE_MC_DATA_RECV			= 105,

		CMD_RD_TRAY_PART_INFO			= 110,
		CMD_WR_TRAY_PART_INFO			= 111,
		CMD_RD_COBOT_PART_INFO			= 112,
		CMD_WR_COBOT_PART_INFO			= 113,
		CMD_RD_FOAM_PART_INFO			= 114,
		CMD_WR_FOAM_PART_INFO			= 115,
		CMD_RD_LABEL_PART_INFO			= 116,
		CMD_WR_LABEL_PART_INFO			= 117,
		CMD_RD_MBB_PART_INFO			= 118,
		CMD_WR_MBB_PART_INFO			= 119,

		CMD_RD_ROBOT_IO					= 140,
		CMD_WR_ROBOT_IO					= 141,

		CMD_RD_ROBOT_STATUS				= 142,
		CMD_RD_ROBOT_POS_TABLE			= 143,
		CMD_WR_ROBOT_POS_TABLE			= 144,
		CMD_WR_ROBOT_CMD				= 145,
	};

	enum MOTORCMD
	{
		MTCMD_SERVO_ON					= 0,
		MTCMD_SERVO_OFF					= 1,
		MTCMD_INDEX_MOVE				= 2,
		MTCMD_JOG_MOVE					= 3,
		MTCMD_JOG_STOP					= 4,
		MTCMD_R_MOVE					= 5,
		MTCMD_A_MOVE					= 6,
		MTCMD_ALL_HOME					= 7,
		MTCMD_HOME						= 8,
		MTCMD_RESET						= 9,
		MTCMD_STOP						= 10,
		MTCMD_BRAKE_ON					= 11,
		MTCMD_BRAKE_OFF					= 12,
	};

	enum ROBOTCMD
	{
		RBCMD_ARM_ACTIVATION			= 0,
		RBCMD_ARM_POWER_OFF				= 1,
		RBCMD_CHANGE_REAL_MODE			= 2,
		RBCMD_START_PROGRAM_ONCE		= 3,
		RBCMD_START_PROGRAM_REPEAT		= 4,
		RBCMD_STOP_PROGRAM				= 5,
		RBCMD_PAUSE_PROGRAM				= 6,
		RBCMD_RESUME_PAUSE				= 7,
		RBCMD_RESUME_COLLISION			= 8,
		RBCMD_LOAD_DEFAULT_PROGRAM		= 9,
		RBCMD_LOAD_PROGRAM_TABLE		= 10,
		RBCMD_RESET_SOS_FLAG			= 11,
		RBCMD_SPEED_BAR_CHANGE			= 12,

		RBCMD_INDEX_JMOVE				= 13,
		RBCMD_INDEX_LMOVE				= 14,
	};

	enum UnitList
	{
		UNIT_SORT_PICK					= 0,
		UNIT_MAX						,
	};


	enum LaserIndexList
	{
		LaserIndex1						= 0,
		maxLaserIndex					,
	};

#pragma pack(push, 1)
	// Tray Data
	typedef struct _LotInfo
	{
		char			lotCloseChk[128];			// LOT_CLOSE_CHK		ex) Y/N								LOT CLOSE 유무 정보
		char			lotId[128];					// LOTID				ex) HTEL380MAH						LOT ID
		char			partId[128];				// PART_NO				ex) K4AAG045WA-BCWEM00-HHE45L		SCST ID의 PARTID 정보
		char			packingType[128];			// PACKING_TYPE			ex) TRAY/TR							LOT의 TYPE 정보
		int				lotQty;						// LOT_QTY				ex) 5120							LOT의 전산 수량정보
		char			lotStep[128];				// LOT_STEP				ex) T170							LOT의 전산 STEP정보
		int				trayArrayX;					// TRAY_ARRAY_X			ex) 8								TRAY X 배열 정보
		int				trayArrayY;					// TRAY_ARRAY_Y			ex) 16								TRAY Y 배열 정보
		double			trayThick;					// TRAY_THICK			ex) 5.62							TRAY 두께정보
		char			trayMarking[128];			// TRAY_MARKING			ex) "BGA-7.5X11.0-8X16-0 C-034"		TRAY 각인
		double			pocketPitchX;				// POCKET_PITCH_X		ex) 15.16							TRAY POCKET PITCH X 길이
		double			pocketPitchY;				// POCKET_PITCH_Y		ex) 19								TRAY POCKET PITCH Y 길이
		char			trayCode[128];				// TRAY_CODE			ex) LA69-01444A						TRAY 종류
		int				boxCnt;						// BOX_CNT				ex) 4								총 SCST 수량 정보
		char			semiProd[128];				// SEMI_PROD			ex) 								반제품여부
		char			holdCode[128];				// HOLDCODE				ex) 								제품HOLD CODE 정보
		char			largeBox[128];				// LARGE_BOX			ex) Y								대박스 포장 Y/N

		char			labelComparison[128];		// Label comparison variable // buffer 25.06.02 Use
		int				trayCnt;					// Current Tray Count
		char			buffer[148];				// Spare
	}LotInfo;									// 1600 Byte Set

	typedef struct _SBoxLabelInfo
	{
		char			sBoxId[128];				// SBOXID				ex)	HTEL380MAH01280213101			SCST ID 
		char			lotId[128];					// LOTID				ex)	HTEL380MAH						LOT ID
		char			partNo[128];				// PART_NO				ex)	K4AAG045WA-BCWEM00-HHE45L		SCST ID의 PARTID 정보
		char			packingType[128];			// PACKING_TYPE			ex)	TRAY/TR							LOT의 TYPE 정보
		char			lotStep[128];				// LOT_STEP				ex)	T170							LOT의 전산 STEP정보
		char			semiProd[128];				// SEMI_PROD			ex)									반제품여부
		int				boxCnt;						// BOX_CNT				ex)	4								총 SCST 수량 정보
		int				labelCnt;					// LABEL_CNT			ex)	1								라벨 수량 정보
		char			largeBox[128];				// LARGE_CST			ex)	Y/N								대박스 포장 Y/N
		char			sBoxLabel[20000];			// S_CST_LABEL			ex)									samsung label 정보
		char			specialLabel[20000];		// SPECIAL_LABEL		ex)									partial label 정보
		char			buffer[120];				// 
	}SBoxLabelInfo;								// 21024 Byte Set

	typedef struct _TrayFlag
	{
		bool			labelInspDid;			// Cobot - Top Foam Label Inspection
		bool			packFaom2DReadDid;		// Scanner - Pack Foam Label 2D Read
		bool			btmFoldDid;				// Btm Fold
		bool			btmPartialFoamReq;		// Btm Foam Type 1:Normal 0:Partial
		bool			ltCobotLabelPrinted;	// Top Foam Label Printed
		bool			rtCobotLabelPrinted;	// Mbb Label Printed
		bool			ltCobotLabelPickup;		// Label Pickup
		bool			rtCobotLabelPickup;		// 
		bool			lotCloseCheck;			// 
		bool			dummy[7];
	}TrayFlag;									// 16 Byte Set

	typedef struct _TrayPartData				// Only SEQ
	{
		int				exist;
		LotInfo			lotInfo;				// MMI Data Receive
		SBoxLabelInfo	labelInfo;				// MMI Data Receive
		TrayFlag		flag;
	}TrayPartData;

	typedef struct _TrayPartInfo				// MMI <-> SEQ
	{
		int				nStart;
		int				nEnd;
		TrayPartData	data[nvTrayPartMax];
	}TrayPartInfo;

	// Cobot Data
	typedef struct _LtCobotFlag
	{
		bool			foamDustHicViDid;
		bool			preCoverFodViDid;		// FOD:Foreign Object Inclusion
		bool			foamLabelAttchedViDid;
		bool			partialFoamReq;
		bool			dummy[12];
	}LtCobotFlag;								// 16 Byte Set

	typedef struct _RtCobotFlag
	{
		bool			mbbMisplaceViDid;
		bool			mbbLabelAttchedViDid;
		bool			dummy[14];
	}RtCobotFlag;								// 16 Byte Set

	typedef struct _CobotPartData				// Only SEQ
	{
		int				exist;					// Foam 1:Btm 2:Partial 3:Top or Mbb
		int				existLabel;
		LotInfo			lotInfo;				// MMI Data Receive
		SBoxLabelInfo	labelInfo[2];			// 0: Mbb or Foam // 1: Label Picker // MMI Data Receive 
		LtCobotFlag		flagLt;
		RtCobotFlag		flagRt;
	}CobotPartData;

	typedef struct _CobotPartInfo				// MMI <-> SEQ
	{
		int				nStart;
		int				nEnd;
		CobotPartData	data[nvCobotPartMax];
	}CobotPartInfo;

	// Foam Stack Data
	typedef struct _FoamFlag
	{
		bool			levelDid;				// Stack
		bool			recvFullDid;			// Stack
		bool			sendEmptyDid;			// Stack
		bool			flipCheckDid;			// Foam Pk
		bool			cstMaterialEmpty;		// Foam Cst
		bool			amrLoadCallDid;			// Foam Cst
		bool			amrUnloadCallDid;		// Foam Cst
		bool			cstRfidReadDid;			// Foam Cst RFID Read
		bool			dummy[8];
	}FoamFlag;									// 16 Byte Set

	typedef struct _FoamPartData				// Only SEQ
	{
		int				exist;					// Foam Exist 
		int				cnt;					// Count		// LtCst Max:120, RtCst Max: 64
		SBoxLabelInfo	labelInfo;				// Label Info
		FoamFlag		flag;
	}FoamPartData;

	typedef struct _FoamPartInfo				// MMI <-> SEQ
	{
		int				nStart;
		int				nEnd;
		FoamPartData	data[nvFoamPartMax];
	}FoamPartInfo;

	// Label Part Data
	typedef struct _LabelFlag
	{
		bool			existCst;
		bool			inUseCst;
		bool			firstFeeding;			// 1st Feeding Flag
		bool			cstMaterialEmpty;		// The feeder has no label
		bool			amrLoadCallDid;
		bool			amrUnloadCallDid;
		bool			cstRfidReadDid;
		bool			lastlabel;				// 1번과 2번 중 나중에 뽑은 것
		bool			dummy[8];
	}LabelFlag;									// 16 Byte Set

	typedef struct _LabelPartData				// Only SEQ
	{
		int				exist;					// Label & Feeder Exist 
		int				cnt;					// Count
		LotInfo			lotInfo;				// MMI Data Receive
		SBoxLabelInfo	labelInfo;				// MMI Data Receive
		XYT				PrsResult[MAX_LABEL_PRS_CNT];
		LabelFlag		flag;
	}LabelPartData;

	typedef struct _LabelPartInfo				// MMI <-> SEQ
	{
		int				nStart;
		int				nEnd;
		LabelPartData	data[nvLabelPartMax];
	}LabelPartInfo;

	// Mbb Part Data
	typedef struct _MbbFlag
	{
		bool			existCst;				// Cst 자체 유무
		bool			inUseCst;				// 사용중
		bool			cstMaterialEmpty;		// Cst안의 자재가 없는 경우
		bool			amrLoadCallDid;
		bool			amrUnloadCallDid;
		bool			cstRfidReadDid;
		bool			dummy[10];
	}MbbFlag;									// 16 Byte Set

	typedef struct _MbbPartData					// Only SEQ
	{
		int				exist;					// Mbb Exist 
		int				cnt;					// Count
		LotInfo			lotInfo;				// MMI Data Receive
		SBoxLabelInfo	labelInfo;				// Label Info
		MbbFlag			flag;
	}MbbPartData;

	typedef struct _MbbPartInfo					// MMI <-> SEQ
	{
		int				nStart;
		int				nEnd;
		MbbPartData		data[nvMbbPartMax];
	}MbbPartInfo;


	// Lot Data & For Display
	typedef struct _LotData
	{
		LotInfo		lotInfo;					// 

		int			lotTotalCnt;
		int			lotInCnt;
		int			lotOutCnt;
	}LotData;

	typedef struct _LotList
	{
		LotData			lotData[nvLotMax];
	}LotList;


	typedef struct _FireDetector
	{
		int				target;
		double			curValue;
		double			setWarningValue;
		double			setAlarmValue;

		bool			isRun;					// 0 : init, 1: run
		bool			smokeAlarm;				// 0 : normal, 1 : detect
		bool			tempWarning;			// 0 : normal, 1 : warning
		bool			tempAlarm;				// 0 : normal, 1 : alarm
	}FireDetector;

	typedef struct _SeqVersion
	{
		char            data[128];				// 1.0.0.0  (동일설비.H/W add.S/W add.S/W bug)
	}SeqVersion;

	typedef struct _FireDetectorInfo			// MMI <-> SEQ
	{
		int				nStart;
		int				nEnd;
		FireDetector	data[nvFireDetectorMax];
	}FireDetectorInfo;

	// Cobot
	typedef struct _RobotCmd
	{
		int			nRobotNo;
		int			nValue;
		int			nCmd;
	}RobotCmd;

	typedef struct _RobotTable
	{
		int			nRobotNo;

		int					nVel[MAX_INDEX_ROBOT];
		Robot::RobotTcp		tcp[MAX_INDEX_ROBOT];
		Robot::RobotJoint	joint[MAX_INDEX_ROBOT];
	}RobotTable;

	typedef struct _RobotState
	{
		int			robotNo;

		Robot::RobotTcp		curTcp;
		Robot::RobotJoint	curJoint;

		BOOL		activated;
		BOOL		realMode;
		BOOL		collisionDetected;
		BOOL		armPowerEngaged;
		BOOL		directTeachingMode;
		BOOL		moving;
		BOOL		pauseState;
		BOOL		teachingPendantConnecred;
		BOOL		programRun;
		BOOL		noArcModeOn;
		BOOL		emgButtonReleased;
		BOOL		firstProgramRun;
		BOOL		lastProgramLoadDone;
		BOOL		underActivation;
		BOOL		collisionState;
		BOOL		systemSOS;
		BOOL		homeIsBegin;
		BOOL		controlBoxHeartBeat;

		double		controlBoxAmpere;
		double		tcpMass;

		int			collisionThreshold;
		int			speedBarValue;

		int			curIdx;

		BOOL		isHome;
		BOOL		isHoming;
	}RobotState;

	typedef struct _RobotReadIO
	{
		int			robotNo;
		uint16_t	inCH;
		uint16_t	outCH;
	}RobotReadIO;

	typedef struct _RobotWriteIO
	{
		int			robotNo;
		int			nIONo;
		BOOL		bOn;
	}RobotWriteIO;

	typedef union _IpcShared
	{
		CHAR				bufferSize[MMI_BUFFER_SIZE];

		MtCmd				mtControl;
		MtTable				mtTable;
		MtStatus			mtStatus;
		WriteIO				wrIO;
		ReadIO				rdIO;

		DoubleData			pkg;
		DoubleData			ddm;
		IntData				ndm;
		IntData				use;
		DoubleData			sys;
		DoubleData			rcp;

		ErrWr				errWr;
		MsgNotice			msgNoticeTitle;

		VacCmd				vacControl;
		VacState			vacState[100];
		VacTable			vacTable;
		VacFdcInfo			vacFdcInfo;		/** FDC VACUUM DATA **/

		CylCmd				cylControl;
		CylState			cylState[100];
		CylTable			cylTable;

		MtUpdateMmi			mtupdatemmi;
		PkgUpdateMmi		pkgupdatemmi;

		BarcodeResult		barcodeResult;
		PrsViData			prsViData;
		QcViData			qcViData;
		BcdViData			bcdViData;

		RobotCmd			robotCmd;
		RobotTable			robotTable;
		RobotState			robotState;
		RobotReadIO			rdRobotIO;
		RobotWriteIO		wtRobotIO;

		TrayPartInfo		trayPartInfo;
		CobotPartInfo		cobotPartInfo;
		FoamPartInfo		foamPartInfo;
		LabelPartInfo		labelPartInfo;
		MbbPartInfo			mbbPartInfo;

		SeqVersion          seqVersion;
		FireDetectorInfo	fireDetectorInfo;

	}IpcShared;

	typedef struct _IpcBuffer
	{
		IpcShared		buffer;
		INT32			command;
	}IpcBuffer;
#pragma pack(pop)
}
