#include "SLogBase.h"
#include "SLog.h"

namespace sloglib
{
	SMemoryBase::~SMemoryBase()
	{

	}

	SMemoryBase* __stdcall	CreateSMemory(const char* mapName, const char* mmiName, const char* seqName)
	{
		SMemoryBase* pBase = NULL;
		pBase = new SMemory(mapName, mmiName, seqName);
		return pBase;
	}

	void __stdcall	DestroySMemory(SMemoryBase* pBase)
	{
		delete pBase;
	}

	SLogBase::~SLogBase()
	{

	}

	SLogBase* __stdcall	CreateSLog(SYSTEMTIME* pSystemTime)
	{
		SLogBase* pBase = NULL;
		pBase = new SLog(pSystemTime);
		return pBase;
	}

	void __stdcall	DestroySLog(SLogBase* pBase)
	{
		delete pBase;
	}
}//namespace


