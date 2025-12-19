#pragma once
#include "..\COM\FSM.h"

#define MAX_ERROR_ARRAY		10

namespace COM
{
	template<class T> class CError
	{
	private:
		void Sort(void);
		void Swap(int nStart, int nEnd);

	public:
		char m_strName[100];
		int m_nErrCode;
		int m_err[MAX_ERROR_ARRAY];

		void Save(T nErrCode);
		void Del(T nErrCode);
		void Clear(void);
		BOOL Find(T nErrCode);
		PINT GetAddr(void);
		int GetNo(int nError = 0);
		int GetCnt(void);

	public:
		CError()
		{
			ZeroMemory(this, sizeof(CError));
		}
		~CError() {}
	};

	template<class T> void CError<T>::Sort(void)
	{
		for (int i = 0; i < MAX_ERROR_ARRAY; i++)
		{
			Swap(i, MAX_ERROR_ARRAY);
		}
	}

	template<class T> void CError<T>::Swap(int nStart, int nEnd)
	{
		int nCnt = 0;
		int nTmp = 0;

		for (nCnt = nStart + 1; nCnt < nEnd; nCnt++)
		{
			if (m_err[nStart] == m_err[nCnt])
			{
				m_err[nCnt] = 0;
			}

			if (0 < m_err[nCnt])
			{
				if ((m_err[nStart] > m_err[nCnt]) || (0 == m_err[nStart]))
				{
					nTmp = m_err[nStart];
					m_err[nStart] = m_err[nCnt];
					m_err[nCnt] = nTmp;
				}
			}
		}
	}

	template<class T> void CError<T>::Save(T nErrCode)
	{
		if (TRUE == Find(nErrCode) || nErrCode <= 0)
			return;

		if ((m_err[MAX_ERROR_ARRAY - 1] == 0) || (m_err[MAX_ERROR_ARRAY - 1] > nErrCode))
		{
			m_err[MAX_ERROR_ARRAY - 1] = nErrCode;
			Sort();
		}
	}

	template<class T> void CError<T>::Del(T nErrCode)
	{
		for (int i = 0; i < MAX_ERROR_ARRAY; i++)
		{
			if (m_err[i] == nErrCode)
			{
				m_err[i] = 0;
				Sort();
				break;
			}
		}
	}

	template<class T> void CError<T>::Clear(void)
	{
		for (int i = 0; i < MAX_ERROR_ARRAY; i++)
		{
			m_err[i] = 0;
		}
	}

	template<class T> BOOL CError<T>::Find(T nErrCode)
	{
		for (int i = 0; i < MAX_ERROR_ARRAY; i++)
		{
			if (m_err[i] == nErrCode)
				return (TRUE);
		}

		return (FALSE);
	}

	template<class T> PINT CError<T>::GetAddr(void)
	{
		return (&m_err[0]);
	}

	template<class T> int CError<T>::GetNo(int nError)
	{
		return (m_err[nError]);
	}

	template<class T> int CError<T>::GetCnt(void)
	{
		int nCnt = 0;

		for (int i = 0; i < MAX_ERROR_ARRAY; i++)
		{
			if (0 != m_err[i])
				nCnt++;
		}

		return (nCnt);
	}
}