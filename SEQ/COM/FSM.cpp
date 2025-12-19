#include "..\Includes.h"

namespace COM
{
	CFSM::CFSM()
	{
		ZeroMemory(this, sizeof(*this));
		m_shouldRstDelay = TRUE;
		m_shouldRstTimeLimit = TRUE;
		m_nMsg = NOT_DEFINED;
	}

	CFSM::~CFSM()
	{

	}

	void CFSM::Set(int nCmd, int nMsg)
	{
		if (NOT_DEFINED != nMsg)
			m_nMsg = nMsg;

		if (C_IDLE == nCmd)
		{
			m_isCancel = FALSE;
			m_nMsg = NOT_DEFINED;
		}

		if (m_nCmd != nCmd)
		{
			m_shouldRstDelay = TRUE;
			m_shouldRstTimeLimit = TRUE;
			m_isOnce = TRUE;
		}
		
		m_nCmd = nCmd;
	}

	int CFSM::Get(void)
	{
		return m_nCmd;
	}

	int CFSM::IsBusy(void)
	{
		return m_nCmd;
	}

	BOOL CFSM::IsBusy(int nCmd, DWORD lDelay)
	{
		if (m_nCmd == nCmd)
		{
			if (0 >= lDelay || Delay(lDelay))
				return (TRUE);
		}
		return (FALSE);
	}

	BOOL CFSM::Between(int nMin, int nMax)
	{
		if (nMin > Get())
			return (FALSE);
		if (nMax < Get())
			return (FALSE);

		return (TRUE);
	}

	BOOL CFSM::Once(void)
	{
		BOOL bRet = m_isOnce;

		m_isOnce = FALSE;
		return (bRet);
	}

	BOOL CFSM::TimeLimit(DWORD lLimitTime)
	{
		if (m_shouldRstTimeLimit)
		{
			m_shouldRstTimeLimit = (FALSE);
			m_tmLimit.Reset();
			return (FALSE);
		}

		return m_tmLimit.TmOver(lLimitTime);
	}

	void CFSM::RstDelay(BOOL rstFlag)
	{
		m_shouldRstDelay = rstFlag;
		m_tmDelay.Reset();
	}

	void CFSM::RstTimeLimit(BOOL rstFlag)
	{
		m_shouldRstTimeLimit = rstFlag;
		m_tmLimit.Reset();
	}

	BOOL CFSM::Delay(DWORD lDelay)
	{
		if (m_shouldRstDelay)
		{
			m_shouldRstDelay = FALSE;
			m_tmDelay.Reset();
		}

		return (m_tmDelay.TmOver(lDelay));
	}

	BOOL CFSM::DelaySec(double secDelay)
	{
		if (m_shouldRstDelay)
		{
			m_shouldRstDelay = FALSE;
			m_tmDelay.Reset();
		}

		return (m_tmDelay.TmOverSec(secDelay));
	}

	double CFSM::Elapsed(void)
	{
		if (m_shouldRstDelay)
		{
			m_shouldRstDelay = FALSE;
			m_tmDelay.Reset();
		}

		return m_tmDelay.Elapsed();
	}

	double CFSM::ElapsedSec(void)
	{
		if (m_shouldRstDelay)
		{
			m_shouldRstDelay = FALSE;
			m_tmDelay.Reset();
		}

		double elapsedSec = (double)m_tmDelay.Elapsed() / 1000.;
		return elapsedSec;
	}

	int CFSM::GetMsg(void)
	{
		return (m_nMsg);
	}

	BOOL CFSM::IsCanceled(void)
	{
		return (m_isCancel);
	}

	void CFSM::Cancel(void)
	{
		if (C_IDLE < m_nCmd)
			m_isCancel = TRUE;
	}

	void CFSM::Init(std::function<void(LPVOID)> conditionEvent,
		std::function<void(const int&, const int&, LPVOID)> beforeEvent,
		std::function<void(const int&, const int&, LPVOID)> afterEvent, PVOID lpParam)
	{
		m_nCmd.Init(conditionEvent, beforeEvent, afterEvent, lpParam);
	}
}
