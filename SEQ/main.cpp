#define _MAIN_CPP_
#include "Includes.h"

//## main.cpp 파일은 자동화 장비 프로그램이 하나만 실행되도록 보장하고,
//## 높은 우선순위로 실행 환경을 구축한 뒤,
//## InitSequence를 통해 하드웨어를 준비하고 MainSequence를 통해 장비의 핵심 동작을 시작하는 운영체제와의 인터페이스 역할을 수행

//## Ctrl+C, 시스템 종료, 로그오프 등 콘솔 제어 신호가 들어오면, CMain::isClose = true 플래그를 설정하고, 메인 스레드가 작업을 정리할 수 있도록 60초(Sleep(60000)) 대기 후 종료를 진행
BOOL CtrlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		{

		}
		break;
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
		{

		}
		break;
	}
	CMain::isClose = true; //## CLASS Cmain (BaseAll.h) 의 정적 멤버 변수 isClose를 true로 설정하여, 프로그램 종료 절차 시작)
	Sleep(60000);
	return (FALSE);
}

BOOL DisableQuickEditMode(void) //## 사용자가 콘솔을 클릭했을 때 프로그램 실행이 일시 정지되는 것을 방지
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD prev_mode = 0;
	if (GetConsoleMode(hInput, &prev_mode))
	{
		DWORD new_mode = prev_mode & ~ENABLE_QUICK_EDIT_MODE;
		if (SetConsoleMode(hInput, new_mode))
			return (TRUE);
	}
	return (FALSE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		DisableQuickEditMode();
		CMain::isClose = false;
		CMain::Event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"KOSES_SEQUENCE");
		if (CMain::Event)
			throw CException(CException::EXCUTED, __LINE__, __FUNCTION__); //## 이미 동일한 프로그램이 실행 중인 경우, 이중 실행 차단

		CMain::Event = CreateEvent(NULL, FALSE, FALSE, L"KOSES_SEQUENCE");
		if (CMain::Event)
		{
			ResetEvent(CMain::Event);
			SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CException::ExceptionFilter);
			SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
			HANDLE hProcess = GetCurrentProcess();
			if (NULL == hProcess)
			{
				throw CException(CException::NO_PROCESS, __LINE__, __FUNCTION__);
			}
			SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS); //## 프로세스의 우선순위를 높음(HIGH으로 설정하여, 운영체제에서 다른 일반 작업보다 이 프로그램에 더 많은 CPU 시간을 할당하도록 보장
			timeBeginPeriod(1); //## 고해상도 타이머를 활성화하고 설정하여, 정밀한 시간 측정 및 갱신을 준비
			QueryPerformanceFrequency(&CTimer::Frequency());
			CTimer::Update();
			CSysTm::Update();
		}
		else
		{
			throw CException(CException::EVENT_NULL, __LINE__, __FUNCTION__);
		}
		InitSequence(); //## Init.cpp 메인 함수 호출 - 장비의 모든 컴포넌트들을 설정하고 초기화
		MainSequence(); //## SEQ.cpp 메인 루프 호출 - 장비의 주된 동작(예: 생산 공정, 반복 작업)을 처리하는 메인 루프
	}
	catch (CException& e)
	{
		//## InitSequence()나 MainSequence() 실행 중 CException이 발생하면, 역시 CMain::isClose = true를 설정. 이후 logMAX 개수만큼 모든 로깅 채널을 순회하며 LOG[lgno].EndEvent()를 호출해 로깅을 안전하게 종료
		CMain::isClose = true;
		for (int i = 0; i < logMAX; i++)
		{
			LogList lgno = LogList(i);
			LOG[lgno].EndEvent();
		}
		Sleep(2000);
		e.End();
	}
	CMain::isClose = true;
	Sleep(2000);
	return 0;
}