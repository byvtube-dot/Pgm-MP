#include "..\..\Includes.h"

CFoamStackConv FOAM_STACK_CONV;

CFoamStackConv::CFoamStackConv()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CFoamStackConv::~CFoamStackConv()
{

}

void CFoamStackConv::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = "$";
	if ("FOAM_1_TO_2_SEND" == newValue)
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "FOAM_STACK_RECV_CONV", "FOAM_STACK_REAR_CONV");

		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRecvElevLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRecvElevRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackRtStopFb);
		
		FOAM_STACK_CONV.m_pSLog->RecordingOn(CV_FOAM_STACK);

	}
	else if ("FOAM_1_TO_3_SEND" == newValue)
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "FOAM_STACK_RECV_CONV", "FOAM_STACK_FRONT_CONV");

		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRecvElevLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRecvElevRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(CV_FOAM_STACK);
	}
	else if ("FOAM_1_TO_4_SEND" == newValue)
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "FOAM_STACK_RECV_CONV", "FOAM_STACK_SEND_CONV");

		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRecvElevLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRecvElevRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamSendElevLtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamSendElevRtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(CV_FOAM_STACK);
	}
	else if ("FOAM_2_TO_3_SEND" == newValue)
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "FOAM_STACK_REAR_CONV", "FOAM_STACK_FRONT_CONV");

		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(CV_FOAM_STACK);
	}
	else if ("FOAM_2_TO_4_SEND" == newValue)
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "FOAM_STACK_REAR_CONV", "FOAM_STACK_SEND_CONV");

		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamRrStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamSendElevLtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamSendElevRtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(CV_FOAM_STACK);
	}
	else if ("FOAM_3_TO_4_SEND" == newValue)
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "FOAM_STACK_FRONT_CONV", "FOAM_STACK_SEND_CONV");

		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamFtStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamSendElevLtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(cylFoamSendElevRtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOn(CV_FOAM_STACK);
	}
	else
	{
		FOAM_STACK_CONV.m_pSLog->XFR(newValue, MatId, "", "");
		
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRecvElevLtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRecvElevRrAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRecvElevLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRecvElevRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRrStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRrStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamRrStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamFtStackLiftUd);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamFtStackLtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamFtStackRtStopFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamSendElevLtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(cylFoamSendElevRtAlignFb);
		FOAM_STACK_CONV.m_pSLog->RecordingOff(CV_FOAM_STACK);
	}
}

void CFoamStackConv::Auto(void)
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

	if (!IsCanCycleFoamMove())
		return;

	m_pFsm->Set(C_FOAM_MOVE_START, GetMsgIdx());
	return;
}

void CFoamStackConv::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
		{
			CONV[CV_FOAM_STACK].Stop();
			m_pFsm->Set(C_IDLE);
		}
		return;
	}

	CycleFoamMove();
}

void CFoamStackConv::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "FoamCov");
	m_pFsm = &FSM[fsmFoamConv];

	m_pSLog = &SLOG[slogFoamConv];
	m_pSLog->SetXFRDeviceId("FOAM_CONV");
	m_pSLog->SetXFRMaterialType("FOAM");
	m_pSLog->Add(cylFoamRecvElevLtAlignFb, TRUE);
	m_pSLog->Add(cylFoamRecvElevRrAlignFb, TRUE);
	m_pSLog->Add(cylFoamRecvElevLtStopFb, TRUE);
	m_pSLog->Add(cylFoamRecvElevRtStopFb, TRUE);
	m_pSLog->Add(cylFoamRrStackLiftUd, TRUE);
	m_pSLog->Add(cylFoamRrStackLtStopFb, TRUE);
	m_pSLog->Add(cylFoamRrStackRtStopFb, TRUE);
	m_pSLog->Add(cylFoamFtStackLiftUd, TRUE);
	m_pSLog->Add(cylFoamFtStackLtStopFb, TRUE);
	m_pSLog->Add(cylFoamFtStackRtStopFb, TRUE);
	m_pSLog->Add(cylFoamSendElevLtAlignFb, TRUE);
	m_pSLog->Add(cylFoamSendElevRtAlignFb, TRUE);

	m_pSLog->Add(CV_FOAM_STACK, TRUE);
}

BOOL CFoamStackConv::Common(void)
{
	if (!Between(NV.rcp[foamStackConvStopDelay], 0.0, 5000.0))
		NV.rcp[foamStackConvStopDelay] = 200.0;

	if (!Exist1())
		FOAM_PART_DATA[nvFoamRecvElev].cnt = 0;
	if (!Exist2())
		FOAM_PART_DATA[nvFoamRrStack].cnt = 0;
	if (!Exist3())
		FOAM_PART_DATA[nvFoamFtStack].cnt = 0;
	if (!Exist4())
		FOAM_PART_DATA[nvFoamSendElev].cnt = 0;
	return TRUE;
}

BOOL CFoamStackConv::IsHwErr(void)
{
	if (!MT[mtFoamSendElevZ].m_state.isHome)
		return TRUE;
	if (!MT[mtFoamRecvElevZ].m_state.isHome)
		return TRUE;

	if (CYL[cylFoamRecvElevLtAlignFb].GetErr())
		return TRUE;
	if (CYL[cylFoamRecvElevRrAlignFb].GetErr())
		return TRUE;
	if (CYL[cylFoamRecvElevLtStopFb].GetErr())
		return TRUE;
	if (CYL[cylFoamRecvElevRtStopFb].GetErr())
		return TRUE;
	if (CYL[cylFoamRrStackLiftUd].GetErr())
		return TRUE;
	if (CYL[cylFoamRrStackLtStopFb].GetErr())
		return TRUE;
	if (CYL[cylFoamRrStackRtStopFb].GetErr())
		return TRUE;
	if (CYL[cylFoamFtStackLiftUd].GetErr())
		return TRUE;
	if (CYL[cylFoamFtStackLtStopFb].GetErr())
		return TRUE;
	if (CYL[cylFoamFtStackRtStopFb].GetErr())
		return TRUE;
	if (CYL[cylFoamSendElevLtAlignFb].GetErr())
		return TRUE;
	if (CYL[cylFoamSendElevRtAlignFb].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CFoamStackConv::IsHwRdy(void)
{
	if (!MT[mtFoamSendElevZ].IsRdy())
		return TRUE;
	if (!MT[mtFoamRecvElevZ].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CFoamStackConv::CylInit(int nRoute)
{
	BOOL isReady = TRUE;
	switch (nRoute)
	{
		case msgMzMoveLift1To2:		// Recv -> Rear
		{
			if (!CYL[cylFoamRecvElevLtStopFb].IsBwd() ||
				!CYL[cylFoamRecvElevRtStopFb].IsBwd())
			{
				CYL[cylFoamRecvElevLtStopFb].Bwd(FOAM_RECV_ELEV_LT_STOP_BWD);
				CYL[cylFoamRecvElevRtStopFb].Bwd(FOAM_RECV_ELEV_RT_STOP_BWD);
				isReady = FALSE;
			}

			if (!CYL[cylFoamRrStackLtStopFb].IsFwd() ||
				!CYL[cylFoamRrStackRtStopFb].IsFwd() || 
				!CYL[cylFoamRrStackLiftUd].IsDn())
			{
				CYL[cylFoamRrStackLtStopFb].Fwd(FOAM_RR_STACK_LT_STOP_FWD);
				CYL[cylFoamRrStackRtStopFb].Fwd(FOAM_RR_STACK_RT_STOP_FWD);
				CYL[cylFoamRrStackLiftUd].Dn(FOAM_RR_STACK_LIFT_DN);
				isReady = FALSE;
			}
			break;
		}
		case msgMzMoveLift1To3:		// Recv -> Rear -> Front
		{
			if (!CYL[cylFoamRecvElevLtStopFb].IsBwd() ||
				!CYL[cylFoamRecvElevRtStopFb].IsBwd())
			{
				CYL[cylFoamRecvElevLtStopFb].Bwd(FOAM_RECV_ELEV_LT_STOP_BWD);
				CYL[cylFoamRecvElevRtStopFb].Bwd(FOAM_RECV_ELEV_RT_STOP_BWD);
				isReady = FALSE;
			}
			
			if (!CYL[cylFoamRrStackLtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackRtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackLiftUd].IsDn())
			{
				CYL[cylFoamRrStackLtStopFb].Bwd(FOAM_RR_STACK_LT_STOP_BWD);
				CYL[cylFoamRrStackRtStopFb].Bwd(FOAM_RR_STACK_RT_STOP_BWD);
				CYL[cylFoamRrStackLiftUd].Dn(FOAM_RR_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamFtStackLtStopFb].IsFwd() ||
				!CYL[cylFoamFtStackRtStopFb].IsFwd() ||
				!CYL[cylFoamFtStackLiftUd].IsDn())
			{
				CYL[cylFoamFtStackLtStopFb].Fwd(FOAM_FT_STACK_LT_STOP_FWD);
				CYL[cylFoamFtStackRtStopFb].Fwd(FOAM_FT_STACK_RT_STOP_FWD);
				CYL[cylFoamFtStackLiftUd].Dn(FOAM_FT_STACK_LIFT_DN);
				isReady = FALSE;
			}

			break;
		}
		case msgMzMoveLift1To4:		// Recv -> Rear -> Front -> Send
		{
			if (!CYL[cylFoamRecvElevLtStopFb].IsBwd() ||
				!CYL[cylFoamRecvElevRtStopFb].IsBwd())
			{
				CYL[cylFoamRecvElevLtStopFb].Bwd(FOAM_RECV_ELEV_LT_STOP_BWD);
				CYL[cylFoamRecvElevRtStopFb].Bwd(FOAM_RECV_ELEV_RT_STOP_BWD);
				isReady = FALSE;
			}

			if (!CYL[cylFoamRrStackLtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackRtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackLiftUd].IsDn())
			{
				CYL[cylFoamRrStackLtStopFb].Bwd(FOAM_RR_STACK_LT_STOP_BWD);
				CYL[cylFoamRrStackRtStopFb].Bwd(FOAM_RR_STACK_RT_STOP_BWD);
				CYL[cylFoamRrStackLiftUd].Dn(FOAM_RR_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamFtStackLtStopFb].IsBwd() ||
				!CYL[cylFoamFtStackRtStopFb].IsBwd() ||
				!CYL[cylFoamFtStackLiftUd].IsDn())
			{
				CYL[cylFoamFtStackLtStopFb].Bwd(FOAM_FT_STACK_LT_STOP_BWD);
				CYL[cylFoamFtStackRtStopFb].Bwd(FOAM_FT_STACK_RT_STOP_BWD);
				CYL[cylFoamFtStackLiftUd].Dn(FOAM_FT_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamSendElevLtAlignFb].IsBwd() ||
				!CYL[cylFoamSendElevRtAlignFb].IsBwd())
			{
				CYL[cylFoamSendElevLtAlignFb].Bwd(FOAM_SEND_ELEV_LT_ALIGN_BWD);
				CYL[cylFoamSendElevRtAlignFb].Bwd(FOAM_SEND_ELEV_RT_ALIGN_BWD);
				isReady = FALSE;
			}

			break;
		}
		case msgMzMoveLift2To3:		// Rear -> Front
		{
			if (!CYL[cylFoamRrStackLtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackRtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackLiftUd].IsDn())
			{
				CYL[cylFoamRrStackLtStopFb].Bwd(FOAM_RR_STACK_LT_STOP_BWD);
				CYL[cylFoamRrStackRtStopFb].Bwd(FOAM_RR_STACK_RT_STOP_BWD);
				CYL[cylFoamRrStackLiftUd].Dn(FOAM_RR_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamFtStackLtStopFb].IsFwd() ||
				!CYL[cylFoamFtStackRtStopFb].IsFwd() ||
				!CYL[cylFoamFtStackLiftUd].IsDn())
			{
				CYL[cylFoamFtStackLtStopFb].Fwd(FOAM_FT_STACK_LT_STOP_FWD);
				CYL[cylFoamFtStackRtStopFb].Fwd(FOAM_FT_STACK_RT_STOP_FWD);
				CYL[cylFoamFtStackLiftUd].Dn(FOAM_FT_STACK_LIFT_DN);
				isReady = FALSE;
			}
			break;
		}
		case msgMzMoveLift2To4:		// Rear -> Front -> Send
		{
			if (!CYL[cylFoamRrStackLtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackRtStopFb].IsBwd() ||
				!CYL[cylFoamRrStackLiftUd].IsDn())
			{
				CYL[cylFoamRrStackLtStopFb].Bwd(FOAM_RR_STACK_LT_STOP_BWD);
				CYL[cylFoamRrStackRtStopFb].Bwd(FOAM_RR_STACK_RT_STOP_BWD);
				CYL[cylFoamRrStackLiftUd].Dn(FOAM_RR_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamFtStackLtStopFb].IsBwd() ||
				!CYL[cylFoamFtStackRtStopFb].IsBwd() ||
				!CYL[cylFoamFtStackLiftUd].IsDn())
			{
				CYL[cylFoamFtStackLtStopFb].Bwd(FOAM_FT_STACK_LT_STOP_BWD);
				CYL[cylFoamFtStackRtStopFb].Bwd(FOAM_FT_STACK_RT_STOP_BWD);
				CYL[cylFoamFtStackLiftUd].Dn(FOAM_FT_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamSendElevLtAlignFb].IsBwd() ||
				!CYL[cylFoamSendElevRtAlignFb].IsBwd())
			{
				CYL[cylFoamSendElevLtAlignFb].Bwd(FOAM_SEND_ELEV_LT_ALIGN_BWD);
				CYL[cylFoamSendElevRtAlignFb].Bwd(FOAM_SEND_ELEV_RT_ALIGN_BWD);
				isReady = FALSE;
			}

			break;
		}
		case msgMzMoveLift3To4:		// Front -> Send
		{
			if (!CYL[cylFoamFtStackLtStopFb].IsBwd() ||
				!CYL[cylFoamFtStackRtStopFb].IsBwd() ||
				!CYL[cylFoamFtStackLiftUd].IsDn())
			{
				CYL[cylFoamFtStackLtStopFb].Bwd(FOAM_FT_STACK_LT_STOP_BWD);
				CYL[cylFoamFtStackRtStopFb].Bwd(FOAM_FT_STACK_RT_STOP_BWD);
				CYL[cylFoamFtStackLiftUd].Dn(FOAM_FT_STACK_LIFT_DN);
				isReady = FALSE;
			}

			if (!CYL[cylFoamSendElevLtAlignFb].IsBwd() ||
				!CYL[cylFoamSendElevRtAlignFb].IsBwd())
			{
				CYL[cylFoamSendElevLtAlignFb].Bwd(FOAM_SEND_ELEV_LT_ALIGN_BWD);
				CYL[cylFoamSendElevRtAlignFb].Bwd(FOAM_SEND_ELEV_RT_ALIGN_BWD);
				isReady = FALSE;
			}
			break;
		}
		default:
		{
			if (!CYL[cylFoamRecvElevLtStopFb].IsFwd() ||
				!CYL[cylFoamRecvElevRtStopFb].IsFwd())
			{
				CYL[cylFoamRecvElevLtStopFb].Fwd(FOAM_RECV_ELEV_LT_STOP_FWD);
				CYL[cylFoamRecvElevRtStopFb].Fwd(FOAM_RECV_ELEV_RT_STOP_FWD);
				isReady = FALSE;
			}
			if (!CYL[cylFoamRrStackLtStopFb].IsFwd() ||
				!CYL[cylFoamRrStackRtStopFb].IsFwd() ||
				!CYL[cylFoamRrStackLiftUd].IsUp())
			{
				CYL[cylFoamRrStackLtStopFb].Fwd(FOAM_RR_STACK_LT_STOP_FWD);
				CYL[cylFoamRrStackRtStopFb].Fwd(FOAM_RR_STACK_RT_STOP_FWD);
				CYL[cylFoamRrStackLiftUd].Up(FOAM_RR_STACK_LIFT_UP);
				isReady = FALSE;
			}
			if (!CYL[cylFoamFtStackLtStopFb].IsFwd() ||
				!CYL[cylFoamFtStackRtStopFb].IsFwd() ||
				!CYL[cylFoamFtStackLiftUd].IsUp())
			{
				CYL[cylFoamFtStackLtStopFb].Fwd(FOAM_FT_STACK_LT_STOP_FWD);
				CYL[cylFoamFtStackRtStopFb].Fwd(FOAM_FT_STACK_RT_STOP_FWD);
				CYL[cylFoamFtStackLiftUd].Up(FOAM_FT_STACK_LIFT_UP);
				isReady = FALSE;
			}

			break;
		}
	}

	return isReady;
}

void CFoamStackConv::SetFoamConvAlarm(int nRoute)
{
	DWORD delay = 1000;
	if (OPR.isDryRun)
		delay = 0;

	switch (nRoute)
	{
		case msgMzMoveLift1To2:
		{
			if (!AOff(iFoamRecvElevExistBtm, delay) || !AOff(iFoamRecvElevExistTop, delay))
				ER.Save(ER_FOAM_RECV_ELEV_SENSOR_ON);
			if (!AOn(iFoamRrStackExistBtm, delay) || !AOn(iFoamRrStackExistTop, delay))
				ER.Save(ER_FOAM_RR_STACK_SENSOR_OFF);
			break;
		}
		case msgMzMoveLift1To3:
		{
			if (!AOff(iFoamRecvElevExistBtm, delay) || !AOff(iFoamRecvElevExistTop, delay))
				ER.Save(ER_FOAM_RECV_ELEV_SENSOR_ON);
			if (!AOff(iFoamRrStackExistBtm, delay) || !AOff(iFoamRrStackExistTop, delay))
				ER.Save(ER_FOAM_RR_STACK_SENSOR_ON);
			if (!AOn(iFoamFtStackExistBtm, delay) || !AOn(iFoamFtStackExistTop, delay))
				ER.Save(ER_FOAM_FT_STACK_SENSOR_OFF);
			break;
		}
		case msgMzMoveLift1To4:
		{
			if (!AOff(iFoamRecvElevExistBtm, delay) || !AOff(iFoamRecvElevExistTop, delay))
				ER.Save(ER_FOAM_RECV_ELEV_SENSOR_ON);
			if (!AOff(iFoamRrStackExistBtm, delay) || !AOff(iFoamRrStackExistTop, delay))
				ER.Save(ER_FOAM_RR_STACK_SENSOR_ON);
			if (!AOff(iFoamFtStackExistBtm, delay) || !AOff(iFoamFtStackExistTop, delay))
				ER.Save(ER_FOAM_FT_STACK_SENSOR_ON);
			if (!AOn(iFoamSendElevExistBtm, delay)/* || !AOn(iFoamSendElevExistTop, delay)*/)
				ER.Save(ER_FOAM_SEND_STACK_SENSOR_OFF);
			break;
		}
		case msgMzMoveLift2To3:
		{
			if (!AOff(iFoamRrStackExistBtm, delay) || !AOff(iFoamRrStackExistTop, delay))
				ER.Save(ER_FOAM_RR_STACK_SENSOR_ON);
			if (!AOn(iFoamFtStackExistBtm, delay) || !AOn(iFoamFtStackExistTop, delay))
				ER.Save(ER_FOAM_FT_STACK_SENSOR_OFF);

			break;
		}
		case msgMzMoveLift2To4:
		{
			if (!AOff(iFoamRrStackExistBtm, delay) || !AOff(iFoamRrStackExistTop, delay))
				ER.Save(ER_FOAM_RR_STACK_SENSOR_ON);
			if (!AOff(iFoamFtStackExistBtm, delay) || !AOff(iFoamFtStackExistTop, delay))
				ER.Save(ER_FOAM_FT_STACK_SENSOR_ON);
			if (!AOn(iFoamSendElevExistBtm, delay)/* || !AOn(iFoamSendElevExistTop, delay)*/)
				ER.Save(ER_FOAM_SEND_STACK_SENSOR_OFF);

			break;
		}
		case msgMzMoveLift3To4:
		{
			if (!AOff(iFoamFtStackExistBtm, delay) || !AOff(iFoamFtStackExistTop, delay))
				ER.Save(ER_FOAM_FT_STACK_SENSOR_ON);
			if (!AOn(iFoamSendElevExistBtm, delay)/* || !AOn(iFoamSendElevExistTop, delay)*/)
				ER.Save(ER_FOAM_SEND_STACK_SENSOR_OFF);

			break;
		}
	}

}

BOOL CFoamStackConv::SetFoamConvMem(int nRoute)
{
	int ndleay = (int)NV.rcp[foamStackConvStopDelay];
	if (OPR.isDryRun)
		ndleay = 0;
	switch (nRoute)
	{
		case msgMzMoveLift1To2:
		{
			if (AOff(iFoamRecvElevExistBtm, ndleay) &&
				AOff(iFoamRecvElevExistTop, ndleay) &&
				AOn(iFoamRrStackExistBtm, ndleay) &&
				AOn(iFoamRrStackExistTop, ndleay))
			{
				memcpy(&FOAM_PART_DATA[nvFoamRrStack], &FOAM_PART_DATA[nvFoamRecvElev], sizeof(IPC_MMI::FoamPartData));
				ZeroMemory(&FOAM_PART_DATA[nvFoamRrStack].flag, sizeof(IPC_MMI::FoamFlag));
				ZeroMemory(&FOAM_PART_DATA[nvFoamRecvElev], sizeof(IPC_MMI::FoamPartData));
				return TRUE;
			}
			break;
		}
		case msgMzMoveLift1To3:
		{
			if (AOff(iFoamRecvElevExistBtm, ndleay) &&
				AOff(iFoamRecvElevExistTop, ndleay) &&
				AOff(iFoamRrStackExistBtm, ndleay) &&
				AOff(iFoamRrStackExistTop, ndleay) &&
				AOn(iFoamFtStackExistBtm, ndleay) &&
				AOn(iFoamFtStackExistTop, ndleay))
			{
				memcpy(&FOAM_PART_DATA[nvFoamFtStack], &FOAM_PART_DATA[nvFoamRecvElev], sizeof(IPC_MMI::FoamPartData));
				ZeroMemory(&FOAM_PART_DATA[nvFoamFtStack].flag, sizeof(IPC_MMI::FoamFlag));
				ZeroMemory(&FOAM_PART_DATA[nvFoamRecvElev], sizeof(IPC_MMI::FoamPartData));
				return TRUE;
			}
			break;
		}
		case msgMzMoveLift1To4:
		{
			if (AOff(iFoamRecvElevExistBtm, ndleay) &&
				AOff(iFoamRecvElevExistTop, ndleay) &&
				AOff(iFoamRrStackExistBtm, ndleay) &&
				AOff(iFoamRrStackExistTop, ndleay) &&
				AOff(iFoamFtStackExistBtm, ndleay) &&
				AOff(iFoamFtStackExistTop, ndleay) &&
				AOn(iFoamSendElevExistBtm, ndleay)/* &&
				AOn(iFoamSendElevExistTop, ndleay)*/)
			{
				memcpy(&FOAM_PART_DATA[nvFoamSendElev], &FOAM_PART_DATA[nvFoamRecvElev], sizeof(IPC_MMI::FoamPartData));
				ZeroMemory(&FOAM_PART_DATA[nvFoamSendElev].flag, sizeof(IPC_MMI::FoamFlag));
				ZeroMemory(&FOAM_PART_DATA[nvFoamRecvElev], sizeof(IPC_MMI::FoamPartData));
				return TRUE;
			}
			break;
		}
		case msgMzMoveLift2To3:
		{
			if (AOff(iFoamRrStackExistBtm, ndleay) &&
				AOff(iFoamRrStackExistTop, ndleay) &&
				AOn(iFoamFtStackExistBtm, ndleay) &&
				AOn(iFoamFtStackExistTop, ndleay))
			{
				memcpy(&FOAM_PART_DATA[nvFoamFtStack], &FOAM_PART_DATA[nvFoamRrStack], sizeof(IPC_MMI::FoamPartData));
				ZeroMemory(&FOAM_PART_DATA[nvFoamFtStack].flag, sizeof(IPC_MMI::FoamFlag));
				ZeroMemory(&FOAM_PART_DATA[nvFoamRrStack], sizeof(IPC_MMI::FoamPartData));
				return TRUE;
			}
			break;
		}
		case msgMzMoveLift2To4:
		{
			if (AOff(iFoamRrStackExistBtm, ndleay) &&
				AOff(iFoamRrStackExistTop, ndleay) &&
				AOff(iFoamFtStackExistBtm, ndleay) &&
				AOff(iFoamFtStackExistTop, ndleay) &&
				AOn(iFoamSendElevExistBtm, ndleay)/* &&
				AOn(iFoamSendElevExistTop, ndleay)*/)
			{
				memcpy(&FOAM_PART_DATA[nvFoamSendElev], &FOAM_PART_DATA[nvFoamRrStack], sizeof(IPC_MMI::FoamPartData));
				ZeroMemory(&FOAM_PART_DATA[nvFoamSendElev].flag, sizeof(IPC_MMI::FoamFlag));
				ZeroMemory(&FOAM_PART_DATA[nvFoamRrStack], sizeof(IPC_MMI::FoamPartData));
				return TRUE;
			}
			break;
		}
		case msgMzMoveLift3To4:
		{
			if (AOff(iFoamFtStackExistBtm, ndleay) &&
				AOff(iFoamFtStackExistTop, ndleay) &&
				AOn(iFoamSendElevExistBtm, ndleay)/* &&
				AOn(iFoamSendElevExistTop, ndleay)*/)
			{
				memcpy(&FOAM_PART_DATA[nvFoamSendElev], &FOAM_PART_DATA[nvFoamFtStack], sizeof(IPC_MMI::FoamPartData));
				ZeroMemory(&FOAM_PART_DATA[nvFoamSendElev].flag, sizeof(IPC_MMI::FoamFlag));
				ZeroMemory(&FOAM_PART_DATA[nvFoamFtStack], sizeof(IPC_MMI::FoamPartData));
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

BOOL CFoamStackConv::IsCanCycleFoamMove(void)
{
	int nIdx = GetMsgIdx();
	if (!Between(nIdx, msgMzMoveLift1To2, msgMzMoveLift3To4))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Index out of range");
		return FALSE;
	}

	if (FOAM_ELEV[enFoamRecvElev].m_pFsm->IsBusy() ||
		FOAM_ELEV[enFoamSendElev].m_pFsm->IsBusy())
		return FALSE;

	if (nIdx == msgMzMoveLift1To2 || nIdx == msgMzMoveLift1To3 || nIdx == msgMzMoveLift1To4)
	{
		if (FOAM_ELEV[enFoamRecvElev].m_pFsm->IsBusy())
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Recv Elev Cycle is Busy");
			return FALSE;
		}
	}

	if (nIdx == msgMzMoveLift1To4 || nIdx == msgMzMoveLift2To4 || nIdx == msgMzMoveLift3To4)
	{
		if (FOAM_ELEV[enFoamSendElev].m_pFsm->IsBusy())
		{
			MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
				"Foam Send Elev Cycle is Busy");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFoamStackConv::ExistCrossCheck(void)
{
	if (!m_pFsm->IsBusy())
	{
		if (!Exist2() && (AOn(iFoamRrStackExistBtm) || AOn(iFoamRrStackExistTop)))
		{
			ER.Save(ER_FOAM_RECV_ELEV_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist2() && AOff(iFoamRrStackExistBtm) && AOff(iFoamRrStackExistTop))
		{
			ER.Save(ER_FOAM_RECV_ELEV_MEMORY_STATUS);
			return TRUE;
		}

		if (!Exist3() && (AOn(iFoamFtStackExistBtm) || AOn(iFoamFtStackExistTop)))
		{
			ER.Save(ER_FOAM_RECV_ELEV_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist3() && AOff(iFoamFtStackExistBtm) && AOff(iFoamFtStackExistTop))
		{
			ER.Save(ER_FOAM_RECV_ELEV_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CFoamStackConv::Exist1(void)
{
	return FOAM_PART_DATA[nvFoamRecvElev].exist;
}

int& CFoamStackConv::Exist2(void)
{
	return FOAM_PART_DATA[nvFoamRrStack].exist;
}

int& CFoamStackConv::Exist3(void)
{
	return FOAM_PART_DATA[nvFoamFtStack].exist;
}

int& CFoamStackConv::Exist4(void)
{
	return FOAM_PART_DATA[nvFoamSendElev].exist;
}

BOOL CFoamStackConv::IsExist1(void)
{
	if (!FOAM_PART_DATA[nvFoamRecvElev].exist ||
		(AOff(iFoamRecvElevExistBtm) && AOff(iFoamRecvElevExistTop)))
	{
		ER.Save(ER_FOAM_RECV_ELEV_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsEmpty1(void)
{
	if (FOAM_PART_DATA[nvFoamRecvElev].exist ||
		AOn(iFoamRecvElevExistBtm) || AOn(iFoamRecvElevExistTop))
	{
		ER.Save(ER_FOAM_RECV_ELEV_EXIST);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsExist2(void)
{
	if (!FOAM_PART_DATA[nvFoamRrStack].exist ||
		(AOff(iFoamRrStackExistBtm) && AOff(iFoamRrStackExistTop)))
	{
		ER.Save(ER_FOAM_RR_STACK_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsEmpty2(void)
{
	if (FOAM_PART_DATA[nvFoamRrStack].exist ||
		AOn(iFoamRrStackExistBtm) || AOn(iFoamRrStackExistTop))
	{
		ER.Save(ER_FOAM_RR_STACK_EXIST);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsExist3(void)
{
	if (!FOAM_PART_DATA[nvFoamFtStack].exist ||
		(AOff(iFoamFtStackExistBtm) && AOff(iFoamFtStackExistTop)))
	{
		ER.Save(ER_FOAM_FT_STACK_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsEmpty3(void)
{
	if (FOAM_PART_DATA[nvFoamFtStack].exist ||
		AOn(iFoamFtStackExistBtm) || AOn(iFoamFtStackExistTop))
	{
		ER.Save(ER_FOAM_FT_STACK_EXIST);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsExist4(void)
{
	if (!FOAM_PART_DATA[nvFoamSendElev].exist ||
		(AOff(iFoamSendElevExistBtm) && AOff(iFoamSendElevExistTop)))
	{
		ER.Save(ER_FOAM_SEND_STACK_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CFoamStackConv::IsEmpty4(void)
{
	if (FOAM_PART_DATA[nvFoamSendElev].exist ||
		AOn(iFoamSendElevExistBtm)/* || AOn(iFoamSendElevExistTop)*/)
	{
		ER.Save(ER_FOAM_SEND_STACK_EXIST);
		return FALSE;
	}

	return TRUE;
}

int CFoamStackConv::GetMsgIdx(void)
{
	if (!Exist4())
	{
		if (Exist3()) return msgMzMoveLift3To4;
		if (Exist2()) return msgMzMoveLift2To4;
		if (Exist1() && FOAM_PART_DATA[nvFoamRecvElev].flag.recvFullDid) return msgMzMoveLift1To4;
	}
	else if (!Exist3())
	{
		if (Exist2()) return msgMzMoveLift2To3;
		if (Exist1() && FOAM_PART_DATA[nvFoamRecvElev].flag.recvFullDid) return msgMzMoveLift1To3;
	}
	else if (!Exist2())
	{
		if (Exist1() && FOAM_PART_DATA[nvFoamRecvElev].flag.recvFullDid) return msgMzMoveLift1To2;
	}

	return NOT_DEFINED;
}

void CFoamStackConv::CycleFoamMove(void)
{
	if (!m_pFsm->Between(C_FOAM_MOVE_START, C_FOAM_MOVE_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_FOAM_MOVE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nRoute = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_FOAM_MOVE_START:
		{
			if (m_pFsm->Once())
			{
				if (nRoute == msgMzMoveLift1To2)
					m_state = "FOAM_1_TO_2_SEND";
				else if (nRoute == msgMzMoveLift1To3)
					m_state = "FOAM_1_TO_3_SEND";
				else if (nRoute == msgMzMoveLift1To4)
					m_state = "FOAM_1_TO_4_SEND";
				else if (nRoute == msgMzMoveLift2To3)
					m_state = "FOAM_2_TO_3_SEND";
				else if (nRoute == msgMzMoveLift2To4)
					m_state = "FOAM_2_TO_4_SEND";
				else if (nRoute == msgMzMoveLift3To4)
					m_state = "FOAM_3_TO_4_SEND";

				LOG[logSEQ].Message("<%s> C_FOAM_MOVE_START [%01d]", m_strName, nRoute);
				m_tmCycleTime.Reset();
			}

			if (!CONV[CV_FOAM_STACK].IsStop())
			{
				CONV[CV_FOAM_STACK].Stop();
				break;
			}

			if (nRoute == msgMzMoveLift1To2 ||
				nRoute == msgMzMoveLift1To3 || nRoute == msgMzMoveLift1To4)
			{
				if (MT_NOTHING == MtPosMove(mtFoamRecvElevZ, P_ELEV_Z_BTM))
					break;
			}

			if (nRoute == msgMzMoveLift1To4 ||
				nRoute == msgMzMoveLift2To4 || nRoute == msgMzMoveLift3To4)
			{
				if (MT_NOTHING == MtPosMove(mtFoamSendElevZ, P_ELEV_Z_BTM))
					break;
			}

			if (!CylInit(nRoute))
				break;

			m_pFsm->Set(C_FOAM_MOVE_BELT_RUN);
			break;
		}
		case C_FOAM_MOVE_BELT_RUN:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_MOVE_BELT_RUN", m_strName);
				CONV[CV_FOAM_STACK].Fwd();
			}

			if (m_pFsm->TimeLimit(_30sec))
			{
				SetFoamConvAlarm(nRoute);
				m_pFsm->Set(C_ERROR);
				return;
			}

			if (!SetFoamConvMem(nRoute))
				break;

			m_pFsm->Set(C_FOAM_MOVE_BELT_STOP_DELAY);
			break;
		}
		case C_FOAM_MOVE_BELT_STOP_DELAY:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_FOAM_MOVE_BELT_STOP_DELAY", m_strName);
				CONV[CV_FOAM_STACK].Stop();
			}

			if (!CONV[CV_FOAM_STACK].IsStop(200))
				break;

			m_pFsm->Set(C_FOAM_MOVE_STOPPER_FWD);
			break;
		}
		case C_FOAM_MOVE_STOPPER_FWD:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_MOVE_STOPPER_FWD", m_strName);

			if (!CylInit())
				break;

			m_pFsm->Set(C_FOAM_MOVE_END);
			break;
		}
		case C_FOAM_MOVE_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_FOAM_MOVE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}