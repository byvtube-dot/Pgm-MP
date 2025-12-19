#include "..\..\Includes.h"

//void Stage::OrgCycle(void)
//{
//	if (!m_pFsm->Between(C_ORG_START, C_ORG_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"OrgCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(30000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_ORG_CYCLE_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//	if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//		return;
//
//	if (!IsExist())
//	{
//		ER.Save((ErrName)(ER_INDEX_1_MEMORY_STATUS + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	VC[vcIndex].On();
//	if (!VC[vcIndex].AOn())
//	{
//		if (m_pFsm->Delay(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX_1_VACUUM_STATUS + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//		return;
//	}
//
//	switch (m_pFsm->Get())
//	{
//	case C_ORG_START:
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
//			LOG[logSEQ].Message("<%s> C_ORG_START", m_strName);
//			viCnt = 0;
//			m_cycleTimeCnt = 0;
//			m_tmCycleTime.Reset();
//			m_inspMode = MODE_PRS_ORG;
//			VI[ViNum].SetMode(m_inspMode);
//			VI[ViNum].Reset();
//			//ZeroMemory(&m_unitPRs, sizeof(m_unitPRs));
//		}
//
//		m_pFsm->Set(C_ORG_MOVE);
//	}
//	break;
//
//	case C_ORG_MOVE:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_ORG_MOVE", m_strName);
//		}
//		
//		if (MT[MtXNum].InCmd(PLX_ORG_POS_1 + (m_no % 2)) &&
//			MT[MtYNum].InCmd(PY_ORG_POS) &&
//			MT[MtZNum].InCmd(PLZ_VI_ORG_1 + (m_no % 2)))
//		{
//			m_pFsm->Set(C_ORG_INSPECTION);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_ORG_POS_1 + (m_no % 2));
//			MtPosMove(MtYNum, PY_ORG_POS);
//			MtPosMove(MtZNum, PLZ_VI_ORG_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_ORG_INSPECTION:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_ORG_INSPECTION", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_ORG_RECV_RESULT);
//	}
//	break;
//
//	case C_ORG_RECV_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_ORG_RECV_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.recvCnt = 1;
//				VI[ViNum].m_prsData.result[0] = 1;
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
//		if (VI[ViNum].m_prsData.result[0])
//		{
//			m_pFsm->Set(C_ORG_END);
//		}
//		else
//		{
//			ER.Save((ErrName)(ER_INDEX1_ORG_DATA_ERROR + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//
//	case C_ORG_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_ORG_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//		}
//		IsOrg() = TRUE;
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}
//
//void Stage::BarcodeCycle(void)
//{
//	if (!m_pFsm->Between(C_BARCODE_START, C_BARCODE_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"BarcodeCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(30000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_BARCODE_CYCLE_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//	if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//		return;
//
//	if (!IsExist())
//	{
//		ER.Save((ErrName)(ER_INDEX_1_MEMORY_STATUS + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	VC[vcIndex].On();
//	if (!VC[vcIndex].AOn())
//	{
//		if (m_pFsm->Delay(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX_1_VACUUM_STATUS + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//		return;
//	}
//
//	switch (m_pFsm->Get())
//	{
//	case C_BARCODE_START:
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
//			LOG[logSEQ].Message("<%s> C_BARCODE_START", m_strName);
//			viUnitCnt = 0;
//			m_cycleTimeCnt = 0;
//			m_tmCycleTime.Reset();
//			m_inspMode = MODE_PRS_FILM_BARCODE;
//			VI[ViNum].SetMode(m_inspMode);
//			VI[ViNum].Reset();
//			strcpy(TRACEABILITY.pcbInfo[IPC_MMI::FILM_INDEX1 + m_no].FilmBcd, "");
//		}
//
//		m_pFsm->Set(C_BARCODE_MOVE_FILM);
//	}
//	break;
//
//	case C_BARCODE_MOVE_FILM:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_MOVE_FILM", m_strName);
//		}
//
//		POINT2D coordVi = GetFilmBarPos();
//		POINT2D posVision = GetVisPos(coordVi);
//		POINT2D posError = GetGlassCalErr(posVision);
//		POINT2D posAlign = posVision + posError;
//
//		if (MT[MtXNum].InCmd(PLX_BARCODE_SPEED, posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_BARCODE_SPEED, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_VI_BARCODE_1 + (m_no % 2)))
//		{
//			m_pFsm->Set(C_BARCODE_TRIG_FILM);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_BARCODE_SPEED, posAlign.dX);
//			MtPosMove(MtYNum, PY_BARCODE_SPEED, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_BARCODE_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_BARCODE_TRIG_FILM:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_TRIG_FILM", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_BARCODE_RECV_FILM);
//	}
//	break;
//
//	case C_BARCODE_RECV_FILM:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_RECV_FILM", m_strName);
//
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_unitData.recvCnt = 1;
//				VI[ViNum].m_unitData.result[0] = 1;
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
//		if (VI[ViNum].m_unitData.result[0])
//		{
//			viCnt = 0;
//			int FilmNum = IPC_MMI::FILM_INDEX1 + m_no;
//			memcpy(&TRACEABILITY.pcbInfo[FilmNum].FilmBcd, &VI[ViNum].m_unitData.barcode, sizeof(TRACEABILITY.pcbInfo[FilmNum].FilmBcd));
//			m_pFsm->Set(C_BARCODE_MOVE_UNIT);
//		}
//		else
//		{
//			ER.Save((ErrName)(ER_INDEX1_BARCODE_DATA_ERROR + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//
//	case C_BARCODE_MOVE_UNIT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_MOVE_UNIT", m_strName);
//		}
//
//		m_inspMode = MODE_PRS_UNIT_BARCODE;
//		VI[ViNum].SetMode(m_inspMode);
//		VI[ViNum].Reset();
//
//		if (viUnitCnt < NV.pkg[PKG_UNIT_CNT])
//		{
//			if (FILM_MAP[m_no].val[viUnitCnt].isSelected)
//			{
//				POINT2D coordVi = GetUnitBarPos(viUnitCnt);
//				POINT2D posVision = GetVisPos(coordVi);
//				POINT2D posError = GetGlassCalErr(posVision);
//				POINT2D posAlign = posVision + posError;
//
//				if (MT[MtXNum].InCmd(PLX_BARCODE_SPEED, posAlign.dX) &&
//					MT[MtYNum].InCmd(PY_BARCODE_SPEED, posAlign.dY) &&
//					MT[MtZNum].InCmd(PLZ_VI_BARCODE_1 + (m_no % 2)))
//				{
//					m_pFsm->Set(C_BARCODE_TRIG_UNIT);
//				}
//				else
//				{
//					MtPosMove(MtXNum, PLX_BARCODE_SPEED, posAlign.dX);
//					MtPosMove(MtYNum, PY_BARCODE_SPEED, posAlign.dY);
//					MtPosMove(MtZNum, PLZ_VI_BARCODE_1 + (m_no % 2));
//				}
//			}
//			else
//			{
//				viUnitCnt++;
//			}
//		}
//		else
//		{
//			m_pFsm->Set(C_BARCODE_END);
//		}
//	}
//	break;
//
//	case C_BARCODE_TRIG_UNIT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_TRIG_UNIT", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_BARCODE_RECV_UNIT);
//	}
//	break;
//
//	case C_BARCODE_RECV_UNIT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_RECV_UNIT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_unitData.recvCnt = 1;
//				VI[ViNum].m_unitData.result[0] = 1;
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
//		if (VI[ViNum].m_unitData.result[0])
//		{
//			int FilmNum = IPC_MMI::FILM_INDEX1 + m_no;
//			memcpy(&TRACEABILITY.pcbInfo[FilmNum].UnitInfo[viUnitCnt].UnitBcd,
//				&VI[ViNum].m_unitData.barcode, sizeof(TRACEABILITY.pcbInfo[FilmNum].UnitInfo[viUnitCnt].UnitBcd));
//
//			viUnitCnt++;
//			m_pFsm->Set(C_BARCODE_MOVE_UNIT);
//		}
//		else
//		{
//			ER.Save((ErrName)(ER_INDEX1_BARCODE_DATA_ERROR + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//
//	case C_BARCODE_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_BARCODE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//		}
//		IsBarcode() = TRUE;
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}
//
//void Stage::UnitPRsCycle(void)
//{
//	if (!m_pFsm->Between(C_UNIT_PRS_START, C_UNIT_PRS_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"UnitPRsCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(30000)) 
//	{
//		ER.Save((ErrName)(ER_INDEX1_UNIT_PRS_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//	
//	DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//	if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//		return;
//
//	if (!IsExist())
//	{
//		ER.Save((ErrName)(ER_INDEX_1_MEMORY_STATUS + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//	
//	VC[vcIndex].On();
//	if (!VC[vcIndex].AOn())
//	{
//		if (m_pFsm->Delay(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX_1_VACUUM_STATUS + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//		return;
//	}
//
//	switch (m_pFsm->Get()) 
//	{
//	case C_UNIT_PRS_START:
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
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_START", m_strName);
//			
//			viCnt = 0;
//			viUnitCnt = 0;
//			m_cycleTimeCnt = 0;
//			m_tmCycleTime.Reset();
//			m_inspMode = MODE_PRS_UNIT_ALIGN;
//			VI[ViNum].Clear();
//			VI[ViNum].SetMode(m_inspMode);
//
//			int  FilmNum;
//			if (m_no == 0) FilmNum = IPC_MMI::FILM_INDEX1;
//			else if (m_no == 1) FilmNum = IPC_MMI::FILM_INDEX2;
//			ZeroMemory(&STAGE_DATA[m_no].PrsData, sizeof(STAGE_DATA[m_no].PrsData));
//		}
//
//		m_pFsm->Set(C_UNIT_PRS_CHECK_UNIT);
//	}
//	break;
//
//	case C_UNIT_PRS_CHECK_UNIT:
//	{
//		if (m_pFsm->IsCanceled())
//		{
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_CHECK_UNIT: IsCanceled", m_strName);
//			m_pFsm->Set(C_IDLE);
//			break;
//		}
//		else if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_CHECK_UNIT", m_strName);
//		}
//
//		if (viUnitCnt >= NV.pkg[PKG_UNIT_CNT])
//		{
//			m_pFsm->Set(C_UNIT_PRS_END);
//			break;
//		}
//
//		if (!FILM_MAP[m_no].val[viUnitCnt].isSelected || FILM_MAP[m_no].val[viUnitCnt].isLayer1Comp)
//		{
//			viUnitCnt++;
//			break;
//		}
//		else
//		{
//			viCnt = 0;
//			m_pFsm->Set(C_UNIT_PRS_CHECK_FIDUCIAL);
//		}
//	}
//	break;
//
//	case C_UNIT_PRS_CHECK_FIDUCIAL:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_CHECK_FIDUCIAL", m_strName);
//		}
//
//		if (viCnt >= STAGE_DATA[m_no].Dwg.Unit[viUnitCnt].nFiducialCnt)
//		{
//			viUnitCnt++;
//			m_pFsm->Set(C_UNIT_PRS_CHECK_UNIT);
//			break;
//		}
//		else
//		{
//			m_pFsm->Set(C_UNIT_PRS_MOVE);
//		}
//	}
//	break;
//
//	case C_UNIT_PRS_MOVE:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_MOVE", m_strName);
//			VI[ViNum].Reset();
//		}
//
//		POINT2D coordVi = GetUnitFiducialCoord(viUnitCnt, viCnt);
//		POINT2D posVision = GetVisPos(coordVi);
//		POINT2D posError = GetGlassCalErr(posVision);
//		POINT2D posAlign = posVision + posError;
//
//		if (MT[MtXNum].InCmd(PLX_TEACHING_SPEED, posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_TEACHING_SPEED, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_VI_PRS_1 + (m_no % 2)))
//		{
//			m_pFsm->Set(C_UNIT_PRS_INSPECTION);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//			MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_PRS_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_UNIT_PRS_INSPECTION:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_INSPECTION", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (/*OPR.isNoDevice || */VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_UNIT_PRS_RECV_RESULT);
//	}
//	break;
//
//	case C_UNIT_PRS_RECV_RESULT:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_RECV_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.result[0] = 1;
//				VI[ViNum].m_prsData.data[0].dX = 0.1;
//				VI[ViNum].m_prsData.data[0].dY = 0.1;
//				VI[ViNum].m_prsData.data[0].dT = 0.1;
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
//		if (!WriteVisResult(viUnitCnt,viCnt, VI[ViNum].m_prsData))
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_VI_INSPECTION + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		viCnt++;
//		m_pFsm->Set(C_UNIT_PRS_CHECK_FIDUCIAL);
//	}
//	break;
//
//	case C_UNIT_PRS_END:
//	{
//		if (m_pFsm->Once()) 
//		{
//			if (0 < m_cycleTimeCnt) 
//			{
//				double prsTotalTime = m_tmCycleTime.Elapsed();
//				double prs1unitTime = prsTotalTime / m_cycleTimeCnt;
//			}
//			LOG[logSEQ].Message("<%s> C_UNIT_PRS_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//		}
//
//		BOOL Create = TRUE;
//
//		for (int Unit = 0; Unit < STAGE_DATA[m_no].Dwg.nUnitCnt; Unit++)
//		{
//			if (FILM_MAP[m_no].val[Unit].isSelected)
//			{
//				if (CreateCoord(Unit))
//				{
//					LOG[logSEQ].Message("<%s> [%2d]Unit Success CreateCoord", m_strName, Unit);
//				}
//				else
//				{
//					LOG[logSEQ].Message("<%s> [%2d]Unit Fail CreateCoord", m_strName , Unit);
//				}
//			}
//		}
//		if (Create)
//		{
//			IsAlignComp() = TRUE;
//			m_pFsm->Set(C_IDLE);
//		}
//		else
//		{
//			ER.Save((ErrName)(ER_INDEX1_CREATE_UNIT_DATA_FAIL + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//	}
//}
//
//
//void Stage::QcCycle(void)
//{
//	if (!m_pFsm->Between(C_QC_START, C_QC_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"QcCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(30000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_QC_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//	if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//		return;
//
//	if (!IsExist())
//	{
//		ER.Save((ErrName)(ER_INDEX_1_MEMORY_STATUS + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	VC[vcIndex].On();
//	if (!VC[vcIndex].AOn())
//	{
//		if (m_pFsm->Delay(5000))
//		{
//			ER.Save((ErrName)(ER_INDEX_1_VACUUM_STATUS + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//		return;
//	}
//
//	switch (m_pFsm->Get())
//	{
//	case C_QC_START:
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
//			LOG[logSEQ].Message("<%s> C_QC_START", m_strName);
//
//			viCnt = 0;
//			viUnitCnt = 0;
//			m_cycleTimeCnt = 0;
//			m_tmCycleTime.Reset();
//			m_inspMode = MODE_PRS_PREQc;
//			VI[ViNum].SetMode(m_inspMode);
//			VI[ViNum].Reset();
//			int  FilmNum;
//			if (m_no == 0) FilmNum = IPC_MMI::FILM_INDEX1;
//			else if (m_no == 1) FilmNum = IPC_MMI::FILM_INDEX2;
//
////			ZeroMemory(TRACEABILITY.pcbInfo[FilmNum].UnitInfo[0].FiducialPos, sizeof(TRACEABILITY.pcbInfo[FilmNum].UnitInfo[0].FiducialPos));
//		}
//
//		m_pFsm->Set(C_QC_CHECK_UNIT);
//	}
//	break;
//
//	case C_QC_CHECK_UNIT:
//	{
//		if (m_pFsm->IsCanceled())
//		{
//			LOG[logSEQ].Message("<%s> C_QC_CHECK_UNIT: IsCanceled", m_strName);
//			m_pFsm->Set(C_IDLE);
//			break;
//		}
//		else if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_QC_CHECK_UNIT", m_strName);
//		}
//
//		if (viUnitCnt >= NV.pkg[PKG_UNIT_CNT])
//		{
//			m_pFsm->Set(C_QC_END);
//			break;
//		}
//
//		if (!FILM_MAP[m_no].val[viUnitCnt].isSelected)
//		{
//			viUnitCnt++;
//			break;
//		}
//		else
//		{
//			viCnt = 0;
//			m_pFsm->Set(C_QC_CHECK_FIDUCIAL);
//		}
//	}
//	break;
//
//	case C_QC_CHECK_FIDUCIAL:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_QC_CHECK_FIDUCIAL", m_strName);
//		}
//
//		if (viCnt >= STAGE_DATA[m_no].Dwg.Unit[viUnitCnt].nPreQcCnt)
//		{
//			viUnitCnt++;
//			m_pFsm->Set(C_QC_CHECK_UNIT);
//			break;
//		}
//		else
//		{
//			m_pFsm->Set(C_QC_MOVE);
//		}
//	}
//	break;
//
//	case C_QC_MOVE:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_QC_MOVE", m_strName);
//			VI[ViNum].Reset();
//		}
//
//		POINT2D coordVi = GetUnitQcCoord(viUnitCnt, viCnt);
//		POINT2D posVision = GetVisPos(coordVi);
//		POINT2D posError = GetGlassCalErr(posVision);
//		POINT2D posAlign = posVision + posError;
//
//		if (MT[MtXNum].InCmd(PLX_TEACHING_SPEED, posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_TEACHING_SPEED, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_VI_PRS_1 + (m_no % 2)))
//		{
//			m_pFsm->Set(C_QC_INSPECTION);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//			MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_PRS_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_QC_INSPECTION:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_QC_INSPECTION", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_QC_RECV_RESULT);
//	}
//	break;
//
//	case C_QC_RECV_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_QC_RECV_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
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
//		if (viCnt + 1 < STAGE_DATA[m_no].Dwg.Unit[viUnitCnt].nPreQcCnt)
//		{
//			viCnt++;
//			m_pFsm->Set(C_QC_CHECK_FIDUCIAL);
//			break;
//		}
//		else
//		{
//			if (!VI[ViNum].m_isRcvComp)
//				break;
//
//			if (!WriteVisQcResult(viUnitCnt, VI[ViNum].m_prsData))
//			{
//				ER.Save((ErrName)(ER_INDEX1_FAIL_VI_INSPECTION + m_no));
//				m_pFsm->Set(C_ERROR);
//				break;
//			}
//			viUnitCnt++;
//			m_pFsm->Set(C_QC_CHECK_UNIT);
//		}		
//	}
//	break;
//
//	case C_QC_END:
//	{
//		if (m_pFsm->Once())
//		{
//			if (0 < m_cycleTimeCnt)
//			{
//				double prsTotalTime = m_tmCycleTime.Elapsed();
//				double prs1unitTime = prsTotalTime / m_cycleTimeCnt;
//			}
//			LOG[logSEQ].Message("<%s> C_QC_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//		}
//		IsPreQcComp() = TRUE;
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}
//
//void Stage::ConfirmCycle(void)
//{
//	if (!m_pFsm->Between(C_CONFIRM_START, C_CONFIRM_END))
//		return;
//
//	if (m_pFsm->TimeLimit(600000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_CONFIRM_CYCLE_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (!IsExist())
//	{
//		ER.Save((ErrName)(ER_INDEX_1_MEMORY_STATUS + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (!MT[MtXNum].IsRdy() || !MT[MtYNum].IsRdy() || !MT[MtZNum].IsRdy())
//		return;
//
//	switch (m_pFsm->Get())
//	{
//	case C_CONFIRM_START:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_START", m_strName);
//			LaserCnt = 0;
//			LaserStepCnt = 0;
//			m_cycleTimeCnt = 0;
//			m_tmCycleTime.Reset();
//
//			VI[ViNum].Clear();
//		}
//		else
//		{
//			m_pFsm->Set(C_CONFIRM_CHECK_UNIT);
//		}
//	}
//	break;
//
//	case C_CONFIRM_CHECK_UNIT:
//	{
//		if (m_pFsm->IsCanceled())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_CHECK_UNIT: IsCanceled", m_strName);
//			m_pFsm->Set(C_IDLE);
//			break;
//		}
//		else if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_CHECK_UNIT", m_strName);
//		}
//
//		if (LaserCnt >= STAGE_DATA[m_no].Dwg.nUnitCnt)
//		{
//			m_pFsm->Set(C_CONFIRM_END);
//			break;
//		}
//
//		if (!FILM_MAP[m_no].val[LaserCnt].isSelected)
//		{
//			LaserCnt++;
//			break;
//		}
//		else
//		{
//			LaserStepCnt = 0;
//			m_pFsm->Set(C_CONFIRM_CHECK_STEP);
//		}
//	}
//	break;
//
//	case C_CONFIRM_CHECK_STEP:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_CHECK_STEP", m_strName);
//		}
//
//		if (LaserStepCnt >= STAGE_DATA[m_no].Dwg.Unit[LaserCnt].nStepCnt)
//		{
//			LaserCnt++;
//			m_pFsm->Set(C_CONFIRM_CHECK_UNIT);
//			break;
//		}
//		else
//		{
//			if (CreateScannerCoord(LaserCnt, LaserStepCnt))
//			{
//				m_pFsm->Set(C_CONFIRM_MOVE);
//			}
//			else
//			{
//				ER.Save((ErrName)(ER_INDEX1_CREATE_SCAN_COORD_FAIL + m_no));
//				LOG[logSEQ].Message("<%s> C_CONFIRM_CHECK_STEP: CreateScannerCoord Error", m_strName);
//				m_pFsm->Set(C_ERROR);
//			}
//		}
//	}
//	break;
//
//	case C_CONFIRM_MOVE:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_MOVE", m_strName);
//
//			ViMode viConfirm = ViMode::MODE_PRS_UNIT;
//			VI[ViNum].SetMode(viConfirm);
//			VI[ViNum].Reset();
//		}
//
//		BOOL isSuccess = FALSE;
//		POINT2D coordFovCen = GetLasFovCenCoord(LaserCnt, LaserStepCnt);
//		POINT2D posFovCen = GetVisPos(coordFovCen);
//		POINT2D posError = GetStageCalErr(posFovCen);
//		POINT2D posAlign = posFovCen + posError;
//
//		if (MT[MtXNum].InCmd(PLX_CENTER_POS_1 + (m_no % 2), posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_CENTER_POS, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_LASER_1 + (m_no % 2)))
//		{
//			m_pFsm->Set(C_CONFIRM_INSPECTION);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_CENTER_POS_1 + (m_no % 2), posAlign.dX);
//			MtPosMove(MtYNum, PY_CENTER_POS, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_LASER_1 + (m_no % 2));
//		}
//	}
//	break;
//
//	case C_CONFIRM_INSPECTION:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_INSPECTION", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_CONFIRM_RECV_RESULT);
//	}
//	break;
//
//	case C_CONFIRM_RECV_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_RECV_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.recvCnt = 1;
//				VI[ViNum].m_prsData.result[0] = 1;
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
//		if ((LaserStepCnt + 1) < STAGE_DATA[m_no].Dwg.Unit[LaserCnt].nStepCnt)
//		{
//			LaserStepCnt++;
//			m_pFsm->Set(C_CONFIRM_CHECK_STEP);
//		}
//		else
//		{
//			LaserStepCnt = 0;
//			LaserCnt++;
//			m_pFsm->Set(C_LASER_CHECK_UNIT);//??
//		}
//	}
//	break;
//
//	case C_CONFIRM_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_CONFIRM_END", m_strName);
//		}
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}