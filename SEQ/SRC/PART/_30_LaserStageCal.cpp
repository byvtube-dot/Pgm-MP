#include "..\..\Includes.h"

//void Stage::StageCalCycle(void)
//{
//	if (!m_pFsm->Between(C_STAGE_CAL_START, C_STAGE_CAL_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"StageCalCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(600000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_STAGE_CAL_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//	
//	if (!MT[MtXNum].IsRdy() || !MT[MtYNum].IsRdy() || !MT[MtZNum].IsRdy())
//		return;
//
//	switch (m_pFsm->Get())
//	{
//	case C_STAGE_CAL_START:
//	{
//		if (!VI[ViNum].IsAuto() && !OPR.isDryRun)
//		{
//			ER.Save(ER_VISION_NOT_AUTO);
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_START", m_strName);
//
//
//			m_tmCycleTime.Reset();
//
//			m_inspMode = MODE_PRS_STAGECAL;
//			VI[ViNum].Clear();
//			VI[ViNum].SetMode(m_inspMode);
//			LAS[LaserNum].Clear();
//			LAS[LaserNum].Reset();
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//			viCnt = 0;
//			LaserCnt = 0;
//			ZeroMemory(m_viResult, sizeof(m_viResult));
//			ZeroMemory(&m_stageCal, sizeof(m_stageCal));
//			m_stageCal.Count.x = (int)NV.sys[STAGE_CAL_CNT_X];
//			m_stageCal.Count.y = (int)NV.sys[STAGE_CAL_CNT_Y];
//			m_stageCal.VectorPitchX.dX = m_stageCal.PitchX = NV.sys[STAGE_CAL_PITCH_X];
//			m_stageCal.VectorPitchY.dY = m_stageCal.PitchY = NV.sys[STAGE_CAL_PITCH_Y];
//			m_stageCal.StartPos.dX = MT[MtXNum].PosTable(PLX_STAGE_CAL_START_1 + (m_no % 2));
//			m_stageCal.StartPos.dY = MT[MtYNum].PosTable(PY_STAGE_CAL_START);
//			STAGE_CAL[m_no].StartPos = m_stageCal.StartPos;
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
//			m_pFsm->Set(C_STAGE_CAL_MOVE_MARKING);
//	}
//	break;
//
//	case C_STAGE_CAL_MOVE_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_STAGE_CAL_MOVE_MARKING: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_STAGE_CAL_MOVE_MARKING", m_strName);
//			}
//		}
//
//		int maxCnt = m_stageCal.Count.x * m_stageCal.Count.y;
//		if (maxCnt > LaserCnt)
//		{
//			int xIdx = LaserCnt / m_stageCal.Count.y;
//			int yIdx = LaserCnt % m_stageCal.Count.y;
//			POINT2D offset = (m_stageCal.VectorPitchX * xIdx) + (m_stageCal.VectorPitchY * yIdx);
//			m_posCalMarking = m_stageCal.StartPos + offset;
//			POINT2D posError = m_stageCal.val[xIdx][yIdx] = GetStageCalErr(m_posCalMarking);
//			POINT2D posAlign = m_posCalMarking + posError;
//
//			if (MT[MtXNum].InCmd(PLX_TEACHING_SPEED, posAlign.dX) &&
//				MT[MtYNum].InCmd(PY_TEACHING_SPEED, posAlign.dY) &&
//				MT[MtZNum].InCmd(PLZ_SCAN_CAL_1 + (m_no % 2)))
//			{
//				LOG[logLAS].Message("<%s>,[C_STAGE_CAL_MOVE_MARKING],m_posCalInsp,%.3f,%.3f", m_strName, m_posCalMarking.dX, m_posCalMarking.dY);
//				LOG[logLAS].Message("<%s>,[C_STAGE_CAL_MOVE_MARKING],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//				LOG[logLAS].Message("<%s>,[C_STAGE_CAL_MOVE_MARKING],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//				m_pFsm->Set(C_STAGE_CAL_SEND_COORD);
//			}
//			else
//			{
//				MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//				MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//				MtPosMove(MtZNum, PLZ_SCAN_CAL_1 + (m_no % 2));
//			}
//		}
//		else
//		{
//			LaserCnt = 0;
//			viCnt = 0;
//			m_pFsm->Set(C_STAGE_CAL_MOVE_VISION);
//		}
//	}
//	break;
//	case C_STAGE_CAL_SEND_COORD:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_SEND_COORD", m_strName);
//
//			LAS[LaserNum].m_scanData.iStageNo = m_no;
//			LAS[LaserNum].m_scanData.scanCoord.unitCnt = 1;
//			LAS[LaserNum].SetMode(MODE_CROSSHAIR);
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
//			m_pFsm->Set(C_STAGE_CAL_MARKING);
//		}
//		else if (LAS[LaserNum].m_tmWaitCoordData.TmOver(10000))
//		{
//			ER.Save((ErrName)(ER_INDEX1_SEND_SCAN_COORD_TIMEOVER + m_no));
//			m_pFsm->Set(C_ERROR);
//		}
//	}
//	break;
//
//	case C_STAGE_CAL_MARKING:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_MARKING", m_strName);	
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
//		if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isDryRun || LAS[LaserNum].Start())
//			m_pFsm->Set(C_STAGE_CAL_MARKING_BUSY);	
//	}
//	break;
//
//	case C_STAGE_CAL_MARKING_BUSY:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_MARKING_BUSY", m_strName);
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
//		LaserCnt++;
//		m_pFsm->Set(C_STAGE_CAL_MOVE_MARKING);
//	}
//	break;
//	case C_STAGE_CAL_MOVE_VISION:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_STAGE_CAL_MOVE_VISION: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_STAGE_CAL_MOVE_VISION", m_strName);
//			}
//
//			VI[ViNum].Reset();
//		}
//
//		int maxCnt = m_stageCal.Count.x * m_stageCal.Count.y;
//		if (maxCnt > viCnt)
//		{
//			int xIdx = viCnt / m_stageCal.Count.y;
//			int yIdx = viCnt % m_stageCal.Count.y;
//			POINT2D offset = (m_stageCal.VectorPitchX * xIdx) + (m_stageCal.VectorPitchY * yIdx);
//			m_posCalInsp = m_stageCal.StartPos + offset - ViToLas();
//			POINT2D posError = GetGlassCalErr(m_posCalInsp);
//			POINT2D posAlign = m_posCalInsp + posError;
//
//			if (MT[MtXNum].InCmd(PLX_TEACHING_SPEED, posAlign.dX) &&
//				MT[MtYNum].InCmd(PY_TEACHING_SPEED, posAlign.dY) &&
//				MT[MtZNum].InCmd(PLZ_VI_SCAN_CAL_1 + (m_no % 2)))
//			{
//				LOG[logLAS].Message("<%s>,[C_STAGE_CAL_MOVE_VISION],m_posCalInsp,%.3f,%.3f", m_strName, m_posCalInsp.dX, m_posCalInsp.dY);
//				LOG[logLAS].Message("<%s>,[C_STAGE_CAL_MOVE_VISION],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//				LOG[logLAS].Message("<%s>,[C_STAGE_CAL_MOVE_VISION],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//				m_pFsm->Set(C_STAGE_CAL_VISION_INSP);
//			}
//			else
//			{
//				MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//				MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//				MtPosMove(MtZNum, PLZ_VI_SCAN_CAL_1 + (m_no % 2));
//			}
//		}
//		else
//		{
//			m_pFsm->Set(C_STAGE_CAL_COLLECT_DATA);
//		}
//	}
//	break;
//
//	case C_STAGE_CAL_VISION_INSP:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_VISION_INSP", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_STAGE_CAL_VISION_RESULT);
//	}
//	break;
//
//	case C_STAGE_CAL_VISION_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_VISION_RESULT", m_strName);
//			if (OPR.isNoDevice || OPR.isDryRun)
//			{
//				VI[ViNum].m_isRcvComp = true;
//				VI[ViNum].m_prsData.result[0] = 1;
//				SetXYT(VI[ViNum].m_prsData.data[0], 0);
//			}
//		}
//
//		if (m_pFsm->Delay(10000)) 
//		{
//			ER.Save((ErrName)(ER_INDEX1_FAIL_RCV_VI_RESULT + m_no));
//			m_pFsm->Set(C_ERROR);
//			break;
//		}
//
//		if (VI[ViNum].IsBusy() || VI[ViNum].IsStart())
//			break;
//
//		int maxCnt = m_stageCal.Count.x * m_stageCal.Count.y;
//		if (maxCnt > (viCnt + 1))
//		{
//			int xIdx = (viCnt + 1) / m_stageCal.Count.y;
//			int yIdx = (viCnt + 1) % m_stageCal.Count.y;
//			POINT2D offset = (m_stageCal.VectorPitchX * xIdx) + (m_stageCal.VectorPitchY * yIdx);
//			m_posCalInsp = m_stageCal.StartPos + offset - ViToLas();
//			POINT2D posError = GetGlassCalErr(m_posCalInsp);
//			POINT2D posAlign = m_posCalInsp + posError;
//
//			MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//			MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_GLASS_CAL_1 + (m_no % 2));
//		}
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
//		m_pFsm->Set(C_STAGE_CAL_COLLECT_DATA);
//	}
//	break;
//
//	case C_STAGE_CAL_COLLECT_DATA:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_COLLECT_DATA", m_strName);
//		}
//
//		int maxCnt = m_stageCal.Count.x * m_stageCal.Count.y;
//
//		int xIdx = viCnt / m_stageCal.Count.y;
//		int yIdx = viCnt % m_stageCal.Count.y;
//
//		m_viResult[xIdx][yIdx].dX = VI[ViNum].m_prsData.data[0].dX;
//		m_viResult[xIdx][yIdx].dY = VI[ViNum].m_prsData.data[0].dY;
//
//		viCnt++;
//
//		if (maxCnt <= viCnt)
//		{
//			for (int xCnt = 0; xCnt < m_stageCal.Count.x; xCnt++)
//			{
//				for (int yCnt = 0; yCnt < m_stageCal.Count.y; yCnt++)
//				{
//					m_stageCal.val[xCnt][yCnt].dX -= m_viResult[xCnt][yCnt].dX;
//					m_stageCal.val[xCnt][yCnt].dY -= m_viResult[xCnt][yCnt].dY;
//				}
//			}
//
//			m_pFsm->Set(C_STAGE_CAL_END);
//		}
//		else
//		{
//			m_pFsm->Set(C_STAGE_CAL_MOVE_VISION);
//		}
//	}
//	break;
//
//	case C_STAGE_CAL_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_STAGE_CAL_END", m_strName);
//		}
//		memcpy(&STAGE_CAL[m_no], &m_stageCal, sizeof(STAGE_CAL[m_no]));
//		STAGE_CAL[m_no].isSuccess = (TRUE);
//		MtPosMove(MtXNum, PLX_WAIT);
//		MtPosMove(MtYNum, PY_LOADING);
//		MtPosMove(MtZNum, PLZ_WAIT);
//	
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"<%s> Stage Calibaration Finish..!", m_strName);
//
//		NV_STAGE_CAL.Flush();
//		CreateStageCalFile();
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}
//
////POINT2D Stage::GetStagePosAddCompenErr(POINT2D pos)
////{
////	POINT2D compenErr = GetStageCalErr(pos);
////	POINT2D posLaser2 = pos + compenErr;
////
////	return (posLaser2);
////}
//
//POINT2D Stage::GetStageCalPosXY(int idxX, int idxY)
//{
//	POINT2D pos;
//	kasiolib::GetCalibrationPosXY(&STAGE_CAL[m_no], idxX, idxY, &pos);
//	return pos;
//}
//
//POINT2D Stage::GetStageCalErr(POINT2D pos)
//{
//	POINT2D calErr;
//	if (!kasiolib::GetCalibrationErr(&STAGE_CAL[m_no], &pos, &calErr))
//	{
//		calErr.dX = calErr.dY = 0.;
//		// Need Message.
//	}
//	return (calErr);
//}
//
//void Stage::CreateStageCalFile(void)
//{
//	time_t curTm;
//	tm* pTm;
//
//	time(&curTm);
//	pTm = localtime(&curTm);
//
//	WCHAR strFolderPath[MAX_PATH] = { 0, };
//	if (!GetCurDirectory(strFolderPath))
//		return;
//
//	WCHAR fileName[1024] = { 0, };
//	swprintf_s(fileName, L"%s\\INDEX%02d_STAGE_%02d%02d%02d_%02d%02d%02d.ini", strFolderPath, m_no,
//		((pTm->tm_year + 1900) % 2000), (pTm->tm_mon + 1), pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
//
//	FILE* fp = _wfopen(fileName, L"w+");
//	if (NULL != fp)
//	{
//		fwprintf(fp, L"\n ;Ver[%04d%02d%02d  %02d:%02d:%02d]",
//			(pTm->tm_year + 1900), (pTm->tm_mon + 1), pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
//
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"\n[Config]");
//		fwprintf(fp, L"\n MAX_CNT_X = %d", m_stageCal.Count.x);
//		fwprintf(fp, L"\n MAX_CNT_Y = %d", m_stageCal.Count.y);
//		fwprintf(fp, L"\n PITCH_X = %f", m_stageCal.PitchX);
//		fwprintf(fp, L"\n PITCH_Y = %f", m_stageCal.PitchY);
//		fwprintf(fp, L"\n ANGLE_T = %f", m_stageCal.AngleT);
//		fwprintf(fp, L"\n PITCH_DELTAX_X = %f", m_stageCal.VectorPitchX.dX);
//		fwprintf(fp, L"\n PITCH_DELTAX_Y = %f", m_stageCal.VectorPitchX.dY);
//		fwprintf(fp, L"\n PITCH_DELTAY_X = %f", m_stageCal.VectorPitchY.dX);
//		fwprintf(fp, L"\n PITCH_DELTAY_Y = %f", m_stageCal.VectorPitchY.dY);
//		fwprintf(fp, L"\n STARTPOS_X = %f", m_stageCal.StartPos.dX);
//		fwprintf(fp, L"\n STARTPOS_Y = %f", m_stageCal.StartPos.dY);
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"\n[Calibration Data] \n");
//
//		int nX, nY;
//		for (nX = 0; nX < STAGE_CAL[m_no].Count.x; nX++)
//		{
//			for (nY = 0; nY < STAGE_CAL[m_no].Count.y; nY++)
//			{
//				fwprintf(fp, L"\n X%02d_Y%02d = %3f, %3f", nX, nY, STAGE_CAL[m_no].val[nX][nY].dX, STAGE_CAL[m_no].val[nX][nY].dY);
//			}
//			fwprintf(fp, L"\n");
//		}
//
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"==============================================================================");
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"\n[Vi Result Data] \n");
//
//		for (nX = 0; nX < m_stageCal.Count.x; nX++)
//		{
//			for (nY = 0; nY < m_stageCal.Count.y; nY++)
//			{
//				fwprintf(fp, L"\n X%02d_Y%02d = %3f, %3f", nX, nY, m_viResult[nX][nY].dX, m_viResult[nX][nY].dY);
//			}
//			fwprintf(fp, L"\n");
//		}
//		fclose(fp);
//	}
//}
//
//void Stage::JigFlatCheckCycle(void)
//{
//	POINT2D posError;
//	POINT2D	posMove;
//
//	posError.dX = 0.;
//	posError.dY = 0.;
//
//	posMove.dX = 0.;
//	posMove.dY = 0.;
//
//	if (!m_pFsm->Between(C_JIG_FLAT_CHECK_START, C_JIG_FLAT_CHECK_END))
//	{
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(600000)) 
//	{
//		ER.Save((ErrName)(ER_INDEX1_JIG_FLAT_CHECK_CYCLE_TIME_OVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//	if (!MT[MtXNum].IsRdy() || !MT[MtYNum].IsRdy() || !MT[MtZNum].IsRdy())
//		return;
//
//	if (IsExist())
//	{
//		ER.Save((ErrName)(ER_INDEX_1_MEMORY_STATUS + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	int refNo = m_no + 1;
//	if (m_no % 2)
//		refNo = m_no - 1;
//
//	switch (m_pFsm->Get())
//	{
//	case C_JIG_FLAT_CHECK_START:
//	{
//		if (m_pFsm->Once()) {
//			m_tmCycleTime.Reset();
//			FlatCheckCnt = 0;
//
//			if (2 > NV.sys[FLAT_CHECK_POINT_CNT_X]) 
//			{
//				NV.sys[FLAT_CHECK_POINT_CNT_X] = 2;
//			}
//			if (2 > NV.sys[FLAT_CHECK_POINT_CNT_Y])
//			{
//				NV.sys[FLAT_CHECK_POINT_CNT_Y] = 2;
//			}
//
//			if (10 < NV.sys[FLAT_CHECK_POINT_CNT_X])
//			{
//				NV.sys[FLAT_CHECK_POINT_CNT_X] = 10;
//			}
//			if (10 < NV.sys[FLAT_CHECK_POINT_CNT_Y])
//			{
//				NV.sys[FLAT_CHECK_POINT_CNT_Y] = 10;
//			}
//
//			if (IsRefIndexBusy())
//			{
//				MSGNOTICE.Set(MSG_NOTICE,
//					"CLOSE", NULL, NULL, NULL,
//					"Another Index Is Cycle Running");
//
//				m_pFsm->Set(C_ERROR);
//				break;
//			}
//			FlatCheckMax = (int)(NV.sys[FLAT_CHECK_POINT_CNT_X] * NV.sys[FLAT_CHECK_POINT_CNT_Y]);
//			NV.ndm[(NDmList)(INDEX1_FLAT_CHECK_REQ + m_no)] = COMM_IDLE;
//
//			memset(FLAT_CAL.Flat[m_no].indexFlat, 0, sizeof(FLAT_CAL.Flat[m_no].indexFlat));
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_START", m_strName);
//		}
//
//		m_pFsm->Set(C_JIG_FLAT_CHECK_INIT);
//	}
//	break;
//
//	case C_JIG_FLAT_CHECK_INIT:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_INIT", m_strName);
//		}
//		posMove.dX = MT[MtXNum].PosTable(PLX_PLAT_START_1 + m_no);
//		posMove.dY = MT[MtYNum].PosTable(PY_PLAT_CHECK_START);
//
//		int XIndex = PLX_PLAT_START_1 + m_no;
//		int ZIndex = PLZ_LASER_1 + m_no;
//		if (MT[MtXNum].InPos(XIndex, posMove.dX) && MT[MtYNum].InPos(PY_PLAT_CHECK_START,posMove.dY) && MT[MtZNum].InPos(ZIndex))
//		{
//			// ZeroSet이 수동으로 이루어짐. 사이클에서 빠짐
//			m_pFsm->Set(C_JIG_FLAT_CHECK_MOVE);
//		}
//		else 
//		{
//			MtPosMove(MtXNum, XIndex, posMove.dX);
//			MtPosMove(MtYNum, PY_PLAT_CHECK_START, posMove.dY);
//			MtPosMove(MtZNum, ZIndex);
//		}
//	}
//	break;
//
//	case C_JIG_FLAT_CHECK_MOVE:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_MOVE", m_strName);
//		}
//
//		if (FlatCheckCnt < FlatCheckMax) {
//
//			int xCurIdx = FlatCheckCnt / (int)NV.sys[FLAT_CHECK_POINT_CNT_Y];
//			int yCurIdx = FlatCheckCnt % (int)NV.sys[FLAT_CHECK_POINT_CNT_Y];
//
//			POINT2D offset;
//			double dPitchX = NV.sys[FLAT_CHECK_JIG_SIZE_X] / (int(NV.sys[FLAT_CHECK_POINT_CNT_X]) - 1);
//			double dPitchY = NV.sys[FLAT_CHECK_JIG_SIZE_Y] / (int(NV.sys[FLAT_CHECK_POINT_CNT_Y]) - 1);
//
//			offset.dX = (dPitchX * xCurIdx);
//			offset.dY = (dPitchY * yCurIdx);
//
//			posMove.dX = MT[MtXNum].PosTable(PLX_PLAT_START_1 + (m_no % 2));
//			posMove.dY = MT[MtYNum].PosTable(PY_PLAT_CHECK_START);
//			posMove.dX += offset.dX;
//			posMove.dY += offset.dY;
//
//			int XIndex = PLX_PLAT_START_1 + (m_no % 2);
//
//			if (MT[MtXNum].InPos(XIndex, posMove.dX) && MT[MtYNum].InPos(PY_PLAT_CHECK_START, posMove.dY))
//			{
//				m_pFsm->Set(C_JIG_FLAT_CHECK_RCV_RESULT);
//			}
//			else {
//				MtPosMove(MtXNum, XIndex, posMove.dX);
//				MtPosMove(MtYNum, PY_PLAT_CHECK_START, posMove.dY);
//			}
//		}
//		else {
//			m_pFsm->Set(C_JIG_FLAT_CHECK_END);
//		}
//	}
//	break;
//
//	case C_JIG_FLAT_CHECK_RCV_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_RCV_RESULT", m_strName);
//		}
//
//		if (!MT[MtXNum].m_isRdy.TmOver(500) || !MT[MtYNum].m_isRdy.TmOver(500) )
//			return;
//
//		if (OPR.isDryRun)
//		{
//			NV.ndm[NDmList(INDEX1_FLAT_CHECK_REQ + m_no)] = COMM_IDLE;
//			FlatCheckCnt++;
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_RCV_RESULT: DRY_RUN", m_strName);
//
//			m_pFsm->Set(C_JIG_FLAT_CHECK_MOVE);
//			break;
//		}
//
//		if (COMM_IDLE == NV.ndm[NDmList(INDEX1_FLAT_CHECK_REQ + m_no)]) {
//			NV.ndm[NDmList(INDEX1_FLAT_CHECK_REQ + m_no)] = COMM_REQ;
//			m_tmRevTime.Reset();
//		}
//		else if (COMM_COMP == NV.ndm[NDmList(INDEX1_FLAT_CHECK_REQ + m_no)]) {
//			FLAT_CAL.Flat[m_no].indexFlat[FlatCheckCnt] = NV.ddm[DDmList(INDEX1_FLAT_CHECK_RESULT + m_no)];
//			NV.ndm[NDmList(INDEX1_FLAT_CHECK_REQ + m_no)] = COMM_IDLE;
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_RCV_RESULT: %.3f", m_strName, FLAT_CAL.Flat[m_no].indexFlat[FlatCheckCnt]);
//			if (!FlatCheckCnt)
//			{
//				NV.ddm[DDmList(INDEX1_FLAT_CHECK_INIT + m_no)] = NV.ddm[DDmList(INDEX1_FLAT_CHECK_RESULT + m_no)];
//			}
//			FlatCheckCnt++;
//
//			m_pFsm->Set(C_JIG_FLAT_CHECK_MOVE);
//		}
//		else if (COMM_ERR == NV.ndm[NDmList(INDEX1_FLAT_CHECK_REQ + m_no)]) {
//			ER.Save((ErrName)(ER_INDEX1_JIG_FLAT_CHECK_RCV_FAIL + m_no));
//			m_pFsm->Set(C_ERROR);
//			return;
//		}
//		else if (m_tmRevTime.TmOver(3000)) 
//		{
//			ER.Save((ErrName)(ER_INDEX1_JIG_FLAT_CHECK_RCV_TIME_OVER + m_no));
//			m_pFsm->Set(C_ERROR);
//			return;
//		}
//	}
//	break;
//
//	case C_JIG_FLAT_CHECK_END:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_END", m_strName);
//		}
//
//		double dFlatMax = FLAT_CAL.Flat[m_no].indexFlat[0];
//		double dFlatMin = FLAT_CAL.Flat[m_no].indexFlat[0];
//
//		for (int i = 0; i < FlatCheckMax; i++) {
//			if (dFlatMax < FLAT_CAL.Flat[m_no].indexFlat[i]) {
//				dFlatMax = FLAT_CAL.Flat[m_no].indexFlat[i];
//			}
//
//			if (dFlatMin > FLAT_CAL.Flat[m_no].indexFlat[i]) {
//				dFlatMin = FLAT_CAL.Flat[m_no].indexFlat[i];
//			}
//		}
//
//		LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_END : Max : %.3f, Min : %.3f, Offset : %.3f", m_strName, dFlatMax, dFlatMin, (dFlatMax - dFlatMin));
//
//		if (NV.sys[FLAT_CHECK_TOLERANCE] < fabs(dFlatMax - dFlatMin)) {
//			ER.Save((ErrName)(ER_INDEX1_JIG_FLAT_CHECK_OVER + m_no));
//			m_pFsm->Set(C_ERROR);
//			return;
//		}
//		NV_FLAT_CAL.Flush();
//		IsFlatChk() = TRUE;
//		m_pFsm->Set(C_IDLE);
//		LOG[logSEQ].Message("<%s> C_JIG_FLAT_CHECK_END : Time[%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
//	}
//	break;
//
//	default:
//		break;
//	}
//}
