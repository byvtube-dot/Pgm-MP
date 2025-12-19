#ifndef _SEALINGLIB_H_
#define _SEALINGLIB_H_

#include <Windows.h>
#include "..\..\BASE\BaseAll.h"

// Constants
#define	SIZE_OUTPUT  20
#define	SIZE_INPUT	 64


namespace SEALDATA
{
#pragma pack(push, 1)
	typedef struct _WriteRegister
	{
		UINT16 controlFlags;			// 제어 플래그 (명령 실행을 위한 비트)
		UINT16 commandCode;				// 실행할 명령 코드 (예: Start, Abort 등)
		UINT16 commandParam;			// 명령에 필요한 추가 파라미터
		UINT16 firstParamValue;			// 첫 번째 명령 파라미터 값
		UINT16 secondParamValue;		// 두 번째 명령 파라미터 값
		UINT16 forceSensor;				// 외부 힘 센서 값
		UINT16 distanceSensor;			// 외부 거리 센서 값
		UINT16 reserved[3];				// 예약된 공간 (현재 사용되지 않음) -> 메뉴얼에는 5 X 2 오류일거같음.
	}WriteRegister;

	typedef struct _ReadRegister
	{
		// Status
		UINT8 protocolVersion;			// 프로토콜 버전 (WCU와의 통신 규격)
		UINT8 datasetNumber;			// 현재 데이터셋 번호
		UINT8 machineStatus;			// 기계 상태 (작동 여부 확인)
		UINT8 machineMode;				// 기계 모드 (설정된 모드 정보)
		UINT16 machineError;			// 오류 코드 (0이면 오류 없음)
		UINT16 commandStatus;			// 마지막 실행된 명령 코드
		UINT16 ioStatus;				// I/O 상태 (입출력 상태 확인)
		// Results
		UINT8 processResult;			// 초음파 프로세스 결과 (성공 또는 실패)
		UINT8 resultDatasetNumber;		// 결과 데이터셋 번호
		UINT32 processCounter;			// 프로세스 실행 횟수
		UINT8 limitStatus[4];			// 설정된 한계치 검사 결과	
		UINT16 resultValues[18];		// 결과 값 (프로세스 실행 후 저장된 값들)
		// Data
		UINT16 cammandParameterId;		// 
		UINT16 cammandParameterValue1;  // 
		UINT16 cammandParameterValue2;  // 
		// Spare
		UINT8 rserved;					// 
		UINT8 communicationCounter;		// 
	}ReadRegister;
#pragma pack(pop)
}

class CSealingLib
{
public:
	CSealingLib() : m_bConnected(false)
	{
	}
	virtual ~CSealingLib() {}

	// Constants
	const UINT16 m_bitMask[16] =
	{
		0x0001, 0x0002, 0x0004, 0x0008,
		0x0010, 0x0020, 0x0040, 0x0080,
		0x0100, 0x0200, 0x0400, 0x0800,
		0x1000, 0x2000, 0x4000, 0x8000,
	};

	// Variables
	SEALDATA::ReadRegister	m_ReadData[1];
	SEALDATA::WriteRegister	m_WriteData[1];

	bool m_bConnected;

	int	m_nInputAddress;
	int m_nOutputAddress;

	int m_nSubCycleNum;

	void Init();
	void SetAddress(int nInput, int nOutput)
	{
		m_nInputAddress = nInput;
		m_nOutputAddress = nOutput;
	}
	void ReadInput(void);
	void WriteOutput(void);
	void Update(void);
	void GetState(void);
	void GetErr(void);

};

EXTERN CSealingLib SEALLIB;

#endif