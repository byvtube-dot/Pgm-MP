#include "..\Includes.h"

namespace Robot
{
	void CRobot::RobotIOTimer(void)
	{
		for (int no = 0; no < 16; no++)
		{
			if (m_robotIo.robotDi & RobotBitMask[no])
			{
				if (!(m_robotIo.memRobotDi & RobotBitMask[no]))
				{
					LOG[logROBOT].Message("[%s], [digital input on : %d is on]", m_name.c_str(), no);
				}
				m_robotIo.tmRobotDiOff[no].Reset();
			}
			else
			{
				if (m_robotIo.memRobotDi & RobotBitMask[no])
				{
					LOG[logROBOT].Message("[%s], [digital input off : %d if off]", m_name.c_str(), no);
				}
				m_robotIo.tmRobotDiOn[no].Reset();
			}

			if (m_robotIo.robotDo & RobotBitMask[no])
			{
				if (!(m_robotIo.memRobotDo & RobotBitMask[no]))
				{
					LOG[logROBOT].Message("[%s], [digital output on : %d is on]", m_name.c_str(), no);
				}
				m_robotIo.tmRobotDoOff[no].Reset();
			}
			else
			{
				if (m_robotIo.memRobotDo & RobotBitMask[no])
				{
					LOG[logROBOT].Message("[%s], [digital output off : %d is off]", m_name.c_str(), no);
				}
				m_robotIo.tmRobotDoOn[no].Reset();
			}
		}

		memcpy(&m_robotIo.memRobotDi, &m_robotIo.robotDi, sizeof(m_robotIo.memRobotDi));
		memcpy(&m_robotIo.memRobotDo, &m_robotIo.robotDo, sizeof(m_robotIo.memRobotDo));
	}

	BOOL CRobot::IsOn(RobotDi ioNo, DWORD dwDelayTime)
	{
		if (m_robotIo.robotDi & RobotBitMask[ioNo])
		{
			if (!dwDelayTime || m_robotIo.tmRobotDiOn[ioNo].TmOver(dwDelayTime))
				return (TRUE);
		}

		return (FALSE);
	}
	BOOL CRobot::IsOff(RobotDi ioNo, DWORD dwDelayTime)
	{
		if (!IsOn(ioNo))
		{
			if (!dwDelayTime || m_robotIo.tmRobotDiOff[ioNo].TmOver(dwDelayTime))
				return (TRUE);
		}

		return (FALSE);
	}
	void CRobot::Set(RobotDi ioNo, BOOL isOn)
	{
		if (isOn)
			m_robotIo.robotDi |= RobotBitMask[ioNo];
		else
			m_robotIo.robotDi &= ~RobotBitMask[ioNo];
	}

	BOOL CRobot::IsOn(RobotDo ioNo, DWORD dwDelayTime)
	{
		if (m_robotIo.robotDo & RobotBitMask[ioNo])
		{
			if (!dwDelayTime || m_robotIo.tmRobotDoOn[ioNo].TmOver(dwDelayTime))
				return (TRUE);
		}

		return (FALSE);
	}
	BOOL CRobot::IsOff(RobotDo ioNo, DWORD dwDelayTime)
	{
		if (!IsOn(ioNo))
		{
			if (!dwDelayTime || m_robotIo.tmRobotDoOff[ioNo].TmOver(dwDelayTime))
				return (TRUE);
		}

		return (FALSE);
	}
	void CRobot::Set(RobotDo ioNo, BOOL isOn)
	{
		if (isOn)
			m_robotIo.robotDo |= RobotBitMask[ioNo];
		else
			m_robotIo.robotDo &= ~RobotBitMask[ioNo];
	}

	BOOL CRobot::AOn(RobotDi ioNo, DWORD dwDelayTime)
	{
		return IsOn((RobotDi)ioNo, dwDelayTime);
	}
	BOOL CRobot::AOff(RobotDi ioNo, DWORD dwDelayTime)
	{
		return IsOff((RobotDi)ioNo, dwDelayTime);
	}
	BOOL CRobot::BOn(RobotDi ioNo, DWORD dwDelayTime)
	{
		return IsOff((RobotDi)ioNo, dwDelayTime);
	}
	BOOL CRobot::BOff(RobotDi ioNo, DWORD dwDelayTime)
	{
		return IsOn((RobotDi)ioNo, dwDelayTime);
	}

	void CRobot::On(RobotDo ioNo)
	{
		Set((RobotDo)ioNo, TRUE);
	}
	void CRobot::Off(RobotDo ioNo)
	{
		Set((RobotDo)ioNo, FALSE);
	}
}