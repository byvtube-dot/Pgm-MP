#pragma once
#define _USE_MATH_DEFINES
#define NOMINMAX
#include <tchar.h>
#include <process.h>
#include <stdio.h>
#include <shlwapi.h>
#include <time.h>
#include <mmsystem.h>
#include <cassert>
#include <queue>
#include <math.h>
#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>
#include <memory>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <array>
#include <utility>
#include <string_view>
#include <atlbase.h>

using namespace std;

#pragma warning(disable : 4996)
#pragma warning(disable : 4828)
#pragma warning(disable : 4099)

#define NOT_DEFINED					(-1)
#define	MAX_NVM_SIZE				(100 * 1024 * 1000)

#include "MMF.h"
#include "Property.h"
#include "Queue.h"
#include "SanSu.h"
#include "Timer.h"
#include "List.h"
#include "TreeNode.h"

typedef struct tagOprFlag
{
	BOOL	isNoDevice;
	BOOL	isDryRun;
	BOOL	isDebugMode;
	BOOL	isWrongParam;
	BOOL	isEmg;
	BOOL	isStop;
	BOOL	isAuto;
	BOOL	isPaused;
	BOOL	isDoorOpen;
	BOOL	isDoorUnlock;
	BOOL	isCycleRun;
	BOOL	isAllHome;
	BOOL	isEmgMem;
	BOOL	isWrongPkg;
	BOOL	isWrongSys;
	BOOL	isWrongRcp;
	BOOL	isCheckErr;
	BOOL	isCheckPkg;
	BOOL	isCheckSys;
	BOOL	isCheckRcp;
	BOOL	isCanMoveMsg;
}OprFlag;

inline int UniToAscCode(const WCHAR* uniCode, const char* ascCode)
{
	int len = (int)wcslen(uniCode);
	if (1024 < len)
		len = 1024;

	wcstombs((char*)ascCode, uniCode, len);
	return len;
}

inline int AscToUniCode(const char* ascCode, const WCHAR* uniCode)
{
	int len = (int)strlen(ascCode);
	if (1024 < len)
		len = 1024;

	mbstowcs((WCHAR*)uniCode, ascCode, len);
	return len;
}

inline const char* GetAscCode(const WCHAR* string)
{
	static char strResult[2048] = { 0, };
	ZeroMemory(strResult, sizeof(strResult));
	UniToAscCode(string, strResult);
	return strResult;
}

inline const WCHAR* GetUniCode(const char* string)
{
	static WCHAR strResult[2048] = { 0, };
	ZeroMemory(strResult, sizeof(strResult));
	AscToUniCode(string, strResult);
	return strResult;
}

inline BOOL GetCurDirectory(LPWSTR lpName)
{
	ZeroMemory(lpName, sizeof(WCHAR) * MAX_PATH);
	GetModuleFileName(NULL, lpName, MAX_PATH);
	PathRemoveFileSpec(lpName);
	return (TRUE);
}

// 정수형 변환 (오버로딩)
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline std::string ToString(T _Val) 
{
	return std::to_string(_Val);
}

// 실수형 변환 (오버로딩)
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline std::string ToString(T _Val, int precision) 
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(precision) << _Val;
	return oss.str();
}

// 정수형 변환을 위한 오버로드 함수
template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline T ToNumber(const std::string& str, T defaultValue = T())
{
	std::istringstream iss(str);
	T value;

	if (!(iss >> value))
	{
		return defaultValue; // 변환 실패 시 기본값 반환
	}

	return value;
}

// 실수형 변환을 위한 오버로드 함수
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline std::string ToNumber(const std::string& str, int precision, T defaultValue = T())
{
	std::istringstream iss(str);
	T value;

	if (!(iss >> value))
	{
		return std::to_string(defaultValue); // 변환 실패 시 기본값 반환
	}

	// 문자열을 숫자로 변환한 후, 지정된 소수점 자리수로 설정
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(precision) << value;

	return oss.str(); // 변환된 문자열을 그대로 반환
}

template <typename T, T Value>
constexpr std::string_view EnumToStringImpl()
{
	constexpr std::string_view funcSig = __FUNCSIG__;
	constexpr std::string_view prefix = "std::string_view __cdecl EnumToStringImpl<";
	constexpr std::string_view suffix = ">(void)";

	size_t start = funcSig.find(prefix) + prefix.size();
	size_t end = funcSig.find(suffix);
	std::string_view name = funcSig.substr(start, end - start);

	size_t commaPos = name.find(',');
	if (commaPos != std::string_view::npos)
	{
		name.remove_prefix(commaPos + 1);
		while (name.starts_with(" ")) name.remove_prefix(1); // 공백 제거
	}

	size_t scopePos = name.rfind("::");
	if (scopePos != std::string_view::npos)
	{
		name.remove_prefix(scopePos + 2);
	}

	if (name.find(' ') != std::string_view::npos || name.starts_with("T)0x"))
	{
		return "";
	}

	return name;
}

template <typename T, std::size_t LastValue, std::size_t... Indices>
constexpr auto GenerateEnumStringsImpl(std::index_sequence<Indices...>)
{
	return std::array<std::string_view, sizeof...(Indices)>
	{
		EnumToStringImpl<T, static_cast<T>(Indices)>()...
	};
}

template <typename T, typename ListType, std::size_t Start, std::size_t... Indices>
void processBatch(ListType& list, std::index_sequence<Indices...>)
{
	constexpr std::array<std::pair<T, std::string_view>, sizeof...(Indices)> buffer =
	{
		std::make_pair(static_cast<T>(Start + Indices), EnumToStringImpl<T, static_cast<T>(Start + Indices)>())...
	};

	for (const auto& [value, name] : buffer)
	{
		if (name.empty())
			continue;

		list.emplace_back(value, std::string(name));
	}
}

template <typename T, typename ListType, std::size_t Start, std::size_t End>
void populateListWithEnumImpl(ListType& list)
{
	constexpr std::size_t BatchSize = 1024;
	if (Start < End)
	{
		constexpr std::size_t NextBatch = (Start + BatchSize < End) ? Start + BatchSize : End;
		processBatch<T, ListType, Start>(list, std::make_index_sequence<NextBatch - Start>{});
		populateListWithEnumImpl<T, ListType, NextBatch, End>(list);
	}
}

template <typename T, std::size_t LastValue>
inline auto ToStrings()
{
	std::vector<std::pair<T, std::string>> list;
	if (std::is_enum<T>::value)
	{
		populateListWithEnumImpl<T, std::vector<std::pair<T, std::string>>, 0, LastValue>(list);
	}
	return list;
}

inline bool ExecuteFile(const std::wstring& filepath)
{
	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	if (CreateProcessW(filepath.c_str(),   // 실행할 파일 경로
		NULL,               // 명령줄 인자 (없으면 NULL)
		NULL, NULL,         // 보안 속성 (NULL 기본값)
		FALSE,              // 핸들 상속 여부
		0,                  // 프로세스 생성 플래그
		NULL,               // 환경 변수
		NULL,               // 현재 디렉토리
		&si, &pi))          // 프로세스 정보
	{
		std::wcout << L"프로그램 실행 성공, PID: " << pi.dwProcessId << std::endl;
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
	}
	else {
		std::wcerr << L"프로그램 실행 실패: " << GetLastError() << std::endl;
		return false;
	}
}

class CException
{
public:
	enum ErrorCode
	{
		NO_HANDLER		= 0,
		EXCUTED			= 1,
		EXCUTE_FAIL		= 2,
		NO_PROCESS		= 3,
		EVENT_NULL		= 4,
		INIT_FAIL		= 5,
		OUT_OF_RANGE	= 6,
		TIMED_OUT		= 7,
	};

private:
	static const WCHAR* ErrorCodeAsString(ErrorCode errorCode)
	{
		switch (errorCode)
		{
		case NO_HANDLER: return L"NO_HANDLER";
		case EXCUTED: return L"EXCUTED";
		case EXCUTE_FAIL: return L"EXCUTE_FAIL";
		case NO_PROCESS: return L"NO_PROCESS";
		case EVENT_NULL: return L"EVENT_NULL";
		case INIT_FAIL: return L"INIT_FAIL";
		case OUT_OF_RANGE: return L"OUT_OF_RANGE";
		case TIMED_OUT: return L"TIMED_OUT";
		default: return L"UNKNOWN";
		}
	}

public:
	CException(){}
	CException(ErrorCode errorCode, DWORD line, const char* function)
	{
		WCHAR errorMessage[1024] = { 0, };
		WCHAR errorFunction[1024] = { 0, };
		wcscpy(errorMessage, ErrorCodeAsString(errorCode));
		int len = (int)strlen(function) + 1;
		mbstowcs(errorFunction, function, len);

		WCHAR strFolderPath[MAX_PATH] = { 0, };
		if (!GetCurDirectory(strFolderPath))
			return;

		wcscat(strFolderPath, L"\\LOG");
		CreateDirectory(strFolderPath, NULL);

		time_t curTm;
		time(&curTm);
		tm* pTm = localtime(&curTm);

		WCHAR fileName[1024] = { 0, };
		swprintf_s(fileName, L"%s\\ETC_ERROR_%02d%02d%02d_%02d%02d%02d.ini", strFolderPath,
			(pTm->tm_year % 100), (pTm->tm_mon + 1), pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

		FILE* fp = _wfopen(fileName, L"w+");
		if (NULL != fp)
		{
			fwprintf(fp, L"\n ;Ver[%04d%02d%02d  %02d:%02d:%02d]",
				(pTm->tm_year % 100), (pTm->tm_mon + 1), pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
			fwprintf(fp, L"\n\n");
			fwprintf(fp, L"Error code : %03d \n", (int)errorCode);
			fwprintf(fp, L"Error name : %s \n", errorMessage);
			fwprintf(fp, L"Error line : %03d \n", (int)line);
			fwprintf(fp, L"Error function : %s \n", errorFunction);

			fclose(fp);
		}
		::MessageBox(NULL, fileName, L"Check the file log in the path below", MB_OK);
	}
	~CException(){}

	static const wchar_t* ExceptionCodeAsString(long lExceptionCode)
	{
		switch (lExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION: return L"EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_DATATYPE_MISALIGNMENT: return L"EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_BREAKPOINT: return L"EXCEPTION_BREAKPOINT";
		case EXCEPTION_SINGLE_STEP: return L"EXCEPTION_SINGLE_STEP";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_FLT_DENORMAL_OPERAND: return L"EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO: return L"EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT: return L"EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION: return L"EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW: return L"EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK: return L"EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW: return L"EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_INT_DIVIDE_BY_ZERO: return L"EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW: return L"EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_PRIV_INSTRUCTION: return L"EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR: return L"EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_ILLEGAL_INSTRUCTION: return L"EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return L"EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_STACK_OVERFLOW: return L"EXCEPTION_STACK_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION: return L"EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_GUARD_PAGE: return L"EXCEPTION_GUARD_PAGE";
		case EXCEPTION_INVALID_HANDLE: return L"EXCEPTION_INVALID_HANDLE";
		default: return L"EXCEPTION_UNKNOWN";
		}
	}

	static LONG ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
	{
		PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;
		PVOID addr = pExceptionRecord->ExceptionAddress;
		ULONG_PTR section = 0;
		ULONG_PTR offset = 0;

		ULONG_PTR hMod = (ULONG_PTR)&__ImageBase;
		if (NULL == hMod)
			return EXCEPTION_EXECUTE_HANDLER;

		PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;
		PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHdr);
		ULONG_PTR rva = (ULONG_PTR)addr ? ((ULONG_PTR)pExceptionRecord->ExceptionAddress - (ULONG_PTR)hMod) : NULL;
#ifdef WIN32
		ULONG_PTR imageBaseAddress = 0x40000;
#else
		ULONG_PTR imageBaseAddress = 0x140000000;
#endif
		for (unsigned int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++)
		{
			ULONG_PTR sectionStart = pSection->VirtualAddress;
			ULONG_PTR sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);
			if ((rva >= sectionStart) && (rva <= sectionEnd))
			{
				section = i + 1;
				offset = rva ? (rva - sectionStart) : NULL;
				break;
			}
		}

		long exceptionCode = pExceptionRecord->ExceptionCode;
		ULONGLONG exceptionAddress = (ULONG_PTR)pExceptionRecord->ExceptionAddress;
		ULONGLONG mapFileSection = section;
		ULONGLONG mapFileOffset = offset;
		ULONGLONG mapFileRva = rva + imageBaseAddress;

		WCHAR strFolderPath[MAX_PATH] = { 0, };
#ifdef UNDER_RTSS
		WCHAR strFolderPath1[MAX_PATH] = { 0, };
		HANDLE hProcess = GetCurrentProcess();
		size_t size1 = (size_t)RtGetModuleBaseName(hProcess, NULL, strFolderPath1, MAX_PATH);
		if (0 >= size1)
			return EXCEPTION_EXECUTE_HANDLER;

		WCHAR strFolderPath2[MAX_PATH] = { 0, };
		size_t size2 = (size_t)GetModuleFileName(NULL, strFolderPath2, MAX_PATH);
		if (0 >= size2)
			return EXCEPTION_EXECUTE_HANDLER;

		size_t size3 = size2 - size1 - 1;
		if (0 >= size3)
			return EXCEPTION_EXECUTE_HANDLER;

		memcpy(strFolderPath, strFolderPath2, sizeof(WCHAR) * size3);
#else
		GetModuleFileName(NULL, strFolderPath, MAX_PATH);
		PathRemoveFileSpec(strFolderPath);
#endif
		wcscat(strFolderPath, L"\\LOG");
		CreateDirectory(strFolderPath, NULL);

		time_t curTm;
		time(&curTm);
		tm* pTm = localtime(&curTm);

		WCHAR fileName[1024];
		swprintf_s(fileName, L"%s\\EXCEPTION_%02d%02d%02d_%02d%02d%02d.ini", strFolderPath,
			(pTm->tm_year % 100), (pTm->tm_mon + 1), pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

		FILE* fp = _wfopen(fileName, L"w+");
		fwprintf(fp, L"\n ;Ver[%04d%02d%02d  %02d:%02d:%02d]",
			(pTm->tm_year % 100), (pTm->tm_mon + 1), pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
		fwprintf(fp, L"\n\n");
		fwprintf(fp, L"Exception code : 0x%08llX \n", (ULONGLONG)exceptionCode);
		fwprintf(fp, L"Exception name : %s \n", ExceptionCodeAsString(exceptionCode));
		fwprintf(fp, L"Exception address : 0x%08llX \n", exceptionAddress);
		fwprintf(fp, L"Map File Exception Point Information. \n");
		fwprintf(fp, L"Address:[%04llx:%08llx],Rva + Base:[0x%08llx] \n", mapFileSection, mapFileOffset, mapFileRva);

		fclose(fp);
		throw CException();
		return EXCEPTION_EXECUTE_HANDLER;
	}

	static void End(void)
	{
		ExitProcess(0);
	}
};

class CEnterCriticalSection
{
private:
	CRITICAL_SECTION*	_pcs;

public:
	CEnterCriticalSection(CRITICAL_SECTION* pcs)
		: _pcs(pcs)
	{
		::EnterCriticalSection(_pcs);
	}
	~CEnterCriticalSection()
	{
		::LeaveCriticalSection(_pcs);
	}
};

class CTryEnterCriticalSection
{
private:
	BOOL				activated;
	CRITICAL_SECTION*	_pcs;

public:
	CTryEnterCriticalSection(CRITICAL_SECTION* pcs)
		: _pcs(pcs)
	{
		activated = ::TryEnterCriticalSection(_pcs);
	}
	~CTryEnterCriticalSection()
	{
		if (activated)
			::LeaveCriticalSection(_pcs);
	}

	BOOL Activated(void)
	{
		return activated;
	}
};

class CEnterMutex
{
private:
	HANDLE  _hMutex;
	DWORD	_dwMilliseconds;
	DWORD   _dwResult; // 인스턴스 변수로 변경된 결과 값

public:
	DWORD Result() const { return _dwResult; }

	CEnterMutex(HANDLE hMutex, DWORD dwMilliseconds)
		: _hMutex(hMutex), _dwMilliseconds(dwMilliseconds)
	{
		_dwResult = ::WaitForSingleObject(_hMutex, _dwMilliseconds);
	}

	virtual ~CEnterMutex()
	{
		if (WAIT_OBJECT_0 == _dwResult)
			::ReleaseMutex(_hMutex);
	}
};

class CThread
{
private:
	HANDLE	hHandle;
	UINT	threadId;

public:
	CThread()
	{

	}

	~CThread()
	{
		if (NULL != hHandle)
			CloseHandle(hHandle);

		hHandle = NULL;
	}

	BOOL Run(SIZE_T stackSize, LPTHREAD_START_ROUTINE startAddress, LPVOID argList, int nPriority)
	{
		hHandle = CreateThread(
			nullptr,			// 기본 보안 속성
			stackSize,			// 기본 스택 사이즈 (1MB : 최소 사이즈)
			startAddress,		// 쓰레드 함수
			argList,			// 쓰레드 함수의 매개변수를 위한 인자
			0,					// 디폴트 생성 flag
			(LPDWORD)&threadId	// 쓰레드 ID를 전달받기 위한 인자
		);
		assert(NULL != hHandle);
		if (NULL != nPriority)
			assert(!!SetThreadPriority(hHandle, nPriority));

		return (TRUE);
	}
};

class CMain
{
public:
	inline static HANDLE Event;
	inline static bool isClose;

	CMain()
	{

	};

	~CMain()
	{
		if (CMain::Event)
			CloseHandle(CMain::Event);
	}
};

class CNvm
{
private:
	CMMF	m_MMF;
	int		m_MaxSize;
	BOOL	m_isFlush;

public:
	CNvm()
	{
		m_MaxSize = 0;
		m_isFlush = FALSE;
	}

	CNvm(const WCHAR* strName, int size)
	{
		Init(strName, size);
	}

	~CNvm() {}

	void Flush(void)
	{
		m_isFlush = TRUE;
	}

	void Update(void)
	{
		if (m_isFlush)
			m_MMF.Flush();

		m_isFlush = FALSE;
	}

	PBYTE GetAddr(void)
	{
		return m_MMF.GetAddr();
	}

	BOOL Init(const WCHAR* strName, int size, const WCHAR* strCustomDirectory = NULL)
	{
		m_MaxSize = 0;
		m_isFlush = FALSE;
		if (1 > size || 0 < m_MaxSize || MAX_NVM_SIZE < size)
			return (FALSE);

		if (NULL != m_MMF.GetAddr())
			return (FALSE);

		WCHAR strPath[MAX_PATH] = { 0 };
		if (NULL == strCustomDirectory)
		{
			GetModuleFileName(NULL, strPath, MAX_PATH);
			PathRemoveFileSpec(strPath);
			wcscat(strPath, L"\\");
			wcscat(strPath, L"DAT");
			CreateDirectory(strPath, NULL);
			wcscat(strPath, L"\\");
			wcscat(strPath, strName);
			wcscat(strPath, L".dat");
		}
		else
		{
			wcscat(strPath, strCustomDirectory);
			wcscat(strPath, L"\\");
			wcscat(strPath, strName);
			wcscat(strPath, L".dat");
		}
		m_MMF.Init(strName, strPath, size);
		if (!m_MMF.Open())
			return (FALSE);

		if (NULL == m_MMF.GetAddr())
			return (FALSE);

		m_MaxSize = size;
		return (TRUE);
	}
};

template<typename T1, typename T2, T2 maxValue>
class CheckParser
{
private:
	T1 value[maxValue];

public:
	T1& operator[](T2 index)
	{
		if (!Between(index, 0, maxValue - 1))
			throw std::out_of_range("Index out of range");

		return value[index];
	}

	CheckParser() {};
	~CheckParser() {};
};