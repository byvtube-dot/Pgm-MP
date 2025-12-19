#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CRtCobot
{
public:
	enum Cmd
	{
		C_LABEL_PICK_START				= 100,
		C_LABEL_PICK_MOVE				,
		C_LABEL_PICK_VAC				,
		C_LABEL_PICK_UP					,
		C_LABEL_PICK_END				= 199,

		C_MBB_VI_START					= 200,
		C_MBB_VI_MOVE					,
		C_MBB_VI_RST					,
		C_MBB_VI_TRIG					,
		C_MBB_VI_RESULT					,
		C_MBB_VI_UP						,
		C_MBB_VI_END					= 299,

		C_LABEL_ATTACH_START			= 300,
		C_LABEL_ATTACH_MOVE				,
		C_LABEL_ATTACH_Z_DN				,
		C_LABEL_ATTACH_VAC				,
		C_LABEL_ATTACH_UP				,
		C_LABEL_ATTACH_END				= 399,

		C_LABEL_ATTACH_VI_START			= 400,
		C_LABEL_ATTACH_VI_DATA_SEND		,
		C_LABEL_ATTACH_VI_MOVE			,
		C_LABEL_ATTACH_VI_RST			,
		C_LABEL_ATTACH_VI_TRIG			,
		C_LABEL_ATTACH_VI_RESULT		,
		C_LABEL_ATTACH_VI_UP			,
		C_LABEL_ATTACH_VI_END			= 499,

		C_MBB_PICK_START				= 500,
		C_MBB_PICK_MOVE					,
		C_MBB_PICK_VAC					,
		C_MBB_PICK_UP					,
		C_MBB_PICK_END					= 599,

		C_MBB_PLACE_START				= 600,
		C_MBB_PLACE_MOVE				,
		C_MBB_PLACE_DN					,
		C_MBB_PLACE_UP					,
		C_MBB_PLACE_END					= 699,

		C_LABEL_REJECT_START			= 700,
		C_LABEL_REJECT_MOVE				,
		C_LABEL_REJECT_VAC				,
		C_LABEL_REJECT_UP				,
		C_LABEL_REJECT_END				= 799,

		C_MBB_REJECT_START				= 800,
		C_MBB_REJECT_MOVE				,
		C_MBB_REJECT_VAC				,
		C_MBB_REJECT_UP					,
		C_MBB_REJECT_END				= 899,

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
	int					m_nViRetryCnt;

	CRtCobot();
	virtual ~CRtCobot();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCycleLabelPick(void);
	BOOL IsCanCycleMbbVi(void);
	BOOL IsCanCycleLabelAttach(void);
	BOOL IsCanCycleLabelAttachVi(void);
	BOOL IsCanCycleMbbPick(void);
	BOOL IsCanCycleMbbPlace(void);
	BOOL IsCanCycleLabelReject(void);
	BOOL IsCanCycleMbbReject(void);

	BOOL ExistCrossCheck(void);
	int& Exist(int materialType);
	BOOL IsExist(int materialType);
	BOOL IsEmpty(int materialType);

	BOOL GetLabelPickup(void);
	BOOL SetLabelPickup(void);

	void CycleLabelPick(void);
	void CycleMbbVi(void);
	void CycleLabelAttach(void);
	void CycleLabelAttachVi(void);
	void CycleMbbPick(void);
	void CycleMbbPlace(void);
	void CycleLabelReject(void);
	void CycleMbbReject(void);
};

extern CRtCobot RT_COBOT;