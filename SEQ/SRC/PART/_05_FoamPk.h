#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CFoamPk
{
public:
	enum CmdMsg
	{
		msgLtFoamBox = 0,
		msgRtFoamBox,

		msgFoamRecvElev = 0,
		msgFoamPartialElev,
	};

	enum Cmd
	{
		C_FOAM_PICK_UP_START				= 100,
		C_FOAM_PICK_UP_MOVE					,
		C_FOAM_PICK_UP_VERIFY_MOVE			,
		C_FOAM_PICK_UP_VERIFY_Z_DN			,
		C_FOAM_PICK_UP_Z_DN					,
		C_FOAM_PICK_UP_Z_UP					,
		C_FOAM_PICK_UP_END					= 199,

		C_FOAM_PLACE_START					= 200,
		C_FOAM_PLACE_MOVE					,
		C_FOAM_PLACE_Z_DN					,
		C_FOAM_PLACE_Z_UP					,
		C_FOAM_PLACE_END					= 299,
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

	POINT				m_xyCnt;
	POINT				m_maxCnt;
	XYT					m_coord;
	double				m_ZPickPos;
	int					m_nExistCheckRetryCnt;
	char				strMsg[50];

	CFoamPk();
	virtual ~CFoamPk();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCyclePickUp(BOOL bPartialFoam);
	BOOL IsCanCyclePlace(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	POINT BlockNo(int xUnit, int yUnit);
	POINT2D GetUnitCoord(int xUnit, int yUnit);
	XYT GetWorkPos(int nCmd, XYT coord);
	BOOL CreateWorkCoord(int nCmd, POINT xyCnt, XYT& workCoord);

	void CycleFoamPick(void);
	void CycleFoamPlace(void);
};

extern CFoamPk FOAM_PK;