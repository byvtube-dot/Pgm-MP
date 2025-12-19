#pragma once
#include "..\Defines.h"
#include "..\..\BASE\BaseAll.h"
#include "..\..\BASE\kamelasbase.h"
#include "..\..\BASE\KASIO.h"
#include "IPC_MMI.h"

enum
{
	MT_NOTHING			= 0,
	MT_INPOS			= 100,
	MT_MOVE				= 200,
};

enum
{
	C_ALLHOME_START		= 100,
	C_ALLHOME_END		= 199,

	C_LOTEND_START		= 200,
	C_LOTEND_PROCESS	= 201,
	C_LOTEND_END		= 299,

	C_OPERATION_START	= 300,
	C_OPERATION_END		= 399,
};

typedef struct _tagTraceAbility
{
//	IPC_MMI::ReferMap refmap;
}TraceAbility;

typedef struct _tagCalData
{
	POINT2D val[100][100];
}CalData;

typedef struct _tagGlassCal
{
	BOOL		isSuccess;
	POINT		Count;
	double		PitchX;
	double		PitchY;
	double		AngleT;
	POINT2D		VectorPitchX;
	POINT2D		VectorPitchY;
	POINT2D		StartPos;
	POINT2D		val[100][100];
}GlassCal;

typedef struct _tagStageCal
{
	BOOL		isSuccess;
	POINT		Count;
	double		PitchX;
	double		PitchY;
	double		AngleT;
	POINT2D		VectorPitchX;
	POINT2D		VectorPitchY;
	POINT2D		StartPos;
	POINT2D		val[100][100];
}StageCal;

typedef struct _MesureTime
{
	CTimer		timer;
	BOOL		isStarted;
	DWORD		setTime;
	DWORD		getTime;
}MesureTime;

typedef struct _PrsUnit
{
	XYT		FiducialOffset[IPC_MMI::MAX_FIDUCIAL];
	XYT		StepPos[IPC_MMI::MAX_STEP];
	XYT		PreQcPos[IPC_MMI::MAX_STEP];
}PrsUnit;

typedef struct _UnitData
{
	PrsUnit Unit[IPC_MMI::MAX_UNIT];
}UnitData;

typedef struct _StageData
{
	IPC_LASER::LaserDwg Dwg;
	UnitData	PrsData;
}StageData;

typedef struct _UphTable
{
	int index;
	int val[360];
	int RelaCnt;
	int MachineCnt;
	double MachineTime;
}UphTable;

class COpBtn
{
private:
	BOOL	m_stopBtnState;
	BOOL	m_startBtnState;
	BOOL	m_resetBtnState;
	BOOL	m_pressedBreak;

	CTimer  m_tmBtnStop;
	CTimer  m_tmBtnStart;
	CTimer  m_tmBtnReset;

	void Stop(void);
	void Start(void);
	void Reset(void);
	void Break(void);

public:
	COpBtn();
	virtual ~COpBtn() {}

	void Run(void);
};

class CKamelasComm
{
private:
	kamelaslib::KamelasBase* m_pBase;

public:
	BOOL Run(LPCWSTR lpName, UINT32 nSize, BOOL bServer)
	{
		this->m_pBase = kamelaslib::CreateKamelas(bServer, (LPWSTR)lpName, nSize);
		BOOL bSuccess = this->m_pBase->open();
		return (bSuccess);
	}

	BOOL Recv(PVOID pBuffer, DWORD dwMilliSeconds, UINT32& nSize)
	{
		BOOL bRet = this->m_pBase->recv(pBuffer, dwMilliSeconds);
		return (bRet);
	}

	BOOL Send(PVOID pBuffer, DWORD dwMilliSeconds, UINT32 nSize)
	{
		BOOL bRet = this->m_pBase->send(pBuffer, dwMilliSeconds);
		return (bRet);
	}

public:
	CKamelasComm()
	{

	}
	~CKamelasComm()
	{
		if (NULL != this->m_pBase)
			kamelaslib::DestroyKamelas(this->m_pBase);

		this->m_pBase = NULL;
	}
};

namespace IPC_LASER
{
	class CIPC
	{
	public:
		CIPC()
		{
			if (Init())
				return;

			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
		}
		virtual ~CIPC() {}

		CTimer tmWatchDog;

		void Comm(void);
		BOOL Init(void);

	private:
		CKamelasComm m_kamelas;
	};
}

namespace IPC_MMI
{
	class CIPC
	{
	public:
		CIPC()
		{
			if (Init())
				return;

			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
		}
		virtual ~CIPC() {}

		CTimer tmWatchDog;

		void Comm(void);
		BOOL Init(void);

	private:
		CKamelasComm m_kamelas;
		void MtControl(MtCmd cmd);
		void RobotControl(RobotCmd robotCommand);
	};
}

void MainSequence(void);

//_01_Init.cpp
void InitSequence();
BOOL InitLOG(void);
BOOL InitINI(void);
BOOL InitIO(void);
BOOL InitMOTOR(void);
BOOL InitSEAL(void);
BOOL InitINTERPOLATION(void);
BOOL InitVISION(void);
BOOL InitVACUUM(void);
BOOL InitCYLINDER(void);
BOOL InitCONVEYOR(void);
BOOL InitLASER(void);
BOOL InitROBOT(void);
BOOL InitETC(void);

//_02_Error.cpp
BOOL ErrorProcess(void);

//_03_Auto.cpp
void AutoSequence();
void StartStop(void);

//_04_Common.cpp
void CommonSequence(void);
void UpdateForMMI(void);
void AntiCollisionF(void);
void AllCycleOff(void);
void CalUPH(void);
BOOL IsEMO(void);
BOOL IsMotorPwr(void);
void MotorPwr(BOOL bOn, int onStep);
BOOL IsDoorOpen(void);
BOOL IsDoorUnlock(void);
void DoorlockOn(void);
void DoorlockOff(void);
void AlarmDoorOpen(void);
void AlarmDoorLock(void);
void AlarmFire(void);
void AlarmFan(void);
BOOL IsAnyCycleRun(void);
BOOL IsWrongPkg(void);
BOOL IsWrongSys(void);
BOOL IsWrongRcp(void);
void SetMesureTime(int no);
void GetMesureTime(int no);
BOOL Align2Point(ALIGN_POINT_DATA data, POINT2D coord, XYT& offset);
BOOL Align3Point(ALIGN_POINT_DATA data, POINT2D coord, XYT& offset);
BOOL Align4Point(ALIGN_POINT_DATA data, POINT2D coord, XYT& offset);
int MtPosMove(AxisList mtno, int posIdx);
int MtPosMove(AxisList mtno, int posIdx, double pos, double vel = 0., double acc = 0.);
void AntiCollisionJogFwd(AxisList nMtNo, double vel, double acc);
void AntiCollisionJogBwd(AxisList nMtNo, double vel, double acc);
void AntiCollisionRMove(AxisList nMtNo, double cmdDist, double pos, double vel, double acc);
void AntiCollisionMove(AxisList nMtNo, int nIndex);
void JustBeforePkgValueChanges(int nNo);
void JustBeforeNdmValueChanges(int nNo);
void JustBeforeDdmValueChanges(int nNo);
void JustBeforeUseValueChanges(int nNo);
void JustBeforeSysValueChanges(int nNo);
void JustBeforeRcpValueChanges(int nNo);

//_05_Manual.cpp
void ManualSequence();

//_06_Home.cpp
void AllHomeCycle(void);
void MtHome(AxisList mtno);

//_07_LotEnd.cpp
void LotEndProcess(void);
void LotEndCycle(void);
BOOL IsPreLotCondition(void);

//_08_DryRun.cpp
void DryRunProcess(void);

//_09_OpButton.cpp
BOOL StartProcess(void);
BOOL StopProcess(void);
BOOL ResetProcess(void);
void OperationCycle(void);

//_10_Notice.cpp
BOOL NoticeProcess(void);

//_11_CanMove.cpp
BOOL LtCobotCanMove(int cmd);
BOOL InPkXCanMove(int cmd);
BOOL InPkZCanMove(int cmd);
BOOL TrayShuttleYCanMove(int cmd);
BOOL TrayPushXCanMove(int cmd);
BOOL MbbOpenZCanMove(int cmd);
BOOL FoamSendElevZCanMove(int cmd);
BOOL FoamTurnTCanMove(int cmd);
BOOL FoamRecvElevZCanMove(int cmd);
BOOL FoamPartialElevCanMove(int cmd);
BOOL FoamPkXCanMove(int cmd);
BOOL FoamPkYCanMove(int cmd);
BOOL FoamPkZCanMove(int cmd);

BOOL RtCobotCanMove(int cmd);
BOOL MbbShuttleYCanMove(int cmd);
BOOL MbbShuttleConvCanMove(int cmd);
BOOL MbbQcYCanMove(int cmd);
BOOL MbbFoldXCanMove(int cmd);
BOOL MbbFoldZCanMove(int cmd);
BOOL LabelLtShuttleYCanMove(int cmd);
BOOL LabelRtShuttleYCanMove(int cmd);
BOOL LabelLaserXCanMove(int cmd);
BOOL LabelLtFeedCanMove(int cmd);
BOOL LabelRtFeedCanMove(int cmd);
BOOL MbbLtElevZCanMove(int cmd);
BOOL MbbRtElevZCanMove(int cmd);
BOOL MbbMbbQcXCanMove(int cmd);

BOOL cylInPkOcCanMove(int dir, int index);

// Program Optimization (프로그램 최적화) // Message PopUp & Check
static BOOL CommonCanMove(int mtNo);
static BOOL IsMtRdy(int mtNo);
static BOOL IsMtSafetyPos(int mtNo, int cmd1 = -1, int cmd2 = -1, int cmd3 = -1, int cmd4 = -1, int cmd5 = -1, int cmd6 = -1, int cmd7 = -1, int cmd8 = -1, int cmd9 = -1, int cmd10 = -1);
static BOOL IsMtSafetyIdx(int mtNo, int cmd1 = -1, int cmd2 = -1, int cmd3 = -1, int cmd4 = -1, int cmd5 = -1, int cmd6 = -1, int cmd7 = -1, int cmd8 = -1, int cmd9 = -1, int cmd10 = -1);

static BOOL IsCylSafetyPos(int pmNo, int pmPos);

static BOOL IsSensorAOn(INT32 iono, DWORD dwDelayTime = 0);
static BOOL IsSensorAOff(INT32 iono, DWORD dwDelayTime = 0);
static BOOL IsSensorBOn(INT32 iono, DWORD dwDelayTime = 0);
static BOOL IsSensorBOff(INT32 iono, DWORD dwDelayTime = 0);

static BOOL IsNdmOn(NDmList ndmno);
static BOOL IsNdmOff(NDmList ndmno);
//_12_Event.cpp
//void Index1FsmEvent(int oldValue, int newValue, LPVOID lpParam);
//void Index2FsmEvent(int oldValue, int newValue, LPVOID lpParam);
//void IndexLockEvent(INT32 oldValue, INT32 newValue, LPVOID lpParam);

extern MesureTime MESURETIME[MAX_MESURETIME_CNT];
extern CNvm NV_TRACEABILITY;
extern CNvm NV_GLASS_CAL;
extern CNvm NV_STAGE_CAL;
extern CNvm NV_FLAT_CAL;
extern CNvm NV_STAGE_DATA;
extern CNvm NV_TRAY_PART_DATA;
extern CNvm NV_COBOT_PART_DATA;
extern CNvm NV_FOAM_PART_DATA;
extern CNvm NV_LABEL_PART_DATA;
extern CNvm NV_MBB_PART_DATA;
extern CNvm NV_FIRE_DETECTOR_DATA;
extern CNvm NV_SEQ_VERSION;
extern CNvm NV_ROBOT;

extern TraceAbility& TRACEABILITY;
extern GlassCal(&GLASS_CAL)[IPC_MMI::maxLaserIndex];
extern StageCal(&STAGE_CAL)[IPC_MMI::maxLaserIndex];
extern StageData(&STAGE_DATA)[MAX_STAGE];
extern IPC_MMI::TrayPartData(&TRAY_PART_DATA)[nvTrayPartMax];
extern IPC_MMI::CobotPartData(&COBOT_PART_DATA)[nvCobotPartMax];
extern IPC_MMI::FoamPartData(&FOAM_PART_DATA)[nvFoamPartMax];
extern IPC_MMI::LabelPartData(&LABEL_PART_DATA)[nvLabelPartMax];
extern IPC_MMI::MbbPartData(&MBB_PART_DATA)[nvMbbPartMax];
extern IPC_MMI::FireDetector(&FIRE_DETECTOR_DATA)[nvFireDetectorMax];
extern IPC_MMI::SeqVersion& SEQ_VERSION;
extern IPC_MMI::RobotTable(&ROBOT_TABLE)[2];
