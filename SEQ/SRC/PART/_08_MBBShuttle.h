#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CMbbShuttle
{
public:
	enum Cmd
	{
		C_MBB_OPEN_START				= 100,
		C_MBB_OPEN_Y_MOVE				,
		C_MBB_OPEN_SENSOR_OFF_MOVE		,
		C_MBB_OPEN_MBB_ALIGN			,
		C_MBB_OPEN_MBB_ALIGN_VEL_MOVE	,
		C_MBB_OPEN_MBB_ALIGN_CHECK		,
		C_MBB_OPEN_Z_DN					,
		C_MBB_OPEN_TOP_VAC				,
		C_MBB_OPEN_CLAMP				,
		C_MBB_OPEN_Z_UP					,
		C_MBB_OPEN_AIR_BLOW_ON			,
		C_MBB_OPEN_AIR_BLOW_OFF			,
		C_MBB_OPEN_INIT					,
		C_MBB_OPEN_END					= 199,

		C_MBB_SEALING_START				= 200,
		C_MBB_SEALING_Y_MOVE			,
		C_MBB_SEALING_NOZZLE_FWD		,
		C_MBB_SEALING_CLAMP_CLOSE		,
		C_MBB_SEALING_VAC				,
		C_MBB_SEALING_N2_BLOW			,
		C_MBB_SEALING_N2_BLOW_CHECK		,
		C_MBB_SEALING_WORK				,
		C_MBB_SEALING_RESULT			,
		C_MBB_SEALING_CLAMP_OPEN		,
		C_MBB_SEALING_END				= 299,

		C_MBB_PACK_SEND_START			= 300,
		C_MBB_PACK_SEND_MOVE			,
		C_MBB_PACK_SEND_CHECK			,
		C_MBB_PACK_SEND_END				= 399,

		// TEST
		C_MBB_TEST_SEALING_START		= 400,
		C_MBB_TEST_SEALING_CLAMP		,
		C_MBB_TEST_SEALING_WORK			,
		C_MBB_TEST_SEALING_WORK2		,
		C_MBB_TEST_SEALING_UP			,
		C_MBB_TEST_SEALING_END			= 499,

		C_N2_TEST_START					= 500,
		C_N2_TEST_VAC					,
		C_N2_TEST_BLOW					,
		C_N2_TEST_BLOW_CHECK			,
		C_N2_TEST_END					= 599,

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

	double				m_dBackwardPos;
	int					m_nAirBlowCnt;		// AirBlow Retry
	char				m_strLogBlowOn[100];
	char				m_strLogBlowOff[100];

	string				m_prcLogLotId;
	string				m_prcLogMatId;
	string				m_prcRecipeId;

	CMbbShuttle();
	virtual ~CMbbShuttle();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);
	//BOOL CylInitOpen(void);
	//BOOL CylInitSealing(void);

	// Can Cycle Check
	BOOL IsCanCycleMbbOpen(void);
	BOOL IsCanCycleMbbSealing(void);
	BOOL IsCanCycleMbbPackSend(void);

	BOOL ExistCrossCheck(void);
	int& Exist(void);
	BOOL IsExist(void);
	BOOL IsEmpty(void);

	void CycleMbbOpen(void);
	void CycleMbbSealing(void);
	void CycleMbbPackSend(void);
	void CycleMbbSealingTest(void);
	void CycleN2Test(void);
};

extern CMbbShuttle MBB_SHUTTLE;