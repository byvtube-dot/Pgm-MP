#include "..\..\Includes.h"

CLabelShuttle LABEL_SHUTTLE[enlabelMax];

CLabelShuttle::CLabelShuttle()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CLabelShuttle::~CLabelShuttle()
{

}

void CLabelShuttle::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	int m_no = *reinterpret_cast<int*>(lpParam);
	if (oldValue == newValue)
		return;

	string MatId = static_cast<string> (LABEL_PART_DATA[nvLabelLtSuttle + m_no].labelInfo.sBoxId);
	if (0 == strcmp(LABEL_PART_DATA[nvLabelLtSuttle + m_no].labelInfo.sBoxId, ""))
		MatId = "$";

	if ("RECV" == newValue)
	{
		MatId = static_cast<string> (LABEL_PART_DATA[nvLabelLtFeeder + m_no].labelInfo.sBoxId);
		if (0 == strcmp(LABEL_PART_DATA[nvLabelLtFeeder + m_no].labelInfo.sBoxId, ""))
			MatId = "$";
		LABEL_SHUTTLE[m_no].m_pSLog->XFR(newValue, MatId, "LABEL_SHUTTLE", "LABEL_SHUTTLE");

		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn(LABEL_SHUTTLE[m_no].mtLabelShuttleY);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn(mtLabelLaserX);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn(LABEL_FEEDER[m_no].mtFeedRewindNum);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederMountFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederLockFtFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederLockRrFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvEjectPinLtUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvEjectPinRtUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((VacuumList)LABEL_SHUTTLE[m_no].m_pVacLabelRecv->m_no);
	}
	else if ("QC" == newValue)
	{
		LABEL_SHUTTLE[m_no].m_pSLog->XFR(newValue, MatId, "LABEL_SHUTTLE", "LABEL_SHUTTLE");

		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn(LABEL_SHUTTLE[m_no].mtLabelShuttleY);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederMountFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederLockFtFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederLockRrFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvEjectPinLtUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvEjectPinRtUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn(cylLabelQcViFb);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn((VacuumList)LABEL_SHUTTLE[m_no].m_pVacLabelRecv->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOn(VI_LABEL_QC);
	}
	else
	{
		LABEL_SHUTTLE[m_no].m_pSLog->XFR(newValue, MatId, "", "");

		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff(LABEL_SHUTTLE[m_no].mtLabelShuttleY);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff(mtLabelLaserX);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff(LABEL_FEEDER[m_no].mtFeedRewindNum);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederMountFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederLockFtFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelFeederLockRrFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvFb->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvEjectPinLtUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((CylinderList)LABEL_SHUTTLE[m_no].m_pLabelRecvEjectPinRtUd->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff(cylLabelQcViFb);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff((VacuumList)LABEL_SHUTTLE[m_no].m_pVacLabelRecv->m_no);
		LABEL_SHUTTLE[m_no].m_pSLog->RecordingOff(VI_LABEL_QC);
	}
}

void CLabelShuttle::Auto(void)
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
		if (!IsCanCycleLabelRecv())
			return;

		m_pFsm->Set(C_LABEL_RECV_START);
		return;
	}
	else
	{
		if (!IsCanCycleLabelQc())
			return;

		m_pFsm->Set(C_LABEL_QC_VI_START);
		return;
	}
}

void CLabelShuttle::Cycle(void)
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
			NV.ndm[ltLabelPrintTest] = FALSE;
			NV.ndm[rtLabelPrintTest] = FALSE;
		}
		return;
	}

	CycleLabelRecv();
	CycleLabelQc();
}

void CLabelShuttle::Init(int nIdx)
{
	m_no = nIdx;
	if (m_no == enlabelLt)
	{
		strcpy(m_strName, "LtLabelShuttle");
		m_pFsm = &FSM[fsmLtLabelShuttle];
		m_pRefFsm = &FSM[fsmRtLabelShuttle];

		m_pSLog = &SLOG[slogLtLabelShuttle];
		m_pSLog->SetXFRDeviceId("LT_LABEL_SHUTTLE");
		m_pSLog->SetXFRMaterialType("LABEL");

		m_pSLog->Add(mtLabelLtShuttleY);
		m_pSLog->Add(mtLabelLaserX);
		m_pSLog->Add(mtLabelLtFeed);

		m_pSLog->Add(cylLtLabelFeederFb);
		m_pSLog->Add(cylLtLabelFeederMountFb);
		m_pSLog->Add(cylLtLabelFeederLockFtFb);
		m_pSLog->Add(cylLtLabelFeederLockRrFb);
		m_pSLog->Add(cylLtLabelRecvUd);
		m_pSLog->Add(cylLtLabelRecvFb);
		m_pSLog->Add(cylLtLabelRecvEjectPinLtUd);
		m_pSLog->Add(cylLtLabelRecvEjectPinRtUd);
		m_pSLog->Add(cylLabelQcViFb);

		m_pSLog->Add(vcLtLabelRecv);

		m_pSLog->Add(VI_LABEL_QC);

		nvLabelShuttle = nvLabelLtSuttle;
		nvRefLabelShuttle = nvLabelRtSuttle;
		mtLabelShuttleY = mtLabelLtShuttleY;

		m_pLabelFeederFb = &CYL[cylLtLabelFeederFb];
		m_pLabelFeederMountFb = &CYL[cylLtLabelFeederMountFb];
		m_pLabelFeederLockFtFb = &CYL[cylLtLabelFeederLockFtFb];
		m_pLabelFeederLockRrFb = &CYL[cylLtLabelFeederLockRrFb];
		m_pLabelRecvUd = &CYL[cylLtLabelRecvUd];
		m_pLabelRecvFb = &CYL[cylLtLabelRecvFb];
		m_pLabelRecvEjectPinLtUd = &CYL[cylLtLabelRecvEjectPinLtUd];
		m_pLabelRecvEjectPinRtUd = &CYL[cylLtLabelRecvEjectPinRtUd];

		iLabelRecvExist = iLtLabelRecvExist;
		iLabelCstLabelExist = iLtLabelFeederLabelExist;
		iLabelCstDetectFt = iLtLabelFeederDetectFt;
		iLabelCstDetectRr = iLtLabelFeederDetectRr;

		m_pVacLabelRecv = &VC[vcLtLabelRecv];
	}
	else
	{
		strcpy(m_strName, "RtLabelShuttle");
		m_pFsm = &FSM[fsmRtLabelShuttle];
		m_pRefFsm = &FSM[fsmLtLabelShuttle];

		m_pSLog = &SLOG[slogRtLabelShuttle];
		m_pSLog->SetXFRDeviceId("RT_LABEL_SHUTTLE");
		m_pSLog->SetXFRMaterialType("LABEL");
		
		m_pSLog->Add(mtLabelRtShuttleY);
		m_pSLog->Add(mtLabelLaserX);
		m_pSLog->Add(mtLabelRtFeed);

		m_pSLog->Add(cylRtLabelFeederFb);
		m_pSLog->Add(cylRtLabelFeederMountFb);
		m_pSLog->Add(cylRtLabelFeederLockFtFb);
		m_pSLog->Add(cylRtLabelFeederLockRrFb);
		m_pSLog->Add(cylRtLabelRecvUd);
		m_pSLog->Add(cylRtLabelRecvFb);
		m_pSLog->Add(cylRtLabelRecvEjectPinLtUd);
		m_pSLog->Add(cylRtLabelRecvEjectPinRtUd);
		m_pSLog->Add(cylLabelQcViFb);

		m_pSLog->Add(vcRtLabelRecv);

		m_pSLog->Add(VI_LABEL_QC);

		nvLabelShuttle = nvLabelRtSuttle;
		nvRefLabelShuttle = nvLabelLtSuttle;
		mtLabelShuttleY = mtLabelRtShuttleY;

		m_pLabelFeederFb = &CYL[cylRtLabelFeederFb];
		m_pLabelFeederMountFb = &CYL[cylRtLabelFeederMountFb];
		m_pLabelFeederLockFtFb = &CYL[cylRtLabelFeederLockFtFb];
		m_pLabelFeederLockRrFb = &CYL[cylRtLabelFeederLockRrFb];
		m_pLabelRecvUd = &CYL[cylRtLabelRecvUd];
		m_pLabelRecvFb = &CYL[cylRtLabelRecvFb];
		m_pLabelRecvEjectPinLtUd = &CYL[cylRtLabelRecvEjectPinLtUd];
		m_pLabelRecvEjectPinRtUd = &CYL[cylRtLabelRecvEjectPinRtUd];

		iLabelRecvExist = iRtLabelRecvExist;
		iLabelCstLabelExist = iRtLabelFeederLabelExist;
		iLabelCstDetectFt = iRtLabelFeederDetectFt;
		iLabelCstDetectRr = iRtLabelFeederDetectRr;

		m_pVacLabelRecv = &VC[vcRtLabelRecv];
	}
}

BOOL CLabelShuttle::Common(void)
{

	return TRUE;
}

BOOL CLabelShuttle::IsHwErr(void)
{
	if (!MT[mtLabelShuttleY].m_state.isHome)
		return TRUE;

	if (m_pLabelRecvUd->GetErr())
		return TRUE;
	if (m_pLabelRecvFb->GetErr())
		return TRUE;
	if (m_pLabelRecvEjectPinLtUd->GetErr())
		return TRUE;
	if (m_pLabelRecvEjectPinRtUd->GetErr())
		return TRUE;

	return FALSE;
}

BOOL CLabelShuttle::IsHwRdy(void)
{
	if (!MT[mtLabelShuttleY].IsRdy())
		return TRUE;
	if (!MT[mtLabelLaserX].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CLabelShuttle::CylInit(void)
{
	BOOL isCylInitDone = TRUE;

	if (!m_pLabelRecvUd->IsDn())
	{
		m_pLabelRecvUd->Dn(LT_LABEL_RECV_DN);
		isCylInitDone = FALSE;
	}

	if (!m_pLabelRecvFb->IsBwd())
	{
		m_pLabelRecvFb->Bwd(LT_LABEL_RECV_BWD);
		isCylInitDone = FALSE;
	}

	if (!m_pLabelRecvEjectPinLtUd->IsDn() || !m_pLabelRecvEjectPinRtUd->IsDn())
	{
		m_pLabelRecvEjectPinLtUd->Dn(LT_LABEL_RECV_EJECT_PIN_LT_DN);
		m_pLabelRecvEjectPinRtUd->Dn(LT_LABEL_RECV_EJECT_PIN_RT_DN);
		isCylInitDone = FALSE;
	}

	if (m_pVacLabelRecv->IsOn())
	{
		m_pVacLabelRecv->BlowOn();
		isCylInitDone = FALSE;
	}

	return isCylInitDone;
}

BOOL CLabelShuttle::IsCanCycleLabelRecv(void)
{
	if (LABEL_LASER.m_pFsm->Between(LABEL_LASER.C_PWR_METER_START, LABEL_LASER.C_PWR_METER_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Laser Power Meter Cycle is Busy");
		return FALSE;
	}

	if (LABEL_FEEDER[m_no].m_pFsm->IsBusy() || LABEL_FEEDER[m_no].m_pRefFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Cycle is Busy");
		return FALSE;
	}

	if (LABEL_PART_DATA[nvLabelLtFeeder + m_no].exist != enLabelExistPrinted)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Memory Mismatch");
		return FALSE;
	}

	if (Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Memory Exist");
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelShuttle::IsCanCycleLabelQc(void)
{
	if (LABEL_LASER.m_pFsm->Between(LABEL_LASER.C_PWR_METER_START, LABEL_LASER.C_PWR_METER_END))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Laser Power Meter Cycle is Busy");
		return FALSE;
	}

	if (LABEL_PART_DATA[nvLabelShuttle].exist != enLabelExistPrinted)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Memory Mismatch");
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelShuttle::ExistCrossCheck(void)
{
	if (!m_pFsm->Between(C_LABEL_RECV_START, C_LABEL_RECV_END) &&
		!LT_COBOT.m_pFsm->Between(LT_COBOT.C_LABEL_PICK_START, LT_COBOT.C_LABEL_PICK_END) &&
		!RT_COBOT.m_pFsm->Between(RT_COBOT.C_LABEL_PICK_START, RT_COBOT.C_LABEL_PICK_END))
	{
		if (!Exist() && (AOn(iLabelRecvExist) || m_pVacLabelRecv->AOn()))
		{
			ER.Save((ErrName)(ER_LABEL_LT_SHUTTLE_SENSOR_STATUS + (m_no * 2)));
			return TRUE;
		}
		if (Exist() && (AOff(iLabelRecvExist) || m_pVacLabelRecv->AOff()))
		{
			ER.Save((ErrName)(ER_LABEL_LT_SHUTTLE_MEMORY_STATUS + (m_no * 2)));
			return TRUE;
		}
	}

	return FALSE;
}

int& CLabelShuttle::Exist(void)
{
	return LABEL_PART_DATA[nvLabelShuttle].exist;
}

BOOL CLabelShuttle::IsExist(void)
{
	if (!LABEL_PART_DATA[nvLabelShuttle].exist || m_pVacLabelRecv->AOff())
	{
		ER.Save((ErrName)(ER_LABEL_LT_SHUTTLE_EMPTY + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelShuttle::IsEmpty(void)
{
	if (LABEL_PART_DATA[nvLabelShuttle].exist ||
		AOn(iLabelRecvExist) || m_pVacLabelRecv->AOn())
	{
		ER.Save((ErrName)(ER_LABEL_LT_SHUTTLE_EXIST + (m_no * 2)));
		return FALSE;
	}

	return TRUE;
}

void CLabelShuttle::CycleLabelRecv(void)
{
	if (!m_pFsm->Between(C_LABEL_RECV_START, C_LABEL_RECV_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save((ErrName)(ER_LABEL_LT_RECV_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_RECV_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "RECV";
				m_FeedStep = 0;
			}

			if (!IsEmpty())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CylInit() || !LABEL_FEEDER[m_no].CylInit())
				break;

			if (!m_pLabelFeederMountFb->IsFwd())
			{
				if (!LABEL_FEEDER[m_no].FeederTrqZeroStop())
					break;

				m_pLabelFeederMountFb->Fwd(LT_LABEL_FEEDER_MOUNT_FWD);
				break;
			}

			if (!m_pLabelRecvFb->IsFwd())
				m_pLabelRecvFb->Fwd(LT_LABEL_RECV_FWD);

			m_pFsm->Set(C_LABEL_RECV_FEED_STOP);
			break;
		}
		case C_LABEL_RECV_FEED_STOP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_FEED_STOP", m_strName);

			if (!MT[LABEL_FEEDER[m_no].mtFeedRewindNum].IsRdy() ||
				AxisCommandMode::Position != MT[LABEL_FEEDER[m_no].mtFeedRewindNum].GetCmdMode())
			{
				MT[LABEL_FEEDER[m_no].mtFeedRewindNum].Stop();
				MT[LABEL_FEEDER[m_no].mtFeedRewindNum].SetCmdMode(AxisCommandMode::Position);
				break;
			}

			m_pFsm->Set(C_LABEL_RECV_MOVE);
			break;
		}
		case C_LABEL_RECV_MOVE:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_MOVE", m_strName);
				LABEL_PART_DATA[nvLabelLtFeeder + m_no].exist = false;
			}

			MtPosMove(mtLabelShuttleY, P_LABEL_SHUTTLE_Y_RECV);
			MtPosMove(mtLabelLaserX, P_LASER_X_LT_EXIST + m_no);

			if (!MT[mtLabelShuttleY].InPos(P_LABEL_SHUTTLE_Y_RECV) ||
				!MT[mtLabelLaserX].InPos(P_LASER_X_LT_EXIST + m_no))
				break;

			if (AOff(iLaserLabelPos))
				m_FeedStep = 1;

			m_pFsm->Set(C_LABEL_RECV_FEED_MOVE);
			break;
		}
		case C_LABEL_RECV_FEED_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_FEED_MOVE", m_strName);

			if (!MT[LABEL_FEEDER[m_no].mtFeedRewindNum].IsRdy())
				break;

			if (MT[LABEL_FEEDER[m_no].mtFeedRewindNum].CanMove(P_LABEL_T_FEED))
			{
				double dPos = 0.0;
				if (m_FeedStep)
				{
					dPos = MT[LABEL_FEEDER[m_no].mtFeedRewindNum].m_state.realCnt + MT[LABEL_FEEDER[m_no].mtFeedRewindNum].PosTable(P_LABEL_T_FEED_FAST);
					MT[LABEL_FEEDER[m_no].mtFeedRewindNum].PosMove(P_LABEL_T_FEED_FAST, dPos);
				}
				else
				{
					dPos = MT[LABEL_FEEDER[m_no].mtFeedRewindNum].m_state.realCnt + MT[LABEL_FEEDER[m_no].mtFeedRewindNum].PosTable(P_LABEL_T_FEED);
					MT[LABEL_FEEDER[m_no].mtFeedRewindNum].PosMove(P_LABEL_T_FEED_FAST, dPos);
				}

				m_pFsm->Set(C_LABEL_RECV_SENSOR_CHECK);
			}
			
			break;
		}
		case C_LABEL_RECV_SENSOR_CHECK:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_SENSOR_CHECK", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_RECV_FEED_SENSOR_CHECK_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (m_FeedStep)		// Sensor On Check
			{
				if (AOn(iLaserLabelPos, 10))
				{
					MT[LABEL_FEEDER[m_no].mtFeedRewindNum].Stop();
					m_FeedStep = 0;
					if (!m_pLabelRecvUd->IsUp())
					{
						if (m_pLabelRecvFb->IsFwd())
							m_pLabelRecvUd->Up(LT_LABEL_RECV_UP);
					}
					m_pFsm->Set(C_LABEL_RECV_FEED_MOVE);
					break;
				}
			}
			else				// Sensor Off Check
			{
				if (AOff(iLaserLabelPos, (int)NV.rcp[labelArrivalDelay]))
				{
					MT[LABEL_FEEDER[m_no].mtFeedRewindNum].Stop();
					// TODO: 25.09.10 To be replaced by sensor conditions
					LABEL_PART_DATA[nvLabelLtFeeder + m_no].cnt++;
					if (LABEL_PART_DATA[nvLabelLtFeeder + m_no].cnt >= NV.rcp[labelMaxCount])
						LABEL_PART_DATA[nvLabelLtFeeder + m_no].flag.cstMaterialEmpty = true;
					else
						LABEL_FEEDER[m_no].Exist() = TRUE;

					m_pFsm->Set(C_LABEL_RECV_UP);
				}
			}
			
			break;
		}
		case C_LABEL_RECV_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_UP", m_strName);

			MT[LABEL_FEEDER[m_no].mtFeedRewindNum].Stop();
			if (!m_pLabelRecvFb->IsFwd())
			{
				m_pLabelRecvFb->Fwd(LT_LABEL_RECV_FWD);
				break;
			}

			if (!m_pLabelRecvUd->IsUp())
			{
				if (m_pLabelRecvFb->IsFwd())
					m_pLabelRecvUd->Up(LT_LABEL_RECV_UP);
				break;
			}

			m_pFsm->Set(C_LABEL_RECV_VAC);
			break;
		}
		case C_LABEL_RECV_VAC:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_VAC", m_strName);

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_VAC_ALARM_LEFT_LABEL_RECV + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!m_pVacLabelRecv->IsOn())
				m_pVacLabelRecv->On(LT_LABEL_RECV_ON);

			if (!m_pVacLabelRecv->AOn() || !m_pVacLabelRecv->IsOn())
				break;

			m_pFsm->Set(C_LABEL_RECV_QC_MOVE);
			break;
		}
		case C_LABEL_RECV_QC_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_QC_MOVE", m_strName);
			
			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save((ErrName)(ER_LABEL_LT_RECV_FEED_SENSOR_CHECK_TIME_OVER + m_no));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!m_pLabelRecvUd->IsDn())
				m_pLabelRecvUd->Dn(LT_LABEL_RECV_DN);

			if (!m_pLabelRecvFb->IsBwd())
			{
				if (m_pLabelRecvUd->IsDn())
					m_pLabelRecvFb->Bwd(LT_LABEL_RECV_BWD);
			}

			if (MtPosMove(mtLabelShuttleY, P_LABEL_SHUTTLE_Y_QC) != MT_INPOS)
				break;

			if (!m_pLabelRecvUd->IsDn() || !m_pLabelRecvFb->IsBwd())
				break;

			if (!AOn(iLabelRecvExist))
				break;

			m_pFsm->Set(C_LABEL_RECV_END);
			break;
		}
		case C_LABEL_RECV_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_RECV_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			memcpy(&LABEL_PART_DATA[nvLabelShuttle].labelInfo, &LABEL_PART_DATA[nvLabelLtFeeder + m_no].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
			ZeroMemory(&LABEL_PART_DATA[nvLabelLtFeeder + m_no].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));

			LABEL_PART_DATA[nvLabelLtFeeder + m_no].exist = enLabelExist;
			Exist() = enLabelExistPrinted;

			if (LABEL_PART_DATA[nvRefLabelShuttle].exist)
				LABEL_PART_DATA[nvLabelShuttle].flag.lastlabel = true;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLabelShuttle::CycleLabelQc(void)
{
	if (!m_pFsm->Between(C_LABEL_QC_VI_START, C_LABEL_QC_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save((ErrName)(ER_LABEL_LT_QC_CYCLE_TIME_OVER + m_no));
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_QC_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "QC";

				viMode = MODE_LABEL_QC_PRINTED;
				VI[VI_LABEL_QC].Clear();
			}

			m_pFsm->Set(C_LABEL_QC_VI_DATA_SEND);
			break;
		}
		case C_LABEL_QC_VI_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				if (m_no == _RT)
					LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_DATA_SEND [RIGHT]", m_strName);
				else
					LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_DATA_SEND [LEFT]", m_strName);
				NV.ndm[reqViDataSendLabelQc] = COMM_REQ + m_no;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_LABEL_QC_VI_DATA_SEND_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[reqViDataSendLabelQc] != COMM_COMP)
				break;

			m_pFsm->Set(C_LABEL_QC_VI_MOVE);
			break;
		}
		case C_LABEL_QC_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_MOVE", m_strName);

			MtPosMove(mtLabelShuttleY, P_LABEL_SHUTTLE_Y_QC);

			if (m_no == _LT && !CYL[cylLabelQcViFb].IsFwd())
			{
				CYL[cylLabelQcViFb].Fwd(LABEL_QC_VI_FWD);
				break;
			}
			if (m_no == _RT && !CYL[cylLabelQcViFb].IsBwd())
			{
				CYL[cylLabelQcViFb].Bwd(LABEL_QC_VI_BWD);
				break;
			}

			if (MT_INPOS != MtPosMove(mtLabelShuttleY, P_LABEL_SHUTTLE_Y_QC))
				break;

			m_pFsm->Set(C_LABEL_QC_VI_RST);
			break;
		}
		case C_LABEL_QC_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_RST", m_strName);

			if (!VI[VI_LABEL_QC].IsModeOn(viMode, 10))
			{
				VI[VI_LABEL_QC].SetMode(viMode);
				break;
			}

			VI[VI_LABEL_QC].Reset();
			m_pFsm->Set(C_LABEL_QC_VI_TRIG);
			break;
		}
		case C_LABEL_QC_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_LABEL_QC_PRINTED_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_LABEL_QC].Start(viMode))
				m_pFsm->Set(C_LABEL_QC_VI_RESULT);

			break;
		}
		case C_LABEL_QC_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_LABEL_QC_PRINTED_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_LABEL_QC].IsBusy() || !VI[VI_LABEL_QC].m_isRcvComp)
				break;

			if (!VI[VI_LABEL_QC].m_qcData.result[0])
			{
				ER.Save(ER_VISION_LABEL_QC_PRINTED_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_QC_VI_SEND_MOVE);
			break;
		}
		case C_LABEL_QC_VI_SEND_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_SEND_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtLabelShuttleY, P_LABEL_SHUTTLE_Y_SEND))
				break;

			m_pFsm->Set(C_LABEL_QC_VI_END);
			break;
		}
		case C_LABEL_QC_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_QC_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			LABEL_PART_DATA[nvLabelShuttle].exist = enLabelExistQc;
			m_state = "";

			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}