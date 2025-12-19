#pragma once
#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\SRC\IPC_MMI.h"
#include "..\COM\SLog.h"
#include "..\..\BASE\Ini.h"

#define HOME_CANMOVE			(-999)
#define P_JOG					(99)

#define RATED_RPM				3000

#define _1MM_PITCH_				1000 * 50
#define _4MM_PITCH_				4000 * 50
#define _5MM_PITCH_				5000 * 50
#define _6MM_PITCH_				6000 * 50
#define _10MM_PITCH_			10000 * 50
#define _15MM_PITCH_			15000 * 50
#define _20MM_PITCH_			20000 * 50

#define _0_1G_					1000 * 1000
#define _0_5G_					5000 * 1000
#define _1G_					10000 * 1000
#define _2G_					20000 * 1000
#define _3G_					30000 * 1000
#define _5G_					50000 * 1000
#define _10G_					100000 * 1000
#define _20G_					200000 * 1000
#define _30G_					300000 * 1000
#define _50G_					500000 * 1000
#define _100G_					1000000 * 1000
#define _200G_					2000000 * 1000
#define _300G_					3000000000L
#define _500G_					5000000000L

#define	_0010PUSLE_RATE			100.
#define	_0100PUSLE_RATE			10.
#define	_0200PUSLE_RATE			5.
#define	_0500PUSLE_RATE			2.
#define	_1000PUSLE_RATE			1.

namespace HW
{
#pragma pack(push, 1)
	typedef struct
	{
		BYTE	isHwCwSen : 1;
		BYTE	isHwCCwSen : 1;
		BYTE	isSwCwSen : 1;
		BYTE	isSwCCwSen : 1;
		BYTE	isHomeSen : 1;
		BYTE	isServoOn : 1;
		BYTE	isBrakeOn : 1;
		BYTE	isAlarm : 1;

		BYTE	isDriving : 1;
		BYTE	isPaused : 1;
		BYTE	isHome : 1;
		BYTE	isServoRdy : 1;
		BYTE	isPosSet : 1;
		BYTE	isDelayedPosSet : 1;
		BYTE	isDummy : 2;

		double	cmdCnt;
		double	realCnt;
		double	realVel;
		double	realTrq;
	}MotorState;
#pragma pack(pop)

	class Motor
	{
	public:
		static const int maxDevice = (motorMAX > 0) ? motorMAX : 1;
		int									m_no;
		BOOL								isVirtualSW;	//노드번호를 점유안하는 바이패스
		BOOL								isVirtualHW;	//노드번호를 점유하는 바이패스
		string								m_name;
		List<string>						m_indexName;
		MotorState							m_state;
		COM::MtCommand::SingleParamter		m_command;
		int									m_curIdx;
		int									m_nxtIdx;
		int									m_brakeNo;
		double								m_moveStroke;
		double								m_moveTime;
		double								m_limitP;
		double								m_limitN;
		double								m_inposBand;
		double								m_jerkRatio;
		int									m_isDirP;
		int									m_homePosIdx;
		int									m_usePause;
		double								m_maxVel;
		double								m_maxAcc;
		double								m_maxJerkAcc;
		double								m_pulseRate;
		double								m_distLStoIndexPulse;
		int									m_homeDirection;
		COM::CFSM							m_fsmDrv;
		CTimer								m_isRdy;
		int									m_motionMode;
		string								m_sLogFncAddStart;
		string								m_sLogFncAddEnd;
		string								m_sLogDir;

	public:
		static BOOL DefaultCanMove(int cmd);
		static void CreateInitParameter();
		void SLogFncAddMessage(string message);
		void SLogStartEvent();
		void SLogEndEvent();
		COM::MtCommand::SingleParamter CommandTable(int idx);
		void GetState(void);
		void Stop(void);
		BOOL StartHomeCommand(void);
		void CancelHomeSearch(void);
		void HomeInternalCycle(void);
		void HomeSearchCycle(void);
		void JogCommandCycle(void);
		void PosCommandCycle(void);
		void VelCommandCycle(void);
		void TrqCommandCycle(void);
		void SetCnt(double cnt);
		void SetElecGearRatio(UINT32 numerator, UINT32 denominator);
		void ServoOn(void);
		void ServoOff(void);
		void BrakeOn(void);
		void BrakeOff(void);
		void AlarmClear(void);

		// TrqMove->구동시 모터 튕김 현상 방지
		int GetCmdMode(void);
		void SetCmdMode(AxisCommandMode::T axisCommandMode);

		BOOL IsRdy(int posIdx = NOT_DEFINED);
		BOOL IsRdywithoutPause(int posIdx = NOT_DEFINED);
		BOOL InPos(int posIdx);
		BOOL InPos(int posIdx, double pos, double inposBand = NOT_DEFINED);
		BOOL InCmd(int posIdx);
		BOOL InCmd(int posIdx, double pos);
		BOOL BetweenIdxPos(int posIdx1, int posIdx2);
		BOOL BetweenPos(double pos1, double pos2);
		BOOL CurIdx(int posIdx);
		BOOL CmdIdx(int posIdx);
		BOOL CurIdx(int posIdx1, int posIdx2);
		BOOL CmdIdx(int posIdx1, int posIdx2);
		BOOL CmdOrCurIdx(int posIdx);
		BOOL CmdOrCurIdx(int posIdx1, int posIdx2);
		double& PosTable(int posIdx);
		double& VelTable(int posIdx);
		double& AccTable(int posIdx);
		BOOL PosMove(int posIdx);
		BOOL PosMove(int posIdx, double pos, double vel = 0, double acc = 0);
		BOOL VelFwd(int posIdx);
		BOOL VelBwd(int posIdx);
		BOOL VelFwd(int posIdx, double vel, double acc);
		BOOL VelBwd(int posIdx, double vel, double acc);
		BOOL TrqMove(int posIdx, double trq, double vel);
		BOOL JogFwd(double vel, double acc);
		BOOL JogBwd(double vel, double acc);
		void Paused(void);
		BOOL Resume(void);
		BOOL(*CanMove)(int) = nullptr;
		void(*HomeExternalCycle)(void) = nullptr;

	public:
		Motor();
		virtual ~Motor();
	};

	class InterpolationMotor
	{
	public:
		COM::MtCommand::InterpolationParamter m_command;
		static const int maxDevice = (interpolationMAX > 0) ? interpolationMAX : 1;

		int				isNoDevice;
		int				m_no;
		string			m_name;

	public:
		static void CreateInitParameter();
		void CreateParameter(void);

		void SetPulseRate(double pulseRate);

		void SetAxis(int mtno1, int mtno2);
		void SetAxis(int mtno1, int mtno2, int mtno3);
		void SetAxis(int mtno1, int mtno2, int mtno3, int mtno4);
		void SetAxis(int mtno1, int mtno2, int mtno3, int mtno4, int mtno5);

		void SetIndex(int idxno1, int idxno2);
		void SetIndex(int idxno1, int idxno2, int idxno3);
		void SetIndex(int idxno1, int idxno2, int idxno3, int idxno4);
		void SetIndex(int idxno1, int idxno2, int idxno3, int idxno4, int idxno5);

		void SetPos(double pos1, double pos2);
		void SetPos(double pos1, double pos2, double pos3);
		void SetPos(double pos1, double pos2, double pos3, double pos4);
		void SetPos(double pos1, double pos2, double pos3, double pos4, double pos5);
		void SetVel(double percent);

		BOOL IsRdy(void);

		BOOL IsHome(void);
		BOOL IsAlarm(void);
		BOOL IsDriving(void);
		BOOL IsDelayedPosSet(void);

		BOOL Move(void);
		void Stop(void);

		void PosCommandCycle(void);
	public:
		InterpolationMotor();
		virtual ~InterpolationMotor();

	};

	class MtSync
	{
	public:
		INT32 m_master;
		INT32 m_slave;
		INT32 m_groupId;
		INT32 m_wmxErrCode;

		MtSync() {};
		virtual ~MtSync() {}

		BOOL Set(void);
		BOOL SetGroup(void);
		BOOL Resolve(void);
	};
}

#ifndef CHECK_ACTUATOR_ARRAY
extern HW::Motor MT[HW::Motor::maxDevice];
extern HW::InterpolationMotor LN[HW::InterpolationMotor::maxDevice];
extern HW::MtSync GT[HW::Motor::maxDevice];
#else
extern CheckParser<HW::Motor, AxisList, AxisList(HW::Motor::maxDevice)> MT;
extern CheckParser<HW::InterpolationMotor, InterpolationMotorList, InterpolationMotorList(HW::InterpolationMotor::maxDevice)> LN;
#endif