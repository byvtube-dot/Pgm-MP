#include "..\Includes.h"

using namespace ModbusTCP;
namespace Robot
{
	CRobot::CRobot() : m_robot(NULL)
	{
		for (int i = 0; i < MAX_INDEX_MOTOR; i++)
			m_indexName.Add("");

		ZeroMemory(&m_modbusData, sizeof(ModbusData));

		ZeroMemory(&m_robotState, sizeof(RobotState));
		ZeroMemory(&m_robotCurPos, sizeof(RobotPos));
		ZeroMemory(&m_writeCmdPos, sizeof(RobotPos));
		ZeroMemory(&m_readCmdPos, sizeof(RobotPos));

		m_communicationFailCnt = 0;
		m_shouldReconnect = false;

		m_hMutex = NULL;
		m_hEvent = NULL;

		m_sLogFncAddStart = m_sLogFncAddEnd = "";
	}
	CRobot::~CRobot()
	{
		if (m_hMutex != NULL)
			ReleaseMutex(m_hMutex);
		if (m_hEvent != NULL)
			CloseHandle(m_hEvent);
	}

	UINT CRobot::UpdateThread(LPVOID lpParam)
	{
		CRobot* ptrRobot = reinterpret_cast<CRobot*>(lpParam);
		DWORD interval = 1;

		bool onceCycle = false;
		int	cycleCnt = 0;
		double maxCycleTime = 0;
		CTimer tmOneTime = CTimer(TRUE);
		CTimer tmAvgTime = CTimer(TRUE);

		while (!CMain::isClose)
		{
			ptrRobot->UpdateModbus();
			Sleep(interval);

			cycleCnt++;
			double dtmOneTime = tmOneTime.Elapsed();
			if (maxCycleTime < dtmOneTime)
				maxCycleTime = dtmOneTime;

			if (10000 <= cycleCnt)
			{
				double mSec = tmAvgTime.Elapsed() / 10000.;
				if (onceCycle)
				{
					LOG[logSYS].Message(ptrRobot->m_name + " modbus loop avg time " + ToString(mSec, 3) +
						" msec, max time " + ToString(maxCycleTime, 3) + " msec, 1-loop time during 10000-loop");
				}

				onceCycle = 1;
				cycleCnt = 0;
				maxCycleTime = 0.;
				tmAvgTime.Reset();
			}
			tmOneTime.Reset();
		}

		if (NULL != ptrRobot->m_hEvent)
			SetEvent(ptrRobot->m_hEvent);

		CMain::isClose = true;
		return 0;
	}

	void CRobot::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotIndexName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < RainbowRobotics::robotMAX; i++)
		{
			RainbowRobotics::RobotList no = RainbowRobotics::RobotList(i);
			ROBOT[no].m_robotNo = i;

			if (!CanWrite)
				continue;

			CIni ini = { 0 };
			TCHAR strSection[1024] = { 0 };
			TCHAR strKeyCode[1024] = { 0 };
			TCHAR strStrName[1024] = { 0 };
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotName.ini");
			swprintf_s(strKeyCode, 100, L"ROBOT%d", i);
			ini.GetString(L"ROBOTNAME", strKeyCode, strStrName, 512);
			ROBOT[no].m_name = GetAscCode(strStrName);

			for (int indexNo = 0; indexNo < MAX_INDEX_MOTOR; indexNo++)
			{
				ZeroMemory(strSection, sizeof(strSection));
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotIndexName.ini");
				swprintf_s(strSection, 100, L"ROBOTINDEXNAME%d", no);
				swprintf_s(strKeyCode, 100, L"ROBOTINDEXNAME%d", indexNo);
				ini.GetString(strSection, strKeyCode, strStrName, 512);
				ROBOT[no].m_indexName[indexNo] = GetAscCode(strStrName);
			}
		}
	}

	void CRobot::SLogFncAddMessage(string message)
	{
		m_sLogFncAddStart = message;
	}

	void CRobot::SLogStartEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(RobotList(m_robotNo)) && SLOG[no].robotLogStart[m_robotNo].m_uniqueXFRNumber)
			{
				if (!Between(m_nxtIdx, 0, MAX_INDEX_MOTOR))
					continue;

				SMESSAGE.Function(SLOG[no].robotLogStart[m_robotNo].GetXFRDeviceId().c_str(),
					(SLOG[no].robotLogStart[m_robotNo].GetXFREventId() + "_MOVE_" + m_indexName[m_nxtIdx] + m_sLogFncAddStart).c_str(), "START",
					SLOG[no].robotLogStart[m_robotNo].GetXFRMaterialId().c_str(),
					SLOG[no].robotLogStart[m_robotNo].GetXFRMaterialType().c_str(),
					"'TYPE'", "'ROBOT'",
					"'ACT_NAME'", ("'R" + ToString(m_robotNo) + "'").c_str(),
					"'INDEX'", ToString(m_nxtIdx).c_str(),
					"'SPEED'", ToString(m_robotState.speedBarValue).c_str());
				SLOG[no].robotLogEnd[m_robotNo] = SLOG[no].robotLogStart[m_robotNo];
			}
		}
		m_sLogFncAddEnd = m_sLogFncAddStart;
		m_sLogFncAddStart = "";
	}

	void CRobot::SLogEndEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].robotLogEnd[m_robotNo].m_uniqueXFRNumber)
			{
				if (!Between(m_nxtIdx, 0, MAX_INDEX_MOTOR))
					continue;

				SMESSAGE.Function(SLOG[no].robotLogEnd[m_robotNo].GetXFRDeviceId().c_str(),
					(SLOG[no].robotLogEnd[m_robotNo].GetXFREventId() + "_MOVE_" + m_indexName[m_nxtIdx] + m_sLogFncAddEnd).c_str(), "END",
					SLOG[no].robotLogEnd[m_robotNo].GetXFRMaterialId().c_str(),
					SLOG[no].robotLogEnd[m_robotNo].GetXFRMaterialType().c_str(),
					"'TYPE'", "'ROBOT'",
					"'ACT_NAME'", ("'R" + ToString(m_robotNo) + "'").c_str(),
					"'INDEX'", ToString(m_nxtIdx).c_str(),
					"'SPEED'", ToString(m_robotState.speedBarValue).c_str());
				SLOG[no].robotLogEnd[m_robotNo].End(TRUE);
			}
		}
		m_sLogFncAddEnd = "";
	}

	BOOL CRobot::Init(LPCWSTR Name, const TCHAR* lpszSlaveIpAddress, USHORT port)
	{
		m_robot = new CModbus(lpszSlaveIpAddress, port);

		if (m_robot->m_modbus->m_ctx == NULL)
			return (FALSE);

		WCHAR tmpMutexName[MAX_PATH];
		WCHAR tmpEventName[MAX_PATH];

		swprintf_s(tmpMutexName, L"%s_Mutex", Name);
		swprintf_s(tmpEventName, L"%s_Event", Name);

		m_hMutex = CreateMutex(NULL, FALSE, tmpMutexName);
		if (NULL == m_hMutex)
			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, tmpEventName);
		if (NULL == m_hEvent)
			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);

		if (!m_thread.Run(NULL, (LPTHREAD_START_ROUTINE)UpdateThread, LPVOID(this), NULL))
			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);

		ReleaseMutex(m_hMutex);

		return (TRUE);
	}

	void CRobot::DebugMode(void)
	{
		ROBOT[robotLt].m_robotState.activated = TRUE;
		ROBOT[robotLt].m_robotState.realMode = TRUE;
		ROBOT[robotLt].m_robotState.programRun = TRUE;

		ROBOT[robotRt].m_robotState.activated = TRUE;
		ROBOT[robotRt].m_robotState.realMode = TRUE;
		ROBOT[robotRt].m_robotState.programRun = TRUE;
	}

	void CRobot::RobotErrCheck(void)
	{
		if (!m_robotState.emgButtonReleased)	ER.Save(ErrName(ER_ROBOT_EMG + m_robotNo + 1));
		else									ER.Del(ErrName(ER_ROBOT_EMG + m_robotNo + 1));

		if (m_robotState.systemSOS)				ER.Save(ErrName(ER_ROBOT_SYSTEM_SOS + m_robotNo + 1));
		else									ER.Del(ErrName(ER_ROBOT_SYSTEM_SOS + m_robotNo + 1));

		if (m_robotState.collisionDetected)		ER.Save(ErrName(ER_ROBOT_COLLISION + m_robotNo + 1));
		else									ER.Del(ErrName(ER_ROBOT_COLLISION + m_robotNo + 1));

		if (m_robotState.pauseState)			WR.Save(WrName(WR_ROBOT_PROGRAM_PAUSE + m_robotNo + 1));
		else									WR.Del(WrName(WR_ROBOT_PROGRAM_PAUSE + m_robotNo + 1));

		if (!m_robotState.activated)			WR.Save(WrName(WR_ROBOT_NOT_ACTIVATED + m_robotNo + 1));
		else									WR.Del(WrName(WR_ROBOT_NOT_ACTIVATED + m_robotNo + 1));

		if (!m_robotState.realMode)				WR.Save(WrName(WR_ROBOT_NOT_REAL_MODE + m_robotNo + 1));
		else									WR.Del(WrName(WR_ROBOT_NOT_REAL_MODE + m_robotNo + 1));

		if (!m_robotState.programRun)			WR.Save(WrName(WR_ROBOT_NOT_PROGRAM_RUN + m_robotNo + 1));
		else									WR.Del(WrName(WR_ROBOT_NOT_PROGRAM_RUN + m_robotNo + 1));

		if (m_robotState.directTeachingMode)	WR.Save(WrName(WR_ROBOT_DIRECT_TEATCH + m_robotNo + 1));
		else									WR.Del(WrName(WR_ROBOT_DIRECT_TEATCH + m_robotNo + 1));

		if (!m_robotState.emgButtonReleased || m_robotState.systemSOS || m_robotState.collisionDetected)
			m_isErr = true;
		else
			m_isErr = false;

		if (OPR.isAuto)
		{
			if (!m_robotState.activated)
				Activation();
			else if (!m_robotState.realMode)
				ChangeRealMode();
			else if (!m_robotState.programRun)
				ProgramRepeatStart();
		}

		if (!m_robotState.programRun)
			SetSpeed(0);

	}

	void CRobot::Update(void)
	{
		RobotErrCheck();

		MoveCommandCycle();
		StartHomeSearch();

		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex, 0))
		{
			ResetEvent(m_hEvent);

			if (m_robot->m_modbus->m_connected)
			{
				UpdateRobotIO();
				UpdateRobotSate();
				UpdateRealPos();
				UpdateRobotCmd();
				UpdateUserArea();
			}
			else
			{
				if (OPR.isNoDevice)
				{
					UpdateRobotSate();
				}
			}
			if (ReleaseMutex(m_hMutex))
				SetEvent(m_hEvent);
		}
	}

	void CRobot::UpdateModbus(void)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent, 0))
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex, 0))
			{
				char result[MAX_PATH] = { 0 };
				int	DataCnt = 0;
				uint16_t* Data = NULL;

				if (m_robot->m_modbus->m_connected)
				{
					if (m_shouldReconnect)
					{
						m_robot->CloseModbus();
						Sleep(100);
					}
					else
					{
						bool isCommunication = true;
						// Read 
						Data = m_modbusData.m_regs_0_100;
						DataCnt = sizeof(m_modbusData.m_regs_0_100) / sizeof(uint16_t);
						ZeroMemory(result, sizeof(result));
						if (!m_robot->ReadReg(Data, DataCnt, 0, result))
						{
							LOG[logROBOT].Message("[%s], [MBus Read Robot Address 0 ~ 100 Fail: %s]", m_name.c_str(), result);
							isCommunication &= false;
							m_communicationFailCnt++;
						}

						Data = m_modbusData.m_regs_256_356;
						DataCnt = sizeof(m_modbusData.m_regs_256_356) / sizeof(uint16_t);
						ZeroMemory(result, sizeof(result));
						if (!m_robot->ReadReg(Data, DataCnt, 256, result))
						{
							LOG[logROBOT].Message("[%s], [MBus Read Robot Address 256 ~ 356 Fail: %s]", m_name.c_str(), result);
							isCommunication &= false;
							m_communicationFailCnt++;
						}

						Data = m_modbusData.UserAreaRead;
						DataCnt = sizeof(m_modbusData.UserAreaRead) / sizeof(uint16_t);
						ZeroMemory(result, sizeof(result));
						if (!m_robot->ReadReg(Data, DataCnt, AddressUserCmdRd, result))
						{
							LOG[logROBOT].Message("[%s], [MBus Read Robot User Area: %s]", m_name.c_str(), result);
							isCommunication &= false;
							m_communicationFailCnt++;
						}

						// Write
						Data = &m_modbusData.robotDo;
						DataCnt = sizeof(m_modbusData.robotDo) / sizeof(uint16_t);
						ZeroMemory(result, sizeof(result));
						if (memcmp(&m_modbusData.robotDo, &m_modbusPrevData.robotDo, sizeof(m_modbusData.robotDo)) != 0)
						{
							if (!m_robot->WriteReg(Data, DataCnt, AddressDo, result))
							{
								LOG[logROBOT].Message("[%s], [MBus Write Robot DO Fail: %s]", m_name.c_str(), result);
								isCommunication &= false;
								m_communicationFailCnt++;
							}
							else
								memcpy(&m_modbusPrevData.robotDo, &m_modbusData.robotDo, sizeof(m_modbusPrevData.robotDo));
						}

						Data = m_modbusData.StateCmd;
						DataCnt = sizeof(m_modbusData.StateCmd) / sizeof(uint16_t);
						ZeroMemory(result, sizeof(result));
						if (memcmp(Data, &m_modbusPrevData.StateCmd, sizeof(m_modbusData.StateCmd)) != 0)
						{
							if (!m_robot->WriteReg(Data, DataCnt, AddressCmd, result))
							{
								LOG[logROBOT].Message("[%s], [MBus Write Robot CMD Fail: %s]", m_name.c_str(), result);
								isCommunication &= false;
								m_communicationFailCnt++;
							}
							else
								memcpy(&m_modbusPrevData.StateCmd, &m_modbusData.StateCmd, sizeof(m_modbusPrevData.StateCmd));
						}
						

						Data = m_modbusData.UserAreaWrite;
						DataCnt = sizeof(m_modbusData.UserAreaWrite) / sizeof(uint16_t);
						ZeroMemory(result, sizeof(result));
						if (memcmp(m_modbusData.UserAreaWrite, &m_modbusPrevData.UserAreaWrite, sizeof(m_modbusData.UserAreaWrite)) != 0)
						{
							if (!m_robot->WriteReg(Data, DataCnt, AddressUserCmdWr, result))
							{
								LOG[logROBOT].Message("[%s], [MBus Write Robot User Area: %s]", m_name.c_str(), result);
								isCommunication &= false;
								m_communicationFailCnt++;
							}
							else
								memcpy(&m_modbusPrevData.UserAreaWrite, &m_modbusData.UserAreaWrite, sizeof(m_modbusPrevData.UserAreaWrite));
						}

						if (isCommunication)
							m_communicationFailCnt = 0;
						else if (m_communicationFailCnt > 50)
							m_shouldReconnect = true;
					}
				}
				else
				{
					ZeroMemory(result, sizeof(result));
					if (m_robot->OpenModbus(result))
					{
						m_communicationFailCnt = 0;
						m_shouldReconnect = false;
					}
					else
					{
						LOG[logROBOT].Message("[%s], [Modbus Open Fail : %s]", m_name.c_str(), result);
						Sleep(1000);
					}
				}

				ReleaseMutex(m_hMutex);
			}
		}
	}

	void CRobot::MoveCommandCycle(void)
	{
		if (!m_fsmRobot.Between(C_MOVE_START, C_MOVE_END))
			return;

		if (m_fsmRobot.TimeLimit(_40sec))
		{
			ER.Save((ErrName)(ER_LT_COBOT_MOVE_CYCLE_TIMEOVER + m_robotNo));
			m_fsmRobot.Set(C_IDLE);
			return;
		}

		switch (m_fsmRobot.Get())
		{
		case C_MOVE_START:
		{
			if (m_speed != NOT_DEFINED)
			{
				if (!Between(m_robotState.speedBarValue, m_speed - 1, m_speed + 1))
				{
					SetSpeed(m_speed);
					break;
				}
			}

			m_modbusData.UserAreaWrite[userCmdWriteNxtIdx] = static_cast<uint16_t>(m_nxtIdx);
			
			if (!OPR.isNoDevice)
			{
				m_moveRun = true;
				if (m_robotState.moving)
				{
					SLogEndEvent();
					SLogStartEvent();
					m_fsmRobot.Set(C_MOVE_END);
				}
			}
			else
				m_fsmRobot.Set(C_MOVE_END);
		}
			break;
		case C_MOVE_PAUSED:
			SLogEndEvent();
			m_fsmRobot.Set(C_IDLE);
			break;
		case C_MOVE_END:
			if (!OPR.isNoDevice)
			{
				if (m_robotState.moving || m_modbusData.UserAreaRead[userCmdReadCurIdx] != (uint16_t)m_nxtIdx)
					break;
			}
			
			SLogEndEvent();

			m_moveRun = false;
			m_curIdx = m_nxtIdx;
			m_fsmRobot.Set(C_IDLE);
			break;
		}
	}

	void CRobot::StartHomeSearch(void)
	{
		if (!m_fsmRobot.Between(C_HOME_START, C_HOME_END))
			return;

		if (m_fsmRobot.TimeLimit(150 * _1sec))
		{
			ER.Save((ErrName)(ER_ROBOT_HOME_TM_OVER + m_robotNo + 1));
			CancelHomeSearch();
			return;
		}

		if (m_robotState.collisionDetected || !m_robotState.emgButtonReleased || m_robotState.systemSOS)
		{
			CancelHomeSearch();
			return;
		}

		switch (m_fsmRobot.Get())
		{
		case C_HOME_START:
			if (m_fsmRobot.Once())
			{
				m_HomeRun = false;
				m_isHome = false;
			}

			if (!m_robot->m_modbus->m_connected)
			{
				m_fsmRobot.Set(C_IDLE);
				break;
			}
			if (CanMove(HOME_CANMOVE))
			{
				m_HomeRun = true;
				m_fsmRobot.Set(C_HOME_END);
			}
			break;
		case C_HOME_END:

			if (m_robotState.moving || !m_robotState.homeIsBegin)
				break;

			m_fsmRobot.Set(C_IDLE);
			m_isHome = true;
			m_curIdx = 0;
			m_nxtIdx = 0;

			break;
		}
	}

	void CRobot::CancelHomeSearch(void)
	{
		if (m_robotState.moving || m_robotState.programRun)
		{
			m_HomeRun = false;
			ProgramStop();

			m_curIdx = P_JOG;
			m_nxtIdx = P_JOG;
			m_isHome = false;
			return;
		}

		m_fsmRobot.Set(C_IDLE);
	}


	void CRobot::UpdateRobotIO(void)
	{
		RobotIOTimer();

		uint16_t DiTemp = 0x0000;
		for (int DiNo = iRobot00; DiNo < robotDiMax; DiNo++)
		{
			if (m_modbusData.m_regs_0_100[addressDi] & ModbusBitMask[DiNo])
				DiTemp |= RobotBitMask[DiNo];
		}
		memcpy(&m_robotIo.robotDi, &DiTemp, sizeof(uint16_t));

		uint16_t DoTemp = 0x0000;
		for (int DoNo = oRobot00; DoNo < robotDoMax; DoNo++)
		{
			if (m_robotIo.robotDo & RobotBitMask[DoNo])
				DoTemp |= ModbusBitMask[DoNo];
		}
		memcpy(&m_modbusData.robotDo, &DoTemp, sizeof(uint16_t));
	}

	void CRobot::UpdateRobotSate(void)
	{
		m_robotState.activated = m_modbusData.m_regs_0_100[addressActivated];
		m_robotState.realMode = m_modbusData.m_regs_0_100[addressRealMode];
		m_robotState.collisionDetected = m_modbusData.m_regs_0_100[addressCollisionDetected];
		m_robotState.armPowerEngaged = m_modbusData.m_regs_0_100[addressArmPowerEngaged];
		m_robotState.directTeachingMode = m_modbusData.m_regs_0_100[addressDirectTeachingMode];
		m_robotState.moving = m_modbusData.m_regs_0_100[addressMoving];
		m_robotState.pauseState = m_modbusData.m_regs_0_100[addressPauseState];
		m_robotState.teachingPendantConnecred = m_modbusData.m_regs_0_100[addressTeachingPendantConnecred];

		m_robotState.programRun = m_modbusData.m_regs_0_100[addressProgramRun];
		m_robotState.noArcModeOn = m_modbusData.m_regs_0_100[addressNoArcModeOn];
		m_robotState.emgButtonReleased = m_modbusData.m_regs_0_100[addressEmgButtonReleased];
		m_robotState.firstProgramRun = m_modbusData.m_regs_0_100[addressFirstProgramRun];
		m_robotState.lastProgramLoadDone = m_modbusData.m_regs_0_100[addressLastProgramLoadDone];
		m_robotState.controlBoxAmpere = static_cast<double> (m_modbusData.m_regs_0_100[addressControlBoxAmpere]) * 0.02;
		m_robotState.underActivation = m_modbusData.m_regs_0_100[addressUnderActivation];
		m_robotState.collisionState = m_modbusData.m_regs_0_100[addressCollisionState];
		m_robotState.collisionThreshold = static_cast<int> (m_modbusData.m_regs_0_100[addressCollisionThreshold]);
		
		m_robotState.tcpMass = static_cast<double> (m_modbusData.m_regs_0_100[addressTcpMass]) * 0.1;
		m_robotState.systemSOS = m_modbusData.m_regs_0_100[addressSystemSOS];
		m_robotState.homeIsBegin = m_modbusData.m_regs_0_100[addressHomeIsBegin];
		m_robotState.controlBoxHeartBeat = m_modbusData.m_regs_0_100[addressControlBoxHeartBeat];
		m_robotState.speedBarValue = static_cast<int> (m_modbusData.m_regs_0_100[addressSpeedBarValue]);


		if (OPR.isNoDevice)
		{
			m_robotState.speedBarValue = m_StateCmd.cmd[cmdSpeedBarChage];
			m_robotState.emgButtonReleased = true;
		}
	}

	void CRobot::UpdateRealPos(void)
	{
		RobotPos responsePos;

		if (m_modbusData.m_regs_256_356[addressTCPCurrent0] & 0x8000)	responsePos.tcp.tcpPx = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressTCPCurrent0]) * 0.1;
		else															responsePos.tcp.tcpPx = static_cast<double>(m_modbusData.m_regs_256_356[addressTCPCurrent0]) * 0.1;
		if (m_modbusData.m_regs_256_356[addressTCPCurrent1] & 0x8000)	responsePos.tcp.tcpPy = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressTCPCurrent1]) * 0.1;
		else															responsePos.tcp.tcpPy = static_cast<double>(m_modbusData.m_regs_256_356[addressTCPCurrent1]) * 0.1;
		if (m_modbusData.m_regs_256_356[addressTCPCurrent2] & 0x8000)	responsePos.tcp.tcpPz = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressTCPCurrent2]) * 0.1;
		else															responsePos.tcp.tcpPz = static_cast<double>(m_modbusData.m_regs_256_356[addressTCPCurrent2]) * 0.1;
		if (m_modbusData.m_regs_256_356[addressTCPCurrent3] & 0x8000)	responsePos.tcp.tcpRx = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressTCPCurrent3]) * 0.02;
		else															responsePos.tcp.tcpRx = static_cast<double>(m_modbusData.m_regs_256_356[addressTCPCurrent3]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressTCPCurrent4] & 0x8000)	responsePos.tcp.tcpRy = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressTCPCurrent4]) * 0.02;
		else															responsePos.tcp.tcpRy = static_cast<double>(m_modbusData.m_regs_256_356[addressTCPCurrent4]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressTCPCurrent5] & 0x8000)	responsePos.tcp.tcpRz = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressTCPCurrent5]) * 0.02;
		else															responsePos.tcp.tcpRz = static_cast<double>(m_modbusData.m_regs_256_356[addressTCPCurrent5]) * 0.02;

		if (m_modbusData.m_regs_256_356[addressJointCurrent0] & 0x8000)	responsePos.joint.joint_0 = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressJointCurrent0]) * 0.02;
		else															responsePos.joint.joint_0 = static_cast<double>(m_modbusData.m_regs_256_356[addressJointCurrent0]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressJointCurrent1] & 0x8000)	responsePos.joint.joint_1 = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressJointCurrent1]) * 0.02;
		else															responsePos.joint.joint_1 = static_cast<double>(m_modbusData.m_regs_256_356[addressJointCurrent1]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressJointCurrent2] & 0x8000)	responsePos.joint.joint_2 = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressJointCurrent2]) * 0.02;
		else															responsePos.joint.joint_2 = static_cast<double>(m_modbusData.m_regs_256_356[addressJointCurrent2]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressJointCurrent3] & 0x8000)	responsePos.joint.joint_3 = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressJointCurrent3]) * 0.02;
		else															responsePos.joint.joint_3 = static_cast<double>(m_modbusData.m_regs_256_356[addressJointCurrent3]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressJointCurrent4] & 0x8000)	responsePos.joint.joint_4 = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressJointCurrent4]) * 0.02;
		else															responsePos.joint.joint_4 = static_cast<double>(m_modbusData.m_regs_256_356[addressJointCurrent4]) * 0.02;
		if (m_modbusData.m_regs_256_356[addressJointCurrent5] & 0x8000)	responsePos.joint.joint_5 = static_cast<double>((int16_t)m_modbusData.m_regs_256_356[addressJointCurrent5]) * 0.02;
		else															responsePos.joint.joint_5 = static_cast<double>(m_modbusData.m_regs_256_356[addressJointCurrent5]) * 0.02;

		m_robotCurPos.joint.joint_0 = round(responsePos.joint.joint_0 * 10.) / 10.;
		m_robotCurPos.joint.joint_1 = round(responsePos.joint.joint_1 * 10.) / 10.;
		m_robotCurPos.joint.joint_2 = round(responsePos.joint.joint_2 * 10.) / 10.;
		m_robotCurPos.joint.joint_3 = round(responsePos.joint.joint_3 * 10.) / 10.;
		m_robotCurPos.joint.joint_4 = round(responsePos.joint.joint_4 * 10.) / 10.;
		m_robotCurPos.joint.joint_5 = round(responsePos.joint.joint_5 * 10.) / 10.;

		m_robotCurPos.tcp.tcpPx = round(responsePos.tcp.tcpPx * 10.) / 10.;
		m_robotCurPos.tcp.tcpPy = round(responsePos.tcp.tcpPy * 10.) / 10.;
		m_robotCurPos.tcp.tcpPz = round(responsePos.tcp.tcpPz * 10.) / 10.;
		m_robotCurPos.tcp.tcpRx = round(responsePos.tcp.tcpRx * 10.) / 10.;
		m_robotCurPos.tcp.tcpRy = round(responsePos.tcp.tcpRy * 10.) / 10.;
		m_robotCurPos.tcp.tcpRz = round(responsePos.tcp.tcpRz * 10.) / 10.;
	}

	void CRobot::UpdateUserArea(void)
	{
		double responseCmdPos;

		if (m_modbusData.UserAreaRead[userCmdReadTcpPzOffset] & 0x8000)	responseCmdPos = static_cast<double>((int16_t)m_modbusData.UserAreaRead[userCmdReadTcpPzOffset]) * 0.1;
		else															responseCmdPos = static_cast<double>(m_modbusData.UserAreaRead[userCmdReadTcpPzOffset]) * 0.1;
		
		m_readCmdPos = round(responseCmdPos *10.)/10.;
		m_modbusData.UserAreaWrite[userCmdWriteTcpPzOffset] = static_cast<uint16_t>((int)(m_writeCmdPos * 10));

		if (m_moveRun)
		{
			if (!m_fsmRobot.Between(C_MOVE_START, C_MOVE_END))
				m_moveRun = false;
		}

		if (m_HomeRun)
		{
			if (!m_fsmRobot.Between(C_HOME_START, C_HOME_END))
				m_HomeRun = false;
		}

		m_modbusData.UserAreaWrite[userCmdWriteMove] = static_cast<uint16_t>(m_moveRun);
		m_modbusData.UserAreaWrite[userCmdWriteHome] = static_cast<uint16_t>(m_HomeRun);
	}

	void CRobot::UpdateRobotCmd(void)
	{
		for (int cmdNo = 0; cmdNo < cmdMax; cmdNo++)
		{
			if (cmdSpeedBarChage != cmdNo)
			{
				if (cmdIdle < m_StateCmd.cmd[cmdNo])
				{
					if (m_StateCmd.tmKeepOrder[cmdNo].TmOver(1000))
						m_StateCmd.cmd[cmdNo] = cmdIdle;
				}
				else
					m_StateCmd.tmKeepOrder[cmdNo].Reset();
			}

			m_modbusData.StateCmd[cmdNo] = static_cast<uint16_t>(m_StateCmd.cmd[cmdNo]);
		}
	}
}

Robot::CRobot ROBOT[Robot::CRobot::maxDevice];