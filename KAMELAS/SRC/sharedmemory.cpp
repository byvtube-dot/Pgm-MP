#include "sharedmemory.h"
#include "kamelasconfig.h"
#include "debuglog.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <algorithm>

#ifdef _MSC_BUILD
#pragma comment(lib, "winmm.lib")
#endif

namespace kamelaslib
{

#define SIGNITURE(name)		_T(#name)
#define TARGET_RESOLUTION	(1)         // 1-millisecond target resolution

class SharedMemoryPimpl
{
public:
	UINT				wTimerRes;	
	HANDLE				hMap;
    HANDLE				hMutex;						
	HANDLE				hRecvEvent[2];		/// 수신시 해당 이벤트 대기	/// auto reset
	TCHAR				name[MAX_PATH];
	UINT32				size;							
	PBYTE				pBegin;
	PHEADER				pHeader;
	PBYTE				pData;
	BOOL				bServer;

	SharedMemoryPimpl()
		: hMap(NULL)
		, size(0)
		, bServer(FALSE)
	{
	    hMutex			= hMutex;
		hRecvEvent[0]	= hRecvEvent[1]	= NULL;
		pBegin			= NULL;
		pHeader			= NULL;
		pData			= NULL;	
	}
	virtual ~SharedMemoryPimpl()
	{}
};

SharedMemory::SharedMemory(LPCTSTR lpName, UINT32 size, BOOL bServer)
{  
	assert(NULL != lpName);
	assert(size > 0);	

	this->_pPimpl = new SharedMemoryPimpl;
	assert(this->_pPimpl);

	_tcscpy(this->_pPimpl->name, lpName);
	this->_pPimpl->size = size;

	this->_pPimpl->bServer = bServer;

	::TIMECAPS tc;
	if (timeGetDevCaps(&tc, sizeof(tc)) != TIMERR_NOERROR)
	{
		assert( !"kamelas : timer information cant't queiable");
	}

	this->_pPimpl->wTimerRes = std::min <UINT> ( std::max<UINT> (tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);

}

SharedMemory::~SharedMemory()
{
	this->close();
	delete this->_pPimpl;
}

BOOL __stdcall 	SharedMemory::send(PVOID pBuffer, UINT32 milliSeconds)
{
	assert( pBuffer != NULL);
    BOOL bSuccess = FALSE;
	::timeBeginPeriod(this->_pPimpl->wTimerRes);

	if (TRUE == this->_pPimpl->bServer)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject( this->_pPimpl->hMutex, milliSeconds))
		{
			::ResetEvent(_pPimpl->hRecvEvent[0]);
			::ResetEvent(_pPimpl->hRecvEvent[1]);
			::CopyMemory(this->_pPimpl->pData, pBuffer, this->_pPimpl->size);
			if (::ReleaseMutex(this->_pPimpl->hMutex))
			{
				bSuccess = ::SetEvent(_pPimpl->hRecvEvent[1]);
			}
		}
        else
			DebugLog(0, _TEXT("kamelas : %s shared memory server send fail"), _pPimpl->name);
	}
	else
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject( this->_pPimpl->hMutex, milliSeconds))
        {
            ::ResetEvent(_pPimpl->hRecvEvent[1]);
            ::ResetEvent(_pPimpl->hRecvEvent[0]);
            ::CopyMemory(this->_pPimpl->pData, pBuffer, this->_pPimpl->size);
			if (::ReleaseMutex(this->_pPimpl->hMutex))
			{
				bSuccess = ::SetEvent(_pPimpl->hRecvEvent[0]);
			}
        }
        else
			DebugLog(0, _TEXT("kamelas : %s shared memory client send fail"), _pPimpl->name);
    }

	::timeEndPeriod( this->_pPimpl->wTimerRes);
    return bSuccess;	
}


BOOL __stdcall 	SharedMemory::recv(PVOID pBuffer, UINT32 milliSeconds)
{
	assert( pBuffer != NULL);
	BOOL bSuccess = FALSE;

	::timeBeginPeriod(this->_pPimpl->wTimerRes);

	if (TRUE == this->_pPimpl->bServer)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(_pPimpl->hRecvEvent[0], milliSeconds))
		{
			if (WAIT_OBJECT_0 == ::WaitForSingleObject( this->_pPimpl->hMutex, milliSeconds))
			{
				::CopyMemory(pBuffer, this->_pPimpl->pData, this->_pPimpl->size);
				::ReleaseMutex( this->_pPimpl->hMutex);
				bSuccess = TRUE;
			}
            else
				DebugLog(0, _TEXT("kamelas : %s shared memory server recv fail"), _pPimpl->name);
		}
        //else
		//	DebugLog(0, _TEXT("%s shared memory server recv timeout"), _pPimpl->name);            
	}
	else
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(_pPimpl->hRecvEvent[1], milliSeconds))
		{
			if (WAIT_OBJECT_0 == ::WaitForSingleObject( this->_pPimpl->hMutex, milliSeconds))
			{
				::CopyMemory(pBuffer, this->_pPimpl->pData, this->_pPimpl->size);
				::ReleaseMutex( this->_pPimpl->hMutex);
				bSuccess = TRUE;
			}
            else
				DebugLog(0, _TEXT("kamelas : %s shared memory client recv fail"), _pPimpl->name);
		}
        //else
		//	DebugLog(0, _TEXT("%s shared memory client recv timeout, code = %d"), _pPimpl->name, dwRet);            
	}

	::timeEndPeriod( this->_pPimpl->wTimerRes);
	return bSuccess;	
}

LPCTSTR __stdcall SharedMemory::name() const
{
	return this->_pPimpl->name;
}

UINT32 __stdcall SharedMemory::size() const
{
	return this->_pPimpl->size;
}

BOOL __stdcall SharedMemory::isOpen()
{
	return NULL != this->_pPimpl->hMap;
}

BOOL __stdcall SharedMemory::open()
{
	assert(NULL == this->_pPimpl->hRecvEvent[0]);
	assert(NULL == this->_pPimpl->hRecvEvent[1]);

	TCHAR tmpMapName[MAX_PATH];
	TCHAR tmpMutexName[MAX_PATH];
	TCHAR tmpRecvEventName0[MAX_PATH];
	TCHAR tmpRecvEventName1[MAX_PATH];

	_tcscpy(tmpMapName, this->_pPimpl->name);
	_tcscat(tmpMapName, SIGNITURE(_FILEMAP) );
		
	_tcscpy(tmpMutexName, this->_pPimpl->name);
	_tcscat(tmpMutexName, SIGNITURE(_MUTEX) );

	_tcscpy(tmpRecvEventName0, this->_pPimpl->name);
	_tcscat(tmpRecvEventName0, SIGNITURE(_RECV_EVENT_0) );
	_tcscpy(tmpRecvEventName1, this->_pPimpl->name);
	_tcscat(tmpRecvEventName1, SIGNITURE(_RECV_EVENT_1) );

	///// 파일 매핑 실시
	this->_pPimpl->hMap = ::OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, tmpMapName );	
	if (NULL == this->_pPimpl->hMap)
	{
		DebugLog(0, _TEXT("kamelas : %s shared memory try to create"), _pPimpl->name);
		this->_pPimpl->hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, 
													NULL, 
													PAGE_READWRITE | SEC_COMMIT, 
													0, 
													this->_pPimpl->size + sizeof(HEADER), 
													tmpMapName );
		if (NULL == this->_pPimpl->hMap)
		{
			DebugLog(-1, _TEXT("kamelas : %s shared memory fail to open or creation !"),_pPimpl->name);
			return FALSE;
		}

		/// first contact
		this->_pPimpl->hMutex		= ::CreateMutex(NULL, FALSE, tmpMutexName);
		assert(NULL != this->_pPimpl->hMutex);
		this->_pPimpl->hRecvEvent[0]= ::CreateEvent(NULL, FALSE, FALSE, tmpRecvEventName0);
		assert(NULL != this->_pPimpl->hRecvEvent[0]);
		this->_pPimpl->hRecvEvent[1]= ::CreateEvent(NULL, FALSE, FALSE, tmpRecvEventName1);
		assert(NULL != this->_pPimpl->hRecvEvent[1]);

		this->_pPimpl->pBegin = (PBYTE)::MapViewOfFile(this->_pPimpl->hMap, FILE_MAP_ALL_ACCESS, 0, 0, this->_pPimpl->size  + sizeof(HEADER) );
		assert(this->_pPimpl->pBegin && _TEXT("kamelas : invalid the pointer of begin"));

		/// initialize the header
		::ZeroMemory(this->_pPimpl->pBegin, sizeof(HEADER) + this->_pPimpl->size);

		this->_pPimpl->pHeader	= (PHEADER)this->_pPimpl->pBegin;
		this->_pPimpl->pData	= this->_pPimpl->pBegin + sizeof(HEADER);
		::InterlockedExchange((volatile LONG*)&this->_pPimpl->pHeader->headerLength	, sizeof(HEADER));
		::InterlockedExchange((volatile LONG*)&this->_pPimpl->pHeader->version		, KAMELAS_VERSION);
		::InterlockedExchange((volatile LONG*)&this->_pPimpl->pHeader->totalLength	, sizeof(HEADER) + this->_pPimpl->size);

	}
	else
	{
		/// attach to the peer
		DebugLog(0, _TEXT("kamelas : %s shared memory already exist"), _pPimpl->name);
		this->_pPimpl->hMutex		= ::OpenMutex(MUTEX_ALL_ACCESS , FALSE, tmpMutexName);
		assert(NULL != this->_pPimpl->hMutex);
		this->_pPimpl->hRecvEvent[0]= ::OpenEvent(EVENT_ALL_ACCESS , FALSE, tmpRecvEventName0);
		assert(NULL != this->_pPimpl->hRecvEvent[0]);
		this->_pPimpl->hRecvEvent[1]= ::OpenEvent(EVENT_ALL_ACCESS , FALSE, tmpRecvEventName1);
		assert(NULL != this->_pPimpl->hRecvEvent[1]);

		this->_pPimpl->pBegin = (PBYTE)::MapViewOfFile(this->_pPimpl->hMap, FILE_MAP_ALL_ACCESS, 0, 0, this->_pPimpl->size  + sizeof(HEADER) );
		assert(this->_pPimpl->pBegin && _TEXT("kamelas : invalid the pointer of begin"));

		this->_pPimpl->pHeader	= (PHEADER)this->_pPimpl->pBegin;
		this->_pPimpl->pData	= this->_pPimpl->pBegin + sizeof(HEADER);

		/// 헤더 정보 검사
		if (this->_pPimpl->pHeader->headerLength != sizeof(HEADER) )
		{
			DebugLog(-1, _TEXT("kamelas : %s shared memory header length mismatched"), _pPimpl->name);
			return FALSE;
		}

		if (KAMELAS_VERSION != this->_pPimpl->pHeader->version)
		{
			DebugLog(-1, _TEXT("kamelas : %s shared memory header version mismatched"), _pPimpl->name);
			return FALSE;
		}

		if (this->_pPimpl->pHeader->totalLength != (sizeof(HEADER) + this->_pPimpl->size))
		{
			DebugLog(-1, _TEXT("kamelas : %s shared memory total legnth mismatched"), _pPimpl->name);
			return FALSE; 
		}		
	}

	assert(this->_pPimpl->hMap != NULL && _TEXT("kamelas : fail to open or create the file mapping"));

	if (NULL == this->_pPimpl->pBegin)
	{
		::CloseHandle(this->_pPimpl->hMap);
		this->_pPimpl->hMap = NULL;
		return FALSE;
	}

	/// 기존 이벤트 리셋
	::ResetEvent(_pPimpl->hRecvEvent[0]);
	::ResetEvent(_pPimpl->hRecvEvent[1]);
	::ReleaseMutex(this->_pPimpl->hMutex);	
	DebugLog(0, _TEXT("kamelas : %s shared memory opened. %d bytes size"), _pPimpl->name, this->_pPimpl->size);
	return TRUE;
}

BOOL __stdcall SharedMemory::close()
{
	if (false == this->isOpen())
		return FALSE;

	DebugLog(0, _TEXT("kamelas : %s shared memory closing"), _pPimpl->name);

	if (NULL != this->_pPimpl->pBegin)
	{
		::UnmapViewOfFile( (PBYTE)this->_pPimpl->pBegin);
		this->_pPimpl->pBegin = NULL;
	}

	if (NULL != this->_pPimpl->hMap && INVALID_HANDLE_VALUE	!= this->_pPimpl->hMap)
	{
		::CloseHandle(this->_pPimpl->hMap);
		this->_pPimpl->hMap = NULL;
	}

	::CloseHandle(this->_pPimpl->hRecvEvent[0]);
	::CloseHandle(this->_pPimpl->hRecvEvent[1]);
	this->_pPimpl->hRecvEvent[0] = NULL;
	this->_pPimpl->hRecvEvent[1] = NULL;

	::CloseHandle(this->_pPimpl->hMutex);
	this->_pPimpl->hMutex = NULL;

	this->_pPimpl->pData = NULL;
	this->_pPimpl->pHeader = NULL;
	return TRUE;
}

}//namespace