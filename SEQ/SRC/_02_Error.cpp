#include "..\Includes.h"

BOOL ErrorProcess(void)
{
	if (OPR.isDebugMode)		ER.Save(ER_DEBUG);
	else						ER.Del(ER_DEBUG);

	if (IsEMO())				ER.Save(ER_EMG);
	else						ER.Del(ER_EMG);

	if (!WMX.isConnected)		ER.Save(ER_WMX_COMM);
	else						ER.Del(ER_WMX_COMM);

	if (OPR.isDryRun)			WR.Save(WR_DRY_RUN);
	else						WR.Del(WR_DRY_RUN);

	if (OPR.isAuto)
		NV.use[useMasterKeyMode] = 0;

	OPR.isEmg = (IsEMO() || !WMX.isConnected);
	OPR.isWrongParam = (IsWrongPkg() || IsWrongSys() || IsWrongRcp());	// TODO: Setting need

	LASERIO.Set(oLaserEmo, !OPR.isEmg);
	if (OPR.isEmg && !OPR.isEmgMem)
	{
		for (int mtno = 0; mtno < motorMAX; mtno++)
			MT[AxisList(mtno)].CancelHomeSearch();

		for (int i = 0; i < fsmMAX; i++)
			FSM[FsmList(i)].Set(C_IDLE);

		WMX.OutDataClear();

		OPR.isCheckPkg = (TRUE);
		OPR.isCheckSys = (TRUE);
		OPR.isCheckRcp = (TRUE);

		OPR.isStop = (TRUE);
		OPR.isAuto = (FALSE);
	}

	OPR.isEmgMem = OPR.isEmg;

	// Error ETC Part Start
	//if (NV.ndm[DeviceNxtJobNo])//?
	//{
	//	NV.ndm[DeviceNxtJobNo] = 0;
	//	for (int mtno = 0; mtno < mMAX; mtno++)
	//		MT[mtno].CancelHomeSearch();

	//	NV.ndm[SCR_ALLHOME_COMP] = 0;
	//}
	// Error ETC Part End

	// Error Motor Part Start
	BOOL isActuatorsRun = (FALSE);
	BOOL isActuatorsHome = (FALSE);
	BOOL isActuatorsHoming = (FALSE);
	BOOL isAllHomeAlarm = (TRUE);
	OPR.isPaused = (FALSE);
	OPR.isAllHome = (TRUE);
	for (int i = 0; i < motorMAX; i++)
	{
		AxisList mtno = AxisList(i);
		if (MT[mtno].isVirtualSW || MT[mtno].isVirtualHW)
			continue;

		if (!OPR.isEmg)
		{
			if (MT[mtno].m_state.isAlarm)
				ER.Save((ErrName)(ER_MT_ALARM + mtno + 1));

			if (!MT[mtno].m_state.isServoOn)
				ER.Save((ErrName)(ER_MT_SERVO_OFF + mtno + 1));
		}

		if (MT[mtno].m_state.isHome)
		{
			if (MT[mtno].m_state.isHwCwSen || MT[mtno].m_state.isHwCCwSen)
			{
				for (int gtNo = 0; gtNo < gtMAX; gtNo++)
				{
					if (mtno == GT[gtNo].m_slave)
						MT[GT[gtNo].m_master].CancelHomeSearch();
				}
				
				MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "axis hardware limit is detected");
				MT[mtno].CancelHomeSearch();
				ER.Save((ErrName)(ER_MT_HW_LIMIT + mtno + 1));
			}

			if (MT[mtno].m_state.isSwCwSen || MT[mtno].m_state.isSwCCwSen)
			{
				MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "axis software limit is detected");
				MT[mtno].CancelHomeSearch();
				ER.Save((ErrName)(ER_MT_SW_LIMIT + mtno + 1));
			}
		}

		isActuatorsRun |= MT[mtno].m_state.isDriving;
		isActuatorsRun |= MT[mtno].m_fsmDrv.IsBusy();
		isActuatorsHome |= MT[mtno].m_state.isHome;
		isActuatorsHoming |= MT[mtno].m_fsmDrv.Between(C_HOME_START, C_HOME_END);

		OPR.isPaused |= MT[mtno].m_state.isPaused;
		OPR.isAllHome &= MT[mtno].m_state.isHome;
		isAllHomeAlarm &= !MT[mtno].m_state.isHome;

		if (MT[mtno].m_state.isPaused)
			WR.Save((WrName)(WR_MT_PAUSE + mtno + 1));
		else
			WR.Del((WrName)(WR_MT_PAUSE + mtno + 1));
	}

	for (int i = 0; i < robotMAX; i++)
	{
		RobotList mtno = RobotList(i);

		isActuatorsRun |= ROBOT[mtno].m_robotState.moving;
		isActuatorsRun |= ROBOT[mtno].m_fsmRobot.IsBusy();
		if (ROBOT[mtno].m_isHome)
			isActuatorsHome = TRUE;
		isActuatorsHoming |= ROBOT[mtno].m_fsmRobot.Between(C_HOME_START, C_HOME_END);

		OPR.isPaused |= ROBOT[mtno].m_robotState.pauseState;
		if (!ROBOT[mtno].m_isHome)
			OPR.isAllHome = FALSE;
		isAllHomeAlarm &= !ROBOT[mtno].m_isHome;
	}

	isActuatorsHoming |= FSM[fsmSEQ].Between(C_ALLHOME_START, C_ALLHOME_END);		// ADD 25.07.04 
	isAllHomeAlarm |= FSM[fsmSEQ].Between(C_ALLHOME_START, C_ALLHOME_END);			// ADD 25.07.25
	if (isAllHomeAlarm)
		ER.Save(ER_MT_ALL_HOME);
	else
	{
		ER.Del(ER_MT_ALL_HOME);
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].isVirtualSW || MT[mtno].isVirtualHW)
				continue;

			BOOL isGantrySlave = FALSE;
			for (int gtNo = 0; gtNo < gtMAX; gtNo++)
			{
				if (i == GT[gtNo].m_slave)
					isGantrySlave = TRUE;
			}
			if (isGantrySlave)
				continue;

			if (MT[mtno].m_state.isHome)
				ER.Del((ErrName)(ER_MT_ALL_HOME + mtno + 1));
			else
				ER.Save((ErrName)(ER_MT_ALL_HOME + mtno + 1));
		}
	}

	for (int i = 0; i < robotMAX; i++)
	{
		RobotList mtno = RobotList(i);
		if (ROBOT[mtno].m_isHome)
			ER.Del((ErrName)(ER_ROBOT_HOME + mtno + 1));
		else
			ER.Save((ErrName)(ER_ROBOT_HOME + mtno + 1));
	}

	OPR.isAllHome &= !FSM[fsmSEQ].Between(C_ALLHOME_START, C_ALLHOME_END);
	NV.ndm[scrAllHomeComp] = isActuatorsHoming ? 2 : !!OPR.isAllHome;	// Motor Homing
	// Error Motor Part End

	// Error Pneumatic Part Start
	for (int i = 0; i < cylinderMAX; i++)
	{
		CylinderList cyno = CylinderList(i);
		if (CYL[cyno].m_isError)
			ER.Save((ErrName)(ER_CYL_ALARM + cyno + 1));
		else
			ER.Del((ErrName)(ER_CYL_ALARM + cyno + 1));
	}

	for (int i = 0; i < vacuumMAX; i++)
	{
		VacuumList vcno = VacuumList(i);
		if (VC[vcno].m_isError)
			ER.Save((ErrName)(ER_VAC_ALARM + vcno + 1));
		else
			ER.Del((ErrName)(ER_VAC_ALARM + vcno + 1));
	}
	// Error Pneumatic Part End

	//for (int cyno = 0; cyno < cylinderMAX; cyno++)
	//	ER.Save((ErrName)CYL[cyno].GetErr());

	//int mainAirError = VC[vcMAIN_AIR].GetErr();
	//if (0 < mainAirError)
	//	ER.Save((ErrName)mainAirError);

	//for (int vcno = 0; vcno < vacuumMAX; vcno++)
	//{
	//	if ((vcno == vcVAC_TANK_1) || (vcno == vcVAC_TANK_2)) continue;

	//	ER.Save((ErrName)VC[vcno].GetErr());
	//}
	

	// Error Door Part Start
	OPR.isDoorOpen = IsDoorOpen();
	OPR.isDoorUnlock = IsDoorUnlock();

	if (OPR.isDoorUnlock /*|| OPR.isDoorOpen*/)
	{
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].isVirtualHW) continue;

			BOOL isGantrySlave = FALSE;
			for (int gtNo = 0; gtNo < gtMAX; gtNo++)
			{
				if (i == GT[gtNo].m_slave)
					isGantrySlave = TRUE;
			}
			if (isGantrySlave)
				continue;

			BOOL shouldStop = FALSE;
			shouldStop = !MT[mtno].m_state.isPaused;
			shouldStop &= !NV.ndm[jobEnable];

			if (shouldStop)
				MT[mtno].Paused();
		}

		if (FSM[fsmSEQ].IsBusy())
			FSM[fsmSEQ].Set(C_ERROR);
	}

	OPR.isCycleRun = isActuatorsHoming;
	for (int i = 0; i < fsmMAX; i++)
		OPR.isCycleRun |= FSM[FsmList(i)].IsBusy();

	int nMaster, nSlave;
	BOOL bCanSync = FALSE;
	for (int gtNo = 0; gtNo < gtMAX; gtNo++)
	{
		nMaster = GT[gtNo].m_master;
		nSlave = GT[gtNo].m_slave;

		if (MT[nMaster].m_state.isServoOn && !MT[nSlave].m_state.isServoOn)
			MT[nSlave].ServoOn();
		if (!MT[nMaster].m_state.isServoOn && MT[nSlave].m_state.isServoOn)
			MT[nSlave].ServoOff();

		bCanSync = (MT[nMaster].m_state.isServoOn && MT[nSlave].m_state.isServoOn);
		bCanSync &= (!MT[nMaster].m_fsmDrv.IsBusy() && !MT[nSlave].m_fsmDrv.IsBusy());
		//bCanSync &= (!MTHome.m_fsm[nMaster].IsBusy());

		if (bCanSync)
		{
			if (OperationState::Sync != WMX.status.axesStatus[nSlave].opState)
			{
				if (!ER.Find((ErrName)(ER_GANTRY_SYNC_ALARM + gtNo)))
					LOG[logSEQ].Message("Slave status[{:%d}]", WMX.status.axesStatus[nSlave].opState);

				if (GT[gtNo].Set())
					ER.Save((ErrName)(ER_GANTRY_SYNC_ALARM + gtNo));

				Sleep(50);
			}
		}
	}

	BOOL isDoorLock = (!OPR.isStop || OPR.isCycleRun || isActuatorsRun);
	isDoorLock &= !NV.ndm[bypassDoorLock];

	if (isDoorLock)
		DoorlockOn();
	else
		DoorlockOff();

	AlarmDoorOpen();
	AlarmDoorLock();
	// Error Door Part End
	AlarmFire();
	AlarmFan();

	int nErrCode = ER.GetNo();

	OPR.isCheckErr = (TRUE);
	if (0 != nErrCode)
	{
		OPR.isStop = (TRUE);
		OPR.isAuto = (FALSE);
	}

	if (OPR.isEmg || OPR.isWrongParam)
	{
		if (FSM[fsmSEQ].IsBusy() && !FSM[fsmSEQ].IsBusy(C_ERROR))
			FSM[fsmSEQ].Set(C_ERROR);

		return (TRUE);
	}

	return (FALSE);
}