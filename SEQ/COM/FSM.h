#pragma once
#include "..\BASE\Timer.h"

#define C_IDLE			0
#define C_ERROR			(-1)

namespace COM
{
	class CFSM
	{
	private:
		int					m_nMsg;
		Property<int>		m_nCmd;
		BOOL				m_isOnce;
		BOOL				m_isCancel;
		BOOL				m_shouldRstDelay;
		BOOL				m_shouldRstTimeLimit;

		CTimer				m_tmDelay;
		CTimer				m_tmLimit;

	public:
		void Set(int nCmd, int nMsg = NOT_DEFINED);
		int Get(void);
		int IsBusy(void);
		BOOL IsBusy(int nCmd, DWORD lDelay = 0);
		BOOL Between(int nMin, int nMax);
		BOOL Once(void);
		BOOL TimeLimit(DWORD lLimitTime);
		void RstDelay(BOOL rstFlag = TRUE);
		void RstTimeLimit(BOOL rstFlag = TRUE);
		BOOL Delay(DWORD lDelay);
		BOOL DelaySec(double secDelay);
		double Elapsed(void);
		double ElapsedSec(void);
		int GetMsg(void);
		BOOL IsCanceled(void);
		void Cancel(void);
		void Init(std::function<void(LPVOID)> conditionEvent,
			std::function<void(const int&, const int&, LPVOID)> beforeEvent,
			std::function<void(const int&, const int&, LPVOID)> afterEvent, PVOID lpParam = nullptr);

		CFSM();
		virtual ~CFSM();
	};
}

