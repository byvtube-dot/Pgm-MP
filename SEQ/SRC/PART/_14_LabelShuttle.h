#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CLabelShuttle
{
public:
	enum Cmd
	{
		C_LABEL_RECV_START				= 100,
		C_LABEL_RECV_MOVE				,
		C_LABEL_RECV_FEED_STOP			,
		C_LABEL_RECV_FEED_MOVE			,
		C_LABEL_RECV_SENSOR_CHECK		,
		C_LABEL_RECV_UP					,
		C_LABEL_RECV_VAC				,
		C_LABEL_RECV_QC_MOVE			,
		C_LABEL_RECV_END				= 199,

		C_LABEL_QC_VI_START				= 200,
		C_LABEL_QC_VI_DATA_SEND			,
		C_LABEL_QC_VI_MOVE				,
		C_LABEL_QC_VI_RST				,
		C_LABEL_QC_VI_TRIG				,
		C_LABEL_QC_VI_RESULT			,
		C_LABEL_QC_VI_SEND_MOVE			,
		C_LABEL_QC_VI_END				= 299,
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

	int					m_FeedStep;
	ViMode				viMode;

	NvLabelPartList		nvLabelShuttle;
	NvLabelPartList		nvRefLabelShuttle;
	AxisList			mtLabelShuttleY;

	HW::Vacuum*			m_pVacLabelRecv;

	HW::Cylinder*		m_pLabelFeederFb;
	HW::Cylinder*		m_pLabelFeederMountFb;
	HW::Cylinder*		m_pLabelFeederLockFtFb;
	HW::Cylinder*		m_pLabelFeederLockRrFb;
	HW::Cylinder*		m_pLabelRecvUd;
	HW::Cylinder*		m_pLabelRecvFb;
	HW::Cylinder*		m_pLabelRecvEjectPinLtUd;
	HW::Cylinder*		m_pLabelRecvEjectPinRtUd;

	enDI				iLabelRecvExist;
	enDI				iLabelCstLabelExist;
	enDI				iLabelCstDetectFt;
	enDI				iLabelCstDetectRr;

	CLabelShuttle();
	virtual ~CLabelShuttle();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleLabelRecv(void);
	BOOL IsCanCycleLabelQc(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	void CycleLabelRecv(void);
	void CycleLabelQc(void);
};

extern CLabelShuttle LABEL_SHUTTLE[];