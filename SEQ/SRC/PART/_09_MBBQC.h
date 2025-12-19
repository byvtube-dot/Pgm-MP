#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CMbbQc
{
public:
	enum Cmd
	{
		C_MBB_QC_VI_START		= 100,
		C_MBB_QC_VI_CLAMP		,
		C_MBB_QC_VI_DATA_SEND	,
		C_MBB_QC_VI_MOVE		,
		C_MBB_QC_VI_RST			,
		C_MBB_QC_VI_TRIG		,
		C_MBB_QC_VI_RESULT		,
		C_MBB_QC_VI_UNCLAMP		,
		C_MBB_QC_VI_END			= 199,

		C_MBB_QC_SEND_START		= 200,
		C_MBB_QC_SEND_MOVE		,
		C_MBB_QC_SEND_CHECK		,
		C_MBB_QC_SEND_END		= 299,
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
	int					m_nViCntTemp;
	int					m_nViRetryCnt;

	CMbbQc();
	virtual ~CMbbQc();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanQcCycle(void);
	BOOL IsCanSendCycle(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	void CycleQcVi(void);
	void CycleSend(void);
};

extern CMbbQc MBB_QC;