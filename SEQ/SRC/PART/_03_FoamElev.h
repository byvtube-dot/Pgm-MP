#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CFoamElev
{
public:
	enum CmdMsg
	{
		msgSend = 0,
		msgRecv,
	};

	enum Cmd
	{
		C_LEVEL_CHECK_START			= 100,
		C_LEVEL_CHECK_ALIGN			,
		C_LEVEL_CHECK_Z_DN			,
		C_LEVEL_CHECK_Z_DN_CHECK	,
		C_LEVEL_CHECK_Z_UP			,
		C_LEVEL_CHECK_Z_UP_CHECK	,
		C_LEVEL_CHECK_Z_PITCH_MOVE	,
		C_LEVEL_CHECK_INIT			,
		C_LEVEL_CHECK_CNT_CHECK		,
		C_LEVEL_CHECK_END			= 199,

		C_FOAM_FLIP_START			= 200,
		C_FOAM_FLIP_MOVE			,
		C_FOAM_FLIP_SLOW_READY_MOVE	,
		C_FOAM_FLIP_READY_MOVE		,
		C_FOAM_FLIP_END				= 299,

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

	int					m_nvFoamStack;
	int					m_nFoamStackMaxCnt;
	enDI				iElevBtm;
	enDI				iElevTop;
	RcpList				rcpLevelDnDelay;
	RcpList				rcpLevelUpDelay;
	AxisList			MtZNum;

	int					m_nLevelCheckCnt;
	double				m_dPos;

	CFoamElev();
	virtual ~CFoamElev();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);
	BOOL SetCylAlign(void);

	// Can Cycle Check
	BOOL IsCanCycleLevelCheck(void);
	BOOL IsCanCycleFlip(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);

	void CycleLevelCheck(void);
	void CycleFlip(void);
};

extern CFoamElev FOAM_ELEV[];