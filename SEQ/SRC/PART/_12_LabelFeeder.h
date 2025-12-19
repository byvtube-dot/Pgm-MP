#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CLabelFeeder
{
public:
	enum Cmd
	{
		C_LABEL_FEED_START			= 100,
		C_LABEL_FEED_MOVE			,
		C_LABEL_FEED_STOP			,
		C_LABEL_FEED_SENSOR_CHECK	,
		C_LABEL_FEED_END			= 199,

		C_LABEL_CST_LD_START		= 200,
		C_LABEL_CST_LD_REQ_ON		,
		C_LABEL_CST_LD_SAFETY_MOVE	,
		C_LABEL_CST_LD_RDY_ON		,
		C_LABEL_CST_LD_REQ_OFF		,
		C_LABEL_CST_LD_RDY_OFF		,
		C_LABEL_CST_LD_COMP_OFF		,
		C_LABEL_CST_LD_RFID_READ	,
		C_LABEL_CST_LD_END			= 299,

		C_LABEL_CST_ULD_START		= 300,
		C_LABEL_CST_ULD_REQ_ON		,
		C_LABEL_CST_ULD_SAFETY_MOVE	,
		C_LABEL_CST_ULD_RDY_ON		,
		C_LABEL_CST_ULD_REQ_OFF		,
		C_LABEL_CST_ULD_RDY_OFF		,
		C_LABEL_CST_ULD_COMP_OFF		,
		C_LABEL_CST_ULD_END			= 399,

	};

private:
	CTimer				m_tmCycleTime;
	static void StateUpdate(string oldValue, string newValue, LPVOID lpParam);

public:
	int					m_no;
	CFSM*				m_pFsm;
	CFSM*				m_pRefFsm;	// Reference
	SLog*				m_pSLog;
	char				m_strName[50];
	Property<string>	m_state;
	int					m_isRun;

	NvLabelPartList		nvLabelFeeder;
	NvLabelPartList		nvRefLabelFeeder;
	AxisList			mtFeedRewindNum;

	HW::Cylinder*		m_pLabelFeederFb;
	HW::Cylinder*		m_pLabelFeederMountFb;
	HW::Cylinder*		m_pLabelFeederLockFtFb;
	HW::Cylinder*		m_pLabelFeederLockRrFb;

	enDI				iLabelCstLabelExist;
	enDI				iLabelCstDetectFt;
	enDI				iLabelCstDetectRr;
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

	CLabelFeeder();
	virtual ~CLabelFeeder();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleLabelFeed(void);
	BOOL IsCanCycleLabelCstLoad(void);
	BOOL IsCanCycleLabelCstUnload(void);

	BOOL ExistCrossCheck(void);
	bool& ExistCst(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsExistCst(void);
	BOOL IsEmpty(void);
	BOOL IsEmptyCst(void);

	bool FeederTrqZeroStop();

	void CycleLabelFeed(void);
	void CycleLabelCstLoad(void);
	void CycleLabelCstUnload(void);
};

extern CLabelFeeder LABEL_FEEDER[];