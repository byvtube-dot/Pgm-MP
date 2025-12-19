#include "..\..\Includes.h"

CFoamPk FOAM_PK;

CFoamPk::CFoamPk()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CFoamPk::~CFoamPk()
{

}

void CFoamPk::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	if ("NORMAL_PICKUP" == newValue)
		FOAM_PK.m_pSLog->XFR(newValue, "$", "FOAM_CST", "FOAM_PK");
	else if ("PARTIAL_PICKUP" == newValue)
		FOAM_PK.m_pSLog->XFR(newValue, "$", "FOAM_PARTIAL_CST", "FOAM_PK");
	else if ("NORMAL_PLACE" == newValue)
		FOAM_PK.m_pSLog->XFR(newValue, "$", "FOAM_PK", "FOAM_STACK");
	else if ("PARTIAL_PLACE" == newValue)
		FOAM_PK.m_pSLog->XFR(newValue, "$", "FOAM_PK", "FOAM_PARTIAL_STACK");
	else
		FOAM_PK.m_pSLog->XFR(newValue, "$", "", "");

	if ("" == newValue)
	{
		FOAM_PK.m_pSLog->RecordingOff(mtFoamPkX);
		FOAM_PK.m_pSLog->RecordingOff(mtFoamPkY);
		FOAM_PK.m_pSLog->RecordingOff(mtFoamPkY2);
		FOAM_PK.m_pSLog->RecordingOff(mtFoamPkZ);
		FOAM_PK.m_pSLog->RecordingOff(vcFoamPk);
		FOAM_PK.m_pSLog->RecordingOff(vcFoamPk2);
	}
	else
	{
		FOAM_PK.m_pSLog->RecordingOn(mtFoamPkX);
		FOAM_PK.m_pSLog->RecordingOn(mtFoamPkY);
		FOAM_PK.m_pSLog->RecordingOn(mtFoamPkY2);
		FOAM_PK.m_pSLog->RecordingOn(mtFoamPkZ);
		FOAM_PK.m_pSLog->RecordingOn(vcFoamPk);
		FOAM_PK.m_pSLog->RecordingOn(vcFoamPk2);
	}
}

void CFoamPk::Auto(void)
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
		bool bPartialFoamPick = FALSE;
		if (FOAM_PART_DATA[nvFoamRrStack].exist || FOAM_PART_DATA[nvFoamFtStack].exist || FOAM_PART_DATA[nvFoamSendElev].exist)
		{
			if (!FOAM_PART_DATA[nvFoamPartialElev].flag.recvFullDid &&
				FOAM_PART_DATA[nvFoamRtCst].exist && !FOAM_PART_DATA[nvFoamRtCst].flag.cstMaterialEmpty)
				bPartialFoamPick = TRUE;
		}

		if (!IsCanCyclePickUp(bPartialFoamPick))
			return;

		if (bPartialFoamPick)
			m_pFsm->Set(C_FOAM_PICK_UP_START, msgRtFoamBox);
		else if (!FOAM_PART_DATA[nvFoamRecvElev].flag.recvFullDid &&
			FOAM_PART_DATA[nvFoamLtCst].exist && !FOAM_PART_DATA[nvFoamLtCst].flag.cstMaterialEmpty)
			m_pFsm->Set(C_FOAM_PICK_UP_START, msgLtFoamBox);

		return;
	}
	else
	{
		if (!IsCanCyclePlace())
			return;

		if (Exist() == enFoamExistTop)
			m_pFsm->Set(C_FOAM_PLACE_START, msgFoamRecvElev);
		else if (Exist() == enFoamExistBtmPartial)
			m_pFsm->Set(C_FOAM_PLACE_START, msgFoamPartialElev);

		return;
	}
}

void CFoamPk::Cycle(void)
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

	CycleFoamPick();
	CycleFoamPlace();
}

void CFoamPk::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "FoamPk");
	m_pFsm = &FSM[fsmFoamPk];

	m_pSLog = &SLOG[slogFoamPk];
	m_pSLog->SetXFRDeviceId("FOAM_PK");
	m_pSLog->SetXFRMaterialType("FOAM");

	m_pSLog->Add(mtFoamPkX);
	m_pSLog->Add(mtFoamPkY);
	m_pSLog->Add(mtFoamPkY2);
	m_pSLog->Add(mtFoamPkZ);
	m_pSLog->Add(vcFoamPk);
	m_pSLog->Add(vcFoamPk2);

}

BOOL CFoamPk::Common(void)
{
	NV.ddm[foamPkLaserDisplacement] = VC[vcFoamPkLaser].m_state.value;

	return TRUE;
}

BOOL CFoamPk::IsHwErr(void)
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

BOOL CFoamPk::IsHwRdy(void)
{
	if (m_pFsm->Get() != C_FOAM_PICK_UP_VERIFY_MOVE)
	{
		if (!MT[mtFoamPkX].IsRdy())
			return TRUE;
	}
	if (!MT[mtFoamPkY].IsRdy())
		return TRUE;
	//if (!MT[mtFoamPkY2].IsRdy())
	//	return TRUE;
	if (!MT[mtFoamPkZ].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CFoamPk::CylInit(void)
{
	if (Exist())
	{
		if (!VC[vcFoamPk].IsOn() || !VC[vcFoamPk2].IsOn())
		{
			VC[vcFoamPk].On(FOAM_PK_ON);
			VC[vcFoamPk2].On(FOAM_PK2_ON);
		}
	}
	else
	{
		if ((!VC[vcFoamPk].AOn() && !VC[vcFoamPk].IsOff()) ||
			(!VC[vcFoamPk2].AOn() && !VC[vcFoamPk2].IsOff()))
		{
			VC[vcFoamPk].Off(FOAM_PK_OFF);
			VC[vcFoamPk2].Off(FOAM_PK2_OFF);
		}
	}

	return TRUE;
}

BOOL CFoamPk::IsCanCyclePickUp(BOOL bPartialFoam)
{
	if (FOAM_CST[enFoamBoxLt].m_pFsm->IsBusy() || FOAM_CST[enFoamBoxRt].m_pFsm->IsBusy() ||
		AOn(FOAM_CST[enFoamBoxLt].iPioValId) || AOn(FOAM_CST[enFoamBoxRt].iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"FoamCst Cycle is Busy");
		return FALSE;
	}

	if (!bPartialFoam)
	{
		// Normal Foam 
		if (!FOAM_CST[enFoamBoxLt].Exist())
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Left Cst Empty");
			return FALSE;
		}
		if (FOAM_PART_DATA[nvFoamRecvElev].flag.recvFullDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Normal Foam Recv Elev Full");
			return FALSE;
		}
		if (!FOAM_PART_DATA[nvFoamLtCst].exist)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Left Foam Cst Empty");
			return FALSE;
		}
		if (FOAM_PART_DATA[nvFoamLtCst].flag.cstMaterialEmpty)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Left Foam Cst Flag Empty");
			return FALSE;
		}
	}
	else
	{
		if (!FOAM_CST[enFoamBoxRt].Exist())
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Right Cst Empty");
			return FALSE;
		}
		if (FOAM_PART_DATA[nvFoamPartialElev].flag.recvFullDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Partial Foam Recv Elev Full");
			return FALSE;
		}
		if (!FOAM_PART_DATA[nvFoamRtCst].exist)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Right Foam Cst Empty");
			return FALSE;
		}
		if (FOAM_PART_DATA[nvFoamRtCst].flag.cstMaterialEmpty)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Right Foam Cst Flag Empty");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFoamPk::IsCanCyclePlace(void)
{
	if (FOAM_CST[enFoamBoxLt].m_pFsm->IsBusy() || FOAM_CST[enFoamBoxRt].m_pFsm->IsBusy() ||
		AOn(FOAM_CST[enFoamBoxLt].iPioValId) || AOn(FOAM_CST[enFoamBoxRt].iPioValId))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"FoamCst Cycle is Busy");
		return FALSE;
	}

	if (!NV.ndm[ltCobotFoamPkCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot Cycle is Running");
		return FALSE;
	}

	if (Exist() != enFoamExistTop && Exist() != enFoamExistBtmPartial)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Memory Mismatch");
		return FALSE;
	}

	if (Exist() == enFoamExistTop)
	{
		if (!FOAM_PART_DATA[nvFoamRecvElev].flag.levelDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"RecvElev Level Check Not Did");
			return FALSE;
		}

		if (FOAM_PART_DATA[nvFoamRecvElev].flag.recvFullDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"RecvElev Full");
			return FALSE;
		}

		if (FOAM_ELEV[enFoamRecvElev].m_pFsm->IsBusy())
			return FALSE;
	}
	if (Exist() == enFoamExistBtmPartial)
	{
		if (!FOAM_PART_DATA[nvFoamPartialElev].flag.levelDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"PartialElev Level Check Not Did");
			return FALSE;
		}

		if (FOAM_PART_DATA[nvFoamPartialElev].flag.recvFullDid)
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"PartialElev Full");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFoamPk::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy() && MT[mtFoamPkZ].InPos(P_FOAM_PK_Z_READY))
	{
		if (!Exist() && (VC[vcFoamPk].AOn() || VC[vcFoamPk2].AOn()))
		{
			ER.Save(ER_FOAM_PK_SENSOR_STATUS);
			return TRUE;
		}

		if (Exist() && (VC[vcFoamPk].AOff() || VC[vcFoamPk2].AOff()))
		{
			ER.Save(ER_FOAM_PK_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CFoamPk::Exist(void)
{
	return FOAM_PART_DATA[nvFoamPk].exist;
}

BOOL CFoamPk::IsExist(void)
{
	if (!FOAM_PART_DATA[nvFoamPk].exist || VC[vcFoamPk].AOff() || VC[vcFoamPk2].AOff())
	{
		ER.Save(ER_FOAM_PK_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamPk::IsEmpty(void)
{
	if (FOAM_PART_DATA[nvFoamPk].exist || VC[vcFoamPk].AOn() || VC[vcFoamPk2].AOn())
	{
		ER.Save(ER_FOAM_PK_EXIST);
		return FALSE;
	}

	return TRUE;
}

POINT2D CFoamPk::GetUnitCoord(int xUnit, int yUnit)
{
	POINT2D coord;
	POINT curBlock = BlockNo(xUnit, yUnit);
	double* pBlockOffsetX = (double*)&NV.rcp[FoamBoxBlock1OffsetX];
	double* pBlockOffsetY = (double*)&NV.rcp[FoamBoxBlock1OffsetY];
	double* pEndX = (double*)&NV.rcp[FoamBoxBlock1EndCntX];
	double* pEndY = (double*)&NV.rcp[FoamBoxBlock1EndCntY];

	POINT idx;
	idx.x = xUnit; idx.y = yUnit;
	if (0 < curBlock.x)
		idx.x = xUnit - (int)pEndX[(curBlock.x - 1)];
	if (0 < curBlock.y)
		idx.y = yUnit - (int)pEndY[(curBlock.y - 1)];

	coord.dX = (idx.x * NV.rcp[FoamBoxUnitPitchX]) + pBlockOffsetX[curBlock.x];
	coord.dY = (idx.y * NV.rcp[FoamBoxUnitPitchY]) + pBlockOffsetY[curBlock.y];

	return (coord);
}

POINT CFoamPk::BlockNo(int xUnit, int yUnit)
{
	POINT block;
	block.x = block.y = 0;

	double* pEndX = (double*)&NV.rcp[FoamBoxBlock1EndCntX];
	double* pEndY = (double*)&NV.rcp[FoamBoxBlock1EndCntY];

	for (int i = 0; i < (int)NV.rcp[FoamBoxBlockCntX]; i++)
	{
		if (xUnit < (int)pEndX[i])
		{
			block.x = i;
			break;
		}
	}

	for (int i = 0; i < (int)NV.rcp[FoamBoxBlockCntY]; i++)
	{
		if (yUnit < (int)pEndY[i])
		{
			block.y = i;
			break;
		}
	}

	return (block);
}

XYT CFoamPk::GetWorkPos(int nCmd, XYT coord)
{
	XYT posWork;
	posWork.dX = posWork.dY = posWork.dT = 0.;

	posWork.dX = MT[mtFoamPkX].PosTable(P_FOAM_PK_X_LT_CST_PICK + nCmd) + coord.dX;
	posWork.dY = MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_LT_CST_PICK + nCmd) + coord.dY;

	return (posWork);
}

BOOL CFoamPk::CreateWorkCoord(int nCmd, POINT xyCnt, XYT& workCoord)
{
	m_xyCnt.x = xyCnt.x;
	m_xyCnt.y = xyCnt.y;
	workCoord.dX = workCoord.dY = workCoord.dT = 0.;
	if (0 > xyCnt.x || 0 > xyCnt.y)
		return (FALSE);

	XYT coord;
	coord.dX = coord.dY = coord.dT = 0.;
	
	POINT2D unitCoord = GetUnitCoord(m_xyCnt.x, m_xyCnt.y);

	coord.dX = unitCoord.dX;
	coord.dY = unitCoord.dY;

	workCoord = coord;
	return (TRUE);
}

void CFoamPk::CycleFoamPick(void)
{
	if (!m_pFsm->Between(C_FOAM_PICK_UP_START, C_FOAM_PICK_UP_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save(ER_FOAM_PICK_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsg = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_FOAM_PICK_UP_START:
		{
			if (m_pFsm->Once())
			{
				if (nMsg == msgLtFoamBox)
				{
					strcpy(strMsg, "LT_FOAM_CST");
					m_state = "NORMAL_PICKUP";
				}
				else
				{
					strcpy(strMsg, "RT_FOAM_CST");
					m_state = "PARTIAL_PICKUP";
				}

				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_START [%s]", m_strName, strMsg);

				m_tmCycleTime.Reset();
				m_nExistCheckRetryCnt = 0;
				FOAM_PART_DATA[nvFoamPk].flag.flipCheckDid = false;
			}

			if (!IsEmpty())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
				break;

			if (FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt >= (NV.rcp[normalFoamMaxCnt + nMsg] - 1))
			{
				FOAM_PART_DATA[nvFoamLtCst + nMsg].flag.cstMaterialEmpty = true;
				m_pFsm->Set(C_FOAM_PICK_UP_END);
				break;
			}

			m_pFsm->Set(C_FOAM_PICK_UP_MOVE);
			break;
		}
		case C_FOAM_PICK_UP_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_MOVE COUNT: %02d", m_strName, FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt);

			m_maxCnt.x = (LONG)NV.rcp[FoamBoxUnitCntX];
			m_maxCnt.y = (LONG)NV.rcp[FoamBoxUnitCntY];
			int nUnitNo = FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt % (m_maxCnt.x * m_maxCnt.y);
			m_xyCnt = CntToCoordi(m_maxCnt, nUnitNo,
				(int)NV.rcp[FoamBoxStartEdge], (int)NV.rcp[FoamBoxDirection], (BOOL)NV.rcp[FoamBoxZigZag]);

			if (0 > m_xyCnt.x || 0 > m_xyCnt.y)
			{
				m_pFsm->Set(C_FOAM_PICK_UP_Z_UP);
				break;
			}

			if (!CreateWorkCoord(nMsg, m_xyCnt, m_coord))
			{
				ER.Save((ErrName)(ER_FOAM_PK_NORMAL_FAOM_PICK_NOT_COORD_CREATE + nMsg));
				m_pFsm->Set(C_ERROR);
				break;
			}

			XYT ptPos = GetWorkPos(nMsg, m_coord);
			MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_CST_PICK + nMsg, ptPos.dX);
			MtPosMove(mtFoamPkY, P_FOAM_PK_Y_LT_CST_PICK + nMsg, ptPos.dY);
			if (!MT[mtFoamPkX].InPos(P_FOAM_PK_X_LT_CST_PICK + nMsg, ptPos.dX) ||
				!MT[mtFoamPkY].InPos(P_FOAM_PK_Y_LT_CST_PICK + nMsg, ptPos.dY))
				break;
			
			if (FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt > 0)
				FOAM_PART_DATA[nvFoamPk].flag.flipCheckDid = true;

			//if (!NV.use[useFoamPkMixFlipInsp] || FOAM_PART_DATA[nvFoamPk].flag.flipCheckDid)
			//	m_pFsm->Set(C_FOAM_PICK_UP_Z_DN);
			//else
			//	m_pFsm->Set(C_FOAM_PICK_UP_VERIFY_Z_DN);
			m_pFsm->Set(C_FOAM_PICK_UP_VERIFY_Z_DN);

			break;
		}
		case C_FOAM_PICK_UP_VERIFY_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_VERIFY_Z_DN", m_strName);

			int m_ZPitchCnt = FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt / (m_maxCnt.x * m_maxCnt.y);
			double dZPitch = m_ZPitchCnt * NV.rcp[normalFoamThickness + nMsg];
			m_ZPickPos = MT[mtFoamPkZ].PosTable(P_FOAM_PK_Z_LT_CST_CHECK_MIX_FLIP + (nMsg * 2)) + dZPitch;
			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_LT_CST_CHECK_MIX_FLIP + (nMsg * 2), m_ZPickPos))
				break;

			if (!MT[mtFoamPkZ].m_isRdy.TmOver(100))
				return;

			double dHeightMin = NV.rcp[foamPkLaserFoamBaseHeight + (nMsg * 2)] - NV.ddm[foamPkLaserHeightTolerance];
			double dHeightMax = NV.rcp[foamPkLaserFoamBaseHeight + (nMsg * 2)] + NV.ddm[foamPkLaserHeightTolerance];
			if (Between(VC[vcFoamPkLaser].m_state.value, dHeightMin, dHeightMax))
			{
				NV.ddm[foamPkOffsetPitch] = VC[vcFoamPkLaser].m_state.value - NV.rcp[foamPkLaserFoamBaseHeight + (nMsg * 2)];
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_VERIFY_Z_DN_ANALOG_DATA: [%.3f] OFFSET_Z_DATA: [%.3f]", m_strName, VC[vcFoamPkLaser].m_state.value, NV.ddm[foamPkOffsetPitch]);
				if (FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt > 0 || !NV.use[useFoamPkMixFlipInsp] || FOAM_PART_DATA[nvFoamPk].flag.flipCheckDid)
					m_pFsm->Set(C_FOAM_PICK_UP_Z_DN);
				else
					m_pFsm->Set(C_FOAM_PICK_UP_VERIFY_MOVE);
			}
			else
			{
				m_nExistCheckRetryCnt++;
				if (m_nExistCheckRetryCnt > 1)
				{
					ER.Save((ErrName)(ER_FOAM_PK_NORMAL_FOAM_EXIST_CHECK_COUNT_OVER + nMsg));
					m_pFsm->Set(C_ERROR);
					break;
				}
				FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt++;
				m_pFsm->Set(C_FOAM_PICK_UP_MOVE);
			}
			break;
		}
		case C_FOAM_PICK_UP_VERIFY_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_VERIFY_MOVE", m_strName);

			MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_CST_LT_CHECK_MIX_FLIP + (nMsg * 2));

			double dHeightMin = NV.rcp[foamPkLaserFoamWallHeight + (nMsg * 2)] - NV.ddm[foamPkLaserHeightTolerance];
			double dHeightMax = NV.rcp[foamPkLaserFoamWallHeight + (nMsg * 2)] + NV.ddm[foamPkLaserHeightTolerance];
			
			if (Between(VC[vcFoamPkLaser].m_state.value, dHeightMin, dHeightMax))
			{
				MT[mtFoamPkX].Stop();
				FOAM_PART_DATA[nvFoamPk].flag.flipCheckDid = true;
				m_pFsm->Set(C_FOAM_PICK_UP_MOVE);
			}

			if (MT_INPOS != MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_CST_LT_CHECK_MIX_FLIP + (nMsg * 2)))
				break;

			if (!Between(VC[vcFoamPkLaser].m_state.value, dHeightMin, dHeightMax))
			{
				if (OPR.isDryRun)
				{
					FOAM_PART_DATA[nvFoamPk].flag.flipCheckDid = true;
					m_pFsm->Set(C_FOAM_PICK_UP_MOVE);
					break;
				}
				ER.Save((ErrName)(ER_FOAM_PK_NORMAL_FAOM_MIX_FLIP + nMsg));
				m_pFsm->Set(C_ERROR);
				break;
			}

			break;
		}
		case C_FOAM_PICK_UP_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_Z_DN", m_strName);

			int m_ZPitchCnt = FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt / (m_maxCnt.x * m_maxCnt.y);
			double dZPitch = m_ZPitchCnt * NV.rcp[normalFoamThickness + nMsg];
			m_ZPickPos = MT[mtFoamPkZ].PosTable(P_FOAM_PK_Z_LT_CST_PICK + nMsg) + dZPitch + NV.ddm[foamPkOffsetPitch];
			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_LT_CST_PICK + nMsg, m_ZPickPos))
				break;

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (!VC[vcFoamPk].AOn() || !VC[vcFoamPk2].AOn())
				{
					ER.Save((ErrName)(ER_VAC_FOAM_PK_NORMAL_FAOM + nMsg));
					m_pFsm->Set(C_ERROR);
					break;
				}
			}

			if (!VC[vcFoamPk].IsOn() || !VC[vcFoamPk2].IsOn())
			{
				VC[vcFoamPk].On(FOAM_PK_ON);
				VC[vcFoamPk2].On(FOAM_PK2_ON);
			}

			if (!VC[vcFoamPk].IsOn() || !VC[vcFoamPk2].IsOn() ||
				!VC[vcFoamPk].AOn() || !VC[vcFoamPk2].AOn())
				break;

			m_pFsm->Set(C_FOAM_PICK_UP_Z_UP);
			break;
		}
		case C_FOAM_PICK_UP_Z_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_Z_UP", m_strName);

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
				break;

			if (!VC[vcFoamPk].AOn() || !VC[vcFoamPk2].AOn())
			{
				ER.Save((ErrName)(ER_VAC_FOAM_PK_NORMAL_FAOM_DROP + nMsg));
				m_pFsm->Set(C_ERROR);
				break;
			}

			Exist() = enFoamExistTop + nMsg;
			FOAM_PART_DATA[nvFoamLtCst + nMsg].cnt++;
			m_pFsm->Set(C_FOAM_PICK_UP_END);
			break;
		}
		case C_FOAM_PICK_UP_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PICK_UP_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CFoamPk::CycleFoamPlace(void)
{
	if (!m_pFsm->Between(C_FOAM_PLACE_START, C_FOAM_PLACE_END))
		return;

	if (m_pFsm->TimeLimit(_1min))
	{
		ER.Save(ER_FOAM_PLACE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsg = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_FOAM_PLACE_START:
		{
			if (m_pFsm->Once())
			{
				if (nMsg == msgLtFoamBox)
				{
					strcpy(strMsg, "NORMAL_FOAM_ELEV");
					m_state = "NORMAL_PLACE";
				}
				else
				{
					strcpy(strMsg, "PARTIAL_FOAM_ELEV");
					m_state = "PARTIAL_PLACE";
				}

				LOG[logSEQ].Message("<%s> C_FOAM_PLACE_START [%s]", m_strName, strMsg);
				m_tmCycleTime.Reset();
			}

			// TODO: Check  Decide whether to include it or not.
			if (!IsCanCyclePlace())
				break;

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
				break;

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (nMsg == msgLtFoamBox)
				FOAM_ELEV[enFoamRecvElev].CylInit();
			else
				FOAM_ELEV[enFoamPartialElev].CylInit();

			m_pFsm->Set(C_FOAM_PLACE_MOVE);
			break;
		}
		case C_FOAM_PLACE_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PLACE_MOVE", m_strName);

			if (!MT[mtFoamPkX].InPos(P_FOAM_PK_X_LT_ELEV_PLACE + nMsg) ||
				!MT[mtFoamPkY].InPos(P_FOAM_PK_Y_LT_ELEV_PLACE + nMsg))
			{
				MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_ELEV_PLACE + nMsg);
				MtPosMove(mtFoamPkY, P_FOAM_PK_Y_LT_ELEV_PLACE + nMsg);
				break;
			}

			if (nMsg == msgLtFoamBox)
			{
				if (!FOAM_ELEV[enFoamRecvElev].CylInit())
					break;
			}
			else
			{
				if (!FOAM_ELEV[enFoamPartialElev].CylInit())
					break;
			}

			m_pFsm->Set(C_FOAM_PLACE_Z_DN);
			break;
		}
		case C_FOAM_PLACE_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PLACE_Z_DN", m_strName);

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_LT_ELEV_PLACE + nMsg))
				break;

			if (!VC[vcFoamPk].IsOff() || !VC[vcFoamPk2].IsOff())
			{
				VC[vcFoamPk].Off(FOAM_PK_OFF);
				VC[vcFoamPk2].Off(FOAM_PK2_OFF);
			}

			if (!VC[vcFoamPk].AOff() || !VC[vcFoamPk2].AOff() ||
				!VC[vcFoamPk].IsOff() || !VC[vcFoamPk2].IsOff())
				break;

			m_pFsm->Set(C_FOAM_PLACE_Z_UP);
			break;
		}
		case C_FOAM_PLACE_Z_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PLACE_Z_UP", m_strName);

			if (MT_INPOS != MtPosMove(mtFoamPkZ, P_FOAM_PK_Z_READY))
				break;

			m_pFsm->Set(C_FOAM_PLACE_END);
			break;
		}
		case C_FOAM_PLACE_END:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_PLACE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

				FOAM_PART_DATA[nvFoamRecvElev + (nMsg * nvFoamPartialElev)].flag.levelDid = false;
				FOAM_PART_DATA[nvFoamRecvElev + (nMsg * nvFoamPartialElev)].exist = enFoamExistTop + nMsg;
				FOAM_PART_DATA[nvFoamRecvElev + (nMsg * nvFoamPartialElev)].cnt++;
				ZeroMemory(&FOAM_PART_DATA[nvFoamPk], sizeof(IPC_MMI::FoamPartData));
			}

			// 25.10.13 
			int nDirCst = 0;		// Normal
			if (FOAM_PART_DATA[nvFoamRrStack].exist || FOAM_PART_DATA[nvFoamFtStack].exist || FOAM_PART_DATA[nvFoamSendElev].exist)
			{
				if (!FOAM_PART_DATA[nvFoamPartialElev].flag.recvFullDid &&
					FOAM_PART_DATA[nvFoamRtCst].exist && !FOAM_PART_DATA[nvFoamRtCst].flag.cstMaterialEmpty)
					nDirCst = 1;	// Partial
			}

			m_maxCnt.x = (LONG)NV.rcp[FoamBoxUnitCntX];
			m_maxCnt.y = (LONG)NV.rcp[FoamBoxUnitCntY];
			int nUnitNo = FOAM_PART_DATA[nvFoamLtCst + nDirCst].cnt % (m_maxCnt.x * m_maxCnt.y);
			m_xyCnt = CntToCoordi(m_maxCnt, nUnitNo, 
				(int)NV.rcp[FoamBoxStartEdge], (int)NV.rcp[FoamBoxDirection], (BOOL)NV.rcp[FoamBoxZigZag]);

			if (-1 < m_xyCnt.x && -1 < m_xyCnt.y)
			{
				if (!CreateWorkCoord(nDirCst, m_xyCnt, m_coord))
				{
					ER.Save((ErrName)(ER_FOAM_PK_NORMAL_FAOM_PICK_NOT_COORD_CREATE + nMsg));
					m_pFsm->Set(C_ERROR);
					break;
				}

				XYT ptPos = GetWorkPos(nDirCst, m_coord);
				MtPosMove(mtFoamPkX, P_FOAM_PK_X_LT_CST_PICK + nDirCst, ptPos.dX);
				MtPosMove(mtFoamPkY, P_FOAM_PK_Y_LT_CST_PICK + nDirCst, ptPos.dY);
				if (!MT[mtFoamPkX].InPos(P_FOAM_PK_X_LT_CST_PICK + nDirCst, ptPos.dX) ||
					!MT[mtFoamPkY].InPos(P_FOAM_PK_Y_LT_CST_PICK + nDirCst, ptPos.dY))
					break;
			}
			else
			{
				if (MT_INPOS != MtPosMove(mtFoamPkY, P_FOAM_PK_Y_READY))
					break;
			}
			// 

			//if (MT_INPOS != MtPosMove(mtFoamPkY, P_FOAM_PK_Y_READY))
			//	break;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}