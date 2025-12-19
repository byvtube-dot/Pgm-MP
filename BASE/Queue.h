#pragma once
#include <windows.h>

template<class T, size_t maxCnt = 10000> class CQueue
{
private:
	unsigned int		m_st;
	unsigned int		m_ed;
	T					data[maxCnt];

public:
	void push(T result);
	T pop(void);
	T peek(void);
	T back(void);
	bool empty(void);
	size_t size(void);
	T* flush(void);
	void clear(void);

	CQueue& operator=(const CQueue& q)
	{
		int size = sizeof(CQueue<T>);
		memcpy(this, &q, size);
		return *this;
	}

	CQueue();
	virtual ~CQueue();
};

template<class T, size_t maxCnt> CQueue<T, maxCnt>::CQueue()
{
	m_st = m_ed = 0;
	ZeroMemory(&data, sizeof(data));
};

template<class T, size_t maxCnt> CQueue<T, maxCnt>::~CQueue()
{

};

template<class T, size_t maxCnt> void CQueue<T, maxCnt>::push(T result)
{
	if (maxCnt <= (int)size())
		pop();

	if ((m_ed / maxCnt) > (m_st / maxCnt))
	{
		if (((m_ed + 1) % maxCnt) < (m_st % maxCnt))
		{
			data[m_ed % maxCnt] = result;
			m_ed++;
		}
	}
	else if (m_ed >= m_st)
	{
		data[m_ed % maxCnt] = result;
		m_ed++;
	}
}

template<class T, size_t maxCnt> T CQueue<T, maxCnt>::pop(void)
{
	T result;
	ZeroMemory(&result, sizeof(T));
	if (!empty())
	{
		result = data[m_st % maxCnt];
		m_st++;
	}
	if ((maxCnt <= m_st) && (maxCnt <= m_ed))
	{
		m_st = m_st % maxCnt;
		m_ed = m_ed % maxCnt;
	}
	return result;
}

template<class T, size_t maxCnt> T CQueue<T, maxCnt>::peek(void)
{
	T result;
	ZeroMemory(&result, sizeof(T));
	if (!empty())
	{
		result = data[m_st % maxCnt];
	}
	if ((maxCnt <= m_st) && (maxCnt <= m_ed))
	{
		m_st = m_st % maxCnt;
		m_ed = m_ed % maxCnt;
	}
	return result;
}

template<class T, size_t maxCnt> T CQueue<T, maxCnt>::back(void)
{
	T result;
	ZeroMemory(&result, sizeof(T));

	if (!empty())
	{
		result = data[(m_ed - 1) % maxCnt];
	}

	return result;
}

template<class T, size_t maxCnt> bool CQueue<T, maxCnt>::empty(void)
{
	if (m_st == m_ed)
		return true;

	return false;
}

template<class T, size_t maxCnt> size_t CQueue<T, maxCnt>::size(void)
{
	if (m_st < m_ed)
		return (size_t)(m_ed - m_st);

	return (size_t)0;
}

template<class T, size_t maxCnt> T* CQueue<T, maxCnt>::flush(void)
{
	T result[maxCnt];
	ZeroMemory(&result, sizeof(result));
	int maxCnt = (int)size();
	for (int i = 0; i < maxCnt; i++)
	{
		if (empty())
			break;

		result[i] = pop();
	}
	return &result[0];
}

template<class T, size_t maxCnt> void CQueue<T, maxCnt>::clear(void)
{
	m_st = m_ed = 0;
}