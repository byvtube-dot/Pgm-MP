#pragma once
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>

class CTimer
{
private:
	LARGE_INTEGER   m_startTime;
	LARGE_INTEGER   m_endTime;
	LARGE_INTEGER   m_elapsedTime;
	BOOL			m_useRealTime;

	static LARGE_INTEGER& GetTm(void)
	{
		static LARGE_INTEGER getTime;
		return (getTime);
	}

public:
	// 아래 함수 임의사용 금지!
	static LARGE_INTEGER& Frequency(void)
	{
		static LARGE_INTEGER frequency;
		return (frequency);
	}

	static void Update(void)
	{
		QueryPerformanceCounter(&GetTm());
	}

	void Init(BOOL useRealTime)
	{
		m_useRealTime = useRealTime;
		Reset();
	}

	BOOL Reset()
	{
		if (m_useRealTime)
		{
			QueryPerformanceCounter(&m_startTime);
		}
		else
			m_startTime = GetTm();

		m_endTime = m_startTime;
		return (TRUE);
	}

	BOOL Reset(BOOL shouldRst)
	{
		if (shouldRst)
			Reset();

		return (TRUE);
	}

	UINT64 Elapsed_nSec()
	{
		if (m_useRealTime)
		{
			QueryPerformanceCounter(&m_endTime);
		}
		else
			m_endTime = GetTm();

		m_elapsedTime.QuadPart = m_endTime.QuadPart - m_startTime.QuadPart;
		double duringtime = (double)m_elapsedTime.QuadPart / (double)CTimer::Frequency().QuadPart;
		return (UINT64)round(duringtime * 1000000);
	}

	double Elapsed_mSec()
	{
		double time = (double)Elapsed_nSec() / 1000.;
		return (time);
	}

	double ElapsedSec()
	{
		double time = (double)Elapsed_nSec() / 1000000.;
		return (time);
	}

	double Elapsed()
	{
		return Elapsed_mSec();		//milli-seconds
	}

	BOOL TmOver(DWORD dwDelay)
	{
		DWORD dwElapsedTime = (DWORD)Elapsed_mSec();

		if (dwDelay < dwElapsedTime)
			return (TRUE);

		return (FALSE);
	}

	BOOL TmOverSec(double secDelay)
	{
		DWORD msDelay = (DWORD)(secDelay * 1000.0);
		return (TmOver(msDelay));
	}

	CTimer()
	{
		m_useRealTime = FALSE;
		m_startTime = GetTm();
		m_endTime = m_startTime;
	}

	CTimer(BOOL useRealTime)
	{
		m_useRealTime = FALSE;
		m_startTime = GetTm();
		m_endTime = m_startTime;
		Init(useRealTime);
	}

	~CTimer()
	{

	}
};

class CBlinkTimer
{
private:
	CTimer m_timer;
	BOOL m_bBlink;

public:
	BOOL Blink(BOOL bBlinkOn, int nOnTime, int nOffTime)
	{
		if (!bBlinkOn)
		{
			m_timer.Reset();
			m_bBlink = (TRUE);
			return (FALSE);
		}

		if (m_bBlink)
		{
			if (!m_timer.TmOver(nOnTime))
				return (TRUE);

			m_timer.Reset();
			m_bBlink = (FALSE);
		}
		else
		{
			if (!m_timer.TmOver(nOffTime))
				return (FALSE);

			m_timer.Reset();
			m_bBlink = (TRUE);
		}

		return m_bBlink;
	}

	CBlinkTimer()
	{
		m_bBlink = FALSE;
	}

	~CBlinkTimer()
	{

	}
};

class CSysTm
{
private:
	static SYSTEMTIME* Tm(void)
	{
		static SYSTEMTIME getTime;
		return (&getTime);
	}

public:
	// 아래 함수 임의사용 금지!
	static void Update(void)
	{
		GetLocalTime(Tm());
	}

	static SYSTEMTIME& GetTm(void)
	{
		return *Tm();
	}

	static WCHAR* GetDate(void)
	{
		static WCHAR strDate[50];
		wsprintf(strDate, L"[%.2d:%.2d:%.2d-%.2d:%.2d:%.2d:%.3d]",
			Tm()->wYear % 100, Tm()->wMonth, Tm()->wDay,
			Tm()->wHour, Tm()->wMinute, Tm()->wSecond, Tm()->wMilliseconds);

		return strDate;
	}

	CSysTm() {};
	~CSysTm() {};
};