#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CTrayShuttle
{
public:
	enum Cmd
	{
		C_FOAM_PACK_SEND_START			= 100,
		C_FOAM_PACK_SEND_MOVE			,
		C_FOAM_PACK_SEND_2D_READ		,
		C_FOAM_PACK_SEND_PUSH_READY		,
		C_FOAM_PACK_SEND_1ST_TRQ_MOVE	,
		C_FOAM_PACK_SEND_1ST_POS_CHECK	,
		C_FOAM_PACK_SEND_2ND_TRQ_MOVE	,
		C_FOAM_PACK_SEND_2ND_POS_CHECK	,
		C_FOAM_PACK_SEND_INIT			,
		C_FOAM_PACK_SEND_END			= 199,
	};

private:
	CTimer				m_tmCycleTime;
	static void StateUpdate(string oldValue, string newValue, LPVOID lpParam);

public:
	int					m_no;
	CFSM*				m_pFsm;
	SLog*				m_pSLog;
	char				m_strName[50];
	Property<string>	m_state;
	int					m_isRun;

	int					m_n2DReadCnt;		// 2D Read Retry

	CTrayShuttle();
	virtual ~CTrayShuttle();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleFoamPackSend(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	bool& DidTrayShuttle2D(void);

	void CycleFoamPackSend(void);
};

extern CTrayShuttle TRAY_SHUTTLE;