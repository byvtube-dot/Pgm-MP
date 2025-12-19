#include "..\Includes.h"

void CTenkeyOpr::Init(PUSHORT pInAddr, PUSHORT pOutAddr)
{
	m_tenkey.SetIO(pInAddr, pOutAddr);
	m_tenkey.Enable();
}

void CTenkeyOpr::SetUnitofHundred(BOOL isUnitofHundred)
{
	m_tenkey.SetUnitofHundred(isUnitofHundred);
}

int CTenkeyOpr::GetTenkeyNo(void)
{
	BOOL disableTenkey = NV.ndm[jobEnable];
	disableTenkey |= OPR.isAuto;
	disableTenkey |= OPR.isEmg;
	disableTenkey |= OPR.isWrongParam;
	disableTenkey |= OPR.isCycleRun;
	disableTenkey |= OPR.isPaused;
	disableTenkey |= OPR.isDoorUnlock;
	disableTenkey |= OPR.isDoorOpen;
	disableTenkey |= FSM[fsmSEQ].IsBusy();

	if (disableTenkey)
	{
		m_isDisable = TRUE;
		m_tenkey.Disable();
	}
	else
	{
		if (NV.ndm[jobEnable])
		{
			m_isDisable = TRUE;
			m_tenkey.Disable();
		}
		else if (m_isDisable)
		{
			m_isDisable = FALSE;
			m_tenkey.Enable();
		}
	}

	int tenkeyNo = m_tenkey.GetTenkeyNo();
	if (0 <= tenkeyNo)
	{
		LOG[logSEQ].Message("Pressed Remote-Tenkey[%d]", tenkeyNo);
	}
	else if (tenkeyNo == HW::NO_KEY)
	{
		if (0 < NV.ndm[scrTenkeyNo])
		{
			tenkeyNo = NV.ndm[scrTenkeyNo];
			LOG[logSEQ].Message("Pressed Screen-Tenkey[%d]", tenkeyNo);
		}
	}
	NV.ndm[scrTenkeyNo] = HW::NO_KEY;
	
	if (!disableTenkey && 0 <= tenkeyNo)
		return tenkeyNo;
	
	return NOT_DEFINED;
}

void CTenkeyOpr::Jog(void)
{
	AxisList axisNo = AxisList(NV.ndm[jogAxisNo]);

	if (1 > NV.ndm[jogSpeed])
		NV.ndm[jogSpeed] = 1;
	if ((int)MT[axisNo].m_maxVel < NV.ndm[jogSpeed])
		NV.ndm[jogSpeed] = (int)MT[axisNo].m_maxVel;

	BOOL isLpressed = (HW::CLR_KEY == m_tenkey.GetKeyPadNo());
	BOOL isRpressed = (HW::SET_KEY == m_tenkey.GetKeyPadNo());

	BOOL stopJog = !NV.ndm[jobEnable];
	stopJog |= OPR.isAuto;
	stopJog |= OPR.isEmg;
	stopJog |= OPR.isWrongParam;
	stopJog |= OPR.isCycleRun;
	stopJog |= OPR.isPaused;
	stopJog |= OPR.isDoorUnlock;
	stopJog |= OPR.isDoorOpen;
	stopJog |= FSM[fsmSEQ].IsBusy();

	if (stopJog)
	{
		if ((C_JOGSTOP > m_fsmJog.Get()) && (C_IDLE != m_fsmJog.Get()))
		{
			m_fsmJog.Set(C_JOGSTOP);
		}
	}

	switch (m_fsmJog.Get())
	{
		case C_JOGWAIT:
		{
			if (stopJog || MT[axisNo].m_state.isDriving)
				break;

			if (OPR.isDoorUnlock || OPR.isDoorOpen)
				break;

			if (isLpressed || isRpressed)
			{
				if (MT[axisNo].m_nxtIdx < P_JOG)
				{
					MT[axisNo].m_nxtIdx = MT[axisNo].m_curIdx = P_JOG;
				}
			}
			double vel = (double)NV.ndm[jogSpeed];
			double acc = MT[axisNo].AccTable(MT[axisNo].m_homePosIdx);
			if (isRpressed)
			{
				if (NV.use[useMasterKeyMode])
				{
					MT[axisNo].JogBwd(vel, acc);
				}
				else
				{
					if (!MT[axisNo].CanMove(P_JOG))
						break;

					AntiCollisionJogBwd(axisNo, vel, acc);
				}

				m_fsmJog.Set(C_JOGMOVE_N);
			}
			else if (isLpressed)
			{
				if (NV.use[useMasterKeyMode])
				{
					MT[axisNo].JogFwd(vel, acc);
				}
				else
				{
					if (!MT[axisNo].CanMove(P_JOG))
						break;

					AntiCollisionJogFwd(axisNo, vel, acc);
				}

				m_fsmJog.Set(C_JOGMOVE_P);
			}
		}
		break;
		case C_JOGMOVE_N:
		{
			if (stopJog || !isRpressed)
			{
				m_fsmJog.Set(C_JOGSTOP);
			}
			else if (!MT[axisNo].m_state.isDriving)
			{
				if (m_fsmJog.Delay(100))
				{
					m_fsmJog.Set(C_IDLE);
				}
			}
		}
		break;
		case C_JOGMOVE_P:
		{
			if (stopJog || !isLpressed)
			{
				m_fsmJog.Set(C_JOGSTOP);
			}
			else if (!MT[axisNo].m_state.isDriving)
			{
				if (m_fsmJog.Delay(100))
				{
					m_fsmJog.Set(C_IDLE);
				}
			}
		}
		break;
		case C_JOGSTOP:
		{
			MT[axisNo].Stop();
			m_fsmJog.Set(C_IDLE);
		}
		break;
		case C_IDLE:
		{
			if (stopJog)
				break;
			if (HW::NO_KEY == m_tenkey.GetKeyPadNo())
				m_fsmJog.Set(C_JOGWAIT);
		}
		break;
		default:
		{
			m_fsmJog.Set(C_IDLE);
		}
		break;
	}
}