#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CFoamStackConv
{
public:
	enum CmdMsg
	{
		msgMzMoveLift1To2 = 0,
		msgMzMoveLift1To3,
		msgMzMoveLift1To4,
		msgMzMoveLift2To3,
		msgMzMoveLift2To4,
		msgMzMoveLift3To4,
	};

	enum Cmd
	{
		C_FOAM_MOVE_START = 100,
		C_FOAM_MOVE_BELT_RUN,
		C_FOAM_MOVE_BELT_STOP_DELAY,
		C_FOAM_MOVE_STOPPER_FWD,
		C_FOAM_MOVE_END = 199,

	};

private:
	CTimer				m_tmCycleTime;
	static void StateUpdate(string oldValue, string newValue, LPVOID lpParam);

public:
	int					m_no;
	CFSM*				m_pFsm;
	SLog*				m_pSLog;
	char				m_strName[50];
	string				m_strStep;
	Property<string>	m_state;
	int					m_isRun;

	int					m_nvFoamConv;

	CFoamStackConv();
	virtual ~CFoamStackConv();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(int nRoute = -1);
	void SetFoamConvAlarm(int nRoute);
	BOOL SetFoamConvMem(int nRoute);
	int GetMsgIdx(void);

	// Can Cycle Check
	BOOL IsCanCycleFoamMove(void);

	BOOL ExistCrossCheck(void);	
	int& Exist1(void);				// nvFoamRecvElev
	int& Exist2(void);				// nvFoamRrStack
	int& Exist3(void);				// nvFoamFtStack
	int& Exist4(void);				// nvFoamSendStack
	BOOL IsExist1(void);
	BOOL IsEmpty1(void);
	BOOL IsExist2(void);
	BOOL IsEmpty2(void);
	BOOL IsExist3(void);
	BOOL IsEmpty3(void);
	BOOL IsExist4(void);
	BOOL IsEmpty4(void);

	void CycleFoamMove(void);
};

extern CFoamStackConv FOAM_STACK_CONV;