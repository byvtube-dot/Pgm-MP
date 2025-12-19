#pragma once
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <tchar.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <vector>
#include "tlhelp32.h"

using namespace std;

#define SLOG_VERSION			0x0001		// 프로토콜 버전
#define MAX_SLOG_MESSAGE_SIZE	1024
#define MAX_SLOG_QUEUE_SIZE		20000


#ifdef SLOGLIB_EXPORTS
#define DLLINTERFACE __declspec(dllexport)
#else
#define DLLINTERFACE __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	namespace sloglib
	{
#pragma pack(push, 1)
		class Header
		{
		public:
			UINT32			headerLength;	// 헤더크기 = sizeof(HEADER)
			UINT32			totalLength;	// 전체크기
			UINT32			version;		// 버전
			BYTE			padding[52];	// 예약됨(미래를 위하여)

		public:
			Header()
			{
				::ZeroMemory(this, sizeof(*this));
			};

			~Header()
			{

			};
		};

		class Data
		{
		public:
			char			logMessage[MAX_SLOG_QUEUE_SIZE][MAX_SLOG_MESSAGE_SIZE];
			int				count;

		public:
			Data()
			{
				ZeroMemory(this, sizeof(*this));
			};

			~Data()
			{

			};

			Data& operator=(const Data& q)
			{
				int size = sizeof(*this);
				memcpy(this, &q, size);
				return *this;
			}

			bool push(const char* message)
			{
				if (MAX_SLOG_QUEUE_SIZE <= count)
					return false;

				memcpy(logMessage[count], message, sizeof(logMessage[count]));
				count++;
				return true;
			}

			const char* pop(void)
			{
				if (!empty())
					return logMessage[--count];

				return "";
			}

			const char* peek(void)
			{
				if (!empty())
					return logMessage[count - 1];

				return "";
			}

			const char* back(void)
			{
				if (!empty())
					return logMessage[0];

				return "";
			}

			bool empty(void)
			{
				if (1 > count)
					return true;

				return false;
			}
		};
#pragma pack(pop)

		class DLLINTERFACE SMemoryBase
		{
		public:
			virtual BOOL	__stdcall open() = 0;
			virtual BOOL	__stdcall close() = 0;
			virtual BOOL	__stdcall recv(Data& data, UINT32 milliSeconds) = 0;
			virtual BOOL	__stdcall send(Data& data, UINT32 milliSeconds) = 0;
			virtual ~SMemoryBase();
		};

		DLLINTERFACE SMemoryBase*	__stdcall	CreateSMemory(const char* mapName, const char* mmiName = NULL, const char* seqName = NULL);
		DLLINTERFACE void			__stdcall	DestroySMemory(SMemoryBase* pBase);

		class DLLINTERFACE SLogBase
		{
		public:
			virtual bool	__stdcall logOverFlow() = 0;
			virtual Data&	__stdcall logData() = 0;
			virtual bool	__stdcall logProcess(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* lotId, const char* recipeId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual bool	__stdcall logTransfer(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* materialType, const char* fromDevice, const char* toDevice, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual bool	__stdcall logFunction(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* materialType, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual bool	__stdcall logLot(const char* deviceId, const char* eventId, const char* lotId, const char* recipeId, const char* carrierId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual bool	__stdcall logConfigure(const char* deviceId, const char* cfgId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual bool	__stdcall logAlarm(const char* deviceId, const char* eventId, const char* alarmCode, const char* status, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual bool	__stdcall logComm(const char* deviceId, const char* eventId, const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL, const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL, const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL) = 0;
			virtual ~SLogBase();
		};

		DLLINTERFACE SLogBase*		__stdcall	CreateSLog(SYSTEMTIME* pSystemTime);
		DLLINTERFACE void			__stdcall	DestroySLog(SLogBase* pBase);
	}//namespace
#ifdef __cplusplus
}
#endif  //__cplusplus