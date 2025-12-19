#include "..\..\Includes.h"

CLabelFeeder LABEL_FEEDER[enlabelMax];

CLabelFeeder::CLabelFeeder()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CLabelFeeder::~CLabelFeeder()
{

}

void CLabelFeeder::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	int m_no = *reinterpret_cast<int*>(lpParam);
	if (oldValue == newValue)
		return;

	if ("FEED" == newValue)
	{
		LABEL_FEEDER[m_no].m_pSLog->XFR(newValue, "$", "LABEL_FEEDER", "LABEL_FEEDER");

		if (m_no == enlabelLt)
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(mtLabelLaserX);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(mtLabelLtFeed);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederLockRrFb);
		}
		else
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(mtLabelLaserX);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(mtLabelRtFeed);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederLockRrFb);
		}

	}
	else if ("CST_LOAD" == newValue)
	{
		LABEL_FEEDER[m_no].m_pSLog->XFR(newValue, "$", "AMR", "LABEL_FEEDER");

		if (m_no == enlabelLt)
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederLockRrFb);
		}
		else
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederLockRrFb);
		}
	}
	else if ("CST_UNLOAD" == newValue)
	{
		LABEL_FEEDER[m_no].m_pSLog->XFR(newValue, "$", "LABEL_FEEDER", "AMR");

		if (m_no == enlabelLt)
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylLtLabelFeederLockRrFb);
		}
		else
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOn(cylRtLabelFeederLockRrFb);
		}
	}
	else
	{
		LABEL_FEEDER[m_no].m_pSLog->XFR(newValue, "$", "", "");

		if (m_no == enlabelLt)
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(mtLabelLaserX);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(mtLabelLtFeed);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylLtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylLtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylLtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylLtLabelFeederLockRrFb);
		}
		else
		{
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(mtLabelLaserX);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(mtLabelRtFeed);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylRtLabelFeederFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylRtLabelFeederMountFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylRtLabelFeederLockFtFb);
			LABEL_FEEDER[m_no].m_pSLog->RecordingOff(cylRtLabelFeederLockRrFb);
		}
	}
}

void CLabelFeeder::Auto(void)
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

	if (!ExistCst())
	{
		if (!IsCanCycleLabelCstLoad())
			return;

		m_pFsm->Set(C_LABEL_CST_LD_START);
		return;
	}
	else
	{
		if (LABEL_PART_DATA[nvLabelFeeder].flag.cstMaterialEmpty)
		{
			if (!IsCanCycleLabelCstUnload())
				return;

			m_pFsm->Set(C_LABEL_CST_ULD_START);
			return;
		}
		else
		{
			if (!Exist() && LABEL_PART_DATA[nvLabelFeeder].flag.inUseCst)
			{
				if (!IsCanCycleLabelFeed())
					return;

				m_pFsm->Set(C_LABEL_FEED_START);
				return;
			}
		}
	}
}

void CLabelFeeder::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
		{
			Off(oPioLdReq);
			Off(oPioUldReq);
			Off(oPioAbort);
			Off(oPioRdy);
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleLabelFeed();
	CycleLabelCstLoad();
	CycleLabelCstUnload();
}

void CLabelFeeder::Init(int nIdx)
{
	m_no = nIdx;
	if (m_no == enlabelLt)
	{
		strcpy(m_strName, "LtLabelFeeder");
		m_pFsm = &FSM[fsmLtLabelFeeder];
		m_pRefFsm = &FSM[fsmRtLabelFeeder];

		m_pSLog = &SLOG[slogLtLabelFeeder];
		m_pSLog->SetXFRDeviceId("LT_LABEL_FEEDER");
		m_pSLog->SetXFRMaterialType("LABEL");
		m_pSLog->Add(mtLabelLaserX);
		m_pSLog->Add(mtLabelLtFeed);

		m_pSLog->Add(cylLtLabelFeederFb);
		m_pSLog->Add(cylLtLabelFeederMountFb);
		m_pSLog->Add(cylLtLabelFeederLockFtFb);
		m_pSLog->Add(cylLtLabelFeederLockRrFb);

		nvLabelFeeder = nvLabelLtFeeder;
		nvRefLabelFeeder = nvLabelRtFeeder;
		mtFeedRewindNum = mtLabelLtFeed;

		m_pLabelFeederFb = &CYL[cylLtLabelFeederFb];
		m_pLabelFeederMountFb = &CYL[cylLtLabelFeederMountFb];
		m_pLabelFeederLockFtFb = &CYL[cylLtLabelFeederLockFtFb];
		m_pLabelFeederLockRrFb = &CYL[cylLtLabelFeederLockRrFb];

		iLabelCstLabelExist = iLtLabelFeederLabelExist;
		iLabelCstDetectFt = iLtLabelFeederDetectFt;
		iLabelCstDetectRr = iLtLabelFeederDetectRr;

		iPioValId = iLabelLtFeederPioValId;
		iPioCs0 = iLabelLtFeederPioCs0;
		iPioCs1 = iLabelLtFeederPioCs1;
		iPioCs2 = iLabelLtFeederPioCs2;
		iPioCs3 = iLabelLtFeederPioCs3;
		iPioTrReq = iLabelLtFeederPioTrReq;
		iPioBusy = iLabelLtFeederPioBusy;
		iPioComp = iLabelLtFeederPioComp;

		oPioLdReq = oLabelLtFeederPioLdReq;
		oPioUldReq = oLabelLtFeederPioUldReq;
		oPioAbort = oLabelLtFeederPioAbort;
		oPioRdy = oLabelLtFeederPioRdy;
	}
	else
	{
		strcpy(m_strName, "RtLabelFeeder");
		m_pFsm = &FSM[fsmRtLabelFeeder];
		m_pRefFsm = &FSM[fsmLtLabelFeeder];

		m_pSLog = &SLOG[slogRtLabelFeeder];
		m_pSLog->SetXFRDeviceId("RT_LABEL_FEEDER");
		m_pSLog->SetXFRMaterialType("LABEL");
		m_pSLog->Add(mtLabelLaserX);
		m_pSLog->Add(mtLabelRtFeed);

		m_pSLog->Add(cylRtLabelFeederFb);
		m_pSLog->Add(cylRtLabelFeederMountFb);
		m_pSLog->Add(cylRtLabelFeederLockFtFb);
		m_pSLog->Add(cylRtLabelFeederLockRrFb);

		nvLabelFeeder = nvLabelRtFeeder;
		nvRefLabelFeeder = nvLabelLtFeeder;
		mtFeedRewindNum = mtLabelRtFeed;

		m_pLabelFeederFb = &CYL[cylRtLabelFeederFb];
		m_pLabelFeederMountFb = &CYL[cylRtLabelFeederMountFb];
		m_pLabelFeederLockFtFb = &CYL[cylRtLabelFeederLockFtFb];
		m_pLabelFeederLockRrFb = &CYL[cylRtLabelFeederLockRrFb];

		iLabelCstLabelExist = iRtLabelFeederLabelExist;
		iLabelCstDetectFt = iRtLabelFeederDetectFt;
		iLabelCstDetectRr = iRtLabelFeederDetectRr;

		iPioValId = iLabelRtFeederPioValId;
		iPioCs0 = iLabelRtFeederPioCs0;
		iPioCs1 = iLabelRtFeederPioCs1;
		iPioCs2 = iLabelRtFeederPioCs2;
		iPioCs3 = iLabelRtFeederPioCs3;
		iPioTrReq = iLabelRtFeederPioTrReq;
		iPioBusy = iLabelRtFeederPioBusy;
		iPioComp = iLabelRtFeederPioComp;

		oPioLdReq = oLabelRtFeederPioLdReq;
		oPioUldReq = oLabelRtFeederPioUldReq;
		oPioAbort = oLabelRtFeederPioAbort;
		oPioRdy = oLabelRtFeederPioRdy;
	}
}

BOOL CLabelFeeder::Common(void)
{
	if (LABEL_PART_DATA[nvLabelLtFeeder].flag.existCst && LABEL_PART_DATA[nvLabelRtFeeder].flag.existCst)
	{
		if (!LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst && !LABEL_PART_DATA[nvLabelLtFeeder].flag.cstMaterialEmpty &&
			!LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseCst && !LABEL_PART_DATA[nvLabelRtFeeder].flag.cstMaterialEmpty)
			LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst = true;
		else
		{
			if (LABEL_PART_DATA[nvLabelLtFeeder].flag.cstMaterialEmpty)
			{
				LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst = false;

				if (LABEL_PART_DATA[nvLabelRtFeeder].flag.existCst &&
					!LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseCst &&
					!LABEL_PART_DATA[nvLabelRtFeeder].flag.cstMaterialEmpty)
				{
					LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseCst = true;
				}
			}

			if (LABEL_PART_DATA[nvLabelRtFeeder].flag.cstMaterialEmpty)
			{
				LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseCst = false;
				if (LABEL_PART_DATA[nvLabelLtFeeder].flag.existCst &&
					!LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst &&
					!LABEL_PART_DATA[nvLabelLtFeeder].flag.cstMaterialEmpty)
				{
					LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst = true;
				}
			}
		}
	}
	else if (LABEL_PART_DATA[nvLabelLtFeeder].flag.existCst)
	{
		if (LABEL_PART_DATA[nvLabelLtFeeder].flag.cstMaterialEmpty)
			LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst = false;
	}
	else if (LABEL_PART_DATA[nvLabelRtFeeder].flag.existCst)
	{
		if (LABEL_PART_DATA[nvLabelRtFeeder].flag.cstMaterialEmpty)
			LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseCst = false;
	}


	if (!ExistCst())
	{
		ZeroMemory(&LABEL_PART_DATA[nvLabelLtFeeder + m_no], sizeof(IPC_MMI::FoamPartData));
		if (NV.use[useLabelFeederAmr] && !LABEL_PART_DATA[nvLabelLtFeeder + m_no].flag.amrLoadCallDid)
			LABEL_PART_DATA[nvLabelLtFeeder + m_no].flag.amrLoadCallDid = true;
	}
	else
	{
		if (NV.use[useLabelFeederAmr] &&
			LABEL_PART_DATA[nvLabelLtFeeder + m_no].flag.cstMaterialEmpty && !LABEL_PART_DATA[nvLabelLtFeeder + m_no].flag.amrUnloadCallDid)
			LABEL_PART_DATA[nvLabelLtFeeder + m_no].flag.amrUnloadCallDid = true;
	}
	//if (LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseFeeder &&
	//	LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseFeeder)
	//{
	//	LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseFeeder = true;
	//	LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseFeeder = false;
	//}

	if (AOn(iPioValId))
	{
		int onCnt = 0;
		for (int csNo = iPioCs0; csNo <= iPioCs3; csNo++)
		{
			if (AOn((enDI)csNo))
				onCnt++;
		}
		// 전부 OFF 이거나 두개이상 ON이면 에러처리
		if (onCnt == 0 || onCnt >= 2)
			ER.Save((ErrName)(ER_LABEL_LT_CST_PIO_CS_ABNORMAL + m_no));
	}

	return TRUE;
}

BOOL CLabelFeeder::IsHwErr(void)
{
	if (!MT[mtLabelLaserX].m_state.isHome)
		return TRUE;
	if (!MT[mtFeedRewindNum].m_state.isHome)
		return TRUE;

	if (m_pLabelFeederFb->GetErr())
		return TRUE;
	if (m_pLabelFeederMountFb->GetErr())
		return TRUE;
	if (m_pLabelFeederLockFtFb->GetErr())
		return TRUE;
	if (m_pLabelFeederLockRrFb->GetErr())
		return TRUE;

	return FALSE;
}

BOOL CLabelFeeder::IsHwRdy(void)
{
	if (!MT[mtLabelLaserX].IsRdy())
		return TRUE;
	//if (!MT[mtFeedRewindNum].IsRdy())
	//	return TRUE;

	return FALSE;
}

BOOL CLabelFeeder::CylInit(void)
{
	if (!m_pLabelFeederFb->IsBwd() ||
		!m_pLabelFeederLockFtFb->IsFwd() || !m_pLabelFeederLockRrFb->IsFwd())
	{
		if (!m_pLabelFeederMountFb->IsBwd())
		{
			m_pLabelFeederMountFb->Bwd(LT_LABEL_FEEDER_MOUNT_BWD);
			return FALSE;
		}
	}

	if (!m_pLabelFeederLockFtFb->IsFwd() || !m_pLabelFeederLockRrFb->IsFwd())
	{
		m_pLabelFeederLockFtFb->Fwd(LT_LABEL_FEEDER_LOCK_FT_FWD);
		m_pLabelFeederLockRrFb->Fwd(LT_LABEL_FEEDER_LOCK_RR_FWD);
		return FALSE;
	}

	if (!m_pLabelFeederFb->IsBwd())
	{
		m_pLabelFeederFb->Bwd(LT_LABEL_FEEDER_BWD);
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelFeeder::IsCanCycleLabelFeed(void)
{
	if (LABEL_LASER.m_pFsm->Between(LABEL_LASER.C_PWR_METER_START, LABEL_LASER.C_PWR_METER_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Laser Power Meter Cycle is Busy");
		return FALSE;
	}

	if (Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Memory Exist");
		return FALSE;
	}

	if (!ExistCst())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Cst Memory Exist");
		return FALSE;
	}

	if (LABEL_SHUTTLE[m_no].Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Memory Exist");
		return FALSE;
	}

	if (LABEL_SHUTTLE[m_no].m_pFsm->IsBusy() || LABEL_SHUTTLE[m_no].m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}

	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelFeeder::IsCanCycleLabelCstLoad(void)
{
	if (LABEL_LASER.m_pFsm->Between(LABEL_LASER.C_PWR_METER_START, LABEL_LASER.C_PWR_METER_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Laser Power Meter Cycle is Busy");
		return FALSE;
	}

	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Cycle is Busy");
		return FALSE;
	}

	if (LABEL_SHUTTLE[m_no].Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Memory Exist");
		return FALSE;
	}

	if (LABEL_SHUTTLE[m_no].m_pFsm->IsBusy() || LABEL_SHUTTLE[m_no].m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}

	if (ExistCst())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Cst Exist");
		return FALSE;
	}

	if (!AOn(iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"iPioValId Off");
		return FALSE;
	}
	return TRUE;
}

BOOL CLabelFeeder::IsCanCycleLabelCstUnload(void)
{
	if (LABEL_LASER.m_pFsm->Between(LABEL_LASER.C_PWR_METER_START, LABEL_LASER.C_PWR_METER_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Laser Power Meter Cycle is Busy");
		return FALSE;
	}

	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Cycle is Busy");
		return FALSE;
	}

	if (LABEL_SHUTTLE[m_no].Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Memory Exist");
		return FALSE;
	}

	if (LABEL_SHUTTLE[m_no].m_pFsm->IsBusy() || LABEL_SHUTTLE[m_no].m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}

	if (!ExistCst())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Cst Empty");
		return FALSE;
	}

	if (!AOn(iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"iPioValId Off");
		return FALSE;
	}

	if (!LABEL_PART_DATA[nvLabelFeeder].flag.cstMaterialEmpty)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"cstMaterialEmpty Flag Off");

		return FALSE;
	}

	return TRUE;
}

BOOL CLabelFeeder::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy())
	{
		if (!ExistCst() && (AOn(iLabelCstLabelExist) || AOn(iLabelCstDetectFt) || AOn(iLabelCstDetectRr)))
		{
			ER.Save((ErrName)(ER_LABEL_LT_FEEDER_SENSOR_STATUS + (m_no * 2)));
			return TRUE;
		}
		// TODO: iLabelCstLabelExist Only Empty Check?
		if (ExistCst() && (AOff(iLabelCstLabelExist) || AOff(iLabelCstDetectFt) || AOff(iLabelCstDetectRr)))
		{
			ER.Save((ErrName)(ER_LABEL_LT_FEEDER_MEMORY_STATUS + (m_no * 2)));
			return TRUE;
		}
	}

	return FALSE;
}

bool& CLabelFeeder::ExistCst(void)
{
	return LABEL_PART_DATA[nvLabelFeeder].flag.existCst;
}

int& CLabelFeeder::Exist(void)
{
	return LABEL_PART_DATA[nvLabelFeeder].exist;
}

BOOL CLabelFeeder::IsExist(void)
{
	if (!LABEL_PART_DATA[nvLabelFeeder].exist)
	{
		ER.Save((ErrName)(ER_LABEL_LT_FEEDER_EMPTY + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelFeeder::IsExistCst(void)
{
	if (!LABEL_PART_DATA[nvLabelFeeder].flag.existCst ||
		AOff(iLabelCstLabelExist) || AOff(iLabelCstDetectFt) || AOff(iLabelCstDetectRr))
	{
		ER.Save((ErrName)(ER_LABEL_LT_FEEDER_CST_EMPTY + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelFeeder::IsEmpty(void)
{
	if (LABEL_PART_DATA[nvLabelFeeder].exist)
	{
		ER.Save((ErrName)(ER_LABEL_LT_FEEDER_EXIST + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelFeeder::IsEmptyCst(void)
{
	if (LABEL_PART_DATA[nvLabelFeeder].flag.existCst ||
		AOn(iLabelCstLabelExist) || AOn(iLabelCstDetectFt) || AOn(iLabelCstDetectRr))
	{
		ER.Save((ErrName)(ER_LABEL_LT_FEEDER_CST_EXIST + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

bool CLabelFeeder::FeederTrqZeroStop(void)
{
	if (!MT[mtFeedRewindNum].IsRdy())
	{
		if (MT[mtFeedRewindNum].m_state.isDriving)
			MT[mtFeedRewindNum].Stop();
		return false;
	}

	if (0 != MT[mtFeedRewindNum].m_command.trq)
	{
		MT[mtFeedRewindNum].TrqMove(P_LABEL_T_READY, 0, 1);
		return false;
	}

	return true;
}


void CLabelFeeder::CycleLabelFeed(void)
{
	if (!m_pFsm->Between(C_LABEL_FEED_START, C_LABEL_FEED_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save(ErrName(ER_LABEL_LT_FEED_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_FEED_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_FEED_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "FEED";
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_FEED_SENSOR_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!IsCanCycleLabelFeed() || !CylInit())
				break;

			if (!m_pLabelFeederMountFb->IsFwd())
			{
				if (!FeederTrqZeroStop())
					break;

				m_pLabelFeederMountFb->Fwd(LT_LABEL_FEEDER_MOUNT_FWD);
				break;
			}

			if (MT_INPOS != MtPosMove(mtLabelLaserX, P_LASER_X_LT_EXIST + m_no))
				break;

			if (AOff(iLaserLabelPos, (int)NV.rcp[labelArrivalDelay]))
			{
				m_pFsm->Set(C_LABEL_FEED_END);
				break;
			}
			else
			{
				if (AOff(iLaserLabelPos))
					break;
			}

			m_pFsm->Set(C_LABEL_FEED_STOP);
			break;
		}
		case C_LABEL_FEED_STOP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_FEED_STOP", m_strName);

			if (!MT[mtFeedRewindNum].IsRdy() || 
				AxisCommandMode::Position != MT[mtFeedRewindNum].GetCmdMode())
			{
				MT[mtFeedRewindNum].Stop();
				MT[mtFeedRewindNum].SetCmdMode(AxisCommandMode::Position);
				break;
			}

			if (!m_pFsm->Delay(100))
				break;

			m_pFsm->Set(C_LABEL_FEED_MOVE);
			break;
		}
		case C_LABEL_FEED_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_FEED_MOVE", m_strName);

			if (MT[mtFeedRewindNum].CanMove(P_LABEL_T_FEED))
			{
				double dPos = MT[mtFeedRewindNum].m_state.realCnt + MT[mtFeedRewindNum].PosTable(P_LABEL_T_FEED_FAST);
				MT[mtFeedRewindNum].PosMove(P_LABEL_T_FEED, dPos);
				m_pFsm->Set(C_LABEL_FEED_SENSOR_CHECK);
			}
			break;
		}
		case C_LABEL_FEED_SENSOR_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_FEED_SENSOR_CHECK", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				MT[mtFeedRewindNum].Stop();
				ER.Save((ErrName)(ER_LABEL_LT_FEED_SENSOR_CHECK_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iLaserLabelPos, (int)NV.rcp[labelArrivalDelay]))
			{
				if (!MT[mtFeedRewindNum].IsRdy())
				{
					MT[mtFeedRewindNum].Stop();
					break;
				}
				m_pFsm->Set(C_LABEL_FEED_END);
			}
			break;
		}
		case C_LABEL_FEED_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_FEED_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist() = enLabelExist;
			//LABEL_PART_DATA[nvLabelLtFeeder + m_no].cnt++;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLabelFeeder::CycleLabelCstLoad(void)
{
	if (!m_pFsm->Between(C_LABEL_CST_LD_START, C_LABEL_CST_LD_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save((ErrName)(ER_LABEL_LT_CST_LOAD_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_CST_LD_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "CST_LOAD";
			}

			if (!CylInit())
				break;

			if (!m_pLabelFeederMountFb->IsBwd())
			{
				m_pLabelFeederMountFb->Bwd(LT_LABEL_FEEDER_MOUNT_BWD);
				break;
			}

			if (!IsEmptyCst())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_CST_LD_REQ_ON);
			break;
		}
		case C_LABEL_CST_LD_REQ_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_REQ_ON", m_strName);

			if (!IsOn(oPioLdReq))
			{
				On(oPioLdReq);
				LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_ON", "START", "$", "0", oPioLdReq, "'ON'");
				break;
			}
			LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_ON", "END", "$", "0", oPioLdReq, "'ON'");
			LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_REQ_IN_ON_WAIT", "START", "$", "100", iPioTrReq, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_LD_REQ_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioTrReq, 100))
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_REQ_IN_ON_WAIT", "END", "$", "100", iPioTrReq, "'ON'");
				m_pFsm->Set(C_LABEL_CST_LD_SAFETY_MOVE);
			}

			break;
		}
		case C_LABEL_CST_LD_SAFETY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_SAFETY_MOVE", m_strName);

			if (!m_pLabelFeederMountFb->IsBwd())
			{
				m_pLabelFeederMountFb->Bwd(LT_LABEL_FEEDER_MOUNT_BWD);
				break;
			}

			if (!m_pLabelFeederFb->IsFwd())
			{
				m_pLabelFeederFb->Fwd(LT_LABEL_FEEDER_FWD);
				break;
			}

			if (!m_pLabelFeederLockFtFb->IsBwd() || !m_pLabelFeederLockRrFb->IsBwd())
			{
				m_pLabelFeederLockFtFb->Bwd(LT_LABEL_FEEDER_LOCK_FT_BWD);
				m_pLabelFeederLockRrFb->Bwd(LT_LABEL_FEEDER_LOCK_RR_BWD);
				break;
			}

			m_pFsm->Set(C_LABEL_CST_LD_RDY_ON);
			break;
		}
		case C_LABEL_CST_LD_RDY_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_RDY_ON", m_strName);

			if (!IsOn(oPioRdy))
			{
				On(oPioRdy);
				LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_LD_RDY_OUT_ON", "START", "$", "0", oPioRdy, "'ON'");
				break;
			}
			LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_LD_RDY_OUT_ON", "END", "$", "0", oPioRdy, "'ON'");
			LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_WAIT", "START", "$", "100", iLabelCstLabelExist, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_LD_BUSY_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioBusy, 100))
				m_pFsm->Set(C_LABEL_CST_LD_REQ_OFF);

			break;
		}
		case C_LABEL_CST_LD_REQ_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_REQ_OFF", m_strName);

			if (m_pFsm->TimeLimit(_50sec))
			{
				if (AOff(iLabelCstLabelExist))
					ER.Save((ErrName)(ER_LABEL_LT_CST_LD_REQ_LABEL_EXIST_SENSOR_OFF + m_no));

				if (AOff(iLabelCstLabelExist) || 
					AOff(iLabelCstDetectFt) || AOff(iLabelCstDetectRr))
					ER.Save((ErrName)(ER_LABEL_LT_CST_LD_REQ_LABEL_CST_DETECT_SENSOR_OFF + m_no));

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (IsOn(oPioLdReq))
			{
				if (AOn(iLabelCstLabelExist, 1000) &&
					AOn(iLabelCstDetectFt, 1000) && AOn(iLabelCstDetectRr, 1000))
				{
					Off(oPioLdReq);
					LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_WAIT", "END", "$", "100", iLabelCstLabelExist, "'ON'");
					LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_OFF", "START", "$", "0", oPioLdReq, "'OFF'");
				}
			}
			else
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_OFF", "END", "$", "0", oPioLdReq, "'OFF'");
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_COMP_ON_WAIT", "START", "$", "100", iPioComp, "'ON'");

				if (AOff(iPioBusy, 100))
					m_pFsm->Set(C_LABEL_CST_LD_RDY_OFF);
			}

			break;
		}
		case C_LABEL_CST_LD_RDY_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_RDY_OFF", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_LD_COMP_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioComp, 100))
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_COMP_ON_WAIT", "END", "$", "100", iPioComp, "'ON'");

				if (!CylInit())
					break;

				Off(oPioRdy);
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_COMP_OFF_WAIT", "START", "$", "100", iPioComp, "'OFF'");
				m_pFsm->Set(C_LABEL_CST_LD_COMP_OFF);
			}

			break;
		}
		case C_LABEL_CST_LD_COMP_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_COMP_OFF", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_LD_COMP_OFF_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iPioComp, 100))
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_LD_COMP_OFF_WAIT", "END", "$", "100", iPioComp, "'OFF'");
				ZeroMemory(&LABEL_PART_DATA[nvLabelFeeder], sizeof(IPC_MMI::LabelPartData));
				LABEL_PART_DATA[nvLabelFeeder].flag.existCst = TRUE;
				LABEL_PART_DATA[nvLabelFeeder].flag.firstFeeding = true;

				m_pFsm->Set(C_LABEL_CST_LD_RFID_READ);
			}

			break;
		}
		case C_LABEL_CST_LD_RFID_READ:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_RFID_READ", m_strName);
				NV.ndm[commEqpLabelLtFeederRfidRead + m_no] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_LD_RFID_READ_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[commEqpLabelLtFeederRfidRead + m_no] != COMM_COMP)
				break;

			m_pFsm->Set(C_LABEL_CST_LD_END);
			break;
		}
		case C_LABEL_CST_LD_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_LD_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLabelFeeder::CycleLabelCstUnload(void)
{
	if (!m_pFsm->Between(C_LABEL_CST_ULD_START, C_LABEL_CST_ULD_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save((ErrName)(ER_LABEL_LT_CST_UNLOAD_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_CST_ULD_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "CST_UNLOAD";
			}

			if (!IsExistCst())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_CST_ULD_REQ_ON);
			break;
		}
		case C_LABEL_CST_ULD_REQ_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_REQ_ON", m_strName);

			if (!IsOn(oPioUldReq))
			{
				On(oPioUldReq);
				LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_ON", "START", "$", "0", oPioUldReq, "'ON'");
				break;
			}
			LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_ON", "END", "$", "0", oPioUldReq, "'ON'");
			LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_REQ_IN_ON_WAIT", "START", "$", "100", iPioTrReq, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_ULD_REQ_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioTrReq, 100))
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_REQ_IN_ON_WAIT", "END", "$", "100", iPioTrReq, "'ON'");
				m_pFsm->Set(C_LABEL_CST_ULD_SAFETY_MOVE);
			}

			break;
		}
		case C_LABEL_CST_ULD_SAFETY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_SAFETY_MOVE", m_strName);

			if (!m_pLabelFeederMountFb->IsBwd())
			{
				m_pLabelFeederMountFb->Bwd(LT_LABEL_FEEDER_MOUNT_BWD);
				break;
			}

			if (!m_pLabelFeederFb->IsFwd())
			{
				m_pLabelFeederFb->Fwd(LT_LABEL_FEEDER_FWD);
				break;
			}

			if (!m_pLabelFeederLockFtFb->IsBwd() || !m_pLabelFeederLockRrFb->IsBwd())
			{
				m_pLabelFeederLockFtFb->Bwd(LT_LABEL_FEEDER_LOCK_FT_BWD);
				m_pLabelFeederLockRrFb->Bwd(LT_LABEL_FEEDER_LOCK_RR_BWD);
				break;
			}

			m_pFsm->Set(C_LABEL_CST_ULD_RDY_ON);
			break;
		}
		case C_LABEL_CST_ULD_RDY_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_RDY_ON", m_strName);

			if (!IsOn(oPioRdy))
			{
				On(oPioRdy);
				LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_ULD_RDY_OUT_ON", "START", "$", "0", oPioRdy, "'ON'");
				break;
			}
			LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_ULD_RDY_OUT_ON", "END", "$", "0", oPioRdy, "'ON'");
			LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_WAIT", "START", "$", "100", iLabelCstLabelExist, "'OFF'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_ULD_BUSY_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioBusy, 100))
				m_pFsm->Set(C_LABEL_CST_ULD_REQ_OFF);

			break;
		}
		case C_LABEL_CST_ULD_REQ_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_REQ_OFF", m_strName);

			if (m_pFsm->TimeLimit(_50sec))
			{
				if (AOn(iLabelCstLabelExist))
					ER.Save((ErrName)(ER_LABEL_LT_CST_ULD_REQ_LABEL_EXIST_SENSOR_ON + m_no));

				if (AOn(iLabelCstDetectFt) || AOn(iLabelCstDetectRr))
					ER.Save((ErrName)(ER_LABEL_LT_CST_ULD_REQ_LABEL_CST_DETECT_SENSOR_ON + m_no));

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (IsOn(oPioUldReq))
			{
				if (AOff(iLabelCstLabelExist, 1000) &&
					AOff(iLabelCstDetectFt, 1000) && AOff(iLabelCstDetectRr, 1000))
				{
					Off(oPioUldReq);
					LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_WAIT", "END", "$", "100", iLabelCstLabelExist, "'OFF'");
					LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_OFF", "START", "$", "0", oPioLdReq, "'OFF'");
				}
			}
			else
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_OFF", "END", "$", "0", oPioLdReq, "'OFF'");
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_ON_WAIT", "START", "$", "100", iPioComp, "'ON'");
				if (AOff(iPioBusy, 100))
					m_pFsm->Set(C_LABEL_CST_ULD_RDY_OFF);
			}

			break;
		}
		case C_LABEL_CST_ULD_RDY_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_RDY_OFF", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_ULD_COMP_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioComp, 100))
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_ON_WAIT", "END", "$", "100", iPioComp, "'ON'");
				Off(oPioRdy);
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_OFF_WAIT", "START", "$", "100", iPioComp, "'OFF'");
				m_pFsm->Set(C_LABEL_CST_ULD_COMP_OFF);
			}

			break;
		}
		case C_LABEL_CST_ULD_COMP_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_COMP_OFF", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_CST_ULD_COMP_OFF_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iPioComp, 100))
			{
				LABEL_FEEDER[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_OFF_WAIT", "END", "$", "100", iPioComp, "'OFF'");
				ZeroMemory(&LABEL_PART_DATA[nvLabelFeeder], sizeof(IPC_MMI::LabelPartData));
				m_pFsm->Set(C_LABEL_CST_ULD_END);
			}

			// TODO: RFID Read Step ADD.
			break;
		}
		case C_LABEL_CST_ULD_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_CST_ULD_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}