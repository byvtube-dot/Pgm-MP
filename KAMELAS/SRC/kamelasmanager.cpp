#include "kamelasmanager.h"
#include "debuglog.h"
#include <set>
#include <string>
#include <cassert>

namespace kamelaslib
{

/// 컨테이너 타입
typedef std::set<const KamelasBase*> CONTAINER;
typedef CONTAINER::iterator CONTAINER_IT;

class KamelasManagerPimpl
{
public:
	CONTAINER container;
	CRITICAL_SECTION cs;

	KamelasManagerPimpl()
	{
		::InitializeCriticalSection(&cs);
	}
	virtual ~KamelasManagerPimpl()
	{
		::DeleteCriticalSection(&cs);
	}
};

/// 크리티컬 섹션 동기화 객체 
/// 생성자, 소멸자를 이용한 안전한 자원해제 기법 
class Monitor
{
private:
	LPCRITICAL_SECTION	_pcs;
public:
	Monitor(LPCRITICAL_SECTION pcs)
		: _pcs(pcs)
	{
		assert(NULL != pcs);
		::EnterCriticalSection(_pcs);
	}
	~Monitor()
	{
		::LeaveCriticalSection(_pcs);
	}
};


KamelasManager::KamelasManager()
{
	_pPimpl = new KamelasManagerPimpl;
	assert(NULL != _pPimpl);

}

KamelasManager::~KamelasManager()
{
	this->clear();
	delete _pPimpl;
}

/// 인스턴스 개수
UINT32 KamelasManager::size()
{
	return (UINT32)_pPimpl->container.size();
}

/// 인스턴스 모두 제거
void KamelasManager::clear()
{
	Monitor m( &_pPimpl->cs);

	CONTAINER_IT it;

	while(false == _pPimpl->container.empty())
	{
		it = _pPimpl->container.begin();
		this->detach(*it);
	}
}

/// 인스턴스 추가
bool KamelasManager::attach(const KamelasBase *pBase)
{	
	Monitor m( &_pPimpl->cs);
			
	if (NULL != this->query(pBase->name()))
	{
		DebugLog(-1, _TEXT("kamelas : manager detect duplicated instance attached. so abort : %s"), pBase->name() );
		return false;
	}

	_pPimpl->container.insert( pBase);
	DebugLog(0, _TEXT("kamelas : instance attached : %s"), pBase->name());

	return true;
}

/// 인스턴스 제거
bool KamelasManager::detach(const KamelasBase *pBase)
{
	Monitor m( &_pPimpl->cs);
				
	CONTAINER_IT it = _pPimpl->container.find(pBase);
		
	if ( it == _pPimpl->container.end() )
	{
		DebugLog(-1, _TEXT("kamelas : fail to search the instance : %s"), pBase->name());			
		return false;
	}

	DebugLog(0, _TEXT("kamelas : detach the instance : %s"), pBase->name());
	const_cast<KamelasBase*>(*it)->close();
	delete (*it);
	_pPimpl->container.erase( it );

	return TRUE;
}

/// 인스턴스 이름으로 조회
KamelasBase* KamelasManager::query(LPCTSTR lpName)
{
	Monitor m( &_pPimpl->cs);

	CONTAINER_IT it = _pPimpl->container.begin();
		
	for ( ; it != _pPimpl->container.end();  ++it)
	{
		if (!_tcscmp((*it)->name() , lpName))
		{
			return const_cast<KamelasBase*>(*it);
		}
	}		

	return NULL;
}

/// 인스턴스 인덱스로 조회
KamelasBase*	KamelasManager::at(UINT32 index)
{
	Monitor m( &_pPimpl->cs);
	CONTAINER_IT it;
	it = _pPimpl->container.begin();
		
	for (UINT i=0 ; i < index;  ++i, ++it)
		if (it == _pPimpl->container.end())
			return NULL;
			
	return const_cast<KamelasBase*>(*it);	
}

/// 인스턴스 추가
bool KamelasManager::operator+=(const KamelasBase* pBase)
{
	return this->attach(pBase);
}

/// 인스턴스 제거
bool KamelasManager::operator-=(const KamelasBase* pBase)
{
	return this->detach(pBase);
}

/// 인스턴스 이름으로 조회
KamelasBase& KamelasManager::operator[] (LPCTSTR lpName)
{
	assert(NULL != lpName);
	return *(const_cast<KamelasBase*>(this->query(lpName)));
}

}//namespace

/// 매니저 객체 얻기(싱글턴)
void WINAPI GetManager(kamelaslib::KamelasManager** pManager)
{	
	static kamelaslib::KamelasManager manager;
	*pManager = &manager;
}