#ifndef sharedmemoryH
#define sharedmemoryH


#include "kamelasbase.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
namespace kamelaslib
{
	/** Win32 Memory Mapped File 
	* 
	*  
	*	2009.6
	*	Hong Chan, Choi
	*	hcchoi@koses.co.kr
	* 
	*  Win32 에서 지원하는 IPC기법들중 공유메모리(MMF)를 사용한 데이타 공유기법을 사용하며,
	*  1:1 통신기능을 지원하기 위해 송신및 수신용 채널을 분리하였다.
	*  즉, 메모리 영역을 두개의 큰 영역으로 나누어, 
	*  송신시에는 상위메모리 영역을 사용하고,
	*  수신시에는 하위메모리 영역을 사용하게 된다.
	*  각 채널간 타임아웃 지원을 위해 송수신 이벤트를 분리하여 적용함.
	* 
	*/

class SharedMemoryPimpl;
class SharedMemory
	: public KamelasBase
{
public:
	virtual BOOL	__stdcall	open();
	virtual	BOOL	__stdcall	close();
	virtual	BOOL	__stdcall	isOpen();  
	virtual	LPCTSTR __stdcall	name() const;
	virtual	UINT32	__stdcall	size() const;
	virtual	BOOL	__stdcall	recv(PVOID pBuffer, UINT32 milliSeconds);
	virtual	BOOL	__stdcall	send(PVOID pBuffer, UINT32 milliSeconds);

private:
	SharedMemoryPimpl*	_pPimpl;

public:
	explicit SharedMemory(LPCTSTR lpName, UINT32 size, BOOL bServer);
	virtual ~SharedMemory();
};
	
}//namespace

#ifdef __cplusplus
}
#endif  //__cplusplus


#endif
