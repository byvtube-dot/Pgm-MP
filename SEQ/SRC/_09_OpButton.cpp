#include "..\Includes.h"

BOOL StartProcess(void)
{
	if (FSM[fsmSEQ].IsBusy())
		return (FALSE);

	// TODO: Program

	LOG[logSEQ].Message("Pressed Start Button");
	FSM[fsmSEQ].Set(C_OPERATION_START);
	return (TRUE);
}

BOOL StopProcess(void)
{
	if (OPR.isNoDevice)
	{
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].m_fsmDrv.Between(C_HOME_SEQ_START, C_HOME_SEQ_END))
				MT[mtno].m_fsmDrv.Cancel();
		}
	}

	//LASER_INDEX[Index1].IsOneCycle() = FALSE;
	//LASER_INDEX[Index2].IsOneCycle() = FALSE;
	//INPICK.m_oneCycle = "";//??CJS
	return (TRUE);
}

BOOL ResetProcess(void)
{
	// Vacuum Off
	for (int nVcNum = 0; nVcNum < vacuumMAX; nVcNum++)
	{
		if (nVcNum == vcSealNozzle)
		{
			VC[nVcNum].Off();
			continue;
		}

		if (nVcNum == vcSealNozzleN2)
			continue;

		if (!VC[nVcNum].AOn() && !VC[nVcNum].IsOff())
			VC[nVcNum].Off();
	}

	SEAL[sealMbb].ResetProcess();
	return (TRUE);
}

void OperationCycle(void)
{
	if (!FSM[fsmSEQ].Between(C_OPERATION_START, C_OPERATION_END))
		return;

	if (FSM[fsmSEQ].TimeLimit(_10sec))
	{
		ER.Save(ER_OPERATION_CYCLE_TIMEOVER);
		FSM[fsmSEQ].Set(C_ERROR);
		return;
	}

	if (0 < ER.GetNo() && ER.GetNo() < ER_MAX)
	{
		FSM[fsmSEQ].Set(C_ERROR);
		return;
	}

	switch (FSM[fsmSEQ].Get())
	{
	case C_OPERATION_START:
	{
		if (FSM[fsmSEQ].Once())
		{
			LOG[logSEQ].Message("C_OPERATION_START");
		}

		if (!FSM[fsmSEQ].Delay(200))
			break;

		FSM[fsmSEQ].Set(C_OPERATION_END);
	}
	break;
	case C_OPERATION_END:
	{
		if (FSM[fsmSEQ].Once())
		{
			LOG[logSEQ].Message("C_OPERATION_END");
		}

		OPR.isAuto = TRUE;
		OPR.isStop = FALSE;
		FSM[fsmSEQ].Set(C_IDLE);
	}
	break;
	}
}

COpBtn::COpBtn()
{
	m_stopBtnState = FALSE;
	m_startBtnState = FALSE;
	m_resetBtnState = FALSE;
}

void COpBtn::Run(void)
{
	BOOL isLampOn = LAMPBUZZER.GetBlink();

	if (OPR.isAuto)
	{
		On(oSwLampStart);
		On(oSwLampRrStart);
		NV.ndm[scrLampStart] = 1;
	}
	else
	{
		if (OPR.isCycleRun && isLampOn)
		{
			On(oSwLampStart);
			On(oSwLampRrStart);
			NV.ndm[scrLampStart] = 1;
		}
		else
		{
			Off(oSwLampStart);
			Off(oSwLampRrStart);
			NV.ndm[scrLampStart] = 0;
		}
	}

	Set(oSwLampStop, !OPR.isAuto);
	Set(oSwLampRrStop, !OPR.isAuto);
	NV.ndm[scrLampStop] = (int)!OPR.isAuto;

	if (0 < ER.GetNo() && ER.GetNo() < ER_MAX && isLampOn)
	{
		On(oSwLampReset);
		On(oSwLampRrReset);
		NV.ndm[scrLampReset] = 1;
	}
	else
	{
		Off(oSwLampReset);
		Off(oSwLampRrReset);
		NV.ndm[scrLampReset] = 0;
	}

	Start();
	Stop();
	Reset();
	Break();
}

void COpBtn::Stop(void)
{
	if (!WMX.isConnected)
	{
		NV.ndm[scrBtnStop] = 0;
		return;
	}

	BOOL isStopBtnOn = AOn(iSwStop) || AOn(iSwRrStop);
	m_tmBtnStop.Reset(!isStopBtnOn);

	BOOL isPressedStop = NV.ndm[scrBtnStop];
	NV.ndm[scrBtnStop] = 0;

	if (m_stopBtnState != isStopBtnOn)
	{
		if (isStopBtnOn)
		{
			if (m_tmBtnStop.TmOver(10))
			{
				isPressedStop = TRUE;
				m_stopBtnState = isStopBtnOn;
			}
		}
		else
		{
			m_stopBtnState = isStopBtnOn;
		}
	}

	if (isPressedStop)
	{
		LOG[logSEQ].Message("Pressed Stop Button");

		NV.ndm[jobEnable] = 0;
		for (int i = 0; i < fsmMAX; i++)
			FSM[FsmList(i)].Cancel();

		StopProcess();
		OPR.isStop = (TRUE);
	}
}

void COpBtn::Start(void)
{
	if (OPR.isEmg || OPR.isWrongParam)
	{
		NV.ndm[scrBtnStart] = 0;
		return;
	}

	BOOL isStartBtnOn = AOn(iSwStart) || AOn(iSwRrStart);
	if (AOn(iSwStop) || AOn(iSwReset) || AOn(iSwRrStop) || AOn(iSwRrReset))
		isStartBtnOn = (FALSE);

	m_tmBtnStart.Reset(!isStartBtnOn);

	BOOL isPressedStart = NV.ndm[scrBtnStart];
	NV.ndm[scrBtnStart] = 0;

	if (m_startBtnState != isStartBtnOn)
	{
		if (isStartBtnOn)
		{
			if (m_tmBtnStart.TmOver(10))
			{
				isPressedStart = TRUE;
				m_startBtnState = isStartBtnOn;
			}
		}
		else
		{
			m_startBtnState = isStartBtnOn;
		}
	}

	if (isPressedStart)
	{
		NV.use[useMasterKeyMode] = 0;
		NV.ndm[jobEnable] = 0;

		if (!OPR.isCheckErr)
			return;

		if (0 < ER.GetNo() && ER.GetNo() < ER_MAX)
			return;

		if (!OPR.isAllHome || !OPR.isStop || OPR.isPaused || OPR.isDebugMode)
			return;

		if (ST_LOT_IDLE == NV.ndm[lotState])
		{
			WR.Save(WR_LOT_IDLE);
			if (NV.use[useLotInput])
			{
				ER.Save(ER_LOT_NO_REGISTRATION);
				return;
			}
		}

		StartProcess();
	}
}

void COpBtn::Reset(void)
{
	if (!WMX.isConnected)
	{
		NV.ndm[scrBtnReset] = 0;
		return;
	}

	BOOL isResetBtnOn = AOn(iSwReset) || AOn(iSwRrReset);
	m_tmBtnReset.Reset(!isResetBtnOn);

	BOOL isPressedReset = NV.ndm[scrBtnReset];
	isPressedReset |= LASERIO.AOn(iLaserReset);
	NV.ndm[scrBtnReset] = 0;

	if (m_resetBtnState != isResetBtnOn)
	{
		if (isResetBtnOn)
		{
			if (m_tmBtnReset.TmOver(10))
			{
				isPressedReset = TRUE;
				m_resetBtnState = isResetBtnOn;
			}
		}
		else
		{
			m_resetBtnState = isResetBtnOn;
		}
	}

	if (isPressedReset)
	{
		if (!LAMPBUZZER.m_shouldBuzzerOff)
		{
			LAMPBUZZER.BuzzerOff();
			return;
		}

		NV.ndm[scrBtnReset] = 0;
		NV.ndm[jobEnable] = 0;

		OPR.isWrongPkg = (FALSE);
		OPR.isWrongSys = (FALSE);
		OPR.isWrongRcp = (FALSE);

		for (int i = 0; i < cylinderMAX; i++)
		{
			CylinderList cyno = CylinderList(i);
			CYL[cyno].Reset();
		}
		for (int i = 0; i < vacuumMAX; i++)
		{
			VacuumList vcno = VacuumList(i);
			VC[vcno].Reset();
		}
		for (int i = 0; i < laserMAX; i++)
		{
			LaserList lano = LaserList(i);
			if (LAS[lano].m_isWaitData)
				continue;

			LAS[lano].m_isErrorMode = false;
			LAS[lano].m_isErrorReady = false;
			LAS[lano].m_isErrorBusy = false;
			LAS[lano].m_isErrorCoordDataWait = false;
			LAS[lano].m_isErrorStop = false;
			LAS[lano].m_isErrorAuto = false;
			LAS[lano].Reset();
		}

		if (!OPR.isEmg)
		{
			for (int i = 0; i < motorMAX; i++)
			{
				AxisList mtno = AxisList(i);

				if (MT[mtno].isVirtualSW || MT[mtno].isVirtualHW) continue;

				if (MT[mtno].m_state.isAlarm)
				{
					MT[mtno].ServoOff();
					MT[mtno].AlarmClear();
				}
				else if (!MT[mtno].m_state.isServoOn)
				{
					MT[mtno].ServoOn();
				}
			}
		}

		ResetProcess();

		OPR.isCheckErr = (FALSE);
		ER.Clear();
		WR.Clear();

		LOG[logSEQ].Message("Pressed Reset Button");
	}
}

void COpBtn::Break(void)
{
	if (((AOn(iSwStop, 1000) && AOn(iSwReset, 1000)) || (AOn(iSwRrStop, 1000) && AOn(iSwRrReset, 1000))) && !m_pressedBreak)
	{
		MSGNOTICE.Set(MSG_OP_BREAK,
			"YES", "NO", NULL, NULL,
			"Cycle Stop and Motor Move Stop?");

		m_pressedBreak = 1;
	}
	else
	{
		if ((AOff(iSwStop, 1000) && AOff(iSwReset, 1000)) || (AOff(iSwRrStop, 1000) && AOff(iSwRrReset, 1000)))
			m_pressedBreak = 0;
	}
}

//COpBtn	OPBTN;