#include "kamelasfactory.h"
#include "sharedmemory.h"
#include <cassert>


namespace kamelaslib
{

KamelasFactory::KamelasFactory()
{
}

KamelasFactory::~KamelasFactory()
{
}

KamelasBase* KamelasFactory::create(const KAMELASFLAG& flag)
{
	KamelasBase* pBase = NULL;

	switch(flag.type)
	{
	case TYPE_SHAREDMEMORY_SERVER:
		pBase = new SharedMemory(flag.name, flag.size,TRUE);
		break;
	case TYPE_SHAREDMEMORY_CLIENT:
		pBase = new SharedMemory(flag.name, flag.size,FALSE);
		break;
	}

	return pBase;
}

void  KamelasFactory::remove(KamelasBase* pBase)
{
    delete pBase;
}

}//namespace

void WINAPI GetFactory(kamelaslib::KamelasFactory** pFactory)
{
	static kamelaslib::KamelasFactory factory;
	*pFactory = &factory;
}

