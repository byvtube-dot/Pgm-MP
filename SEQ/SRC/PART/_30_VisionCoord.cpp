#include "..\..\Includes.h"

//BOOL Stage::CreateCoord(int Unit)
//{
//	if (Unit > STAGE_DATA[m_no].Dwg.nUnitCnt)
//	{
//		LOG[logLAS].Message("<%s>,[CreateCoord],Unit Over[%2d]", m_strName, Unit);
//		return FALSE;
//	}
//	/*
//	if (STAGE_DATA[m_no].Dwg.Unit[Unit].nFiducialCnt == 4)
//	{
//		DATA_4POS FiducialPos;
//		POINT2D StepPos;
//		POINT2D Offset;
//		XYT		OffsetAngle;
//
//		FiducialPos.pos[0].coord.dX = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[0].dX;
//		FiducialPos.pos[0].offset.dX = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[0].dX;
//		FiducialPos.pos[0].coord.dY = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[0].dY;
//		FiducialPos.pos[0].offset.dY = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[0].dY;
//
//		FiducialPos.pos[1].coord.dX = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[1].dX;
//		FiducialPos.pos[1].offset.dX = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[1].dX;
//		FiducialPos.pos[1].coord.dY = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[1].dY;
//		FiducialPos.pos[1].offset.dY = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[1].dY;
//
//		FiducialPos.pos[2].coord.dX = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[2].dX;
//		FiducialPos.pos[2].offset.dX = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[2].dX;
//		FiducialPos.pos[2].coord.dY = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[2].dY;
//		FiducialPos.pos[2].offset.dY = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[2].dY;
//
//		FiducialPos.pos[3].coord.dX = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[3].dX;
//		FiducialPos.pos[3].offset.dX = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[3].dX;
//		FiducialPos.pos[3].coord.dY = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[3].dY;
//		FiducialPos.pos[3].offset.dY = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[3].dY;
//
//		for (int Step = 0; Step < STAGE_DATA[m_no].Dwg.Unit[Unit].nStepCnt; Step++)
//		{
//			StepPos = STAGE_DATA[m_no].Dwg.Unit[Unit].StepPos[Step];
//			if (Align4Point(FiducialPos, StepPos, Offset))
//			{
//				STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dX = StepPos.dX + Offset.dX;
//				STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dY = StepPos.dY + Offset.dY;
//				//STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dT = Offset.dT;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> [%2d]Unit [%2d]Step Make Data Fail", m_strName, Unit, Step);
//				return FALSE;
//				//ERROR
//			}
//		}
//		for (int PreQc = 0; PreQc < STAGE_DATA[m_no].Dwg.Unit[Unit].nPreQcCnt; PreQc++)
//		{
//			StepPos = STAGE_DATA[m_no].Dwg.Unit[Unit].PreQcPos[PreQc];
//			if (Align4Point(FiducialPos, StepPos, Offset))
//			{
//				STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[PreQc].dX = StepPos.dX + Offset.dX;
//				STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[PreQc].dY = StepPos.dY + Offset.dY;
//				//STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[PreQc].dT = Offset.dT;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> [%2d]Unit [%2d]PreQc Make Data Fail", m_strName, Unit, PreQc);
//				return FALSE;
//				//ERROR
//			}
//		}
//	}
//	else if (STAGE_DATA[m_no].Dwg.Unit[Unit].nFiducialCnt == 2)
//
//	*/
//	{
//		ALIGN_POINT_DATA FiducialPos;
//		POINT2D StepPos;
//		XYT Offset;
//
//		FiducialPos.coord[ALIGN_POINT1].dX = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[0].dX;
//		FiducialPos.offset[ALIGN_POINT1].dX = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[0].dX;
//		FiducialPos.coord[ALIGN_POINT1].dY = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[0].dY;
//		FiducialPos.offset[ALIGN_POINT1].dY = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[0].dY;
//
//		FiducialPos.coord[ALIGN_POINT2].dX = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[1].dX;
//		FiducialPos.offset[ALIGN_POINT2].dX = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[1].dX;
//		FiducialPos.coord[ALIGN_POINT2].dY = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[1].dY;
//		FiducialPos.offset[ALIGN_POINT2].dY = STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[1].dY;
//
//		for (int Step = 0; Step < STAGE_DATA[m_no].Dwg.Unit[Unit].nStepCnt; Step++)
//		{
//			StepPos = STAGE_DATA[m_no].Dwg.Unit[Unit].StepPos[Step];
//			if (Align2Point(FiducialPos, StepPos, Offset))
//			{
//				STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dX = StepPos.dX + Offset.dX;
//				STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dY = StepPos.dY + Offset.dY;
//				STAGE_DATA[m_no].PrsData.Unit[Unit].StepPos[Step].dT = Offset.dT;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> [%2d]Unit [%2d]Step Make Data Fail", m_strName, Unit, Step);
//				return FALSE;
//				//ERROR
//			}
//		}
//		for (int PreQc = 0; PreQc < STAGE_DATA[m_no].Dwg.Unit[Unit].nPreQcCnt; PreQc++)
//		{
//			StepPos = STAGE_DATA[m_no].Dwg.Unit[Unit].PreQcPos[PreQc];
//			if (Align2Point(FiducialPos, StepPos, Offset))
//			{
//				STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[PreQc].dX = StepPos.dX + Offset.dX;
//				STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[PreQc].dY = StepPos.dY + Offset.dY;
//				STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[PreQc].dT = Offset.dT;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> [%2d]Unit [%2d]PreQc Make Data Fail", m_strName, Unit, PreQc);
//				return FALSE;
//				//ERROR
//			}
//		}
//	}
//	return TRUE;
//}
//
//POINT2D Stage::GetUnitQcCoord(int Unit, int QcCnt)
//{
//	POINT2D coord = { 0,0 };
//
//	if (Unit > STAGE_DATA[m_no].Dwg.nUnitCnt)
//	{
//		LOG[logLAS].Message("<%s>,[GetUnitQcCoord],Unit Over[%2d]", m_strName, Unit);
//		return (coord);
//	}
//	if (QcCnt > STAGE_DATA[m_no].Dwg.Unit[Unit].nPreQcCnt)
//	{
//		LOG[logLAS].Message("<%s>,[GetUnitQcCoord],[%2d] Unit - PreQcCnt Over[%2d]", m_strName, Unit, QcCnt);
//		return (coord);
//	}
//
//	coord.dX = STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[QcCnt].dX;
//	coord.dY = STAGE_DATA[m_no].PrsData.Unit[Unit].PreQcPos[QcCnt].dY;
//
//	return (coord);
//}
//
//POINT2D Stage::GetUnitFiducialCoord(int Unit, int Fiducial)
//{
//	POINT2D coord = { 0,0 };
//
//	if (Unit > STAGE_DATA[m_no].Dwg.nUnitCnt)
//	{
//		LOG[logLAS].Message("<%s>,[GetFiducialCoord],Unit Over[%2d]", m_strName, Unit);
//		return (coord);
//	}
//	if (Fiducial > STAGE_DATA[m_no].Dwg.Unit[Unit].nFiducialCnt)
//	{
//		LOG[logLAS].Message("<%s>,[GetFiducialCoord],[%2d] Unit - Fiducial Over[%2d]", m_strName, Unit, Fiducial);
//		return (coord);
//	}
//
//	coord = STAGE_DATA[m_no].Dwg.Unit[Unit].Fiducial[Fiducial];
//
//	return (coord);
//}
//
//POINT2D Stage::GetFilmBarPos(void)
//{
//	POINT2D DwgPos = STAGE_DATA[m_no].Dwg.bcdFilm;
//
//	return DwgPos;
//}
//
//POINT2D Stage::GetUnitBarPos(int Unit)
//{
//	POINT2D DwgPos = STAGE_DATA[m_no].Dwg.Unit[Unit].bcdUnit;
//
//	return DwgPos;
//}
//
//BOOL Stage::WriteVisResult(int Unit, int Fiducial, IPC_MMI::PrsViData Data)
//{
//	if (Unit > STAGE_DATA[m_no].Dwg.nUnitCnt)
//	{
//		LOG[logLAS].Message("<%s>,[WriteVisResult],Unit Over[%2d]", m_strName, Unit);
//		return FALSE;
//	}
//	if (Fiducial > STAGE_DATA[m_no].Dwg.Unit[Unit].nFiducialCnt)
//	{
//		LOG[logLAS].Message("<%s>,[WriteVisResult],[%2d] Unit - Fiducial Over[%2d]", m_strName, Unit, Fiducial);
//		return FALSE;
//	}
//
//	if (!Data.result[0])
//	{
//		return FALSE;
//	}
//	else
//	{
//		if ((Data.data[0].dX > 5.00) || (Data.data[0].dY > 5.00))
//		{
//			return FALSE;
//		}
//		else
//		{
//			STAGE_DATA[m_no].PrsData.Unit[Unit].FiducialOffset[Fiducial] = Data.data[0];
//			return TRUE;
//		}
//	}
//	return FALSE;
//}
//
//BOOL Stage::WriteVisQcResult(int Unit, IPC_MMI::PrsViData Data)
//{
//	if (Unit > STAGE_DATA[m_no].Dwg.nUnitCnt)
//	{
//		LOG[logLAS].Message("<%s>,[WriteVisQcResult],Unit Over[%2d]", m_strName, Unit);
//		return FALSE;
//	}
//	else
//	{
//		int  FilmNum;
//		if (m_no == 0) FilmNum = IPC_MMI::FILM_INDEX1;
//		else if (m_no == 1) FilmNum = IPC_MMI::FILM_INDEX2;
//		TRACEABILITY.pcbInfo[FilmNum].UnitInfo[Unit].PreQc = Data.result[0];
//	}
//	return TRUE;
//}