#include "..\..\Includes.h"

CMbbShuttle MBB_SHUTTLE;

CMbbShuttle::CMbbShuttle()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CMbbShuttle::~CMbbShuttle()
{

}

void CMbbShuttle::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = static_cast<string> (TRAY_PART_DATA[nvMbbShuttle].labelInfo.sBoxId);
	if (0 == strcmp(TRAY_PART_DATA[nvMbbShuttle].labelInfo.sBoxId, ""))
		MatId = "$";

	if ("OPEN" == newValue)
		MBB_SHUTTLE.m_pSLog->XFR(newValue, MatId, "MBB_SHUTTLE", "MBB_SHUTTLE");
	else if ("SEALING" == newValue)
		MBB_SHUTTLE.m_pSLog->XFR(newValue, MatId, "MBB_SHUTTLE", "MBB_SHUTTLE");
	else if ("SEND" == newValue)
	{
		MBB_SHUTTLE.m_pSLog->XFR(newValue, MatId, "MBB_SHUTTLE", "MBB_QC");
		MBB_SHUTTLE.m_pSLog->RecordingOn(mtMbbQcX);
		MBB_SHUTTLE.m_pSLog->RecordingOn(cylQcSealedTrayStopUd);
	}
	else
	{
		MBB_SHUTTLE.m_pSLog->XFR(newValue, MatId, "", "");
		MBB_SHUTTLE.m_pSLog->RecordingOff(mtMbbQcX);
		MBB_SHUTTLE.m_pSLog->RecordingOff(cylQcSealedTrayStopUd);
	}
}

void CMbbShuttle::Auto(void)
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

	if (Exist() == enExistMbb)
	{
		if (!IsCanCycleMbbOpen())
			return;

		m_pFsm->Set(C_MBB_OPEN_START);
		return;
	}

	if (Exist() == enExistMbbTray)
	{
		if (!IsCanCycleMbbSealing())
			return;

		m_pFsm->Set(C_MBB_SEALING_START);
		return;
	}

	if (Exist() == enExistSealed)
	{
		if (!IsCanCycleMbbPackSend())
			return;

		m_pFsm->Set(C_MBB_PACK_SEND_START);
		return;
	}
}

void CMbbShuttle::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || TRAY_SHUTTLE.IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
		{
			NV.ddm[sealingPressureRegulator] = 0.3;
			VC[vcSealNozzle].Off(SEAL_NOZZLE_OFF);
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleMbbOpen();
	CycleMbbSealing();
	CycleMbbPackSend();
	CycleMbbSealingTest();
	CycleN2Test();
}

void CMbbShuttle::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "MbbShuttle");
	m_pFsm = &FSM[fsmMbbShuttle];

	m_pSLog = &SLOG[slogMbbShuttle];
	m_pSLog->SetXFRDeviceId("MBB_SHUTTLE");
	m_pSLog->SetXFRMaterialType("MBB");

	m_pSLog->Add(mtMbbOpenZ, TRUE);
	m_pSLog->Add(mtMbbShuttleY, TRUE);
	m_pSLog->Add(mtMbbShuttleConv, TRUE);
	m_pSLog->Add(mtMbbQcX, TRUE);

	m_pSLog->Add(cylTrayShuttleNozzleUd, TRUE);
	m_pSLog->Add(cylTrayShuttleBridgeUd, TRUE);
	m_pSLog->Add(cylTrayShuttleBridgeFb, TRUE);
	m_pSLog->Add(cylTrayShuttleFtStopFb, TRUE);
	m_pSLog->Add(cylTrayShuttleRrStopFb, TRUE);
	m_pSLog->Add(cylTrayShuttleFtMaskUd, TRUE);
	m_pSLog->Add(cylTrayShuttleRrMaskUd, TRUE);
	m_pSLog->Add(cylMbbOpenTopGuideFb, TRUE);
	m_pSLog->Add(cylMbbOpenTopVacUd, TRUE);
	m_pSLog->Add(cylMbbOpenTopClampUd, TRUE);
	m_pSLog->Add(cylSealNozzleFb, TRUE);
	m_pSLog->Add(cylSealMbbOpenGuideFtOc, TRUE);
	m_pSLog->Add(cylSealMbbOpenGuideRrOc, TRUE);
	m_pSLog->Add(cylSealNozzleMfcN2Oc, TRUE);
	m_pSLog->Add(cylSealBtmGuideUd, TRUE);
	m_pSLog->Add(cylSealerBtmUd, TRUE);
	m_pSLog->Add(cylSealerTopFtUd, TRUE);
	m_pSLog->Add(cylSealerTopRrUd, TRUE);
	m_pSLog->Add(cylSealTopGuideUd, TRUE);
	m_pSLog->Add(cylMbbShuttleAlignFtFb, TRUE);
	m_pSLog->Add(cylMbbShuttleAlignRrFb, TRUE);
	m_pSLog->Add(cylMbbShuttleClampFtFb, TRUE);
	m_pSLog->Add(cylMbbShuttleClampRrFb, TRUE);

	m_pSLog->Add(vcMbbOpenBtm, TRUE);
	m_pSLog->Add(vcMbbOpenTop, TRUE);
	m_pSLog->Add(vcMbbOpenTop2, TRUE);
	m_pSLog->Add(vcSealNozzle, TRUE);
	m_pSLog->Add(vcSealNozzleN2, TRUE);

	m_pSLog->Add(CV_QC, TRUE);
}

BOOL CMbbShuttle::Common(void)
{
	int nLoadCell = (int)(::Get(VC[vcLoadCellFt].m_aiNo)) + (int)(::Get(VC[vcLoadCellRr].m_aiNo));
	Set(aoLoadCellSeal, (int)nLoadCell);

	// Newton Display
	NV.ddm[sealingloadcell1] = VC[vcLoadCellRr].m_state.value;
	NV.ddm[sealingloadcell2] = VC[vcLoadCellFt].m_state.value;
	NV.ddm[sealingloadcellOutputValue] = nLoadCell * 5000 / 27648;

	double nAirValue = 0;
	if (Between(NV.ddm[setmfcValueAnalogOutput], 0.1, 5.0))
		nAirValue = (NV.ddm[setmfcValueAnalogOutput] / 5) * WEIDMULLER_AD_S7;
	else
	{
		if (NV.ddm[setmfcValueAnalogOutput] == 0)
			NV.ddm[mfcValuemilliLiter] = 0;
	}
	Set(aoAirSealNozzleN2, (int)nAirValue);

	if (NV.ddm[mfcValuemilliLiter] >= NV.rcp[setNozzleN2SetMilliLiter])
		NV.ddm[setmfcValueAnalogOutput] = 0;

	return TRUE;
}

BOOL CMbbShuttle::IsHwErr(void)
{
	if (!MT[mtMbbShuttleY].m_state.isHome)
		return TRUE;
	if (!MT[mtMbbShuttleConv].m_state.isHome)
		return TRUE;

	if (CYL[cylMbbOpenTopGuideFb].GetErr())
		return TRUE;
	if (CYL[cylMbbOpenTopVacUd].GetErr())
		return TRUE;
	if (CYL[cylMbbOpenTopClampUd].GetErr())
		return TRUE;
	if (CYL[cylSealNozzleFb].GetErr())
		return TRUE;
	if (CYL[cylSealMbbOpenGuideFtOc].GetErr())
		return TRUE;
	if (CYL[cylSealMbbOpenGuideRrOc].GetErr())
		return TRUE;
	if (CYL[cylSealBtmGuideUd].GetErr())
		return TRUE;
	if (CYL[cylSealerBtmUd].GetErr())
		return TRUE;
	if (CYL[cylSealerTopFtUd].GetErr())
		return TRUE;
	if (CYL[cylSealerTopRrUd].GetErr())
		return TRUE;
	if (CYL[cylSealTopGuideUd].GetErr())
		return TRUE;

	int nSealError = SEAL[sealMbb].GetError();
	if (nSealError > 0)
	{
		ER.Save((ErrName)(nSealError));
		return TRUE;
	}
	return FALSE;
}

BOOL CMbbShuttle::IsHwRdy(void)
{
	if (!MT[mtMbbShuttleY].IsRdy())
		return TRUE;
	//if (!MT[mtMbbShuttleConv].IsRdy()) return TRUE;

	return FALSE;
}

BOOL CMbbShuttle::CylInit(void)
{
	BOOL isReady = TRUE;

	if (!VC[vcMbbOpenBtm].IsOff())
	{
		VC[vcMbbOpenBtm].Off(MBB_OPEN_BTM_OFF);
		isReady = FALSE;
	}
	if (!VC[vcMbbOpenTop].IsOff() || !VC[vcMbbOpenTop2].IsOff())
	{
		VC[vcMbbOpenTop].Off(MBB_OPEN_TOP_OFF);
		VC[vcMbbOpenTop2].Off(MBB_OPEN_TOP2_OFF);
		isReady = FALSE;
	}
	if (!VC[vcSealNozzle].IsOff())
	{
		VC[vcSealNozzle].Off(SEAL_NOZZLE_OFF);
		isReady = FALSE;
	}
	if (!VC[vcSealNozzleN2].IsOff())
	{
		VC[vcSealNozzleN2].Off(SEAL_NOZZLE_N2_OFF);
		isReady = FALSE;
	}

	if (!isReady)
		return FALSE;

	if (!CYL[cylMbbOpenTopVacUd].IsUp())
	{
		CYL[cylMbbOpenTopVacUd].Up(MBB_OPEN_TOP_VAC_UP);
		isReady = FALSE;
	}
	if (!CYL[cylMbbOpenTopClampUd].IsUp())
	{
		CYL[cylMbbOpenTopClampUd].Up(MBB_OPEN_TOP_CLAMP_UP);
		isReady = FALSE;
	}
	if (!CYL[cylSealMbbClampUd].IsUp())
	{
		CYL[cylSealMbbClampUd].Up(SEAL_MBB_CLAMP_UP);
		isReady = FALSE;
	}

	if (!CYL[cylSealMbbClampFtOc].IsOpen())
	{
		CYL[cylSealMbbClampFtOc].Open(SEAL_MBB_CLAMP_OPEN);
		isReady = FALSE;
	}

	if (!CYL[cylSealMbbClampRrOc].IsOpen())
	{
		CYL[cylSealMbbClampRrOc].Open(SEAL_MBB_CLAMP_OPEN);
		isReady = FALSE;
	}

	if (!CYL[cylMbbShuttleClampUd].IsUp())
	{
		CYL[cylMbbShuttleClampUd].Up(MBB_SHUTTLE_CLAMP_UP);
		isReady = FALSE;
	}

	if (!isReady)
		return FALSE;

	if (!CYL[cylSealMbbClampFtFb].IsBwd())
	{
		CYL[cylSealMbbClampFtFb].Bwd(SEAL_MBB_CLAMP_BWD);
		isReady = FALSE;
	}

	if (!CYL[cylSealMbbClampRrFb].IsBwd())
	{
		CYL[cylSealMbbClampRrFb].Bwd(SEAL_MBB_CLAMP_BWD);
		isReady = FALSE;
	}

	if (!CYL[cylMbbOpenTopGuideFb].IsBwd())
	{
		CYL[cylMbbOpenTopGuideFb].Bwd(MBB_OPEN_TOP_GUIDE_BWD);
		isReady = FALSE;
	}

	if (!CYL[cylSealerBtmUd].IsDn())
	{
		CYL[cylSealerBtmUd].Dn(SEALER_BTM_DN);
		isReady = FALSE;
	}
	if (!CYL[cylSealerTopFtUd].IsUp())
	{
		CYL[cylSealerTopFtUd].Up(SEALER_TOP_FT_UP);
		isReady = FALSE;
	}
	if (!CYL[cylSealerTopRrUd].IsUp())
	{
		CYL[cylSealerTopRrUd].Up(SEALER_TOP_RR_UP);
		isReady = FALSE;
	}
	if (!CYL[cylSealBtmGuideUd].IsDn())
	{
		CYL[cylSealBtmGuideUd].Dn(SEAL_BTM_GUIDE_DN);
		isReady = FALSE;
	}
	if (!CYL[cylSealTopGuideUd].IsUp())
	{
		CYL[cylSealTopGuideUd].Up(SEAL_TOP_GUIDE_UP);
		isReady = FALSE;
	}
	if (!CYL[cylMbbShuttleAlignFtFb].IsBwd())
	{
		CYL[cylMbbShuttleAlignFtFb].Bwd(MBB_SHUTTLE_ALIGN_FT_BWD);
		isReady = FALSE;
	}
	if (!CYL[cylMbbShuttleAlignRrFb].IsBwd())
	{
		CYL[cylMbbShuttleAlignRrFb].Bwd(MBB_SHUTTLE_ALIGN_RR_BWD);
		isReady = FALSE;
	}
	if (!CYL[cylMbbShuttleClampFtFb].IsBwd())
	{
		CYL[cylMbbShuttleClampFtFb].Bwd(MBB_SHUTTLE_CLAMP_FT_BWD);
		isReady = FALSE;
	}
	if (!CYL[cylMbbShuttleClampRrFb].IsBwd())
	{
		CYL[cylMbbShuttleClampRrFb].Bwd(MBB_SHUTTLE_CLAMP_RR_BWD);
		isReady = FALSE;
	}

	if (!isReady)
		return FALSE;

	if (!CYL[cylSealNozzleFb].IsBwd())
	{
		CYL[cylSealNozzleFb].Bwd(SEAL_NOZZLE_BWD);
		isReady = FALSE;
	}
	
	if (!isReady)
		return FALSE;

	if (!CYL[cylSealMbbOpenGuideFtOc].IsClose())
	{
		CYL[cylSealMbbOpenGuideFtOc].Close(SEAL_MBB_OPEN_GUIDE_FT_CLOSE);
		isReady = FALSE;
	}
	if (!CYL[cylSealMbbOpenGuideRrOc].IsClose())
	{
		CYL[cylSealMbbOpenGuideRrOc].Close(SEAL_MBB_OPEN_GUIDE_RR_CLOSE);
		isReady = FALSE;
	}
	
	return isReady;
}

BOOL CMbbShuttle::IsCanCycleMbbOpen(void)
{
	if (FOAM_RAIL.m_pFsm->Between(FOAM_RAIL.C_TOP_FOAM_SEND_START, FOAM_RAIL.C_TOP_FOAM_SEND_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Top Foam Send Cycle is Busy");
		return FALSE;
	}

	if (TRAY_SHUTTLE.Exist() != enExistTrayTopFoamQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Tray Shuttle Memory Mismatch");
		return FALSE;
	}

	if (!MT[mtTrayShuttleY].IsRdy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"mtTrayShuttleY motor is not Ready.");
		return FALSE;
	}

	if (TRAY_SHUTTLE.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Tray Shuttle Cycle is Busy");
		return FALSE;
	}

	if (Exist() != enExistMbb)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Shuttle Memory Mismatch");
		return FALSE;
	}

	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_PLACE_START, RT_COBOT.C_MBB_PLACE_DN))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Right Cobot Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbShuttle::IsCanCycleMbbSealing(void)
{
	if (TRAY_SHUTTLE.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Tray Shuttle Cycle is Busy");
		return FALSE;
	}

	if (Exist() != enExistMbbTray)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Shuttle Memory Mismatch");
		return FALSE;
	}

	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_PLACE_START, RT_COBOT.C_MBB_PLACE_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Right Cobot Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbShuttle::IsCanCycleMbbPackSend(void)
{
	if (TRAY_SHUTTLE.m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Tray Shuttle Cycle is Busy");
		return FALSE;
	}

	if (Exist() != enExistSealed)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Mbb Shuttle Memory Mismatch");
		return FALSE;
	}

	if (RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_PLACE_START, RT_COBOT.C_MBB_PLACE_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Right Cobot Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbShuttle::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy() && !RT_COBOT.m_pFsm->Between(RT_COBOT.C_MBB_PLACE_START, RT_COBOT.C_MBB_PLACE_END))
	{
		//if (!Exist() && (AOn(iMbbShuttleConvExist) ||
		//	AOn(iMbbOpenBtmBlockExistFt) || AOn(iMbbOpenBtmBlockExistRr)))
		if (!Exist() && AOn(iMbbShuttleConvExist, 500))
		{
			ER.Save(ER_MBB_SHUTTLE_SENSOR_STATUS);
			return TRUE;
		}
		//if (Exist() && AOff(iMbbShuttleConvExist) &&
		//	AOff(iMbbOpenBtmBlockExistFt) && AOff(iMbbOpenBtmBlockExistRr))
		if (Exist() && AOff(iMbbShuttleConvExist, 500))
		{
			ER.Save(ER_MBB_SHUTTLE_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CMbbShuttle::Exist(void)
{
	return TRAY_PART_DATA[nvMbbShuttle].exist;
}

BOOL CMbbShuttle::IsExist(void)
{
	//	(AOff(iMbbShuttleConvExist) &&
	//	AOff(iMbbOpenBtmBlockExistFt) && AOff(iMbbOpenBtmBlockExistRr)))
	if (!TRAY_PART_DATA[nvMbbShuttle].exist || AOff(iMbbShuttleConvExist))
	{
		ER.Save(ER_MBB_SHUTTLE_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CMbbShuttle::IsEmpty(void)
{
	//if (TRAY_PART_DATA[nvMbbShuttle].exist || AOn(iMbbShuttleConvExist) ||
	//	AOn(iMbbOpenBtmBlockExistFt) || AOn(iMbbOpenBtmBlockExistRr))
	if (TRAY_PART_DATA[nvMbbShuttle].exist || AOn(iMbbShuttleConvExist))
	{
		ER.Save(ER_MBB_SHUTTLE_EXIST);
		return FALSE;
	}

	return TRUE;
}

void CMbbShuttle::CycleMbbOpen(void)
{
	if (!m_pFsm->Between(C_MBB_OPEN_START, C_MBB_OPEN_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_OPEN_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_OPEN_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "OPEN";
				m_nAirBlowCnt = 0;
				NV.ndm[commMbbSealingStart] = COMM_IDLE;
			}

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (Exist() != enExistMbb || TRAY_SHUTTLE.Exist() != enExistTrayTopFoamQc)
			{
				if (Exist() != enExistMbb)
					ER.Save(ER_MBB_OPEN_MEMORY_MBB_MISMATCH);
				if (TRAY_SHUTTLE.Exist() != enExistTrayTopFoamQc)
					ER.Save(ER_TRAY_SHUTTLE_MEMORY_TRAY_TOP_FOAM_MISMATCH);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!MT[mtMbbShuttleConv].IsRdy())
			{
				MT[mtMbbShuttleConv].Stop();
				break;
			}

			if (!CylInit())
				break;

			if (MT_INPOS != MtPosMove(mtMbbOpenZ, P_MBB_OPEN_Z_READY))
				break;

			m_pFsm->Set(C_MBB_OPEN_Y_MOVE);
			break;
		}
		case C_MBB_OPEN_Y_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_Y_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtMbbShuttleY, P_MBB_SHUTTLE_Y_RECV))
				break;

			if (!MT[mtTrayShuttleY].InPos(P_TRAY_SHUTTLE_Y_SEND))
			{
				if (MT_INPOS != MtPosMove(mtTrayPusherX, P_TRAY_SHUTTLE_Y_READY))
					break;

				MtPosMove(mtTrayShuttleY, P_TRAY_SHUTTLE_Y_SEND);
				break;
			}

			TRAY_SHUTTLE.CylInit();

			m_pFsm->Set(C_MBB_OPEN_MBB_ALIGN);
			break;
		}
		case C_MBB_OPEN_MBB_ALIGN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_MBB_ALIGN", m_strName);

			if (AOn(iMbbShuttleConvExist))
			{
				if (!CYL[cylMbbShuttleAlignFtFb].IsFwd())
					CYL[cylMbbShuttleAlignFtFb].Fwd(MBB_SHUTTLE_ALIGN_FT_FWD);
				if (!CYL[cylMbbShuttleAlignRrFb].IsFwd())
					CYL[cylMbbShuttleAlignRrFb].Fwd(MBB_SHUTTLE_ALIGN_RR_FWD);

				if (!CYL[cylMbbShuttleAlignFtFb].IsFwd() || !CYL[cylMbbShuttleAlignRrFb].IsFwd())
					break;
			}

			if (AOn(iMbbOpenBtmBlockExistFt) || AOn(iMbbOpenBtmBlockExistRr))
				m_pFsm->Set(C_MBB_OPEN_SENSOR_OFF_MOVE);
			else
				m_pFsm->Set(C_MBB_OPEN_MBB_ALIGN_VEL_MOVE);
			break;
		}
		case C_MBB_OPEN_SENSOR_OFF_MOVE:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_SENSOR_OFF_MOVE", m_strName);
				m_dBackwardPos = MT[mtMbbShuttleConv].m_state.realCnt + MT[mtMbbShuttleConv].PosTable(P_MBB_SHUTTLE_T_BACK_PITCH);
			}

			if (MT_INPOS != MtPosMove(mtMbbShuttleConv, P_MBB_SHUTTLE_T_BACK_PITCH, m_dBackwardPos))
				break;

			if (m_pFsm->TimeLimit(_10sec))
			{
				m_pFsm->Set(C_MBB_OPEN_MBB_ALIGN);
				break;
			}

			if (AOff(iMbbOpenBtmBlockExistFt) && AOff(iMbbOpenBtmBlockExistRr))
				m_pFsm->Set(C_MBB_OPEN_MBB_ALIGN_VEL_MOVE);

			break;
		}
		case C_MBB_OPEN_MBB_ALIGN_VEL_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_MBB_ALIGN_VEL_MOVE", m_strName);

			if (MT[mtMbbShuttleConv].CanMove(P_MBB_SHUTTLE_T_RECV_VEL))
			{
				MT[mtMbbShuttleConv].VelBwd(P_MBB_SHUTTLE_T_RECV_VEL);
				m_pFsm->Set(C_MBB_OPEN_MBB_ALIGN_CHECK);
			}

			break;
		}
		case C_MBB_OPEN_MBB_ALIGN_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_MBB_ALIGN_CHECK", m_strName);

			int nDelay = (int)NV.rcp[mbbConvStopArrivalDelay];
			if (OPR.isDryRun)
				nDelay = 0;
			if (AOn(iMbbOpenBtmBlockExistFt, nDelay) && AOn(iMbbOpenBtmBlockExistRr, nDelay))
			{
				if (MT[mtMbbShuttleConv].IsRdy())
					m_pFsm->Set(C_MBB_OPEN_Z_DN);
				else
					MT[mtMbbShuttleConv].Stop();
			}

			break;
		}
		case C_MBB_OPEN_Z_DN:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_Z_DN", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commMbbSealingStart] = COMM_REQ;
			}

			if (m_pFsm->Delay(20000))
			{
				if (!VC[vcMbbOpenBtm].AOn())
					ER.Save(ER_VAC_ALARM_VC_MBB_OPEN_BTM_MBB_OPEN_Z_DN);
				else
					ER.Save(ER_MBB_OPEN_Z_DN_CYCLE_TIME_OVER);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CYL[cylTrayShuttleFtMaskUd].IsUp())
				CYL[cylTrayShuttleFtMaskUd].Up(TRAY_SHUTTLE_FT_MASK_UP);
			if (!CYL[cylTrayShuttleRrMaskUd].IsUp())
				CYL[cylTrayShuttleRrMaskUd].Up(TRAY_SHUTTLE_RR_MASK_UP);

			if (!CYL[cylMbbOpenTopVacUd].IsDn())
				CYL[cylMbbOpenTopVacUd].Dn(MBB_OPEN_TOP_VAC_DN);

			if (OPR.isDryRun)
			{
				if (MT_INPOS != MtPosMove(mtMbbOpenZ, P_MBB_OPEN_Z_WORK))
					break;
			}
			else
			{
				if (MT_INPOS != MtPosMove(mtMbbOpenZ, P_MBB_OPEN_Z_PICK))
					break;
			}

			if (!CYL[cylMbbOpenTopVacUd].IsDn())
				break;

			if (!VC[vcMbbOpenBtm].IsOn())
				VC[vcMbbOpenBtm].On(MBB_OPEN_BTM_ON);

			if (!VC[vcMbbOpenBtm].IsOn() || !VC[vcMbbOpenBtm].AOn())
				break;

			m_pFsm->Set(C_MBB_OPEN_TOP_VAC);
			break;
		}
		case C_MBB_OPEN_TOP_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_TOP_VAC", m_strName);

			if (m_pFsm->Delay(20000))
			{
				if (!VC[vcMbbOpenTop].AOn())
					ER.Save(ER_VAC_ALARM_VC_MBB_OPEN_TOP_MBB_OPEN_Z_DN);
				if (!VC[vcMbbOpenTop2].AOn())
					ER.Save(ER_VAC_ALARM_VC_MBB_OPEN_TOP2_MBB_OPEN_Z_DN);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!VC[vcMbbOpenTop].IsOn() || !VC[vcMbbOpenTop2].IsOn())
			{
				VC[vcMbbOpenTop].On(MBB_OPEN_TOP_ON);
				VC[vcMbbOpenTop2].On(MBB_OPEN_TOP2_ON);
				break;
			}

			if (!VC[vcMbbOpenTop].AOn() || !VC[vcMbbOpenTop2].AOn())
				break;

			m_pFsm->Set(C_MBB_OPEN_CLAMP);
			break;
		}
		case C_MBB_OPEN_CLAMP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_CLAMP", m_strName);

			if (!CYL[cylMbbOpenTopVacUd].IsUp())
				CYL[cylMbbOpenTopVacUd].Up(MBB_OPEN_TOP_VAC_UP);
			if (!CYL[cylTrayShuttleFtMaskUd].IsUp())
				CYL[cylTrayShuttleFtMaskUd].Up(TRAY_SHUTTLE_FT_MASK_UP);
			if (!CYL[cylTrayShuttleRrMaskUd].IsUp())
				CYL[cylTrayShuttleRrMaskUd].Up(TRAY_SHUTTLE_RR_MASK_UP);

			if (!CYL[cylTrayShuttleFtMaskUd].IsUp() || !CYL[cylTrayShuttleRrMaskUd].IsUp())
				break;

			if (!CYL[cylTrayShuttleBridgeFb].IsFwd())
			{
				if (!CYL[cylTrayShuttleBridgeUd].IsUp())
					CYL[cylTrayShuttleBridgeUd].Up(TRAY_SHUTTLE_BRIDGE_UP);
			}

			if (!CYL[cylMbbShuttleAlignFtFb].IsBwd() || !CYL[cylMbbShuttleAlignRrFb].IsBwd())
			{
				CYL[cylMbbShuttleAlignFtFb].Bwd(MBB_SHUTTLE_ALIGN_FT_BWD);
				CYL[cylMbbShuttleAlignRrFb].Bwd(MBB_SHUTTLE_ALIGN_RR_BWD);
				break;
			}

			if (!CYL[cylMbbOpenTopVacUd].IsUp())
				break;

			if (!CYL[cylMbbOpenTopGuideFb].IsFwd())
			{
				if (!CYL[cylMbbOpenTopClampUd].IsUp())
					CYL[cylMbbOpenTopClampUd].Up(MBB_OPEN_TOP_CLAMP_UP);

				if (CYL[cylMbbOpenTopClampUd].IsUp())
					CYL[cylMbbOpenTopGuideFb].Fwd(MBB_OPEN_TOP_GUIDE_FWD);
			}
			else
			{
				if (!CYL[cylMbbOpenTopClampUd].IsDn())
					CYL[cylMbbOpenTopClampUd].Dn(MBB_OPEN_TOP_CLAMP_DN);
			}
			// Alarm 추가 필요
			//if (!VC[vcMbbOpenBtm].AOn())
			//{
			//	if (m_pFsm->Delay(1000))
			//	{
			//		ER.Save(ER_VAC_ALARM_VC_MBB_OPEN_BTM_MBB_OPEN_Z_DN);
			//		m_pFsm->Set(C_ERROR);
			//		break;
			//	}
			//	break;
			//}

			if (!CYL[cylTrayShuttleBridgeFb].IsFwd())
			{
				if (!CYL[cylTrayShuttleBridgeUd].IsUp())
					CYL[cylTrayShuttleBridgeUd].Up(TRAY_SHUTTLE_BRIDGE_UP);
				else
					CYL[cylTrayShuttleBridgeFb].Fwd(TRAY_SHUTTLE_BRIDGE_FWD);
			}
			else
			{
				if (!CYL[cylTrayShuttleBridgeUd].IsDn())
					CYL[cylTrayShuttleBridgeUd].Dn(TRAY_SHUTTLE_BRIDGE_DN);
			}

			if (!CYL[cylMbbOpenTopGuideFb].IsFwd() || !CYL[cylTrayShuttleBridgeFb].IsFwd() ||
				!CYL[cylMbbOpenTopClampUd].IsDn() || !CYL[cylTrayShuttleBridgeUd].IsDn())
				break;

			m_pFsm->Set(C_MBB_OPEN_Z_UP);
			break;
		}
		case C_MBB_OPEN_Z_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_Z_UP", m_strName);

			if (!VC[vcMbbOpenBtm].IsOff())
				VC[vcMbbOpenBtm].Off(MBB_OPEN_BTM_OFF);

			if (!VC[vcMbbOpenTop].IsOff() || !VC[vcMbbOpenTop2].IsOff())
			{
				VC[vcMbbOpenTop].Off(MBB_OPEN_TOP_OFF);
				VC[vcMbbOpenTop2].Off(MBB_OPEN_TOP2_OFF);
			}

			if (!CYL[cylTrayShuttleNozzleUd].IsUp())
				CYL[cylTrayShuttleNozzleUd].Up(TRAY_SHUTTLE_NOZZLE_UP);

			if (MT_INPOS != MtPosMove(mtMbbOpenZ, P_MBB_OPEN_Z_READY))
				break;

			On(oTrayShuttleAirBlowOn);

			if (!IsOn(oTrayShuttleAirBlowOn, 1000))
				break;

			if (!VC[vcMbbOpenBtm].AOff() || !VC[vcMbbOpenBtm].IsOff() ||
				!VC[vcMbbOpenTop].AOff() || !VC[vcMbbOpenTop].IsOff() ||
				!VC[vcMbbOpenTop2].AOff() || !VC[vcMbbOpenTop2].IsOff() ||
				!CYL[cylTrayShuttleNozzleUd].IsUp())
				break;

			if (NV.use[useTrayShuttleAirBlow])
				m_pFsm->Set(C_MBB_OPEN_AIR_BLOW_ON);
			else
				m_pFsm->Set(C_MBB_OPEN_AIR_BLOW_OFF);
			break;
		}
		case C_MBB_OPEN_AIR_BLOW_ON:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_AIR_BLOW_ON", m_strName);

			if (m_nAirBlowCnt < (int)NV.rcp[mbbOpenAirBlowCnt])
			{
				if (!CYL[cylTrayShuttleNozzleUd].IsUp())
				{
					CYL[cylTrayShuttleNozzleUd].Up(TRAY_SHUTTLE_NOZZLE_UP);
					break;
				}

				if (!IsOn(oTrayShuttleAirBlowOn))
				{
					On(oTrayShuttleAirBlowOn);
					sprintf(m_strLogBlowOn, "X%04d", (int)NV.rcp[mbbOpenAirBlowOnTime]);
					MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_ON", "START", "$", m_strLogBlowOn, oTrayShuttleAirBlowOn, "'ON'");
				}
				else
				{
					if (IsOn(oTrayShuttleAirBlowOn), (int)NV.rcp[mbbOpenAirBlowOnTime])
					{
						m_nAirBlowCnt++;
						m_pFsm->Set(C_MBB_OPEN_AIR_BLOW_OFF);
						break;
					}
				}
			}
			else
				m_pFsm->Set(C_MBB_OPEN_AIR_BLOW_OFF);

			break;
		}
		case C_MBB_OPEN_AIR_BLOW_OFF:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_AIR_BLOW_OFF", m_strName);
				MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_ON", "END", "$", m_strLogBlowOn, oTrayShuttleAirBlowOn, "'ON'");
				Off(oTrayShuttleAirBlowOn);
				MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_OFF", "START", "$", "0", oTrayShuttleAirBlowOn, "'OFF'");
			}

			// Retry 필요시
			//if (m_nAirBlowCnt < (int)NV.rcp[mbbOpenAirBlowCnt])
			//{
			//	if (IsOn(oTrayShuttleAirBlowOn))
			//	{
			//		MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_ON", "END", "$", m_strLogBlowOn, oTrayShuttleAirBlowOn, "'ON'");
			//		Off(oTrayShuttleAirBlowOn);
			//		sprintf(m_strLogBlowOff, "X%04d", (int)NV.rcp[mbbOpenAirBlowOffTime]);
			//		MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_OFF", "START", "$", m_strLogBlowOff, oTrayShuttleAirBlowOn, "'OFF'");
			//	}
			//
			//	if (IsOff(oTrayShuttleAirBlowOn), (int)NV.rcp[mbbOpenAirBlowOffTime])
			//	{
			//		MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_OFF", "END", "$", m_strLogBlowOff, oTrayShuttleAirBlowOn, "'OFF'");
			//		m_pFsm->Set(C_MBB_OPEN_AIR_BLOW_ON);
			//	}
			//}
			//else
			//	m_pFsm->Set(C_MBB_OPEN_INIT);

			if (IsOff(oTrayShuttleAirBlowOn))
				m_pFsm->Set(C_MBB_OPEN_INIT);

			break;
		}
		case C_MBB_OPEN_INIT:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_INIT", m_strName);
				if (IsOff(oTrayShuttleAirBlowOn))
					MBB_SHUTTLE.m_pSLog->LogFncOut("MBB_OPEN_AIR_BLOW_OFF", "END", "$", "0", oTrayShuttleAirBlowOn, "'OFF'");
			}

			if (!CYL[cylTrayShuttleNozzleUd].IsDn())
				CYL[cylTrayShuttleNozzleUd].Dn(TRAY_SHUTTLE_NOZZLE_DN);

			if (!CYL[cylTrayShuttleNozzleUd].IsDn())
				break;

			m_pFsm->Set(C_MBB_OPEN_END);
			break;
		}
		case C_MBB_OPEN_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_OPEN_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			TRAY_PART_DATA[nvMbbShuttle].exist = enExistMbbOpen;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbShuttle::CycleMbbSealing(void)
{
	if (!m_pFsm->Between(C_MBB_SEALING_START, C_MBB_SEALING_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_SEALING_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_SEALING_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_START", m_strName);
				m_tmCycleTime.Reset();

				m_prcLogLotId = TRAY_PART_DATA[nvMbbShuttle].lotInfo.lotId;
				m_prcLogMatId = TRAY_PART_DATA[nvMbbShuttle].labelInfo.sBoxId;
				m_prcRecipeId = "RECIPE_ID";
				m_pSLog->LogPrc("MBB_SEALING", "START", m_prcLogMatId, m_prcLogLotId, m_prcRecipeId, 0);

				m_state = "SEALING";
			}

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (Exist() != enExistMbbTray)
			{
				ER.Save(ER_MBB_OPEN_MEMORY_MBB_TRAY_MISMATCH);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!MT[mtMbbShuttleConv].IsRdy())
			{
				MT[mtMbbShuttleConv].Stop();
				break;
			}
			
			if (MT[mtMbbShuttleY].InPos(P_MBB_SHUTTLE_Y_SEAL) &&
				CYL[cylSealMbbOpenGuideFtOc].IsOpen() && CYL[cylSealMbbOpenGuideRrOc].IsOpen())
			{
				m_pFsm->Set(C_MBB_SEALING_NOZZLE_FWD);
				break;
			}

			if (!CylInit())
				break;

			m_pFsm->Set(C_MBB_SEALING_Y_MOVE);
			break;
		}
		case C_MBB_SEALING_Y_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_Y_MOVE", m_strName);

			if (!CYL[cylMbbShuttleClampFtFb].IsFwd() || !CYL[cylMbbShuttleClampRrFb].IsFwd())
			{
				CYL[cylMbbShuttleClampFtFb].Fwd(MBB_SHUTTLE_CLAMP_FT_FWD);
				CYL[cylMbbShuttleClampRrFb].Fwd(MBB_SHUTTLE_CLAMP_RR_FWD);
				break;
			}

			//if (!CYL[cylMbbShuttleClampUd].IsDn())
			//{
			//	CYL[cylMbbShuttleClampUd].Dn(MBB_SHUTTLE_CLAMP_DN);
			//	break;
			//}

			if (MT_INPOS != MtPosMove(mtMbbShuttleY, P_MBB_SHUTTLE_Y_SEAL))
				break;

			m_pFsm->Set(C_MBB_SEALING_NOZZLE_FWD);
			break;
		}
		case C_MBB_SEALING_NOZZLE_FWD:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_NOZZLE_FWD", m_strName);

			if (!CYL[cylSealNozzleFb].IsFwd())
			{
				if (!CYL[cylSealMbbOpenGuideFtOc].IsClose() || !CYL[cylSealMbbOpenGuideRrOc].IsClose())
				{
					CYL[cylSealMbbOpenGuideFtOc].Close(SEAL_MBB_OPEN_GUIDE_FT_CLOSE);
					CYL[cylSealMbbOpenGuideRrOc].Close(SEAL_MBB_OPEN_GUIDE_RR_CLOSE);
					break;
				}

				CYL[cylSealNozzleFb].Fwd(SEAL_NOZZLE_FWD);
				break;
			}
			else
			{
				if (!CYL[cylMbbShuttleClampFtFb].IsBwd() || !CYL[cylMbbShuttleClampRrFb].IsBwd())
				{
					CYL[cylMbbShuttleClampFtFb].Bwd(MBB_SHUTTLE_CLAMP_FT_BWD);
					CYL[cylMbbShuttleClampRrFb].Bwd(MBB_SHUTTLE_CLAMP_RR_BWD);
				}

				if (!CYL[cylSealMbbOpenGuideFtOc].IsOpen() || !CYL[cylSealMbbOpenGuideRrOc].IsOpen())
				{
					CYL[cylSealMbbOpenGuideFtOc].Open(SEAL_MBB_OPEN_GUIDE_FT_OPEN);
					CYL[cylSealMbbOpenGuideRrOc].Open(SEAL_MBB_OPEN_GUIDE_RR_OPEN);
				}

				if (!CYL[cylMbbShuttleClampFtFb].IsBwd() || !CYL[cylMbbShuttleClampRrFb].IsBwd() ||
					!CYL[cylSealMbbOpenGuideFtOc].IsOpen() || !CYL[cylSealMbbOpenGuideRrOc].IsOpen())
					break;

				m_pFsm->Set(C_MBB_SEALING_CLAMP_CLOSE);
			}
			break;
		}
		case C_MBB_SEALING_CLAMP_CLOSE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_CLAMP_CLOSE", m_strName);

			if (!CYL[cylSealBtmGuideUd].IsUp())
				CYL[cylSealBtmGuideUd].Up(SEAL_BTM_GUIDE_UP);

			if (!CYL[cylSealTopGuideUd].IsDn())
				CYL[cylSealTopGuideUd].Dn(SEAL_TOP_GUIDE_DN);

			if (!CYL[cylSealBtmGuideUd].IsUp() || !CYL[cylSealTopGuideUd].IsDn())
				break;

			m_pFsm->Set(C_MBB_SEALING_VAC);
			break;
		}
		case C_MBB_SEALING_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_VAC", m_strName);

			if (m_pFsm->Delay(10000))
			{
				ER.Save(ER_MBB_SEALING_VAC_CYCLE_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			//if (!CYL[cylMbbShuttleClampFtFb].IsFwd() || !CYL[cylMbbShuttleClampRrFb].IsFwd())
			//{
			//	CYL[cylMbbShuttleClampFtFb].Fwd(MBB_SHUTTLE_CLAMP_FT_FWD);
			//	CYL[cylMbbShuttleClampRrFb].Fwd(MBB_SHUTTLE_CLAMP_RR_FWD);
			//}

			if (!CYL[cylSealNozzleMfcN2Oc].IsClose())
			{
				CYL[cylSealNozzleMfcN2Oc].Close(SEAL_NOZZLE_MFC_CLOSE);
				break;
			}

			if (!VC[vcSealNozzle].IsOn())
				VC[vcSealNozzle].On(SEAL_NOZZLE_ON);

			if (!VC[vcSealNozzle].AOn() || !VC[vcSealNozzle].IsOn())
				break;

			m_pFsm->Set(C_MBB_SEALING_N2_BLOW);
			break;
		}
		case C_MBB_SEALING_N2_BLOW:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_N2_BLOW", m_strName);

			if (!VC[vcSealNozzle].IsOff())
			{
				VC[vcSealNozzle].Off(SEAL_NOZZLE_OFF);
				break;
			}

			if (!CYL[cylSealNozzleMfcN2Oc].IsOpen())
			{
				CYL[cylSealNozzleMfcN2Oc].Open(SEAL_NOZZLE_MFC_OPEN);
				break;
			}

			// N2 Supply
			if (!Between(NV.rcp[setNozzleN2AoSetValue], 0.1, 5.0))
				NV.rcp[setNozzleN2AoSetValue] = 5.0;
			NV.ddm[setmfcValueAnalogOutput] = NV.rcp[setNozzleN2AoSetValue];
			On(oCylSealNozzleMfcN2Op);
			VC[vcSealNozzleN2].On();

			double nAirValue = ((int)NV.rcp[setNozzleN2AoSetValue] / 100) * WEIDMULLER_AD_S7;
			Set(aoAirSealNozzleN2, (int)nAirValue);

			m_pFsm->Set(C_MBB_SEALING_N2_BLOW_CHECK);
			break;
		}
		case C_MBB_SEALING_N2_BLOW_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_N2_BLOW_CHECK", m_strName);

			if (m_pFsm->TimeLimit(_30sec))
			{
				VC[vcSealNozzleN2].Off();
				Off(oCylSealNozzleMfcN2Op);

				ER.Save(ER_MMBB_SEALING_N2_BLOW_CHECK_CYCLE_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (OPR.isDryRun && m_pFsm->TimeLimit(_3sec))
				NV.ddm[setmfcValueAnalogOutput] = 0;

			if (NV.ddm[setmfcValueAnalogOutput] != 0)
				break;

			VC[vcSealNozzleN2].Off();
			Off(oCylSealNozzleMfcN2Op);
			m_pFsm->Set(C_MBB_SEALING_WORK);
			break;
		}
		case C_MBB_SEALING_WORK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_WORK", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				if (BOn(iBSealcontactDetected))
					ER.Save(ER_SEALER_CONTACT_DETECT_TIME_OVER);
				else
					ER.Save(ER_MBB_SEALING_WORK_CYCLE_TIME_OVER);

				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CYL[cylSealNozzleFb].IsBwd())
			{
				CYL[cylSealNozzleFb].Bwd(SEAL_NOZZLE_BWD);
				break;
			}

			NV.ddm[sealingPressureRegulator] = NV.rcp[sealing1stPressureRegulator];

			if (!CYL[cylSealerBtmUd].IsUp())
			{
				CYL[cylSealerBtmUd].Up(SEALER_BTM_UP);
				break;
			}

			if (!CYL[cylSealerTopFtUd].IsDn() || !CYL[cylSealerTopRrUd].IsDn())
			{
				CYL[cylSealerTopFtUd].Dn(SEALER_TOP_FT_DN);
				CYL[cylSealerTopRrUd].Dn(SEALER_TOP_RR_DN);
				break;
			}

			if (!OPR.isDryRun && BOn(iBSealcontactDetected))
				break;

			if (!OPR.isDryRun)
				SEAL[sealMbb].Start();

			if (OPR.isDryRun || SEAL[sealMbb].m_pReadData->machineStatus == 4)
			{
				NV.ddm[sealingPressureRegulator] = NV.rcp[sealing2ndPressureRegulator];
				m_pFsm->Set(C_MBB_SEALING_RESULT);
			}
			break;
		}
		case C_MBB_SEALING_RESULT:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_RESULT", m_strName);
			}

			if (!OPR.isDryRun && SEAL[sealMbb].m_pReadData->machineStatus != 3)
				break;

			//if (SEAL[sealMbb].GetResult() != 1)
			//	break;

			m_pFsm->Set(C_MBB_SEALING_CLAMP_OPEN);
			break;
		}
		case C_MBB_SEALING_CLAMP_OPEN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_CLAMP_OPEN", m_strName);

			if (!CylInit())
				break;

			m_pFsm->Set(C_MBB_SEALING_END);
			break;
		}
		case C_MBB_SEALING_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_SEALING_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Exist() = enExistSealed;
			m_state = "";

			m_pSLog->LogPrc("MBB_SEALING", "END", m_prcLogMatId, m_prcLogLotId, m_prcRecipeId, 0);
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbShuttle::CycleMbbPackSend(void)
{
	if (!m_pFsm->Between(C_MBB_PACK_SEND_START, C_MBB_PACK_SEND_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_PACK_SEND_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_PACK_SEND_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_PACK_SEND_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "SEND";
			}

			if (!IsExist() || !MBB_QC.IsEmpty())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (Exist() != enExistSealed)
			{
				ER.Save(ER_MBB_OPEN_MEMORY_SEALED_MISMATCH);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!MT[mtMbbShuttleConv].IsRdy())
			{
				MT[mtMbbShuttleConv].Stop();
				break;
			}

			if (!CylInit() || !MBB_QC.CylInit())
				break;

			if (MT_INPOS != MtPosMove(mtMbbQcX, P_MBB_QC_X_RECV))
				break;

			m_pFsm->Set(C_MBB_PACK_SEND_MOVE);
			break;
		}
		case C_MBB_PACK_SEND_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PACK_SEND_MOVE", m_strName);

			//if (!CYL[cylMbbShuttleClampFtFb].IsFwd() || !CYL[cylMbbShuttleClampRrFb].IsFwd())
			//{
			//	CYL[cylMbbShuttleClampFtFb].Fwd(MBB_SHUTTLE_CLAMP_FT_FWD);
			//	CYL[cylMbbShuttleClampRrFb].Fwd(MBB_SHUTTLE_CLAMP_RR_FWD);
			//	break;
			//}

			if (MT_INPOS != MtPosMove(mtMbbShuttleY, P_MBB_SHUTTLE_Y_SEND))
				break;

			if (!CONV[CV_QC].IsFwd())
			{
				CONV[CV_QC].Fwd();
				break;
			}

			if (MT[mtMbbShuttleConv].CanMove(P_MBB_SHUTTLE_T_SEND_VEL))
			{
				MT[mtMbbShuttleConv].VelFwd(P_MBB_SHUTTLE_T_SEND_VEL);
				m_pFsm->Set(C_MBB_PACK_SEND_CHECK);
			}

			break;
		}
		case C_MBB_PACK_SEND_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PACK_SEND_CHECK", m_strName);

			// TODO : CHECK H/W
			if (AOn(iQcConvExistEnd, 500) || (OPR.isDryRun && AOn(iQcConvExistEnd)))
			{
				if (!CONV[CV_QC].IsStop() || !MT[mtMbbShuttleConv].IsRdy())
				{
					if (!CONV[CV_QC].IsStop())
						CONV[CV_QC].Stop();

					if (!MT[mtMbbShuttleConv].IsRdy())
						MT[mtMbbShuttleConv].Stop();

					break;
				}
				
				m_pFsm->Set(C_MBB_PACK_SEND_END);
			}
			break;
		}
		case C_MBB_PACK_SEND_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_PACK_SEND_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			memcpy(&TRAY_PART_DATA[nvMbbQc], &TRAY_PART_DATA[nvMbbShuttle], sizeof(IPC_MMI::TrayPartData));
			ZeroMemory(&TRAY_PART_DATA[nvMbbShuttle], sizeof(IPC_MMI::TrayPartData));
			if (!NV.use[useMbbQc])
				TRAY_PART_DATA[nvMbbQc].exist = enExistSealedQc;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbShuttle::CycleMbbSealingTest(void)
{
	if (!m_pFsm->Between(C_MBB_TEST_SEALING_START, C_MBB_TEST_SEALING_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_PACK_SEND_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_MBB_TEST_SEALING_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_START", m_strName);
				m_tmCycleTime.Reset();
				NV.ndm[850] = 0;
			}

			// 압력 변경
			NV.ddm[sealingPressureRegulator] = NV.rcp[sealing1stPressureRegulator];
			SEAL[sealMbb].SetInit();
			
			m_pFsm->Set(C_MBB_TEST_SEALING_CLAMP);
			break;
		}
		case C_MBB_TEST_SEALING_CLAMP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_CLAMP", m_strName);

			if (!CYL[cylSealBtmGuideUd].IsUp())
				CYL[cylSealBtmGuideUd].Up(SEAL_BTM_GUIDE_UP);

			if (!CYL[cylSealTopGuideUd].IsDn())
				CYL[cylSealTopGuideUd].Dn(SEAL_TOP_GUIDE_DN);

			if (!CYL[cylSealBtmGuideUd].IsUp() || !CYL[cylSealTopGuideUd].IsDn())
				break;

			if (!CYL[cylSealerBtmUd].IsUp())
			{
				CYL[cylSealerBtmUd].Up(SEALER_BTM_UP);
				break;
			}

			if (!CYL[cylSealerTopFtUd].IsDn() || !CYL[cylSealerTopRrUd].IsDn())
			{
				CYL[cylSealerTopFtUd].Dn(SEALER_TOP_FT_DN);
				CYL[cylSealerTopRrUd].Dn(SEALER_TOP_RR_DN);
				break;
			}

			m_pFsm->Set(C_MBB_TEST_SEALING_WORK);
			break;
		}
		case C_MBB_TEST_SEALING_WORK:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_WORK", m_strName);
			}

			if (BOn(iBSealcontactDetected))
				break;

			SEAL[sealMbb].Start();

			if (NV.ndm[850] == 0)
			{
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_WORK L1:[%0.3f], L2:[%0.3f], LOut[%.3f]", m_strName, NV.ddm[sealingloadcell1], NV.ddm[sealingloadcell2], NV.ddm[sealingloadcellOutputValue]);
				NV.ndm[850] = 1;
			}

			if (SEAL[sealMbb].m_pReadData->machineStatus == 4)
			{
				NV.ddm[sealingPressureRegulator] = NV.rcp[sealing2ndPressureRegulator];
				m_pFsm->Set(C_MBB_TEST_SEALING_UP);
			}

			break;
		}
		case C_MBB_TEST_SEALING_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_UP", m_strName);

			if (SEAL[sealMbb].m_pReadData->machineStatus != 3)
				break;

			if (NV.ndm[850] == 1)
			{
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_WORK L1:[%0.3f], L2:[%0.3f], LOut[%.3f]", m_strName, NV.ddm[sealingloadcell1], NV.ddm[sealingloadcell2], NV.ddm[sealingloadcellOutputValue]);
				NV.ndm[850] = 0;
			}

			if (!CYL[cylSealTopGuideUd].IsUp())
				CYL[cylSealTopGuideUd].Up(SEAL_TOP_GUIDE_DN);

			if (!CYL[cylSealerTopFtUd].IsUp() || !CYL[cylSealerTopRrUd].IsUp())
			{
				CYL[cylSealerTopFtUd].Up(SEALER_TOP_FT_DN);
				CYL[cylSealerTopRrUd].Up(SEALER_TOP_RR_DN);
			}

			if (!CYL[cylSealTopGuideUd].IsUp() ||
				!CYL[cylSealerTopFtUd].IsUp() || !CYL[cylSealerTopRrUd].IsUp())
				break;

			//if (!CylInit())
			//	break;

			m_pFsm->Set(C_MBB_TEST_SEALING_END);
			break;
		}
		case C_MBB_TEST_SEALING_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_MBB_TEST_SEALING_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CMbbShuttle::CycleN2Test(void)
{
	if (!m_pFsm->Between(C_N2_TEST_START, C_N2_TEST_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_MBB_PACK_SEND_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
	case C_N2_TEST_START:
	{
		if (m_pFsm->Once())
		{
			LOG[logSEQ].Message("<%s> C_N2_TEST_START", m_strName);
			m_tmCycleTime.Reset();
		}

		m_pFsm->Set(C_N2_TEST_VAC);
		break;
	}
	case C_N2_TEST_VAC:
	{
		if (m_pFsm->Once())
			LOG[logSEQ].Message("<%s> C_N2_TEST_VAC", m_strName);

		if (m_pFsm->Delay(10000))
		{
			VC[vcSealNozzle].Off(SEAL_NOZZLE_OFF);

			ER.Save(ER_MBB_SEALING_VAC_CYCLE_TIME_OVER);
			m_pFsm->Set(C_ERROR);
			break;
		}

		if (!CYL[cylSealNozzleMfcN2Oc].IsClose())
		{
			CYL[cylSealNozzleMfcN2Oc].Close(SEAL_NOZZLE_MFC_CLOSE);
			break;
		}

		if (!VC[vcSealNozzle].IsOn())
			VC[vcSealNozzle].On(SEAL_NOZZLE_ON);

		if (!VC[vcSealNozzle].AOn())
			break;

		m_pFsm->Set(C_N2_TEST_BLOW);
		break;
	}
	case C_N2_TEST_BLOW:
	{
		if (m_pFsm->Once())
		{
			LOG[logSEQ].Message("<%s> C_N2_TEST_BLOW", m_strName);
		}

		if (!VC[vcSealNozzle].IsOff())
		{
			VC[vcSealNozzle].Off(SEAL_NOZZLE_OFF);
			break;
		}

		if (!CYL[cylSealNozzleMfcN2Oc].IsOpen())
		{
			CYL[cylSealNozzleMfcN2Oc].Open(SEAL_NOZZLE_MFC_OPEN);
			break;
		}

		// N2 Supply
		if (!Between(NV.rcp[setNozzleN2AoSetValue], 0.1, 5.0))
			NV.rcp[setNozzleN2AoSetValue] = 5.0;
		NV.ddm[setmfcValueAnalogOutput] = NV.rcp[setNozzleN2AoSetValue];
		On(oCylSealNozzleMfcN2Op);
		VC[vcSealNozzleN2].On();

		//double nAirValue = ((int)NV.rcp[setNozzleN2AoSetValue] / 100) * WEIDMULLER_AD_S7;
		//Set(aoAirSealNozzleN2, (int)nAirValue);

		m_pFsm->Set(C_N2_TEST_BLOW_CHECK);
		break;
	}
	case C_N2_TEST_BLOW_CHECK:
	{
		if (m_pFsm->Once())
			LOG[logSEQ].Message("<%s> C_N2_TEST_BLOW_CHECK", m_strName);

		if (m_pFsm->Delay(_30sec))
		{
			VC[vcSealNozzleN2].Off();
			Off(oCylSealNozzleMfcN2Op);

			ER.Save(ER_MMBB_SEALING_N2_BLOW_CHECK_CYCLE_TIME_OVER);
			m_pFsm->Set(C_ERROR);
			break;
		}

		//if (OPR.isDryRun && m_pFsm->Delay(_3sec))
		//	NV.ddm[setmfcValueAnalogOutput] = 0;

		if (NV.ddm[setmfcValueAnalogOutput] != 0)
			break;

		VC[vcSealNozzleN2].Off();
		Off(oCylSealNozzleMfcN2Op);

		m_pFsm->Set(C_N2_TEST_END);
		break;
	}
	case C_N2_TEST_END:
	{
		if (m_pFsm->Once())
			LOG[logSEQ].Message("<%s> C_N2_TEST_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

		m_pFsm->Set(C_IDLE);
		break;
	}
	}
}