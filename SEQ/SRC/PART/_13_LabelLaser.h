#pragma once
#include "..\..\Defines.h"
#include "..\..\..\BASE\BaseAll.h"

class CLabelLaser
{
public:
	enum Cmd
	{
		C_PRS_VI_START						= 100,
		C_PRS_VI_MOVE						,
		C_PRS_VI_RST						,
		C_PRS_VI_TRIG						,
		C_PRS_VI_RESULT						,
		C_PRS_VI_END						= 199,

		C_LASER_START						= 200,
		C_LASER_SEND_DEVICE					,
		C_LASER_MOVE						,
		C_LASER_SEND_COORD					,
		C_LASER_TRIG						,
		C_LASER_BUSY						,
		C_LASER_GEM							,	
		C_LASER_END							= 299,

		C_PWR_METER_START					= 300,
		C_PWR_METER_MOVE					,
		C_PWR_METER_SEND_COORD				,
		C_PWR_METER_TRIG					,
		C_PWR_METER_BUSY					,
		C_PWR_METER_END						= 399,
		
		C_LASER_1POINT_CAL_START			= 500,
		C_LASER_1POINT_CAL_SEND_DEVICE		,
		C_LASER_1POINT_CAL_MOVE				,
		C_LASER_1POINT_CAL_SEND_COORD		,
		C_LASER_1POINT_CAL_TRIG				,
		C_LASER_1POINT_CAL_BUSY				,
		C_LASER_1POINT_CAL_VI_START			,
		C_LASER_1POINT_CAL_VI_MOVE			,
		C_LASER_1POINT_CAL_VI_RST			,
		C_LASER_1POINT_CAL_VI_TRIG			,
		C_LASER_1POINT_CAL_VI_RESULT		,
		C_LASER_1POINT_CAL_RESULT_SEND		,
		C_LASER_1POINT_CAL_END				= 599,



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
	bool				m_PowerMeterDid;

	CLabelLaser();
	virtual ~CLabelLaser();

	void Auto(void);
	void Cycle(void);
	void Init(int nIdx = 0);
	BOOL Common(void);
	BOOL IsHwErr(void);
	BOOL IsHwRdy(void);

	// Cylinder Init
	BOOL CylInit(void);

	// Can Cycle Check
	BOOL IsCanCyclePrs(void);
	BOOL IsCanCycleLaser(void);
	BOOL IsCanCyclePowerMeter(void);

	BOOL SetLabelPrintReq(int nDir);
	void SetLabelPrinted(void);
	BOOL CreateScannerCoord(int Unit, int Step);
	XYT GetUsrGlobalOffset(void);
	POINT2D GetLasFovCenCoord(int index);

	void CyclePrs(void);
	void CycleLaser(void);
	void CyclePowerMeter(void);
	void Cycle1PointCal(void);
};

extern CLabelLaser LABEL_LASER;