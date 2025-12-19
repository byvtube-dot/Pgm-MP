#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CInPk
{
public:
	enum Cmd
	{
		C_BTM_FOAM_VI_START				= 100,
		C_BTM_FOAM_VI_MOVE				,
		C_BTM_FOAM_VI_DN				,
		C_BTM_FOAM_VI_RST				,
		C_BTM_FOAM_VI_TRIG				,
		C_BTM_FOAM_VI_RESULT			,
		C_BTM_FOAM_VI_UP				,
		C_BTM_FOAM_VI_END				= 199,

		C_TRAY_PICK_UP_START			= 200,
		C_TRAY_PICK_UP_DATA_RECV		,
		C_TRAY_PICK_UP_SEMEMA_CHECK		,
		C_TRAY_PICK_UP_MOVE				,
		C_TRAY_PICK_UP_Z_DN				,
		C_TRAY_PICK_UP_Z_UP				,
		C_TRAY_PICK_UP_LOT_DISPLAY		,
		C_TRAY_PICK_UP_LOT_CLOSE_CHECK	,
		C_TRAY_PICK_UP_LABEL_INFO		,
		C_TRAY_PICK_UP_END				= 299,

		C_BANDING_VI_START				= 300,
		C_BANDING_VI_DATA_SEND			,
		C_BANDING_VI_MOVE				,
		C_BANDING_VI_DN					,
		C_BANDING_VI_RST				,
		C_BANDING_VI_TRIG				,
		C_BANDING_VI_RESULT				,
		C_BANDING_VI_UP					,
		C_BANDING_VI_END				= 399,

		C_TRAY_PLACE_START				= 400,
		C_TRAY_PLACE_MOVE				,
		C_TRAY_PLACE_Z_DN				,
		C_TRAY_PLACE_Z_UP				,
		C_TRAY_PLACE_END				= 499,

		C_BTM_FOAM_TRAY_VI_START		= 500,
		C_BTM_FOAM_TRAY_VI_DATA_SEND	,
		C_BTM_FOAM_TRAY_VI_MOVE			,
		C_BTM_FOAM_TRAY_VI_DN			,
		C_BTM_FOAM_TRAY_VI_RST			,
		C_BTM_FOAM_TRAY_VI_TRIG			,
		C_BTM_FOAM_TRAY_VI_RESULT		,
		C_BTM_FOAM_TRAY_VI_UP			,
		C_BTM_FOAM_TRAY_VI_END			= 599,

		C_LABEL_ATTACH_VI_START			= 600,
		C_LABEL_ATTACH_VI_DATA_SEND		,
		C_LABEL_ATTACH_VI_MOVE			,
		C_LABEL_ATTACH_VI_DN			,
		C_LABEL_ATTACH_VI_RST			,
		C_LABEL_ATTACH_VI_TRIG			,
		C_LABEL_ATTACH_VI_RESULT		,
		C_LABEL_ATTACH_VI_UP			,
		C_LABEL_ATTACH_VI_END			= 699,
	};

private:
	CTimer				m_tmCycleTime;
	static void StateUpdate(string oldValue, string newValue, LPVOID lpParam);

public:
	CInPk();
	virtual ~CInPk();

public:
	int					m_no;
	CFSM*				m_pFsm;
	SLog*				m_pSLog;
	char				m_strName[50];
	Property<string>	m_state;
	int					m_isRun;

	ViMode				viMode;
	int					m_nViCnt;
	int					m_nViCntTemp;
	int					m_nViRetryCnt;
	CTimer				m_tmDryRunInterval;

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	BOOL CylInit(void);

	// ETC
	double GetCurBtmFoamViZOffset(void);
	double GetCurTopFoamViZOffset(void);
	int GetCurFoamType(void);
	// Can Cycle Check
	BOOL IsCanCycleBtmFoamVi(void);
	BOOL IsCanCyclePickUp(void);
	BOOL IsCanCycleBandingVi(void);
	BOOL IsCanCycleBtmFoamTrayVi(void);
	BOOL IsCanCycleTopFoamVi(void);
	BOOL IsCanCyclePlace(void);

	// Exist
	BOOL ExistCrossCheck(void);
	int& Exist(void);

	BOOL IsExist(void);
	BOOL IsEmpty(void);

	// Cycle
	void CycleBtmFoamVi(void);
	void CyclePickUp(void);
	void CycleBandingVi(void);
	void CyclePlace(void);
	void CycleBtmFoamTrayVi(void);
	void CycleTopFoamVi(void);
};

extern CInPk IN_PK;
