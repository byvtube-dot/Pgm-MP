#include "kamelasbase.h"
#include "sharedmemory.h"

namespace kamelaslib
{


KamelasBase::~KamelasBase()
{
}

KamelasBase*	__stdcall	CreateKamelas(BOOL bServer, TCHAR* lpszName, UINT32 cbSize)
{
	KamelasBase* pBase = NULL;
	pBase = new kamelaslib::SharedMemory(lpszName, cbSize, bServer);
	return pBase;
}

void __stdcall	DestroyKamelas(KamelasBase* pBase)
{
	delete pBase;
}


}//namespace


