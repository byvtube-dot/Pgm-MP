#include "..\..\Includes.h"

CMbbQc MBB_QC;

CMbbQc::CMbbQc()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CMbbQc::~CMbbQc()
{

}

void CMbbQc::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = static_cast<string> (TRAY_PART_DATA[nvMbbQc].labelInfo.sBoxId);
	if (0 == strcmp(TRAY_PART_DATA[nvMbbQc].labelInfo.sBoxId, ""))
		MatId = "$";

	if ("QC" == newValue)
	{
		MBB_QC.m_pSLog->XFR(newValue, MatId, "MBB_QC", "MBB_QC");
		MBB_QC.m_pSLog->RecordingOn(mtMbbQcX);
		MBB_QC.m_pSLog->RecordingOn(cylQcSealedTrayStopUd);
	}
	else if ("SEND" == newValue)
	{
		MBB_QC.m_pSLog->XFR(newValue, MatId, "MBB_QC", "MBB_FOLD");
		MBB_QC.m_pSLog->RecordingOn(cylQcSealedTrayStopUd);
	}
	else
	{
		MBB_QC.m_pSLog->XFR(newValue, MatId, "", "");
		MBB_QC.m_pSLog->RecordingOff(mtMbbQcX);
		MBB_QC.m_pSLog->RecordingOff(cylQcSealedTrayStopUd);
	}
}

void CMbbQc::Auto(void)
{
	if (!Common())
	{
		m_isRun = FALSE;
		return;
	}

	if (!m_isRun)
	{
		if (!m_pFsm->IsBusy())
			m_state = "";
		return;
	}

	m_isRun = FALSE;

	if (ExistCrossCheck() || IsHwErr() || IsHwRdy())
		return;

	if (m_pFsm->IsBusy())
		return;

	if (Exist() == enExistSealed)
	{
		if (!IsCanQcCycle())
			return;

		m_pFsm->Set(C_MBB_QC_VI_START);
		return;
	}

	if (Exist() == enExistSealedQc)
	{
		if (!IsCanSendCycle())
			return;

		m_pFsm->Set(C_MBB_QC_SEND_START);
		return;
	}
}

void CMbbQc::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
		{
			CONV[CV_QC].Stop();
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleQcVi();
	CycleSend();
}

void CMbbQc::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "MbbQc");
	m_pFsm = &FSM[fsmMbbQc];

	m_pSLog = &SLOG[slogMbbQc];
	m_pSLog->SetXFRDeviceId("MBB_QC");
	m_pSLog->SetXFRMaterialType("SEALED_MBB");

	m_pSLog->Add(mtMbbQcViY, TRUE);
	m_pSLog->Add(mtMbbShuttleY, TRUE);
	m_pSLog->Add(mtMbbQcX, TRUE);

	m_pSLog->Add(cylQcSealedHeadBtmFtVacUd, TRUE);
	m_pSLog->Add(cylQcSealedHeadBtmRrVacUd, TRUE);
	m_pSLog->Add(cylQcSealedHeadTopFtClampUd, TRUE);
	m_pSLog->Add(cylQcSealedHeadTopRrClampUd, TRUE);
	m_pSLog->Add(cylQcSealedTrayStopUd, TRUE);
	m_pSLog->Add(cylQcSealedTrayAlignFtFb, TRUE);
	m_pSLog->Add(cylQcSealedTrayAlignRrFb, TRUE);

	m_pSLog->Add(vcQcSealedHead, TRUE);

	m_pSLog->Add(VI_MBB_QC, TRUE);

	m_pSLog->Add(CV_QC, TRUE);
}

BOOL CMbbQc::Common(void)
{

	return TRUE;
}

BOOL CMbbQc::IsHwErr(void)
{
	if (!MT[mtMbbQcViY].m_state.isHome)
		return TRUE;

	if (CYL[cylQcSealedHeadBtmFtVacUd].GetErr())
		return TRUE;
	if (CYL[cylQcSealedHeadTopFtClampUd].GetErr())
		return TRUE;
	if (CYL[cylQcSealedTrayStopUd].GetErr())
		return TRUE;
	if (CYL[cylQcSealedTrayAlignFtFb].GetErr())
		return TRUE;
	if (CYL[cylQcSealedTrayAlignRrFb].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CMbbQc::IsHwRdy(void)
{
	if (!MT[mtMbbQcViY].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CMbbQc::CylInit(void)
{
	BOOL isCylInitDone = TRUE;

	if (!VC[vcQcSealedHead].IsOff())
	{
		VC[vcQcSealedHead].Off(QC_SEALED_HEAD_OFF);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylQcSealedHeadBtmFtVacUd].IsDn() || !CYL[cylQcSealedHeadBtmRrVacUd].IsDn())
	{
		CYL[cylQcSealedHeadBtmFtVacUd].Dn(QC_SEALED_HEAD_BTM_FT_VAC_DN);
		CYL[cylQcSealedHeadBtmRrVacUd].Dn(QC_SEALED_HEAD_BTM_RR_VAC_DN);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylQcSealedHeadTopFtClampUd].IsUp() || !CYL[cylQcSealedHeadTopRrClampUd].IsUp())
	{
		CYL[cylQcSealedHeadTopFtClampUd].Up(QC_SEALED_HEAD_TOP_FT_CLAMP_UP);
		CYL[cylQcSealedHeadTopRrClampUd].Up(QC_SEALED_HEAD_TOP_RR_CLAMP_UP);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylQcSealedTrayStopUd].IsUp())
	{
		CYL[cylQcSealedTrayStopUd].Up(QC_SEALED_TRAY_STOP_UP);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylQcSealedTrayAlignFtFb].IsBwd() || !CYL[cylQcSealedTrayAlignRrFb].IsBwd())
	{
		CYL[cylQcSealedTrayAlignFtFb].Bwd(QC_SEALED_TRAY_ALIGN_FT_BWD);
		CYL[cylQcSealedTrayAlignRrFb].Bwd(QC_SEALED_TRAY_ALIGN_RR_BWD);
		isCylInitDone = FALSE;
	}

	return isCylInitDone;
}

BOOL CMbbQc::IsCanQcCycle(void)
{
	if (!NV.use[useMbbQc])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"useMbbQc Skip");
		return FALSE;
	}

	if (Exist() != enExistSealed)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Qc Memory Mismatch");
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbQc::IsCanSendCycle(void)
{
	if (Exist() != enExistSealedQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Qc Memory Mismatch");
		return FALSE;
	}

	if (BTM_FOLD_CONV.Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Btm Fold Conv Exist");
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbQc::ExistCrossCheck(void)
{
	if (!m_pFsm->Between(MBB_QC.C_MBB_QC_SEND_START, MBB_QC.C_MBB_QC_SEND_END) &&
		!MBB_SHUTTLE.m_pFsm->Between(MBB_SHUTTLE.C_MBB_PACK_SEND_START, MBB_SHUTTLE.C_MBB_PACK_SEND_END))
	{
		if (!Exist() && (AOn(iQcConvExistIn) || AOn(iQcConvExistEnd)))
		{
			ER.Save(ER_MBB_QC_SENSOR_STATUS);
			return TRUE;
		}
		else if (Exist() && AOff(iQcConvExistIn) && AOff(iQcConvExistEnd))
		{
			ER.Save(ER_MBB_QC_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CMbbQc::Exist(void)
{
	return TRAY_PART_DATA[nvMbbQc].exist;
}

BOOL CMbbQc::IsExist(void)
{
	if (!TRAY_PART_DATA[nvMbbQc].exist || (AOff(iQcConvExistIn) && AOff(iQcConvExistEnd)))
	{
		ER.Save(ER_MBB_QC_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbQc::IsEmpty(void)
{
	if (TRAY_PART_DATA[nvMbbQc].exist || AOn(iQcConvExistIn) || AOn(iQcConvExistEnd))
	{
		ER.Save(ER_MBB_QC_EXIST);
		return FALSE;
	}

	return TRUE;
}

void CMbbQc::CycleQcVi(void)
{
	if (!m_pFsm->Between(C_MBB_QC_VI_START, C_MBB_QC_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_QC_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_QC_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "QC";

				m_nViCnt = -1;
				m_nViRetryCnt = 0;
				VI[VI_MBB_QC].Clear();
			}

			if (!VI[VI_MBB_QC].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (CYL[cylQcSealedHeadBtmFtVacUd].IsUp() && CYL[cylQcSealedHeadBtmRrVacUd].IsUp() &&
				CYL[cylQcSealedHeadTopFtClampUd].IsDn() && CYL[cylQcSealedHeadTopRrClampUd].IsDn())
			{
				m_pFsm->Set(C_MBB_QC_VI_MOVE);
				break;
			}

			// TODO: TEST
			//if (!CONV[CV_QC].IsFwd())
			//{
			//	CONV[CV_QC].Fwd();
			//	break;
			//}

			int nIndex = 0;
			if (Between(TRAY_PART_DATA[nvMbbQc].lotInfo.trayCnt, 3, 11))
				nIndex = TRAY_PART_DATA[nvMbbQc].lotInfo.trayCnt - 11;
			if (MT_INPOS != MtPosMove(mtMbbQcX, P_MBB_QC_X_WORK_11EA + nIndex))
				break;

			if (!CYL[cylQcSealedTrayAlignFtFb].IsFwd() || !CYL[cylQcSealedTrayAlignRrFb].IsFwd())
			{
				CYL[cylQcSealedTrayAlignFtFb].Fwd(QC_SEALED_TRAY_ALIGN_FT_FWD);
				CYL[cylQcSealedTrayAlignRrFb].Fwd(QC_SEALED_TRAY_ALIGN_RR_FWD);
				break;
			}

			CONV[CV_QC].Stop();
			m_pFsm->Set(C_MBB_QC_VI_CLAMP);
			break;
		}
		case C_MBB_QC_VI_CLAMP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_CLAMP", m_strName);

			if (m_pFsm->Delay(20000))
			{
				if (!VC[vcQcSealedHead].AOn())
					ER.Save(ER_VAC_ALARM_VC_QC_SEALED_HEAD_MBB_QC_VI_CLAMP);
				else
					ER.Save(ER_MBB_QC_VI_CLAMP_CYCLE_TIME_OVER);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CONV[CV_QC].IsStop())
			{
				CONV[CV_QC].Stop();
				break;
			}

			// TODO: 25.09.29
			//if (!CYL[cylQcSealedTrayAlignFtFb].IsBwd() || !CYL[cylQcSealedTrayAlignRrFb].IsBwd())
			//{
			//	CYL[cylQcSealedTrayAlignFtFb].Bwd(QC_SEALED_TRAY_ALIGN_FT_BWD);
			//	CYL[cylQcSealedTrayAlignRrFb].Bwd(QC_SEALED_TRAY_ALIGN_RR_BWD);
			//}

			if (!CYL[cylQcSealedHeadBtmFtVacUd].IsUp() || !CYL[cylQcSealedHeadBtmRrVacUd].IsUp())
			{
				CYL[cylQcSealedHeadBtmFtVacUd].Up(QC_SEALED_HEAD_BTM_FT_VAC_UP);
				CYL[cylQcSealedHeadBtmRrVacUd].Up(QC_SEALED_HEAD_BTM_RR_VAC_UP);
				break;
			}

			if (!VC[vcQcSealedHead].IsOn())
				VC[vcQcSealedHead].On(QC_SEALED_HEAD_ON);

			// TODO: 25.09.29
			//if (!CYL[cylQcSealedTrayAlignFtFb].IsBwd() || !CYL[cylQcSealedTrayAlignRrFb].IsBwd())
			//	break;

			if (!CYL[cylQcSealedHeadTopFtClampUd].IsDn() || !CYL[cylQcSealedHeadTopRrClampUd].IsDn())
			{
				CYL[cylQcSealedHeadTopFtClampUd].Dn(QC_SEALED_HEAD_TOP_FT_CLAMP_DN);
				CYL[cylQcSealedHeadTopRrClampUd].Dn(QC_SEALED_HEAD_TOP_RR_CLAMP_DN);
				break;
			}

			if (!CYL[cylQcSealedTrayAlignFtFb].IsBwd() || !CYL[cylQcSealedTrayAlignRrFb].IsBwd())
			{
				CYL[cylQcSealedTrayAlignFtFb].Bwd(QC_SEALED_TRAY_ALIGN_FT_BWD);
				CYL[cylQcSealedTrayAlignRrFb].Bwd(QC_SEALED_TRAY_ALIGN_RR_BWD);
			}
			MtPosMove(mtMbbShuttleY, P_MBB_SHUTTLE_Y_RECV);

			//if (!VC[vcQcSealedHead].AOn() || !VC[vcQcSealedHead].IsOn())
			if (!VC[vcQcSealedHead].IsOn())
				break;

			m_pFsm->Set(C_MBB_QC_VI_MOVE);
			break;
		}
		case C_MBB_QC_VI_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_DATA_SEND", m_strName);
				NV.ndm[reqViDataSendMbbSealing] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_MBB_QC_VI_DATA_SEND_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[reqViDataSendMbbSealing] != COMM_COMP)
				break;

			m_pFsm->Set(C_MBB_QC_VI_MOVE);
			break;
		}
		case C_MBB_QC_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtMbbShuttleY, P_MBB_SHUTTLE_Y_RECV))
				break;

			if (m_nViCnt != -1 && m_nViCnt != 0 && m_nViCnt >= NV.rcp[qcViInspCnt])
			{
				m_pFsm->Set(C_MBB_QC_VI_RESULT);
				break;
			}

			if (m_nViCnt == -1)
			{
				if (MT_INPOS != MtPosMove(mtMbbQcViY, P_MBB_QC_VI_Y_MARK_INSP))
					break;
			}
			else
			{
				double dPos = MT[mtMbbQcViY].PosTable(P_MBB_QC_VI_Y_WORK) + (m_nViCnt * NV.rcp[qcViInspYPitch]);
				if (MT_INPOS != MtPosMove(mtMbbQcViY, P_MBB_QC_VI_Y_WORK, dPos))
					break;
			}

			m_pFsm->Set(C_MBB_QC_VI_RST);
			break;
		}
		case C_MBB_QC_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_RST", m_strName);

			if (m_nViCnt == -1)
				viMode = MODE_MBB_QC_SEALED_MARK;
			else if (m_nViCnt == 0)
				viMode = MODE_MBB_QC_SEALED_TOP;
			else if (m_nViCnt == NV.rcp[qcViInspCnt] - 1)
				viMode = MODE_MBB_QC_SEALED_BTM;
			else
				viMode = MODE_MBB_QC_SEALED_MID;

			if (!VI[VI_MBB_QC].IsModeOn(viMode, 100))
			{
				VI[VI_MBB_QC].SetMode(viMode);
				break;
			}

			VI[VI_MBB_QC].Reset();
			m_pFsm->Set(C_MBB_QC_VI_TRIG);
			break;
		}
		case C_MBB_QC_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_MBB_QC_SEALED_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_MBB_QC].Start(viMode))
			{
				m_nViCntTemp = m_nViCnt;
				m_pFsm->Set(C_MBB_QC_VI_RESULT);
			}
			break;
		}
		case C_MBB_QC_VI_RESULT:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_RESULT", m_strName);
				m_nViCntTemp++;
			}

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_MBB_QC_SEALED_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_MBB_QC].IsBusy())
				break;

			if (m_nViCntTemp < NV.rcp[qcViInspCnt])
			{
				double dPos = MT[mtMbbQcViY].PosTable(P_MBB_QC_VI_Y_WORK) + (m_nViCntTemp * NV.rcp[qcViInspYPitch]);
				if (MT_INPOS != MtPosMove(mtMbbQcViY, P_MBB_QC_VI_Y_WORK, dPos))
					break;
			}

			if (!VI[VI_MBB_QC].m_isRcvComp)
				break;

			if (!VI[VI_MBB_QC].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[mbbQcRetryCnt])
				{
					m_pFsm->Set(C_MBB_QC_VI_MOVE);
					break;
				}

				ER.Save(ER_VISION_MBB_QC_SEALED_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_nViRetryCnt = 0;
			m_nViCnt++;
			if (m_nViCnt < NV.rcp[qcViInspCnt])
				m_pFsm->Set(C_MBB_QC_VI_MOVE);
			else
				m_pFsm->Set(C_MBB_QC_VI_UNCLAMP);
			break;
		}
		case C_MBB_QC_VI_UNCLAMP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_UNCLAMP", m_strName);

			if (!CylInit())
				break;

			m_pFsm->Set(C_MBB_QC_VI_END);
			break;
		}
		case C_MBB_QC_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			MtPosMove(mtMbbQcViY, P_MBB_QC_VI_Y_WORK);
			Exist() = enExistSealedQc;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbQc::CycleSend(void)
{
	if (!m_pFsm->Between(C_MBB_QC_SEND_START, C_MBB_QC_SEND_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_QC_SEND_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_QC_SEND_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_QC_SEND_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "SEND";
				NV.ndm[commMbbSealingEnd] = COMM_IDLE;
			}

			if (!IsExist() || !BTM_FOLD_CONV.IsEmpty())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (Exist() != enExistSealedQc)
			{
				ER.Save(ER_MBB_QC_SEND_QC_NOT_DID);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CylInit() || !FOLD_PK.CylInit())
				break;

			m_pFsm->Set(C_MBB_QC_SEND_MOVE);
			break;
		}
		case C_MBB_QC_SEND_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_SEND_MOVE", m_strName);

			if (!CYL[cylQcSealedTrayStopUd].IsDn())
			{
				CYL[cylQcSealedTrayStopUd].Dn(QC_SEALED_TRAY_STOP_DN);
				break;
			}

			CONV[CV_QC].Fwd();
			CONV[CV_MBB_FOLD].Fwd();

			if (CONV[CV_QC].IsFwd() && CONV[CV_MBB_FOLD].IsFwd())
				m_pFsm->Set(C_MBB_QC_SEND_CHECK);

			break;
		}
		case C_MBB_QC_SEND_CHECK:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_QC_SEND_CHECK", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbSealingEnd] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_15sec))
			{
				ER.Save(ER_MBB_QC_SEND_CHECK_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			int nDelay = (int)NV.rcp[mbbFoldConvStopArrivalDelay];
			if (OPR.isDryRun)
				nDelay = 0;
			if (AOn(iMbbFoldConvExist, nDelay))
			{
				CONV[CV_QC].Stop();
				CONV[CV_MBB_FOLD].Stop();

				if (CONV[CV_QC].IsStop() && CONV[CV_MBB_FOLD].IsStop())
					m_pFsm->Set(C_MBB_QC_SEND_END);
			}

			break;
		}
		case C_MBB_QC_SEND_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_QC_SEND_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			memcpy(&TRAY_PART_DATA[nvMbbFold], &TRAY_PART_DATA[nvMbbQc], sizeof(IPC_MMI::TrayPartData));
			ZeroMemory(&TRAY_PART_DATA[nvMbbQc], sizeof(IPC_MMI::TrayPartData));
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}