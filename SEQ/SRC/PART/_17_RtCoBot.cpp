#include "..\..\Includes.h"

CRtCobot RT_COBOT;

CRtCobot::CRtCobot()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CRtCobot::~CRtCobot()
{

}

void CRtCobot::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = "$";
	if ("LABEL_PICKUP" == newValue)
	{
		int nDir;
		if (LABEL_PART_DATA[nvLabelLtSuttle].exist)
			nDir = _LT;
		else if (LABEL_PART_DATA[nvLabelRtSuttle].exist)
			nDir = _RT;
		else
			return;

		MatId = static_cast<string> (LABEL_PART_DATA[nvLabelLtSuttle + nDir].labelInfo.sBoxId);
		if (0 == strcmp(LABEL_PART_DATA[nvLabelLtSuttle + nDir].labelInfo.sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "LABEL_SHUTTLE", "RT_COBOT");
	}
	else if ("MBB_VI" == newValue)
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "RT_COBOT_VI", "RT_COBOT_VI");
	else if ("LABEL_ATTACH" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL].sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "RT_COBOT", "MBB");
	}
	else if ("LABEL_ATTACH_VI" == newValue)
	{
		int nDir;
		if (MBB_PART_DATA[nvLtMbbStack].exist)
			nDir = _LT;
		else if (MBB_PART_DATA[nvRtMbbStack].exist)
			nDir = _RT;
		else
			return;

		MatId = static_cast<string> (MBB_PART_DATA[nvLtMbbStack + nDir].labelInfo.sBoxId);
		if (0 == strcmp(MBB_PART_DATA[nvLtMbbStack + nDir].labelInfo.sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "RT_COBOT_VI", "RT_COBOT_VI");
	}
	else if ("MBB_PICKUP" == newValue)
	{
		int nDir;
		if (MBB_PART_DATA[nvLtMbbStack].exist)
			nDir = _LT;
		else if (MBB_PART_DATA[nvRtMbbStack].exist)
			nDir = _RT;
		else
			return;

		MatId = static_cast<string> (MBB_PART_DATA[nvLtMbbStack + nDir].labelInfo.sBoxId);
		if (0 == strcmp(MBB_PART_DATA[nvLtMbbStack + nDir].labelInfo.sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "MBB_ELEV", "RT_COBOT");
	}
	else if ("MBB_PLACE" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB].sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "RT_COBOT", "MBB_SHUTTLE");
	}
	else if ("LABEL_REJECT" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL].sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "RT_COBOT", "LABEL_REJECT");
	}
	else if ("MBB_REJECT" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB].sBoxId, ""))
			MatId = "$";
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "RT_COBOT", "MBB_REJECT");
	}
	else
	{
		RT_COBOT.m_pSLog->XFR(newValue, MatId, "", "");
	}
}

void CRtCobot::Auto(void)
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

	if (Exist(_MBB))
	{
		if (Exist(_MBB) == enMbbExistLabelQc)
		{
			if (IsCanCycleMbbPlace() && !MBB_SHUTTLE.Exist())
			{
				m_pFsm->Set(C_MBB_PLACE_START);
				return;
			}
		}
	}
	else
	{
		if (MBB_PART_DATA[nvLtMbbStack].exist == enMbbExistLabelQc ||
			MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistLabelQc)
		{
			if (IsCanCycleMbbPick())
			{
				int nMsgDir = _LT;
				if (MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistLabelQc)
					nMsgDir = _RT;

				m_pFsm->Set(C_MBB_PICK_START, nMsgDir);
				return;
			}
		}

		if (Exist(_LABEL))
		{
			if (MBB_PART_DATA[nvLtMbbStack].exist == enMbbExist ||
				MBB_PART_DATA[nvRtMbbStack].exist == enMbbExist)
			{
				if (IsCanCycleMbbVi())
				{
					int nMsgDir = _LT;
					if (MBB_PART_DATA[nvRtMbbStack].exist == enMbbExist)
						nMsgDir = _RT;

					m_pFsm->Set(C_MBB_VI_START, nMsgDir);
					return;
				}
			}

			if (MBB_PART_DATA[nvLtMbbStack].exist == enMbbExistQc ||
				MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistQc)
			{
				if (IsCanCycleLabelAttach())
				{
					int nMsgDir = _LT;
					if (MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistQc)
						nMsgDir = _RT;

					m_pFsm->Set(C_LABEL_ATTACH_START, nMsgDir);
					return;
				}
			}
		}
		else
		{
			if (MBB_PART_DATA[nvLtMbbStack].exist == enMbbExistLabel ||
				MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistLabel)
			{
				if (IsCanCycleLabelAttachVi())
				{
					int nMsgDir = _LT;
					if (MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistLabel)
						nMsgDir = _RT;

					m_pFsm->Set(C_LABEL_ATTACH_VI_START, nMsgDir);
					return;
				}
			}
			else if (MBB_PART_DATA[nvLtMbbStack].exist || MBB_PART_DATA[nvRtMbbStack].exist)
			{
				if (LABEL_SHUTTLE[enlabelLt].Exist() == enLabelExistQc ||
					LABEL_SHUTTLE[enlabelRt].Exist() == enLabelExistQc)
				{
					if (IsCanCycleLabelPick())
					{
						int nMsgDir;
						if (LABEL_SHUTTLE[enlabelLt].Exist() && !LABEL_PART_DATA[enlabelLt].flag.lastlabel)
							nMsgDir = _LT;
						else if (LABEL_SHUTTLE[enlabelRt].Exist() && !LABEL_PART_DATA[enlabelRt].flag.lastlabel)
							nMsgDir = _RT;
						else
							return;

						m_pFsm->Set(C_LABEL_PICK_START, nMsgDir);
					}

					return;
				}
			}
		}
	}
}

void CRtCobot::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
		{
			Off(oSolLtMbbCstAirBlow);
			Off(oSolRtMbbCstAirBlow);
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleLabelPick();
	CycleMbbVi();
	CycleLabelAttach();
	CycleLabelAttachVi();
	CycleMbbPick();
	CycleMbbPlace();
	CycleLabelReject();
	CycleMbbReject();
}

void CRtCobot::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "RtCobot");
	m_pFsm = &FSM[fsmRtCobot];

	m_pSLog = &SLOG[slogRtCobot];
	m_pSLog->SetXFRDeviceId("RT_COBOT");
	m_pSLog->SetXFRMaterialType("MBB");

	// TODO: Cobot Info Set
	m_pSLog->Add(robotRt, TRUE);
	m_pSLog->Add(cylRtCobotLabelPkUd, TRUE);
	m_pSLog->Add(cylRtCobotLabelPkTurnFb, TRUE);

	m_pSLog->Add(vcRtCobotLabel, TRUE);
	m_pSLog->Add(vcRtCobotMbb, TRUE);
	m_pSLog->Add(vcRtCobotMbb2, TRUE);

	m_pSLog->Add(VI_RT_COBOT, TRUE);
}

BOOL CRtCobot::Common(void)
{
	bool isBusy = false;
	bool isMoving = false;

	// Right Cobot 동작 중일때, Label Shuttle Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_LABEL_PICK_MOVE, C_LABEL_PICK_END);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_LABEL_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_LABEL_PICK_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[rtCobotLabelShuttleCanMove] = FALSE;
	else
		NV.ndm[rtCobotLabelShuttleCanMove] = TRUE;

	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_MBB_VI_START, C_MBB_PICK_END);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_VI);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_VI_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_MBB_VI_TO_LABEL_ATTACH_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_VI);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_VI_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_MBB_VI_TO_LABEL_ATTACH_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_ATTACH_VI);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH_TO_ATTACH_VI);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI_TO_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_MBB_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_MBB_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_MBB_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_MBB_PICK_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[rtCobotMbbElevCanMove] = FALSE;
	else
		NV.ndm[rtCobotMbbElevCanMove] = TRUE;

	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END);
	isBusy = isBusy || m_pFsm->Between(C_MBB_REJECT_START, C_MBB_REJECT_END);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_LABEL_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_LABEL_PICK_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[rtCobotLtCobotLabelPickCanMove] = FALSE;
	else
		NV.ndm[rtCobotLtCobotLabelPickCanMove] = TRUE;

	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END);
	isBusy = isBusy || m_pFsm->Between(C_MBB_PLACE_START, C_MBB_PLACE_END);
	isBusy = isBusy || m_pFsm->Between(C_MBB_REJECT_START, C_MBB_REJECT_END);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_LT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_LT_LABEL_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_RT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_RT_LABEL_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_MBB_PLACE);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_MBB_PLACE_TO_READY);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_MBB_REJECT);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_MBB_REJECT_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[rtCobotLtCobotRejectCanMove] = FALSE;
	else
		NV.ndm[rtCobotLtCobotRejectCanMove] = TRUE;

	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_MBB_PLACE_MOVE, C_MBB_PLACE_END);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurIdx(P_RT_COBOT_MBB_PLACE);
	isMoving = isMoving || ROBOT[robotRt].CmdOrCurMovingIdx(P_RT_COBOT_MBB_PLACE_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[rtCobotMbbShuttleCanMove] = FALSE;
	else
		NV.ndm[rtCobotMbbShuttleCanMove] = TRUE;

	return TRUE;
}

BOOL CRtCobot::IsHwErr(void)
{
	if (CYL[cylRtCobotLabelPkUd].GetErr())
		return TRUE;
	if (CYL[cylRtCobotLabelPkTurnFb].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CRtCobot::IsHwRdy(void)
{
	if (!ROBOT[robotRt].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CRtCobot::CylInit(void)
{
	if (!CYL[cylRtCobotLabelPkUd].IsUp())
		CYL[cylRtCobotLabelPkUd].Up(RT_COBOT_LABEL_PK_UP);

	if (!CYL[cylRtCobotLabelPkTurnFb].IsBwd())
		CYL[cylRtCobotLabelPkTurnFb].Bwd(RT_COBOT_LABEL_PK_TURN_BWD);

	if (!CYL[cylRtCobotLabelPkUd].IsUp() || !CYL[cylRtCobotLabelPkTurnFb].IsBwd())
		return FALSE;

	return TRUE;
}

BOOL CRtCobot::IsCanCycleLabelPick(void)
{
	if (LABEL_SHUTTLE[enlabelLt].m_pFsm->IsBusy() || LABEL_SHUTTLE[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}

	if (!GetLabelPickup())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Rt Cobot No required labels available.");
		return FALSE;
	}

	if (!NV.ndm[ltCobotRtCobotRejectCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Lt Cobot is not in safety position");
		return FALSE;
	}

	return TRUE;
}

BOOL CRtCobot::IsCanCycleMbbVi(void)
{
	if (MBB_STACK[enMbbCstLt].m_pFsm->IsBusy() || MBB_STACK[enMbbCstRt].m_pFsm->IsBusy() ||
		AOn(MBB_STACK[enMbbCstLt].iPioValId) || AOn(MBB_STACK[enMbbCstRt].iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Stack Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CRtCobot::IsCanCycleLabelAttach(void)
{
	if (MBB_STACK[enMbbCstLt].m_pFsm->IsBusy() || MBB_STACK[enMbbCstRt].m_pFsm->IsBusy() ||
		AOn(MBB_STACK[enMbbCstLt].iPioValId) || AOn(MBB_STACK[enMbbCstRt].iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Stack Cycle is Busy");
		return FALSE;
	}

	if (MBB_PART_DATA[nvLtMbbStack].exist != enMbbExistQc && MBB_PART_DATA[nvRtMbbStack].exist != enMbbExistQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Stack Memory Mismatch");
		return FALSE;
	}

	return TRUE;
}

BOOL CRtCobot::IsCanCycleLabelAttachVi(void)
{
	if (MBB_STACK[enMbbCstLt].m_pFsm->IsBusy() || MBB_STACK[enMbbCstRt].m_pFsm->IsBusy() ||
		AOn(MBB_STACK[enMbbCstLt].iPioValId) || AOn(MBB_STACK[enMbbCstRt].iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Stack Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CRtCobot::IsCanCycleMbbPick(void)
{
	if (MBB_STACK[enMbbCstLt].m_pFsm->IsBusy() || MBB_STACK[enMbbCstRt].m_pFsm->IsBusy() ||
		AOn(MBB_STACK[enMbbCstLt].iPioValId) || AOn(MBB_STACK[enMbbCstRt].iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Stack Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CRtCobot::IsCanCycleMbbPlace(void)
{
	if (MBB_SHUTTLE.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Shuttle Cycle is Busy");
		return FALSE;
	}

	if (!NV.ndm[ltCobotRtCobotMbbPlaceCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Lt Cobot Cycle is Busy");
		return FALSE;
	}

	// TODO: 25.09.30
	//if (MBB_QC.m_pFsm->Between(MBB_QC.C_MBB_QC_VI_START, MBB_QC.C_MBB_QC_VI_CLAMP))
	//{
	//	MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
	//		"MBB QC Cycle is Busy");
	//	return FALSE;
	//}

	return TRUE;
}

BOOL CRtCobot::IsCanCycleLabelReject(void)
{
	return TRUE;
}

BOOL CRtCobot::IsCanCycleMbbReject(void)
{
	if (!NV.ndm[ltCobotRtCobotRejectCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Lt Cobot Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CRtCobot::ExistCrossCheck(void)
{
	if (!m_pFsm->Between(C_MBB_PICK_START, C_MBB_PICK_END) && 
		!m_pFsm->Between(C_MBB_PLACE_START, C_MBB_PLACE_END) &&
		!m_pFsm->Between(C_MBB_REJECT_START, C_MBB_REJECT_END))
	{
		if (!Exist(_MBB) && (VC[vcRtCobotMbb].AOn() || VC[vcRtCobotMbb2].AOn()))
		{
			ER.Save(ER_RT_COBOT_MBB_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist(_MBB) && (VC[vcRtCobotMbb].AOff() || VC[vcRtCobotMbb2].AOff()))
		{
			ER.Save(ER_RT_COBOT_MBB_MEMORY_STATUS);
			return TRUE;
		}
	}

	if (!m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END) &&
		!m_pFsm->Between(C_LABEL_ATTACH_START, C_LABEL_ATTACH_END) &&
		!m_pFsm->Between(C_LABEL_REJECT_START, C_LABEL_REJECT_END))
	{
		if (!Exist(_LABEL) && VC[vcRtCobotLabel].AOn())
		{
			ER.Save(ER_RT_COBOT_LABEL_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist(_LABEL) && VC[vcRtCobotLabel].AOff())
		{
			ER.Save(ER_RT_COBOT_LABEL_MEMORY_STATUS);
			return TRUE;
		}
	}
	return FALSE;
}

int& CRtCobot::Exist(int materialType)
{
	if (materialType == _MBB)	// Mbb
		return COBOT_PART_DATA[nvRtCobot].exist;
	else						// Label
		return COBOT_PART_DATA[nvRtCobot].existLabel;
}

BOOL CRtCobot::IsExist(int materialType)
{
	if (materialType == _MBB)
	{
		if (!COBOT_PART_DATA[nvRtCobot].exist || VC[vcRtCobotMbb].AOff() || VC[vcRtCobotMbb2].AOff())
		{
			ER.Save(ER_RT_COBOT_MBB_EMPTY);
			return FALSE;
		}
	}
	else
	{
		if (!COBOT_PART_DATA[nvRtCobot].existLabel || VC[vcRtCobotLabel].AOff())
		{
			ER.Save(ER_RT_COBOT_LABEL_EMPTY);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CRtCobot::IsEmpty(int materialType)
{
	if (materialType == _MBB)
	{
		if (COBOT_PART_DATA[nvRtCobot].exist || VC[vcRtCobotMbb].AOn() || VC[vcRtCobotMbb2].AOn())
		{
			ER.Save(ER_RT_COBOT_MBB_EXIST);
			return FALSE;
		}
	}
	else
	{
		if (COBOT_PART_DATA[nvRtCobot].existLabel || VC[vcRtCobotLabel].AOn())
		{
			ER.Save(ER_RT_COBOT_LABEL_EXIST);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CRtCobot::GetLabelPickup(void)
{
	for (int nvNo = nvTrayShuttle; nvNo > -1; nvNo--)
	{
		if (Between(TRAY_PART_DATA[nvNo].exist, enExistBtmFoam, enExistBtmFoamQc))
			continue;

		if (TRAY_PART_DATA[nvNo].exist)
		{
			if (TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup &&
				TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CRtCobot::SetLabelPickup(void)
{
	for (int nvNo = nvTrayShuttle; nvNo > -1; nvNo--)
	{
		if (Between(TRAY_PART_DATA[nvNo].exist, enExistBtmFoam, enExistBtmFoamOpen))
			continue;

		if (TRAY_PART_DATA[nvNo].exist)
		{
			if (TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup &&
				TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup)
			{
				TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup = true;
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CRtCobot::CycleLabelPick(void)
{
	if (!m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_LABEL_PICK_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();

	switch (m_pFsm->Get())
	{
		case C_LABEL_PICK_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "LABEL_PICKUP";
				NV.ndm[commMbbLabelAttachStart] = COMM_IDLE;
			}

			if (!IsCanCycleLabelPick())
				break;

			if (!CylInit() || !LABEL_SHUTTLE[nMsgDir].CylInit())
				break;

			if (!IsEmpty(_LABEL))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (nMsgDir == _LT)
			{
				if (LABEL_SHUTTLE[enlabelRt].m_pFsm->IsBusy())
					break;

				if (MT_INPOS != MtPosMove(LABEL_SHUTTLE[_RT].mtLabelShuttleY, P_LABEL_SHUTTLE_Y_READY))
					break;
			}

			if (MT_INPOS != MtPosMove(LABEL_SHUTTLE[nMsgDir].mtLabelShuttleY, P_LABEL_SHUTTLE_Y_SEND))
				break;

			m_pFsm->Set(C_LABEL_PICK_MOVE);
			break;
		}
		case C_LABEL_PICK_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_MOVE", m_strName);

			if (!CYL[cylRtCobotLabelPkUd].IsDn())
				CYL[cylRtCobotLabelPkUd].Dn(RT_COBOT_LABEL_PK_DN);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_LABEL_PICK + (nMsgDir * 2)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_LABEL_PICK + (nMsgDir * 2)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_LABEL_PICK + (nMsgDir * 2));

				break;
			}

			if (!CYL[cylRtCobotLabelPkUd].IsDn())
				break;

			m_pFsm->Set(C_LABEL_PICK_VAC);
			break;
		}
		case C_LABEL_PICK_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_VAC", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (!VC[vcRtCobotLabel].AOn())
				{
					ER.Save(ER_VAC_RT_COBOT_LABEL);
					m_pFsm->Set(C_ERROR);
					break;
				}

				if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->AOff())
				{
					ER.Save((ErrName)(ER_VAC_RT_COBOT_LT_LABEL_SHUTTLE + nMsgDir));
					m_pFsm->Set(C_ERROR);
					break;
				}
			}

			if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->IsOff())
				LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->BlowOn();

			if (!VC[vcRtCobotLabel].IsOn())
				VC[vcRtCobotLabel].On(RT_COBOT_LABEL_ON);

			if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->AOff() || !VC[vcRtCobotLabel].AOn())
				break;

			m_pFsm->Set(C_LABEL_PICK_UP);
			break;
		}
		case C_LABEL_PICK_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_Z_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbLabelAttachStart] = COMM_REQ;
			}

			if (!CYL[cylRtCobotLabelPkUd].IsUp())
				CYL[cylRtCobotLabelPkUd].Up(RT_COBOT_LABEL_PK_UP);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_LABEL_PICK_TO_READY + (nMsgDir * 2)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_LABEL_PICK_TO_READY + (nMsgDir * 2)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_LABEL_PICK_TO_READY + (nMsgDir * 2));

				break;
			}
			
			if (!CYL[cylRtCobotLabelPkUd].IsUp())
				break;

			m_pFsm->Set(C_LABEL_PICK_END);
			break;
		}
		case C_LABEL_PICK_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_LABEL) = TRUE;
			memcpy(&COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL], &LABEL_PART_DATA[nvLabelLtSuttle + nMsgDir].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&LABEL_PART_DATA[nvLabelLtSuttle + nMsgDir], sizeof(IPC_MMI::LabelPartData));

			m_state = "";

			if (!SetLabelPickup())
			{
				ER.Save(ER_RT_COBOT_LABEL_MEMORY_ABNORMAL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			MtPosMove(LABEL_SHUTTLE[nMsgDir].mtLabelShuttleY, P_LABEL_SHUTTLE_Y_RECV);
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CRtCobot::CycleMbbVi(void)
{
	if (!m_pFsm->Between(C_MBB_VI_START, C_MBB_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_MBB_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_MBB_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "MBB_VI";

				m_nViRetryCnt = 0;
				viMode = MODE_RT_COBOT_MBB_MISPLACED;
				VI[VI_RT_COBOT].Clear();
			}

			if (!VI[VI_RT_COBOT].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CylInit())
				break;

			if (!VI[VI_RT_COBOT].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_MBB_VI_MOVE);
			break;
		}
		case C_MBB_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_VI_MOVE", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_VI + (nMsgDir * 3)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_VI + (nMsgDir * 3)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_VI + (nMsgDir * 3));

				break;
			}

			m_pFsm->Set(C_MBB_VI_RST);
			break;
		}
		case C_MBB_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_VI_RST", m_strName);

			if (m_nViRetryCnt > 0)
			{
				if (!m_pFsm->TimeLimit(_1sec))
					break;
			}

			if (!VI[VI_RT_COBOT].IsModeOn(viMode, 10))
			{
				VI[VI_RT_COBOT].SetMode(viMode);
				break;
			}

			VI[VI_RT_COBOT].Reset();
			m_pFsm->Set(C_MBB_VI_TRIG);
			break;
		}
		case C_MBB_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_RT_COBOT_MBB_MISPLACED_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_RT_COBOT].Start(viMode))
				m_pFsm->Set(C_MBB_VI_RESULT);

			break;
		}
		case C_MBB_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_RT_COBOT_MBB_MISPLACED_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_RT_COBOT].IsBusy() || !VI[VI_RT_COBOT].m_isRcvComp)
				break;

			if (!VI[VI_RT_COBOT].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[rtCobotlabelViRetryCnt])
				{
					m_pFsm->Set(C_MBB_VI_RST);
					break;
				}

				ER.Save(ER_VISION_RT_COBOT_MBB_MISPLACED_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_MBB_VI_UP);
			break;
		}
		case C_MBB_VI_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_VI_UP", m_strName);

			if (NV.use[useRtCobotContinueSeq])
			{
				if (!CYL[cylRtCobotLabelPkUd].IsDn())
					CYL[cylRtCobotLabelPkUd].Dn(RT_COBOT_LABEL_PK_DN);

				if (!CYL[cylRtCobotLabelPkTurnFb].IsFwd())
					CYL[cylRtCobotLabelPkTurnFb].Fwd(RT_COBOT_LABEL_PK_TURN_FWD);

				if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_VI_TO_LABEL_ATTACH_READY + (nMsgDir * 3)))
				{
					if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_VI_TO_LABEL_ATTACH_READY + (nMsgDir * 3)))
						ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_VI_TO_LABEL_ATTACH_READY + (nMsgDir * 3));

					break;
				}
			}
			else
			{
				if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_VI_TO_READY + (nMsgDir * 3)))
				{
					if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_VI_TO_READY + (nMsgDir * 3)))
						ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_VI_TO_READY + (nMsgDir * 3));

					break;
				}
			}
			
			m_pFsm->Set(C_MBB_VI_END);
			break;
		}
		case C_MBB_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			MBB_PART_DATA[nMsgDir].exist = enMbbExistQc;
			m_state = "";
			m_pFsm->Set(C_IDLE);

			if (NV.use[useRtCobotContinueSeq])
				m_pFsm->Set(C_LABEL_ATTACH_START, nMsgDir);

			break;
		}
	}
}

void CRtCobot::CycleLabelAttach(void)
{
	if (!m_pFsm->Between(C_LABEL_ATTACH_START, C_LABEL_ATTACH_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_LABEL_ATTACH_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_LABEL_ATTACH_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "LABEL_ATTACH";
			}

			if (!IsCanCycleLabelAttach())
				break;

			if (!IsExist(_LABEL))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_MOVE);
			break;
		}
		case C_LABEL_ATTACH_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_MOVE", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_VI_TO_LABEL_ATTACH_READY + (nMsgDir * 3)) &&
				!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_READY + (nMsgDir * 4)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH_READY + (nMsgDir * 4)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH_READY + (nMsgDir * 4));

				break;
			}

			if (!CYL[cylRtCobotLabelPkUd].IsDn())
				CYL[cylRtCobotLabelPkUd].Dn(RT_COBOT_LABEL_PK_DN);

			if (!CYL[cylRtCobotLabelPkTurnFb].IsFwd())
				CYL[cylRtCobotLabelPkTurnFb].Fwd(RT_COBOT_LABEL_PK_TURN_FWD);

			if (!CYL[cylRtCobotLabelPkUd].IsDn() || !CYL[cylRtCobotLabelPkTurnFb].IsFwd())
				break;

			m_pFsm->Set(C_LABEL_ATTACH_Z_DN);
			break;
		}
		case C_LABEL_ATTACH_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_Z_DN", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH + (nMsgDir * 4)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH + (nMsgDir * 4)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH + (nMsgDir * 4));

				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VAC);
			break;
		}
		case C_LABEL_ATTACH_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VAC", m_strName);

			if (!VC[vcRtCobotLabel].IsOff() || !VC[vcRtCobotLabel].AOff())
			{
				VC[vcRtCobotLabel].Off(RT_COBOT_LABEL_OFF);
				break;
			}

			if (!CYL[cylRtCobotLabelPkUd].IsUp())
				CYL[cylRtCobotLabelPkUd].Up(RT_COBOT_LABEL_PK_UP);

			m_pFsm->Set(C_LABEL_ATTACH_UP);
			break;
		}
		case C_LABEL_ATTACH_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_UP", m_strName);

			if (!CYL[cylRtCobotLabelPkUd].IsUp())
			{
				CYL[cylRtCobotLabelPkUd].Up(RT_COBOT_LABEL_PK_UP);
				break;
			}

			if (!CYL[cylRtCobotLabelPkTurnFb].IsBwd())
				CYL[cylRtCobotLabelPkTurnFb].Bwd(RT_COBOT_LABEL_PK_TURN_BWD);

			if (NV.use[useRtCobotContinueSeq])
			{
				if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_ATTACH_VI + (nMsgDir * 4)))
				{
					if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_ATTACH_VI + (nMsgDir * 4)))
						ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_ATTACH_VI + (nMsgDir * 4));

					break;
				}
			}
			else
			{
				if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_READY + (nMsgDir * 4)))
				{
					if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_READY + (nMsgDir * 4)))
						ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_READY + (nMsgDir * 4));

					break;
				}
			}

			if (!CYL[cylRtCobotLabelPkTurnFb].IsBwd())
				break;

			m_pFsm->Set(C_LABEL_ATTACH_END);
			break;
		}
		case C_LABEL_ATTACH_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_LABEL) = FALSE;
			MBB_PART_DATA[nvLtMbbStack + nMsgDir].exist = enMbbExistLabel;
			memcpy(&MBB_PART_DATA[nvLtMbbStack + nMsgDir].labelInfo, &COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL], sizeof(IPC_MMI::SBoxLabelInfo));
			m_state = "";
			m_pFsm->Set(C_IDLE);

			if (NV.use[useRtCobotContinueSeq])
				m_pFsm->Set(C_LABEL_ATTACH_VI_START, nMsgDir);

			break;
		}
	}
}

void CRtCobot::CycleLabelAttachVi(void)
{
	if (!m_pFsm->Between(C_LABEL_ATTACH_VI_START, C_LABEL_ATTACH_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_LABEL_ATTACH_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_LABEL_ATTACH_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "LABEL_ATTACH_VI";

				m_nViRetryCnt = 0;
				viMode = MODE_RT_COBOT_MBB_LABEL_ATTACHED;
				VI[VI_RT_COBOT].Clear();
				
				NV.ndm[commMbbLabelAttachEnd] = COMM_IDLE;
			}

			if (!VI[VI_RT_COBOT].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CylInit())
				break;

			if (!VI[VI_RT_COBOT].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VI_DATA_SEND);
			break;
		}
		case C_LABEL_ATTACH_VI_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_DATA_SEND", m_strName);
				NV.ndm[reqViDataSendRtCobotLtMbbStackMode1 + nMsgDir] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_RT_COBOT_LABEL_ATTACH_VI_DATA_SEND_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[reqViDataSendRtCobotLtMbbStackMode1 + nMsgDir] != COMM_COMP)
				break;

			m_pFsm->Set(C_LABEL_ATTACH_VI_MOVE);
			break;
		}
		case C_LABEL_ATTACH_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_MOVE", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_ATTACH_VI + (nMsgDir * 4)) &&
				!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI + (nMsgDir * 3)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI + (nMsgDir * 3)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI + (nMsgDir * 3));

				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VI_RST);
			break;
		}
		case C_LABEL_ATTACH_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_RST", m_strName);

			if (m_nViRetryCnt > 0)
			{
				if (!m_pFsm->TimeLimit(_1sec))
					break;
			}

			if (!VI[VI_RT_COBOT].IsModeOn(viMode, 10))
			{
				VI[VI_RT_COBOT].SetMode(viMode);
				break;
			}

			VI[VI_RT_COBOT].Reset();
			m_pFsm->Set(C_LABEL_ATTACH_VI_TRIG);
			break;
		}
		case C_LABEL_ATTACH_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_RT_COBOT_MBB_LABEL_ATTACHED_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_RT_COBOT].Start(viMode))
				m_pFsm->Set(C_LABEL_ATTACH_VI_RESULT);

			break;
		}
		case C_LABEL_ATTACH_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_15sec))
			{
				ER.Save(ER_VISION_RT_COBOT_MBB_LABEL_ATTACHED_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_RT_COBOT].IsBusy() || !VI[VI_RT_COBOT].m_isRcvComp)
				break;

			if (!VI[VI_RT_COBOT].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[rtCobotlabelViRetryCnt])
				{
					m_pFsm->Set(C_LABEL_ATTACH_VI_RST);
					break;
				}

				ER.Save(ER_VISION_RT_COBOT_MBB_LABEL_ATTACHED_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VI_UP);
			break;
		}
		case C_LABEL_ATTACH_VI_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbLabelAttachEnd] = COMM_REQ;
			}

			if (NV.use[useRtCobotContinueSeq])
			{
				if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_PICK + (nMsgDir * 3)))
				{
					if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_PICK + (nMsgDir * 3)))
						ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_PICK + (nMsgDir * 3));

					break;
				}
			}
			else
			{
				if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_READY + (nMsgDir * 3)))
				{
					if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_READY + (nMsgDir * 3)))
						ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_READY + (nMsgDir * 3));

					break;
				}
			}

			m_pFsm->Set(C_LABEL_ATTACH_VI_END);
			break;
		}
		case C_LABEL_ATTACH_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			MBB_PART_DATA[nMsgDir].exist = enMbbExistLabelQc;
			m_state = "";
			m_pFsm->Set(C_IDLE);

			if (NV.use[useRtCobotContinueSeq])
				m_pFsm->Set(C_MBB_PICK_START, nMsgDir);

			break;
		}
	}
}

void CRtCobot::CycleMbbPick(void)
{
	if (!m_pFsm->Between(C_MBB_PICK_START, C_MBB_PICK_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_MBB_PICK_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_MBB_PICK_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_PICK_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "MBB_PICKUP";
				NV.ndm[commMbbTransferStart] = COMM_IDLE;
			}

			if (!IsCanCycleMbbPick())
				break;

			if (!CylInit())
				break;

			if (!IsEmpty(_MBB))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_MBB_PICK_MOVE);
			break;
		}
		case C_MBB_PICK_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PICK_MOVE", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_PICK + (nMsgDir * 3)) &&
				!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_PICK + (nMsgDir * 2)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_PICK + (nMsgDir * 2)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_PICK + (nMsgDir * 2));

				break;
			}

			m_pFsm->Set(C_MBB_PICK_VAC);
			break;
		}
		case C_MBB_PICK_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PICK_VAC", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (!VC[vcRtCobotMbb].AOn() || !VC[vcRtCobotMbb2].AOn())
				{
					ER.Save(ER_VAC_RT_COBOT_MBB);
					m_pFsm->Set(C_ERROR);
					break;
				}
			}

			if (!VC[vcRtCobotMbb].IsOn() || !VC[vcRtCobotMbb2].IsOn())
			{
				VC[vcRtCobotMbb].On(RT_COBOT_MBB_ON);
				VC[vcRtCobotMbb2].On(RT_COBOT_MBB2_ON);
			}

			if (!VC[vcRtCobotMbb].AOn() || !VC[vcRtCobotMbb2].AOn() ||
				!VC[vcRtCobotMbb].IsOn() || !VC[vcRtCobotMbb2].IsOn())
				break;

			m_pFsm->Set(C_MBB_PICK_UP);
			break;
		}
		case C_MBB_PICK_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_PICK_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbTransferStart] = COMM_REQ + nMsgDir;
			}

			if (nMsgDir == _LT)
				On(oSolLtMbbCstAirBlow);
			else
				On(oSolRtMbbCstAirBlow);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LT_MBB_PICK_TO_READY + (nMsgDir * 2)))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LT_MBB_PICK_TO_READY + (nMsgDir * 2)))
					ROBOT[robotRt].Move(P_RT_COBOT_LT_MBB_PICK_TO_READY + (nMsgDir * 2));

				break;
			}

			m_pFsm->Set(C_MBB_PICK_END);
			break;
		}
		case C_MBB_PICK_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PICK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Off(oSolLtMbbCstAirBlow);
			Off(oSolRtMbbCstAirBlow);

			COBOT_PART_DATA[nvRtCobot].exist = enMbbExistLabelQc;
			MBB_PART_DATA[nvLtMbbStack + nMsgDir].exist = enEmpty;
			MBB_PART_DATA[nvLtMbbStack + nMsgDir].cnt++;
			memcpy(&COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB], &MBB_PART_DATA[nvLtMbbStack + nMsgDir].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&MBB_PART_DATA[nvLtMbbStack + nMsgDir].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));

			if (AOff(MBB_STACK[nMsgDir].iMbbCstExist))
				MBB_PART_DATA[nvLtMbbStack + nMsgDir].flag.cstMaterialEmpty = true;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CRtCobot::CycleMbbPlace(void)
{
	if (!m_pFsm->Between(C_MBB_PLACE_START, C_MBB_PLACE_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_MBB_PLACE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_PLACE_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_PLACE_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "MBB_PLACE";
				NV.ndm[commMbbTransferEnd] = COMM_IDLE;
			}

			if (!IsCanCycleMbbPlace())
				break;

			if (!CylInit())
				break;

			if (!IsExist(_MBB))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (MT_INPOS != MtPosMove(mtMbbShuttleY, P_MBB_SHUTTLE_Y_RECV))
				break;

			m_pFsm->Set(C_MBB_PLACE_MOVE);
			break;
		}
		case C_MBB_PLACE_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PLACE_MOVE", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_MBB_PLACE))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_MBB_PLACE))
					ROBOT[robotRt].Move(P_RT_COBOT_MBB_PLACE);

				break;
			}

			m_pFsm->Set(C_MBB_PLACE_DN);
			break;
		}
		case C_MBB_PLACE_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PLACE_DN", m_strName);

			if (!VC[vcRtCobotMbb].IsOff() || !VC[vcRtCobotMbb2].IsOff())
			{
				VC[vcRtCobotMbb].Off(RT_COBOT_MBB_OFF);
				VC[vcRtCobotMbb2].Off(RT_COBOT_MBB2_OFF);
			}

			if (!VC[vcRtCobotMbb].AOff() || !VC[vcRtCobotMbb2].AOff() ||
				!VC[vcRtCobotMbb].IsOff() || !VC[vcRtCobotMbb2].IsOff())
				break;

			Exist(_MBB) = FALSE;
			TRAY_PART_DATA[nvMbbShuttle].exist = enExistMbb;
			memcpy(&TRAY_PART_DATA[nvMbbShuttle].labelInfo, &COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvRtCobot].flagRt, sizeof(IPC_MMI::RtCobotFlag));
			m_pFsm->Set(C_MBB_PLACE_UP);
			break;
		}
		case C_MBB_PLACE_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_PLACE_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbTransferEnd] = COMM_REQ;
			}

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_MBB_PLACE_TO_READY))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_MBB_PLACE_TO_READY))
					ROBOT[robotRt].Move(P_RT_COBOT_MBB_PLACE_TO_READY);

				break;
			}

			m_pFsm->Set(C_MBB_PLACE_END);
			break;
		}
		case C_MBB_PLACE_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PLACE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CRtCobot::CycleLabelReject(void)
{
	if (!m_pFsm->Between(C_LABEL_REJECT_START, C_LABEL_REJECT_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_LABEL_REJECT_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_REJECT_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_REJECT_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "LABEL_REJECT";
			}

			if (!IsCanCycleLabelReject())
				break;

			if (!CylInit())
				break;

			if (!IsExist(_LABEL))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_REJECT_MOVE);
			break;
		}
		case C_LABEL_REJECT_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_REJECT_MOVE", m_strName);

			if (!CYL[cylRtCobotLabelPkTurnFb].IsFwd())
				CYL[cylRtCobotLabelPkTurnFb].Fwd(RT_COBOT_LABEL_PK_TURN_FWD);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LABEL_REJECT))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LABEL_REJECT))
					ROBOT[robotRt].Move(P_RT_COBOT_LABEL_REJECT);

				break;
			}

			if (!CYL[cylRtCobotLabelPkTurnFb].IsFwd())
				break;

			if (!CYL[cylRtCobotLabelPkUd].IsDn())
			{
				CYL[cylRtCobotLabelPkUd].Dn(RT_COBOT_LABEL_PK_DN);
				break;
			}

			m_pFsm->Set(C_LABEL_REJECT_VAC);
			break;
		}
		case C_LABEL_REJECT_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_REJECT_VAC", m_strName);

			if (!VC[vcRtCobotLabel].IsOff() || VC[vcRtCobotLabel].AOn())
			{
				VC[vcRtCobotLabel].Off(RT_COBOT_LABEL_OFF);
				break;
			}

			// TODO: Sensor Check.

			m_pFsm->Set(C_LABEL_REJECT_UP);
			break;
		}
		case C_LABEL_REJECT_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_REJECT_UP", m_strName);

			if (!CYL[cylRtCobotLabelPkUd].IsUp())
			{
				CYL[cylRtCobotLabelPkUd].Up(RT_COBOT_LABEL_PK_UP);
				break;
			}

			if (!CYL[cylRtCobotLabelPkTurnFb].IsBwd())
				CYL[cylRtCobotLabelPkTurnFb].Bwd(RT_COBOT_LABEL_PK_TURN_BWD);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_LABEL_REJECT_TO_READY))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_LABEL_REJECT_TO_READY))
					ROBOT[robotRt].Move(P_RT_COBOT_LABEL_REJECT_TO_READY);

				break;
			}

			if (!CYL[cylRtCobotLabelPkTurnFb].IsBwd())
				break;

			m_pFsm->Set(C_LABEL_REJECT_END);
			break;
		}
		case C_LABEL_REJECT_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_REJECT_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_LABEL) = FALSE;
			ZeroMemory(&COBOT_PART_DATA[nvRtCobot].labelInfo[_LABEL], sizeof(IPC_MMI::SBoxLabelInfo));

			if (NV.ndm[ltLabelPrintTest])
			{
				NV.ndm[ltLabelPrintTest] = FALSE;
				NV.ndm[reqLabelDataSend] = COMM_COMP;
			}
			if (NV.ndm[rtLabelPrintTest])
			{
				NV.ndm[rtLabelPrintTest] = FALSE;
				NV.ndm[reqLabelDataSend] = COMM_COMP;
			}

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CRtCobot::CycleMbbReject(void)
{
	if (!m_pFsm->Between(C_MBB_REJECT_START, C_MBB_REJECT_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_RT_COBOT_MBB_REJECT_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_REJECT_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_REJECT_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "MBB_REJECT";
			}

			if (!IsCanCycleMbbReject())
				break;

			if (!CylInit())
				break;

			if (!IsExist(_MBB))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_MBB_REJECT_MOVE);
			break;
		}
		case C_MBB_REJECT_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_REJECT_MOVE", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_MBB_REJECT))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_MBB_REJECT))
					ROBOT[robotRt].Move(P_RT_COBOT_MBB_REJECT);

				break;
			}

			m_pFsm->Set(C_MBB_REJECT_VAC);
			break;
		}
		case C_MBB_REJECT_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_REJECT_VAC", m_strName);

			if (!VC[vcRtCobotMbb].IsOff() || !VC[vcRtCobotMbb2].IsOff())
			{
				VC[vcRtCobotMbb].Off(RT_COBOT_MBB_OFF);
				VC[vcRtCobotMbb2].Off(RT_COBOT_MBB2_OFF);
			}

			if (!VC[vcRtCobotMbb].IsOff() || !VC[vcRtCobotMbb2].IsOff() ||
				!VC[vcRtCobotMbb].IsOff() || !VC[vcRtCobotMbb2].IsOff())
				break;

			m_pFsm->Set(C_MBB_REJECT_UP);
			break;
		}
		case C_MBB_REJECT_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_REJECT_UP", m_strName);

			if (!ROBOT[robotRt].IsRdy(P_RT_COBOT_MBB_REJECT_TO_READY))
			{
				if (ROBOT[robotRt].CanMove(P_RT_COBOT_MBB_REJECT_TO_READY))
					ROBOT[robotRt].Move(P_RT_COBOT_MBB_REJECT_TO_READY);

				break;
			}

			m_pFsm->Set(C_MBB_REJECT_END);
			break;
		}
		case C_MBB_REJECT_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_REJECT_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_MBB) = FALSE;
			ZeroMemory(&COBOT_PART_DATA[nvRtCobot].labelInfo[_MBB], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvRtCobot].flagLt, sizeof(IPC_MMI::LtCobotFlag));

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}