#pragma once
#include "..\Defines.h"
#include "..\..\BASE\BaseAll.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#define LOG_KEY_CNT			2
#define LOG_STR_SIZE		400
#define LOG_BUFFER_CNT		2000

namespace COM
{
	class CLog
	{
	private:
		typedef struct
		{
			char			string[LOG_STR_SIZE];
		}MESSAGE;

		typedef struct _BUFFER
		{
			int				cnt;
			MESSAGE			message[LOG_BUFFER_CNT];
		}BUFFER;

		int					m_Key;
		int					isLocking[LOG_KEY_CNT];
		int					isWaiting[LOG_KEY_CNT];

		BUFFER				m_buffer[LOG_KEY_CNT];
		HANDLE				m_hIpc[LOG_KEY_CNT];
		HANDLE				m_hEvent[LOG_KEY_CNT];
		HANDLE				m_hExitEvent[LOG_KEY_CNT];
		CThread				m_Thread[LOG_KEY_CNT];
		CTimer				tmUpdate[LOG_KEY_CNT];

		CRITICAL_SECTION	comm_cs;

		std::shared_ptr<spdlog::logger> m_spdlog;

		static UINT Update0(LPVOID lpParam);
		static UINT Update1(LPVOID lpParam);
		void Run(int no);

	public:
		void Update(void);
		void EndEvent(void);
		HANDLE ExitEvent(int no);
		BOOL Init(LPCWSTR lpName, LPCWSTR lpFolder, LPCWSTR lpFile, BOOL rotating, BOOL display, int nCnt = 30);
		void Message(const char* strText, ...);
		void Message(string strText);

		CLog();
		virtual ~CLog();
	};
}