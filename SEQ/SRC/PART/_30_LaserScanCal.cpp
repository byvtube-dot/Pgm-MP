#include "..\..\Includes.h"

//void Stage::CrossHairCycle(void)
//{
//	if (!m_pFsm->Between(C_CROSS_HAIR_START, C_CROSS_HAIR_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"CrossHairCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(30000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_CROSSHAIR_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (!MT[MtXNum].IsRdy() || !MT[MtYNum].IsRdy() || !MT[MtZNum].IsRdy())
//		return;
//
//
//	switch (m_pFsm->Get())
//	{
//	case C_CROSS_HAIR_START:
//	{
//		if (!VI[ViNum].IsAuto() && !OPR.isDryRun)
//		{
//			ER.Save(ER_VISION_NOT_AUTO);
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_START", m_strName);
//
//			m_tmCycleTime.Reset();
//			if (CROSS_CAL_FILM == m_pFsm->GetMsg())
//				m_inspMode = MODE_PRS_CROSS_FILM;
//			else
//				m_inspMode = MODE_PRS_STAGECAL;
//
//			VI[ViNum].SetMode(m_inspMode);
//			VI[ViNum].Reset();
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//		}
//
//		if (IsRefIndexBusy())
//		{
//			MSGNOTICE.Set(MSG_NOTICE,
//				"CLOSE", NULL, NULL, NULL,
//				"Another Index Is Cycle Running");
//
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (CROSS_CAL_FILM == m_pFsm->GetMsg())
//		{
//			VC[vcIndex].On();
//			if (!VC[vcIndex].AOn())
//			{
//				if (m_pFsm->Delay(5000))
//				{
//					ER.Save((ErrName)(ER_INDEX_1_VACUUM_STATUS + m_no));
//					m_pFsm->Set(C_ERROR);
//				}
//				return;
//			}
//		}
//		else if (CROSS_CAL_PLATE == m_pFsm->GetMsg())
//		{
//			VC[vcIndex].On();
//			if (!VC[vcIndex].IsOn(1000))
//				return;
//		}
//		else if (CROSS_CAL_SUB == m_pFsm->GetMsg())
//		{
//			if (BOff(isubPlate) ||
//				(NV.ndm[(NDmList)(CROSS_HAIR_CNT_LASER1 + m_no)] >= (NV.sys[CROSS_HAIR_X_CNT] * NV.sys[CROSS_HAIR_Y_CNT])))
//			{
//				ER.Save((ErrName)(ER_INDEX1_CROSSHAIR_PLATE_EXIST + m_no));
//				m_pFsm->Set(C_ERROR);
//				return;
//			}
//		}
//
//		if (!NV.use[USE_DUST] ||
//			!OPR.isNoDevice && NV.use[USE_DUST] &&
//			AOn(DUSTCOLLECTOR.iRunNo1, (DWORD)NV.sys[SET_DUST_WARMUP_TIME]) &&
//			AOn(DUSTCOLLECTOR.iRunNo2, (DWORD)NV.sys[SET_DUST_WARMUP_TIME]))
//			m_pFsm->Set(C_CROSS_HAIR_MOVE_MARKING);
//	}
//	break;
//
//	case C_CROSS_HAIR_MOVE_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_MOVE_MARKING", m_strName);
//		}
//		
//		m_posCalMarking = GetCrossHairPos(m_pFsm->GetMsg());
//		POINT2D posError = GetStageCalErr(m_posCalMarking);
//		POINT2D posAlign = m_posCalMarking + posError;
//
//		// 모드에 따라 서 Z축 위치와 XY위치 변경
//		if (MT[MtYNum].InCmd(PY_CROSS_HAIR, posAlign.dY) &&
//			MT[MtXNum].InCmd(PLX_CROSS_HAIR_1 + (m_no % 2), posAlign.dX) &&
//			MT[MtZNum].InCmd(PLZ_CROSS_HAIR_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_MARKING],m_posCalMarking,%.3f,%.3f", m_strName, m_posCalMarking.dX, m_posCalMarking.dY);
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_MARKING],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_MARKING],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//
//			m_posSaveMarking.dX = posAlign.dX;
//			m_posSaveMarking.dY = posAlign.dY;
//			m_pFsm->Set(C_CROSS_HAIR_SEND_COORD);
//		}
//		else
//		{
//			MtPosMove(MtYNum, PY_CROSS_HAIR, posAlign.dY);
//			MtPosMove(MtXNum, PLX_CROSS_HAIR_1 + (m_no % 2), posAlign.dX);
//			MtPosMove(MtZNum, PLZ_CROSS_HAIR_1 + (m_no % 2));
//		}	
//	}
//	break;
//
//	case C_CROSS_HAIR_SEND_COORD:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_SEND_COORD", m_strName);
//
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//			LAS[LaserNum].m_scanData.scanCoord.unitCnt = 1;
//			LAS[LaserNum].SetMode(MODE_CROSSHAIR);
//			if (!OPR.isDryRun && !LAS[LaserNum].SendCoordData())
//			{
//				ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
//				m_pFsm->Set(C_ERROR);
//				break;
//			}
//
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				LAS[LaserNum].m_isWaitCoordData1 = false;
//				LAS[LaserNum].m_isWaitCoordData2 = false;
//			}
//		}
//
//		if (!LAS[LaserNum].m_isWaitCoordData1 && !LAS[LaserNum].m_isWaitCoordData2)
//		{
//			m_pFsm->Set(C_CROSS_HAIR_MARKING);
//		}
//		else if (LAS[LaserNum].m_tmWaitCoordData.TmOver(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_SEND_SCAN_COORD_TIMEOVER + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//	case C_CROSS_HAIR_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_MARKING", m_strName);
//		}
//
//		if (LAS[LaserNum].m_isError)
//		{
//			OPR.isStop = TRUE;
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[SET_LASER_DELAY_MS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) ||
//			!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) ||
//			!MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			break;
//
//		if (OPR.isDryRun || LAS[LaserNum].Start())
//			m_pFsm->Set(C_CROSS_HAIR_MOVE_VISION);	
//	}
//	break;
//
//	case C_CROSS_HAIR_MOVE_VISION:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_MOVE_VISION", m_strName);
//			VI[ViNum].Reset();
//
//			if (CROSS_CAL_SUB == m_pFsm->GetMsg())
//				NV.ndm[(NDmList)(CROSS_HAIR_CNT_LASER1 + m_no)]++;
//		}
//
//		if (LAS[LaserNum].m_isError)
//		{
//			OPR.isStop = TRUE;
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (LAS[LaserNum].m_isWaitData)
//			break;
//
//		m_posCalInsp = m_posCalMarking - ViToLas();
//		POINT2D posError = GetGlassCalErr(m_posCalInsp);
//		POINT2D posAlign = m_posCalInsp + posError;
//
//		if (MT[MtYNum].InCmd(PY_CROSS_HAIR, posAlign.dY) &&
//			MT[MtXNum].InCmd(PLX_CROSS_HAIR_1 + (m_no % 2), posAlign.dX) &&
//			MT[MtZNum].InCmd(PLZ_VI_CROSS_HAIR_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_VISION],m_posCalMarking,%.3f,%.3f", m_strName, m_posCalInsp.dX, m_posCalInsp.dY);
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_VISION],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_VISION],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//
//
//			m_pFsm->Set(C_CROSS_HAIR_VISION_INSP);
//		}
//		else
//		{
//			MtPosMove(MtYNum, PY_CROSS_HAIR, posAlign.dY);
//			MtPosMove(MtXNum, PLX_CROSS_HAIR_1 + (m_no % 2), posAlign.dX);
//			MtPosMove(MtZNum, PLZ_VI_CROSS_HAIR_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_CROSS_HAIR_VISION_INSP:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_VISION_INSP", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_CROSS_HAIR_VISION_RESULT);
//	}
//	break;
//
//	case C_CROSS_HAIR_VISION_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_VISION_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.result[0] = 1;
//				SetXYT(VI[ViNum].m_prsData.data[0], 0);
//			}
//		}
//
//		if (m_pFsm->Delay(5000)) 
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_RCV_VI_RESULT + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (VI[ViNum].IsBusy() || VI[ViNum].IsStart())
//			break;
//
//		if (!VI[ViNum].m_isRcvComp)
//			break;
//
//		LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_VISION_RESULT],X,%.3f,Y,%.3f",
//			m_strName, VI[ViNum].m_prsData.data[0].dX, VI[ViNum].m_prsData.data[0].dY);
//
//		if (!VI[ViNum].m_prsData.result[0] ||
//			fabs(VI[ViNum].m_prsData.data[0].dX) > fabs(NV.sys[CROSS_HAIR_DATA_DEV]) ||
//			fabs(VI[ViNum].m_prsData.data[0].dY) > fabs(NV.sys[CROSS_HAIR_DATA_DEV])) 
//		{
//			MSGNOTICE.Set(MSG_NOTICE,
//				"CLOSE", NULL, NULL, NULL,
//				"<%s> Cross Hair Calibaration Wrong Data..!", m_strName);
//
//			ER.Save((ErrName)(ER_INDEX1_CROSSHAIR_WRONG_DATA + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//		else
//		{
//			ViToLas().dX -= VI[ViNum].m_prsData.data[0].dX;
//			ViToLas().dY -= VI[ViNum].m_prsData.data[0].dY;
//			
//			if (m_pFsm->GetMsg() == CROSS_CAL_FILM)
//			{
//				m_pFsm->Set(C_CROSS_HAIR_END);		
//				IsCrossHairComp() = TRUE;
//			}
//			else if(m_pFsm->GetMsg() == CROSS_CAL_PLATE)
//			{
//				m_pFsm->Set(C_CROSS_HAIR_MOVE_CIRCLE);
//				IsCrossHairComp() = TRUE;
//			}			
//			else if (m_pFsm->GetMsg() == CROSS_CAL_SUB)
//			{
//				if (fabs(VI[ViNum].m_prsData.data[0].dX) < fabs(NV.sys[CROSS_HAIR_LIMIT]) ||
//					fabs(VI[ViNum].m_prsData.data[0].dY) < fabs(NV.sys[CROSS_HAIR_LIMIT]))
//				{
//					IsCrossHairComp() = TRUE;
//				}
//				m_pFsm->Set(C_CROSS_HAIR_MOVE_CIRCLE);
//			}
//		}
//	}
//	break;
//
//	case C_CROSS_HAIR_MOVE_CIRCLE:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_MOVE_CIRCLE", m_strName);
//		}
//		if (MT[MtYNum].InCmd(PY_CROSS_HAIR, m_posSaveMarking.dY) &&
//			MT[MtXNum].InCmd(PLX_CROSS_HAIR_1 + (m_no % 2), m_posSaveMarking.dX) &&
//			MT[MtZNum].InCmd(PLZ_CROSS_HAIR_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_CROSS_HAIR_MOVE_CIRCLE],m_posSaveMarking,%.3f,%.3f", m_strName, m_posSaveMarking.dX, m_posSaveMarking.dY);
//			m_pFsm->Set(C_CROSS_HAIR_SEND_CIRCLE);
//		}
//		else
//		{
//			MtPosMove(MtYNum, PY_CROSS_HAIR, m_posSaveMarking.dY);
//			MtPosMove(MtXNum, PLX_CROSS_HAIR_1 + (m_no % 2), m_posSaveMarking.dX);
//			MtPosMove(MtZNum, PLZ_CROSS_HAIR_1 + (m_no % 2));
//		}		
//	}
//	break;
//	case C_CROSS_HAIR_SEND_CIRCLE:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_SEND_CIRCLE", m_strName);
//
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//			LAS[LaserNum].m_scanData.scanCoord.unitCnt = 1;
//			LAS[LaserNum].SetMode(MODE_ERASER_MARKING);
//			if (!OPR.isDryRun && !LAS[LaserNum].SendCoordData())
//			{
//				ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
//				m_pFsm->Set(C_ERROR);
//				break;
//			}
//
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				LAS[LaserNum].m_isWaitCoordData1 = false;
//				LAS[LaserNum].m_isWaitCoordData2 = false;
//			}
//		}
//
//		if (!LAS[LaserNum].m_isWaitCoordData1 && !LAS[LaserNum].m_isWaitCoordData2)
//		{
//			m_pFsm->Set(C_CROSS_HAIR_CIRCLE_MARKING);
//		}
//		else if (LAS[LaserNum].m_tmWaitCoordData.TmOver(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_SEND_SCAN_COORD_TIMEOVER + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//
//	case C_CROSS_HAIR_CIRCLE_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_CIRCLE_MARKING", m_strName);
//		}
//
//		if (LAS[LaserNum].m_isError)
//		{
//			OPR.isStop = TRUE;
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[SET_LASER_DELAY_MS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) ||
//			!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) ||
//			!MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			break;
//
//		if (OPR.isDryRun || LAS[LaserNum].Start())
//			m_pFsm->Set(C_CROSS_HAIR_END);
//	}
//	break;
//
//	case C_CROSS_HAIR_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_END[%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//		}
//
//		if (LAS[LaserNum].m_isError)
//		{
//			OPR.isStop = TRUE;
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (LAS[LaserNum].m_isWaitData)
//			break;
//
//		if (IsCrossHairComp())
//		{
//			if (CROSS_CAL_PLATE == m_pFsm->GetMsg())
//			{
//				MtPosMove(MtXNum, PLX_WAIT);
//				MtPosMove(MtYNum, PY_LOADING);
//
//				MSGNOTICE.Set(MSG_NOTICE,
//					"CLOSE", NULL, NULL, NULL,
//					"<%s> Cross Hair Calibaration Finish..!", m_strName);
//			}
//			m_pFsm->Set(C_IDLE);
//		}
//		else
//		{
//			LOG[logSEQ].Message("<%s> C_CROSS_HAIR_RE_START", m_strName);
//			m_pFsm->Set(C_CROSS_HAIR_START);
//		}
//	}
//	break;
//	}
//}
//
//void Stage::ScanCalCycle(void)
//{
//	if (!m_pFsm->Between(C_SCAN_CAL_START, C_SCAN_CAL_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"ScanCalCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(600000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_SCANCAL_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (!MT[MtXNum].IsRdy() ||
//		!MT[MtYNum].IsRdy() ||
//		!MT[MtZNum].IsRdy())
//		return;
//
//	VC[vcIndex].On();
//	if (!VC[vcIndex].IsOn(1000))
//		return;
//
//	switch (m_pFsm->Get())
//	{
//	case C_SCAN_CAL_START:
//	{
//		if (!VI[ViNum].IsAuto() && !OPR.isDryRun)
//		{
//			ER.Save(ER_VISION_NOT_AUTO);
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_START", m_strName);
//
//			m_scanCalCnt = 0;
//
//			NV.sys[SCAN_CAL_PITCH_X] = NV.sys[SCAN_CAL_FOV_X] / (NV.sys[SCAN_CAL_POINT_CNT] - 1.0);
//			NV.sys[SCAN_CAL_PITCH_Y] = NV.sys[SCAN_CAL_FOV_Y] / (NV.sys[SCAN_CAL_POINT_CNT] - 1.0);
//
//			m_dFOV.dX = NV.sys[SCAN_CAL_FOV_X];
//			m_dFOV.dY = NV.sys[SCAN_CAL_FOV_Y];
//			m_dMarkPitch.dX = NV.sys[SCAN_CAL_PITCH_X];
//			m_dMarkPitch.dY = NV.sys[SCAN_CAL_PITCH_Y];
//
//			LAS[LaserNum].Clear();
//			LAS[LaserNum].Reset();
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//
//			m_tmCycleTime.Reset();
//			m_inspMode = MODE_PRS_STAGECAL;
//			VI[ViNum].SetMode(m_inspMode);
//			VI[ViNum].Reset();
//		}
//
//		int refNo = m_no + 1;
//		if (m_no % 2)
//			refNo = m_no - 1;
//
//		if (IsRefIndexBusy())
//		{
//			MSGNOTICE.Set(MSG_NOTICE,
//				"CLOSE", NULL, NULL, NULL,
//				"Another Index Is Cycle Running");
//
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (!NV.use[USE_DUST] ||
//			!OPR.isNoDevice && NV.use[USE_DUST] &&
//			AOn(DUSTCOLLECTOR.iRunNo1, (DWORD)NV.sys[SET_DUST_WARMUP_TIME]) &&
//			AOn(DUSTCOLLECTOR.iRunNo2, (DWORD)NV.sys[SET_DUST_WARMUP_TIME]))
//			m_pFsm->Set(C_SCAN_CAL_MOVE_MARKING);
//	}
//	break;
//
//	case C_SCAN_CAL_MOVE_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_SCAN_CAL_MOVE_MARKING: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_SCAN_CAL_MOVE_MARKING", m_strName);
//			}
//		}
//
//		m_posCalMarking = GetScanCalPos();
//		POINT2D posError = GetStageCalErr(m_posCalMarking);
//		POINT2D posAlign = m_posCalMarking + posError;
//
//		if (MT[MtYNum].InCmd(PY_SCAN_CAL, posAlign.dY) &&
//			MT[MtXNum].InCmd(PLX_SCAN_CAL_1 + (m_no % 2), posAlign.dX) &&
//			MT[MtZNum].InCmd(PLZ_SCAN_CAL_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_MARKING],m_posCalMarking,%.3f,%.3f", m_strName, m_posCalMarking.dX, m_posCalMarking.dY);
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_MARKING],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_MARKING],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//			m_pFsm->Set(C_SCAN_CAL_SEND_COORD);
//		}
//		else
//		{
//			MtPosMove(MtYNum, PY_SCAN_CAL, posAlign.dY);
//			MtPosMove(MtXNum, PLX_SCAN_CAL_1 + (m_no % 2), posAlign.dX);
//			MtPosMove(MtZNum, PLZ_SCAN_CAL_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_SCAN_CAL_SEND_COORD:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_SEND_COORD", m_strName);
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//			LAS[LaserNum].m_scanData.scanCal.iCalPoint = (int)NV.sys[SCAN_CAL_POINT_CNT];
//			LAS[LaserNum].m_scanData.scanCal.dPitch = m_dMarkPitch;
//			LAS[LaserNum].m_scanData.scanCal.dFOV = m_dFOV;
//			LAS[LaserNum].SetMode(MODE_FIELD_CORRECTION);
//
//			if (!OPR.isDryRun && !LAS[LaserNum].SendCoordData())
//			{
//				ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
//				m_pFsm->Set(C_ERROR);
//				break;
//			}
//
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				LAS[LaserNum].m_isWaitCoordData1 = false;
//				LAS[LaserNum].m_isWaitCoordData2 = false;
//			}
//		}
//
//		if (!LAS[LaserNum].m_isWaitCoordData1 && !LAS[LaserNum].m_isWaitCoordData2)
//		{
//			m_pFsm->Set(C_SCAN_CAL_MARKING);
//		}
//		else if (LAS[LaserNum].m_tmWaitCoordData.TmOver(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_SEND_SCAN_COORD_TIMEOVER + m_no));
//			m_pFsm->Set(C_ERROR);
//			return;
//		}
//	}
//	break;
//
//	case C_SCAN_CAL_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_SCAN_CAL_MARKING: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_SCAN_CAL_MARKING", m_strName);
//			}
//		}
//
//		if (LAS[LaserNum].m_isError)
//		{
//			OPR.isStop = TRUE;
//			ER.Save((ErrName)(ER_INDEX1_SCANNER_ERROR + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[SET_LASER_DELAY_MS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isDryRun || LAS[LaserNum].Start())
//			m_pFsm->Set(C_SCAN_CAL_MOVE_VISION_CEN);
//	}
//	break;
//
//	case C_SCAN_CAL_MOVE_VISION_CEN:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_MOVE_VISION_CEN", m_strName);
//			VI[ViNum].Reset();
//		}
//
//		if (LAS[LaserNum].m_isError)
//		{
//			OPR.isStop = TRUE;
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (LAS[LaserNum].m_isWaitData)
//			break;
//
//		m_posCalInsp = m_posCalMarking - ViToLas();
//		POINT2D posError = GetGlassCalErr(m_posCalInsp);
//		POINT2D posAlign = m_posCalInsp + posError;
//
//		if (MT[MtXNum].InCmd(PLX_SCAN_CAL_1 + (m_no % 2), posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_SCAN_CAL, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_VI_SCAN_CAL_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_VISION_CEN],m_posCalInsp,%.3f,%.3f", m_strName, m_posCalInsp.dX, m_posCalInsp.dY);
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_VISION_CEN],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_VISION_CEN],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//			m_pFsm->Set(C_SCAN_CAL_VISION_CEN_INSP);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_SCAN_CAL_1 + (m_no % 2), posAlign.dX);
//			MtPosMove(MtYNum, PY_SCAN_CAL, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_SCAN_CAL_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_SCAN_CAL_VISION_CEN_INSP:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_VISION_CEN_INSP", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_SCAN_CAL_VISION_CEN_RESULT);
//	}
//	break;
//
//	case C_SCAN_CAL_VISION_CEN_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_VISION_CEN_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.result[0] = 1;
//				SetXYT(VI[ViNum].m_prsData.data[0], 0);
//			}
//		}
//
//		if (m_pFsm->Delay(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_RCV_VI_RESULT + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (VI[ViNum].IsBusy() || VI[ViNum].IsStart())
//			break;
//
//		if (!VI[ViNum].m_isRcvComp)
//			break;
//
//		LOG[logSEQ].Message("<%s> C_SCAN_CAL_VISION_CEN_RESULT - COMP", m_strName);
//
//		if (!VI[ViNum].m_prsData.result[0] ||
//			5.0 < fabs(VI[ViNum].m_prsData.data[0].dX) ||
//			5.0 < fabs(VI[ViNum].m_prsData.data[0].dY))
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_VI_INSPECTION + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		ViToLas().dX -= VI[ViNum].m_prsData.data[0].dX;
//		ViToLas().dY -= VI[ViNum].m_prsData.data[0].dY;
//		m_pFsm->Set(C_SCAN_CAL_MOVE_VISION);
//	}
//	break;
//
//	case C_SCAN_CAL_MOVE_VISION:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_SCAN_CAL_MOVE_VISION: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_SCAN_CAL_MOVE_VISION", m_strName);
//			}
//						
//			VI[ViNum].Reset();
//		}
//
//		m_posCalInsp = GetScanCalViPos(m_scanCalCnt);
//		POINT2D posError = GetGlassCalErr(m_posCalInsp);
//		POINT2D posAlign = m_posCalInsp + posError;
//
//		if (MT[MtXNum].InCmd(PLX_SCAN_CAL_1 + (m_no % 2), posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_SCAN_CAL, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_VI_SCAN_CAL_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_VISION],m_posCalInsp,%.3f,%.3f", m_strName, m_posCalInsp.dX, m_posCalInsp.dY);
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_VISION],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//			LOG[logLAS].Message("<%s>,[C_SCAN_CAL_MOVE_VISION],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//			m_pFsm->Set(C_SCAN_CAL_VISION_INSP);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_SCAN_CAL_1 + (m_no % 2), posAlign.dX);
//			MtPosMove(MtYNum, PY_SCAN_CAL, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_SCAN_CAL_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_SCAN_CAL_VISION_INSP:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_VISION_INSP", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_SCAN_CAL_VISION_RESULT);
//	}
//	break;
//
//	case C_SCAN_CAL_VISION_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_VISION_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.result[0] = 1;
//				SetXYT(VI[ViNum].m_prsData.data[0], 0);
//			}
//		}
//
//		if (m_pFsm->Delay(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_RCV_VI_RESULT + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (VI[ViNum].IsBusy() || VI[ViNum].IsStart())
//			break;
//
//		if (!VI[ViNum].m_isRcvComp)
//			break;
//
//		if (!VI[ViNum].m_prsData.result[0] ||
//			5.0 < fabs(VI[ViNum].m_prsData.data[0].dX) ||
//			5.0 < fabs(VI[ViNum].m_prsData.data[0].dY))
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_VI_INSPECTION + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		m_pFsm->Set(C_SCAN_CAL_COLLECT_DATA);
//	}
//	break;
//
//	case C_SCAN_CAL_COLLECT_DATA:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_COLLECT_DATA", m_strName);
//		}
//
//		int markCnt = (int)NV.sys[SCAN_CAL_POINT_CNT];
//		int maxCnt = markCnt * markCnt;
//		int cenMarkNo = (markCnt - 1) / 2;
//
//		int xIdx = m_scanCalCnt / markCnt;
//		int yIdx = m_scanCalCnt % markCnt;
//
//		m_scanCalQcViData[xIdx][yIdx].dX = VI[ViNum].m_prsData.data[0].dX;
//		m_scanCalQcViData[xIdx][yIdx].dY = VI[ViNum].m_prsData.data[0].dY;
//		m_scanCalCnt++;
//
//		if (maxCnt <= m_scanCalCnt)
//		{
//			POINT2D cenMarkCoord;
//			cenMarkCoord.dX = cenMarkNo * NV.sys[SCAN_CAL_PITCH_X];
//			cenMarkCoord.dY = cenMarkNo * NV.sys[SCAN_CAL_PITCH_Y];
//
//			POINT2D cenQcViData = m_scanCalQcViData[cenMarkNo][cenMarkNo];
//			POINT2D curQcViData, curMarkCoord;
//			
//			for (int xCnt = 0; xCnt < markCnt; xCnt++)
//			{
//				for (int yCnt = 0; yCnt < markCnt; yCnt++)
//				{
//					curMarkCoord.dX = ((NV.sys[SCAN_CAL_PITCH_X] * xCnt) - cenMarkCoord.dX);
//					curMarkCoord.dY = (cenMarkCoord.dY - (NV.sys[SCAN_CAL_PITCH_Y] * yCnt));
//
//					curQcViData = m_scanCalQcViData[xCnt][yCnt] - cenQcViData;
//
//					LAS[LaserNum].m_scanData.scanCal.val[xCnt][yCnt].dX = (curMarkCoord.dX + curQcViData.dX);
//					LAS[LaserNum].m_scanData.scanCal.val[xCnt][yCnt].dY = (curMarkCoord.dY + curQcViData.dY);
//				}
//			}
//
//			m_pFsm->Set(C_SCAN_CAL_SEND_DATA);
//		}
//		else
//		{
//			m_pFsm->Set(C_SCAN_CAL_MOVE_VISION);
//		}
//
//	}
//	break;
//
//	case C_SCAN_CAL_SEND_DATA:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_SEND_DATA", m_strName);
//
//			LAS[LaserNum].SetMode(MODE_SCANCAL);
//			if (!OPR.isDryRun && !LAS[LaserNum].SendCoordData())
//			{
//				ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
//				m_pFsm->Set(C_ERROR);
//				return;
//			}
//
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				LAS[LaserNum].m_isWaitCoordData1 = false;
//				LAS[LaserNum].m_isWaitCoordData2 = false;
//			}
//		}
//
//		if (!LAS[LaserNum].m_isWaitCoordData1 && !LAS[LaserNum].m_isWaitCoordData2)
//		{
//			m_pFsm->Set(C_SCAN_CAL_END);
//		}
//		else if (LAS[LaserNum].m_tmWaitCoordData.TmOver(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_SEND_SCANCAL_DATA_TIMEOVER + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//
//	case C_SCAN_CAL_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_SCAN_CAL_END[%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//		}
//
//		MtPosMove(MtXNum, PLX_WAIT);
//		MtPosMove(MtYNum, PY_LOADING);
//
//		if (NOT_DEFINED == m_pFsm->GetMsg())
//		{
//			MSGNOTICE.Set(MSG_NOTICE,
//				"CLOSE", NULL, NULL, NULL,
//				"<%s> Scanner Calibaration Finish..!", m_strName);
//		}
//
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}
//
//POINT2D Stage::GetScanCalPos(void)
//{
//	POINT2D scanCalPos = { 0,0 };
//	scanCalPos.dX = MT[MtXNum].PosTable(PLX_SCAN_CAL_1 + (m_no % 2));
//	scanCalPos.dY = MT[MtYNum].PosTable(PY_SCAN_CAL);
//	
//	return scanCalPos;
//}
//
//POINT2D Stage::GetScanCalViPos(int cnt)
//{
//	int markCnt = (int)NV.sys[SCAN_CAL_POINT_CNT];
//
//	if (!Between(markCnt, 3, 17))
//	{
//		NV.sys[SCAN_CAL_POINT_CNT] = 17;
//		markCnt = (int)NV.sys[SCAN_CAL_POINT_CNT];
//	}
//
//	int xCnt = cnt / markCnt;
//	int yCnt = cnt % markCnt;
//
//	int cenMarkNo = (markCnt - 1) / 2;
//
//	POINT2D offset;
//	offset.dX = (xCnt - cenMarkNo) * NV.sys[SCAN_CAL_PITCH_X];
//	offset.dY = (cenMarkNo - yCnt) * NV.sys[SCAN_CAL_PITCH_Y];
//
//	POINT2D posVi;
//	posVi.dX = MT[MtXNum].PosTable(PLX_SCAN_CAL_1 + (m_no % 2)) - ViToLas().dX;
//	posVi.dY = MT[MtYNum].PosTable(PY_SCAN_CAL) - ViToLas().dY;
//	
//	POINT2D ptRet = posVi + offset;
//	return (ptRet);
//}
//
//POINT2D Stage::GetCrossHairPos(int nCrossHairMode)
//{
//	POINT2D crossHairPos;	
//	if (CROSS_CAL_SUB == nCrossHairMode)
//	{
//		int xCnt = NV.ndm[(NDmList)(CROSS_HAIR_CNT_LASER1 + m_no)] % (INT32)NV.sys[CROSS_HAIR_X_CNT];
//		int yCnt = NV.ndm[(NDmList)(CROSS_HAIR_CNT_LASER1 + m_no)] / (INT32)NV.sys[CROSS_HAIR_X_CNT];
//
//		crossHairPos.dX = MT[MtXNum].PosTable(PLX_CROSS_HAIR_SUB_1 + (m_no % 2)) + (xCnt * NV.sys[CROSS_HAIR_X_PITCH]);
//		crossHairPos.dY = MT[MtYNum].PosTable(PY_CROSS_HAIR_SUB) + (yCnt * NV.sys[CROSS_HAIR_Y_PITCH]);
//	}
//	else if (CROSS_CAL_FILM == nCrossHairMode)
//	{
//		POINT2D coord;
//		coord.dX = NV.pkg[PCB_CROSS_HAIR_PITCH_X];
//		coord.dY = NV.pkg[PCB_CROSS_HAIR_PITCH_Y];
//		crossHairPos = GetVisPos(coord) + ViToLas();
//	}
//	else
//	{
//		crossHairPos.dX = MT[MtXNum].PosTable(PLX_CROSS_HAIR_1 + (m_no % 2));
//		crossHairPos.dY = MT[MtYNum].PosTable(PY_CROSS_HAIR);
//	}
//	return crossHairPos;
//}