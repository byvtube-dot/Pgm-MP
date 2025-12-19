#include "..\Includes.h"

namespace HW
{
	Motor::Motor()
	{
		for (int i = 0; i < MAX_INDEX_MOTOR; i++)
			m_indexName.Add("");

		m_name = "";
		m_no = NOT_DEFINED;
		m_brakeNo = NOT_DEFINED;
		isVirtualSW = (motorMAX > 0) ? 0 : 1;
		isVirtualHW = (FALSE);
		m_distLStoIndexPulse = 0;
		m_motionMode = 0;
		m_sLogFncAddStart = m_sLogFncAddEnd = "";
		m_sLogDir = "";
	}

	Motor::~Motor()
	{

	}

	BOOL Motor::DefaultCanMove(int cmd)
	{
		if (OPR.isDoorOpen || OPR.isDoorUnlock)
		{
			if (OPR.isCanMoveMsg)
			{
				MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Door Is Open.");
				return (FALSE);
			}
		}
		return (TRUE);
	}

	void Motor::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorIndexName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList no = AxisList(i);
			MT[no].isVirtualSW = (FALSE);
			MT[no].isVirtualHW = (FALSE);
			MT[no].m_no = NV.motTable[i].nMtNo = i;
			MT[no].CanMove = Motor::DefaultCanMove;
			MT[no].HomeExternalCycle = NULL;
			MT[no].m_moveStroke = NOT_DEFINED;
			MT[no].m_maxVel = 10;
			MT[no].m_maxAcc = 10;
			MT[no].m_maxJerkAcc = 100;
			MT[no].m_usePause = (FALSE);
			MT[no].m_homePosIdx = 0;
			MT[no].m_inposBand = 0.05;
			MT[no].m_jerkRatio = 0.5;
			MT[no].m_pulseRate = _1000PUSLE_RATE;

			if (!CanWrite)
				continue;
			
			ZeroMemory(strSection, sizeof(strSection));
			ZeroMemory(strKeyCode, sizeof(strKeyCode));
			ZeroMemory(strStrName, sizeof(strStrName));
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorName.ini");
			swprintf_s(strKeyCode, 100, L"AXIS%d", no);
			ini.GetString(L"AXISNAME", strKeyCode, strStrName, 512);
			MT[no].m_name = GetAscCode(strStrName);
			for (int indexNo = 0; indexNo < MAX_INDEX_MOTOR; indexNo++)
			{
				ZeroMemory(strSection, sizeof(strSection));
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorIndexName.ini");
				swprintf_s(strSection, 100, L"INDEXNAME%d", no);
				swprintf_s(strKeyCode, 100, L"INDEXNAME%d", indexNo);
				ini.GetString(strSection, strKeyCode, strStrName, 512);
				MT[no].m_indexName[indexNo] = GetAscCode(strStrName);
			}			
		}
	}

	void Motor::SLogFncAddMessage(string message)
	{
		m_sLogFncAddStart = message;
	}

	void Motor::SLogStartEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(AxisList(m_no)) && SLOG[no].axisLogStart[m_no].m_uniqueXFRNumber)
			{
				if (!Between(m_nxtIdx, 0, MAX_INDEX_MOTOR))
					continue;

				SMESSAGE.Function(SLOG[no].axisLogStart[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].axisLogStart[m_no].GetXFREventId() + "_MOVE_" + m_indexName[m_nxtIdx] + m_sLogFncAddStart).c_str(), "START",
					SLOG[no].axisLogStart[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].axisLogStart[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'MOTOR'",
					"'ACT_NAME'", ("'M" + ToString(m_no) + "'").c_str(),
					("'POS_" + m_sLogDir + "'").c_str(), ToString(m_command.pos, 3).c_str(),
					("'VEL_" + m_sLogDir + "'").c_str(), ToString(m_command.profile.vel, 3).c_str(),
					("'ACC_" + m_sLogDir + "'").c_str(), ToString(m_command.profile.acc, 3).c_str());
				SLOG[no].axisLogEnd[m_no] = SLOG[no].axisLogStart[m_no];
			}
		}
		m_sLogFncAddEnd = m_sLogFncAddStart;
		m_sLogFncAddStart = "";
	}

	void Motor::SLogEndEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].axisLogEnd[m_no].m_uniqueXFRNumber)
			{
				if (!Between(m_nxtIdx, 0, MAX_INDEX_MOTOR))
					continue;

				SMESSAGE.Function(SLOG[no].axisLogEnd[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].axisLogEnd[m_no].GetXFREventId() + "_MOVE_" + m_indexName[m_nxtIdx] + m_sLogFncAddEnd).c_str(), "END",
					SLOG[no].axisLogEnd[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].axisLogEnd[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'MOTOR'",
					"'ACT_NAME'", ("'M" + ToString(m_no) + "'").c_str(),
					("'POS_" + m_sLogDir + "'").c_str(), ToString(m_command.pos, 3).c_str(),
					("'VEL_" + m_sLogDir + "'").c_str(), ToString(m_command.profile.vel, 3).c_str(),
					("'ACC_" + m_sLogDir + "'").c_str(), ToString(m_command.profile.acc, 3).c_str());
				SLOG[no].axisLogEnd[m_no].End(TRUE);
			}
		}
		m_sLogFncAddEnd = "";
	}

	MtCommand::SingleParamter Motor::CommandTable(int idx)
	{
		if (!Between(m_no, 0, motorMAX - 1) || !Between(idx, 0, MAX_INDEX_MOTOR - 1))
		{
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);
		}

		MtCommand::SingleParamter command;
		command.no = m_no;
		command.idx = idx;
		command.pos = PosTable(idx);
		command.startPos = m_state.cmdCnt;
		command.limitP = m_limitP;
		command.limitN = m_limitN;
		command.maxVel = m_maxVel;
		command.maxAcc = m_maxAcc;
		command.maxJerkAcc = m_maxJerkAcc;
		command.pulseRate = m_pulseRate;

		command.profile.vel = round(VelTable(idx) * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		command.profile.acc = round(AccTable(idx) * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		command.profile.jerkAcc = m_maxJerkAcc;
		command.profile.jerkRatio = m_jerkRatio;
		command.profile.startingVelocity = 0.;
		command.profile.endVelocity = 0.;

		return command;
	}

	void Motor::GetState(void)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		m_state.isAlarm = !!WMX.status.axesStatus[m_no].ampAlarm;
		m_state.isDriving = !!WMX.status.axesStatus[m_no].opState;
		m_state.cmdCnt = WMX.PulseToMM(WMX.status.axesStatus[m_no].posCmd, m_pulseRate);
		m_state.realCnt = WMX.PulseToMM(WMX.status.axesStatus[m_no].actualPos, m_pulseRate);
		m_state.realVel = WMX.PulseToMM(WMX.status.axesStatus[m_no].actualVelocity, m_pulseRate);
		m_state.realTrq = WMX.status.axesStatus[m_no].actualTorque;
		m_state.isHwCwSen = WMX.status.axesStatus[m_no].positiveLS;
		m_state.isHwCCwSen = WMX.status.axesStatus[m_no].negativeLS;
		m_state.isSwCwSen = WMX.status.axesStatus[m_no].positiveSoftLimit;
		m_state.isSwCCwSen = WMX.status.axesStatus[m_no].negativeSoftLimit;
		m_state.isHomeSen = WMX.status.axesStatus[m_no].homeSwitch;
		m_state.isServoOn = WMX.status.axesStatus[m_no].servoOn;
		m_state.isBrakeOn = WMX.IsBrakeOn(m_brakeNo);
		m_state.isPosSet = WMX.status.axesStatus[m_no].posSet;
		m_state.isDelayedPosSet = WMX.status.axesStatus[m_no].delayedPosSet;
		m_state.isServoRdy = !!(m_state.isServoOn && !m_state.isAlarm);

		if (OPR.isDebugMode)
			return;

		if (!Between(m_homePosIdx, 0, MAX_INDEX_MOTOR - 1))
			ER.Save(ER_HOME_INDEX_SETTING);

		if (m_state.isServoRdy)
		{
			BrakeOff();
		}
		else
		{
			if (m_fsmDrv.IsBusy())
				CancelHomeSearch();

			m_state.isHome = false;
		}

		if (!m_state.isHome)
			m_state.isPaused = false;

		m_isRdy.Reset(!IsRdy());
	}

	void Motor::Stop(void)
	{
		if (isVirtualSW)
			return;

		if (m_motionMode ||
			m_fsmDrv.IsBusy(C_MOTION_BUSY) ||
			m_fsmDrv.IsBusy(C_POS_LINEAR) ||
			m_fsmDrv.Between(C_HOME_START, C_HOME_END))
		{
			CancelHomeSearch();
			return;
		}

		m_state.isPaused = false;
		AxisCommandMode::T	commandMode;
		WMX.coremotion.axisControl->GetAxisCommandMode(m_no, &commandMode);
		if (AxisCommandMode::Torque == commandMode)
			WMX.coremotion.torque->StopTrq(m_no);
		else if (AxisCommandMode::Velocity == commandMode)
			WMX.coremotion.velocity->ExecQuickStop(m_no);
		else
		{
			WMX.coremotion.motion->ExecQuickStop(m_no);
			//m_curIdx = P_JOG;
			//m_nxtIdx = P_JOG;
		}
	}

	BOOL Motor::StartHomeCommand(void)
	{
		if (isVirtualSW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_motionMode)
			return (FALSE);

		if (!WMX.status.axesStatus[m_no].commandReady)
			return (FALSE);

		m_state.isPaused = false;
		m_state.isHome = false;
		m_fsmDrv.Set(C_HOME_SEQ_START);
		return (TRUE);
	}

	void Motor::CancelHomeSearch(void)
	{
		AxisCommandMode::T	commandMode;
		WMX.coremotion.axisControl->GetAxisCommandMode(m_no, &commandMode);
		if (AxisCommandMode::Torque == commandMode)
			WMX.coremotion.torque->StopTrq(m_no);
		else if (AxisCommandMode::Velocity == commandMode)
			WMX.coremotion.velocity->ExecQuickStop(m_no);
		else
			WMX.coremotion.motion->ExecQuickStop(m_no);

		WMX.coremotion.home->Cancel(m_no);

		m_curIdx = P_JOG;
		m_nxtIdx = P_JOG;
		m_state.isHome = false;
		if (!WMX.coremotion.home->SetHomeDone(m_no, 0))
		{
			WMX.status.axesStatus[m_no].homeDone = false;
		}
		m_state.isPaused = false;
		if (C_MOTION_BUSY == m_motionMode || m_fsmDrv.IsBusy(C_MOTION_BUSY))
		{
			int channel = m_fsmDrv.GetMsg();
			if (Between(channel, 0, MAX_API_BUFFER_CHANNEL - 1))
			{
				for (int i = 0; i < apibufferMAX; i++)
				{
					ApiBufferList no = ApiBufferList(i);
					if (APIBUFFER[no].Channel() == channel)
					{
						if (APIBUFFER[no].IsBusy())
							APIBUFFER[no].Stop();

						break;
					}
				}
			}
		}
		else if (C_POS_LINEAR == m_motionMode || m_fsmDrv.IsBusy(C_POS_LINEAR))
		{
			int lnno = m_fsmDrv.GetMsg();
			if (Between(lnno, 0, InterpolationMotor::maxDevice - 1))
			{
				LN[InterpolationMotorList(lnno)].Stop();
				for (int i = 0; i < LN[InterpolationMotorList(lnno)].m_command.axisCount; i++)
				{
					AxisList mtno = (AxisList)LN[InterpolationMotorList(lnno)].m_command.parameter[i].no;
					MT[mtno].m_fsmDrv.Set(C_IDLE);
				}
			}
		}
		m_motionMode = 0;
		m_fsmDrv.Set(C_IDLE);
	}

	void Motor::HomeInternalCycle(void)
	{
		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return;

		if (!m_fsmDrv.Between(C_HOME_SEQ_START, C_HOME_SEQ_END))
			return;

		if (m_fsmDrv.TimeLimit(80 * _1sec))	//60000
		{
			ER.Save((ErrName)(ER_MT_HOME_TMOVER + m_no + 1));
			CancelHomeSearch();
			return;
		}

		if (!m_state.isServoOn || m_state.isAlarm)
		{
			CancelHomeSearch();
			return;
		}

		AxisCommandMode::T	commandMode;
		WMX.coremotion.axisControl->GetAxisCommandMode(m_no, &commandMode);
		if (AxisCommandMode::Position != commandMode)
		{
			if (WMX.coremotion.axisControl->SetAxisCommandMode(m_no, AxisCommandMode::Position))
				CancelHomeSearch();

			return;
		}

		if (!WMX.status.axesStatus[m_no].commandReady)
			return;

		if (CanMove(HOME_CANMOVE))
		{
			m_fsmDrv.Set(C_HOME_SEARCH_START);
		}
	}

	void Motor::HomeSearchCycle(void)
	{
		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return;

		if (!m_fsmDrv.Between(C_HOME_SEARCH_START, C_HOME_SEARCH_END))
			return;

		if (IPCMMI.tmWatchDog.TmOver(1000))
		{
			ER.Save(ER_MMISEQ_COMMUNICATION_TMOVER);
			CancelHomeSearch();
			return;
		}

		if (m_fsmDrv.TimeLimit(150 * _1sec))
		{
			ER.Save((ErrName)(ER_MT_HOME_TMOVER + m_no + 1));
			CancelHomeSearch();
			return;
		}

		if (WMX.status.axesStatus[m_no].homeError || !m_state.isServoOn || m_state.isAlarm)
		{
			CancelHomeSearch();
			return;
		}

		switch (m_fsmDrv.Get())
		{
		case C_HOME_SEARCH_START:
		{
			if (m_fsmDrv.Once())
			{
				m_state.isPaused = false;
				m_state.isHome = false;
				if (OPR.isNoDevice)
				{
					if (ErrorCode::None != WMX.coremotion.home->SetHomeDone(m_no, 1))
					{
						m_fsmDrv.Set(C_IDLE);
						break;
					}
					WMX.status.axesStatus[m_no].homeDone = true;
				}
				else
				{
					if (ErrorCode::None != WMX.coremotion.home->StartHome(m_no))
					{
						m_fsmDrv.Set(C_IDLE);
						break;
					}
					WMX.status.axesStatus[m_no].homeDone = false;
				}
			}

			if (!WMX.status.axesStatus[m_no].homeDone)
				break;

			m_fsmDrv.Set(C_HOME_SEARCH_END);
		}
		break;
		case C_HOME_SEARCH_END:
		{
			if (m_fsmDrv.Once())
			{
				wmx3Api::Home::HomeData homeData;
				ZeroMemory(&homeData, sizeof(homeData));
				WMX.coremotion.home->GetHomeData(&homeData);

				wmx3Api::Config::LimitParam param;
				ZeroMemory(&param, sizeof(param));
				WMX.coremotion.config->GetLimitParam(m_no, &param);

				wmx3Api::Config::HomeParam homeParam;
				ZeroMemory(&homeParam, sizeof(homeParam));
				WMX.coremotion.config->GetHomeParam(m_no, &homeParam);

				m_homeDirection = homeParam.homeDirection;
				m_distLStoIndexPulse = 0;

				if (0. < m_moveStroke)
				{
					m_distLStoIndexPulse = WMX.PulseToMM(homeData.axesHomeData[m_no].distLStoZPulse, m_pulseRate);
					if (wmx3Api::Config::HomeDirection::Positive == homeParam.homeDirection)
					{
						m_limitP = (-1) * m_distLStoIndexPulse + WMX.PulseToMM(homeParam.homePosition, m_pulseRate);
						m_limitN = m_limitP - m_moveStroke + WMX.PulseToMM(homeParam.homePosition, m_pulseRate);
					}
					else
					{
						m_limitN = (-1) * m_distLStoIndexPulse + WMX.PulseToMM(homeParam.homePosition, m_pulseRate);
						m_limitP = m_moveStroke + m_limitN + WMX.PulseToMM(homeParam.homePosition, m_pulseRate);
					}
					param.softLimitNegativePos = WMX.MMToPulse(m_limitN, m_pulseRate);
					param.softLimitPositivePos = WMX.MMToPulse(m_limitP, m_pulseRate);
					//param.softLimitType = wmx3Api::Config::LimitSwitchType::T::ServoOff;
					//WMX.coremotion.config->SetLimitParam(m_no, &param);

					LOG[logMT].Message("[C_HOME_SEARCH_END],[%.2d],[softlimit negative],[%d],[softlimit positive],[%d]",
						m_no, (int)m_limitN, (int)m_limitP);
				}
				else
				{
					LOG[logMT].Message("[C_HOME_SEARCH_END],[%.2d],[softlimit disable]", m_no);
				}
			}

			if (m_state.isDriving || !m_state.isServoRdy)
				break;

			m_fsmDrv.Set(C_IDLE);
			m_state.isHome = true;
			PosMove(m_homePosIdx, PosTable(m_homePosIdx));
		}
		break;
		}
	}

	void Motor::JogCommandCycle(void)
	{
		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return;

		if (!m_fsmDrv.Between(C_JOG_START, C_JOG_END))
			return;

		if (IPCMMI.tmWatchDog.TmOver(1000))
		{
			ER.Save(ER_MMISEQ_COMMUNICATION_TMOVER);
			CancelHomeSearch();
			return;
		}

		switch (m_fsmDrv.Get())
		{
		case C_JOG_START:
		{
			int nErrorCode = WMX.CommandJog(m_command);
			if (CMD_MODE_CHANGE == nErrorCode)
			{
				break;
			}
			if (CMD_ERR_READY == nErrorCode)
			{
				LOG[logMT].Message("[C_JOG_START],[%.2d],[commandReady]", m_no);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_MODE == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_MODE + m_no + 1));
				LOG[logMT].Message("[C_JOG_START],[%.2d],[SetAxisCommandMode]", m_no);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_VEL == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_VEL_CMD + m_no + 1));
				LOG[logMT].Message("[C_JOG_START],[%.2d],[cmdVel],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					m_no, (int)m_command.profile.vel, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_ACC == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_ACC_CMD + m_no + 1));
				LOG[logMT].Message("[C_JOG_START],[%.2d],[cmdAcc],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					m_no, (int)m_command.profile.acc, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}
			if (!!nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_MOTION + m_no + 1));
				LOG[logMT].Message("[C_JOG_START],[%.2d],[cmdPos],[%d],[cmdCnt],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d],[motion command fail]",
					m_no, (int)m_command.pos, (int)m_state.cmdCnt, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}

			LOG[logMT].Message("[C_JOG_START],[%.2d],[cmdPos],[%d],[cmdCnt],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d],[y-trigger]",
				m_no, (int)m_command.pos, (int)m_state.cmdCnt, m_nxtIdx, m_curIdx);

			m_moveTime = 0;
			m_fsmDrv.Set(C_JOG_END);
			m_fsmDrv.RstDelay(FALSE);
		}
		break;
		case C_JOG_END:
		{
			if (!m_fsmDrv.Delay(1))
				break;

			if (m_state.isDriving || !m_state.isDelayedPosSet)
				break;

			m_command.pos = m_state.cmdCnt;

			LOG[logMT].Message("[C_JOG_END],[%.2d],[cmdCnt],[%d],[realCnt],[%d],[arrive time:%.3f]",
				m_no, (int)m_state.cmdCnt, (int)m_state.realCnt, m_fsmDrv.Elapsed() / 1000.);

			m_moveTime = m_fsmDrv.Elapsed();
			m_curIdx = m_nxtIdx;
			m_motionMode = 0;
			m_fsmDrv.Set(C_IDLE);
		}
		break;
		}
	}

	void Motor::PosCommandCycle(void)
	{
		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return;

		if (!m_fsmDrv.Between(C_POS_START, C_POS_END))
			return;

		switch (m_fsmDrv.Get())
		{
		case C_POS_START:
		{
			if (m_command.pos == m_command.startPos)
			{
				LOG[logMT].Message("[C_POS_START],[%.2d],[cmdPos],[%.4f],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[n-trigger]",
					m_no, m_command.pos, m_state.cmdCnt, m_nxtIdx, m_curIdx);
				m_curIdx = m_nxtIdx;
				m_fsmDrv.Set(C_IDLE);
				break;
			}
			int nErrorCode = WMX.CommandPos(m_command);
			if (CMD_MODE_CHANGE == nErrorCode)
			{
				break;
			}
			if (CMD_ERR_READY == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_CMD_READY + m_no + 1));
				LOG[logMT].Message("[C_POS_START],[%.2d],[commandReady]", m_no);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_MODE == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_MODE + m_no + 1));
				LOG[logMT].Message("[C_POS_START],[%.2d],[SetAxisCommandMode]", m_no);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_SOFTLIMIT == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_SW_LIMIT + m_no + 1));
				LOG[logMT].Message("[C_POS_START],[%.2d],[softlimit negative],[%d],[softlimit positive],[%d]",
					m_no, (int)m_command.limitN, (int)m_command.limitP);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_VEL == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_VEL_CMD + m_no + 1));
				LOG[logMT].Message("[C_POS_START],[%.2d],[cmdVel],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					m_no, (int)m_command.profile.vel, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_ACC == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_ACC_CMD + m_no + 1));
				LOG[logMT].Message("[C_POS_START],[%.2d],[cmdAcc],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					m_no, (int)m_command.profile.acc, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}
			if (!!nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_MOTION + m_no + 1));
				LOG[logMT].Message("[C_POS_START],[%.2d],[cmdPos],[%.4f],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[motion command fail]",
					m_no, m_command.pos, m_state.cmdCnt, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}

			SLogEndEvent();
			SLogStartEvent();

			LOG[logMT].Message("[C_POS_START],[%.2d],[cmdPos],[%.4f],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[y-trigger]",
				m_no, m_command.pos, m_state.cmdCnt, m_nxtIdx, m_curIdx);

			m_moveTime = 0;
			m_fsmDrv.Set(C_POS_END);
			m_fsmDrv.RstDelay(FALSE);
		}
		break;
		case C_POS_PAUSED:
		{
			if (!m_fsmDrv.Delay(1))
				break;

			if (m_state.isDriving || !m_state.isDelayedPosSet)
				break;

			SLogEndEvent();

			LOG[logMT].Message("[C_POS_PAUSED],[%.2d],[cmdCnt],[%.4f],[realCnt],[%.4f],[arrive time:%.3f]",
				m_no, m_state.cmdCnt, m_state.realCnt, m_fsmDrv.Elapsed() / 1000.);

			m_fsmDrv.Set(C_IDLE);
		}
		break;
		case C_POS_END:
		{
			if (!m_fsmDrv.Delay(1))
				break;

			if (m_state.isDriving || !m_state.isDelayedPosSet)
				break;

			m_command.pos = m_state.cmdCnt;

			SLogEndEvent();

			LOG[logMT].Message("[C_POS_END],[%.2d],[cmdCnt],[%.4f],[realCnt],[%.4f],[arrive time:%.3f]",
				m_no, m_state.cmdCnt, m_state.realCnt, m_fsmDrv.Elapsed() / 1000.);

			m_moveTime = m_fsmDrv.Elapsed();
			m_curIdx = m_nxtIdx;
			m_motionMode = 0;
			m_fsmDrv.Set(C_IDLE);
		}
		break;
		}
	}

	void Motor::VelCommandCycle(void)
	{
		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return;

		if (!m_fsmDrv.Between(C_VEL_START, C_VEL_END))
			return;

		if (IPCMMI.tmWatchDog.TmOver(1000))
		{
			ER.Save(ER_MMISEQ_COMMUNICATION_TMOVER);
			CancelHomeSearch();
			return;
		}

		switch (m_fsmDrv.Get())
		{
		case C_VEL_START:
		{
			int nErrorCode = WMX.CommandVel(m_command);
			if (CMD_MODE_CHANGE == nErrorCode)
			{
				break;
			}
			if (CMD_ERR_READY == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_CMD_READY + m_no + 1));
				LOG[logMT].Message("[C_VEL_START],[%.2d],[commandReady]", m_no);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_MODE == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_MODE + m_no + 1));
				LOG[logMT].Message("[C_VEL_START],[%.2d],[SetAxisCommandMode]", m_no);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_VEL == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_VEL_CMD + m_no + 1));
				LOG[logMT].Message("[C_VEL_START],[%.2d],[cmdVel],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					m_no, (int)m_command.profile.vel, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}
			if (CMD_ERR_ACC == nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_ACC_CMD + m_no + 1));
				LOG[logMT].Message("[C_VEL_START],[%.2d],[cmdAcc],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					m_no, (int)m_command.profile.acc, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}
			if (!!nErrorCode)
			{
				ER.Save((ErrName)(ER_MT_MOTION + m_no + 1));
				LOG[logMT].Message("[C_VEL_START],[%.2d],[cmdVel],[%d],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[motion command fail]",
					m_no, (int)m_command.profile.vel, m_state.cmdCnt, m_nxtIdx, m_curIdx);
				CancelHomeSearch();
				break;
			}

			SLogEndEvent();
			SLogStartEvent();

			LOG[logMT].Message("[C_VEL_START],[%.2d],[cmdVel],[%d],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[y-trigger]",
				m_no, (int)m_command.profile.vel, m_state.cmdCnt, m_nxtIdx, m_curIdx);

			m_moveTime = 0;
			m_fsmDrv.Set(C_VEL_END);
			m_fsmDrv.RstDelay(FALSE);
		}
		break;
		case C_VEL_END:
		{
			if (!m_fsmDrv.Delay(1))
				break;

			if (m_state.isDriving)
				break;

			m_command.pos = m_state.cmdCnt;

			SLogEndEvent();

			LOG[logMT].Message("[C_VEL_END],[%.2d],[cmdCnt],[%.4f],[realCnt],[%.4f],[arrive time:%.3f]",
				m_no, m_state.cmdCnt, m_state.realCnt, m_fsmDrv.Elapsed() / 1000.);

			m_moveTime = m_fsmDrv.Elapsed();
			m_curIdx = m_nxtIdx;
			m_motionMode = 0;
			m_fsmDrv.Set(C_IDLE);
		}
		break;
		}
	}

	void Motor::TrqCommandCycle(void)
	{
		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return;

		if (isVirtualSW || !m_fsmDrv.Between(C_TRQ_START, C_TRQ_END))
			return;

		if (IPCMMI.tmWatchDog.TmOver(1000))
		{
			ER.Save(ER_MMISEQ_COMMUNICATION_TMOVER);
			CancelHomeSearch();
			return;
		}

		switch (m_fsmDrv.Get())
		{
			case C_TRQ_START:
			{
				int nErrorCode = WMX.CommandTrq(m_command);
				if (CMD_MODE_CHANGE == nErrorCode)
				{
					break;
				}
				if (CMD_ERR_READY == nErrorCode)
				{
					ER.Save((ErrName)(ER_MT_CMD_READY + m_no + 1));
					LOG[logMT].Message("[C_TRQ_START],[%.2d],[commandReady]", m_no);
					CancelHomeSearch();
					break;
				}
				if (CMD_ERR_MODE == nErrorCode)
				{
					ER.Save((ErrName)(ER_MT_MODE + m_no + 1));
					LOG[logMT].Message("[C_TRQ_START],[%.2d],[SetAxisCommandMode]", m_no);
					CancelHomeSearch();
					break;
				}
				if (!!nErrorCode)
				{
					ER.Save((ErrName)(ER_MT_MOTION + m_no + 1));
					LOG[logMT].Message("[C_TRQ_START],[%.2d],[cmdTrq],[%d],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[motion command fail]",
						m_no, (int)m_command.trq, m_state.cmdCnt, m_nxtIdx, m_curIdx);
					CancelHomeSearch();
					break;
				}

				SLogEndEvent();
				SLogStartEvent();

				LOG[logMT].Message("[C_TRQ_START],[%.2d],[cmdTrq],[%d],[cmdCnt],[%.4f],[nxtIdx],[%.2d],[curIdx],[%.2d],[y-trigger]",
					m_no, (int)m_command.trq, m_state.cmdCnt, m_nxtIdx, m_curIdx);

				m_moveTime = 0;
				m_fsmDrv.Set(C_TRQ_END);
				m_fsmDrv.RstDelay(FALSE);
				break;
			}
			case C_TRQ_END:
			{
				if (!m_fsmDrv.Delay(1))
					break;

				if (m_state.isDriving)
					break;

				m_command.pos = m_state.cmdCnt;
				WMX.coremotion.torque->SetMaxTrqLimit(m_no, 300);
				WMX.coremotion.velocity->SetMaxMotorSpeed(m_no, 3000);

				SLogEndEvent();

				LOG[logMT].Message("[C_TRQ_END],[%.2d],[cmdCnt],[%.4f],[realCnt],[%.4f],[arrive time:%.3f]",
					m_no, m_state.cmdCnt, m_state.realCnt, m_fsmDrv.Elapsed() / 1000.);

				m_moveTime = m_fsmDrv.Elapsed();
				m_curIdx = m_nxtIdx;
				m_motionMode = 0;
				m_fsmDrv.Set(C_IDLE);
				break;
			}
		}
	}

	void Motor::SetCnt(double cnt)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		WMX.coremotion.home->SetCommandPos(m_no, cnt);
		WMX.coremotion.home->SetFeedbackPos(m_no, cnt);
	}

	//------------------------------------------------------------------
	// Numerator : 엔코더 분해능, Denominator :
	// 통신이 정지된 상태에서만 적용됨..
	void Motor::SetElecGearRatio(UINT32 numerator, UINT32 denominator)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		WMX.coremotion.config->SetGearRatio(m_no, numerator, denominator);
	}

	//-------------------------------------------------------------------
	void Motor::ServoOn(void)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		BrakeOff();
		WMX.coremotion.axisControl->SetServoOn(m_no, 1);
	}

	//-------------------------------------------------------------------
	void Motor::ServoOff(void)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		WMX.coremotion.axisControl->SetServoOn(m_no, 0);
	}

	//-------------------------------------------------------------------
	void Motor::BrakeOn(void)
	{
		if (isVirtualSW || NOT_DEFINED == m_brakeNo)
			return;

		WMX.SetBrake(m_brakeNo, TRUE);
	}

	//-------------------------------------------------------------------
	void Motor::BrakeOff(void)
	{
		if (isVirtualSW || isVirtualHW || NOT_DEFINED == m_brakeNo)
			return;

		WMX.SetBrake(m_brakeNo, FALSE);
	}

	//-------------------------------------------------------------------
	void Motor::AlarmClear(void)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		WMX.coremotion.axisControl->ClearAmpAlarm(m_no);
		WMX.coremotion.axisControl->ClearAxisAlarm(m_no);
	}

	// TrqMove->구동시 모터 튕김 현상 방지
	INT32 Motor::GetCmdMode(void)
	{
		AxisCommandMode::T axisCommandMode;
		WMX.coremotion.axisControl->GetAxisCommandMode(m_no, &axisCommandMode);
		return axisCommandMode;
	}

	//-------------------------------------------------------------------
	void Motor::SetCmdMode(AxisCommandMode::T setCmdMode)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		WMX.coremotion.axisControl->SetAxisCommandMode(m_no, setCmdMode);
	}

	//-------------------------------------------------------------------
	BOOL Motor::IsRdy(int posIdx)
	{
		if (!m_state.isHome)
			return (FALSE);

		if (m_state.isDriving)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused)
			return (FALSE);

		if (0 <= posIdx)
		{
			if (m_nxtIdx != posIdx)
				return (FALSE);
		}
		return (TRUE);
	}

	BOOL Motor::IsRdywithoutPause(int posIdx)
	{
		if (!m_state.isHome)
			return (FALSE);

		if (m_state.isDriving)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy)
			return (FALSE);

		if (0 <= posIdx)
		{
			if (m_nxtIdx != posIdx)
				return (FALSE);
		}

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::InPos(int posIdx)
	{
		if (m_nxtIdx != posIdx || !Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		double min = PosTable(posIdx) - m_inposBand;
		double max = PosTable(posIdx) + m_inposBand;

		if (min > m_state.realCnt)
			return (FALSE);

		if (max < m_state.realCnt)
			return (FALSE);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::InPos(int posIdx, double pos, double inposBand)
	{
		if (m_nxtIdx != posIdx)
			return (FALSE);

		double min = pos - m_inposBand;
		double max = pos + m_inposBand;

		if (0 < inposBand)
		{
			min = pos - inposBand;
			max = pos + inposBand;
		}

		if (min > m_state.realCnt)
			return (FALSE);
		if (max < m_state.realCnt)
			return (FALSE);

		return (TRUE);
	}
	//-------------------------------------------------------------------
	BOOL Motor::InCmd(int posIdx)
	{
		if (m_nxtIdx != posIdx)
			return (FALSE);

		double cmdPos = round(m_state.cmdCnt * 1000.) / 1000.;
		double setPos = round(PosTable(posIdx) * 1000.) / 1000.;

		if (cmdPos != setPos)
			return (FALSE);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::InCmd(int posIdx, double pos)
	{
		if (m_nxtIdx != posIdx)
			return (FALSE);

		double cmdPos = round(m_state.cmdCnt * 1000.) / 1000.;
		double setPos = round(pos * 1000.) / 1000.;

		if (cmdPos != setPos)
			return (FALSE);

		return (TRUE);
	}
	//-------------------------------------------------------------------
	BOOL Motor::BetweenIdxPos(int posIdx1, int posIdx2)
	{
		double min = 0;
		double max = 0;

		if (PosTable(posIdx1) < PosTable(posIdx2))
		{
			min = PosTable(posIdx1) - m_inposBand;
			max = PosTable(posIdx2) + m_inposBand;
		}
		else
		{
			min = PosTable(posIdx2) - m_inposBand;
			max = PosTable(posIdx1) + m_inposBand;
		}

		if (min > m_state.realCnt)
			return (FALSE);

		if (max < m_state.realCnt)
			return (FALSE);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::BetweenPos(double pos1, double pos2)
	{
		double min = 0;
		double max = 0;

		if (pos1 < pos2)
		{
			min = pos1 - m_inposBand;
			max = pos2 + m_inposBand;
		}
		else
		{
			min = pos2 - m_inposBand;
			max = pos1 + m_inposBand;
		}

		if (min > m_state.realCnt)
			return (FALSE);

		if (max < m_state.realCnt)
			return (FALSE);

		return (TRUE);
	}

	BOOL Motor::CurIdx(int posIdx)
	{
		return (m_curIdx == posIdx);
	}

	BOOL Motor::CmdIdx(int posIdx)
	{
		return (m_nxtIdx == posIdx);
	}

	BOOL Motor::CurIdx(int posIdx1, int posIdx2)
	{
		return ((posIdx1 <= m_curIdx) && (m_curIdx <= posIdx2));
	}

	BOOL Motor::CmdIdx(int posIdx1, int posIdx2)
	{
		return ((posIdx1 <= m_nxtIdx) && (m_nxtIdx <= posIdx2));
	}

	BOOL Motor::CmdOrCurIdx(int posIdx)
	{
		return (CmdIdx(posIdx) || CurIdx(posIdx));
	}

	BOOL Motor::CmdOrCurIdx(int posIdx1, int posIdx2)
	{
		return (CmdIdx(posIdx1, posIdx2) || CurIdx(posIdx1, posIdx2));
	}

	double& Motor::PosTable(int posIdx)
	{
		return (NV.motTable[m_no].dPos[posIdx]);
	}

	double& Motor::VelTable(int posIdx)
	{
		return (NV.motTable[m_no].dVel[posIdx]);
	}

	double& Motor::AccTable(int posIdx)
	{
		return (NV.motTable[m_no].dAcc[posIdx]);
	}

	//-------------------------------------------------------------------
	BOOL Motor::PosMove(int posIdx)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		m_nxtIdx = posIdx;
		m_command = CommandTable(posIdx);
		m_isDirP = (m_command.pos > m_state.cmdCnt);
		m_fsmDrv.Set(C_POS_START);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::PosMove(int posIdx, double pos, double vel, double acc)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(vel, 0., m_maxVel) || !Between(acc, 0., m_maxAcc))
			return (FALSE);

		if (0 >= vel || 0 >= acc)
		{
			if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
				return (FALSE);
		}

		m_nxtIdx = posIdx;
		if (Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_command = CommandTable(posIdx);
		}
		else
		{
			m_command = CommandTable(m_homePosIdx);
			m_command.idx = posIdx;
		}
		m_command.pos = pos;
		if (0 < vel)
			m_command.profile.vel = round(vel * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		if (0 < acc)
			m_command.profile.acc = round(acc * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;

		m_isDirP = (m_command.pos > m_state.cmdCnt);
		m_fsmDrv.Set(C_POS_START);

		return (TRUE);
	}

	BOOL Motor::VelFwd(int posIdx)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		m_nxtIdx = posIdx;
		m_command = CommandTable(posIdx);
		m_isDirP = (0 < m_command.profile.vel);
		m_fsmDrv.Set(C_VEL_START);

		return (TRUE);
	}

	BOOL Motor::VelBwd(int posIdx)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		m_nxtIdx = posIdx;
		m_command = CommandTable(posIdx);
		m_command.profile.vel = -m_command.profile.vel;
		m_isDirP = (0 < m_command.profile.vel);
		m_fsmDrv.Set(C_VEL_START);

		return (TRUE);
	}

	BOOL Motor::VelFwd(int posIdx, double vel, double acc)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		//if (0 >= vel || 0 >= acc)
		//	return (FALSE);

		if (!Between(vel, 1., m_maxVel) || !Between(acc, 1., m_maxAcc))
			return (FALSE);

		if (0 >= vel || 0 >= acc)
		{
			if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
				return (FALSE);
		}

		m_nxtIdx = posIdx;
		if (Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_command = CommandTable(posIdx);
		}
		else
		{
			m_command = CommandTable(m_homePosIdx);
			m_command.idx = posIdx;
		}
		if (0 < vel)
			m_command.profile.vel = round(vel * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		if (0 < acc)
			m_command.profile.acc = round(acc * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;

		m_isDirP = (0 < m_command.profile.vel);
		m_fsmDrv.Set(C_VEL_START);

		return (TRUE);
	}

	BOOL Motor::VelBwd(int posIdx, double vel, double acc)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		//if (0 >= vel || 0 >= acc)
		//	return (FALSE);

		if (!Between(vel, 1., m_maxVel) || !Between(acc, 1., m_maxAcc))
			return (FALSE);

		if (0 == vel || 0 == acc)
		{
			if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
				return (FALSE);
		}

		m_nxtIdx = posIdx;
		if (Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_command = CommandTable(posIdx);
		}
		else
		{
			m_command = CommandTable(m_homePosIdx);
			m_command.idx = posIdx;
		}
		if (0 < vel)
			m_command.profile.vel = round(vel * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		if (0 < acc)
			m_command.profile.acc = round(acc * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;

		m_command.profile.vel = -m_command.profile.vel;

		m_isDirP = (0 < m_command.profile.vel);
		m_fsmDrv.Set(C_VEL_START);

		return (TRUE);
	}

	BOOL Motor::TrqMove(int posIdx, double trq, double vel)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(vel, 1., m_maxVel))
			vel = m_maxVel;

		m_nxtIdx = posIdx;
		m_command = CommandTable(m_homePosIdx);
		m_command.trq = trq;
		m_command.profile.vel = round(vel * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		m_isDirP = (0 < m_command.trq);
		m_fsmDrv.Set(C_TRQ_START);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::JogFwd(double vel, double acc)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(vel, 1., m_maxVel) || !Between(acc, 1., m_maxAcc))
			return (FALSE);

		m_nxtIdx = P_JOG;
		m_command = CommandTable(m_homePosIdx);
		m_command.profile.vel = round(vel * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		m_command.profile.acc = round(acc * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		m_isDirP = (0 < m_command.profile.vel);
		m_fsmDrv.Set(C_JOG_START);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL Motor::JogBwd(double vel, double acc)
	{
		if (isVirtualSW)
			return (FALSE);

		if (OPR.isDebugMode || isVirtualSW || isVirtualHW)
			return (FALSE);

		if (m_fsmDrv.IsBusy() || !m_state.isServoRdy || m_state.isPaused || m_state.isDriving)
			return (FALSE);

		if (!Between(vel, 1., m_maxVel) || !Between(acc, 1., m_maxAcc))
			return (FALSE);

		m_nxtIdx = P_JOG;
		m_command = CommandTable(m_homePosIdx);
		m_command.profile.vel = round(vel * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;
		m_command.profile.acc = round(acc * (NV.sys[machineSpeed] / 100.) * 1000.) / 1000.;

		m_command.profile.vel = -m_command.profile.vel;

		m_isDirP = (0 < m_command.profile.vel);
		m_fsmDrv.Set(C_JOG_START);

		return (TRUE);
	}

	//-------------------------------------------------------------------
	void Motor::Paused(void)
	{
		if (isVirtualSW || isVirtualHW)
			return;

		if (!m_usePause && m_state.isDriving || !m_state.isHome ||
			m_fsmDrv.IsBusy(C_MOTION_BUSY) ||
			m_fsmDrv.IsBusy(C_POS_LINEAR) ||
			m_fsmDrv.Between(C_JOG_START, C_JOG_END) ||
			m_fsmDrv.Between(C_VEL_START, C_VEL_END) ||
			m_fsmDrv.Between(C_TRQ_START, C_TRQ_END) ||
			m_fsmDrv.Between(C_HOME_SEQ_START, C_HOME_SEQ_END) ||
			m_fsmDrv.Between(C_HOME_SEARCH_START, C_HOME_SEARCH_END))
		{
			CancelHomeSearch();
			return;
		}

		if (!m_usePause || !m_state.isDriving)
			return;

		AxisCommandMode::T	commandMode;
		WMX.coremotion.axisControl->GetAxisCommandMode(m_no, &commandMode);
		if (AxisCommandMode::Torque == commandMode)
			WMX.coremotion.torque->StopTrq(m_no);
		else if (AxisCommandMode::Velocity == commandMode)
			WMX.coremotion.velocity->ExecQuickStop(m_no);
		else
		{
			m_state.isPaused = true;
			WMX.coremotion.motion->ExecQuickStop(m_no);
			m_curIdx = P_JOG;
			m_fsmDrv.Set(C_POS_PAUSED);
		}
	}

	//-------------------------------------------------------------------
	BOOL Motor::Resume(void)
	{
		if (isVirtualSW || isVirtualHW || !m_state.isPaused || m_fsmDrv.IsBusy())
			return (FALSE);

		m_state.isPaused = false;

		if (!IsRdywithoutPause() || !CanMove(m_nxtIdx))
		{
			ER.Save((ErrName)(ER_MT_RESUME + m_no + 1));
			CancelHomeSearch();
			return (FALSE);
		}
		m_command.startPos = m_state.cmdCnt;

		m_isDirP = (m_command.pos > m_state.realCnt);
		m_fsmDrv.Set(C_POS_START);
		return (TRUE);
	}

	//-------------------------------------------------------------------
	InterpolationMotor::InterpolationMotor()
	{
		isNoDevice = (interpolationMAX > 0) ? 0 : 1;
	}

	InterpolationMotor::~InterpolationMotor()
	{

	}
	
	void InterpolationMotor::CreateInitParameter()
	{
		for (int i = 0; i < interpolationMAX; i++)
		{
			InterpolationMotorList no = InterpolationMotorList(i);
			LN[no].m_no = no;
		}
	}	

	void InterpolationMotor::CreateParameter(void)
	{
		BOOL isLimit = FALSE;
		for (int i = 0; i < m_command.axisCount; i++)
		{
			if (!Between(m_command.parameter[i].idx, 0, MAX_INDEX_MOTOR - 1))
				isLimit = TRUE;
		}
		m_command.pulseRate = _1000PUSLE_RATE;
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			int idx = m_command.parameter[i].idx;

			if (Between(m_command.parameter[i].idx, 0, MAX_INDEX_MOTOR - 1))
			{
				double pos = MT[mtno].PosTable(idx);
				m_command.parameter[i].pos = pos;
			}

			double vel = (double)round(MT[mtno].m_maxVel * (NV.sys[machineSpeed] / 100.));
			double acc = (double)round(MT[mtno].m_maxAcc * (NV.sys[machineSpeed] / 100.));
			if (!isLimit)
			{
				vel = MT[mtno].CommandTable(idx).profile.vel;
				acc = MT[mtno].CommandTable(idx).profile.acc;
			}

			if (m_command.profile.vel > vel || !m_command.profile.vel)
			{
				m_command.profile.vel = vel;
				m_command.profile.acc = acc;
				m_command.profile.jerkRatio = MT[mtno].m_jerkRatio;
				m_command.profile.jerkAcc = (double)MT[mtno].m_maxJerkAcc;
			}
			m_command.parameter[i].maxVel = (double)MT[mtno].m_maxVel;
			m_command.parameter[i].maxAcc = (double)MT[mtno].m_maxAcc;
			m_command.parameter[i].maxJerkAcc = (double)MT[mtno].m_maxJerkAcc;
			m_command.parameter[i].pulseRate = MT[mtno].m_pulseRate;
		}
	}

	//-------------------------------------------------------------------
	void InterpolationMotor::SetPulseRate(double pulseRate)
	{
		m_command.pulseRate = pulseRate;
	}

	void InterpolationMotor::SetAxis(int mtno1, int mtno2)
	{
		ZeroMemory(&m_command, sizeof(m_command));

		m_command.parameter[0].no = mtno1;
		m_command.parameter[1].no = mtno2;

		m_command.axisCount = 2;

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			m_command.parameter[i].limitP = MT[mtno].m_limitP;
			m_command.parameter[i].limitN = MT[mtno].m_limitN;
		}
	}

	void InterpolationMotor::SetAxis(int mtno1, int mtno2, int mtno3)
	{
		ZeroMemory(&m_command, sizeof(m_command));

		m_command.parameter[0].no = mtno1;
		m_command.parameter[1].no = mtno2;
		m_command.parameter[2].no = mtno3;

		m_command.axisCount = 3;

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			m_command.parameter[i].limitP = MT[mtno].m_limitP;
			m_command.parameter[i].limitN = MT[mtno].m_limitN;
		}
	}

	void InterpolationMotor::SetAxis(int mtno1, int mtno2, int mtno3, int mtno4)
	{
		ZeroMemory(&m_command, sizeof(m_command));

		m_command.parameter[0].no = mtno1;
		m_command.parameter[1].no = mtno2;
		m_command.parameter[2].no = mtno3;
		m_command.parameter[3].no = mtno4;

		m_command.axisCount = 4;

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			m_command.parameter[i].limitP = MT[mtno].m_limitP;
			m_command.parameter[i].limitN = MT[mtno].m_limitN;
		}
	}

	void InterpolationMotor::SetAxis(int mtno1, int mtno2, int mtno3, int mtno4, int mtno5)
	{
		ZeroMemory(&m_command, sizeof(m_command));

		m_command.parameter[0].no = mtno1;
		m_command.parameter[1].no = mtno2;
		m_command.parameter[2].no = mtno3;
		m_command.parameter[3].no = mtno4;
		m_command.parameter[4].no = mtno5;

		m_command.axisCount = 5;

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			m_command.parameter[i].limitP = MT[mtno].m_limitP;
			m_command.parameter[i].limitN = MT[mtno].m_limitN;
		}
	}

	//-------------------------------------------------------------------
	void InterpolationMotor::SetIndex(int idxno1, int idxno2)
	{
		m_command.parameter[0].idx = idxno1;
		m_command.parameter[1].idx = idxno2;
		CreateParameter();
	}

	void InterpolationMotor::SetIndex(int idxno1, int idxno2, int idxno3)
	{
		m_command.parameter[0].idx = idxno1;
		m_command.parameter[1].idx = idxno2;
		m_command.parameter[2].idx = idxno3;
		CreateParameter();
	}

	void InterpolationMotor::SetIndex(int idxno1, int idxno2, int idxno3, int idxno4)
	{
		m_command.parameter[0].idx = idxno1;
		m_command.parameter[1].idx = idxno2;
		m_command.parameter[2].idx = idxno3;
		m_command.parameter[3].idx = idxno4;
		CreateParameter();
	}

	void InterpolationMotor::SetIndex(int idxno1, int idxno2, int idxno3, int idxno4, int idxno5)
	{
		m_command.parameter[0].idx = idxno1;
		m_command.parameter[1].idx = idxno2;
		m_command.parameter[2].idx = idxno3;
		m_command.parameter[3].idx = idxno4;
		m_command.parameter[4].idx = idxno5;
		CreateParameter();
	}

	void InterpolationMotor::SetPos(double pos1, double pos2)
	{
		m_command.parameter[0].pos = pos1;
		m_command.parameter[1].pos = pos2;
	}

	void InterpolationMotor::SetPos(double pos1, double pos2, double pos3)
	{
		m_command.parameter[0].pos = pos1;
		m_command.parameter[1].pos = pos2;
		m_command.parameter[2].pos = pos3;
	}

	void InterpolationMotor::SetPos(double pos1, double pos2, double pos3, double pos4)
	{
		m_command.parameter[0].pos = pos1;
		m_command.parameter[1].pos = pos2;
		m_command.parameter[2].pos = pos3;
		m_command.parameter[3].pos = pos4;
	}

	void InterpolationMotor::SetPos(double pos1, double pos2, double pos3, double pos4, double pos5)
	{
		m_command.parameter[0].pos = pos1;
		m_command.parameter[1].pos = pos2;
		m_command.parameter[2].pos = pos3;
		m_command.parameter[3].pos = pos4;
		m_command.parameter[4].pos = pos5;
	}

	void InterpolationMotor::SetVel(double percent)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);

			double vel = (double)round(MT[mtno].m_maxVel * (percent / 100.) * (NV.sys[machineSpeed] / 100.));
			double acc = (double)round(MT[mtno].m_maxAcc * (percent / 100.) * (NV.sys[machineSpeed] / 100.));

			if (m_command.profile.vel > vel || !m_command.profile.vel)
			{
				m_command.profile.vel = vel;
				m_command.profile.acc = acc;
			}
		}
	}

	//-------------------------------------------------------------------
	BOOL InterpolationMotor::IsRdy(void)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = (AxisList)m_command.parameter[i].no;
			if (!MT[mtno].IsRdy())
				return (FALSE);
		}

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL InterpolationMotor::IsHome(void)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = (AxisList)m_command.parameter[i].no;
			if (!MT[mtno].m_state.isHome)
				return (FALSE);
		}

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL InterpolationMotor::IsAlarm(void)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = (AxisList)m_command.parameter[i].no;
			if (MT[mtno].m_state.isAlarm)
				return (TRUE);
		}

		return (FALSE);
	}

	//-------------------------------------------------------------------
	BOOL InterpolationMotor::IsDriving(void)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = (AxisList)m_command.parameter[i].no;
			if (MT[mtno].m_state.isDriving)
				return (TRUE);
		}

		return (FALSE);
	}

	//-------------------------------------------------------------------
	BOOL InterpolationMotor::IsDelayedPosSet(void)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			if (!MT[mtno].m_state.isDelayedPosSet)
				return (FALSE);
		}

		return (TRUE);
	}

	//-------------------------------------------------------------------
	BOOL InterpolationMotor::Move(void)
	{
		if (isNoDevice || 1 > m_command.axisCount)
			return (FALSE);

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			if (MT[mtno].m_fsmDrv.IsBusy())
				return (FALSE);

			if (!MT[mtno].m_state.isServoRdy || MT[mtno].m_state.isPaused || MT[mtno].m_state.isDriving)
				return (FALSE);
		}

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			int idx = m_command.parameter[i].idx;
			MT[mtno].m_nxtIdx = idx;
			ZeroMemory(&MT[mtno].m_command, sizeof(MT[mtno].m_command));
			MT[mtno].m_command.pos = m_command.parameter[i].pos;
			MT[mtno].m_command.profile.vel = m_command.profile.vel;
			MT[mtno].m_command.profile.acc = m_command.profile.acc;
			MT[mtno].m_isDirP = (MT[mtno].m_command.pos > MT[mtno].m_state.cmdCnt);
			MT[mtno].m_motionMode = C_POS_LINEAR;
			MT[mtno].m_fsmDrv.Set(C_POS_LINEAR, m_no);
		}
		return (TRUE);
	}

	//-------------------------------------------------------------------
	void InterpolationMotor::Stop(void)
	{
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = AxisList(m_command.parameter[i].no);
			MT[mtno].Stop();
		}
	}

	//-------------------------------------------------------------------
	void InterpolationMotor::PosCommandCycle(void)
	{
		if (OPR.isDebugMode)
			return;

		BOOL isError, isBusy, isIdle;
		isError = isBusy = isIdle = FALSE;
		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = (AxisList)m_command.parameter[i].no;
			if (MT[mtno].m_fsmDrv.IsBusy(C_POS_LINEAR))
			{
				if (isIdle || MT[mtno].isVirtualSW || MT[mtno].isVirtualHW || MT[mtno].m_fsmDrv.GetMsg() != m_no)
				{
					isError = TRUE;
					break;
				}
				isBusy = TRUE;
			}
			else
			{
				if (isBusy)
				{
					isError = TRUE;
					break;
				}
				isIdle = TRUE;
			}
		}

		if (isError)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				ER.Save((ErrName)(ER_MT_MOTION + mtno + 1));
				LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[cmdPos],[%d],[cmdCnt],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d],[motion command fail]",
					mtno, (int)m_command.parameter[i].pos, (int)MT[mtno].m_state.cmdCnt, MT[mtno].m_nxtIdx, MT[mtno].m_curIdx);
				MT[mtno].CancelHomeSearch();
			}
			return;
		}

		if (!isBusy)
			return;

		int nErrorCode = WMX.CommandPos(m_command);
		if (CMD_MODE_CHANGE == nErrorCode)
		{
			return;
		}
		if (CMD_ERR_READY == nErrorCode)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				if (!WMX.status.axesStatus[mtno].commandReady)
				{
					ER.Save((ErrName)(ER_MT_CMD_READY + mtno + 1));
					LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[commandReady]", mtno);
				}
				MT[mtno].m_fsmDrv.Set(C_IDLE);
			}
			return;
		}
		if (CMD_ERR_MODE == nErrorCode)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				MT[mtno].m_fsmDrv.Set(C_IDLE);
			}
			return;
		}
		if (CMD_ERR_SOFTLIMIT == nErrorCode)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				MT[mtno].m_fsmDrv.Set(C_IDLE);
				if (round(m_command.parameter[i].limitN) == round(m_command.parameter[i].limitP))
					continue;

				if (Between(m_command.parameter[i].pos, m_command.parameter[i].limitN, m_command.parameter[i].limitP))
					continue;

				ER.Save((ErrName)(ER_MT_SW_LIMIT + mtno + 1));
				LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[softlimit negative],[%d],[softlimit positive],[%d]",
					mtno, (int)m_command.parameter[i].limitN, (int)m_command.parameter[i].limitP);
			}
			return;
		}
		if (CMD_ERR_VEL == nErrorCode)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				MT[mtno].m_fsmDrv.Set(C_IDLE);
				ER.Save((ErrName)(ER_MT_VEL_CMD + mtno + 1));
				LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[cmdVel],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					mtno, (int)m_command.profile.vel, MT[mtno].m_nxtIdx, MT[mtno].m_curIdx);
			}
			return;
		}
		if (CMD_ERR_ACC == nErrorCode)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				MT[mtno].m_fsmDrv.Set(C_IDLE);
				ER.Save((ErrName)(ER_MT_ACC_CMD + mtno + 1));
				LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[cmdAcc],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d]",
					mtno, (int)m_command.profile.acc, MT[mtno].m_nxtIdx, MT[mtno].m_curIdx);
			}
			return;
		}
		if (!!nErrorCode)
		{
			for (int i = 0; i < m_command.axisCount; i++)
			{
				AxisList mtno = (AxisList)m_command.parameter[i].no;
				MT[mtno].CancelHomeSearch();
				ER.Save((ErrName)(ER_MT_MOTION + mtno + 1));
				LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[cmdPos],[%d],[cmdCnt],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d],[motion command fail]",
					mtno, (int)m_command.parameter[i].pos, (int)MT[mtno].m_state.cmdCnt, MT[mtno].m_nxtIdx, MT[mtno].m_curIdx);
			}
			return;
		}

		for (int i = 0; i < m_command.axisCount; i++)
		{
			AxisList mtno = (AxisList)m_command.parameter[i].no;

			MT[mtno].SLogEndEvent();
			MT[mtno].SLogStartEvent();

			LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[cmdPos],[%d],[cmdCnt],[%d],[nxtIdx],[%.2d],[curIdx],[%.2d],[y-trigger]",
				mtno, (int)m_command.parameter[i].pos, (int)MT[mtno].m_state.cmdCnt, MT[mtno].m_nxtIdx, MT[mtno].m_curIdx);

			MT[mtno].m_moveTime = 0;
			MT[mtno].m_fsmDrv.Set(C_POS_END);
			MT[mtno].m_fsmDrv.Delay(0);
		}
	}

	//-------------------------------------------------------------------
	BOOL MtSync::Set(void)
	{
		if (m_master == m_slave)
			return (false);

		m_wmxErrCode = WMX.coremotion.sync->SetSyncMasterSlave(m_master, m_slave);
		return(!!m_wmxErrCode);
	}

	BOOL MtSync::SetGroup(void)
	{
		if (m_master == m_slave)
			return (false);

		wmx3Api::Sync::SyncGroup syncGr;
		syncGr.masterAxis = m_master;
		syncGr.slaveAxis[0] = m_slave;
		syncGr.servoOnOffSynchronization = 1;
		m_wmxErrCode = WMX.coremotion.sync->SetSyncGroup(m_groupId, syncGr);
		return(!!m_wmxErrCode);
	}

	BOOL MtSync::Resolve(void)
	{
		if (m_master == m_slave)
			return (false);

		m_wmxErrCode = WMX.coremotion.sync->ResolveSync(m_slave);
		return(!!m_wmxErrCode);
	}
}

#ifndef CHECK_ACTUATOR_ARRAY
HW::Motor MT[HW::Motor::maxDevice];
HW::InterpolationMotor LN[HW::InterpolationMotor::maxDevice];
HW::MtSync GT[HW::Motor::maxDevice];
#else
CheckParser<HW::Motor, AxisList, AxisList(HW::Motor::maxDevice)> MT;
CheckParser<HW::InterpolationMotor, InterpolationMotorList, InterpolationMotorList(HW::InterpolationMotor::maxDevice)> LN;
#endif