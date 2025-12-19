#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CFoldPk
{
public:
	enum Cmd
	{
		C_PICK_UP_START							= 100,
		C_PICK_UP_RIGHT_ATTACH_READY_MOVE		,
		C_PICK_UP_ALIGN_Z_MOVE					,
		C_PICK_UP_RIGHT_ATTACH_MOVE				,
		C_PICK_UP_RT_CLAMP						,
		C_PICK_UP_MOVE							,
		C_PICK_UP_Z_DN							,
		C_PICK_UP_Z_SLOW_DN						,
		C_PICK_UP_FOLD							,
		C_PICK_UP_FOLD_CLAMP					,
		C_PICK_UP_FOLD_INIT						,	// MbbFoldPkClamp를 제외한 나머지 Open
		C_PICK_UP_Z_SLOW_UP						,
		C_PICK_UP_Z_UP							,
		C_PICK_UP_END							= 199,

		C_PLACE_START							= 200,
		C_PLACE_MOVE							,
		C_PLACE_DATA_SEND						,
		C_PLACE_Z_DN							,
		C_PLACE_Z_SLOW_DN						,
		C_PLACE_Z_SLOW_UP						,
		C_PLACE_Z_UP							,
		C_PLACE_END								= 299,
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

	int					m_nDataSendCnt;

	CFoldPk();
	virtual ~CFoldPk();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCyclePickUp(void);
	BOOL IsCanCyclePlace(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	void CyclePickUp(void);
	void CyclePlace(void);
};

extern CFoldPk FOLD_PK;