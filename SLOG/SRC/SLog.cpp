#include "SLog.h"

namespace sloglib
{
	void DebugLog(char* lpMessage, ...)
	{
#ifdef _DEBUG
		char buffer[1024] = { 0 };
		va_list args;
		va_start(args, lpMessage);
		vsnprintf(buffer, 1024, lpMessage, args);
		va_end(args);

		::OutputDebugString(buffer);
#ifdef _MSC_VER
		::OutputDebugString("\n");
#endif
#endif
	}

	class SMemoryPimpl
	{
	public:
		HANDLE				hMap;
		HANDLE				hSemaphore;
		char				mapName[MAX_PATH];
		char				mmiName[MAX_PATH];
		char				seqName[MAX_PATH];
		Header* pHeader;
		Data* pData;

		SMemoryPimpl()
		{
			ZeroMemory(this, sizeof(*this));
		}
		~SMemoryPimpl()
		{}
	};

	SMemory::SMemory(const char* mapName, const char* mmiName, const char* seqName)
	{
		assert(NULL != mapName);
		this->_pPimpl = new SMemoryPimpl;
		assert(this->_pPimpl);
		strcpy(this->_pPimpl->mapName, mapName);
		if (NULL != mmiName)
			strcpy(this->_pPimpl->mmiName, mmiName);
		if (NULL != seqName)
			strcpy(this->_pPimpl->seqName, seqName);
	}

	SMemory::~SMemory()
	{
		this->close();
		delete this->_pPimpl;
	}

	BOOL __stdcall SMemory::open()
	{
		char tmpMapName[MAX_PATH] = { 0 };
		strcpy(tmpMapName, this->_pPimpl->mapName);
		strcat(tmpMapName, "_FILEMAP");
		char tmpSemaphoreName[MAX_PATH] = { 0 };
		strcpy(tmpSemaphoreName, this->_pPimpl->mapName);
		strcat(tmpSemaphoreName, "_SEMEPHORE");
		char tmpRecvEventName0[MAX_PATH] = { 0 };
		strcpy(tmpRecvEventName0, this->_pPimpl->mapName);
		strcat(tmpRecvEventName0, "_RECV_EVENT_0");
		char tmpRecvEventName1[MAX_PATH] = { 0 };
		strcpy(tmpRecvEventName1, this->_pPimpl->mapName);
		strcat(tmpRecvEventName1, "_RECV_EVENT_1");

		// 파일 매핑 실시
		this->_pPimpl->hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, tmpMapName);
		if (NULL == this->_pPimpl->hMap)
		{
			DebugLog("slog : %s shared memory try to create", this->_pPimpl->mapName);
			this->_pPimpl->hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE | SEC_COMMIT,
				0,
				sizeof(Header) + sizeof(Data),
				tmpMapName);
			if (NULL == this->_pPimpl->hMap)
			{
				DebugLog("slog : %s shared memory fail to open or creation !", this->_pPimpl->mapName);
				return FALSE;
			}

			// first contact
			this->_pPimpl->hSemaphore = ::CreateSemaphore(NULL, 1, 3, tmpSemaphoreName);
			assert(NULL != this->_pPimpl->hSemaphore);
			this->_pPimpl->pHeader = (Header*)::MapViewOfFile(this->_pPimpl->hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Header) + sizeof(Data));
			assert(this->_pPimpl->pHeader && "slog : invalid the pointer of begin");

			// initialize
			::ZeroMemory(this->_pPimpl->pHeader, sizeof(Header) + sizeof(Data));
			this->_pPimpl->pData = (Data*)((PBYTE)this->_pPimpl->pHeader + sizeof(Header));

			this->_pPimpl->pHeader->headerLength = sizeof(Header);
			this->_pPimpl->pHeader->version = SLOG_VERSION;
			this->_pPimpl->pHeader->totalLength = sizeof(Header) + sizeof(Data);
		}
		else
		{
			// attach to the peer
			this->_pPimpl->hSemaphore = ::OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, tmpSemaphoreName);
			assert(NULL != this->_pPimpl->hSemaphore);
			this->_pPimpl->pHeader = (Header*)::MapViewOfFile(this->_pPimpl->hMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Header) + sizeof(Data));
			assert(this->_pPimpl->pHeader && "slog : invalid the pointer of begin");

			this->_pPimpl->pData = (Data*)((PBYTE)this->_pPimpl->pHeader + sizeof(Header));

			// 헤더 정보 검사
			if (this->_pPimpl->pHeader->headerLength != sizeof(Header))
			{
				DebugLog("slog : %s shared memory header length mismatched", this->_pPimpl->mapName);
				return FALSE;
			}
			if (SLOG_VERSION != this->_pPimpl->pHeader->version)
			{
				DebugLog("slog : %s shared memory header version mismatched", this->_pPimpl->mapName);
				return FALSE;
			}
			if (this->_pPimpl->pHeader->totalLength != (sizeof(Header) + sizeof(Data)))
			{
				DebugLog("slog : %s shared memory total legnth mismatched", this->_pPimpl->mapName);
				return FALSE;
			}
		}

		assert(this->_pPimpl->hMap != NULL && "slog : fail to open or create the file mapping");

		if (NULL == this->_pPimpl->pHeader)
		{
			::CloseHandle(this->_pPimpl->hMap);
			this->_pPimpl->hMap = NULL;
			return FALSE;
		}
		DebugLog("slog : %s shared memory opened. %d bytes size", this->_pPimpl->mapName, this->_pPimpl->pHeader->totalLength);
		return TRUE;
	}

	BOOL __stdcall SMemory::close()
	{
		if (NULL == this->_pPimpl->hMap)
			return FALSE;

		DebugLog("slog : %s shared memory closing", this->_pPimpl->mapName);

		if (NULL != this->_pPimpl->pHeader)
			::UnmapViewOfFile((PBYTE)this->_pPimpl->pHeader);

		this->_pPimpl->pHeader = NULL;
		this->_pPimpl->pData = NULL;

		if (NULL != this->_pPimpl->hMap && INVALID_HANDLE_VALUE != this->_pPimpl->hMap)
			::CloseHandle(this->_pPimpl->hMap);

		this->_pPimpl->hMap = NULL;

		if (NULL != this->_pPimpl->hSemaphore)
			::CloseHandle(this->_pPimpl->hSemaphore);

		this->_pPimpl->hSemaphore = NULL;

		return TRUE;
	}

	BOOL __stdcall SMemory::send(Data& data, UINT32 milliSeconds)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(this->_pPimpl->hSemaphore, milliSeconds))
		{
			int index = this->_pPimpl->pData->count;
			int remainCount = MAX_SLOG_QUEUE_SIZE - index;
			if (data.count <= remainCount)
			{
				int copySize = MAX_SLOG_MESSAGE_SIZE * data.count;
				this->_pPimpl->pData->count += data.count;
				data.count = 0;
				memcpy(this->_pPimpl->pData->logMessage[index], data.logMessage, copySize);
			}
			return ::ReleaseSemaphore(this->_pPimpl->hSemaphore, 1, NULL);
		}
		return FALSE;
	}

	BOOL __stdcall SMemory::recv(Data& data, UINT32 milliSeconds)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(this->_pPimpl->hSemaphore, milliSeconds))
		{
			data.count = this->_pPimpl->pData->count;
			this->_pPimpl->pData->count = 0;
			int copySize = MAX_SLOG_MESSAGE_SIZE * data.count;
			memcpy(data.logMessage, this->_pPimpl->pData->logMessage, copySize);
			return ::ReleaseSemaphore(this->_pPimpl->hSemaphore, 1, NULL);
		}
		else
		{
			if (NULL != this->_pPimpl->mmiName && NULL != this->_pPimpl->seqName)
			{
				HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				PROCESSENTRY32 processEntry32;
				processEntry32.dwSize = sizeof(PROCESSENTRY32);
				if (!Process32First(hProcessSnap, &processEntry32))
					CloseHandle(hProcessSnap);

				BOOL isEnable1 = FALSE;
				BOOL isEnable2 = FALSE;
				char mmiName[MAX_PATH] = { 0 };
				strcat(mmiName, this->_pPimpl->mmiName);
				strcat(mmiName, ".exe");
				char seqName[MAX_PATH] = { 0 };
				strcat(seqName, this->_pPimpl->seqName);
				strcat(seqName, ".exe");

				int count = 0;
				while (Process32Next(hProcessSnap, &processEntry32))
				{
					if (0 == strcmp(processEntry32.szExeFile, mmiName))
						isEnable1 = TRUE;

					if (0 == strcmp(processEntry32.szExeFile, seqName))
						isEnable2 = TRUE;

					count++;
					if (10000 <= count)
						break;
				}
				if (!isEnable1 && !isEnable2)
				{
					DebugLog("slog : %s semaphore count is invalid. Resetting semaphore", this->_pPimpl->mapName);
					ReleaseSemaphore(this->_pPimpl->hSemaphore, 1, NULL);
				}
			}
		}
		return FALSE;
	}

	class SLogPimpl
	{
	public:
		Data				data;
		bool				overflow;
		SYSTEMTIME*			pSystemTime;

		SLogPimpl()
		{
			ZeroMemory(this, sizeof(*this));
		}
		~SLogPimpl()
		{}
	};

	SLog::SLog(SYSTEMTIME* pSystemTime)
	{
		this->_pPimpl = new SLogPimpl;
		assert(this->_pPimpl);
		this->_pPimpl->pSystemTime = pSystemTime;
	}

	SLog::~SLog()
	{
		if (NULL != this->_pPimpl)
			delete this->_pPimpl;

		this->_pPimpl = NULL;
	}

	bool __stdcall SLog::logOverFlow()
	{
		if (this->_pPimpl->overflow)
		{
			this->_pPimpl->overflow = false;
			return true;
		}
		return false;
	}

	Data& __stdcall SLog::logData()
	{
		return (Data&)*_pPimpl;
	}

	bool __stdcall SLog::logProcess(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* lotId, const char* recipeId, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "PRC", eventId, status, materialId, lotId, recipeId);
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "PRC", eventId, status, materialId, lotId, recipeId);
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}

	bool __stdcall SLog::logTransfer(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* materialType, const char* fromDevice, const char* toDevice, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "XFR", eventId, status, materialId, materialType, fromDevice, toDevice);
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "XFR", eventId, status, materialId, materialType, fromDevice, toDevice);
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}

	bool __stdcall SLog::logFunction(const char* deviceId, const char* eventId, const char* status, const char* materialId, const char* materialType, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "FNC", eventId, status, materialId, materialType);
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "FNC", eventId, status, materialId, materialType);
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}

	bool __stdcall SLog::logLot(const char* deviceId, const char* eventId, const char* lotId, const char* recipeId, const char* carrierId, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "LEH", eventId, lotId, recipeId, carrierId);	//foup, cassett rfid
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "LEH", eventId, lotId, recipeId, carrierId);	//foup, cassett rfid
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}

	bool __stdcall SLog::logConfigure(const char* deviceId, const char* cfgId, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "CFG", cfgId);	//check, change, generate		
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "CFG", cfgId);	//check, change, generate		
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}

	bool __stdcall SLog::logAlarm(const char* deviceId, const char* eventId, const char* alarmCode, const char* status, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "ALM", eventId, alarmCode, status);	// occured/release
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "ALM", eventId, alarmCode, status);	// occured/release
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}

	bool __stdcall SLog::logComm(const char* deviceId, const char* eventId, const char* key1, const char* value1, const char* key2, const char* value2, const char* key3, const char* value3, const char* key4, const char* value4, const char* key5, const char* value5, const char* key6, const char* value6)
	{
		char strMessage[1024] = { 0 };
		if (NULL == this->_pPimpl->pSystemTime)
		{
			SYSTEMTIME st;
			::GetLocalTime(&st);
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "COMM", eventId);	//ERROR, WARRING, USER_STOP, EQP_STOP
		}
		else
		{
			SYSTEMTIME& st = *this->_pPimpl->pSystemTime;
			sprintf(strMessage, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t'%s'\t'%s'\t'%s'",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				deviceId, "COMM", eventId);	//ERROR, WARRING, USER_STOP, EQP_STOP
		}

		if (NULL != key1)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key1, value1);
		}
		if (NULL != key2)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key2, value2);
		}
		if (NULL != key3)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key3, value3);
		}
		if (NULL != key4)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key4, value4);
		}
		if (NULL != key5)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key5, value5);
		}
		if (NULL != key6)
		{
			strcat(strMessage, "\t");
			sprintf(strMessage + strlen(strMessage), "(%s,%s)", key6, value6);
		}

		if (this->_pPimpl->data.push(strMessage))
			return true;

		this->_pPimpl->overflow = true;
		return false;
	}
}//namespace