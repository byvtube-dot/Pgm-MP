#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CFoamBox
{
public:
	enum Cmd
	{
		C_FOAM_CST_LD_START			= 100,
		C_FOAM_CST_LD_REQ_ON		,
		C_FOAM_CST_LD_SAFETY_MOVE	,
		C_FOAM_CST_LD_RDY_ON		,
		C_FOAM_CST_LD_REQ_OFF		,
		C_FOAM_CST_LD_RDY_OFF		,
		C_FOAM_CST_LD_COMP_OFF		,
		C_FOAM_CST_LD_RFID_READ		,
		C_FOAM_CST_LD_END			= 199,

		C_FOAM_CST_ULD_START		= 200,
		C_FOAM_CST_ULD_REQ_ON		,
		C_FOAM_CST_ULD_SAFETY_MOVE	,
		C_FOAM_CST_ULD_RDY_ON		,
		C_FOAM_CST_ULD_REQ_OFF		,
		C_FOAM_CST_ULD_RDY_OFF		,
		C_FOAM_CST_ULD_COMP_OFF		,
		C_FOAM_CST_ULD_END			= 299,
	};

private:
	CTimer				m_tmCycleTime;
	static void StateUpdate(string oldValue, string newValue, LPVOID lpParam);

public:
	int					m_no;
	CFSM*				m_pFsm;
	CFSM*				m_pRefFsm;
	SLog*				m_pSLog;
	char				m_strName[50];
	Property<string>	m_state;
	int					m_isRun;

	enDI iFoamBoxFoamExist;
	enDI iFoamBoxDetectLtFt;
	enDI iFoamBoxDetectRtFt;
	enDI iFoamBoxDetectLtRr;
	enDI iFoamBoxDetectRtRr;

	// PIO
	enDI iPioValId;
	enDI iPioCs0;
	enDI iPioCs1;
	enDI iPioCs2;
	enDI iPioCs3;
	enDI iPioTrReq;
	enDI iPioBusy;
	enDI iPioComp;

	enDO oPioLdReq;
	enDO oPioUldReq;
	enDO oPioAbort;
	enDO oPioRdy;

	CFoamBox();
	virtual ~CFoamBox();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleFoamBoxLoad(void);
	BOOL IsCanCycleFoamBoxUnload(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	void CycleFoamBoxLoad(void);
	void CycleFoamBoxUnload(void);
};

extern CFoamBox FOAM_CST[];