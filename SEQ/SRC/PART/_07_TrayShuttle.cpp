#include "..\..\Includes.h"

CTrayShuttle TRAY_SHUTTLE;

CTrayShuttle::CTrayShuttle()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CTrayShuttle::~CTrayShuttle()
{

}

void CTrayShuttle::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = static_cast<string> (TRAY_PART_DATA[nvTrayShuttle].labelInfo.sBoxId);
	if (0 == strcmp(TRAY_PART_DATA[nvTrayShuttle].labelInfo.sBoxId, ""))
		MatId = "$";

	if ("SEND" == newValue)
	{
		TRAY_SHUTTLE.m_pSLog->XFR(newValue, MatId, "TRAY_SHUTTLE", "MBB_SHUTTLE");

		TRAY_SHUTTLE.m_pSLog->RecordingOn(mtTrayPusherX);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(mtTrayShuttleY);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleNozzleUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleBridgeUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleBridgeFb);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleFtStopFb);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleRrStopFb);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleFtMaskUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(cylTrayShuttleRrMaskUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOn(CV_TRAY_SHUTTLE);

	}
	else
	{
		TRAY_SHUTTLE.m_pSLog->XFR(newValue, MatId, "", "");

		TRAY_SHUTTLE.m_pSLog->RecordingOff(mtTrayPusherX);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(mtTrayShuttleY);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleNozzleUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleBridgeUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleBridgeFb);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleFtStopFb);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleRrStopFb);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleFtMaskUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(cylTrayShuttleRrMaskUd);
		TRAY_SHUTTLE.m_pSLog->RecordingOff(CV_TRAY_SHUTTLE);
	}
}

void CTrayShuttle::Auto(void)
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

	if (!IsCanCycleFoamPackSend())
		return;

	m_pFsm->Set(C_FOAM_PACK_SEND_START);
	return;
}

void CTrayShuttle::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (!CONV[CV_TRAY_SHUTTLE].IsStop())
			CONV[CV_TRAY_SHUTTLE].Stop();
		if (!MT[mtTrayPusherX].IsRdy())
			MT[mtTrayPusherX].Stop();

		if (OPR.isStop && !OPR.isAuto)
		{
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleFoamPackSend();
}

void CTrayShuttle::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "TrayShuttle");
	m_pFsm = &FSM[fsmTrayShuttle];

	m_pSLog = &SLOG[slogTrayShuttle];
	m_pSLog->SetXFRDeviceId("TRAY_SHUTTLE");
	m_pSLog->SetXFRMaterialType("TRAY");

	m_pSLog->Add(mtTrayPusherX);
	m_pSLog->Add(mtTrayShuttleY);

	m_pSLog->Add(cylTrayShuttleNozzleUd);
	m_pSLog->Add(cylTrayShuttleBridgeUd);
	m_pSLog->Add(cylTrayShuttleBridgeFb);
	m_pSLog->Add(cylTrayShuttleFtStopFb);
	m_pSLog->Add(cylTrayShuttleRrStopFb);
	m_pSLog->Add(cylTrayShuttleFtMaskUd);
	m_pSLog->Add(cylTrayShuttleRrMaskUd);

	m_pSLog->Add(CV_TRAY_SHUTTLE);
}

BOOL CTrayShuttle::Common(void)
{

	return TRUE;
}

BOOL CTrayShuttle::IsHwErr(void)
{
	if (!MT[mtTrayShuttleY].m_state.isHome)
		return TRUE;
	if (!MT[mtTrayPusherX].m_state.isHome)
		return TRUE;

	if (CYL[cylTrayShuttleNozzleUd].GetErr())
		return TRUE;
	if (CYL[cylTrayShuttleBridgeUd].GetErr())
		return TRUE;
	if (CYL[cylTrayShuttleBridgeFb].GetErr())
		return TRUE;
	if (CYL[cylTrayShuttleFtStopFb].GetErr())
		return TRUE;
	if (CYL[cylTrayShuttleRrStopFb].GetErr())
		return TRUE;
	if (CYL[cylTrayShuttleFtMaskUd].GetErr())
		return TRUE;
	if (CYL[cylTrayShuttleRrMaskUd].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CTrayShuttle::IsHwRdy(void)
{
	if (!MT[mtTrayShuttleY].IsRdy())
		return TRUE;
	if (!m_pFsm->Between(C_FOAM_PACK_SEND_1ST_TRQ_MOVE, C_FOAM_PACK_SEND_2ND_POS_CHECK))
	{
		if (!MT[mtTrayPusherX].IsRdy())
			return TRUE;
	}

	return FALSE;
}

BOOL CTrayShuttle::CylInit(void)
{
	if (!CONV[CV_TRAY_SHUTTLE].IsStop())
		CONV[CV_TRAY_SHUTTLE].Stop();

	if (!CYL[cylTrayShuttleNozzleUd].IsDn())
		CYL[cylTrayShuttleNozzleUd].Dn(TRAY_SHUTTLE_NOZZLE_DN);

	if (!CYL[cylTrayShuttleFtStopFb].IsFwd())
		CYL[cylTrayShuttleFtStopFb].Fwd(TRAY_SHUTTLE_FT_STOP_FWD);
	if (!CYL[cylTrayShuttleRrStopFb].IsFwd())
		CYL[cylTrayShuttleRrStopFb].Fwd(TRAY_SHUTTLE_RR_STOP_FWD);

	if (!CYL[cylTrayShuttleFtMaskUd].IsUp())
		CYL[cylTrayShuttleFtMaskUd].Up(TRAY_SHUTTLE_FT_MASK_UP);
	if (!CYL[cylTrayShuttleRrMaskUd].IsUp())
		CYL[cylTrayShuttleRrMaskUd].Up(TRAY_SHUTTLE_RR_MASK_UP);

	if (!CYL[cylTrayShuttleBridgeFb].IsBwd())
	{
		if (!CYL[cylTrayShuttleBridgeUd].IsUp())
		{
			CYL[cylTrayShuttleBridgeUd].Up(TRAY_SHUTTLE_BRIDGE_UP);
			return FALSE;
		}
		CYL[cylTrayShuttleBridgeFb].Bwd(TRAY_SHUTTLE_BRIDGE_BWD);
	}
	else
	{
		if (!CYL[cylTrayShuttleBridgeUd].IsDn())
			CYL[cylTrayShuttleBridgeUd].Dn(TRAY_SHUTTLE_BRIDGE_DN);
	}

	if (!CONV[CV_TRAY_SHUTTLE].IsStop() || !CYL[cylTrayShuttleNozzleUd].IsDn() ||
		!CYL[cylTrayShuttleFtStopFb].IsFwd() || !CYL[cylTrayShuttleRrStopFb].IsFwd() ||
		!CYL[cylTrayShuttleFtMaskUd].IsUp() || !CYL[cylTrayShuttleRrMaskUd].IsUp() ||
		!CYL[cylTrayShuttleBridgeFb].IsBwd() || !CYL[cylTrayShuttleBridgeUd].IsDn())
		return FALSE;

	return TRUE;
}

BOOL CTrayShuttle::IsCanCycleFoamPackSend(void)
{
	if (MBB_SHUTTLE.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Shuttle Cycle is Busy");
		return FALSE;
	}

	if (Exist() != enExistTrayTopFoamQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Tray Shuttle Memory Mismatch");
		return FALSE;
	}

	if (TRAY_PART_DATA[nvMbbShuttle].exist != enExistMbbOpen)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Shuttle Memory Mismatch");
		return FALSE;
	}

	if (!MT[mtTrayShuttleY].InPos(P_TRAY_SHUTTLE_Y_SEND))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"The 'mtTrayShuttleY' motor is not in the 'P_TRAY_SHUTTLE_Y_SEND' position.");
		return FALSE;
	}

	if (!MT[mtMbbShuttleY].InPos(P_MBB_SHUTTLE_Y_RECV))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"The 'mtMbbShuttleY' motor is not in the 'P_MBB_SHUTTLE_Y_RECV' position.");
		return FALSE;
	}

	return TRUE;
}

BOOL CTrayShuttle::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy() && 
		!FOAM_RAIL.m_pFsm->Between(FOAM_RAIL.C_TOP_FOAM_SEND_START, FOAM_RAIL.C_TOP_FOAM_SEND_END))
	{
		if (!Exist() && (AOn(iTrayShuttleExistIn) || AOn(iTrayShuttleExistEnd)))
		{
			ER.Save(ER_TRAY_SHUTTLE_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist() && (AOn(iTrayShuttleExistIn) || AOff(iTrayShuttleExistEnd)))
		{
			ER.Save(ER_TRAY_SHUTTLE_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CTrayShuttle::Exist(void)
{
	return TRAY_PART_DATA[nvTrayShuttle].exist;
}

BOOL CTrayShuttle::IsExist(void)
{
	if (!TRAY_PART_DATA[nvTrayShuttle].exist || (AOff(iTrayShuttleExistIn) && AOff(iTrayShuttleExistEnd)))
	{
		ER.Save(ER_TRAY_SHUTTLE_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CTrayShuttle::IsEmpty(void)
{
	if (TRAY_PART_DATA[nvTrayShuttle].exist || AOn(iTrayShuttleExistIn) || AOn(iTrayShuttleExistEnd))
	{
		ER.Save(ER_TRAY_SHUTTLE_EXIST);
		return FALSE;
	}

	return TRUE;
}

bool& CTrayShuttle::DidTrayShuttle2D(void)
{
	return TRAY_PART_DATA[nvTrayShuttle].flag.packFaom2DReadDid;
}

void CTrayShuttle::CycleFoamPackSend(void)
{
	if (!m_pFsm->Between(C_FOAM_PACK_SEND_START, C_FOAM_PACK_SEND_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_FOAM_PACK_SEND_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_FOAM_PACK_SEND_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "SEND";
				m_n2DReadCnt = 0;
			}

			if (!IsExist() || MBB_SHUTTLE.Exist() != enExistMbbOpen)
			{
				if (MBB_SHUTTLE.Exist() != enExistMbbOpen)
					ER.Save(ER_MBB_OPEN_MEMORY_MBB_OPEN_MISMATCH);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (0 == strcmp(TRAY_PART_DATA[nvTrayShuttle].labelInfo.sBoxId, TRAY_PART_DATA[nvMbbShuttle].labelInfo.sBoxId))
				m_pFsm->Set(C_FOAM_PACK_SEND_MOVE);
			else
			{
				ER.Save(ER_TRAY_SHUTTLE_LABEL_INFO_AND_MBB_INFO_MISMATCH);
				m_pFsm->Set(C_ERROR);
				break;
			}

			break;
		}
		case C_FOAM_PACK_SEND_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_MOVE", m_strName);

			if (!CYL[cylTrayShuttleFtMaskUd].IsDn())
				CYL[cylTrayShuttleFtMaskUd].Dn(TRAY_SHUTTLE_FT_MASK_DN);
			if (!CYL[cylTrayShuttleRrMaskUd].IsDn())
				CYL[cylTrayShuttleRrMaskUd].Dn(TRAY_SHUTTLE_RR_MASK_DN);

			if (!CYL[cylTrayShuttleFtMaskUd].IsDn() || !CYL[cylTrayShuttleRrMaskUd].IsDn())
				break;

			if (MT_INPOS != MtPosMove(mtTrayShuttleY, P_TRAY_SHUTTLE_Y_SEND))
				break;

			if (NV.use[useTrayShuttle2D])
				m_pFsm->Set(C_FOAM_PACK_SEND_2D_READ);
			else
				m_pFsm->Set(C_FOAM_PACK_SEND_PUSH_READY);
			break;
		}
		case C_FOAM_PACK_SEND_2D_READ:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_2D_READ", m_strName);
				NV.ndm[reqTrayShuttle2DRead] = COMM_REQ;
			}

			if (NV.ndm[reqTrayShuttle2DRead] == COMM_ERR)
			{
				ER.Save(ER_TRAY_SHUTTLE_2D_READ_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (m_pFsm->TimeLimit(_5sec))
			{
				if (m_n2DReadCnt < 2)
				{
					m_n2DReadCnt++;
					NV.ndm[reqTrayShuttle2DRead] = COMM_IDLE;
					m_pFsm->Set(C_FOAM_PACK_SEND_MOVE);
				}
				else
				{
					ER.Save(ER_TRAY_SHUTTLE_2D_READ_TIME_OVER);
					m_pFsm->Set(C_ERROR);
				}
				break;
			}

			if (NV.ndm[reqTrayShuttle2DRead] != COMM_COMP)
				break;

			if (NV.use[useSecsGem])
			{
				if (0 == strcmp(TRAY_PART_DATA[nvTrayShuttle].labelInfo.sBoxId, TRAY_PART_DATA[nvTrayShuttle].lotInfo.labelComparison))
					m_pFsm->Set(C_FOAM_PACK_SEND_PUSH_READY);
				else
				{
					ER.Save(ER_TRAY_SHUTTLE_LABEL_INFO_AND_READ_INFO_MISMATCH);
					m_pFsm->Set(C_ERROR);
				}
			}
			else
				m_pFsm->Set(C_FOAM_PACK_SEND_PUSH_READY);

			break;
		}
		case C_FOAM_PACK_SEND_PUSH_READY:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_PUSH_READY", m_strName);

			if (AOn(iTrayShuttleLtColorSensor, 100) || AOn(iTrayShuttleRtColorSensor, 100))
			{
				if (AOn(iTrayShuttleLtColorSensor, 100))
					ER.Save(ER_TRAY_SHUTTLE_LT_COLOR_SENSOR_DETECT);
				if (AOn(iTrayShuttleRtColorSensor, 100))
					ER.Save(ER_TRAY_SHUTTLE_RT_COLOR_SENSOR_DETECT);
					m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CYL[cylTrayShuttleFtMaskUd].IsUp())
				CYL[cylTrayShuttleFtMaskUd].Up(TRAY_SHUTTLE_FT_MASK_UP);
			if (!CYL[cylTrayShuttleRrMaskUd].IsUp())
				CYL[cylTrayShuttleRrMaskUd].Up(TRAY_SHUTTLE_RR_MASK_UP);

			if (!CYL[cylTrayShuttleFtMaskUd].IsUp() || !CYL[cylTrayShuttleRrMaskUd].IsUp())
				break;

			if (!CYL[cylTrayShuttleBridgeFb].IsFwd())
			{
				if (!CYL[cylTrayShuttleBridgeUd].IsUp())
				{
					CYL[cylTrayShuttleBridgeUd].Up(TRAY_SHUTTLE_BRIDGE_UP);
					break;
				}

				CYL[cylTrayShuttleBridgeFb].Fwd(TRAY_SHUTTLE_BRIDGE_FWD);
				break;
			}

			if (!CYL[cylTrayShuttleBridgeUd].IsDn())
			{
				CYL[cylTrayShuttleBridgeUd].Dn(TRAY_SHUTTLE_BRIDGE_DN);
				break;
			}

			if (!CYL[cylTrayShuttleFtStopFb].IsBwd() || !CYL[cylTrayShuttleRrStopFb].IsBwd())
			{
				CYL[cylTrayShuttleFtStopFb].Bwd(TRAY_SHUTTLE_FT_STOP_BWD);
				CYL[cylTrayShuttleRrStopFb].Bwd(TRAY_SHUTTLE_RR_STOP_BWD);
				break;
			}

			m_pFsm->Set(C_FOAM_PACK_SEND_1ST_TRQ_MOVE);
			break;
		}
		case C_FOAM_PACK_SEND_1ST_TRQ_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_1ST_TRQ_MOVE", m_strName);

			// Log Standard .... TODO: Check
			//if (!CONV[cvTrayShuttle].IsFwd())
			//	CONV[cvTrayShuttle].Fwd();

			if (MT[mtTrayPusherX].CanMove(P_TRAY_PUSHER_X_1ST_END))
			{
				MT[mtTrayPusherX].TrqMove(P_TRAY_PUSHER_X_1ST_END, NV.rcp[insertTrq], NV.rcp[insert1stTrqMaxVel]);
				m_pFsm->Set(C_FOAM_PACK_SEND_1ST_POS_CHECK);
			}

			break;
		}
		case C_FOAM_PACK_SEND_1ST_POS_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_1ST_POS_CHECK", m_strName);

			if (m_pFsm->Delay(20000))
			{
				ER.Save(ER_TRAY_SHUTTLE_1ST_TORQUE_MOVE_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			double dCheckPos = MT[mtTrayPusherX].PosTable(P_TRAY_PUSHER_X_1ST_END);
			if (MT[mtTrayPusherX].m_state.realCnt >= dCheckPos)
			{
				if (MT[mtTrayPusherX].IsRdy())
					m_pFsm->Set(C_FOAM_PACK_SEND_2ND_TRQ_MOVE);
				else
					MT[mtTrayPusherX].Stop();
			}
			break;
		}

		case C_FOAM_PACK_SEND_2ND_TRQ_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_2ND_TRQ_MOVE", m_strName);

			if (MT[mtTrayPusherX].CanMove(P_TRAY_PUSHER_X_2ND_END))
			{
				//MT[mtTrayPusherX].SLogFncAddMessage("Torque Move");
				MT[mtTrayPusherX].TrqMove(P_TRAY_PUSHER_X_2ND_END, NV.rcp[insertTrq], NV.rcp[insert2ndTrqMaxVel]);
				m_pFsm->Set(C_FOAM_PACK_SEND_2ND_POS_CHECK);
			}

			break;
		}
		case C_FOAM_PACK_SEND_2ND_POS_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_2ND_POS_CHECK", m_strName);

			if (m_pFsm->Delay(20000))
			{
				ER.Save(ER_TRAY_SHUTTLE_2ND_TORQUE_MOVE_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			double dCheckPos = MT[mtTrayPusherX].PosTable(P_TRAY_PUSHER_X_2ND_END) - MT[mtTrayPusherX].PosTable(P_TRAY_PUSHER_X_END_TOLERANCE);
			if (MT[mtTrayPusherX].m_state.realCnt >= dCheckPos)
			{
				if (!CONV[CV_TRAY_SHUTTLE].IsStop())
					CONV[CV_TRAY_SHUTTLE].Stop();

				if (MT[mtTrayPusherX].IsRdy())
					m_pFsm->Set(C_FOAM_PACK_SEND_INIT);
				else
					MT[mtTrayPusherX].Stop();
			}
			break;
		}
		case C_FOAM_PACK_SEND_INIT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_INIT", m_strName);

			if (MT_INPOS != MtPosMove(mtTrayPusherX, P_TRAY_SHUTTLE_Y_READY))
				break;

			CylInit();
			MBB_SHUTTLE.CylInit();

			if (!CylInit() || !MBB_SHUTTLE.CylInit())
				break;

			if (!CYL[cylTrayShuttleBridgeFb].IsBwd())
			{
				if (!CYL[cylTrayShuttleBridgeUd].IsUp())
				{
					CYL[cylTrayShuttleBridgeUd].Up(TRAY_SHUTTLE_BRIDGE_UP);
					break;
				}
				CYL[cylTrayShuttleBridgeFb].Bwd(TRAY_SHUTTLE_BRIDGE_BWD);
				break;
			}
			else
			{
				if (!CYL[cylTrayShuttleBridgeUd].IsDn())
				{
					CYL[cylTrayShuttleBridgeUd].Dn(TRAY_SHUTTLE_BRIDGE_DN);
					break;
				}
			}

			m_pFsm->Set(C_FOAM_PACK_SEND_END);
			break;
		}
		case C_FOAM_PACK_SEND_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_PACK_SEND_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			memcpy(&TRAY_PART_DATA[nvMbbShuttle], &TRAY_PART_DATA[nvTrayShuttle], sizeof(IPC_MMI::TrayPartData));
			ZeroMemory(&TRAY_PART_DATA[nvTrayShuttle], sizeof(IPC_MMI::TrayPartData));
			TRAY_PART_DATA[nvMbbShuttle].exist = enExistMbbTray;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}