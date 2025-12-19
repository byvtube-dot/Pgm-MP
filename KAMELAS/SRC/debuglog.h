#ifndef DEBUGLOGH
#define DEBUGLOGH

#pragma warning(disable:4996)

#include <windows.h>
#include <stdio.h>


namespace kamelaslib
{

/** Kamelas Debug Logger
	*
	*	2009.7
	*	Hong Chan, Choi
	*	hccoi@koses.co.kr
	*
	* 디버그(DEBUG) 모드로 컴파일시 출력창(Output Window)으로 디버깅 관련 정보를
	* 출력해주는 유틸리티 함수
	*
	*
	*/


void DebugLog(INT32 code, TCHAR* lpMessage, ...);


};

#endif
