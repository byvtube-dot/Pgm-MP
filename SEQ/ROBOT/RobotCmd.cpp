#include "..\Includes.h"

namespace Robot
{
	void CRobot::Activation(void)
	{
		if (m_StateCmd.cmd[cmdRobotArmActivation] != cmdIdle)
			return;

		if (!m_robotState.activated)
			m_StateCmd.cmd[cmdRobotArmActivation] = AddressCmd + cmdRobotArmActivation;
	}

	void CRobot::Deactivation(void)
	{
		if (m_StateCmd.cmd[cmdPowerOffTheRobotArm] != cmdIdle)
			return;

		if (m_robotState.activated)
			m_StateCmd.cmd[cmdPowerOffTheRobotArm] = AddressCmd + cmdPowerOffTheRobotArm;
	}

	void CRobot::ChangeRealMode(void)
	{
		if (m_StateCmd.cmd[cmdChangeToRealMode] != cmdIdle)
			return;

		if (!m_robotState.realMode)
			m_StateCmd.cmd[cmdChangeToRealMode] = AddressCmd + cmdChangeToRealMode;
	}

	void CRobot::ClearSystemSos(void)
	{
		if (m_StateCmd.cmd[cmdResetSosFlag] != cmdIdle)
			return;

		if (m_robotState.realMode)
			m_StateCmd.cmd[cmdResetSosFlag] = AddressCmd + cmdResetSosFlag;
	}

	void CRobot::Pause(void)
	{
		if (m_StateCmd.cmd[cmdPauseProgram] != cmdIdle)
			return;

		if (!m_robotState.pauseState)
			m_StateCmd.cmd[cmdPauseProgram] = AddressCmd + cmdPauseProgram;
	}

	void CRobot::ResumePause(void)
	{
		if (m_StateCmd.cmd[cmdResumefromPause] != cmdIdle)
			return;

		if (m_robotState.pauseState)
			m_StateCmd.cmd[cmdResumefromPause] = AddressCmd + cmdResumefromPause;
	}

	void CRobot::ResumeCollision(void)
	{
		if (m_StateCmd.cmd[cmdResumefromCollision] != cmdIdle)
			return;

		if (m_robotState.collisionDetected || m_robotState.collisionState)
			m_StateCmd.cmd[cmdResumefromCollision] = AddressCmd + cmdResumefromCollision;
	}

	void CRobot::ProgramOnceStart(void)
	{
		if (m_StateCmd.cmd[cmdStartProgramOnce] != cmdIdle)
			return;

		if (!m_robotState.programRun)
			m_StateCmd.cmd[cmdStartProgramOnce] = AddressCmd + cmdStartProgramOnce;
	}

	void CRobot::ProgramRepeatStart(void)
	{
		if (m_StateCmd.cmd[cmdStartProgramRepeat] != cmdIdle)
			return;

		if (!m_robotState.programRun)
			m_StateCmd.cmd[cmdStartProgramRepeat] = AddressCmd + cmdStartProgramRepeat;
	}

	void CRobot::ProgramStop(void)
	{
		if (m_StateCmd.cmd[cmdStopProgram] != cmdIdle)
			return;

		if (m_robotState.programRun)
			m_StateCmd.cmd[cmdStopProgram] = AddressCmd + cmdStopProgram;
	}

	void CRobot::SetSpeed(int percent)
	{
		m_StateCmd.cmd[cmdSpeedBarChage] = static_cast<uint16_t>(percent);
	}

	void CRobot::LoadDefaultProgram(void)
	{
		if (m_robotState.moving || m_robotState.pauseState)
			return;
		if (m_robotState.programRun)
			return;

		m_StateCmd.cmd[cmdLoadDefaultProgram] = static_cast<uint16_t>(AddressCmd + cmdLoadDefaultProgram);
	}

	void CRobot::LoadProgram(int idx)
	{
		if (m_robotState.moving || m_robotState.pauseState)
			return;
		if (m_robotState.programRun)
			return;

		if (!Between(idx, 0, 9))
			return;

		int ProgramTable = AddressCmd + cmdLoadProgramTable + idx;
		m_StateCmd.cmd[cmdLoadProgramTable] = static_cast<uint16_t>(ProgramTable);
	}

	BOOL CRobot::StartHomeCommand(void)
	{
		if (m_robotState.moving || m_robotState.pauseState)
			return FALSE;
		if (!m_robotState.programRun)
			return FALSE;

		m_isHome = false;
		m_fsmRobot.Set(C_HOME_START);
		return TRUE;
	}

	void CRobot::Move(int idx)
	{
		if (!m_robotState.activated)
			return;
		if (!m_robotState.realMode)
			return;
		if (!m_robotState.programRun)
			return;

		if (!m_isHome || m_isErr)
			return;

		if (m_fsmRobot.IsBusy() || m_robotState.moving || m_robotState.pauseState)
			return;

		if (!Between(idx, 0, MAX_INDEX_ROBOT - 1))
			return;

		m_speed = SpeedTable(idx);
		m_writeCmdPos = 0.0;
		m_nxtIdx = idx;

		m_fsmRobot.Set(C_MOVE_START);
	}
	void CRobot::Move(int idx, double zOffset, int speed)
	{
		if (!m_robotState.activated)
			return;
		if (!m_robotState.realMode)
			return;
		if (!m_robotState.programRun)
			return;

		if (!m_isHome || m_isErr)
			return;

		if (m_fsmRobot.IsBusy() || m_robotState.moving || m_robotState.pauseState)
			return;

		if (!Between(idx, 0, MAX_INDEX_ROBOT - 1))
			return;

		if (speed == 0.0)
			m_speed = SpeedTable(idx);
		else
			m_speed = speed;

		m_writeCmdPos = zOffset;
		m_nxtIdx = idx;

		m_fsmRobot.Set(C_MOVE_START);
	}
	BOOL CRobot::IsRdy(int idx)
	{
		if (!m_robotState.activated)
			return(FALSE);
		if (!m_robotState.realMode)
			return(FALSE);
		if (!m_robotState.programRun)
			return(FALSE);
		if (!m_isHome)
			return(FALSE);

		if (m_fsmRobot.IsBusy() || m_robotState.moving || m_robotState.pauseState)
			return(FALSE);

		if (0 <= idx)
		{
			if (m_nxtIdx != idx || m_curIdx != idx)
				return (FALSE);
		}
		return (TRUE);
	}

	BOOL CRobot::JointInPos(int idx, double error)
	{
		if (m_nxtIdx != idx || !Between(idx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		RobotJoint jointIdxPos = JointTable(idx);
		if(!BetweenJoint(jointIdxPos, m_robotCurPos.joint, error))
			return (FALSE);

		return (TRUE);
	}

	BOOL CRobot::TcpInPos(int idx, double error)
	{
		if (m_curIdx != idx || !Between(idx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		RobotTcp tcpIdxPos = TcpTable(idx);
		if (!BetweenTcp(tcpIdxPos, m_robotCurPos.tcp, error))
			return (FALSE);

		return (TRUE);
	}

	BOOL CRobot::TcpInPos(int idx, RobotTcp idxOffset, double error)
	{
		if (m_nxtIdx != idx || !Between(idx, 0, MAX_INDEX_MOTOR - 1))
			return (FALSE);

		RobotTcp tcpOffsetPos = TcpTable(idx);
		tcpOffsetPos.tcpPx += idxOffset.tcpPx;
		tcpOffsetPos.tcpPy += idxOffset.tcpPy;
		tcpOffsetPos.tcpPz += idxOffset.tcpPz;
		tcpOffsetPos.tcpRx += idxOffset.tcpRx;
		tcpOffsetPos.tcpRy += idxOffset.tcpRy;
		tcpOffsetPos.tcpRz += idxOffset.tcpRz;

		if (!BetweenTcp(tcpOffsetPos, m_robotCurPos.tcp, error))
			return (FALSE);

		return (TRUE);
	}

	BOOL CRobot::CmdOrCurIdx(int posIdx)
	{
		return (m_nxtIdx == posIdx || m_curIdx == posIdx);
	}

	BOOL CRobot::CmdOrCurMovingIdx(int posIdx)
	{
		return (m_robotState.moving && (m_nxtIdx == posIdx || m_curIdx == posIdx));
	}

	BOOL CRobot::BetweenJoint(RobotJoint criterion, RobotJoint Comparison, double error)
	{
		if (!Between(criterion.joint_0, Comparison.joint_0 - error, Comparison.joint_0 + error))
			return (FALSE);
		if (!Between(criterion.joint_1, Comparison.joint_1 - error, Comparison.joint_1 + error))
			return (FALSE);
		if (!Between(criterion.joint_2, Comparison.joint_2 - error, Comparison.joint_2 + error))
			return (FALSE);
		if (!Between(criterion.joint_3, Comparison.joint_3 - error, Comparison.joint_3 + error))
			return (FALSE);
		if (!Between(criterion.joint_4, Comparison.joint_4 - error, Comparison.joint_4 + error))
			return (FALSE);
		if (!Between(criterion.joint_5, Comparison.joint_5 - error, Comparison.joint_5 + error))
			return (FALSE);
	
		return (TRUE);
	}
	BOOL CRobot::BetweenTcp(RobotTcp criterion, RobotTcp Comparison, double error)
	{
		if (!Between(criterion.tcpPx, Comparison.tcpPx - error, Comparison.tcpPx + error))
			return (FALSE);
		if (!Between(criterion.tcpPy, Comparison.tcpPy - error, Comparison.tcpPy + error))
			return (FALSE);
		if (!Between(criterion.tcpPz, Comparison.tcpPz - error, Comparison.tcpPz + error))
			return (FALSE);

		if (fabs(criterion.tcpRx) == 180.0)
		{
			if (!Between(criterion.tcpRx, Comparison.tcpRx - error, Comparison.tcpRx + error) &&
				!Between(-1 * criterion.tcpRx, Comparison.tcpRx - error, Comparison.tcpRx + error))
				return (FALSE);
		}
		else
		{
			if (!Between(criterion.tcpRx, Comparison.tcpRx - error, Comparison.tcpRx + error))
				return (FALSE);
		}
		if (fabs(criterion.tcpRy) == 180.0)
		{
			if (!Between(criterion.tcpRy, Comparison.tcpRy - error, Comparison.tcpRy + error) &&
				!Between(-1 * criterion.tcpRy, Comparison.tcpRy - error, Comparison.tcpRy + error))
				return (FALSE);
		}
		else
		{
			if (!Between(criterion.tcpRy, Comparison.tcpRy - error, Comparison.tcpRy + error))
				return (FALSE);
		}
		if (fabs(criterion.tcpRz) == 180.0)
		{
			if (!Between(criterion.tcpRz, Comparison.tcpRz - error, Comparison.tcpRz + error) &&
				!Between(-1 * criterion.tcpRz, Comparison.tcpRz - error, Comparison.tcpRz + error))
				return (FALSE);
		}
		else
		{
			if (!Between(criterion.tcpRz, Comparison.tcpRz - error, Comparison.tcpRz + error))
				return (FALSE);
		}

		return (TRUE);
	}

	int& CRobot::SpeedTable(int idx)
	{
		return(ROBOT_TABLE[m_robotNo].nVel[idx]);
	}

	RobotTcp& CRobot::TcpTable(int idx)
	{
		return (ROBOT_TABLE[m_robotNo].tcp[idx]);
	}

	RobotJoint& CRobot::JointTable(int idx)
	{
		return (ROBOT_TABLE[m_robotNo].joint[idx]);
	}
}