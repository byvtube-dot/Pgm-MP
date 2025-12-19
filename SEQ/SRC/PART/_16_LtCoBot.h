#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CLtCobot
{
public:
	enum Cmd
	{
		C_BTM_FOAM_PICK_START			= 100,
		C_BTM_FOAM_PICK_MOVE			,
		C_BTM_FOAM_PICK_VAC				,
		C_BTM_FOAM_PICK_UP				,
		C_BTM_FOAM_PICK_END				= 199,

		C_BTM_FOAM_PLACE_START			= 200,
		C_BTM_FOAM_PLACE_MOVE			,
		C_BTM_FOAM_PLACE_VAC			,
		C_BTM_FOAM_PLACE_UP				,
		C_BTM_FOAM_PLACE_ALIGN			,
		C_BTM_FOAM_PLACE_ALIGN_END		,
		C_BTM_FOAM_PLACE_END			= 299,

		C_LABEL_PICK_START				= 300,
		C_LABEL_PICK_MOVE				,
		C_LABEL_PICK_Z_DN				,
		C_LABEL_PICK_VAC				,
		C_LABEL_PICK_Z_UP				,
		C_LABEL_PICK_READY_MOVE			,
		C_LABEL_PICK_END				= 399,

		C_LABEL_ATTACH_START			= 400,
		C_LABEL_ATTACH_MOVE				,
		C_LABEL_ATTACH_VAC				,
		C_LABEL_ATTACH_UP				,
		C_LABEL_ATTACH_END				= 499,

		C_TOP_FOAM_PICK_START			= 500,
		C_TOP_FOAM_PICK_MOVE			,
		C_TOP_FOAM_PICK_VAC				,
		C_TOP_FOAM_PICK_UP				,
		C_TOP_FOAM_PICK_END				= 599,

		C_TOP_FOAM_PLACE_START			= 600,
		C_TOP_FOAM_PLACE_MOVE			,
		C_TOP_FOAM_PLACE_VAC			,
		C_TOP_FOAM_PLACE_Z_UP			,
		C_TOP_FOAM_PLACE_END			= 699,

		C_FOAM_REJECT_START				= 700,
		C_FOAM_REJECT_MOVE				,
		C_FOAM_REJECT_VAC				,
		C_FOAM_REJECT_UP				,
		C_FOAM_REJECT_END				= 799,
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

	ViMode				viMode;
	int					m_nViCnt;
	int					m_nViRetryCnt;
	int					m_nMoveStepCnt;

	CLtCobot();
	virtual ~CLtCobot();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleBtmFoamPick(BOOL bPartialFoam);
	BOOL IsCanCycleBtmFoamPlace(void);
	BOOL IsCanCycleLabelPick(void);
	BOOL IsCanCycleLabelAttach(void);
	BOOL IsCanCycleTopFoamPick(void);
	BOOL IsCanCycleTopFoamPlace(void);
	BOOL IsCanCycleFoamReject(void);

	BOOL ExistCrossCheck(void);
	int& Exist(int materialType);
	BOOL IsExist(int materialType);
	BOOL IsEmpty(int materialType);

	BOOL GetLabelPickup(void);
	BOOL SetLabelPickup(void);

	void CycleBtmFoamPick(void);
	void CycleBtmFoamPlace(void);
	void CycleLabelPick(void);
	void CycleLabelAttach(void);
	void CycleTopFoamPick(void);
	void CycleTopFoamPlace(void);
	void CycleFoamReject(void);
};

extern CLtCobot LT_COBOT;