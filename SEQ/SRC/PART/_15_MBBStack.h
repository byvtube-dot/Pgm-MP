#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CMbbStack
{
public:
	enum Cmd
	{
		
		C_LEVEL_CHECK_START			= 100,
		C_LEVEL_CHECK_Z_DN			,
		C_LEVEL_CHECK_Z_DN_CHECK	,
		C_LEVEL_CHECK_Z_UP			,
		C_LEVEL_CHECK_Z_UP_CHECK	,
		C_LEVEL_CHECK_END			= 199,

		C_MBB_CST_LD_START			= 200,
		C_MBB_CST_LD_REQ_ON			,
		C_MBB_CST_LD_SAFETY_MOVE	,
		C_MBB_CST_LD_RDY_ON			,
		C_MBB_CST_LD_REQ_OFF		,
		C_MBB_CST_LD_RDY_OFF		,
		C_MBB_CST_LD_COMP_OFF		,
		C_MBB_CST_LD_RFID_READ		,
		C_MBB_CST_LD_END			= 299,

		C_MBB_CST_ULD_START			= 300,
		C_MBB_CST_ULD_REQ_ON		,
		C_MBB_CST_ULD_SAFETY_MOVE	,
		C_MBB_CST_ULD_RDY_ON		,
		C_MBB_CST_ULD_REQ_OFF		,
		C_MBB_CST_ULD_RDY_OFF		,
		C_MBB_CST_ULD_COMP_OFF		,
		C_MBB_CST_ULD_END			= 399,

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

	NvMbbPartList		nvMbbStack;

	AxisList			MtZNum;

	HW::Cylinder*		m_pMbbCstFb;

	enDI				iMbbCstExist;
	enDI				iMbbCstDetectLt;
	enDI				iMbbCstDetectRt;
	enDI				iMbbCstFtLevel;		// Limit
	enDI				iMbbCstMidLevel;
	enDI				iMbbCstRrLevel;

	// PIO
	enDI				iPioValId;
	enDI				iPioCs0;
	enDI				iPioCs1;
	enDI				iPioCs2;
	enDI				iPioCs3;
	enDI				iPioTrReq;
	enDI				iPioBusy;
	enDI				iPioComp;

	enDO				oPioLdReq;
	enDO				oPioUldReq;
	enDO				oPioAbort;
	enDO				oPioRdy;

	int					m_nLevelCheckCnt;

	CMbbStack();
	virtual ~CMbbStack();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleLevelCheck(void);
	BOOL IsCanCycleMbbCstLoad(void);
	BOOL IsCanCycleMbbCstUnload(void);

	BOOL ExistCrossCheck(void);
	bool& ExistCst(void);
	int& Exist(void);
	BOOL IsExistCst(void);
	BOOL IsEmptyCst(void);

	void CycleMbbCstLoad(void);
	void CycleMbbCstUnload(void);
	void CycleLevelCheck(void);
};

extern CMbbStack MBB_STACK[];