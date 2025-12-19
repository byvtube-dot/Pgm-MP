#include "..\..\Includes.h"

CMbbStack MBB_STACK[enMbbCstMax];

CMbbStack::CMbbStack()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CMbbStack::~CMbbStack()
{

}

void CMbbStack::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	int m_no = *reinterpret_cast<int*>(lpParam);
	if (oldValue == newValue)
		return;

	if ("CST_LOAD" == newValue)
		MBB_STACK[m_no].m_pSLog->XFR(newValue, "$", "AMR", "MBB_CST");
	else if ("CST_UNLOAD" == newValue)
		MBB_STACK[m_no].m_pSLog->XFR(newValue, "$", "MBB_CST", "AMR");
	else if ("LEVEL_CHECK" == newValue)
		MBB_STACK[m_no].m_pSLog->XFR(newValue, "$", "MBB_ELEV", "MBB_ELEV");
	else
		MBB_STACK[m_no].m_pSLog->XFR(newValue, "$", "", "");

}

void CMbbStack::Auto(void)
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

	// TODO: modify
	if (!ExistCst())
	{
		if (!IsCanCycleMbbCstLoad())
			return;

		m_pFsm->Set(C_MBB_CST_LD_START);
		return;
	}
	else
	{
		if (MBB_PART_DATA[nvMbbStack].flag.cstMaterialEmpty)
		{
			if (!IsCanCycleMbbCstUnload())
				return;

			m_pFsm->Set(C_MBB_CST_ULD_START);
			return;
		}
		else
		{
			if (!Exist() && MBB_PART_DATA[nvMbbStack].flag.inUseCst)
			{
				if (!IsCanCycleLevelCheck())
					return;

				m_pFsm->Set(C_LEVEL_CHECK_START);
				return;
			}
		}
	}
}

void CMbbStack::Cycle(void)
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

	CycleMbbCstLoad();
	CycleMbbCstUnload();
	CycleLevelCheck();
}

void CMbbStack::Init(int nIdx)
{
	m_no = nIdx;
	if (m_no == enMbbCstLt)
	{
		strcpy(m_strName, "MbbLtCst");
		m_pFsm = &FSM[fsmMbbLtCst];
		m_pRefFsm = &FSM[fsmMbbRtCst];

		m_pSLog = &SLOG[slogMbbLtCst];
		m_pSLog->SetXFRDeviceId("MBB_LT_CST");
		m_pSLog->SetXFRMaterialType("MBB_CST");
		m_pSLog->Add(mtMbbLtElevZ, TRUE);
		m_pSLog->Add(cylLtMbbCstFb, TRUE);

		nvMbbStack = nvLtMbbStack;

		MtZNum = mtMbbLtElevZ;

		m_pMbbCstFb = &CYL[cylLtMbbCstFb];

		iMbbCstExist = iLtMbbCstExist;
		iMbbCstDetectLt = iLtMbbCstDetectLt;
		iMbbCstDetectRt = iLtMbbCstDetectRt;
		iMbbCstFtLevel = iLtMbbCstFtLevel;
		iMbbCstMidLevel = iLtMbbCstMidLevel;
		iMbbCstRrLevel = iLtMbbCstRrLevel;

		iPioValId = iMbbLtCstPioValId;
		iPioCs0 = iMbbLtCstPioCs0;
		iPioCs1 = iMbbLtCstPioCs1;
		iPioCs2 = iMbbLtCstPioCs2;
		iPioCs3 = iMbbLtCstPioCs3;
		iPioTrReq = iMbbLtCstPioTrReq;
		iPioBusy = iMbbLtCstPioBusy;
		iPioComp = iMbbLtCstPioComp;

		oPioLdReq = oMbbLtCstPioLdReq;
		oPioUldReq = oMbbLtCstPioUldReq;
		oPioAbort = oMbbLtCstPioAbort;
		oPioRdy = oMbbLtCstPioRdy;

	}
	else
	{
		strcpy(m_strName, "MbbRtCst");
		m_pFsm = &FSM[fsmMbbRtCst];
		m_pRefFsm = &FSM[fsmMbbLtCst];

		m_pSLog = &SLOG[slogMbbRtCst];
		m_pSLog->SetXFRDeviceId("MBB_RT_CST");
		m_pSLog->SetXFRMaterialType("MBB_CST");
		m_pSLog->Add(mtMbbRtElevZ, TRUE);
		m_pSLog->Add(cylRtMbbCstFb, TRUE);

		nvMbbStack = nvRtMbbStack;

		MtZNum = mtMbbRtElevZ;

		m_pMbbCstFb = &CYL[cylRtMbbCstFb];

		iMbbCstExist = iRtMbbCstExist;
		iMbbCstDetectLt = iRtMbbCstDetectLt;
		iMbbCstDetectRt = iRtMbbCstDetectRt;
		iMbbCstFtLevel = iRtMbbCstFtLevel;
		iMbbCstMidLevel = iRtMbbCstMidLevel;
		iMbbCstRrLevel = iRtMbbCstRrLevel;

		iPioValId = iMbbRtCstPioValId;
		iPioCs0 = iMbbRtCstPioCs0;
		iPioCs1 = iMbbRtCstPioCs1;
		iPioCs2 = iMbbRtCstPioCs2;
		iPioCs3 = iMbbRtCstPioCs3;
		iPioTrReq = iMbbRtCstPioTrReq;
		iPioBusy = iMbbRtCstPioBusy;
		iPioComp = iMbbRtCstPioComp;

		oPioLdReq = oMbbRtCstPioLdReq;
		oPioUldReq = oMbbRtCstPioUldReq;
		oPioAbort = oMbbRtCstPioAbort;
		oPioRdy = oMbbRtCstPioRdy;
	}
}

BOOL CMbbStack::Common(void)
{
	if (MBB_PART_DATA[nvLtMbbStack].flag.existCst && MBB_PART_DATA[nvRtMbbStack].flag.existCst)
	{
		if (!MBB_PART_DATA[nvLtMbbStack].flag.inUseCst && !MBB_PART_DATA[nvLtMbbStack].flag.cstMaterialEmpty &&
			!MBB_PART_DATA[nvRtMbbStack].flag.inUseCst && !MBB_PART_DATA[nvRtMbbStack].flag.cstMaterialEmpty)
			MBB_PART_DATA[nvLtMbbStack].flag.inUseCst = true;
		else
		{
			if (MBB_PART_DATA[nvLtMbbStack].flag.cstMaterialEmpty)
			{
				MBB_PART_DATA[nvLtMbbStack].flag.inUseCst = false;

				if (MBB_PART_DATA[nvRtMbbStack].flag.existCst &&
					!MBB_PART_DATA[nvRtMbbStack].flag.inUseCst &&
					!MBB_PART_DATA[nvRtMbbStack].flag.cstMaterialEmpty)
				{
					MBB_PART_DATA[nvRtMbbStack].flag.inUseCst = true;
				}
			}

			if (MBB_PART_DATA[nvRtMbbStack].flag.cstMaterialEmpty)
			{
				MBB_PART_DATA[nvRtMbbStack].flag.inUseCst = false;
				if (MBB_PART_DATA[nvLtMbbStack].flag.existCst &&
					!MBB_PART_DATA[nvLtMbbStack].flag.inUseCst &&
					!MBB_PART_DATA[nvLtMbbStack].flag.cstMaterialEmpty)
				{
					MBB_PART_DATA[nvLtMbbStack].flag.inUseCst = true;
				}
			}
		}
	}
	else if (MBB_PART_DATA[nvLtMbbStack].flag.existCst)
	{
		if (MBB_PART_DATA[nvLtMbbStack].flag.cstMaterialEmpty)
			MBB_PART_DATA[nvLtMbbStack].flag.inUseCst = false;
		else if (AOn(iLtMbbCstExist))
			MBB_PART_DATA[nvLtMbbStack].flag.inUseCst = true;
	}
	else if (MBB_PART_DATA[nvRtMbbStack].flag.existCst)
	{
		if (MBB_PART_DATA[nvRtMbbStack].flag.cstMaterialEmpty)
			MBB_PART_DATA[nvRtMbbStack].flag.inUseCst = false;
		else if (AOn(iRtMbbCstExist))
			MBB_PART_DATA[nvRtMbbStack].flag.inUseCst = true;
	}

	if (!ExistCst())
	{
		ZeroMemory(&MBB_PART_DATA[nvLtMbbStack + m_no], sizeof(IPC_MMI::MbbPartData));
		if (NV.use[useMbbCstAmr] && !MBB_PART_DATA[nvLtMbbStack + m_no].flag.amrLoadCallDid)
			MBB_PART_DATA[nvLtMbbStack + m_no].flag.amrLoadCallDid = true;
	}
	else
	{
		if (NV.use[useMbbCstAmr] &&
			MBB_PART_DATA[nvLtMbbStack + m_no].flag.cstMaterialEmpty && !MBB_PART_DATA[nvLtMbbStack + m_no].flag.amrUnloadCallDid)
			MBB_PART_DATA[nvLtMbbStack + m_no].flag.amrUnloadCallDid = true;
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
			ER.Save((ErrName)(ER_MBB_LT_CST_PIO_CS_ABNORMAL + m_no));
	}
	return TRUE;
}

BOOL CMbbStack::IsHwErr(void)
{
	if (!MT[MtZNum].m_state.isHome)
		return TRUE;

	if (m_pMbbCstFb->GetErr())
		return TRUE;

	return FALSE;
}

BOOL CMbbStack::IsHwRdy(void)
{
	//if (!MT[MtZNum].IsRdy())
	//	return TRUE;

	return FALSE;
}

BOOL CMbbStack::CylInit(void)
{
	BOOL isCylInitDone = TRUE;

	if (!m_pMbbCstFb->IsBwd())
	{
		if (!MtPosMove(MtZNum, P_ELEV_Z_BTM))
			return FALSE;

		m_pMbbCstFb->Bwd(LT_MBB_CST_BWD);
		isCylInitDone = FALSE;
	}

	return isCylInitDone;
}

BOOL CMbbStack::IsCanCycleLevelCheck(void)
{
	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_VI_START, RT_COBOT.C_MBB_PICK_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"RtCobot Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbStack::IsCanCycleMbbCstLoad(void)
{
	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Cycle is Busy");
		return FALSE;
	}

	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_VI_START, RT_COBOT.C_MBB_PICK_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"RtCobot Cycle is Busy");
		return FALSE;
	}

	if (ExistCst())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"MBB Cst Exist");
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

BOOL CMbbStack::IsCanCycleMbbCstUnload(void)
{
	if (m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"m_pRefFsm Cycle is Busy");
		return FALSE;
	}

	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_VI_START, RT_COBOT.C_MBB_PICK_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"RtCobot Cycle is Busy");
		return FALSE;
	}

	if (!ExistCst())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"MBB Cst Empty");
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

BOOL CMbbStack::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy())
	{
		if (!ExistCst() && AOn(iMbbCstExist))
		{
			ER.Save((ErrName) (ER_MBB_LT_STACK_SENSOR_STATUS + (m_no * 2)));
			return TRUE;
		}
		if (ExistCst() && AOff(iMbbCstExist))
		{
			ER.Save((ErrName)(ER_MBB_LT_STACK_MEMORY_STATUS + (m_no * 2)));
			return TRUE;
		}
	}

	return FALSE;
}

bool& CMbbStack::ExistCst(void)
{
	return MBB_PART_DATA[nvMbbStack].flag.existCst;
}

int& CMbbStack::Exist(void)
{
	return MBB_PART_DATA[nvMbbStack].exist;
}

BOOL CMbbStack::IsExistCst(void)
{
	if (!ExistCst() || AOff(iMbbCstExist))
	{
		ER.Save((ErrName)(ER_MBB_LT_STACK_CST_EXIST + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbStack::IsEmptyCst(void)
{
	if (ExistCst() || AOn(iMbbCstExist))
	{
		ER.Save((ErrName) (ER_MBB_LT_STACK_CST_EMPTY + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

void CMbbStack::CycleMbbCstLoad(void)
{
	if (!m_pFsm->Between(C_MBB_CST_LD_START, C_MBB_CST_LD_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save((ErrName)(ER_MBB_LT_CST_LOAD_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsg = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_MBB_CST_LD_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "CST_LOAD";
			}

			if (!IsEmptyCst())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			// TODO: Check safety conditions before proceeding to the next action

			m_pFsm->Set(C_MBB_CST_LD_REQ_ON);
			break;
		}
		case C_MBB_CST_LD_REQ_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_REQ_ON", m_strName);

			if (!IsOn(oPioLdReq))
			{
				On(oPioLdReq);
				MBB_STACK[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_ON", "START", "$", "0", oPioLdReq, "'ON'");
				break;
			}
			MBB_STACK[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_ON", "END", "$", "0", oPioLdReq, "'ON'");
			MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_REQ_IN_ON_WAIT", "START", "$", "100", iPioTrReq, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_LD_REQ_ON_TIME_OVER + nMsg));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioTrReq, 100))
			{
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_REQ_IN_ON_WAIT", "END", "$", "100", iPioTrReq, "'ON'");
				m_pFsm->Set(C_MBB_CST_LD_SAFETY_MOVE);
			}

			break;
		}
		case C_MBB_CST_LD_SAFETY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_SAFETY_MOVE", m_strName);

			if (!ROBOT[robotRt].TcpInPos(P_RT_COBOT_READY))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_READY))
					ROBOT[robotRt].Move(P_RT_COBOT_READY);

				break;
			}

			m_pFsm->Set(C_MBB_CST_LD_RDY_ON);
			break;
		}
		case C_MBB_CST_LD_RDY_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_RDY_ON", m_strName);

			if (!IsOn(oPioRdy))
			{
				On(oPioRdy);
				MBB_STACK[m_no].m_pSLog->LogFncOut("CST_LD_RDY_OUT_ON", "START", "$", "0", oPioRdy, "'ON'");
				break;
			}
			MBB_STACK[m_no].m_pSLog->LogFncOut("CST_LD_RDY_OUT_ON", "END", "$", "0", oPioRdy, "'ON'");
			MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_WAIT", "START", "$", "100", iMbbCstExist, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_LD_BUSY_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioBusy, 100))
				m_pFsm->Set(C_MBB_CST_LD_REQ_OFF);

			break;
		}
		case C_MBB_CST_LD_REQ_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_REQ_OFF", m_strName);

			if (m_pFsm->TimeLimit(_50sec))
			{
				if (AOff(iMbbCstExist))
					ER.Save((ErrName)(ER_MBB_LT_CST_LD_REQ_MBB_EXIST_SENSOR_OFF + m_no));

				if (AOff(iMbbCstDetectLt) || AOff(iMbbCstDetectRt))
					ER.Save((ErrName)(ER_MBB_LT_CST_LD_REQ_MBB_CST_DETECT_SENSOR_OFF + m_no));

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (IsOn(oPioLdReq))
			{
				if (AOn(iMbbCstExist, 1000) && 
					AOn(iMbbCstDetectLt, 1000) && AOn(iMbbCstDetectRt, 1000))
				{
					Off(oPioLdReq);
					MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_WAIT", "END", "$", "100", iMbbCstExist, "'ON'");
					MBB_STACK[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_OFF", "START", "$", "0", oPioLdReq, "'OFF'");
				}
			}
			else
			{
				MBB_STACK[m_no].m_pSLog->LogFncOut("CST_LD_REQ_OUT_OFF", "END", "$", "0", oPioLdReq, "'OFF'");
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_COMP_ON_WAIT", "START", "$", "100", iPioComp, "'ON'");

				if (AOff(iPioBusy, 100))
					m_pFsm->Set(C_MBB_CST_LD_RDY_OFF);
			}

			break;
		}
		case C_MBB_CST_LD_RDY_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_RDY_OFF", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_LD_COMP_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioComp, 100))
			{
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_COMP_ON_WAIT", "END", "$", "100", iPioComp, "'ON'");
				Off(oPioRdy);
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_COMP_OFF_WAIT", "START", "$", "100", iPioComp, "'OFF'");
				m_pFsm->Set(C_MBB_CST_LD_COMP_OFF);
			}

			break;
		}
		case C_MBB_CST_LD_COMP_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_COMP_OFF", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_LD_COMP_OFF_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iPioComp, 100))
			{
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_LD_COMP_OFF_WAIT", "END", "$", "100", iPioComp, "'OFF'");
				ZeroMemory(&MBB_PART_DATA[nvMbbStack], sizeof(IPC_MMI::MbbPartData));
				MBB_PART_DATA[nvMbbStack].flag.existCst = true;

				m_pFsm->Set(C_MBB_CST_LD_RFID_READ);
			}

			break;
		}
		case C_MBB_CST_LD_RFID_READ:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_RFID_READ", m_strName);
				NV.ndm[commEqpLtMbbStackRfidRead + m_no] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_LD_RFID_READ_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[commEqpLtMbbStackRfidRead + m_no] != COMM_COMP)
				break;

			m_pFsm->Set(C_MBB_CST_LD_END);
			break;
		}
		case C_MBB_CST_LD_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_LD_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbStack::CycleMbbCstUnload(void)
{
	if (!m_pFsm->Between(C_MBB_CST_ULD_START, C_MBB_CST_ULD_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save((ErrName)(ER_MBB_LT_CST_UNLOAD_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsg = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_MBB_CST_ULD_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "CST_UNLOAD";
			}

			if (!IsExistCst())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_MBB_CST_ULD_REQ_ON);
			break;
		}
		case C_MBB_CST_ULD_REQ_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_REQ_ON", m_strName);

			if (!IsOn(oPioUldReq))
			{
				On(oPioUldReq);
				MBB_STACK[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_ON", "START", "$", "0", oPioUldReq, "'ON'");
				break;
			}
			MBB_STACK[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_ON", "END", "$", "0", oPioUldReq, "'ON'");
			MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_REQ_IN_ON_WAIT", "START", "$", "100", iPioTrReq, "'ON'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_ULD_REQ_ON_TIME_OVER + nMsg));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioTrReq, 100))
			{
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_REQ_IN_ON_WAIT", "END", "$", "100", iPioTrReq, "'ON'");
				m_pFsm->Set(C_MBB_CST_ULD_SAFETY_MOVE);
			}

			break;
		}
		case C_MBB_CST_ULD_SAFETY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_SAFETY_MOVE", m_strName);

			if (!ROBOT[robotRt].TcpInPos(P_RT_COBOT_READY))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_READY))
					ROBOT[robotRt].Move(P_RT_COBOT_READY);

				break;
			}

			m_pFsm->Set(C_MBB_CST_ULD_RDY_ON);
			break;
		}
		case C_MBB_CST_ULD_RDY_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_RDY_ON", m_strName);

			if (!IsOn(oPioRdy))
			{
				On(oPioRdy);
				MBB_STACK[m_no].m_pSLog->LogFncOut("CST_ULD_RDY_OUT_ON", "START", "$", "0", oPioRdy, "'ON'");
				break;
			}
			MBB_STACK[m_no].m_pSLog->LogFncOut("CST_ULD_RDY_OUT_ON", "END", "$", "0", oPioRdy, "'ON'");
			MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_WAIT", "START", "$", "100", iMbbCstExist, "'OFF'");

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_ULD_BUSY_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioBusy, 100))
				m_pFsm->Set(C_MBB_CST_ULD_REQ_OFF);

			break;
		}
		case C_MBB_CST_ULD_REQ_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_REQ_OFF", m_strName);

			if (m_pFsm->TimeLimit(_50sec))
			{
				if (AOn(iMbbCstExist))
					ER.Save((ErrName)(ER_MBB_LT_CST_ULD_REQ_MBB_EXIST_SENSOR_ON + m_no));

				if (AOn(iMbbCstDetectLt) || AOn(iMbbCstDetectRt))
					ER.Save((ErrName)(ER_MBB_LT_CST_ULD_REQ_MBB_CST_DETECT_SENSOR_ON + m_no));

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (IsOn(oPioUldReq))
			{
				if (AOff(iMbbCstExist, 1000) && 
					AOff(iMbbCstDetectLt, 1000) && AOff(iMbbCstDetectRt, 1000))
				{
					Off(oPioUldReq);
					MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_WAIT", "END", "$", "100", iMbbCstExist, "'OFF'");
					MBB_STACK[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_OFF", "START", "$", "0", oPioLdReq, "'OFF'");
				}
			}
			else
			{
				MBB_STACK[m_no].m_pSLog->LogFncOut("CST_ULD_REQ_OUT_OFF", "END", "$", "0", oPioLdReq, "'OFF'");
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_ON_WAIT", "START", "$", "100", iPioComp, "'ON'");

				if (AOff(iPioBusy, 100))
					m_pFsm->Set(C_MBB_CST_ULD_RDY_OFF);
			}

			break;
		}
		case C_MBB_CST_ULD_RDY_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_RDY_OFF", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_ULD_COMP_ON_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iPioComp, 100))
			{
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_ON_WAIT", "END", "$", "100", iPioComp, "'ON'");
				Off(oPioRdy);
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_OFF_WAIT", "START", "$", "100", iPioComp, "'OFF'");
				m_pFsm->Set(C_MBB_CST_ULD_COMP_OFF);
			}

			break;
		}
		case C_MBB_CST_ULD_COMP_OFF:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_COMP_OFF", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save((ErrName)(ER_MBB_LT_CST_ULD_COMP_OFF_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOff(iPioComp, 100))
			{
				MBB_STACK[m_no].m_pSLog->LogFncIn("CST_ULD_COMP_OFF_WAIT", "END", "$", "100", iPioComp, "'OFF'");
				ZeroMemory(&MBB_PART_DATA[nvMbbStack], sizeof(IPC_MMI::MbbPartData));
				m_pFsm->Set(C_MBB_CST_ULD_END);
			}

			// TODO: RFID Read Step ADD.
			break;
		}
		case C_MBB_CST_ULD_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_CST_ULD_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbStack::CycleLevelCheck(void)
{
	if (!m_pFsm->Between(C_LEVEL_CHECK_START, C_LEVEL_CHECK_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save((ErrName)(ER_MBB_LT_ELEV_LEVEL_CHECK_CYCLE_TIME_OVER + m_no));
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
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_START %02d", m_strName, MBB_PART_DATA[nvMbbStack].cnt);
				m_tmCycleTime.Reset();
				m_state = "LEVEL_CHECK";
				m_nLevelCheckCnt = 0;
			}

			if (AOff(iMbbCstExist))
			{
				MBB_PART_DATA[nvMbbStack].flag.cstMaterialEmpty = true;
				m_pFsm->Set(C_IDLE);
				break;
			}

			if (!CylInit())
			{
				MtPosMove(MtZNum, P_ELEV_Z_BTM);
				break;
			}

			if (AOn(iMbbCstMidLevel) || AOn(iMbbCstRrLevel))
				m_pFsm->Set(C_LEVEL_CHECK_Z_DN);
			else
				m_pFsm->Set(C_LEVEL_CHECK_Z_UP);

			break;
		}
		case C_LEVEL_CHECK_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_DN", m_strName);

			MtPosMove(MtZNum, P_ELEV_Z_BTM);

			if (MT[MtZNum].m_state.isDriving || MT_INPOS == MtPosMove(MtZNum, P_ELEV_Z_BTM))
				m_pFsm->Set(C_LEVEL_CHECK_Z_DN_CHECK);

			break;
		}
		case C_LEVEL_CHECK_Z_DN_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_DN_CHECK", m_strName);

			if (AOff(iMbbCstMidLevel, (DWORD)NV.rcp[mbbElevLevelDnDelay]) &&
				AOff(iMbbCstRrLevel, (DWORD)NV.rcp[mbbElevLevelDnDelay]))
			{
				if (!MT[MtZNum].IsRdy())
				{
					MT[MtZNum].Stop();
					break;
				}
				m_pFsm->Set(C_LEVEL_CHECK_Z_UP);
			}
			else
			{
				if (MT[MtZNum].InPos(P_ELEV_Z_BTM))
					m_pFsm->Set(C_LEVEL_CHECK_END);
			}
			break;
		}
		case C_LEVEL_CHECK_Z_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_Z_UP", m_strName);

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

			if ((AOn(iMbbCstMidLevel, (DWORD)NV.rcp[mbbElevLevelUpDelay]) &&
				AOn(iMbbCstRrLevel, (DWORD)NV.rcp[mbbElevLevelUpDelay])) ||
				(OPR.isDryRun && AOn(iMbbCstMidLevel) && AOn(iMbbCstRrLevel)))
			{
				if (!MT[MtZNum].IsRdy())
				{
					MT[MtZNum].Stop();
					break;
				}

				m_nLevelCheckCnt++;
				if (1 < m_nLevelCheckCnt)
					m_pFsm->Set(C_LEVEL_CHECK_END);
				else
					m_pFsm->Set(C_LEVEL_CHECK_Z_DN);
			}
			else
			{
				if (MT[MtZNum].InPos(P_ELEV_Z_TOP))
					m_pFsm->Set(C_LEVEL_CHECK_END);
			}
			break;
		}
		case C_LEVEL_CHECK_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LEVEL_CHECK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			if (!MT[MtZNum].IsRdy())
				break;

			if (AOff(iMbbCstExist))
				MBB_PART_DATA[nvMbbStack].flag.cstMaterialEmpty = true;
			else
				Exist() = enMbbExist;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}