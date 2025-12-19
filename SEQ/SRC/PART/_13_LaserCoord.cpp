#include "..\..\Includes.h"

POINT2D CLabelLaser::GetLasFovCenCoord(int index)
{
	POINT2D centerpos = { 0,0 };

	centerpos.dX = MT[mtLabelLaserX].PosTable(P_LASER_X_LT_LASER + index);
	centerpos.dX += GetUsrGlobalOffset().dX;
	centerpos.dY = GetUsrGlobalOffset().dY;

	return centerpos;
}

BOOL CLabelLaser::CreateScannerCoord(int Unit, int Step)
{
	if (Unit > STAGE_DATA[m_no].Dwg.nUnitCnt)
	{
		LOG[logLAS].Message("<%s>,[CreateScannerCoord],Unit Over[%2d]", m_strName, Unit);
		return FALSE;
	}

	//if (Step > STAGE_DATA[m_no].Dwg.Unit[Unit].nStepCnt)
	//{
	//	LOG[logLAS].Message("<%s>,[CreateScannerCoord],Unit[%2d] Setp Over[%2d]", m_strName, Unit, Step);
	//	return FALSE;
	//}

	LAS[LASER].m_scanData.iStageNo = m_no;
	LAS[LASER].m_scanData.scanCoord.idxX[0] = Unit;
	LAS[LASER].m_scanData.scanCoord.idxY[0] = 0;
	LAS[LASER].m_scanData.scanCoord.Step[0] = Step;
	LAS[LASER].m_scanData.scanCoord.unitCnt = 1;

	LAS[LASER].m_scanData.scanCoord.x[0] = 0.0;
//	LAS[LASER].m_scanData.scanCoord.x[0] += GetUsrGlobalOffset().dX;

	LAS[LASER].m_scanData.scanCoord.y[0] = 0.0;
//	LAS[LASER].m_scanData.scanCoord.y[0] += GetUsrGlobalOffset().dY;
	
	LAS[LASER].m_scanData.scanCoord.t[0] = STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dT; 
	LAS[LASER].m_scanData.scanCoord.t[0] += GetUsrGlobalOffset().dT;

	return TRUE;
}

XYT CLabelLaser::GetUsrGlobalOffset(void)
{
	RcpList RcpNo = (RcpList)(usrLtLabelOffsetX + (m_no * 10));

	XYT* pOffset = (XYT*)&NV.rcp[RcpNo];
	return (*pOffset);
}


