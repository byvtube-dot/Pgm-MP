#include "..\..\Includes.h"

CFoamBox FOAM_CST[enFoamBoxMax];	// enFoamBoxLt, enFoamBoxRt

CFoamBox::CFoamBox()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CFoamBox::~CFoamBox()
{

}

void CFoamBox::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	int m_no = *reinterpret_cast<int*>(lpParam);
	if (oldValue == newValue)
		return;

	if ("CST_LOAD" == newValue)
		FOAM_CST[m_no].m_pSLog->XFR(newValue, "$", "AMR", "FOAM_CST");
	else if ("CST_UNLOAD" == newValue)
		FOAM_CST[m_no].m_pSLog->XFR(newValue, "$", "FOAM_CST", "AMR");
	else
		FOAM_CST[m_no].m_pSLog->XFR(newValue, "$", "", "");

	if ("" == newValue)
	{
		FOAM_CST[m_no].m_pSLog->RecordingOff(mtFoamPkX);
		FOAM_CST[m_no].m_pSLog->RecordingOff(mtFoamPkY);
		FOAM_CST[m_no].m_pSLog->RecordingOff(mtFoamPkY2);
		FOAM_CST[m_no].m_pSLog->RecordingOff(mtFoamPkZ);
	}
	else
	{
		FOAM_CST[m_no].m_pSLog->RecordingOn(mtFoamPkX);
		FOAM_CST[m_no].m_pSLog->RecordingOn(mtFoamPkY);
		FOAM_CST[m_no].m_pSLog->RecordingOn(mtFoamPkY2);
		FOAM_CST[m_no].m_pSLog->RecordingOn(mtFoamPkZ);
	}
}

void CFoamBox::Auto(void)
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

	if (!Exist())
	{
		if (!IsCanCycleFoamBoxLoad())
			return;

		m_pFsm->Set(C_FOAM_CST_LD_START);
		return;
	}
	else
	{
		if (!IsCanCycleFoamBoxUnload())
			return;

		m_pFsm->Set(C_FOAM_CST_ULD_START);
		return;
	}
}

void CFoamBox::Cycle(void)
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

	CycleFoamBoxLoad();
	CycleFoamBoxUnload();
}

void CFoamBox::Init(int nIdx)
{
	m_no = nIdx;
	if (m_no == enFoamBoxLt)
	{
		strcpy(m_strName, "FoamLtCst");
		m_pFsm = &FSM[fsmFoamLtCst];
		m_pRefFsm = &FSM[fsmFoamRtCst];

		m_pSLog = &SLOG[slogFoamLtCst];
		m_pSLog->SetXFRDeviceId("FOAM_LT_CST");
		m_pSLog->SetXFRMaterialType("FOAM_CST");
		m_pSLog->Add(mtFoamPkX);
		m_pSLog->Add(mtFoamPkY);
		m_pSLog->Add(mtFoamPkY2);
		m_pSLog->Add(mtFoamPkZ);

		iFoamBoxFoamExist = iFoamLtCstFoamExist;
		iFoamBoxDetectLtFt = iFoamLtCstDetectLtFt;
		iFoamBoxDetectRtFt = iFoamLtCstDetectRtFt;
		iFoamBoxDetectLtRr = iFoamLtCstDetectLtRr;
		iFoamBoxDetectRtRr = iFoamLtCstDetectRtRr;

		iPioValId = iFoamLtStackPioValId;
		iPioCs0 = iFoamLtStackPioCs0;
		iPioCs1 = iFoamLtStackPioCs1;
		iPioCs2 = iFoamLtStackPioCs2;
		iPioCs3 = iFoamLtStackPioCs3;
		iPioTrReq = iFoamLtStackPioTrReq;
		iPioBusy = iFoamLtStackPioBusy;
		iPioComp = iFoamLtStackPioComp;

		oPioLdReq = oFoamLtStackPioLdReq;
		oPioUldReq = oFoamLtStackPioUldReq;
		oPioAbort = oFoamLtStackPioAbort;
		oPioRdy = oFoamLtStackPioRdy;
	}
	else
	{
		strcpy(m_strName, "FoamRtCst");
		m_pFsm = &FSM[fsmFoamRtCst];
		m_pRefFsm = &FSM[fsmFoamLtCst];

		m_pSLog = &SLOG[slogFoamRtCst];
		m_pSLog->SetXFRDeviceId("FOAM_RT_CST");
		m_pSLog->SetXFRMaterialType("FOAM_CST");
		m_pSLog->Add(mtFoamPkX);
		m_pSLog->Add(mtFoamPkY);
		m_pSLog->Add(mtFoamPkY2);
		m_pSLog->Add(mtFoamPkZ);

		iFoamBoxFoamExist = iFoamRtCstFoamExist;
		iFoamBoxDetectLtFt = iFoamRtCstDetectLtFt;
		iFoamBoxDetectRtFt = iFoamRtCstDetectRtFt;
		iFoamBoxDetectLtRr = iFoamRtCstDetectLtRr;
		iFoamBoxDetectRtRr = iFoamRtCstDetectRtRr;

		iPioValId = iFoamRtStackPioValId;
		iPioCs0 = iFoamRtStackPioCs0;
		iPioCs1 = iFoamRtStackPioCs1;
		iPioCs2 = iFoamRtStackPioCs2;
		iPioCs3 = iFoamRtStackPioCs3;
		iPioTrReq = iFoamRtStackPioTrReq;
		iPioBusy = iFoamRtStackPioBusy;
		iPioComp = iFoamRtStackPioComp;

		oPioLdReq = oFoamRtStackPioLdReq;
		oPioUldReq = oFoamRtStackPioUldReq;
		oPioAbort = oFoamRtStackPioAbort;
		oPioRdy = oFoamRtStackPioRdy;
	}
}

BOOL CFoamBox::Common(void)
{
	if (!Exist())
	{
		ZeroMemory(&FOAM_PART_DATA[nvFoamLtCst + m_no], sizeof(IPC_MMI::FoamPartData));
		if (NV.use[useFoamBoxAmr] && !FOAM_PART_DATA[nvFoamLtCst + m_no].flag.amrLoadCallDid)
			FOAM_PART_DATA[nvFoamLtCst + m_no].flag.amrLoadCallDid = true;
	}
	else
	{
		if (NV.use[useFoamBoxAmr] &&
			FOAM_PART_DATA[nvFoamLtCst + m_no].flag.cstMaterialEmpty && !FOAM_PART_DATA[nvFoamLtCst + m_no].flag.amrUnloadCallDid)
			FOAM_PART_DATA[nvFoamLtCst + m_no].flag.amrUnloadCallDid = true;
	}

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

BOOL CFoamBox::IsHwErr(void)
{
	if (!MT[mtFoamPkX].m_state.isHome)
		return TRUE;
	if (!MT[mtFoamPkY].m_state.isHome)
		return TRUE;
	//if (!MT[mtFoamPkY2].m_state.isHome)
	//	return TRUE;
	if (!MT[mtFoamPkZ].m_state.isHome)
		return TRUE;

	return FALSE;
}

BOOL CFoamBox::IsHwRdy(void)
{
	if (!MT[mtFoamPkX].IsRdy())
		return TRUE;
	if (!MT[mtFoamPkY].IsRdy())
		return TRUE;
	//if (!MT[mtFoamPkY2].IsRdy())
	//	return TRUE;
	if (!MT[mtFoamPkZ].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CFoamBox::CylInit(void)
{

	return TRUE;
}

BOOL CFoamBox::IsCanCycleFoamBoxLoad(void)
{
	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Picker Cycle is Busy");
		return FALSE;
	}

	if (FOAM_PK.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Picker Cycle is Busy");
		return FALSE;
	}

	if (Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Memory Exist");
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


BOOL CFoamBox::IsCanCycleFoamBoxUnload(void)
{
	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Picker Cycle is Busy");
		return FALSE;
	}

	if (FOAM_PK.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Picker Cycle is Busy");
		return FALSE;
	}

	if (!Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Memory Empty");
		return FALSE;
	}

	if (!AOn(iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"iPioValId Off");
		return FALSE;
	}

	if (!FOAM_PART_DATA[nvFoamLtCst + m_no].flag.cstMaterialEmpty)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"cstMaterialEmpty Flag Off");

		return FALSE;
	}

	// TODO: ADD Condition.
	return TRUE;
}

BOOL CFoamBox::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy())
	{
		if (!Exist() && AOn(iFoamBoxFoamExist))
		{
			ER.Save((ErrName)(ER_FOAM_LT_CST_SENSOR_STATUS + (m_no * 2)));
			return TRUE;
		}

		if (Exist() && AOff(iFoamBoxFoamExist))
		{
			ER.Save((ErrName)(ER_FOAM_LT_CST_MEMORY_STATUS + (m_no * 2)));
			return TRUE;
		}
	}

	return FALSE;
}

int& CFoamBox::Exist(void)
{
	return FOAM_PART_DATA[nvFoamLtCst + m_no].exist;
}

BOOL CFoamBox::IsExist(void)
{
	if (!Exist() ||
		AOff(iFoamBoxFoamExist) ||
		AOff(iFoamBoxDetectLtFt) || AOff(iFoamBoxDetectRtFt) ||
		AOff(iFoamBoxDetectLtRr) || AOff(iFoamBoxDetectRtRr))
	{
		ER.Save((ErrName)(ER_FOAM_LT_CST_EMPTY + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamBox::IsEmpty(void)
{
	if (Exist() ||
		AOn(iFoamBoxFoamExist) ||
		AOn(iFoamBoxDetectLtFt) || AOn(iFoamBoxDetectRtFt) ||
		AOn(iFoamBoxDetectLtRr) || AOn(iFoamBoxDetectRtRr))
	{
		ER.Save((ErrName)(ER_FOAM_LT_CST_EXIST + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

void CFoamBox::CycleFoamBoxLoad(void)
{
	if (!m_pFsm->Between(C_FOAM_CST_LD_START, C_FOAM_CST_LD_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save((ErrName)(ER_FOAM_LT_CST_LOAD_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_FOAM_CST_LD_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "CST_LOAD";
				NV.ndm[commEqpFoamLtBoxRfidRead] = COMM_IDLE;
			}

			if (!IsEmpty())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_FOAM_CST_LD_REQ_ON);
			break;
		}
		case C_FOAM_CST_LD_REQ_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_REQ_ON", m_strName);

			if (!IsOn(oPioLdReq))
			{
				On(oPioLdReq);
				FOAM_CST[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_ON", "START", "$", "0", oPioLdReq, "'ON'");
				break;
			}
			FOAM_CST[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_ON", "END", "$", "0", oPioLdReq, "'ON'");
			FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_REQ_IN_ON_WAIT", "START", "$", "100", iPioTrReq, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_LD_REQ_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioTrReq, 100))
			{
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_REQ_IN_ON_WAIT", "END", "$", "100", iPioTrReq, "'ON'");
				m_pFsm->Set(C_FOAM_CST_LD_SAFETY_MOVE);
			}

			break;
		}
		case C_FOAM_CST_LD_SAFETY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_SAFETY_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
				break;

			MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_CST_WORK_SAFETY + m_no);
			MtPosMove(mtFoamPkY, P_FOAM_PK_Y_SAFETY);

			if (!MT[mtFoamPkX].InPos(P_FOAM_PK_X_LT_CST_WORK_SAFETY + m_no) || !MT[mtFoamPkY].InPos(P_FOAM_PK_Y_SAFETY))
				break;

			m_pFsm->Set(C_FOAM_CST_LD_RDY_ON);
			break;
		}
		case C_FOAM_CST_LD_RDY_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_RDY_ON", m_strName);

			if (!IsOn(oPioRdy))
			{
				On(oPioRdy);
				FOAM_CST[m_no].m_pSLog->LogFncOut("CST_LD_RDY_OUT_ON", "START", "$", "0", oPioRdy, "'ON'");
				break;
			}
			FOAM_CST[m_no].m_pSLog->LogFncOut("CST_LD_RDY_OUT_ON", "END", "$", "0", oPioRdy, "'ON'");
			FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_WAIT", "START", "$", "100", iFoamBoxFoamExist, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_LD_BUSY_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioBusy, 100))
			{
				m_pFsm->Set(C_FOAM_CST_LD_REQ_OFF);
			}

			break;
		}
		case C_FOAM_CST_LD_REQ_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_REQ_OFF", m_strName);

			if (m_pFsm->TimeLimit(_50sec))
			{
				if (AOff(iFoamBoxFoamExist))
					ER.Save((ErrName)(ER_FOAM_LT_CST_LD_REQ_FOAM_EXIST_SENSOR_OFF + m_no));

				if (AOff(iFoamBoxDetectLtFt) || AOff(iFoamBoxDetectRtFt) ||
					AOff(iFoamBoxDetectLtRr) || AOff(iFoamBoxDetectRtRr))
					ER.Save((ErrName)(ER_FOAM_LT_CST_LD_REQ_FOAM_CST_DETECT_SENSOR_OFF + m_no));

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (IsOn(oPioLdReq))
			{
				if (AOn(iFoamBoxFoamExist, 1000) &&
					AOn(iFoamBoxDetectLtFt, 1000) && AOn(iFoamBoxDetectRtFt, 1000) &&
					AOn(iFoamBoxDetectLtRr, 1000) && AOn(iFoamBoxDetectRtRr, 1000))
				{
					Off(oPioLdReq);
					FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_WAIT", "END", "$", "100", iFoamBoxFoamExist, "'ON'");
					FOAM_CST[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_OFF", "START", "$", "0", oPioLdReq, "'OFF'");
				}
			}
			else
			{
				FOAM_CST[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_OFF", "END", "$", "0", oPioLdReq, "'OFF'");
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_COMP_ON_WAIT", "START", "$", "100", iPioComp, "'ON'");
			
				if (AOff(iPioBusy, 100))
					m_pFsm->Set(C_FOAM_CST_LD_RDY_OFF);
			}
			break;
		}
		case C_FOAM_CST_LD_RDY_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_RDY_OFF", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_LD_COMP_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioComp, 100))
			{
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_COMP_ON_WAIT", "END", "$", "100", iPioComp, "'ON'");
				Off(oPioRdy);
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_COMP_OFF_WAIT", "START", "$", "100", iPioComp, "'OFF'");
				m_pFsm->Set(C_FOAM_CST_LD_COMP_OFF);
			}
			
			break;
		}
		case C_FOAM_CST_LD_COMP_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_COMP_OFF", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_LD_COMP_OFF_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iPioComp, 100))
			{
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_LD_COMP_OFF_WAIT", "END", "$", "100", iPioComp, "'OFF'");
				ZeroMemory(&FOAM_PART_DATA[nvFoamLtCst + m_no], sizeof(IPC_MMI::FoamPartData));
				FOAM_PART_DATA[nvFoamLtCst + m_no].exist = TRUE;

				m_pFsm->Set(C_FOAM_CST_LD_RFID_READ);
			}

			break;
		}
		case C_FOAM_CST_LD_RFID_READ:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_RFID_READ", m_strName);
				NV.ndm[commEqpFoamLtBoxRfidRead + m_no] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_LD_RFID_READ_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[commEqpFoamLtBoxRfidRead + m_no] != COMM_COMP)
				break;

			m_pFsm->Set(C_FOAM_CST_LD_END);
			break;
		}
		case C_FOAM_CST_LD_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_LD_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CFoamBox::CycleFoamBoxUnload(void)
{
	if (!m_pFsm->Between(C_FOAM_CST_ULD_START, C_FOAM_CST_ULD_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save((ErrName)(ER_FOAM_LT_CST_UNLOAD_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_FOAM_CST_ULD_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "CST_UNLOAD";
			}

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_FOAM_CST_ULD_REQ_ON);
			break;
		}
		case C_FOAM_CST_ULD_REQ_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_REQ_ON", m_strName);

			if (!IsOn(oPioUldReq))
			{
				On(oPioUldReq);
				FOAM_CST[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_ON", "START", "$", "0", oPioUldReq, "'ON'");
				break;
			}
			FOAM_CST[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_ON", "END", "$", "0", oPioUldReq, "'ON'");
			FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_REQ_IN_ON_WAIT", "START", "$", "100", iPioTrReq, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_ULD_REQ_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioTrReq, 100))
			{
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_REQ_IN_ON_WAIT", "END", "$", "100", iPioTrReq, "'ON'");
				m_pFsm->Set(C_FOAM_CST_ULD_SAFETY_MOVE);
			}

			break;
		}
		case C_FOAM_CST_ULD_SAFETY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_SAFETY_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
				break;

			MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_CST_WORK_SAFETY + m_no);
			MtPosMove(mtFoamPkY, P_FOAM_PK_Y_SAFETY);

			if (!MT[mtFoamPkX].InPos(P_FOAM_PK_X_LT_CST_WORK_SAFETY + m_no) || !MT[mtFoamPkY].InPos(P_FOAM_PK_Y_SAFETY))
				break;

			m_pFsm->Set(C_FOAM_CST_ULD_RDY_ON);
			break;
		}
		case C_FOAM_CST_ULD_RDY_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_RDY_ON", m_strName);

			if (!IsOn(oPioRdy))
			{
				On(oPioRdy);
				FOAM_CST[m_no].m_pSLog->LogFncOut("CST_ULD_RDY_OUT_ON", "START", "$", "0", oPioRdy, "'ON'");
				break;
			}
			FOAM_CST[m_no].m_pSLog->LogFncOut("CST_ULD_RDY_OUT_ON", "END", "$", "0", oPioRdy, "'ON'");
			FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_WAIT", "START", "$", "100", iFoamBoxFoamExist, "'OFF'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_ULD_BUSY_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioBusy, 100))
				m_pFsm->Set(C_FOAM_CST_ULD_REQ_OFF);

			break;
		}
		case C_FOAM_CST_ULD_REQ_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_REQ_OFF", m_strName);

			if (m_pFsm->TimeLimit(_50sec))
			{
				if (AOn(iFoamBoxFoamExist))
					ER.Save((ErrName)(ER_FOAM_LT_CST_ULD_REQ_FOAM_EXIST_SENSOR_ON + m_no));

				if (AOn(iFoamBoxDetectLtFt) || AOn(iFoamBoxDetectRtFt) ||
					AOn(iFoamBoxDetectLtRr) || AOn(iFoamBoxDetectRtRr))
					ER.Save((ErrName)(ER_FOAM_LT_CST_ULD_REQ_FOAM_CST_DETECT_SENSOR_ON + m_no));

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (IsOn(oPioUldReq))
			{
				if (AOff(iFoamBoxFoamExist, 1000) &&
					AOff(iFoamBoxDetectLtFt, 1000) && AOff(iFoamBoxDetectRtFt, 1000) &&
					AOff(iFoamBoxDetectLtRr, 1000) && AOff(iFoamBoxDetectRtRr, 1000))
				{
					Off(oPioUldReq);
					FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_WAIT", "END", "$", "100", iFoamBoxFoamExist, "'OFF'");
					FOAM_CST[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_OFF", "START", "$", "0", oPioLdReq, "'OFF'");
				}
			}
			else
			{
				FOAM_CST[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_OFF", "END", "$", "0", oPioLdReq, "'OFF'");
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_ON_WAIT", "START", "$", "100", iPioComp, "'ON'");

				if (AOff(iPioBusy, 100))
					m_pFsm->Set(C_FOAM_CST_ULD_RDY_OFF);
			}
			break;
		}
		case C_FOAM_CST_ULD_RDY_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_RDY_OFF", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_ULD_COMP_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioComp, 100))
			{
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_ON_WAIT", "END", "$", "100", iPioComp, "'ON'");
				Off(oPioRdy);
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_OFF_WAIT", "START", "$", "100", iPioComp, "'OFF'");
				m_pFsm->Set(C_FOAM_CST_ULD_COMP_OFF);
			}

			break;
		}
		case C_FOAM_CST_ULD_COMP_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_COMP_OFF", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_FOAM_LT_CST_ULD_COMP_OFF_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iPioComp, 100))
			{
				FOAM_CST[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_OFF_WAIT", "END", "$", "100", iPioComp, "'OFF'");
				ZeroMemory(&FOAM_PART_DATA[nvFoamLtCst + m_no], sizeof(IPC_MMI::FoamPartData));
				m_pFsm->Set(C_FOAM_CST_ULD_END);
			}

			break;
		}
		case C_FOAM_CST_ULD_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_CST_ULD_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}