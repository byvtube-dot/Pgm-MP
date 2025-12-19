#include "..\Includes.h"

void LotEndProcess(void)
{
	if (!NV.use[useLotInput])
	{
		NV.ndm[lotState] = ST_LOT_IDLE;
		NV.ndm[lotStartEvent] = 0;
		NV.ndm[lotEndEvent] = 0;
		NV.ndm[lotEndCmd] = 0;
		NV.ndm[lotInCnt] = 0;
		NV.ndm[lotOutCnt] = 0;
		NV.ndm[lotUnitCnt] = 0;
		return;
	}

	if (FSM[fsmSEQ].IsBusy())
		return;

	int inMcCnt = 0;
//	if (!!NV.ndm[EXIST_LD_TRAY]) inMcCnt++;


	if (OPR.isAuto)
	{
		//if (!g_TrayElv[ElvGood].m_bEmpty || (NV.trayInfo[nvTrayGood1].exist != enTrayNone) || (AOn(g_TrayElv[ElvGood].m_iLevelF) && AOn(g_TrayElv[ElvGood].m_iLevelR)) ||
		//	(NV.trayInfo[nvTrayGood2].exist != enTrayNone) || (NV.trayInfo[nvInPk].exist != enTrayNone))
		//{
		//	g_tmUnitWait.Reset();
		//}
		//else
		//{
		//	if (g_tmUnitWait.TmOver(10000) && !g_bWaitDryRun)
		//	{
		//		MSGNOTICE.Set(MSG_OP_UNITWAIT, "투입 대기", "투입 완료", NULL, NULL, "투입되는 유닛이 없습니다.");
		//		ER.Save(ER_MSG_POP_TIMEOUT);
		//	}
		//}
	}

	int cmdLotEnd = NV.ndm[lotEndCmd];
	NV.ndm[lotEndCmd] = 0;

	if (ST_LOT_IDLE == NV.ndm[lotState])
	{
		if (0 != cmdLotEnd)
		{
			MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Need To Check Lot Status.");
			return;
		}

		if (!NV.ndm[lotStartEvent] || !NV.use[useLotInput])
			return;

		if (0 < inMcCnt)
		{
			ER.Save(ER_LOT_START_FAIL_PACKAGE_IN_MC);
		}
		else
		{
			WR.Del(WR_LOT_END);
			NV.ndm[lotState] = ST_LOT_START;
			NV.ndm[lotEndEvent] = FALSE;

			NV.ndm[lotInCnt] = 0;
			NV.ndm[lotOutCnt] = 0;
			NV.ndm[lotUnitCnt] = 0;

			//LASER_INDEX[IPC_MMI::LaserIndex1].IsPowerCheckComp() = TRUE;
			//LASER_INDEX[IPC_MMI::LaserIndex2].IsPowerCheckComp() = TRUE;
			//if (NV.use[USE_CHECK_LASER_POWER])
			//{
			//	LASER_INDEX[IPC_MMI::LaserIndex1].IsPowerCheckComp() = FALSE;
			//	LASER_INDEX[IPC_MMI::LaserIndex2].IsPowerCheckComp() = FALSE;
			//}

			//LASER_INDEX[IPC_MMI::LaserIndex1].IsCrossHairComp() = TRUE;
			//LASER_INDEX[IPC_MMI::LaserIndex2].IsCrossHairComp() = TRUE;
			//if (NV.use[USE_AUTO_CROSS_HAIR])
			//{
			//	LASER_INDEX[IPC_MMI::LaserIndex1].IsCrossHairComp() = !NV.use[USE_INDEX1];
			//	LASER_INDEX[IPC_MMI::LaserIndex2].IsCrossHairComp() = !NV.use[USE_INDEX2];
			//}

			//LASER_INDEX[IPC_MMI::LaserIndex1].IsFlatChk() = TRUE;
			//LASER_INDEX[IPC_MMI::LaserIndex2].IsFlatChk() = TRUE;
			//if (NV.use[USE_INDEX_FLAT_CHECK])
			//{
			//	LASER_INDEX[IPC_MMI::LaserIndex1].IsFlatChk() = !NV.use[USE_INDEX1];
			//	LASER_INDEX[IPC_MMI::LaserIndex2].IsFlatChk() = !NV.use[USE_INDEX2];
			//}

			//LASER_INDEX[IPC_MMI::LaserIndex1].IsPreQcComp() = TRUE;
			//LASER_INDEX[IPC_MMI::LaserIndex2].IsPreQcComp() = TRUE;
			//if (NV.use[USE_INDEX_PREQc])
			//{
			//	LASER_INDEX[IPC_MMI::LaserIndex1].IsPreQcComp() = !NV.use[USE_INDEX1];
			//	LASER_INDEX[IPC_MMI::LaserIndex2].IsPreQcComp() = !NV.use[USE_INDEX2];
			//}
		}

		NV.ndm[lotStartEvent] = FALSE;
	}
	else
	{
		if (0 != NV.ndm[lotStartEvent])
			NV.ndm[lotStartEvent] = FALSE;

		if (0 != cmdLotEnd)
		{
			if (OPR.isAuto || OPR.isCycleRun)
			{
				MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Machnie is Running.");
			}
			else
			{
				if (!inMcCnt)
				{
					LOG[logSEQ].Message("lot insert count chagne before[%d] to after[%d] for lot end",
						NV.ndm[lotInsertCnt], NV.ndm[lotInCnt]);

					NV.ndm[lotInsertCnt] = NV.ndm[lotInCnt];
					NV.ndm[lotEndEvent] = TRUE;
					NV.ndm[lotState] = ST_LOT_IDLE;
					OPR.isStop = (TRUE);
				}
				else
				{
					MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Inside Machine Have Unit.");
				}
			}
			return;
		}

		if (IsPreLotCondition())
		{
			NV.ndm[lotState] = ST_LOT_PRE_END;
			// 랏앤드 처리 조건시..
			FSM[fsmSEQ].Set(C_LOTEND_START);
		}
		else
		{
			NV.ndm[lotState] = ST_LOT_START;
		}
	}
}

void LotEndCycle(void)
{
	if (!FSM[fsmSEQ].Between(C_LOTEND_START, C_LOTEND_END))
		return;

	if (!Between(NV.ndm[lotState], ST_LOT_PRE_END, ST_LOT_END))
	{
		ER.Save(ER_LOTEND_CYCLE_CANCEL);
		FSM[fsmSEQ].Set(C_ERROR);
		return;
	}

	if (FSM[fsmSEQ].TimeLimit(_5min))
	{
		ER.Save(ER_LOTEND_CYCLE_TM_OVER);
		FSM[fsmSEQ].Set(C_ERROR);
		return;
	}

	switch (FSM[fsmSEQ].Get())
	{
		case C_LOTEND_START:
		{
			if (FSM[fsmSEQ].Once())
			{
				LOG[logSEQ].Message("C_LOTEND_START");
			}

			FSM[fsmSEQ].Set(C_LOTEND_PROCESS);
		}
		break;
		case C_LOTEND_PROCESS:
		{
			if (FSM[fsmSEQ].Once())
			{
				NV.ndm[lotEndEvent] = TRUE;
				NV.ndm[lotState] = ST_LOT_END;
				LOG[logSEQ].Message("C_LOTEND_PROCESS");
			}

			if (!FSM[fsmSEQ].Delay(100) || 0 < NV.ndm[lotEndEvent])
				break;

			//if (EMPTY_TRAY.m_pFsm->IsBusy() || NG_TRAY.m_pFsm->IsBusy() || GOOD_TRAY.m_pFsm->IsBusy() || OUT_TRAY.m_pFsm->IsBusy())
			//	break;

			//EMPTY_TRAY.m_pFsm->Set(SendTray::C_RECV_LEVEL_START);
			//NG_TRAY.m_pFsm->Set(RevTray::C_RECV_LEVEL_START);
			//GOOD_TRAY.m_pFsm->Set(RevTray::C_RECV_LEVEL_START);
			//OUT_TRAY.m_pFsm->Set(RevTray::C_RECV_LEVEL_START);

			FSM[fsmSEQ].Set(C_LOTEND_END);
		}
		break;
		case C_LOTEND_END:
		{
			if (FSM[fsmSEQ].Once())
			{
				LOG[logSEQ].Message("C_LOTEND_END");
			}

			MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Now Lot End.");
			NV.ndm[lotState] = ST_LOT_IDLE;
			WR.Save(WR_LOT_END);
			OPR.isStop = (TRUE);
			FSM[fsmSEQ].Set(C_IDLE);
		}
		break;
	}
}

BOOL IsPreLotCondition(void)
{
	if (!OPR.isAuto || OPR.isStop || IsAnyCycleRun())
		return (FALSE);

	if (!NV.use[useLotInput])
		return (FALSE);

	if (NV.ndm[scrStopLoading])
		return (FALSE);

	//if (SendTray::TRAY_INIT != NV.ndm[DID_IN_TRAY_LEVEL] || IN_TRAY.InBtnPushed)
	//	return (FALSE);

	//if (!AOff(iIN_TRAY_EXIST_IN, 500) || !AOff(iIN_TRAY_EXIST_MID, 500) || !AOff(iIN_TRAY_EXIST_OUT, 500))
	//	return (FALSE);

	//if (!MT[mtIN_TRAY_Z].IsRdy(SEND_TRAY_BTM))
	//	return (FALSE);

	//if (!!NV.ndm[EXIST_LD_TRAY])		return (FALSE);


	return (TRUE);
}