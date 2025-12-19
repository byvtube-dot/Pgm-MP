#pragma once
#include "DEF\AntiCollisionDef.h"
#include "DEF\ApiBufferDef.h"
#include "DEF\AxisDef.h"
#include "DEF\AxisPosDef.h"
#include "DEF\VisionDef.h"
#include "DEF\ErrNameDef.h"
#include "DEF\FsmDef.h"
#include "DEF\IODef.h"
#include "DEF\LaserDef.h"
#include "DEF\LogDef.h"
#include "DEF\PkgData.h"
#include "DEF\PneumaticDef.h"
#include "DEF\PneumaticPosDef.h"
#include "DEF\UserMemoryDef.h"
#include "ROBOT\RainbowRoboticsDef.h"

//hkkim251103 (동일설비.H/W add.S/W add.S/W bug)
#define SEQ_VERSION_DATA			"1.0.0.0" 
#define TO_STRING(NAME) #NAME
#define SLAVE_IO_TYPE				0
#define SLAVE_MT_TYPE				1
#define MAX_CH						50
#define MAX_CONTACT					16
#define MAX_MESURETIME_CNT 			100
#define MAX_USERMEM_CONTACT			8
#define MAX_USERMEM_BUFFER_COUNT	10000
#define MAX_INTERPOLATION_MOTOR		5
#define HOME_CANMOVE				(-999)
#define P_JOG						(99)
#define WEIDMULLER_AD_S7			27648
#define WEIDMULLER_AD_S5			16384
#define KOSES_AD					4096
#define WAGO_AI						32760					
#define KOSES_MAXVOLT				5
#define KOSES_MINVOLT				0
#define ADCONVERT_MAXVOLT			10
#define ADCONVERT_MINVOLT			0
#define ADCONVERT_MAXAMP			20
#define ADCONVERT_MINAMP			0
//	Analogue Sensor Model:MPS - P35R - NCA
#define MPS_P35R_NCA_MAX_PRESSURE	1000		//	(KPA)
#define MPS_P35R_NCA_MIN_PRESSURE	0		//	(KPA)
#define MPS_P35R_NCA_MAXVOLT		5
#define MPS_P35R_NCA_MINVOLT		1
//	Analogue Sensor Model:MPS - V35R - NCA
#define MPS_V35R_NCA_MAX_PRESSURE	-101	//	(KPA)
#define MPS_V35R_NCA_MIN_PRESSURE	0		//	(KPA)
#define MPS_V35R_NCA_MAXVOLT		5
#define MPS_V35R_NCA_MINVOLT		1

//	Analogue Sensor Model:ZSE40A
#define ZSE40A_MAX_PRESSURE			-101	//	(KPA)
#define ZSE40A_MIN_PRESSURE			0		//	(KPA)
#define ZSE40A_MAXVOLT				5
#define ZSE40A_MINVOLT				1

//	Analogue Sensor Model:PSE541-R06
#define PSE541_MAX_PRESSURE			-101	//	(KPA)
#define PSE541_MIN_PRESSURE			0		//	(KPA)
#define PSE541_MAXVOLT				5
#define PSE541_MINVOLT				1

//	Analogue Sensor Model:IL-100
#define IL100_MAX_MM				130		//	(mm)
#define IL100_MIN_MM				75		//	(mm)
#define IL100_MAXVOLT				5
#define IL100_MINVOLT				1

//	Analogue Sensor Model:Baumer LoadCell
#define BAUMER_LOADCELL_MAX_NEWTON	5000	//	(Newton)
#define BAUMER_LOADCELL_MIN_NEWTON	0		//	(Newton)
#define BAUMER_LOADCELL_MAXVOLT		10
#define BAUMER_LOADCELL_MINVOLT		0

//	Analogue Sensor Model:HFC-C-D-302B (MFC)
#define MFC_MAX_ML					5		//	(liter)
#define MFC_MIN_ML					0		//	(liter)
#define MFC_MAXVOLT					5
#define MFC_MINVOLT					0

#define MAX_LIFE_TIME_ITEM			30
#define MAX_MMI_NAME_CHAR			50
#define BUFF_LENGTH					512
#define MAX_ERROR_ARRAY				10

#define QC_VAC_CNT					6

#define _SAMSUNG_LAMP_

#define	_1sec						1000
#define	_3sec						3000
#define	_5sec						5000
#define _7sec						7000
#define _10sec						10000
#define _15sec						15000
#define _30sec						30000
#define _40sec						40000
#define _50sec						50000
#define _1min						60000
#define _5min						300000
#define _10min						600000
#define _30min						30000000
#define _1Hour						60000000
#define _24Hour						1440000000

// 88-24-0042-SEC-2
#define _BTM_FOAM_					0
#define _TOP_FOAM_					1

#define _FOAM						0
#define _MBB						0
#define _LABEL						1

#define _PICK						0
#define _PLACE						1

#define _TRAY_MAX_CNT				11
#define _TRAY_MIN_CNT				3
#define _COVER_TRAY_CNT				1

#define _LT							0
#define _RT							1

#define _NORMAL						0
#define _PARTIAL					1
// 88-24-0042-SEC-2

enum enLotState
{
	ST_LOT_IDLE				= 0,
	ST_LOT_START			= 1,
	ST_LOT_PRE_END			= 2,
	ST_LOT_END				= 3,
};

enum enMsgNotice
{
	// SEQ -> MMI, 단 0은 Ready로 봄, 1~9 : 예외적인 트리거(강제종료 등...) , 값 10부터는 CMD NUMBER로 보면
	// MMI -> SEQ  1: 팝업 중, 2 : 버튼 0 눌렀을때, 3 : 버튼 1 눌렀을때, 4 : 버튼 2 눌렀을때, 5 : 버튼 3 눌렀을때

	RECEIVE_CMD_READY		= 0,
	RECEIVE_CMD_POP			,
	RECEIVE_CMD_BTN1		,
	RECEIVE_CMD_BTN2		,
	RECEIVE_CMD_BTN3		,
	RECEIVE_CMD_BTN4		,
	RECEIVE_CMD_CANCEL		= (-1),

	SEND_CMD_READY			= 0,
	SEND_CMD_CLOSE1			,
	SEND_CMD_CLOSE2			,
	SEND_CMD_CLOSE3			,
	SEND_CMD_CLOSE4			,
	SEND_CMD_CLOSE5			,
	SEND_CMD_CLOSE6			,
	SEND_CMD_CLOSE7			,
	SEND_CMD_CLOSE8			,
	SEND_CMD_CLOSE9			,

	MSG_NOTICE				= 10,
	MSG_CANMOVE				= 11,
	MSG_HOME_FAIL			= 12,
	MSG_INLET_VI_ORG_FAIL	= 13,
	MSG_INLET_VI_ALIGN_FAIL	= 14,

	MSG_OP_BREAK			= 22,
	MSG_OP_UNITWAIT			= 30,

	MSG_CROSS_HAIR_CNT_INDEX1_CLEAR = 31,
	MSG_CROSS_HAIR_CNT_INDEX2_CLEAR = 32,
	MSG_CROSS_HAIR_CNT_INDEX3_CLEAR = 33,
	MSG_CROSS_HAIR_CNT_INDEX4_CLEAR = 34,

	MSG_INPK_PICKUP_FAIL			= 40,
};

enum enCommState
{
	COMM_IDLE		= 0,
	COMM_REQ		= 1,	// 1~ 10
	COMM_TMOVER		= 99,
	COMM_BUSY		= 100,
	COMM_COMP		= 101,
	COMM_ERR		= 102,
};

enum TrayNum
{
	InTray		= 0,
	EmptyTray	,
	
	OutTray		= 0,
	NgTray		,
	GoodTray	,
};

enum IndexNum
{
	Index1		= 0,
	Index2		,

	L			= 0,
	R			,
};

enum UnitNum
{
	UnitAll		= 0,
	Unit1		= 1,
	Unit2		= 2,
	Unit3		= 3,
	Unit4		= 4,
	Unit5		= 5,
	Unit6		= 6,
};

enum SortResult
{
	UnitEmpty	= 0,
	UnitGood	= 1,
	UnitBin1	= 2,
	UnitBin2	= 3,
	UnitBin3	= 4,
};

enum ViItemResult
{
	ViBin1 = 0,
	ViGood = 1,
};

enum enBcdList
{
	bcdMin = 0,
	bcdTrayShuttle = 0,
	bcdMax,
};

enum enMachineState
{
	MC_STATE_STOP			= 0,
	MC_STATE_RUN			= 1,
	MC_STATE_ERROR			= 2,
	MC_STATE_CYCLE			= 3,
	MC_STATE_EMG			= 4,
	MC_STATE_RUN_DOWN		= 5,
	MC_STATE_WARNNING		= 6,
};

enum enExist
{
	enEmpty = 0,

	enExistTray				= 1,	// Tray
	enExistTrayQc			= 2,	// Banding inspection
	enExistBtmFoam			= 3,	// Bottom Foam
	enExistBtmFoamOpen		= 4,	// Bottom Foam Open
	enExistBtmFoamQc		= 5,	// Bottom Foam Qc
	enExistBtmFoamTray		= 6,	// Bottom Foam + Tray
	enExistBtmFoamTrayQc	= 7,	// Bottom Foam + Tray + Vision inspection
	enExistTrayTopFoam		= 8,	// Bottom Foam + Tray + Top foam
	enExistTrayTopFoamQc	= 9,	// Bottom Foam + Tray + Top foam
	enExistMbb				= 10,	// Mbb
	enExistMbbOpen			= 11,	// Mbb open
	enExistMbbTray			= 12,	// Mbb + Tray
	enExistSealed			= 13,	// Mbb + Tray + Sealing
	enExistSealedQc			= 14,	// Mbb + Tray + Sealing + Qc

	enLabelExist			= 1,	// Label
	enLabelExistPrs			= 2,	// Label + Prs
	enLabelExistPrinted		= 3,	// Label + Print
	enLabelExistQc			= 4,	// Label + Print + Qc
	enLabelExistQcNg		= 5,	// Label + Print + Qc Ng

	enFoamExistTop			= 1,	// Top foam
	enFoamExistBtmPartial	= 2,	// Btm Partial foam
	enFoamExistBtm			= 3,	// Btm foam
	enFoamExistTopLabel		= 4,	// Top foam + Label

	enMbbExist				= 1,	// Mbb
	enMbbExistQc			= 2,	// Mbb + Qc
	enMbbExistLabel			= 3,	// Mbb + Label
	enMbbExistLabelQc		= 4,	// Mbb + Label + Qc
};

enum enFoamElevList
{
	enFoamRecvElev = 0,
	enFoamPartialElev,
	enFoamSendElev,
	enFoamElevMax,
};

enum enFoamBoxList
{
	enFoamBoxLt = 0,
	enFoamBoxRt,
	enFoamBoxMax,
};

enum enLabelFeederList
{
	enlabelLt = 0,
	enlabelRt,
	enlabelMax,
};

enum enMbbCstList
{
	enMbbCstLt = 0,
	enMbbCstRt,
	enMbbCstMax,
};

enum NvTrayList
{
	nvInPk = 0,
	nvBtmFoam,
	nvTopFoam,
	nvTrayShuttle,
	nvMbbShuttle,
	nvMbbQc,
	nvMbbFold,
	nvMbbFoldPk,
	nvTrayPartMax,
};

enum NvCobotList
{
	nvLtCobot = 0,
	nvRtCobot,
	nvCobotPartMax,
};

enum NvFoamStackList
{
	nvFoamRecvElev = 0,
	nvFoamRrStack,
	nvFoamFtStack,
	nvFoamSendElev,
	nvFoamFlip,
	nvFoamPartialElev,
	nvFoamLtCst,
	nvFoamRtCst,
	nvFoamPk,
	nvFoamPartMax,
};

enum NvLabelPartList
{
	nvLabelLtFeeder = 0,
	nvLabelRtFeeder,
	nvLabelLtSuttle,
	nvLabelRtSuttle,
	nvLabelPartMax,
};

enum NvMbbPartList
{
	nvLtMbbStack = 0,
	nvRtMbbStack,
	nvMbbPartMax,
};

enum NvFireDetectorList
{
	nvFireDetector1 = 0,
	nvFireDetector2,
	nvFireDetector3,
	nvFireDetector4,
	nvFireDetector5,
	nvFireDetector6,
	nvFireDetector7,
	nvFireDetector8,
	nvFireDetectorMax,
};


enum NvLotList
{
	nvLot1 = 0,
	nvLot2,
	nvLot3,
	nvLot4,
	nvLot5,
	nvLot6,
	nvLot7,
	nvLot8,
	nvLot9,
	nvLot10,
	nvLotMax,
};