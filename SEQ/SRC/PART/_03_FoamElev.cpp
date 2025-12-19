#include "..\..\Includes.h"

CFoamElev FOAM_ELEV[enFoamElevMax];

CFoamElev::CFoamElev()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);

}
CFoamElev::~CFoamElev()
{

}

void CFoamElev::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	int m_no = *reinterpret_cast<int*>(lpParam);
	if (oldValue == newValue)
		return;
	
	string MatId = "$";

	if ("LEVEL_CHECK" == newValue)
	{
		FOAM_ELEV[m_no].m_pSLog->XFR(newValue, MatId, "FOAM_ELEV", "FOAM_ELEV");

		if (m_no == enFoamRecvElev)
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(mtFoamRecvElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamRecvElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamRecvElevRrAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamRecvElevLtStopFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamRecvElevRtStopFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(CV_FOAM_STACK);
		}
		else if (m_no == enFoamPartialElev)
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(mtFoamPartialElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamPartialElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamPartialElevRrAlignFb);
		}
		else
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(mtFoamSendElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(mtFoamFlipT);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamSendElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamSendElevRtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(vcFoamFlip);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(vcFoamFlip2);
		}
	}
	else if ("FOAM_FLIP" == newValue)
	{
		FOAM_ELEV[m_no].m_pSLog->XFR(newValue, MatId, "FOAM_ELEV", "FLIP");

		if (m_no == enFoamSendElev)
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(mtFoamSendElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(mtFoamFlipT);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamSendElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(cylFoamSendElevRtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(vcFoamFlip);
			FOAM_ELEV[m_no].m_pSLog->RecordingOn(vcFoamFlip2);
		}
	}
	else
	{
		FOAM_ELEV[m_no].m_pSLog->XFR(newValue, MatId, "", "");

		if (m_no == enFoamRecvElev)
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(mtFoamRecvElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamRecvElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamRecvElevRrAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamRecvElevLtStopFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamRecvElevRtStopFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(CV_FOAM_STACK);
		}
		else if (m_no == enFoamPartialElev)
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(mtFoamPartialElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamPartialElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamPartialElevRrAlignFb);
		}
		else
		{
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(mtFoamSendElevZ);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(mtFoamFlipT);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamSendElevLtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(cylFoamSendElevRtAlignFb);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(vcFoamFlip);
			FOAM_ELEV[m_no].m_pSLog->RecordingOff(vcFoamFlip2);
		}
	}
}

void CFoamElev::Auto(void)
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

	if (!FOAM_PART_DATA[m_nvFoamStack].flag.levelDid)
	{
		if (enFoamSendElev == m_no)
		{
			if (!FOAM_PART_DATA[m_nvFoamStack].exist)
				return;

			if (FOAM_STACK_CONV.m_pFsm->IsBusy())
			{
				int nIdx = FOAM_STACK_CONV.m_pFsm->GetMsg();
				if (nIdx == FOAM_STACK_CONV.msgMzMoveLift1To4 || nIdx == FOAM_STACK_CONV.msgMzMoveLift2To4 || nIdx == FOAM_STACK_CONV.msgMzMoveLift3To4)
					return;
			}
		}
		else if (enFoamRecvElev == m_no)
		{
			if (FOAM_STACK_CONV.m_pFsm->IsBusy())
			{
				int nIdx = FOAM_STACK_CONV.m_pFsm->GetMsg();
				if (nIdx == FOAM_STACK_CONV.msgMzMoveLift1To2 || nIdx == FOAM_STACK_CONV.msgMzMoveLift1To3 || nIdx == FOAM_STACK_CONV.msgMzMoveLift1To4)
					return;
			}
		}
		m_pFsm->Set(C_LEVEL_CHECK_START);
		return;
	}
	else
	{
		if (enFoamSendElev == m_no && NV.use[useFoamFlip])
		{
			if (!IsCanCycleFlip())
				return;

			m_pFsm->Set(C_FOAM_FLIP_START);
			return;
		}
	}
}

void CFoamElev::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
		{
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleLevelCheck();
	if (enFoamSendElev == m_no)
		CycleFlip();
}

void CFoamElev::Init(int nIdx)
{
	m_no = nIdx;
	
	if (m_no == enFoamRecvElev)
	{
		strcpy(m_strName, "FoamRecvElev");
		m_pFsm = &FSM[fsmFoamRecvElev];
		m_nvFoamStack = nvFoamRecvElev;
		m_nFoamStackMaxCnt = 14;		// Normal Foam : 12

		m_pSLog = &SLOG[slogFoamRecvElev];
		m_pSLog->SetXFRDeviceId("FOAM_RECV_ELEV");
		m_pSLog->SetXFRMaterialType("FOAM");
		m_pSLog->Add(mtFoamRecvElevZ);
		m_pSLog->Add(cylFoamRecvElevLtAlignFb);
		m_pSLog->Add(cylFoamRecvElevRrAlignFb);
		m_pSLog->Add(cylFoamRecvElevLtStopFb);
		m_pSLog->Add(cylFoamRecvElevRtStopFb);
		m_pSLog->Add(CV_FOAM_STACK);

		MtZNum = mtFoamRecvElevZ;
		iElevBtm = iFoamRecvElevExistBtm;
		iElevTop = iFoamRecvElevExistTop;
	}
	else if (m_no == enFoamPartialElev)
	{
		strcpy(m_strName, "FoamPartialElev");
		m_pFsm = &FSM[fsmFoamPartialElev];
		m_nvFoamStack = nvFoamPartialElev;
		m_nFoamStackMaxCnt = 7;			// Partial Foam : 8

		m_pSLog = &SLOG[slogFoamPartialElev];
		m_pSLog->SetXFRDeviceId("FOAM_PARTIAL_ELEV");
		m_pSLog->SetXFRMaterialType("FOAM");
		m_pSLog->Add(mtFoamPartialElevZ);
		m_pSLog->Add(cylFoamPartialElevLtAlignFb);
		m_pSLog->Add(cylFoamPartialElevRrAlignFb);

		MtZNum = mtFoamPartialElevZ;
		iElevBtm = iFoamPartialRecvElevExistBtm;
		iElevTop = iFoamPartialRecvElevExistTop;
	}
	else // if (m_no == enFoamSendElev)
	{
		strcpy(m_strName, "FoamSendElev");
		m_pFsm = &FSM[fsmFoamSendElev];
		m_nvFoamStack = nvFoamSendElev;
		
		m_pSLog = &SLOG[slogFoamSendElev];
		m_pSLog->SetXFRDeviceId("FOAM_SEND_ELEV");
		m_pSLog->SetXFRMaterialType("FOAM");
		m_pSLog->Add(mtFoamSendElevZ);
		m_pSLog->Add(mtFoamFlipT);
		m_pSLog->Add(cylFoamSendElevLtAlignFb);
		m_pSLog->Add(cylFoamSendElevRtAlignFb);

		m_pSLog->Add(vcFoamFlip);
		m_pSLog->Add(vcFoamFlip2);

		MtZNum = mtFoamSendElevZ;
		iElevBtm = iFoamSendElevExistBtm;
		iElevTop = iFoamSendElevExistTop;
	}
}

BOOL CFoamElev::Common(void)
{
	if (m_nvFoamStack != nvFoamSendElev)
	{
		if (FOAM_PART_DATA[m_nvFoamStack].cnt >= m_nFoamStackMaxCnt)
			FOAM_PART_DATA[m_nvFoamStack].flag.recvFullDid = true;
	}

	if (m_nvFoamStack == nvFoamPartialElev)
	{
		if (!FOAM_PART_DATA[m_nvFoamStack].exist)
			FOAM_PART_DATA[m_nvFoamStack].cnt = 0;

		if (FOAM_PART_DATA[m_nvFoamStack].cnt >= m_nFoamStackMaxCnt)
			FOAM_PART_DATA[m_nvFoamStack].flag.recvFullDid = true;
		else
			FOAM_PART_DATA[m_nvFoamStack].flag.recvFullDid = false;

	}

	return TRUE;
}

BOOL CFoamElev::IsHwErr(void)
{
	if (m_no == enFoamRecvElev)
	{
		if (!MT[mtFoamRecvElevZ].m_state.isHome)
			return TRUE;

		if (CYL[cylFoamRecvElevLtAlignFb].GetErr())
			return TRUE;
		if (CYL[cylFoamRecvElevRrAlignFb].GetErr())
			return TRUE;
		if (CYL[cylFoamRecvElevLtStopFb].GetErr())
			return TRUE;
		if (CYL[cylFoamRecvElevRtStopFb].GetErr())
			return TRUE;
	}
	else if (m_no == enFoamPartialElev)
	{
		if (!MT[mtFoamPartialElevZ].m_state.isHome)
			return TRUE;

		if (CYL[cylFoamPartialElevLtAlignFb].GetErr())
			return TRUE;
		if (CYL[cylFoamPartialElevRrAlignFb].GetErr())
			return TRUE;
	}
	else // if (m_no == foamSendElev)
	{
		if (!MT[mtFoamSendElevZ].m_state.isHome)
			return TRUE;
		if (!MT[mtFoamFlipT].m_state.isHome)
			return TRUE;

		if (CYL[cylFoamSendElevLtAlignFb].GetErr())
			return TRUE;
		if (CYL[cylFoamSendElevRtAlignFb].GetErr())
			return TRUE;
	}

	return FALSE;
}

BOOL CFoamElev::IsHwRdy(void)
{
	if (m_no == enFoamSendElev)
	{
		if (!MT[mtFoamFlipT].IsRdy())
			return TRUE;
	}

	return FALSE;
}

BOOL CFoamElev::CylInit(void)
{
	if (m_no == enFoamRecvElev)
	{
		if (!CYL[cylFoamRecvElevLtAlignFb].IsBwd())
			CYL[cylFoamRecvElevLtAlignFb].Bwd(FOAM_RECV_ELEV_LT_ALIGN_BWD);
		if (!CYL[cylFoamRecvElevRrAlignFb].IsBwd())
			CYL[cylFoamRecvElevRrAlignFb].Bwd(FOAM_RECV_ELEV_RR_ALIGN_BWD);

		if (!CYL[cylFoamRecvElevLtStopFb].IsFwd())
			CYL[cylFoamRecvElevLtStopFb].Fwd(FOAM_RECV_ELEV_LT_STOP_FWD);
		if (!CYL[cylFoamRecvElevRtStopFb].IsFwd())
			CYL[cylFoamRecvElevRtStopFb].Fwd(FOAM_RECV_ELEV_RT_STOP_FWD);

		if (!CYL[cylFoamRecvElevLtAlignFb].IsBwd() ||
			!CYL[cylFoamRecvElevRrAlignFb].IsBwd() ||
			!CYL[cylFoamRecvElevLtStopFb].IsFwd() ||
			!CYL[cylFoamRecvElevRtStopFb].IsFwd())
			return FALSE;
	}
	else if(m_no == enFoamPartialElev)
	{
		if (!CYL[cylFoamPartialElevLtAlignFb].IsBwd())
			CYL[cylFoamPartialElevLtAlignFb].Bwd(FOAM_PARTIAL_ELEV_LT_ALIGN_BWD);
		if (!CYL[cylFoamPartialElevRrAlignFb].IsBwd())
			CYL[cylFoamPartialElevRrAlignFb].Bwd(FOAM_PARTIAL_ELEV_FT_ALIGN_BWD);

		if (!CYL[cylFoamPartialElevLtAlignFb].IsBwd() ||
			!CYL[cylFoamPartialElevRrAlignFb].IsBwd())
			return FALSE;
	}
	else // if (m_no == enFoamSendElev)
	{
		if (!CYL[cylFoamFtStackLtStopFb].IsFwd())
			CYL[cylFoamFtStackLtStopFb].Fwd(FOAM_FT_STACK_LT_STOP_FWD);
		if (!CYL[cylFoamFtStackRtStopFb].IsFwd())
			CYL[cylFoamFtStackRtStopFb].Fwd(FOAM_FT_STACK_RT_STOP_FWD);

		if (!CYL[cylFoamFtStackLtStopFb].IsFwd() ||
			!CYL[cylFoamFtStackRtStopFb].IsFwd())
			return FALSE;

		if (!CYL[cylFoamSendElevLtAlignFb].IsBwd())
			CYL[cylFoamSendElevLtAlignFb].Bwd(FOAM_SEND_ELEV_LT_ALIGN_BWD);
		if (!CYL[cylFoamSendElevRtAlignFb].IsBwd())
			CYL[cylFoamSendElevRtAlignFb].Bwd(FOAM_SEND_ELEV_RT_ALIGN_BWD);

		if (!CYL[cylFoamSendElevLtAlignFb].IsBwd() ||
			!CYL[cylFoamSendElevRtAlignFb].IsBwd())
			return FALSE;
	}

	return TRUE;
}

BOOL CFoamElev::SetCylAlign(void)
{
	if (m_no == enFoamRecvElev)
	{
		if (!CYL[cylFoamRecvElevLtAlignFb].IsFwd())
			CYL[cylFoamRecvElevLtAlignFb].Fwd(FOAM_RECV_ELEV_LT_ALIGN_FWD);
		if (!CYL[cylFoamRecvElevRrAlignFb].IsFwd())
			CYL[cylFoamRecvElevRrAlignFb].Fwd(FOAM_RECV_ELEV_RR_ALIGN_FWD);

		if (!CYL[cylFoamRecvElevLtAlignFb].IsFwd() ||
			!CYL[cylFoamRecvElevRrAlignFb].IsFwd())
			return FALSE;
	}
	else if (m_no == enFoamPartialElev)
	{
		if (!CYL[cylFoamPartialElevLtAlignFb].IsFwd())
			CYL[cylFoamPartialElevLtAlignFb].Fwd(FOAM_PARTIAL_ELEV_LT_ALIGN_FWD);
		if (!CYL[cylFoamPartialElevRrAlignFb].IsFwd())
			CYL[cylFoamPartialElevRrAlignFb].Fwd(FOAM_PARTIAL_ELEV_FT_ALIGN_FWD);

		if (!CYL[cylFoamPartialElevLtAlignFb].IsFwd() ||
			!CYL[cylFoamPartialElevRrAlignFb].IsFwd())
			return FALSE;
	}
	else // if (m_no == enFoamSendElev)
	{
		if (!CYL[cylFoamSendElevLtAlignFb].IsFwd())
			CYL[cylFoamSendElevLtAlignFb].Fwd(FOAM_SEND_ELEV_LT_ALIGN_FWD);
		if (!CYL[cylFoamSendElevRtAlignFb].IsFwd())
			CYL[cylFoamSendElevRtAlignFb].Fwd(FOAM_SEND_ELEV_RT_ALIGN_FWD);

		if (!CYL[cylFoamSendElevLtAlignFb].IsFwd() ||
			!CYL[cylFoamSendElevRtAlignFb].IsFwd())
			return FALSE;
	}

	return TRUE;
}

BOOL CFoamElev::IsCanCycleLevelCheck(void)
{
	if (FOAM_PART_DATA[m_nvFoamStack].flag.levelDid)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Level check flag task completed");
		return FALSE;
	}

	if (enFoamSendElev == m_no)
	{
		if (!FOAM_PART_DATA[m_nvFoamStack].exist)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Empty");
			return FALSE;
		}

		if (FOAM_STACK_CONV.m_pFsm->IsBusy())
			return FALSE;
		// Log Standard..
		//if (FOAM_STACK_CONV.m_pFsm->IsBusy())
		//{
		//	int nIdx = FOAM_STACK_CONV.m_pFsm->GetMsg();
		//	if (nIdx == FOAM_STACK_CONV.msgMzMoveLift1To4 || nIdx == FOAM_STACK_CONV.msgMzMoveLift2To4 || nIdx == FOAM_STACK_CONV.msgMzMoveLift3To4)
		//	{
		//		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
		//			"Foam Stack Cycle is busy");
		//		return FALSE;
		//	}
		//}
	}
	else if (enFoamRecvElev == m_no)
	{
		if (FOAM_STACK_CONV.m_pFsm->IsBusy())
			return FALSE;
		// Log Standard..
		//if (FOAM_STACK_CONV.m_pFsm->IsBusy())
		//{
		//	int nIdx = FOAM_STACK_CONV.m_pFsm->GetMsg();
		//	if (nIdx == FOAM_STACK_CONV.msgMzMoveLift1To2 || nIdx == FOAM_STACK_CONV.msgMzMoveLift1To3 || nIdx == FOAM_STACK_CONV.msgMzMoveLift1To4)
		//	{
		//		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
		//			"Foam Stack Cycle is busy");
		//		return FALSE;
		//	}
		//}
	}

	return TRUE;
}

BOOL CFoamElev::IsCanCycleFlip(void)
{
	if (!NV.ndm[ltCobotFlipCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot is Not Safety.");
		return FALSE;
	}

	if (!Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Send Elev: Foam is empty.");
		return FALSE;
	}

	if (FOAM_PART_DATA[nvFoamFlip].exist)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Flip Memory Exist");
		return FALSE;
	}

	if (!FOAM_PART_DATA[m_nvFoamStack].flag.levelDid)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Level check task not completed");
		return FALSE;
	}

	if (AOn(iFoamFlipExistLt) || AOn(iFoamFlipExistRt))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"FlipExist Sensor Detected");
		return FALSE;
	}

	if (FOAM_STACK_CONV.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Stack Conveyor Cycle is Running");
		return FALSE;
	}

	return TRUE;
}
BOOL CFoamElev::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy() && m_no == enFoamSendElev &&
		!LT_COBOT.m_pFsm->Between(LT_COBOT.C_BTM_FOAM_PICK_START, LT_COBOT.C_BTM_FOAM_PICK_END))
	{
		// TODO: RECHECK
		//if (FOAM_PART_DATA[nvFoamFlip].exist && (AOff(iFoamFlipExistLt) || AOff(iFoamFlipExistRt)))
		//{
		//	ER.Save(ER_FOAM_FLIP_TABLE_SENSOR_STATUS);
		//	return TRUE;
		//}
		//if (!FOAM_PART_DATA[nvFoamFlip].exist && (AOn(iFoamFlipExistLt) || AOn(iFoamFlipExistRt)))
		//{
		//	ER.Save(ER_FOAM_FLIP_TABLE_MEMORY_STATUS);
		//	return TRUE;
		//}
		if (FOAM_PART_DATA[nvFoamFlip].exist && (VC[vcFoamFlip].AOff() || VC[vcFoamFlip].AOff()))
		{
			ER.Save(ER_FOAM_FLIP_TABLE_SENSOR_STATUS);
			return TRUE;
		}
		if (!FOAM_PART_DATA[nvFoamFlip].exist && (VC[vcFoamFlip].AOn() || VC[vcFoamFlip].AOn()))
		{
			ER.Save(ER_FOAM_FLIP_TABLE_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CFoamElev::Exist(void)
{
	return FOAM_PART_DATA[m_nvFoamStack].exist;
}

void CFoamElev::CycleLevelCheck(void)
{
	if (!m_pFsm->Between(C_LEVEL_CHECK_START, C_LEVEL_CHECK_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save((ErrName)(ER_RECV_ELEV_LEVEL_CHECK_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LEVEL_CHECK_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "LEVEL_CHECK";
				m_nLevelCheckCnt = 0;
				m_dPos = 0.0;
			}

			if (!CylInit())
				break;

			m_pFsm->Set(C_LEVEL_CHECK_ALIGN);
			break;
		}
		case C_LEVEL_CHECK_ALIGN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_ALIGN", m_strName);

			if (AOn(iElevTop))
				m_pFsm->Set(C_LEVEL_CHECK_Z_DN);
			else
				m_pFsm->Set(C_LEVEL_CHECK_Z_UP);

			break;
		}
		case C_LEVEL_CHECK_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_DN", m_strName);

			if (!CylInit())
				break;

			MtPosMove(MtZNum, P_ELEV_Z_BTM);

			if (MT[MtZNum].m_state.isDriving || MT_INPOS == MtPosMove(MtZNum, P_ELEV_Z_BTM))
				m_pFsm->Set(C_LEVEL_CHECK_Z_DN_CHECK);
	
			break;
		}
		case C_LEVEL_CHECK_Z_DN_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_DN_CHECK", m_strName);

			rcpLevelDnDelay = (RcpList)(sendElevLevelDnDelay + m_no);
			if (AOff(iElevTop, (DWORD)NV.rcp[rcpLevelDnDelay]))
			{
				if (!MT[MtZNum].IsRdy())
				{
					MT[MtZNum].Stop();
					break;
				}
				
				if (m_no == enFoamRecvElev && 0 < m_nLevelCheckCnt)
				{
					m_pFsm->Set(C_LEVEL_CHECK_INIT);
					break;
				}

				m_pFsm->Set(C_LEVEL_CHECK_Z_UP);
			}
			else
			{
				if (MT[MtZNum].InPos(P_ELEV_Z_BTM))
				{
					m_dPos = MT[MtZNum].PosTable(3); // TODO: NAME DEFINE
					m_pFsm->Set(C_LEVEL_CHECK_Z_PITCH_MOVE);
				}
			}
			break;
		}
		case C_LEVEL_CHECK_Z_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_UP", m_strName);

			if (m_no == enFoamSendElev)
			{
				if (!CylInit())
					break;
			}
			else
			{
				if (!SetCylAlign())
					break;
			}

			if (OPR.isDryRun)
			{
				m_pFsm->Set(C_LEVEL_CHECK_Z_UP_CHECK);
				break;
			}

			MtPosMove(MtZNum, P_ELEV_Z_TOP);

			if (MT[MtZNum].m_state.isDriving || MT_INPOS == MtPosMove(MtZNum, P_ELEV_Z_TOP))
				m_pFsm->Set(C_LEVEL_CHECK_Z_UP_CHECK);
			break;
		}
		case C_LEVEL_CHECK_Z_UP_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_UP_CHECK", m_strName);

			rcpLevelUpDelay = (RcpList)(sendElevLevelUpDelay + m_no);
			if (AOn(iElevTop, (DWORD)NV.rcp[rcpLevelUpDelay]))
			{
				if (!MT[MtZNum].IsRdy())
				{
					MT[MtZNum].Stop();
					break;
				}
				m_nLevelCheckCnt++;
				if (1 < m_nLevelCheckCnt)
					m_pFsm->Set(C_LEVEL_CHECK_Z_PITCH_MOVE);
				else
					m_pFsm->Set(C_LEVEL_CHECK_Z_DN);
			}
			else
			{
				if (MT[MtZNum].InPos(P_ELEV_Z_TOP))
					m_pFsm->Set(C_LEVEL_CHECK_INIT);
			}
			break;
		}
		case C_LEVEL_CHECK_Z_PITCH_MOVE:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_PITCH_MOVE", m_strName);
				if (m_dPos != MT[MtZNum].PosTable(3))
					m_dPos = MT[MtZNum].m_state.realCnt + MT[MtZNum].PosTable(P_ELEV_Z_PITCH);
			}
			if (m_dPos <= MT[MtZNum].PosTable(P_ELEV_Z_BTM))
			{
				if (MT_INPOS != MtPosMove(MtZNum, P_ELEV_Z_BTM))
					break;
			}
			else if (m_dPos > MT[MtZNum].PosTable(P_ELEV_Z_TOP))
			{
				if (MT_INPOS != MtPosMove(MtZNum, P_ELEV_Z_TOP))
					break;
			}
			else
			{
				double dVel = MT[MtZNum].VelTable(P_ELEV_Z_PITCH);
				double dAcc = MT[MtZNum].AccTable(P_ELEV_Z_PITCH);
				if (MT_INPOS != MtPosMove(MtZNum, P_ELEV_Z_PITCH, m_dPos, dVel, dAcc))
					break;
			}

			m_pFsm->Set(C_LEVEL_CHECK_INIT);
			break;
		}
		case C_LEVEL_CHECK_INIT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_INIT", m_strName);

			if (!MT[MtZNum].IsRdy())
				break;

			if (!SetCylAlign())
				break;

			m_pFsm->Set(C_LEVEL_CHECK_CNT_CHECK);
			break;
		}
		case C_LEVEL_CHECK_CNT_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_CNT_CHECK", m_strName);

			FOAM_PART_DATA[m_nvFoamStack].flag.levelDid = true;

			if (MT[MtZNum].InPos(P_ELEV_Z_BTM)/* && AOn(iElevTop)*/)
				FOAM_PART_DATA[m_nvFoamStack].flag.recvFullDid = true;
			if (MT[MtZNum].InPos(P_ELEV_Z_TOP))
				FOAM_PART_DATA[m_nvFoamStack].flag.sendEmptyDid = true;

			if (m_no == enFoamSendElev)
			{
				if (FOAM_PART_DATA[m_nvFoamStack].flag.sendEmptyDid)
				{
					ZeroMemory(&FOAM_PART_DATA[nvFoamSendElev], sizeof(IPC_MMI::FoamPartData));
					MtPosMove(MtZNum, P_ELEV_Z_BTM);
					CylInit();
				}
			}

			m_pFsm->Set(C_LEVEL_CHECK_END);
			break;
		}
		case C_LEVEL_CHECK_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			if (!MT[MtZNum].IsRdy())
				break;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CFoamElev::CycleFlip(void)
{
	if (!m_pFsm->Between(C_FOAM_FLIP_START, C_FOAM_FLIP_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save((ErrName)(ER_FLIP_CYCLE_TIME_OVER));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_FOAM_FLIP_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_FLIP_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "FOAM_FLIP";
			}

			if (!FOAM_PART_DATA[m_nvFoamStack].flag.levelDid)
			{
				ER.Save((ErrName)(ER_FOAM_SEND_ELEV_NO_LEVEL_CHECK));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CYL[cylFoamSendElevLtAlignFb].IsFwd())
				CYL[cylFoamSendElevLtAlignFb].Fwd(FOAM_SEND_ELEV_LT_ALIGN_FWD);
			if (!CYL[cylFoamSendElevRtAlignFb].IsFwd())
				CYL[cylFoamSendElevRtAlignFb].Fwd(FOAM_SEND_ELEV_RT_ALIGN_FWD);

			if (!CYL[cylFoamSendElevLtAlignFb].IsFwd() ||
				!CYL[cylFoamSendElevRtAlignFb].IsFwd())
				break;

			m_pFsm->Set(C_FOAM_FLIP_MOVE);
			break;
		}
		case C_FOAM_FLIP_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_FLIP_MOVE", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (!VC[vcFoamFlip].AOn())
					ER.Save(ER_VAC_ALARM_FOAM_FLIP);
				else if (!VC[vcFoamFlip2].AOn())
					ER.Save(ER_VAC_ALARM_FOAM_FLIP2);

				ER.Save(ER_FOAM_FLIP_MOVE_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (MT_INPOS != MtPosMove(mtFoamFlipT, P_FOAM_FLIP_T_PICK))
				break;

			if (!VC[vcFoamFlip].IsOn() || !VC[vcFoamFlip2].IsOn())
			{
				VC[vcFoamFlip].On(FOAM_FLIP_ON);
				VC[vcFoamFlip2].On(FOAM_FLIP2_ON);
			}

			if (!CylInit())
				break;

			if (!VC[vcFoamFlip].IsOn() || !VC[vcFoamFlip2].IsOn() ||
				!VC[vcFoamFlip].AOn() || !VC[vcFoamFlip2].AOn())
				break;

			m_pFsm->Set(C_FOAM_FLIP_SLOW_READY_MOVE);
			break;
		}
		case C_FOAM_FLIP_SLOW_READY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_FLIP_SLOW_READY_MOVE", m_strName);

			double Zoffset = MT[mtFoamFlipT].PosTable(P_FOAM_FLIP_T_SLOW_SEND_OFFSET);
			double pos = MT[mtFoamFlipT].PosTable(P_FOAM_FLIP_T_PICK) - Zoffset;
			double vel = MT[mtFoamFlipT].VelTable(P_FOAM_FLIP_T_SLOW_SEND_OFFSET);
			double acc = MT[mtFoamFlipT].AccTable(P_FOAM_FLIP_T_SLOW_SEND_OFFSET);
			if (MT_INPOS != MtPosMove(mtFoamFlipT, P_FOAM_FLIP_T_SEND, pos, vel, acc))
				break;

			m_pFsm->Set(C_FOAM_FLIP_READY_MOVE);
			break;
		}
		case C_FOAM_FLIP_READY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_FLIP_READY_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtFoamFlipT, P_FOAM_FLIP_T_SEND))
				break;

			if (!VC[vcFoamFlip].AOn() || !VC[vcFoamFlip2].AOn())
			{
				if (!VC[vcFoamFlip].AOn())
					ER.Save(ER_VAC_ALARM_FOAM_FLIP);
				if (!VC[vcFoamFlip2].AOn())
					ER.Save(ER_VAC_ALARM_FOAM_FLIP2);

				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_FOAM_FLIP_END);
			break;
		}
		case C_FOAM_FLIP_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_FLIP_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			FOAM_PART_DATA[nvFoamSendElev].cnt--;
			FOAM_PART_DATA[nvFoamSendElev].flag.levelDid = false;
			FOAM_PART_DATA[nvFoamFlip].exist = enFoamExistBtm;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}