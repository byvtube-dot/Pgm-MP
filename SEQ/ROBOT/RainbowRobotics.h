#pragma once
#include "MODBUS\ModbusTCP.h"
#include "..\..\BASE\BaseAll.h"
#include "..\COM\FSM.h"

using namespace RainbowRobotics;
namespace Robot
{
	static const UINT16 ModbusBitMask[16] =
	{
		0x8000, 0x4000, 0x2000, 0x1000,
		0x0800, 0x0400, 0x0200, 0x0100,
		0x0080, 0x0040, 0x0020, 0x0010,
		0x0008, 0x0004, 0x0002, 0x0001,
	};
	static const UINT16 RobotBitMask[16] =
	{
		0x0001, 0x0002, 0x0004, 0x0008,
		0x0010, 0x0020, 0x0040, 0x0080,
		0x0100, 0x0200, 0x0400, 0x0800,
		0x1000, 0x2000, 0x4000, 0x8000,
	};

	typedef struct
	{
		BYTE		activated : 1;
		BYTE		realMode : 1;
		BYTE		collisionDetected : 1;
		BYTE		armPowerEngaged : 1;
		BYTE		directTeachingMode : 1;
		BYTE		moving : 1;
		BYTE		pauseState : 1;
		BYTE		teachingPendantConnecred : 1;
			
		BYTE		programRun : 1;
		BYTE		noArcModeOn : 1;
		BYTE		emgButtonReleased : 1;
		BYTE		firstProgramRun : 1;
		BYTE		lastProgramLoadDone : 1;
		BYTE		underActivation : 1;
		BYTE		collisionState : 1;
		BYTE		systemSOS : 1;
	
		BYTE		homeIsBegin : 1;
		BYTE		controlBoxHeartBeat : 1;
		BYTE		isHome : 1;
	
		int			collisionThreshold;
		int			speedBarValue;
	
		double		controlBoxAmpere;
		double		tcpMass;
	}RobotState;

	typedef struct
	{
		double		tcpPx;
		double		tcpPy;
		double		tcpPz;
		double		tcpRx;
		double		tcpRy;
		double		tcpRz;
	}RobotTcp;

	typedef struct
	{
		double		joint_0;
		double		joint_1;
		double		joint_2;
		double		joint_3;
		double		joint_4;
		double		joint_5;
	}RobotJoint;

	typedef struct _StateCmd
	{
		CTimer	tmKeepOrder[cmdMax];
		int		cmd[cmdMax];
	}StateCmd;

	typedef struct _RobotIO
	{
		uint16_t		robotDi;
		uint16_t		robotDo;

		uint16_t		memRobotDi;
		uint16_t		memRobotDo;

		CTimer			tmRobotDiOn[robotDiMax];
		CTimer			tmRobotDiOff[robotDiMax];
		CTimer			tmRobotDoOn[robotDoMax];
		CTimer			tmRobotDoOff[robotDoMax];
	}RobotIO;

	typedef struct _RobotPos
	{
		RobotTcp		tcp;
		RobotJoint		joint;
	}RobotPos;

	//typedef struct _ModbusData
	//{
	//	uint16_t		robotDi;
	//	uint16_t		robotDo;

	//	uint16_t		state[28];
	//	uint16_t		CurTcp[6];
	//	uint16_t		CurJoint[6];
	//	uint16_t		StateCmd[cmdMax];
	//	uint16_t		UserAreaRead[userCmdReadMax];
	//	uint16_t		UserAreaWrite[userCmdWriteMax];
	//}ModbusData;

	// 25.12.06 ADD
	typedef struct _ModbusData
	{
		uint16_t		m_regs_0_100[100];		// Di, State
		uint16_t		UserAreaRead[userCmdReadMax];
		uint16_t		m_regs_256_356[100];	// Joint, TCP

		uint16_t		robotDo;
		uint16_t		StateCmd[cmdMax];
		uint16_t		UserAreaWrite[userCmdWriteMax];
	}ModbusData;

	// 25.12.06 ADD
	typedef struct _PrevModbusData
	{
		uint16_t		robotDo;
		uint16_t		StateCmd[cmdMax];
		uint16_t		UserAreaWrite[userCmdWriteMax];
	}PrevModbusData;

	class CRobot
	{
	public:
		enum cmd
		{
			C_HOME_START		= 100,
			C_HOME_SEQ_START	= C_HOME_START,
			C_HOME_SEQ_END		,
			C_HOME_SEARCH_START	,
			C_HOME_SEARCH_END	,
			C_HOME_END			= 199,

			C_MOVE_START		= 300,
			C_MOVE_PAUSED		,
			C_MOVE_END			= 399,
		};
	private:
		ModbusTCP::CModbus*		m_robot;
		ModbusData				m_modbusData;
		PrevModbusData			m_modbusPrevData;

		CThread					m_thread;
		HANDLE					m_hMutex;
		HANDLE					m_hEvent;

		bool					m_shouldReconnect;
		int						m_communicationFailCnt;

		static UINT UpdateThread(LPVOID lpParam);
	public:
		RobotIO					m_robotIo;
		RobotState				m_robotState;
		RobotPos				m_robotCurPos;
		double					m_readCmdPos;
		double					m_writeCmdPos;
		StateCmd				m_StateCmd;

	public:
		static const int maxDevice = (robotMAX > 0) ? robotMAX : 1;
		int						m_robotNo;
		string					m_name;
		List<string>			m_indexName;
		COM::CFSM				m_fsmRobot;
		
		bool					m_isErr;
		bool					m_isHome;

		bool					m_moveRun;
		bool					m_HomeRun;
		int						m_speed;

		int						m_homePosIdx;
		int						m_nxtIdx;
		int						m_curIdx;

		// Standard Log
		string					m_sLogFncAddStart;
		string					m_sLogFncAddEnd;
		string					m_sLogDir;

		CRobot();
		virtual~CRobot();

		static void CreateInitParameter();

		// Standard Log
		void SLogFncAddMessage(string message);
		void SLogStartEvent();
		void SLogEndEvent();

		BOOL Init(LPCWSTR Name, const TCHAR* lpszSlaveIpAddress, USHORT port);
		void RobotErrCheck(void);
		void Update(void);

		BOOL(*CanMove)(int) = nullptr;

		void Activation(void);
		void Deactivation(void);
		void ChangeRealMode(void);
		void ClearSystemSos(void);
		void Pause(void);
		void ResumePause(void);
		void ResumeCollision(void);
		void ProgramOnceStart(void);
		void ProgramRepeatStart(void);
		void ProgramStop(void);
		void SetSpeed(int percent);
		void LoadDefaultProgram(void);
		void LoadProgram(int idx);

		void Move(int idx);
		void Move(int idx, double zOffset, int speed = 0.1);
		BOOL IsRdy(int idx = NOT_DEFINED);
		BOOL JointInPos(int idx, double error = 0.1);
		BOOL TcpInPos(int idx, double error = 0.1);
		BOOL TcpInPos(int idx, RobotTcp idxOffset, double error = 0.1);
		BOOL CmdOrCurIdx(int posIdx);			// 25.10.17
		BOOL CmdOrCurMovingIdx(int posIdx);		// 25.10.17

		BOOL StartHomeCommand(void);
		BOOL BetweenJoint(RobotJoint criterion, RobotJoint Comparison, double error);
		BOOL BetweenTcp(RobotTcp criterion, RobotTcp Comparison, double error = 0.1);

		int&		SpeedTable(int idx);
		RobotTcp&	TcpTable(int idx);
		RobotJoint& JointTable(int idx);

	public:
		BOOL IsOn(RobotDi ioNo, DWORD dwDelayTime = 0);
		BOOL IsOff(RobotDi ioNo, DWORD dwDelayTime = 0);
		void Set(RobotDi ioNo, BOOL isOn);
		BOOL IsOn(RobotDo ioNo, DWORD dwDelayTime = 0);
		BOOL IsOff(RobotDo ioNo, DWORD dwDelayTime = 0);
		void Set(RobotDo ioNo, BOOL isOn);

		BOOL AOn(RobotDi ioNo, DWORD dwDelayTime = 0);
		BOOL AOff(RobotDi ioNo, DWORD dwDelayTime = 0);
		BOOL BOn(RobotDi ioNo, DWORD dwDelayTime = 0);
		BOOL BOff(RobotDi ioNo, DWORD dwDelayTime = 0);
		void On(RobotDo ioNo);
		void Off(RobotDo ioNo);

	private:
		void DebugMode(void);
		void UpdateModbus(void);
		void UpdateRobotIO(void);
		void UpdateRobotSate(void);
		void UpdateRealPos(void);
		void UpdateRobotCmd(void);
		void UpdateUserArea(void);
		void RobotIOTimer(void);
	private:
		void MoveCommandCycle(void);
		void StartHomeSearch(void);
		void CancelHomeSearch(void);
	};
}

extern Robot::CRobot ROBOT[Robot::CRobot::maxDevice];