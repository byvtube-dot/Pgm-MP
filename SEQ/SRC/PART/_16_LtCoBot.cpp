#include "..\..\Includes.h"

CLtCobot LT_COBOT;

CLtCobot::CLtCobot()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CLtCobot::~CLtCobot()
{

}

void CLtCobot::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = "$";
	if ("NORMAL_FOAM_PICK" == newValue)
		LT_COBOT.m_pSLog->XFR(newValue, "$", "NORMAL_FOAM_SEND_ELEV", "LT_COBOT");
	else if ("PARTIAL_FOAM_PICK" == newValue)
		LT_COBOT.m_pSLog->XFR(newValue, "$", "PARTIAL_FOAM_SEND_ELEV", "LT_COBOT");
	else if ("BTM_FOAM_PLACE" == newValue)
	{
		LT_COBOT.m_pSLog->XFR(newValue, "$", "LT_COBOT", "BTM_FOAM_RAIL");
	
		LT_COBOT.m_pSLog->RecordingOn(CV_FOAM_CONV);
	}
	else if ("LABEL_PICKUP" == newValue)
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
		LT_COBOT.m_pSLog->XFR(newValue, MatId, "LABEL_SHUTTLE", "LT_COBOT");
	}
	else if ("LABEL_ATTACH" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvLtCobot].labelInfo[_LABEL].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvLtCobot].labelInfo[_LABEL].sBoxId, ""))
			MatId = "$";
		LT_COBOT.m_pSLog->XFR(newValue, MatId, "LT_COBOT", "TOP_FOAM");
	}
	else if ("TOP_FOAM_PICKUP" == newValue)
	{
		MatId = static_cast<string> (FOAM_PART_DATA[nvFoamSendElev].labelInfo.sBoxId);
		if (0 == strcmp(FOAM_PART_DATA[nvFoamSendElev].labelInfo.sBoxId, ""))
			MatId = "$";
		LT_COBOT.m_pSLog->XFR(newValue, MatId, "FOAM_FLIP", "LT_COBOT");
	}
	else if ("TOP_FOAM_PLACE" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM].sBoxId, ""))
			MatId = "$";
		LT_COBOT.m_pSLog->XFR(newValue, MatId, "LT_COBOT", "TOP_FOAM_RAIL");
	}
	else if ("FOAM_REJECT" == newValue)
	{
		MatId = static_cast<string> (COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM].sBoxId);
		if (0 == strcmp(COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM].sBoxId, ""))
			MatId = "$";
		LT_COBOT.m_pSLog->XFR(newValue, MatId, "LT_COBOT", "FOAM_REJECT");
	}
	else
	{
		LT_COBOT.m_pSLog->XFR(newValue, MatId, "", "");
		LT_COBOT.m_pSLog->RecordingOff(CV_FOAM_CONV);
	}
}

void CLtCobot::Auto(void)
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

	if (Exist(_FOAM))
	{
		if (Exist(_FOAM) == enFoamExistBtm || Exist(_FOAM) == enFoamExistBtmPartial)
		{
			if (IsCanCycleBtmFoamPlace())
			{
				m_pFsm->Set(C_BTM_FOAM_PLACE_START, Exist(_FOAM));
				return;
			}
		}
		else if (Exist(_FOAM) == enFoamExistTopLabel && TRAY_PART_DATA[nvTopFoam].exist == enExistBtmFoamTrayQc)
		{
			if (IsCanCycleTopFoamPlace())
			{
				m_pFsm->Set(C_TOP_FOAM_PLACE_START);
				return;
			}
		}
	}
	else if (Exist(_LABEL) &&
		(FOAM_RAIL.Exist(_BTM_FOAM_) == enExistBtmFoamTrayQc || FOAM_RAIL.Exist(_TOP_FOAM_) == enExistBtmFoamTrayQc))
	{
		if (IsCanCycleLabelAttach())
		{
			m_pFsm->Set(C_LABEL_ATTACH_START);
			return;
		}
	}
	else
	{
		if (!Exist(_LABEL) && GetLabelPickup() &&
			(LABEL_SHUTTLE[enlabelLt].Exist() == enLabelExistQc || LABEL_SHUTTLE[enlabelRt].Exist() == enLabelExistQc) &&
			FOAM_PART_DATA[nvFoamSendElev].exist != enFoamExistTopLabel) // 안전 조건 추가 필요
		{
			if (IsCanCycleLabelPick())
			{
				int nDir;
				if (LABEL_SHUTTLE[enlabelLt].Exist() && !LABEL_PART_DATA[enlabelLt].flag.lastlabel)
					nDir = _LT;
				else if (LABEL_SHUTTLE[enlabelRt].Exist() && !LABEL_PART_DATA[enlabelRt].flag.lastlabel)
					nDir = _RT;
				else
					return;

				m_pFsm->Set(C_LABEL_PICK_START, nDir);
				return;
			}
		}
		else if (FOAM_PART_DATA[nvFoamSendElev].exist == enFoamExistTopLabel &&
			(TRAY_PART_DATA[nvTopFoam].exist == enExistBtmFoamTrayQc || TRAY_PART_DATA[nvBtmFoam].exist == enExistBtmFoamTrayQc))
		{
			if (IsCanCycleTopFoamPick())
			{
				m_pFsm->Set(C_TOP_FOAM_PICK_START);
				return;
			}
		}
		else if (!TRAY_PART_DATA[nvBtmFoam].exist && TRAY_PART_DATA[nvInPk].exist)
		{
			BOOL bPartialFoam = TRAY_PART_DATA[nvInPk].flag.btmPartialFoamReq;
			if (!IsCanCycleBtmFoamPick(bPartialFoam))
				return;

			m_pFsm->Set(C_BTM_FOAM_PICK_START, bPartialFoam);
			return;
		}
	}
}

void CLtCobot::Cycle(void)
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

	CycleBtmFoamPick();
	CycleBtmFoamPlace();
	CycleLabelPick();
	CycleLabelAttach();
	CycleTopFoamPick();
	CycleTopFoamPlace();
}

void CLtCobot::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "LtCobot");
	m_pFsm = &FSM[fsmLtCobot];

	m_pSLog = &SLOG[slogLtCobot];
	m_pSLog->SetXFRDeviceId("LT_COBOT");
	m_pSLog->SetXFRMaterialType("FOAM");

	// TODO: Cobot Info Set
	m_pSLog->Add(robotLt, TRUE);
	m_pSLog->Add(cylLtCobotLabelPkUd, TRUE);
	m_pSLog->Add(cylLtCobotLabelPkTurnFb, TRUE);
	m_pSLog->Add(CV_FOAM_CONV, TRUE);

	m_pSLog->Add(vcLtCobotLabel, TRUE);
	m_pSLog->Add(vcLtCobotFoam, TRUE);
	m_pSLog->Add(vcLtCobotFoam2, TRUE);
}

BOOL CLtCobot::Common(void)
{
	bool isBusy = false;
	bool isMoving = false;

	// Left Cobot 동작 중일때, In Picker Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_BTM_FOAM_PLACE_START, C_BTM_FOAM_PLACE_END);
	isBusy = isBusy || m_pFsm->Between(C_TOP_FOAM_PLACE_START, C_TOP_FOAM_PLACE_END);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_BTM_FOAM_PLACE);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_BTM_FOAM_PLACE_TO_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_TOP_FOAM_PLACE);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_TOP_FOAM_PLACE_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[ltCobotInPkCanMove] = FALSE;
	else
		NV.ndm[ltCobotInPkCanMove] = TRUE;

	// Left Cobot 동작 중일때, Send Elevator Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	if (isBusy || isMoving)
		NV.ndm[ltCobotSendElevCanMove] = FALSE;
	else
		NV.ndm[ltCobotSendElevCanMove] = TRUE;

	// Left Cobot 동작 중일때, Partial Elevator Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	if (isBusy || isMoving)
		NV.ndm[ltCobotPartialElevCanMove] = FALSE;
	else
		NV.ndm[ltCobotPartialElevCanMove] = TRUE;

	// Left Cobot 동작 중일때, Foam Flipper Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || (m_pFsm->Between(C_BTM_FOAM_PICK_START, C_BTM_FOAM_PICK_END) && m_pFsm->GetMsg() == _NORMAL);
	isBusy = isBusy || m_pFsm->Between(C_LABEL_ATTACH_START, C_TOP_FOAM_PICK_END);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_BTM_FOAM_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_BTM_FOAM_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LABEL_ATTACH);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_LABEL_ATTACH_TO_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_TOP_FOAM_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_TOP_FOAM_PICK_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[ltCobotFlipCanMove] = FALSE;
	else
		NV.ndm[ltCobotFlipCanMove] = TRUE;

	// Left Cobot 동작 중일때, Foam Picker Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || (m_pFsm->Between(C_BTM_FOAM_PICK_START, C_BTM_FOAM_PICK_END) && m_pFsm->GetMsg() == _PARTIAL);
	isBusy = isBusy || m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END);
	isBusy = isBusy || m_pFsm->Between(C_FOAM_REJECT_START, C_FOAM_REJECT_END);
	isMoving = isMoving || (!OPR.isNoDevice && ROBOT[robotLt].m_robotCurPos.joint.joint_0 < 110.0);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_BTM_PARTIAL_FOAM_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_BTM_PARTIAL_FOAM_PICK_TO_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK_TO_PICK_READY);
	if (isBusy || isMoving)
		NV.ndm[ltCobotFoamPkCanMove] = FALSE;
	else
		NV.ndm[ltCobotFoamPkCanMove] = TRUE;

	// Left Cobot 동작 중일때, Label Shuttle Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_LABEL_PICK_MOVE, C_LABEL_PICK_END);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK_TO_PICK_READY);
	if (isBusy || isMoving)
		NV.ndm[ltCobotLabelShuttleCanMove] = FALSE;
	else
		NV.ndm[ltCobotLabelShuttleCanMove] = TRUE;

	// Left Cobot 동작 중일때, Right Cobot Mbb Place Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_FOAM_REJECT_START, C_FOAM_REJECT_END);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_FOAM_REJECT);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_FOAM_REJECT_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[ltCobotRtCobotMbbPlaceCanMove] = FALSE;
	else
		NV.ndm[ltCobotRtCobotMbbPlaceCanMove] = TRUE;

	// Left Cobot 동작 중일때, Right Cobot Mbb Reject Cycle 시작을 위한 인터락 조건 확인.
	isBusy = false;
	isMoving = false;
	isBusy = isBusy || m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END);
	isBusy = isBusy || m_pFsm->Between(C_FOAM_REJECT_START, C_FOAM_REJECT_END);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_LT_LABEL_PICK_READY_TO_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_RT_LABEL_PICK_TO_PICK_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_RT_LABEL_PICK_READY_TO_READY);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurIdx(P_LT_COBOT_FOAM_REJECT);
	isMoving = isMoving || ROBOT[robotLt].CmdOrCurMovingIdx(P_LT_COBOT_FOAM_REJECT_TO_READY);
	if (isBusy || isMoving)
		NV.ndm[ltCobotRtCobotRejectCanMove] = FALSE;
	else
		NV.ndm[ltCobotRtCobotRejectCanMove] = TRUE;

	return TRUE;
}

BOOL CLtCobot::IsHwErr(void)
{
	if (CYL[cylLtCobotLabelPkUd].GetErr())
		return TRUE;
	if (CYL[cylLtCobotLabelPkTurnFb].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CLtCobot::IsHwRdy(void)
{
	// TODO: Cobot Info Set
	if (!ROBOT[robotLt].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CLtCobot::CylInit(void)
{
	if (!CYL[cylLtCobotLabelPkUd].IsUp())
		CYL[cylLtCobotLabelPkUd].Up(LT_COBOT_LABEL_PK_UP);

	if (!CYL[cylLtCobotLabelPkTurnFb].IsBwd())
		CYL[cylLtCobotLabelPkTurnFb].Bwd(LT_COBOT_LABEL_PK_TURN_BWD);

	if (!CYL[cylLtCobotLabelPkUd].IsUp() || !CYL[cylLtCobotLabelPkTurnFb].IsBwd())
		return FALSE;

	return TRUE;
}

BOOL CLtCobot::IsCanCycleBtmFoamPick(BOOL bPartialFoam)
{
	if (!bPartialFoam)
	{
		// Normal Foam 
		if (FOAM_ELEV[enFoamSendElev].m_pFsm->IsBusy())
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Send Elev Cycle is Busy");
			return FALSE;
		}

		if (FOAM_PART_DATA[nvFoamFlip].exist == enEmpty)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Flip Empty");
			return FALSE;
		}

		if (!MT[mtFoamFlipT].InPos(P_FOAM_FLIP_T_SEND))
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Flip T is not in safety position");
			return FALSE;
		}
	}
	else
	{
		// Partial Foam
		if (FOAM_PK.m_pFsm->Between(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.C_FOAM_PLACE_END))
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Picker Place Cycle is Busy");
			return FALSE;
		}

		if (MT[mtFoamPkY].m_state.realCnt > MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY))
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Picker Y is not in safety position");
			return FALSE;
		}

		if (FOAM_ELEV[enFoamPartialElev].m_pFsm->IsBusy())
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Partial Foam Send Elev Level Check Flag Task Not Completed");
			return FALSE;
		}

		if (!FOAM_PART_DATA[nvFoamPartialElev].flag.levelDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Partial Foam Send Elev Empty");
			return FALSE;
		}

		if (FOAM_PART_DATA[nvFoamPartialElev].cnt < 1)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Partial Foam Send Elev Count Empty");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CLtCobot::IsCanCycleBtmFoamPlace(void)
{
	if (MT[mtInPkX].PosTable(P_IN_PK_X_READY) - 1 > MT[mtInPkX].m_state.realCnt)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"In Picker is not in safety position");
		return FALSE;
	}

	if (IN_PK.IsHwRdy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"In Picker is not Ready");
		return FALSE;
	}

	if (IN_PK.m_pFsm->Between(IN_PK.C_BANDING_VI_START, IN_PK.C_BANDING_VI_END) ||
		IN_PK.m_pFsm->Between(IN_PK.C_TRAY_PLACE_START, IN_PK.C_TRAY_PLACE_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"In Picker Cycle is Busy");
		return FALSE;
	}

	if (FOAM_RAIL.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Cycle is Busy");
		return FALSE;
	}

	if (FOAM_RAIL.Exist(_BTM_FOAM_))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Memory Exist");
		return FALSE;
	}

	return TRUE;
}

BOOL CLtCobot::IsCanCycleLabelPick(void)
{
	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_PLACE_START, RT_COBOT.C_MBB_PLACE_END) || RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_REJECT_START, RT_COBOT.C_MBB_REJECT_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Rt Cobot MBB Place Cycle is Busy");
		return FALSE;
	}

	if (FOAM_PK.m_pFsm->Between(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.C_FOAM_PLACE_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Picker Place Cycle is Busy");
		return FALSE;
	}

	if (MT[mtFoamPkY].m_state.realCnt > MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Picker Y is not in safety position");
		return FALSE;
	}

	if (!NV.ndm[rtCobotLtCobotLabelPickCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Right Cobot is not in safety position");
		return FALSE;
	}

	if (LABEL_SHUTTLE[enlabelLt].m_pFsm->IsBusy() || LABEL_SHUTTLE[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}

	if (!GetLabelPickup())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Lt Cobot No required labels available.");
		return FALSE;
	}

	return TRUE;
}

BOOL CLtCobot::IsCanCycleLabelAttach(void)
{
	if (FOAM_ELEV[enFoamSendElev].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Send Cycle is Busy");
		return FALSE;
	}

	if (!Exist(_LABEL))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"LtCobot Label is Empty");
		return FALSE;
	}

	if (FOAM_PART_DATA[nvFoamSendElev].exist != enFoamExistTop)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Send Elev Foam Empty");
		return FALSE;
	}

	if (!FOAM_PART_DATA[nvFoamSendElev].flag.levelDid)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Send Elev Level Check Flag Task Not Completed");
		return FALSE;
	}

	if (!FOAM_RAIL.Exist(_BTM_FOAM_) &&	FOAM_RAIL.Exist(_TOP_FOAM_) != enExistBtmFoamTrayQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Memory Empty");
		return FALSE;
	}

	return TRUE;
}

BOOL CLtCobot::IsCanCycleFoamReject(void)
{
	if (!NV.ndm[rtCobotLtCobotRejectCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Right Cobot is not in safety position");
		return FALSE;
	}

	if (FOAM_PK.m_pFsm->Between(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.C_FOAM_PLACE_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Picker Place Cycle is Busy");
		return FALSE;
	}

	if (MT[mtFoamPkY].m_state.realCnt > MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Picker Y is not in safety position");
		return FALSE;
	}
	return TRUE;
}

BOOL CLtCobot::IsCanCycleTopFoamPick(void)
{
	if (!FOAM_PART_DATA[nvFoamSendElev].flag.levelDid)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Send Elev Level Check Flag Task Not Completed");
		return FALSE;
	}

	if (FOAM_PART_DATA[nvFoamSendElev].exist != enFoamExistTopLabel)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Send Elev Empty");
		return FALSE;
	}

	if (FOAM_PART_DATA[nvFoamSendElev].cnt < 1)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Send Elev Count Empty");
		return FALSE;
	}

	return TRUE;
}

BOOL CLtCobot::IsCanCycleTopFoamPlace(void)
{
	if (MT[mtInPkX].PosTable(P_IN_PK_X_READY) - 1 > MT[mtInPkX].m_state.realCnt)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"In Picker is not in safety position");
		return FALSE;
	}

	if (IN_PK.IsHwRdy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"In Picker is not Ready");
		return FALSE;
	}

	if (IN_PK.m_pFsm->Between(IN_PK.C_BANDING_VI_START, IN_PK.C_BANDING_VI_END) ||
		IN_PK.m_pFsm->Between(IN_PK.C_TRAY_PLACE_START, IN_PK.C_TRAY_PLACE_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"In Picker Cycle is Busy");
		return FALSE;
	}

	if (FOAM_RAIL.m_pFsm->Between(FOAM_RAIL.C_TOP_FOAM_SEND_START, FOAM_RAIL.C_TOP_FOAM_SEND_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CLtCobot::ExistCrossCheck(void)
{
	if (!m_pFsm->Between(C_BTM_FOAM_PICK_START, C_BTM_FOAM_PLACE_END) &&
		!m_pFsm->Between(C_TOP_FOAM_PICK_START, C_TOP_FOAM_PLACE_END) &&
		!m_pFsm->Between(C_FOAM_REJECT_START, C_FOAM_REJECT_END))
	{
		if (!Exist(_FOAM) && (VC[vcLtCobotFoam].AOn() || VC[vcLtCobotFoam2].AOn()))
		{
			ER.Save(ER_LT_COBOT_FOAM_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist(_FOAM) && (VC[vcLtCobotFoam].AOff() || VC[vcLtCobotFoam2].AOff()))
		{
			ER.Save(ER_LT_COBOT_FOAM_MEMORY_STATUS);
			return TRUE;
		}
	}

	if (!m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_ATTACH_END))
	{
		if (!Exist(_LABEL) && VC[vcLtCobotLabel].AOn())
		{
			ER.Save(ER_LT_COBOT_LABEL_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist(_LABEL) && VC[vcLtCobotLabel].AOff())
		{
			ER.Save(ER_LT_COBOT_LABEL_MEMORY_STATUS);
			return TRUE;
		}
	}
	return FALSE;
}

int& CLtCobot::Exist(int materialType)
{
	if (materialType == _FOAM)	// Foam
		return COBOT_PART_DATA[nvLtCobot].exist;
	else						// Label
		return COBOT_PART_DATA[nvLtCobot].existLabel;
}

BOOL CLtCobot::IsExist(int materialType)
{
	if (materialType == _FOAM)
	{
		if (!COBOT_PART_DATA[nvLtCobot].exist || VC[vcLtCobotFoam].AOff() || VC[vcLtCobotFoam2].AOff())
		{
			ER.Save(ER_LT_COBOT_FOAM_EMPTY);
			return FALSE;
		}
	}
	else
	{
		if (!COBOT_PART_DATA[nvLtCobot].existLabel || VC[vcLtCobotLabel].AOff())
		{
			ER.Save(ER_LT_COBOT_LABEL_EMPTY);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CLtCobot::IsEmpty(int materialType)
{
	if (materialType == _FOAM)
	{
		if (COBOT_PART_DATA[nvLtCobot].exist || VC[vcLtCobotFoam].AOn() || VC[vcLtCobotFoam2].AOn())
		{
			ER.Save(ER_LT_COBOT_FOAM_EXIST);
			return FALSE;
		}
	}
	else
	{
		if (COBOT_PART_DATA[nvLtCobot].existLabel || VC[vcLtCobotLabel].AOn())
		{
			ER.Save(ER_LT_COBOT_LABEL_EXIST);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CLtCobot::GetLabelPickup(void) //## 2026.01 reject
{
	for (int nvNo = nvTrayShuttle; nvNo > -1; nvNo--)
	{
		if (Between(TRAY_PART_DATA[nvNo].exist, enExistBtmFoam, enExistBtmFoamQc))
			continue;

		if (NV.use[useRtCobotFirst])
		{
			if (TRAY_PART_DATA[nvNo].exist)
			{
				if (TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted && TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup &&
					TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup)
					return TRUE;
			}
		}
		else
		{
			if (TRAY_PART_DATA[nvNo].exist)
			{
				if (TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup)
					return TRUE;
				else if (TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup)
					return FALSE;
			}
		}
	}

	return FALSE;
}



BOOL CLtCobot::SetLabelPickup(void) //## 2026.01 reject
{
	for (int nvNo = nvTrayShuttle; nvNo > -1; nvNo--)
	{
		if (Between(TRAY_PART_DATA[nvNo].exist, enExistBtmFoam, enExistBtmFoamQc))
			continue;

		if (NV.use[useRtCobotFirst])
		{
			if (TRAY_PART_DATA[nvNo].exist)
			{
				if (TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted && TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup &&
					TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup)
				{
					TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup = true;
					return TRUE;
				}
			}
		}
		else
		{
			if (TRAY_PART_DATA[nvNo].exist)
			{
				if (TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup)
				{
					TRAY_PART_DATA[nvNo].flag.ltCobotLabelPickup = true;
					return TRUE;
				}
				else if (TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPickup)
					return FALSE;
			}
		}
	}

	return FALSE;
}

void CLtCobot::CycleBtmFoamPick(void)
{
	if (!m_pFsm->Between(C_BTM_FOAM_PICK_START, C_BTM_FOAM_PICK_END))
		return;

	if (m_pFsm->TimeLimit(90000)) // 40
	{
		ER.Save(ER_LT_COBOT_NORMAL_FOAM_PICK_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_BTM_FOAM_PICK_START:
		{
			if (m_pFsm->Once())
			{
				if (nMsgDir)
				{
					LOG[logSEQ].Message("<%s> C_BTM_FOAM_PICK_START [PARTIAL_FOAM]", m_strName);
					m_state = "PARTIAL_FOAM_PICK";
				}
				else
				{
					LOG[logSEQ].Message("<%s> C_BTM_FOAM_PICK_START [NORMAL_FOAM]", m_strName);
					m_state = "NORMAL_FOAM_PICK";
				}

				m_tmCycleTime.Reset();
				NV.ndm[commFoamPadTransferStart] = COMM_IDLE;
			}

			if (!CylInit())
				break;

			if (!IsCanCycleBtmFoamPick(nMsgDir))
				break;

			if (!IsEmpty(_FOAM))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_BTM_FOAM_PICK_MOVE);
			break;
		}
		case C_BTM_FOAM_PICK_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PICK_MOVE", m_strName);

			if (nMsgDir)
			{
				if (m_pFsm->TimeLimit(_1min)) // 30
				{
					ER.Save(ER_VAC_LT_COBOT_FOAM_PK_SAFETY_MOVE_TIMEOVER);
					m_pFsm->Set(C_ERROR);
					break;
				}

				if (MT[mtFoamPkY].m_state.realCnt > MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY))
				{
					if (!FOAM_PK.m_pFsm->IsBusy())
					{
						if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
							break;

						if (MT_INPOS != MtPosMove(mtFoamPkY, P_FOAM_PK_Y_READY))
							break;
					}

					break;
				}
			}

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_BTM_FOAM_PICK + (nMsgDir * 2)))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_BTM_FOAM_PICK + (nMsgDir * 2)))
					ROBOT[robotLt].Move(P_LT_COBOT_BTM_FOAM_PICK + (nMsgDir * 2));

				break;
			}

			m_pFsm->Set(C_BTM_FOAM_PICK_VAC);
			break;
		}
		case C_BTM_FOAM_PICK_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PICK_VAC", m_strName);

			//if (m_pFsm->TimeLimit(_10sec))
			//{
			//	if (nMsgDir == _NORMAL)
			//	{
			//		if (!VC[vcFoamFlip].AOff() || !VC[vcFoamFlip2].AOff())
			//		{
			//			if (!VC[vcFoamFlip].AOff())
			//				ER.Save(ER_VAC_ALARM_FOAM_FLIP);
			//			else if (!VC[vcFoamFlip2].AOff())
			//				ER.Save(ER_VAC_ALARM_FOAM_FLIP2);

			//			m_pFsm->Set(C_ERROR);
			//			break;
			//		}
			//	}

			//	if (!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn())
			//	{
			//		ER.Save((ErrName)(ER_VAC_LT_COBOT_NORMAL_FAOM + nMsgDir));
			//		m_pFsm->Set(C_ERROR);
			//		break;
			//	}
			//}

			if (!VC[vcLtCobotFoam].IsOn() || !VC[vcLtCobotFoam2].IsOn())
			{
				VC[vcLtCobotFoam].On(LT_COBOT_FOAM_ON);
				VC[vcLtCobotFoam2].On(LT_COBOT_FOAM2_ON);
			}

			if (nMsgDir == _NORMAL)
			{
				if (!VC[vcFoamFlip].IsOff() || !VC[vcFoamFlip2].IsOff())
				{
					VC[vcFoamFlip].Off(FOAM_FLIP_OFF);
					VC[vcFoamFlip2].Off(FOAM_FLIP2_OFF);
				}

				//if (!VC[vcFoamFlip].IsOff() || !VC[vcFoamFlip2].IsOff() ||
				//	VC[vcFoamFlip].AOn() || VC[vcFoamFlip2].AOn())
				//	break;
			}

			if (!m_pFsm->TimeLimit(_10sec))
			{
				if (nMsgDir == _NORMAL)
				{
					if (!VC[vcFoamFlip].IsOff() || !VC[vcFoamFlip2].IsOff() ||
						VC[vcFoamFlip].AOn() || VC[vcFoamFlip2].AOn())
						break;
				}

				if (!VC[vcLtCobotFoam].IsOn() || !VC[vcLtCobotFoam2].IsOn() ||
					!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn())
					break;
			}

			m_pFsm->Set(C_BTM_FOAM_PICK_UP);
			break;
		}
		case C_BTM_FOAM_PICK_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PICK_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commFoamPadTransferStart] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (nMsgDir == _NORMAL)
				{
					if (!VC[vcFoamFlip].AOff() || !VC[vcFoamFlip2].AOff())
					{
						if (!VC[vcFoamFlip].AOff())
							ER.Save(ER_VAC_ALARM_FOAM_FLIP);
						else if (!VC[vcFoamFlip2].AOff())
							ER.Save(ER_VAC_ALARM_FOAM_FLIP2);

						m_pFsm->Set(C_ERROR);
						break;
					}
				}

				if (!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn())
				{
					ER.Save((ErrName)(ER_VAC_LT_COBOT_NORMAL_FAOM + nMsgDir));
					m_pFsm->Set(C_ERROR);
					break;
				}
			}


			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
					ROBOT[robotLt].Move(P_LT_COBOT_READY);

				break;
			}
			 

			//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_BTM_FOAM_PICK_TO_READY + (nMsgDir * 2)))
			//{
			//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_BTM_FOAM_PICK_TO_READY + (nMsgDir * 2)))
			//		ROBOT[robotLt].Move(P_LT_COBOT_BTM_FOAM_PICK_TO_READY + (nMsgDir * 2));

			//	break;
			//}

			if (!VC[vcLtCobotFoam].IsOn() || !VC[vcLtCobotFoam2].IsOn() ||
				!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn())
				break;

			m_pFsm->Set(C_BTM_FOAM_PICK_END);
			break;
		}
		case C_BTM_FOAM_PICK_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PICK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			if (nMsgDir == _PARTIAL)
			{
				FOAM_PART_DATA[nvFoamPartialElev].cnt--;
				FOAM_PART_DATA[nvFoamPartialElev].flag.levelDid = false;
				FOAM_PART_DATA[nvFoamPartialElev].flag.recvFullDid = false;
				COBOT_PART_DATA[nvLtCobot].exist = enFoamExistBtmPartial;
			}
			else
			{
				ZeroMemory(&FOAM_PART_DATA[nvFoamFlip], sizeof(IPC_MMI::FoamPartData));
				COBOT_PART_DATA[nvLtCobot].exist = enFoamExistBtm;
			}

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLtCobot::CycleBtmFoamPlace(void)
{
	if (!m_pFsm->Between(C_BTM_FOAM_PLACE_START, C_BTM_FOAM_PLACE_END))
		return;

	if (m_pFsm->TimeLimit(90000)) // 40sec
	{
		ER.Save(ER_LT_COBOT_BTM_FOAM_PLACE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsg = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_BTM_FOAM_PLACE_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PLACE_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "BTM_FOAM_PLACE";
			}

			if (!IsCanCycleBtmFoamPlace())
				break;

			if (!CylInit())
				break;

			if (!IsExist(_FOAM))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!FOAM_RAIL.CylInit(_BTM_FOAM_) || !FOAM_RAIL.CylInit(_TOP_FOAM_))
				break;

			m_pFsm->Set(C_BTM_FOAM_PLACE_MOVE);
			break;
		}
		case C_BTM_FOAM_PLACE_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PLACE_MOVE", m_strName);

			if (!CYL[cylBtmFoamStopUd].IsDn())
			{
				CYL[cylBtmFoamStopUd].Dn(BTM_FOAM_STOP_DN);
				break;
			}

			int nFoamType = _NORMAL;
			if (nMsg == enFoamExistBtmPartial)
				nFoamType = _PARTIAL;

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_BTM_FOAM_PLACE + (nFoamType * 2)))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_BTM_FOAM_PLACE + (nFoamType * 2)))
					ROBOT[robotLt].Move(P_LT_COBOT_BTM_FOAM_PLACE + (nFoamType * 2));

				break;
			}

			m_pFsm->Set(C_BTM_FOAM_PLACE_VAC);
			break;
		}
		case C_BTM_FOAM_PLACE_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PLACE_VAC", m_strName);

			if (!VC[vcLtCobotFoam].IsOff() || !VC[vcLtCobotFoam2].IsOff())
			{
				VC[vcLtCobotFoam].Off(LT_COBOT_FOAM_OFF);
				VC[vcLtCobotFoam2].Off(LT_COBOT_FOAM2_OFF);
			}

			if (!VC[vcLtCobotFoam].AOff() || !VC[vcLtCobotFoam2].AOff() ||
				!VC[vcLtCobotFoam].IsOff() || !VC[vcLtCobotFoam2].IsOff())
				break;

			m_pFsm->Set(C_BTM_FOAM_PLACE_UP);
			break;
		}
		case C_BTM_FOAM_PLACE_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PLACE_UP", m_strName);
				ZeroMemory(&COBOT_PART_DATA[nvLtCobot].flagLt, sizeof(IPC_MMI::LtCobotFlag));
				Exist(_FOAM) = FALSE;
				TRAY_PART_DATA[nvBtmFoam].exist = enExistBtmFoam;
				TRAY_PART_DATA[nvBtmFoam].flag.btmPartialFoamReq = (nMsg == enFoamExistBtmPartial);
				FOAM_RAIL.m_pFsm->Set(FOAM_RAIL.C_BTM_FOAM_OPEN_START);
			}

			int nFoamType = _NORMAL;
			if (nMsg == enFoamExistBtmPartial)
				nFoamType = _PARTIAL;

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
					ROBOT[robotLt].Move(P_LT_COBOT_READY);

				break;
			}

			//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_BTM_FOAM_PLACE_TO_READY + (nFoamType * 2)))
			//{
			//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_BTM_FOAM_PLACE_TO_READY + (nFoamType * 2)))
			//		ROBOT[robotLt].Move(P_LT_COBOT_BTM_FOAM_PLACE_TO_READY + (nFoamType * 2));

			//	break;
			//}

			m_pFsm->Set(C_BTM_FOAM_PLACE_END);
			break;
		}
		case C_BTM_FOAM_PLACE_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_PLACE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);

			break;
		}
	}
}

void CLtCobot::CycleLabelPick(void)
{
	if (!m_pFsm->Between(C_LABEL_PICK_START, C_LABEL_PICK_END))
		return;

	if (m_pFsm->TimeLimit(180000)) // _1min
	{
		ER.Save(ER_LT_COBOT_LABEL_PICK_CYCLE_TIME_OVER);
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
				NV.ndm[commLabelAttachStart] = COMM_IDLE;
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

			if (nMsgDir == _RT)
			{
				if (LABEL_SHUTTLE[enlabelLt].m_pFsm->IsBusy())
					break;

				if (MT_INPOS != MtPosMove(LABEL_SHUTTLE[_LT].mtLabelShuttleY, P_LABEL_SHUTTLE_Y_READY))
					break;

				if (!CYL[cylLabelQcViFb].IsBwd())
				{
					CYL[cylLabelQcViFb].Bwd(LABEL_QC_VI_BWD);
					break;
				}

			}
			else
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

			if (!CYL[cylLtCobotLabelPkTurnFb].IsFwd())
				CYL[cylLtCobotLabelPkTurnFb].Fwd(LT_COBOT_LABEL_PK_TURN_FWD);

			if (!CYL[cylLtCobotLabelPkUd].IsDn())
				CYL[cylLtCobotLabelPkUd].Dn(LT_COBOT_LABEL_PK_DN);

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_LT_LABEL_PICK_READY + (nMsgDir * 4)))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_LT_LABEL_PICK_READY + (nMsgDir * 4)))
					ROBOT[robotLt].Move(P_LT_COBOT_LT_LABEL_PICK_READY + (nMsgDir * 4));

				break;
			}

			m_pFsm->Set(C_LABEL_PICK_Z_DN);
			break;
		}
		case C_LABEL_PICK_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_MOVE", m_strName);

			if (!CYL[cylLtCobotLabelPkTurnFb].IsFwd())
				CYL[cylLtCobotLabelPkTurnFb].Fwd(LT_COBOT_LABEL_PK_TURN_FWD);

			if (!CYL[cylLtCobotLabelPkUd].IsDn())
				CYL[cylLtCobotLabelPkUd].Dn(LT_COBOT_LABEL_PK_DN);

			if (!CYL[cylLtCobotLabelPkTurnFb].IsFwd() || !CYL[cylLtCobotLabelPkUd].IsDn())
				break;

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_LT_LABEL_PICK + (nMsgDir * 4)))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_LT_LABEL_PICK + (nMsgDir * 4)))
					ROBOT[robotLt].Move(P_LT_COBOT_LT_LABEL_PICK + (nMsgDir * 4));

				break;
			}

			m_pFsm->Set(C_LABEL_PICK_VAC);
			break;
		}
		case C_LABEL_PICK_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_VAC", m_strName);

			//if (m_pFsm->TimeLimit(_10sec))
			//{
			//	if (!VC[vcLtCobotLabel].AOn())
			//	{
			//		ER.Save(ER_VAC_LT_COBOT_LABEL);
			//		m_pFsm->Set(C_ERROR);
			//		break;
			//	}

			//	if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->AOff())
			//	{
			//		ER.Save(ER_VAC_LT_COBOT_LABEL_SHUTTLE);
			//		m_pFsm->Set(C_ERROR);
			//		break;
			//	}
			//}

			if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->IsOff())
				LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->BlowOn();

			if (!VC[vcLtCobotLabel].IsOn())
				VC[vcLtCobotLabel].On(LT_COBOT_LABEL_ON);

			if (!m_pFsm->TimeLimit(_10sec))
			{
				if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->AOff() || !VC[vcLtCobotLabel].AOn())
					break;
			}

			m_pFsm->Set(C_LABEL_PICK_Z_UP);
			break;
		}
		case C_LABEL_PICK_Z_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_Z_UP", m_strName);
				
				//Exist(_LABEL) = TRUE;
				//memcpy(&COBOT_PART_DATA[nvLtCobot].labelInfo[_LABEL], &LABEL_PART_DATA[nvLabelLtSuttle + nMsgDir].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
				//ZeroMemory(&LABEL_PART_DATA[nvLabelLtSuttle + nMsgDir], sizeof(IPC_MMI::LabelPartData));

				//if (NV.use[useSecsGem])
				//	NV.ndm[commLabelAttachStart] = COMM_REQ;
			}

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY + (nMsgDir * 4)))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY + (nMsgDir * 4)))
					ROBOT[robotLt].Move(P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY + (nMsgDir * 4));

				break;
			}

			m_pFsm->Set(C_LABEL_PICK_READY_MOVE);
			break;
		}
		case C_LABEL_PICK_READY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_READY_MOVE", m_strName);

			if (m_pFsm->TimeLimit(_15sec))
			{
				if (!VC[vcLtCobotLabel].AOn())
				{
					ER.Save(ER_VAC_LT_COBOT_LABEL);
					m_pFsm->Set(C_ERROR);
					break;
				}

				if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->AOff())
				{
					ER.Save(ER_VAC_LT_COBOT_LABEL_SHUTTLE);
					m_pFsm->Set(C_ERROR);
					break;
				}
			}

			if (!CYL[cylLtCobotLabelPkTurnFb].IsBwd())
				CYL[cylLtCobotLabelPkTurnFb].Bwd(LT_COBOT_LABEL_PK_TURN_BWD);

			if (!CYL[cylLtCobotLabelPkUd].IsUp())
				CYL[cylLtCobotLabelPkUd].Up(LT_COBOT_LABEL_PK_UP);

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
					ROBOT[robotLt].Move(P_LT_COBOT_READY);

				break;
			}

			//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_LT_LABEL_PICK_READY_TO_READY + (nMsgDir * 4)))
			//{
			//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_LT_LABEL_PICK_READY_TO_READY + (nMsgDir * 4)))
			//		ROBOT[robotLt].Move(P_LT_COBOT_LT_LABEL_PICK_READY_TO_READY + (nMsgDir * 4));

			//	break;
			//}

			if (!CYL[cylLtCobotLabelPkTurnFb].IsBwd() || !CYL[cylLtCobotLabelPkUd].IsUp())
				break;

			if (!LABEL_SHUTTLE[nMsgDir].m_pVacLabelRecv->AOff() || !VC[vcLtCobotLabel].AOn())
				break;

			m_pFsm->Set(C_LABEL_PICK_END);
			break;
		}
		case C_LABEL_PICK_END:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_PICK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

				Exist(_LABEL) = TRUE;
				memcpy(&COBOT_PART_DATA[nvLtCobot].labelInfo[_LABEL], &LABEL_PART_DATA[nvLabelLtSuttle + nMsgDir].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
				ZeroMemory(&LABEL_PART_DATA[nvLabelLtSuttle + nMsgDir], sizeof(IPC_MMI::LabelPartData));

				if (NV.use[useSecsGem])
					NV.ndm[commLabelAttachStart] = COMM_REQ;
			}

			m_state = "";

			if (!SetLabelPickup())
			{
				ER.Save(ER_LT_COBOT_LABEL_MEMORY_ABNORMAL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			MtPosMove(LABEL_SHUTTLE[nMsgDir].mtLabelShuttleY, P_LABEL_SHUTTLE_Y_RECV);
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLtCobot::CycleLabelAttach(void)
{
	if (!m_pFsm->Between(C_LABEL_ATTACH_START, C_LABEL_ATTACH_END))
		return;

	if (m_pFsm->TimeLimit(90000)) // _40sec
	{
		ER.Save(ER_LT_COBOT_LABEL_ATTACH_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

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

			if (!CylInit())
				break;

			if (!IsExist(_LABEL))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (FOAM_PART_DATA[nvFoamSendElev].exist != enFoamExistTop)
			{
				ER.Save(ER_FOAM_FLIP_TABLE_EMPTY);
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

			if (!CYL[cylLtCobotLabelPkUd].IsDn())
				CYL[cylLtCobotLabelPkUd].Dn(LT_COBOT_LABEL_PK_DN);

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_LABEL_ATTACH))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_LABEL_ATTACH))
					ROBOT[robotLt].Move(P_LT_COBOT_LABEL_ATTACH);

				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VAC);
			break;
		}
		case C_LABEL_ATTACH_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VAC", m_strName);

			if (!CYL[cylLtCobotLabelPkUd].IsDn())
			{
				CYL[cylLtCobotLabelPkUd].Dn(LT_COBOT_LABEL_PK_DN);
				break;
			}

			if (!VC[vcLtCobotLabel].IsOff())
				VC[vcLtCobotLabel].Off(LT_COBOT_LABEL_OFF);

			if (!VC[vcLtCobotLabel].AOff() || !VC[vcLtCobotLabel].IsOff())
				break;

			if (!CYL[cylLtCobotLabelPkUd].IsUp())
				CYL[cylLtCobotLabelPkUd].Up(LT_COBOT_LABEL_PK_UP);

			m_pFsm->Set(C_LABEL_ATTACH_UP);
			break;
		}
		case C_LABEL_ATTACH_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_UP", m_strName);

			if (NV.use[useLtCobotContinueSeq])
			{
				if (!CYL[cylLtCobotLabelPkUd].IsUp())
				{
					CYL[cylLtCobotLabelPkUd].Up(LT_COBOT_LABEL_PK_UP);
					break;
				}

				if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_TOP_FOAM_PICK))
				{
					if (ROBOT[robotLt].CanMove(P_LT_COBOT_TOP_FOAM_PICK))
						ROBOT[robotLt].Move(P_LT_COBOT_TOP_FOAM_PICK);

					break;
				}
			}
			else
			{
				if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
				{
					if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
						ROBOT[robotLt].Move(P_LT_COBOT_READY);

					break;
				}

				//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_LABEL_ATTACH_TO_READY))
				//{
				//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_LABEL_ATTACH_TO_READY))
				//		ROBOT[robotLt].Move(P_LT_COBOT_LABEL_ATTACH_TO_READY);

				//	break;
				//}
			}

			if (!CYL[cylLtCobotLabelPkUd].IsUp())
			{
				CYL[cylLtCobotLabelPkUd].Up(LT_COBOT_LABEL_PK_UP);
				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_END);
			break;
		}
		case C_LABEL_ATTACH_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_LABEL) = FALSE;
			FOAM_PART_DATA[nvFoamSendElev].exist = enFoamExistTopLabel;
			memcpy(&FOAM_PART_DATA[nvFoamSendElev].labelInfo, &COBOT_PART_DATA[nvLtCobot].labelInfo[_LABEL], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvLtCobot].labelInfo[_LABEL], sizeof(IPC_MMI::SBoxLabelInfo));
			m_state = "";
			m_pFsm->Set(C_IDLE);

			if (NV.use[useLtCobotContinueSeq])
			{
				if (IsCanCycleTopFoamPick())
					m_pFsm->Set(C_TOP_FOAM_PICK_START);
			}
			break;
		}
	}
}

void CLtCobot::CycleTopFoamPick(void)
{
	if (!m_pFsm->Between(C_TOP_FOAM_PICK_START, C_TOP_FOAM_PICK_END))
		return;

	if (m_pFsm->TimeLimit(90000)) // _40sec
	{
		ER.Save(ER_LT_COBOT_TOP_FOAM_PICK_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_TOP_FOAM_PICK_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PICK_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "TOP_FOAM_PICKUP";
			}

			if (!IsCanCycleTopFoamPick())
				break;

			if (!CylInit())
				break;

			if (!IsEmpty(_FOAM))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_TOP_FOAM_PICK_MOVE);
			break;
		}
		case C_TOP_FOAM_PICK_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PICK_MOVE", m_strName);

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_TOP_FOAM_PICK))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_TOP_FOAM_PICK))
					ROBOT[robotLt].Move(P_LT_COBOT_TOP_FOAM_PICK);

				break;
			}

			m_pFsm->Set(C_TOP_FOAM_PICK_VAC);
			break;
		}
		case C_TOP_FOAM_PICK_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PICK_VAC", m_strName);

			if (m_pFsm->TimeLimit(_15sec))
			{
				if (!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn())
				{
					// 안전조건 추가
					if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_TOP_FOAM_PICK_TO_READY))
					{
						if (ROBOT[robotLt].CanMove(P_LT_COBOT_TOP_FOAM_PICK_TO_READY))
							ROBOT[robotLt].Move(P_LT_COBOT_TOP_FOAM_PICK_TO_READY);

						break;
					}

					ER.Save(ER_VAC_LT_COBOT_TOP_FAOM);
					m_pFsm->Set(C_ERROR);
					break;
				}
			}

			if (!CYL[cylFoamSendElevLtAlignFb].IsBwd())
				CYL[cylFoamSendElevLtAlignFb].Bwd(FOAM_SEND_ELEV_LT_ALIGN_BWD);
			if (!CYL[cylFoamSendElevRtAlignFb].IsBwd())
				CYL[cylFoamSendElevRtAlignFb].Bwd(FOAM_SEND_ELEV_RT_ALIGN_BWD);

			if (!VC[vcLtCobotFoam].IsOn() || !VC[vcLtCobotFoam2].IsOn())
			{
				VC[vcLtCobotFoam].On(LT_COBOT_FOAM_ON);
				VC[vcLtCobotFoam2].On(LT_COBOT_FOAM2_ON);
			}

			if (!CYL[cylFoamSendElevLtAlignFb].IsBwd() ||
				!CYL[cylFoamSendElevRtAlignFb].IsBwd())
				break;

			if (!m_pFsm->TimeLimit(_10sec))
			{
				if (!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn() ||
					!VC[vcLtCobotFoam].IsOn() || !VC[vcLtCobotFoam2].IsOn())
					break;
			}


			m_pFsm->Set(C_TOP_FOAM_PICK_UP);
			break;
		}
		case C_TOP_FOAM_PICK_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PICK_UP", m_strName);

			if (m_pFsm->TimeLimit(_15sec))
			{
				if (!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn())
				{
					ER.Save(ER_VAC_LT_COBOT_TOP_FAOM);
					m_pFsm->Set(C_ERROR);
					break;
				}
			}

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
					ROBOT[robotLt].Move(P_LT_COBOT_READY);

				break;
			}

			//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_TOP_FOAM_PICK_TO_READY))
			//{
			//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_TOP_FOAM_PICK_TO_READY))
			//		ROBOT[robotLt].Move(P_LT_COBOT_TOP_FOAM_PICK_TO_READY);

			//	break;
			//}

			if (!VC[vcLtCobotFoam].AOn() || !VC[vcLtCobotFoam2].AOn() ||
				!VC[vcLtCobotFoam].IsOn() || !VC[vcLtCobotFoam2].IsOn())
				break;

			m_pFsm->Set(C_TOP_FOAM_PICK_END);
			break;
		}
		case C_TOP_FOAM_PICK_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PICK_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			COBOT_PART_DATA[nvLtCobot].exist = enFoamExistTopLabel;
			memcpy(&COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM], &FOAM_PART_DATA[nvFoamSendElev].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&FOAM_PART_DATA[nvFoamSendElev].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&FOAM_PART_DATA[nvFoamSendElev].flag, sizeof(IPC_MMI::FoamFlag));
			FOAM_PART_DATA[nvFoamSendElev].exist = enFoamExistTop;
			FOAM_PART_DATA[nvFoamSendElev].cnt--;
			FOAM_PART_DATA[nvFoamSendElev].flag.levelDid = false;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLtCobot::CycleTopFoamPlace(void)
{
	if (!m_pFsm->Between(C_TOP_FOAM_PLACE_START, C_TOP_FOAM_PLACE_END))
		return;

	if (m_pFsm->TimeLimit(90000)) // _40sec
	{
		ER.Save(ER_LT_COBOT_TOP_FOAM_PLACE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_TOP_FOAM_PLACE_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PLACE_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "TOP_FOAM_PLACE";
				m_nMoveStepCnt = 0;
			}

			if (!IsCanCycleTopFoamPlace())
				break;

			if (!CylInit())
				break;

			if (!IsExist(_FOAM))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (MT_INPOS != MtPosMove(mtTrayShuttleY, P_TRAY_SHUTTLE_Y_SEND))
				break;

			m_pFsm->Set(C_TOP_FOAM_PLACE_MOVE);
			break;
		}
		case C_TOP_FOAM_PLACE_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PLACE_MOVE", m_strName);

			// Subtract the current tray quantity from the maximum tray quantity.
			double Zoffset = IN_PK.GetCurTopFoamViZOffset();

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_TOP_FOAM_PLACE))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_TOP_FOAM_PLACE))
					ROBOT[robotLt].Move(P_LT_COBOT_TOP_FOAM_PLACE, Zoffset);

				break;
			}

			m_pFsm->Set(C_TOP_FOAM_PLACE_VAC);
			break;
		}
		case C_TOP_FOAM_PLACE_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PLACE_VAC", m_strName);

			if (!VC[vcLtCobotFoam].IsOff() || !VC[vcLtCobotFoam2].IsOff())
			{
				VC[vcLtCobotFoam].Off(LT_COBOT_FOAM_OFF);
				VC[vcLtCobotFoam2].Off(LT_COBOT_FOAM2_OFF);
			}

			if (!VC[vcLtCobotFoam].AOff() || !VC[vcLtCobotFoam2].AOff() ||
				!VC[vcLtCobotFoam].IsOff() || !VC[vcLtCobotFoam2].IsOff())
				break;

			m_pFsm->Set(C_TOP_FOAM_PLACE_Z_UP);
			break;
		}
		case C_TOP_FOAM_PLACE_Z_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PLACE_Z_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commFoamPadTransferEnd] = COMM_REQ;
			}


			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
					ROBOT[robotLt].Move(P_LT_COBOT_READY);

				break;
			}

			//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_TOP_FOAM_PLACE_TO_READY))
			//{
			//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_TOP_FOAM_PLACE_TO_READY))
			//		ROBOT[robotLt].Move(P_LT_COBOT_TOP_FOAM_PLACE_TO_READY);

			//	break;
			//}

			m_pFsm->Set(C_TOP_FOAM_PLACE_END);
			break;
		}
		case C_TOP_FOAM_PLACE_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TOP_FOAM_PLACE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_FOAM) = FALSE;
			TRAY_PART_DATA[nvTopFoam].exist = enExistTrayTopFoam;
			memcpy(&FOAM_PART_DATA[nvTopFoam].labelInfo, &COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvLtCobot].flagLt, sizeof(IPC_MMI::LtCobotFlag));

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLtCobot::CycleFoamReject(void)
{
	if (!m_pFsm->Between(C_FOAM_REJECT_START, C_FOAM_REJECT_END))
		return;

	if (m_pFsm->TimeLimit(90000)) // _40sec
	{
		ER.Save(ER_LT_COBOT_FOAM_REJECT_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_FOAM_REJECT_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_REJECT_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "FOAM_REJECT";
			}

			if (!IsCanCycleFoamReject())
				break;

			if (!CylInit())
				break;

			if (!IsExist(_FOAM))
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_FOAM_REJECT_MOVE);
			break;
		}
		case C_FOAM_REJECT_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_REJECT_MOVE", m_strName);

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_FOAM_REJECT))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_FOAM_REJECT))
					ROBOT[robotLt].Move(P_LT_COBOT_FOAM_REJECT);

				break;
			}

			m_pFsm->Set(C_FOAM_REJECT_VAC);
			break;
		}
		case C_FOAM_REJECT_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_REJECT_VAC", m_strName);

			if (!VC[vcLtCobotFoam].IsOff() || !VC[vcLtCobotFoam2].IsOff())
			{
				VC[vcLtCobotFoam].Off(LT_COBOT_FOAM_OFF);
				VC[vcLtCobotFoam2].Off(LT_COBOT_FOAM2_OFF);
			}

			if (!VC[vcLtCobotFoam].AOff() || !VC[vcLtCobotFoam2].AOff() ||
				!VC[vcLtCobotFoam].IsOff() || !VC[vcLtCobotFoam2].IsOff())
				break;

			m_pFsm->Set(C_FOAM_REJECT_UP);
			break;
		}
		case C_FOAM_REJECT_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_REJECT_UP", m_strName);

			if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_READY))
			{
				if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
					ROBOT[robotLt].Move(P_LT_COBOT_READY);

				break;
			}

			//if (!ROBOT[robotLt].IsRdy(P_LT_COBOT_FOAM_REJECT_TO_READY))
			//{
			//	if (ROBOT[robotLt].CanMove(P_LT_COBOT_FOAM_REJECT_TO_READY))
			//		ROBOT[robotLt].Move(P_LT_COBOT_FOAM_REJECT_TO_READY);

			//	break;
			//}

			m_pFsm->Set(C_FOAM_REJECT_END);
			break;
		}
		case C_FOAM_REJECT_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_REJECT_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist(_FOAM) = FALSE;
			ZeroMemory(&COBOT_PART_DATA[nvLtCobot].labelInfo[_FOAM], sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&COBOT_PART_DATA[nvLtCobot].flagLt, sizeof(IPC_MMI::LtCobotFlag));

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}
