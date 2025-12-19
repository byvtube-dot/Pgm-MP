#include "..\..\Includes.h"

CFoldPk FOLD_PK;

CFoldPk::CFoldPk()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CFoldPk::~CFoldPk()
{

}

void CFoldPk::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = static_cast<string> (TRAY_PART_DATA[nvMbbFold].labelInfo.sBoxId);
	if (0 == strcmp(TRAY_PART_DATA[nvMbbFold].labelInfo.sBoxId, ""))
		MatId = "$";

	if ("PICKUP" == newValue)
		FOLD_PK.m_pSLog->XFR(newValue, "$", "BTM_FOLD_CONV", "FOLD_PK");
	else if ("PLACE" == newValue)
	{
		MatId = static_cast<string> (TRAY_PART_DATA[nvMbbFoldPk].labelInfo.sBoxId);
		if (0 == strcmp(TRAY_PART_DATA[nvMbbFoldPk].labelInfo.sBoxId, ""))
			MatId = "$";
		FOLD_PK.m_pSLog->XFR(newValue, "$", "FOLD_PK", "LEAK_INSP_ZONE");
	}
	else
		FOLD_PK.m_pSLog->XFR(newValue, "$", "", "");
}

void CFoldPk::Auto(void)
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
		if (!IsCanCyclePickUp())
			return;

		m_pFsm->Set(C_PICK_UP_START);
		return;
	}
	else
	{
		if (!IsCanCyclePlace())
			return;

		m_pFsm->Set(C_PLACE_START);
		return;
	}
}

void CFoldPk::Cycle(void)
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

	CyclePickUp();
	CyclePlace();
}

void CFoldPk::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "FoldPk");
	m_pFsm = &FSM[fsmFoldPk];

	m_pSLog = &SLOG[slogFoldPk];
	m_pSLog->SetXFRDeviceId("FOLD_PK");
	m_pSLog->SetXFRMaterialType("SEALED_MBB");

	m_pSLog->Add(mtMbbFoldPkX, TRUE);
	m_pSLog->Add(mtMbbFoldPkZ, TRUE);

	// Bottom Fold Part
	m_pSLog->Add(cylMbbFoldAssistUd, TRUE);
	m_pSLog->Add(cylMbbFoldAlignFtFb, TRUE);
	m_pSLog->Add(cylMbbFoldAlignRrFb, TRUE);
	m_pSLog->Add(cylMbbFoldRtFb, TRUE);
	m_pSLog->Add(cylMbbFoldRtFtOc, TRUE);
	m_pSLog->Add(cylMbbFoldRtRrOc, TRUE);

	m_pSLog->Add(cylMbbFoldPkLtBlockTurnFb, TRUE);
	m_pSLog->Add(cylMbbFoldPkLtBlockFtOc, TRUE);
	m_pSLog->Add(cylMbbFoldPkLtBlockRrOc, TRUE);
	m_pSLog->Add(cylMbbFoldPkClampOc, TRUE);

	m_pSLog->Add(CV_MBB_FOLD, TRUE);
}

BOOL CFoldPk::Common(void)
{
	if (Exist() && !m_pFsm->IsBusy())
	{
		On(oSmema11Req);
	}

	if (MT[mtMbbFoldPkZ].InPos(P_MBB_FOLD_PK_Z_READY) ||
		(!MT[mtMbbFoldPkZ].InPos(P_MBB_FOLD_PK_Z_READY) && MT[mtMbbFoldPkX].m_state.isHome && !MT[mtMbbFoldPkX].InPos(P_MBB_FOLD_PK_X_PLACE)))
		On(oSmema13Safety);
	else
		Off(oSmema13Safety);

	return TRUE;
}

BOOL CFoldPk::IsHwErr(void)
{
	if (!MT[mtMbbFoldPkX].m_state.isHome)
		return TRUE;
	if (!MT[mtMbbFoldPkZ].m_state.isHome)
		return TRUE;

	if (CYL[cylMbbFoldPkLtBlockTurnFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldPkLtBlockFtOc].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldPkLtBlockRrOc].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldPkClampOc].GetErr())
		return TRUE;

	// Bottom Fold Part
	if (CYL[cylMbbFoldAssistUd].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldAlignFtFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldAlignRrFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldRtFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldRtFtOc].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldRtRrOc].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CFoldPk::IsHwRdy(void)
{
	if (!MT[mtMbbFoldPkX].IsRdy())
		return TRUE;
	if (!MT[mtMbbFoldPkZ].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CFoldPk::CylInit(void)
{
	BOOL isCylInitDone = TRUE;

	if (!CYL[cylMbbFoldPkLtBlockTurnFb].IsBwd())
	{
		CYL[cylMbbFoldPkLtBlockTurnFb].Bwd(MBB_FOLD_PK_LT_BLOCK_TURN_BWD);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylMbbFoldPkLtBlockFtOc].IsOpen() || !CYL[cylMbbFoldPkLtBlockRrOc].IsOpen())
	{
		CYL[cylMbbFoldPkLtBlockFtOc].Open(MBB_FOLD_PK_LT_BLOCK_FT_OPEN);
		CYL[cylMbbFoldPkLtBlockRrOc].Open(MBB_FOLD_PK_LT_BLOCK_RR_OPEN);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylMbbFoldRtFb].IsBwd())
	{
		CYL[cylMbbFoldRtFb].Bwd(MBB_FOLD_RT_BWD);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylMbbFoldPkLtFtAssist].IsBwd())
	{
		CYL[cylMbbFoldPkLtFtAssist].Bwd(MBB_FOLD_PK_LT_ASSIST_BWD);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylMbbFoldPkLtRrAssist].IsBwd())
	{
		CYL[cylMbbFoldPkLtRrAssist].Bwd(MBB_FOLD_PK_LT_ASSIST_BWD);
		isCylInitDone = FALSE;
	}

	if (Exist() && !CYL[cylMbbFoldPkClampOc].IsClose())
	{
		CYL[cylMbbFoldPkClampOc].Close(MBB_FOLD_PK_CLAMP_CLOSE);
		isCylInitDone = FALSE;
	}

	if (!Exist() && !CYL[cylMbbFoldPkClampOc].IsOpen())
	{
		CYL[cylMbbFoldPkClampOc].Open(MBB_FOLD_PK_CLAMP_OPEN);
		isCylInitDone = FALSE;
	}

	return isCylInitDone;
}

BOOL CFoldPk::IsCanCyclePickUp(void)
{
	if (BTM_FOLD_CONV.Exist() != enExistSealedQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Btm Fold Conv Memory Mismatch");
		return FALSE;
	}

	if (Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Fold Pk Memory Exist");
		return FALSE;
	}

	return TRUE;
}

BOOL CFoldPk::IsCanCyclePlace(void)
{
	if (AOff(iSmema11Receive))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"iSmema11Receive Off");
		return FALSE;
	}

	if (!Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Fold Pk Memory Empty");
		return FALSE;
	}

	return TRUE;
}

BOOL CFoldPk::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy())
	{
		if (!Exist() && (AOn(iMbbFoldPkLtExist) || AOn(iMbbFoldPkRtExist)))
		{
			ER.Save(ER_MBB_FOLD_PK_SENSOR_STATUS);
			return TRUE;
		}
		else if (Exist() && AOff(iMbbFoldPkLtExist) && AOff(iMbbFoldPkRtExist))
		{
			ER.Save(ER_MBB_FOLD_PK_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CFoldPk::Exist(void)
{
	return TRAY_PART_DATA[nvMbbFoldPk].exist;
}

BOOL CFoldPk::IsExist(void)
{
	if (!TRAY_PART_DATA[nvMbbFoldPk].exist || AOff(iMbbFoldPkLtExist) || AOff(iMbbFoldPkRtExist))
	{
		ER.Save(ER_MBB_FOLD_PK_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoldPk::IsEmpty(void)
{
	if (TRAY_PART_DATA[nvMbbFoldPk].exist || AOn(iMbbFoldPkLtExist) || AOn(iMbbFoldPkRtExist))
	{
		ER.Save(ER_MBB_FOLD_PK_EXIST);
		return FALSE;
	}

	return TRUE;
}

void CFoldPk::CyclePickUp(void)
{
	if (!m_pFsm->Between(C_PICK_UP_START, C_PICK_UP_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_FOLD_PK_PICK_UP_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_PICK_UP_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PICK_UP_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "PICKUP";
				NV.ndm[commMbbFoldingStart] = COMM_IDLE;
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY))
				break;

			if (!CylInit() || !BTM_FOLD_CONV.CylInit())
				break;

			if (!IsEmpty() || !BTM_FOLD_CONV.IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_PICK_UP_RIGHT_ATTACH_READY_MOVE);
			break;
		}
		case C_PICK_UP_RIGHT_ATTACH_READY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_RIGHT_ATTACH_READY_MOVE", m_strName);

			if (!CYL[cylMbbFoldAlignFtFb].IsFwd() || !CYL[cylMbbFoldAlignRrFb].IsFwd())
			{
				CYL[cylMbbFoldAlignFtFb].Fwd(MBB_FOLD_ALIGN_FT_FWD);
				CYL[cylMbbFoldAlignRrFb].Fwd(MBB_FOLD_ALIGN_RR_FWD);
			}

			if (!CYL[cylMbbFoldPkLtBlockTurnFb].IsFwd())
				CYL[cylMbbFoldPkLtBlockTurnFb].Fwd(MBB_FOLD_PK_LT_BLOCK_TURN_FWD);

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_RIGHT_ATTACH_START))
				break;

			if (!CYL[cylMbbFoldAlignFtFb].IsFwd() || !CYL[cylMbbFoldAlignRrFb].IsFwd())
				break;

			m_pFsm->Set(C_PICK_UP_ALIGN_Z_MOVE);
			break;
		}
		case C_PICK_UP_ALIGN_Z_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_ALIGN_Z_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_RIGHT_ATTACH))
				break;

			if (!CYL[cylMbbFoldAlignFtFb].IsBwd() || !CYL[cylMbbFoldAlignRrFb].IsBwd())
			{
				CYL[cylMbbFoldAlignFtFb].Bwd(MBB_FOLD_ALIGN_FT_BWD);
				CYL[cylMbbFoldAlignRrFb].Bwd(MBB_FOLD_ALIGN_RR_BWD);
				break;
			}

			if (!CYL[cylMbbFoldPkLtBlockTurnFb].IsFwd())
			{
				CYL[cylMbbFoldPkLtBlockTurnFb].Fwd(MBB_FOLD_PK_LT_BLOCK_TURN_FWD);
				break;
			}

			if (!CYL[cylMbbFoldRtFb].IsFwd())
				CYL[cylMbbFoldRtFb].Fwd(MBB_FOLD_RT_FWD);

			m_pFsm->Set(C_PICK_UP_RIGHT_ATTACH_MOVE);
			break;
		}
		case C_PICK_UP_RIGHT_ATTACH_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_ALIGN_Z_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_RIGHT_ATTACH_END))
				break;

			m_pFsm->Set(C_PICK_UP_RT_CLAMP);
			break;
		}
		case C_PICK_UP_RT_CLAMP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_RT_CLAMP", m_strName);

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_RIGHT_CLAMP))
				break;

			if (!CYL[cylMbbFoldRtFb].IsFwd())
			{
				CYL[cylMbbFoldRtFb].Fwd(MBB_FOLD_RT_FWD);
				break;
			}
			else
			{
				if (!CYL[cylMbbFoldAlignFtFb].IsFwd() || !CYL[cylMbbFoldAlignRrFb].IsFwd())
				{
					CYL[cylMbbFoldAlignFtFb].Fwd(MBB_FOLD_ALIGN_FT_FWD);
					CYL[cylMbbFoldAlignRrFb].Fwd(MBB_FOLD_ALIGN_RR_FWD);
					break;
				}

				if (!CYL[cylMbbFoldRtFtOc].IsClose() || !CYL[cylMbbFoldRtRrOc].IsClose())
				{
					CYL[cylMbbFoldRtFtOc].Close(MBB_FOLD_RT_FT_CLOSE);
					CYL[cylMbbFoldRtRrOc].Close(MBB_FOLD_RT_RR_CLOSE);
					break;
				}
			}

			m_pFsm->Set(C_PICK_UP_MOVE);
			break;
		}

		case C_PICK_UP_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_MOVE", m_strName);

			if (!CYL[cylMbbFoldPkLtBlockTurnFb].IsBwd())
			{
				CYL[cylMbbFoldPkLtBlockTurnFb].Bwd(MBB_FOLD_PK_LT_BLOCK_TURN_BWD);
				break;
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY))
				break;

			if (!CYL[cylMbbFoldAlignFtFb].IsFwd() || !CYL[cylMbbFoldAlignRrFb].IsFwd())
			{
				CYL[cylMbbFoldAlignFtFb].Fwd(MBB_FOLD_ALIGN_FT_FWD);
				CYL[cylMbbFoldAlignRrFb].Fwd(MBB_FOLD_ALIGN_RR_FWD);
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_LEFT_TURN))
				break;

			if (!CYL[cylMbbFoldAlignFtFb].IsFwd() || !CYL[cylMbbFoldAlignRrFb].IsFwd())
				break;

			m_pFsm->Set(C_PICK_UP_Z_DN);
			break;
		}
		case C_PICK_UP_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_Z_DN", m_strName);

			if (!CYL[cylMbbFoldAlignFtFb].IsFwd() || !CYL[cylMbbFoldAlignRrFb].IsFwd())
			{
				CYL[cylMbbFoldAlignFtFb].Fwd(MBB_FOLD_ALIGN_FT_FWD);
				CYL[cylMbbFoldAlignRrFb].Fwd(MBB_FOLD_ALIGN_RR_FWD);
				break;
			}

			double Zoffset = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET);
			double pos = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_PICK) - Zoffset;
			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_PICK, pos))
				break;

			m_pFsm->Set(C_PICK_UP_Z_SLOW_DN);
			break;
		}
		case C_PICK_UP_Z_SLOW_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_Z_SLOW_DN", m_strName);

			double pos = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_PICK);
			double vel = MT[mtMbbFoldPkZ].VelTable(P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET);
			double acc = MT[mtMbbFoldPkZ].AccTable(P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET);
			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_PICK, pos, vel, acc))
				break;

			m_pFsm->Set(C_PICK_UP_FOLD);
			break;
		}
		case C_PICK_UP_FOLD:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_BTM_FOLD", m_strName);

			if (!CYL[cylMbbFoldPkLtBlockTurnFb].IsFwd())
			{
				if (!CYL[cylMbbFoldAssistUd].IsUp())
				{
					CYL[cylMbbFoldAssistUd].Up(MBB_FOLD_ASSIST_UP);
					break;
				}

				CYL[cylMbbFoldPkLtBlockTurnFb].Fwd(MBB_FOLD_PK_LT_BLOCK_TURN_FWD);
				break;
			}
			else
			{
				if (!CYL[cylMbbFoldAssistUd].IsDn())
				{
					CYL[cylMbbFoldAssistUd].Dn(MBB_FOLD_ASSIST_DN);
					break;
				}
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_LEFT_TURN_CLAMP))
				break;

			m_pFsm->Set(C_PICK_UP_FOLD_CLAMP);
			break;
		}
		case C_PICK_UP_FOLD_CLAMP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_FOLD_CLAMP", m_strName);

			if (!CYL[cylMbbFoldPkLtFtAssist].IsFwd() || !CYL[cylMbbFoldPkLtRrAssist].IsFwd())
			{
				if (!CYL[cylMbbFoldPkLtBlockFtOc].IsClose() || !CYL[cylMbbFoldPkLtBlockRrOc].IsClose())
				{
					CYL[cylMbbFoldPkLtBlockFtOc].Close(MBB_FOLD_PK_LT_BLOCK_FT_CLOSE);
					CYL[cylMbbFoldPkLtBlockRrOc].Close(MBB_FOLD_PK_LT_BLOCK_RR_CLOSE);
					break;
				}

				CYL[cylMbbFoldPkLtFtAssist].Fwd(MBB_FOLD_PK_LT_ASSIST_FWD);
				CYL[cylMbbFoldPkLtRrAssist].Fwd(MBB_FOLD_PK_LT_ASSIST_FWD);
				break;
			}
			else
			{
				if (!CYL[cylMbbFoldPkLtBlockFtOc].IsOpen() || !CYL[cylMbbFoldPkLtBlockRrOc].IsOpen())
				{
					CYL[cylMbbFoldPkLtBlockFtOc].Open(MBB_FOLD_PK_LT_BLOCK_FT_OPEN);
					CYL[cylMbbFoldPkLtBlockRrOc].Open(MBB_FOLD_PK_LT_BLOCK_RR_OPEN);
					break;
				}
			}
			
			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_CLAMP))
				break;

			if (!CYL[cylMbbFoldPkClampOc].IsClose())
			{
				CYL[cylMbbFoldPkClampOc].Close(MBB_FOLD_PK_CLAMP_CLOSE);
				break;
			}

			m_pFsm->Set(C_PICK_UP_FOLD_INIT);
			break;
		}
			
		case C_PICK_UP_FOLD_INIT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_FOLD_INIT", m_strName);

			if (!CYL[cylMbbFoldPkLtFtAssist].IsBwd() || !CYL[cylMbbFoldPkLtRrAssist].IsBwd())
			{
				CYL[cylMbbFoldPkLtFtAssist].Bwd(MBB_FOLD_PK_LT_ASSIST_BWD);
				CYL[cylMbbFoldPkLtRrAssist].Bwd(MBB_FOLD_PK_LT_ASSIST_BWD);
			}

			if (!CYL[cylMbbFoldPkLtBlockFtOc].IsOpen() || !CYL[cylMbbFoldPkLtBlockRrOc].IsOpen())
			{
				CYL[cylMbbFoldPkLtBlockFtOc].Open(MBB_FOLD_PK_LT_BLOCK_FT_OPEN);
				CYL[cylMbbFoldPkLtBlockRrOc].Open(MBB_FOLD_PK_LT_BLOCK_RR_OPEN);
			}

			if (!CYL[cylMbbFoldRtFtOc].IsOpen() || !CYL[cylMbbFoldRtRrOc].IsOpen())
			{
				CYL[cylMbbFoldRtFtOc].Open(MBB_FOLD_RT_FT_OPEN);
				CYL[cylMbbFoldRtRrOc].Open(MBB_FOLD_RT_RR_OPEN);
			}

			if (!CYL[cylMbbFoldPkLtFtAssist].IsBwd() || !CYL[cylMbbFoldPkLtRrAssist].IsBwd() ||
				!CYL[cylMbbFoldPkLtBlockFtOc].IsOpen() || !CYL[cylMbbFoldPkLtBlockRrOc].IsOpen() ||
				!CYL[cylMbbFoldRtFtOc].IsOpen() || !CYL[cylMbbFoldRtRrOc].IsOpen())
				break;


			if (!CYL[cylMbbFoldAlignFtFb].IsBwd() || !CYL[cylMbbFoldAlignRrFb].IsBwd())
			{
				CYL[cylMbbFoldAlignFtFb].Bwd(MBB_FOLD_ALIGN_FT_BWD);
				CYL[cylMbbFoldAlignRrFb].Bwd(MBB_FOLD_ALIGN_RR_BWD);
			}

			if (!CYL[cylMbbFoldRtFb].IsBwd())
				CYL[cylMbbFoldRtFb].Bwd(MBB_FOLD_RT_BWD);

			if (!CYL[cylMbbFoldPkLtBlockTurnFb].IsBwd())
				CYL[cylMbbFoldPkLtBlockTurnFb].Bwd(MBB_FOLD_PK_LT_BLOCK_TURN_BWD);

			if (!CYL[cylMbbFoldAlignFtFb].IsBwd() || !CYL[cylMbbFoldAlignRrFb].IsBwd() ||
				!CYL[cylMbbFoldRtFb].IsBwd() || !CYL[cylMbbFoldPkLtBlockTurnFb].IsBwd())
				break;

			m_pFsm->Set(C_PICK_UP_Z_SLOW_UP);
			break;
		}
		case C_PICK_UP_Z_SLOW_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PICK_UP_Z_SLOW_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbFoldingStart] = COMM_REQ;
			}

			double Zoffset = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET);
			double pos = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_PICK) - Zoffset;
			double vel = MT[mtMbbFoldPkZ].VelTable(P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET);
			double acc = MT[mtMbbFoldPkZ].AccTable(P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET);
			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY, pos, vel, acc))
				break;

			m_pFsm->Set(C_PICK_UP_Z_UP);
			break;
		}
		case C_PICK_UP_Z_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PICK_UP_INIT", m_strName);

			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY))
				break;

			m_pFsm->Set(C_PICK_UP_END);
			break;
		}
		case C_PICK_UP_END:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PICK_UP_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
				memcpy(&TRAY_PART_DATA[nvMbbFoldPk], &TRAY_PART_DATA[nvMbbFold], sizeof(IPC_MMI::TrayPartData));
				ZeroMemory(&TRAY_PART_DATA[nvMbbFold], sizeof(IPC_MMI::TrayPartData));
			}

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CFoldPk::CyclePlace(void)
{
	if (!m_pFsm->Between(C_PLACE_START, C_PLACE_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_FOLD_PK_PLACE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_PLACE_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PLACE_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "PLACE";

				NV.ndm[reqNextMcDataSend] = COMM_IDLE;
				NV.ndm[commMbbFoldingEnd] = COMM_IDLE;
				NV.ndm[commTrayOutStart] = COMM_REQ;
				NV.ndm[commTrayOutEnd] = COMM_IDLE;
				m_nDataSendCnt = 0;
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_IN_PK_Z_READY))
				break;

			if (!CylInit())
				break;

			if (AOff(iSmema11Receive))
			{
				if (AOff(iSmema11Receive, _3sec))
				{
					ER.Save(ER_NEXT_MC_NOT_READY);
					m_pFsm->Set(C_ERROR);
				}
				break;
			}

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_PLACE_MOVE);
			break;
		}
		case C_PLACE_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PLACE_MOVE", m_strName);

			if (m_nDataSendCnt > 0)
			{
				if (!m_pFsm->TimeLimit(_5sec))
					break;
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_PLACE))
				break;

			m_pFsm->Set(C_PLACE_DATA_SEND);
			break;
		}
		case C_PLACE_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PLACE_DATA_SEND", m_strName);
				NV.ndm[reqNextMcDataSend] = COMM_REQ;
			}

			if (NV.ndm[reqNextMcDataSend] == COMM_ERR)
			{
				ER.Save(ER_NEXT_MC_DATA_SEND_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (m_pFsm->TimeLimit(_5sec))
			{
				if (m_nDataSendCnt < 2)
				{
					m_nDataSendCnt++;
					NV.ndm[reqNextMcDataSend] = COMM_IDLE;
					m_pFsm->Set(C_PLACE_MOVE);
				}
				else
				{
					ER.Save(ER_NEXT_MC_DATA_SEND_TIME_OVER);
					m_pFsm->Set(C_ERROR);
				}
				break;
			}

			if (NV.ndm[reqNextMcDataSend] != COMM_COMP)
				break;

			On(oSmema12Comp);
			m_pFsm->Set(C_PLACE_Z_DN);
			break;
		}
		case C_PLACE_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PLACE_Z_DN", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				ER.Save(ER_NEXT_MC_COMP_RECV_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (AOn(iSmema12Comp, 100) || (OPR.isDryRun && AOn(iSmema12Comp)))
			{
				double Zoffset = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET);
				double pos = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_PLACE) - Zoffset;
				if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_PLACE, pos))
					break;

				m_pFsm->Set(C_PLACE_Z_SLOW_DN);
			}

			break;
		}
		case C_PLACE_Z_SLOW_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PLACE_Z_SLOW_DN", m_strName);

			double pos = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_PLACE);
			double vel = MT[mtMbbFoldPkZ].VelTable(P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET);
			double acc = MT[mtMbbFoldPkZ].AccTable(P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET);
			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_PLACE, pos, vel, acc))
				break;

			if (!CYL[cylMbbFoldPkClampOc].IsOpen())
			{
				CYL[cylMbbFoldPkClampOc].Open(MBB_FOLD_PK_CLAMP_OPEN);
				break;
			}

			m_pFsm->Set(C_PLACE_Z_SLOW_UP);
			break;
		}
		case C_PLACE_Z_SLOW_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PLACE_Z_SLOW_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbFoldingEnd] = COMM_REQ;
			}

			double Zoffset = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET);
			double pos = MT[mtMbbFoldPkZ].PosTable(P_MBB_FOLD_PK_Z_PLACE) - Zoffset;
			double vel = MT[mtMbbFoldPkZ].VelTable(P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET);
			double acc = MT[mtMbbFoldPkZ].AccTable(P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET);
			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY, pos, vel, acc))
				break;

			m_pFsm->Set(C_PLACE_Z_UP);
			break;
		}
		case C_PLACE_Z_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PLACE_Z_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commTrayOutEnd] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (!AOff(iSmema11Receive) || !AOff(iSmema12Comp))
					ER.Save(ER_NEXT_MC_SENSOR_OFF_TIME_OVER);
				else
					ER.Save(ER_FOLD_PK_PLACE_Z_UP_CYCLE_TIME_OVER);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (MT_INPOS != MtPosMove(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY))
				break;

			if (!IsOff(oSmema11Req))
				Off(oSmema11Req);
			if (!IsOff(oSmema12Comp))
				Off(oSmema12Comp);

			if (MT_INPOS != MtPosMove(mtMbbFoldPkX, P_MBB_FOLD_PK_X_PICK))
				break;

			if (!IsOff(oSmema11Req) || !IsOff(oSmema12Comp) ||
				!AOff(iSmema11Receive) || !AOff(iSmema12Comp))
				break;

			m_pFsm->Set(C_PLACE_END);
			break;
		}
		case C_PLACE_END:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PLACE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());
				ZeroMemory(&TRAY_PART_DATA[nvMbbFoldPk], sizeof(IPC_MMI::TrayPartData));
			}

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}