#include "..\Includes.h"

namespace COM
{
	CLog::CLog()
	{
		m_Key = NOT_DEFINED;
		ZeroMemory(m_buffer, sizeof(m_buffer));
		for (int i = 0; i < LOG_KEY_CNT; i++)
		{
			m_hIpc[i] = NULL;
			m_hEvent[i] = NULL;
			m_hExitEvent[i] = NULL;
		}
		::InitializeCriticalSection(&comm_cs);
	}

	CLog::~CLog()
	{
		for (int i = 0; i < LOG_KEY_CNT; i++)
		{
			if (NULL != m_hIpc[i])
				CloseHandle(m_hIpc[i]);
			if (NULL != m_hEvent[i])
				CloseHandle(m_hEvent[i]);
			if (NULL != m_hExitEvent[i])
				CloseHandle(m_hExitEvent[i]);
		}
	}

	UINT CLog::Update0(LPVOID lpParam)
	{
		CLog* ptrLog = reinterpret_cast<CLog*>(lpParam);
		DWORD interval = 1;
		while (!CMain::isClose)
		{
			ptrLog->Run(0);
			Sleep(interval);
		}

		if (NULL != ptrLog->ExitEvent(0))
			SetEvent(ptrLog->ExitEvent(0));

		CMain::isClose = true;
		return 0;
	}

	UINT CLog::Update1(LPVOID lpParam)
	{
		CLog* ptrLog = reinterpret_cast<CLog*>(lpParam);
		DWORD interval = 1;
		while (!CMain::isClose)
		{
			ptrLog->Run(1);
			Sleep(interval);
		}

		if (NULL != ptrLog->ExitEvent(1))
			SetEvent(ptrLog->ExitEvent(1));

		CMain::isClose = true;
		return 0;
	}

	void CLog::Update(void)
	{
		if (NOT_DEFINED == m_Key)
			return;

		if (isWaiting[!m_Key])
		{
			if (1 == isWaiting[!m_Key] && tmUpdate[!m_Key].TmOver(1000) || 2 == isWaiting[!m_Key])
			{
				isWaiting[!m_Key] = 0;
				tmUpdate[!m_Key].Reset();
			}
			return;
		}

		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hIpc[!m_Key], 0))
		{
			if (0 == m_buffer[!m_Key].cnt)
			{
				isLocking[!m_Key] = TRUE;
				tmUpdate[!m_Key].Reset();
				m_Key = !m_Key;
				ReleaseSemaphore(m_hIpc[!m_Key], 1, NULL);
				isLocking[!m_Key] = FALSE;
				tmUpdate[!m_Key].Reset();
			}
			else
			{
				ReleaseSemaphore(m_hIpc[!m_Key], 1, NULL);
				SetEvent(m_hEvent[!m_Key]);
				tmUpdate[!m_Key].Reset();
			}
		}
		else if (tmUpdate[!m_Key].TmOver(5000))
		{
			ResetEvent(m_hEvent[!m_Key]);
			ReleaseSemaphore(m_hIpc[!m_Key], 1, NULL);
			isLocking[!m_Key] = FALSE;
			isWaiting[!m_Key] = 1;
			tmUpdate[!m_Key].Reset();
		}
	}

	void CLog::EndEvent(void)
	{
		for (int i = 0; i < LOG_KEY_CNT; i++)
		{
			if (NULL != m_hIpc[i])
				ReleaseSemaphore(m_hIpc[i], 1, NULL);
			if (NULL != m_hEvent[i])
				SetEvent(m_hEvent[i]);
		}
	}

	HANDLE CLog::ExitEvent(int no)
	{
		return m_hExitEvent[no];
	}

	void CLog::Run(int no)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent[no], INFINITE))
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(m_hIpc[no], INFINITE))
			{
				CEnterCriticalSection m(&comm_cs);
				SYSTEMTIME sysTime;
				char strDate[50];
				GetLocalTime(&sysTime);
				sprintf(strDate, "[%.2d:%.2d:%.2d-%.2d:%.2d:%.2d:%.3d]",
					sysTime.wYear % 100, sysTime.wMonth, sysTime.wDay,
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

				for (int i = 0; i < m_buffer[no].cnt; i++)
					m_spdlog->info("{:s} {:s}", strDate, m_buffer[no].message[i].string);

				m_buffer[no].cnt = 0;
				ReleaseSemaphore(m_hIpc[no], 1, NULL);
			}
		}
	}

	BOOL CLog::Init(LPCWSTR lpName, LPCWSTR lpFolder, LPCWSTR lpFile, BOOL rotating, BOOL display, int nCnt)
	{
		if (NULL == lpFolder || NULL == lpFile)
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		CreateDirectory(L"C:\\KOSES", NULL);
		CreateDirectory(L"C:\\KOSES\\History", NULL);
		CreateDirectory(L"C:\\KOSES\\History\\Sequence", NULL);
		WCHAR strFolderPath[MAX_PATH] = { L"C:\\KOSES\\History\\Sequence" };
		wcscat(strFolderPath, L"\\");
		wcscat(strFolderPath, lpFolder);
		CreateDirectory(strFolderPath, NULL);
		wcscat(strFolderPath, L"\\");
		wcscat(strFolderPath, lpFile);
		wcscat(strFolderPath, L".txt");

		std::vector<spdlog::sink_ptr> sinks;
		typedef spdlog::sinks::daily_file_sink<spdlog::details::null_mutex> dateonly_daily_file_sink_st;

		if (display)
		{
			sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		}

		if (rotating)
		{
			sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
				GetAscCode(strFolderPath), 1048576 * 20, nCnt)); // 파일 경로, 사이즈, 수량
		}
		else
		{
			sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(
				GetAscCode(strFolderPath), 0, 0)); // 파일 경로, 시간, 분
		}

		m_spdlog = std::make_shared<spdlog::logger>(GetAscCode((LPWSTR)lpName), begin(sinks), end(sinks));
		spdlog::register_logger(m_spdlog);
		spdlog::set_pattern("%v");
		m_spdlog->flush_on(spdlog::level::trace);

		WCHAR tmpMapName[LOG_KEY_CNT][MAX_PATH] = { 0, };
		WCHAR tmpIpcName[LOG_KEY_CNT][MAX_PATH] = { 0, };
		WCHAR tmpEventName[LOG_KEY_CNT][MAX_PATH] = { 0, };
		WCHAR tmpExitEventName[LOG_KEY_CNT][MAX_PATH] = { 0, };
		for (int i = 0; i < LOG_KEY_CNT; i++)
		{
			swprintf_s(tmpMapName[i], L"%s_MAP_%02d", lpName, i);
			swprintf_s(tmpIpcName[i], L"%s_IPC_%02d", lpName, i);
			swprintf_s(tmpEventName[i], L"%s_EVENT_%02d", lpName, i);
			swprintf_s(tmpExitEventName[i], L"%s_EXIT_EVENT_%02d", lpName, i);

			m_hIpc[i] = CreateSemaphore(NULL, 1, 1, tmpIpcName[i]);
			if (NULL == m_hIpc[i])
				throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
			m_hEvent[i] = CreateEvent(NULL, FALSE, FALSE, tmpEventName[i]);
			if (NULL == m_hEvent[i])
				throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
			m_hExitEvent[i] = CreateEvent(NULL, FALSE, FALSE, tmpExitEventName[i]);
			if (NULL == m_hExitEvent[i])
				throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);

			ResetEvent(m_hEvent[i]);
			ResetEvent(m_hExitEvent[i]);
			isLocking[i] = FALSE;
			isWaiting[i] = 2;
			tmUpdate[i].Reset();
		}
		if (!m_Thread[0].Run(NULL, (LPTHREAD_START_ROUTINE)Update0, LPVOID(this), NULL))
			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
		if (!m_Thread[1].Run(NULL, (LPTHREAD_START_ROUTINE)Update1, LPVOID(this), NULL))
			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);

		m_Key = 0;
		return (TRUE);
	}

	void CLog::Message(const char* strText, ...)
	{
		if (NOT_DEFINED == m_Key)
			return;

		if (isWaiting[m_Key])
		{
			if (1 == isWaiting[m_Key] && tmUpdate[m_Key].TmOver(5000) || 2 == isWaiting[m_Key])
			{
				isWaiting[m_Key] = 0;
				tmUpdate[m_Key].Reset();
			}
			return;
		}

		if (isLocking[m_Key] || WAIT_OBJECT_0 == WaitForSingleObject(m_hIpc[m_Key], 0))
		{
			if (LOG_BUFFER_CNT > m_buffer[m_Key].cnt)
			{
				ZeroMemory(m_buffer[m_Key].message[m_buffer[m_Key].cnt].string, LOG_STR_SIZE);
				va_list args;
				va_start(args, strText);
				vsnprintf(m_buffer[m_Key].message[m_buffer[m_Key].cnt].string, LOG_STR_SIZE, strText, args);
				m_buffer[m_Key].cnt++;
				va_end(args);
			}
			isLocking[m_Key] = TRUE;
			tmUpdate[m_Key].Reset();
		}
		else if (tmUpdate[m_Key].TmOver(5000))
		{
			ResetEvent(m_hEvent[m_Key]);
			ReleaseSemaphore(m_hIpc[m_Key], 1, NULL);
			isLocking[m_Key] = FALSE;
			isWaiting[m_Key] = 1;
			tmUpdate[m_Key].Reset();
		}
	}

	void CLog::Message(string strText)
	{
		if (NOT_DEFINED == m_Key)
			return;

		if (isWaiting[m_Key])
		{
			if (1 == isWaiting[m_Key] && tmUpdate[m_Key].TmOver(5000) || 2 == isWaiting[m_Key])
			{
				isWaiting[m_Key] = 0;
				tmUpdate[m_Key].Reset();
			}
			return;
		}

		if (isLocking[m_Key] || WAIT_OBJECT_0 == WaitForSingleObject(m_hIpc[m_Key], 0))
		{
			if (LOG_BUFFER_CNT > m_buffer[m_Key].cnt)
			{
				ZeroMemory(m_buffer[m_Key].message[m_buffer[m_Key].cnt].string, LOG_STR_SIZE);
				size_t size = LOG_STR_SIZE < strText.size() ? LOG_STR_SIZE : strText.size();
				memcpy(m_buffer[m_Key].message[m_buffer[m_Key].cnt].string, strText.c_str(), size);
				m_buffer[m_Key].cnt++;
			}
			isLocking[m_Key] = TRUE;
			tmUpdate[m_Key].Reset();
		}
		else if (tmUpdate[m_Key].TmOver(5000))
		{
			ResetEvent(m_hEvent[m_Key]);
			ReleaseSemaphore(m_hIpc[m_Key], 1, NULL);
			isLocking[m_Key] = FALSE;
			isWaiting[m_Key] = 1;
			tmUpdate[m_Key].Reset();
		}
	}
}
