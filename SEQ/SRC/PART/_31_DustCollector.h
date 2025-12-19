#pragma once
#include "..\..\Defines.h"
#include "Include\BaseAll.h"

class DustCollector
{
public:
	int					m_no;
	int					m_isRun;
	CFSM*				m_pFsm;
	char				m_strName[50];

	enDI				iRunNo1;
	enDO				oRunNo1;
	enDI				iRunNo2;
	enDO				oRunNo2;
	CylinderList		cDampIndex1;
	CylinderList		cDampIndex2;
	CylinderList		cDampTonado1;
	CylinderList		cDampTonado2;
	
	DustCollector(){}
	virtual ~DustCollector(){}

	void Auto(void);
	void Cycle(void);
	void Init(void);
	void Common(void);
	void LaserLamp(void);
	BOOL IsHwErr(void);
	BOOL IsError(BOOL isNotice = TRUE);
};

extern DustCollector DUSTCOLLECTOR;