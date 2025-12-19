#ifndef KAMELASCONFIGH
#define KAMELASCONFIGH

#include <windows.h>
#include <tchar.h>


namespace kamelaslib
{

/**
	*  KAMELAS 내부에서 사용되는 프로토콜 헤더및 버전 정보
	*	2009.6
	*	Hong Chan, Choi
	*	hccoi@koses.co.kr
	*
	*
	*  버전관리를 위한 헤더 구조체 정의
	*  버전 변경시 KAMELAS_VERSION 값을 변경하도록 한다.
	*
	*
	*/


/// KAMELAS 내부에서 사용되는 정보
typedef enum eConstant
{
	KAMELAS_VERSION		= 0x0001,	/// 프로토콜 버전
	KAMELAS_HEADER_SIZE	= 0x0040	/// 헤더 크기(64 bytes for cache-line size)
}KAMELAS_CONSTANTS;


/// KAMELAS 헤더 구조체
#include <PshPack8.h>
typedef struct tagHEADER
{
	volatile UINT32	headerLength;			/// 헤더크기 = sizeof(HEADER)
	volatile UINT32	totalLength;			/// 전체크기
	volatile UINT32	version;				/// 버전
	volatile UINT32	lock;					///
	volatile UINT32	reserved[2];			/// 예약됨
	BYTE			padding[KAMELAS_HEADER_SIZE - 6 * sizeof(UINT32)];	/// 예약됨(미래를 위하여)

	tagHEADER()
	{
		::ZeroMemory(this, sizeof(*this));
		this->version		= KAMELAS_VERSION;
		this->headerLength	= KAMELAS_HEADER_SIZE;
	}
}HEADER, *PHEADER;
#include <PopPack.h>


}//namespace

#endif
