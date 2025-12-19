#include "..\..\Includes.h"

//void Stage::GlassCalCycle(void)
//{
//	if (!m_pFsm->Between(C_GLASS_CAL_START, C_GLASS_CAL_END))
//		return;
//
//	if (IsRefIndexBusy())
//	{
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"GlassCalCycle command is not correct!!");
//
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (m_pFsm->TimeLimit(600000))
//	{
//		ER.Save((ErrName)(ER_INDEX1_GLASS_CAL_TIMEOVER + m_no));
//		m_pFsm->Set(C_ERROR);
//		return;
//	}
//
//	if (!MT[MtXNum].IsRdy() || !MT[MtYNum].IsRdy() || !MT[MtZNum].IsRdy()) 
//		return;
//
//	switch (m_pFsm->Get())
//	{
//	case C_GLASS_CAL_START:
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
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_START", m_strName);
//
//			m_cycleTimeCnt = 0;			
//			m_tmCycleTime.Reset();
//			m_inspMode = MODE_PRS_GLASSCAL;
//			VI[ViNum].Clear();
//			VI[ViNum].SetMode(m_inspMode);
//
//			viCnt = 0;
//			ZeroMemory(m_viResult, sizeof(m_viResult));
//			ZeroMemory(&m_glassCal, sizeof(m_glassCal));
//			m_glassCal.Count.x = (int)NV.sys[GLASS_CAL_CNT_X];
//			m_glassCal.Count.y = (int)NV.sys[GLASS_CAL_CNT_Y];
//			m_glassCal.VectorPitchX.dX = m_glassCal.PitchX = NV.sys[GLASS_CAL_PITCH_X];
//			m_glassCal.VectorPitchY.dY = m_glassCal.PitchY = NV.sys[GLASS_CAL_PITCH_Y];
//			m_posCalInsp.dX = MT[MtXNum].PosTable(PLX_GLASS_CAL_START_1 + (m_no % 2));
//			m_posCalInsp.dY = MT[MtYNum].PosTable(PY_GLASS_CAL_START);
//		}
//
//		m_pFsm->Set(C_GLASS_CAL_ALIGN_MOVE);
//	}
//	break;
//	case C_GLASS_CAL_ALIGN_MOVE:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_GLASS_CAL_ALIGN_MOVE: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_GLASS_CAL_ALIGN_MOVE", m_strName);
//			}
//			VI[ViNum].Reset();
//		}
//
//		POINT2D posAlign = m_posCalInsp;
//
//		if (MT[MtXNum].InCmd(PLX_TEACHING_SPEED, posAlign.dX) &&
//			MT[MtYNum].InCmd(PY_TEACHING_SPEED, posAlign.dY) &&
//			MT[MtZNum].InCmd(PLZ_VI_GLASS_CAL_1 + (m_no % 2)))
//		{
//			LOG[logLAS].Message("<%s>,[C_GLASS_CAL_ALIGN_MOVE],m_posCalInsp,%.3f,%.3f", m_strName, m_posCalInsp.dX, m_posCalInsp.dY);
//			//LOG[logLAS].Message("<%s>,[C_GLASS_CAL_ALIGN_MOVE],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//			//LOG[logLAS].Message("<%s>,[C_GLASS_CAL_ALIGN_MOVE],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//			m_pFsm->Set(C_GLASS_CAL_ALIGN_INSP);
//		}
//		else
//		{
//			MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//			MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//			MtPosMove(MtZNum, PLZ_VI_GLASS_CAL_1 + (m_no % 2));
//		}
//	}
//	break;
//	case C_GLASS_CAL_ALIGN_INSP:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_ALIGN_INSP", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_GLASS_CAL_ALIGN_RESULT);
//	}
//	break;
//
//	case C_GLASS_CAL_ALIGN_RESULT:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_ALIGN_RESULT", m_strName);
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
//		if (0.002 < fabs(VI[ViNum].m_prsData.data[0].dX) ||
//			0.002 < fabs(VI[ViNum].m_prsData.data[0].dY))
//		{
//			m_posCalInsp.dX += VI[ViNum].m_prsData.data[0].dX;
//			m_posCalInsp.dY += VI[ViNum].m_prsData.data[0].dY;
//			m_pFsm->Set(C_GLASS_CAL_ALIGN_MOVE);
//			break;
//		}
//
//		if (0 == viCnt)
//		{
//			m_glassCal.StartPos = m_posCalInsp;
//			m_posCalInsp.dY += (m_glassCal.PitchY * (m_glassCal.Count.y - 1));
//			viCnt++;
//			m_pFsm->Set(C_GLASS_CAL_ALIGN_MOVE);
//		}
//		else
//		{
//			double adj = m_posCalInsp.dX - m_glassCal.StartPos.dX;
//			double opp = m_posCalInsp.dY - m_glassCal.StartPos.dY;
//			m_glassCal.AngleT = GetTheta(adj, opp) - 90.;
//			m_glassCal.VectorPitchX = Rotate(m_glassCal.VectorPitchX, m_glassCal.AngleT);
//			m_glassCal.VectorPitchY = Rotate(m_glassCal.VectorPitchY, m_glassCal.AngleT);
//			viCnt = 0;
//			m_pFsm->Set(C_GLASS_CAL_MOVE_VISION);
//		}
//	}
//	break;
//	case C_GLASS_CAL_MOVE_VISION:
//	{
//		if (m_pFsm->Once())
//		{
//			if (m_pFsm->IsCanceled())
//			{
//				LOG[logSEQ].Message("<%s> C_GLASS_CAL_MOVE_VISION: IsCanceled", m_strName);
//				m_pFsm->Set(C_IDLE);
//				break;
//			}
//			else
//			{
//				LOG[logSEQ].Message("<%s> C_GLASS_CAL_MOVE_VISION", m_strName);
//			}
//			VI[ViNum].Reset();
//		}
//
//		int maxCnt = m_glassCal.Count.x * m_glassCal.Count.y;
//		if (maxCnt > viCnt)
//		{
//			int xIdx = viCnt / m_glassCal.Count.y;
//			int yIdx = viCnt % m_glassCal.Count.y;
//			POINT2D offset = (m_glassCal.VectorPitchX * xIdx) + (m_glassCal.VectorPitchY * yIdx);
//			m_posCalInsp = m_glassCal.StartPos + offset;
//			POINT2D posError = m_glassCal.val[xIdx][yIdx] = GetGlassCalErr(m_posCalInsp);
//			POINT2D posAlign = m_posCalInsp + posError;
//
//			if (MT[MtXNum].InCmd(PLX_TEACHING_SPEED, posAlign.dX) && 
//				MT[MtYNum].InCmd(PY_TEACHING_SPEED, posAlign.dY) &&
//				MT[MtZNum].InCmd(PLZ_VI_GLASS_CAL_1 + (m_no % 2)))
//			{
//				LOG[logLAS].Message("<%s>,[C_GLASS_CAL_MOVE_VISION],m_posCalInsp,%.3f,%.3f", m_strName, m_posCalInsp.dX, m_posCalInsp.dY);
//				LOG[logLAS].Message("<%s>,[C_GLASS_CAL_MOVE_VISION],posError,%.3f,%.3f", m_strName, posError.dX, posError.dY);
//				LOG[logLAS].Message("<%s>,[C_GLASS_CAL_MOVE_VISION],posAlign,%.3f,%.3f", m_strName, posAlign.dX, posAlign.dY);
//				m_pFsm->Set(C_GLASS_CAL_VISION_INSP);
//			}
//			else
//			{
//				MtPosMove(MtXNum, PLX_TEACHING_SPEED, posAlign.dX);
//				MtPosMove(MtYNum, PY_TEACHING_SPEED, posAlign.dY);
//				MtPosMove(MtZNum, PLZ_VI_GLASS_CAL_1 + (m_no % 2));
//			}
//		}
//		else
//		{
//			m_pFsm->Set(C_GLASS_CAL_COLLECT_DATA);
//		}
//	}
//	break;
//	case C_GLASS_CAL_VISION_INSP:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_VISION_INSP", m_strName);
//		}
//
//		DWORD dwDelay_ms = (DWORD)NV.rcp[PRS_INSP_DELAYMS];
//
//		if (!MT[MtXNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtYNum].m_isRdy.TmOver(dwDelay_ms) || !MT[MtZNum].m_isRdy.TmOver(dwDelay_ms))
//			return;
//
//		if (OPR.isNoDevice || VI[ViNum].Start(m_inspMode))
//			m_pFsm->Set(C_GLASS_CAL_VISION_RESULT);
//	}
//	break;
//
//	case C_GLASS_CAL_VISION_RESULT:
//	{
//		if (m_pFsm->Once()) 
//		{
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_VISION_RESULT", m_strName);
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
//		int maxCnt = m_glassCal.Count.x * m_glassCal.Count.y;
//		if (maxCnt > (viCnt + 1))
//		{
//			int xIdx = (viCnt + 1) / m_glassCal.Count.y;
//			int yIdx = (viCnt + 1) % m_glassCal.Count.y;
//			POINT2D offset = (m_glassCal.VectorPitchX * xIdx) + (m_glassCal.VectorPitchY * yIdx);
//			m_posCalInsp = m_glassCal.StartPos + offset;
//			POINT2D posError = m_glassCal.val[xIdx][yIdx] = GetGlassCalErr(m_posCalInsp);
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
//		m_pFsm->Set(C_GLASS_CAL_COLLECT_DATA);
//	}
//	break;
//
//	case C_GLASS_CAL_COLLECT_DATA:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_COLLECT_DATA", m_strName);
//		}
//
//		int maxCnt = m_glassCal.Count.x * m_glassCal.Count.y;
//
//		int xIdx = viCnt / m_glassCal.Count.y;
//		int yIdx = viCnt % m_glassCal.Count.y;
//
//		m_viResult[xIdx][yIdx].dX = VI[ViNum].m_prsData.data[0].dX;
//		m_viResult[xIdx][yIdx].dY = VI[ViNum].m_prsData.data[0].dY;
//
//		viCnt++;
//
//		if (maxCnt <= viCnt)
//		{
//			for (int xCnt = 0; xCnt < m_glassCal.Count.x; xCnt++)
//			{
//				for (int yCnt = 0; yCnt < m_glassCal.Count.y; yCnt++)
//				{
//					m_glassCal.val[xCnt][yCnt].dX += m_viResult[xCnt][yCnt].dX;
//					m_glassCal.val[xCnt][yCnt].dY += m_viResult[xCnt][yCnt].dY;
//				}
//			}
//
//			m_pFsm->Set(C_GLASS_CAL_END);
//		}
//		else
//		{
//			m_pFsm->Set(C_GLASS_CAL_MOVE_VISION);
//		}
//	}
//	break;
//	case C_GLASS_CAL_END:
//	{
//		if (m_pFsm->Once())
//		{
//			LOG[logSEQ].Message("<%s> C_GLASS_CAL_END", m_strName);
//		}
//
//		memcpy(&GLASS_CAL[m_no], &m_glassCal, sizeof(GLASS_CAL[m_no]));
//		GLASS_CAL[m_no].isSuccess = (TRUE);
//		MtPosMove(MtXNum, PLX_WAIT);
//		MtPosMove(MtYNum, PY_LOADING);
//
//		MSGNOTICE.Set(MSG_NOTICE,
//			"CLOSE", NULL, NULL, NULL,
//			"<%s> Glass Calibaration Finish..!", m_strName);
//
//		NV_GLASS_CAL.Flush();
//		CreateGlassCalFile();
//		m_pFsm->Set(C_IDLE);
//	}
//	break;
//	}
//}
//
////POINT2D Stage::GetGlassPosAddCompenErr(POINT2D pos)
////{
////	POINT2D compenErr = GetGlassCalErr(pos);
////	POINT2D posVision2 = pos + compenErr;
////	return (posVision2);
////}
//
//POINT2D Stage::GetGlassCalPosXY(int idxX, int idxY)
//{
//	POINT2D pos;
//	kasiolib::GetCalibrationPosXY(&GLASS_CAL[m_no], idxX, idxY, &pos);
//	return pos;
//}
//
//POINT2D Stage::GetGlassCalErr(POINT2D pos)
//{
//	POINT2D calErr;
//	if (!kasiolib::GetCalibrationErr(&GLASS_CAL[m_no], &pos, &calErr))
//	{
//		calErr.dX = calErr.dY = 0.;
//		// Need Message.
//	}
//	return (calErr);
//}
//
//void Stage::CreateGlassCalFile(void)
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
//	swprintf_s(fileName, L"%s\\INDEX%02d_GLASS_%02d%02d%02d_%02d%02d%02d.ini", strFolderPath, m_no,
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
//		fwprintf(fp, L"\n MAX_CNT_X = %d", m_glassCal.Count.x);
//		fwprintf(fp, L"\n MAX_CNT_Y = %d", m_glassCal.Count.y);
//		fwprintf(fp, L"\n PITCH_X = %f", m_glassCal.PitchX);
//		fwprintf(fp, L"\n PITCH_Y = %f", m_glassCal.PitchY);
//		fwprintf(fp, L"\n ANGLE_T = %f", m_glassCal.AngleT);
//		fwprintf(fp, L"\n PITCH_DELTAX_X = %f", m_glassCal.VectorPitchX.dX);
//		fwprintf(fp, L"\n PITCH_DELTAX_Y = %f", m_glassCal.VectorPitchX.dY);
//		fwprintf(fp, L"\n PITCH_DELTAY_X = %f", m_glassCal.VectorPitchY.dX);
//		fwprintf(fp, L"\n PITCH_DELTAY_Y = %f", m_glassCal.VectorPitchY.dY);
//		fwprintf(fp, L"\n STARTPOS_X = %f", m_glassCal.StartPos.dX);
//		fwprintf(fp, L"\n STARTPOS_Y = %f", m_glassCal.StartPos.dY);
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"\n[Calibration Data] \n");
//
//		int nX, nY;
//		for (nX = 0; nX < GLASS_CAL[m_no].Count.x; nX++)
//		{
//			for (nY = 0; nY < GLASS_CAL[m_no].Count.y; nY++)
//			{
//				fwprintf(fp, L"\n X%02d_Y%02d = %3f, %3f", nX, nY, GLASS_CAL[m_no].val[nX][nY].dX, GLASS_CAL[m_no].val[nX][nY].dY);
//			}
//			fwprintf(fp, L"\n");
//		}
//
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"==============================================================================");
//		fwprintf(fp, L"\n\n\n");
//		fwprintf(fp, L"\n[Vi Result Data] \n");
//
//		for (nX = 0; nX < m_glassCal.Count.x; nX++)
//		{
//			for (nY = 0; nY < m_glassCal.Count.y; nY++)
//			{
//				fwprintf(fp, L"\n X%02d_Y%02d = %3f, %3f", nX, nY, m_viResult[nX][nY].dX, m_viResult[nX][nY].dY);
//			}
//			fwprintf(fp, L"\n");
//		}
//		fclose(fp);
//	}
//}