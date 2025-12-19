#include "..\Includes.h"

void CommonSequence(void)
{
	int inMcCnt = 0;

	for (int nvNo = 0; nvNo < nvTrayPartMax; nvNo++)
	{
		if (TRAY_PART_DATA[nvNo].exist) inMcCnt++;
	}

	if (inMcCnt)
	{
		tmMachineEmpty.Reset();
	}

	if (tmMachineEmpty.TmOver(5000))
	{
		bMachineEmpty = TRUE;
	}
	else
	{
		bMachineEmpty = FALSE;
	}

	//if (BOn(iLASER_LEAK_ALARM))
	//{
	//	ER.Save(ER_LASER_LEAK);
	//}

	if (NV.sys[SafetyLightCurtainAlarmTime] < 10000)
		NV.sys[SafetyLightCurtainAlarmTime] = 10000;

	if (BOn(iBFoamSafetyLightCurtain))
	{
		// TODO: AMR CYCLE 인터락 추가
		if (BOn(iBFoamSafetyLightCurtain, (int)NV.sys[SafetyLightCurtainAlarmTime]))
			ER.Save(ER_FOAM_SAFETY_LIGHT_CURTAIN);

		WR.Save(WR_FOAM_SAFETY_LIGHT_CURTAIN);
	}
	else
	{
		ER.Del(ER_FOAM_SAFETY_LIGHT_CURTAIN);
		WR.Del(WR_FOAM_SAFETY_LIGHT_CURTAIN);
	}

	if (BOn(iBMbbSafetyLightCurtain))
	{
		// TODO: AMR CYCLE 인터락 추가
		if (BOn(iBMbbSafetyLightCurtain, (int)NV.sys[SafetyLightCurtainAlarmTime]))
		{
			if (CYL[cylLtLabelFeederFb].IsBwd() && CYL[cylRtLabelFeederFb].IsBwd() &&
				CYL[cylLtMbbCstFb].IsBwd() && CYL[cylRtMbbCstFb].IsBwd())
				ER.Save(ER_MBB_SAFETY_LIGHT_CURTAIN);
		}

		WR.Save(WR_MBB_SAFETY_LIGHT_CURTAIN);
	}
	else
	{
		ER.Del(ER_MBB_SAFETY_LIGHT_CURTAIN);
		WR.Del(WR_MBB_SAFETY_LIGHT_CURTAIN);
	}

	//--- Check Life Time
	int rcpSet = 0, ddmCur = 0;
	int ddmReset = 0, ddmStart = 0;
	for (int i = 0; i < 30; i++)
	{
		rcpSet = setLifeTime01 + i;
		ddmCur = curLifeTime01 + i;
		ddmReset = resetLifeTime01 + i;
		ddmStart = startLifeTime01 + i;

		if (0.001 < NV.rcp[(RcpList)rcpSet])
		{
			if (NV.rcp[(RcpList)rcpSet] <= NV.ddm[(DDmList)ddmCur])
			{
				ER.Save((ErrName)(ER_LIFE_TIME_01_OVER + i));
			}
			else if ((NV.rcp[(RcpList)rcpSet] * 0.8) <= NV.ddm[(DDmList)ddmCur])
			{
				WR.Save((WrName)(WR_LIFE_TIME_01_OVER + i));
			}
		}
		if (NV.ddm[(DDmList)ddmReset])
		{
			NV.ddm[(DDmList)ddmCur] = 0;
			NV.ddm[(DDmList)ddmReset] = 0;
		}
	}

	//if (0) // 
	//{
	//	if (IsOn(oSOL_VAC_TACK_1_OFF))
	//	{
	//		if ((IsOff(oINDEX_1_VAC_OFF) && !IsOff(oINDEX_1_VAC_OFF, 100)) ||
	//			(IsOff(oINDEX_2_VAC_OFF) && !IsOff(oINDEX_2_VAC_OFF, 100)))
	//		{
	//			Off(oSOL_VAC_TACK_1_OFF);
	//		}
	//	}

	//	if (VC[vcVAC_TANK_1].AOn())
	//	{
	//		On(oSOL_VAC_TACK_1_OFF);
	//	}
	//	else if (VC[vcVAC_TANK_1].AOff())
	//	{
	//		Off(oSOL_VAC_TACK_1_OFF);
	//	}

	//	if (VC[vcVAC_TANK_2].AOn())
	//	{
	//		On(oSOL_VAC_TACK_2_OFF);
	//	}
	//	else if (VC[vcVAC_TANK_2].AOff())
	//	{
	//		Off(oSOL_VAC_TACK_2_OFF);
	//	}
	//}
	//else
	//{
	//	if (VC[vcVAC_TANK_1].AOff())
	//	{
	//		Off(oSOL_VAC_TACK_1_OFF);
	//	}
	//	if (VC[vcVAC_TANK_2].AOff())
	//	{
	//		Off(oSOL_VAC_TACK_2_OFF);
	//	}
	//}

	if (!Between(NV.ddm[sealingPressureRegulator], 0.0, 0.5))
		NV.ddm[sealingPressureRegulator] = 0.5;

	double dAirValue = (NV.ddm[sealingPressureRegulator] * 2 + 0.04) * WEIDMULLER_AD_S7;
	if (NV.ddm[sealingPressureRegulator] == 0)
		dAirValue = 0;
	Set(aoPressureRegulator, (int) dAirValue);

	if (!!NV.ndm[scrLight])
	{
		On(oIlluminator1);
		On(oIlluminator2);
	}
	else
	{
		Off(oIlluminator1);
		Off(oIlluminator2);
	}

	if (!NV.ndm[startSeq])
	{
		NV.ndm[startSeq] = 1;
		NV.ndm[scrLight] = 1;

		//VC[vcQc_INDEX_1].Off();
		//VC[vcQc_INDEX_2].Off();
		//VC[vcQc_INDEX_3].Off();
		//VC[vcQc_INDEX_4].Off();
		//VC[vcQc_INDEX_5].Off();
		//VC[vcQc_INDEX_6].Off();
		//!!NV.ndm[EXIST_LD_TRAY] ? VC[vcLD_TRAY].On() : VC[vcLD_TRAY].Off();
		//!!NV.ndm[EXIST_IN_PICK] ? VC[vcIN_PICK].On() : VC[vcIN_PICK].Off();
		//!!NV.ndm[EXIST_INDEX_1] ? VC[vcINDEX_1].On() : VC[vcINDEX_1].Off();
		//!!NV.ndm[EXIST_INDEX_2] ? VC[vcINDEX_2].On() : VC[vcINDEX_2].Off();
		//!!NV.ndm[EXIST_OUT_PICK] ? VC[vcOUT_PICK].On() : VC[vcOUT_PICK].Off();
		//!!NV.ndm[EXIST_OUT_PICK_SCRAP] ? VC[vcOUT_PICK_SCRAP].On() : VC[vcOUT_PICK_SCRAP].Off();
		//!!NV.ndm[EXIST_SORT_PICK] ? VC[vcSORT_PICK].On() : VC[vcSORT_PICK].Off();
		//!!NV.ndm[EXIST_ULD_TRAY] ? VC[vcULD_TRAY].On() : VC[vcULD_TRAY].Off();
		//!!NV.ndm[EXIST_QC_INDEX_1] ? QcINDEX.VacuumOn(Unit1) : QcINDEX.VacuumOff(Unit1);
		//!!NV.ndm[EXIST_QC_INDEX_2] ? QcINDEX.VacuumOn(Unit2) : QcINDEX.VacuumOff(Unit2);
		//!!NV.ndm[EXIST_QC_INDEX_3] ? QcINDEX.VacuumOn(Unit3) : QcINDEX.VacuumOff(Unit3);
		//!!NV.ndm[EXIST_QC_INDEX_4] ? QcINDEX.VacuumOn(Unit4) : QcINDEX.VacuumOff(Unit4);
		//!!NV.ndm[EXIST_QC_INDEX_5] ? QcINDEX.VacuumOn(Unit5) : QcINDEX.VacuumOff(Unit5);
		//!!NV.ndm[EXIST_QC_INDEX_6] ? QcINDEX.VacuumOn(Unit6) : QcINDEX.VacuumOff(Unit6);

		INDEX_LOCK = NOT_DEFINED;
		tmUphIdle.Reset();
	}
}

void UpdateForMMI(void)
{
	if (!OPR.isAuto || !bMachineEmpty)
	{
		tmRunDown.Reset();
	}

	if (OPR.isEmg || OPR.isWrongParam)
	{
		NV.ndm[scrMcState] = MC_STATE_ERROR;
	}
	else if (OPR.isStop)
	{
		if (OPR.isCycleRun)
			NV.ndm[scrMcState] = MC_STATE_CYCLE;
		else if (0 < ER.GetNo())
			NV.ndm[scrMcState] = MC_STATE_ERROR;
		else
			NV.ndm[scrMcState] = MC_STATE_STOP;
	}
	else if (OPR.isAuto)
	{
		if (tmRunDown.TmOver(60000))
		{
			NV.ndm[scrMcState] = MC_STATE_RUN_DOWN;
		}
		else
		{
			NV.ndm[scrMcState] = MC_STATE_RUN;
		}
	}
}

void AntiCollisionF(void)
{
	if (NV.use[useMasterKeyMode])
		return;

	for (int i = 0; i < motorMAX; i++)
	{
		AxisList mtno = AxisList(i);
		if (!MT[mtno].m_state.isHome || !MT[mtno].m_state.isDriving)
			continue;

		if ((MT[mtno].m_command.pos - MT[mtno].m_state.realCnt) > 0.)
		{
			//if (mtTRAY_PUSH_X == mtno)
			//{
			//	if (!MT[mtIN_PICK_X].m_state.isHome)
			//		continue;

			//	double anticollisionPos1 = MT[mtIN_PICK_X].PosTable(IN_PICK_X_INDEX_1) + 10.;
			//	if (MT[mtIN_PICK_X].m_state.realCnt > anticollisionPos1 ||
			//		MT[mtIN_PICK_X].m_command.pos > anticollisionPos1)
			//	{
			//		double anticollisionPos2 = MT[mtno].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
			//		if (MT[mtno].m_state.realCnt >= anticollisionPos2 ||
			//			MT[mtno].m_command.pos >= anticollisionPos2)
			//		{
			//			MT[mtno].Stop();
			//			MT[mtno].m_state.isHome = false;
			//		}
			//	}
			//}
			//else if (mtIN_PICK_X == mtno)
			//{
			//	if (!MT[mtTRAY_PUSH_X].m_state.isHome)
			//		continue;

			//	double anticollisionPos1 = MT[mtTRAY_PUSH_X].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
			//	if (MT[mtTRAY_PUSH_X].m_state.realCnt > anticollisionPos1 ||
			//		MT[mtTRAY_PUSH_X].m_command.pos > anticollisionPos1)
			//	{
			//		double anticollisionPos2 = MT[mtno].PosTable(IN_PICK_X_INDEX_1) + 10.;
			//		if (MT[mtno].m_state.realCnt >= anticollisionPos2 ||
			//			MT[mtno].m_command.pos >= anticollisionPos2)
			//		{
			//			MT[mtno].Stop();
			//			MT[mtno].m_state.isHome = false;
			//		}
			//	}
			//}
			//else if (mtOUT_PICK_X == mtno)
			//{
			//	if (!MT[mtSORT_PICK_Y].m_state.isHome)
			//		continue;

			//	double anticollisionPos1 = MT[mtSORT_PICK_Y].PosTable(SORT_PICK_Y_ANTICOLLISION) - 3.;
			//	if (MT[mtSORT_PICK_Y].m_state.realCnt < anticollisionPos1 ||
			//		MT[mtSORT_PICK_Y].m_command.pos < anticollisionPos1)
			//	{
			//		double anticollisionPos2 = MT[mtno].PosTable(OUT_PICK_X_INDEX_2) + 10.;
			//		if (MT[mtno].m_state.realCnt >= anticollisionPos2 ||
			//			MT[mtno].m_command.pos >= anticollisionPos2)
			//		{
			//			MT[mtno].Stop();
			//			MT[mtno].m_state.isHome = false;
			//		}
			//	}
			//}
		}
		else if ((MT[mtno].m_command.pos - MT[mtno].m_state.realCnt) < 0.)
		{
			//if (mtSORT_PICK_Y == mtno)
			//{
			//	if (!MT[mtOUT_PICK_X].m_state.isHome)
			//		continue;

			//	double anticollisionPos1 = MT[mtOUT_PICK_X].PosTable(OUT_PICK_X_INDEX_2) + 10.;
			//	if (MT[mtOUT_PICK_X].m_state.realCnt > anticollisionPos1 ||
			//		MT[mtOUT_PICK_X].m_command.pos > anticollisionPos1)
			//	{
			//		double anticollisionPos2 = MT[mtno].PosTable(SORT_PICK_Y_ANTICOLLISION);
			//		if (MT[mtno].m_state.realCnt <= anticollisionPos2 ||
			//			MT[mtno].m_command.pos <= anticollisionPos2)
			//		{
			//			MT[mtno].Stop();
			//			MT[mtno].m_state.isHome = false;
			//		}
			//	}
			//}
		}
	}
}

void AllCycleOff(void)
{
	for (int i = 0; i < fsmMAX; i++)
		FSM[FsmList(i)].Set(C_IDLE);

	OPR.isCheckPkg = (TRUE);
	OPR.isCheckSys = (TRUE);
	OPR.isCheckRcp = (TRUE);

	OPR.isStop = (TRUE);
	OPR.isAuto = (FALSE);

	for (int i = 0; i < motorMAX; i++)
	{
		AxisList mtno = AxisList(i);
		if (MT[mtno].isVirtualHW)
			continue;

		BOOL isGantrySlave = FALSE;
		for (int gtNo = 0; gtNo < gtMAX; gtNo++)
		{
			if (i == GT[gtNo].m_slave)
				isGantrySlave = TRUE;
		}
		if (isGantrySlave)
			continue;

		if (MT[mtno].m_state.isDriving || MT[mtno].m_state.isPaused || MT[mtno].m_fsmDrv.IsBusy())
			MT[mtno].CancelHomeSearch();
	}

	IN_PK.m_isRun = false;
	FOAM_RAIL.m_isRun = false;
	FOAM_ELEV[enFoamRecvElev].m_isRun = false;
	FOAM_ELEV[enFoamPartialElev].m_isRun = false;
	FOAM_ELEV[enFoamSendElev].m_isRun = false;
	FOAM_STACK_CONV.m_isRun = false;
	FOAM_PK.m_isRun = false;
	FOAM_CST[enFoamBoxLt].m_isRun = false;
	FOAM_CST[enFoamBoxRt].m_isRun = false;
	TRAY_SHUTTLE.m_isRun = false;
	MBB_SHUTTLE.m_isRun = false;
	MBB_QC.m_isRun = false;
	BTM_FOLD_CONV.m_isRun = false;
	FOLD_PK.m_isRun = false;
	LABEL_FEEDER[enlabelLt].m_isRun = false;
	LABEL_FEEDER[enlabelRt].m_isRun = false;
	LABEL_LASER.m_isRun = false;
	LABEL_SHUTTLE[enlabelLt].m_isRun = false;
	LABEL_SHUTTLE[enlabelRt].m_isRun = false;
	MBB_STACK[enMbbCstLt].m_isRun = false;
	MBB_STACK[enMbbCstRt].m_isRun = false;
	LT_COBOT.m_isRun = false;
	RT_COBOT.m_isRun = false;

	INDEX_LOCK = NOT_DEFINED;

	//LASER_INDEX[Index1].IsOneCycle() = FALSE;
	//LASER_INDEX[Index2].IsOneCycle() = FALSE;
	//INPICK.m_oneCycle = "";
}

void CalUPH(void)
{
	if (!Between(uphTable.index, 0, 359))
		uphTable.index = 0;

	if (tmUphReal.TmOverSec(10.))
	{
		tmUphReal.Reset();

		uphTable.val[uphTable.index] = uphTable.RelaCnt;
		uphTable.RelaCnt = 0;
		int uph_sum = 0;
		for (int i = 0; i < 360; i++)
		{
			uph_sum += uphTable.val[i];
		}
		uphTable.index++;
		NV.ddm[uphReal] = uph_sum;
	}
}

BOOL IsEMO(void)
{
	BOOL isEmergencyOff = FALSE;
	isEmergencyOff |= AOn(iTenkeyEmo);
	isEmergencyOff |= BOn(iBEmo01);
	isEmergencyOff |= BOn(iBEmo02);
	isEmergencyOff |= BOn(iBEmo03);
	isEmergencyOff |= BOn(iBEmo04);
	isEmergencyOff |= BOn(iBEmo05);
	isEmergencyOff |= BOn(iBEmo06);
	isEmergencyOff |= BOn(iBEmo07);
	isEmergencyOff |= BOn(iBEmo08);
	isEmergencyOff |= BOn(iBEmo09);

	return isEmergencyOff;
}

BOOL IsMotorPwr(void)
{
	BOOL isPowerOn = (TRUE);

	isPowerOn &= IsOn(oMotorPwr01);
	isPowerOn &= IsOn(oMotorPwr02);
	isPowerOn &= IsOn(oMotorPwr03);
	isPowerOn &= IsOn(oMotorPwr04);

	return isPowerOn;
}

void MotorPwr(BOOL bOn, int onStep)
{
	On(oMotorPwr01);
	On(oMotorPwr02);
	On(oMotorPwr03);
	On(oMotorPwr04);
}

BOOL IsDoorOpen(void)
{
	BOOL isDoorOpen = (FALSE);
	if (!NV.ndm[bypassDoorClose])
	{
		for (int nDoorNo = iDoorCloseBtm01; nDoorNo <= iDoorCloseBtm03; nDoorNo++)
			isDoorOpen |= AOff((enDI)nDoorNo);

		for (int nDoorNo = iDoorCloseBtm04; nDoorNo <= iDoorCloseBtm10; nDoorNo++)
			isDoorOpen |= AOff((enDI)nDoorNo);
	}

	return isDoorOpen;
}

BOOL IsDoorUnlock(void)
{
	BOOL isDoorUnlock = (FALSE);
	if (!NV.ndm[bypassDoorLock])
	{
		for (int nLockNo = iDoorLock01; nLockNo <= iDoorLock03; nLockNo++)
		{
			int oLockNo = oDoorLock01 + nLockNo - iDoorLock01;
			if (IsOff((enDO)oLockNo, 100))
			{
				if (AOff((enDI)nLockNo))
					isDoorUnlock |= (TRUE);
			}
		}

		for (int nLockNo = iDoorLock04; nLockNo <= iDoorLock06; nLockNo++)
		{
			int oLockNo = oDoorLock04 + nLockNo - iDoorLock04;
			if (IsOff((enDO)oLockNo, 100))
			{
				if (AOff((enDI)nLockNo))
					isDoorUnlock |= (TRUE);
			}
		}
	}

	return isDoorUnlock;
}

void DoorlockOn(void)
{
	for (int nLockNo = oDoorLock01; nLockNo <= oDoorLock03; nLockNo++)
	{
		if (!OPR.isDebugMode)
		{
			On((enDO)nLockNo);
		}
	}

	for (int nLockNo = oDoorLock04; nLockNo <= oDoorLock06; nLockNo++)
	{
		if (!OPR.isDebugMode)
		{
			On((enDO)nLockNo);
		}
	}
}

void DoorlockOff(void)
{
	for (int nLockNo = oDoorLock01; nLockNo <= oDoorLock03; nLockNo++)
	{
		if (!OPR.isDebugMode)
		{
			Off((enDO)nLockNo);
		}
	}

	for (int nLockNo = oDoorLock04; nLockNo <= oDoorLock06; nLockNo++)
	{
		if (!OPR.isDebugMode)
		{
			Off((enDO)nLockNo);
		}
	}
}

void AlarmDoorOpen(void)
{
	if (NV.ndm[bypassDoorClose])
		return;

	for (int nDoorNo = iDoorCloseBtm01; nDoorNo <= iDoorCloseBtm03; nDoorNo++)
	{
		ErrName code = (ErrName)(ER_DOOR_01 + nDoorNo - iDoorCloseBtm01);
		if (BOn((enDI)nDoorNo))
			ER.Save(code);
		else
			ER.Del(code);
	}

	for (int nDoorNo = iDoorCloseBtm04; nDoorNo <= iDoorCloseBtm10; nDoorNo++)
	{
		ErrName code = (ErrName)(ER_DOOR_04 + nDoorNo - iDoorCloseBtm04);
		if (BOn((enDI)nDoorNo))
			ER.Save(code);
		else
			ER.Del(code);
	}
}

void AlarmDoorLock(void)
{
	if (NV.ndm[bypassDoorLock])
		return;

	for (int nLockNo = iDoorLock01; nLockNo <= iDoorLock03; nLockNo++)
	{
		ErrName code = (ErrName)(ER_LOCK_01 + nLockNo - iDoorLock01);

		int oLockNo = oDoorLock01 + nLockNo - iDoorLock01;
		if (IsOff((enDO)oLockNo, 100))
		{
			if (AOff((enDI)nLockNo))
				ER.Save(code);
			else
				ER.Del(code);
		}
	}

	for (int nLockNo = iDoorLock04; nLockNo <= iDoorLock06; nLockNo++)
	{
		ErrName code = (ErrName)(ER_LOCK_04 + nLockNo - iDoorLock04);

		int oLockNo = oDoorLock04 + nLockNo - iDoorLock04;
		if (IsOff((enDO)oLockNo, 100))
		{
			if (AOff((enDI)nLockNo))
				ER.Save(code);
			else
				ER.Del(code);
		}
	}
}

void AlarmFire(void)
{
	for (int nFireNo = 0; nFireNo < nvFireDetectorMax; nFireNo++)
	{
		ErrName errCode = (ErrName)(ER_FIRE_STOP_01 + nFireNo);
		if (!FIRE_DETECTOR_DATA[nFireNo].isRun)
			ER.Save(errCode);
		else
			ER.Del(errCode);

		errCode = (ErrName)(ER_FIRE_SMOKE_01 + nFireNo);
		if (FIRE_DETECTOR_DATA[nFireNo].smokeAlarm)
			ER.Save(errCode);
		else
			ER.Del(errCode);

		WrName wrCode = (WrName)(WR_FIRE_SMOKE_01 + nFireNo);
		if (FIRE_DETECTOR_DATA[nFireNo].tempWarning)
			WR.Save(wrCode);
		else
			WR.Del(wrCode);

		errCode = (ErrName)(ER_FIRE_TEMP_01 + nFireNo);
		if (FIRE_DETECTOR_DATA[nFireNo].tempAlarm)
			ER.Save(errCode);
		else
			ER.Del(errCode);
	}
}

void AlarmFan(void)
{
	if (NV.use[useFanAlarmCheck])
	{
		int firstFanNo;
		for (int nFanNo = iFanAlarmTop01; nFanNo <= iFanAlarmTop08; nFanNo++)
		{
			firstFanNo = iFanAlarmTop01;
			if (AOn((enDI)nFanNo, 5000) || AOff((enDI)nFanNo, 5000))
				ER.Save((ErrName)(ER_FAN_TOP1_ALARM + nFanNo - firstFanNo));
			else
				ER.Del((ErrName)(ER_FAN_TOP1_ALARM + nFanNo - firstFanNo));
		}
		for (int nFanNo = iFanAlarmTop09; nFanNo <= iFanAlarmTop16; nFanNo++)
		{
			firstFanNo = iFanAlarmTop09;
			if (AOn((enDI)nFanNo, 5000) || AOff((enDI)nFanNo, 5000))
				ER.Save((ErrName)(ER_FAN_TOP9_ALARM + nFanNo - firstFanNo));
			else
				ER.Del((ErrName)(ER_FAN_TOP9_ALARM + nFanNo - firstFanNo));
		}
		for (int nFanNo = iFanAlarmTop17; nFanNo <= iFanAlarmTop24; nFanNo++)
		{
			firstFanNo = iFanAlarmTop17;
			if (AOn((enDI)nFanNo, 5000) || AOff((enDI)nFanNo, 5000))
				ER.Save((ErrName)(ER_FAN_TOP17_ALARM + nFanNo - firstFanNo));
			else
				ER.Del((ErrName)(ER_FAN_TOP17_ALARM + nFanNo - firstFanNo));
		}
		for (int nFanNo = iFanAlarmBtm01; nFanNo <= iFanAlarmBtm06; nFanNo++)
		{
			firstFanNo = iFanAlarmBtm01;
			if (AOn((enDI)nFanNo, 5000) || AOff((enDI)nFanNo, 5000))
				ER.Save((ErrName)(ER_FAN_BTM1_ALARM + nFanNo - firstFanNo));
			else
				ER.Del((ErrName)(ER_FAN_BTM1_ALARM + nFanNo - firstFanNo));
		}
	}

}

BOOL IsAnyCycleRun(void)
{
	OPR.isCycleRun = (FALSE);
	for (int i = 0; i < motorMAX; i++)
	{
		AxisList mtno = AxisList(i);
		if (MT[mtno].isVirtualSW || MT[mtno].isVirtualHW)
			continue;

		OPR.isCycleRun |= (MT[mtno].m_state.isDriving || MT[mtno].m_fsmDrv.IsBusy());
	}

	for (int i = 0; i < fsmMAX; i++)
		OPR.isCycleRun |= FSM[FsmList(i)].IsBusy();

	return OPR.isCycleRun;
}

BOOL IsWrongPkg(void)
{
	if (!OPR.isCheckPkg)
		return (FALSE);

	for (int i = 0; i < PKG_MAX; i++)
	{
		double dMin = 0.;
		double dMax = 0.;
		BOOL isUse = FALSE;
		PkgList PkgDataNo = (PkgList)i;

		switch (PkgDataNo)
		{
			case unitCnt:
			{
				//dMin = 1.0;
				//dMax = 6.0;
				//isUse = TRUE;
				break;
			}
		}
		
		if (isUse && !Between(NV.pkg[PkgDataNo], dMin, dMax))
		{
			if (!OPR.isWrongPkg)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"PACKAGE [%3d] : %s\nCur Value:[%.3f]\nMin Value:[%.3f]\nMax Value:[%.3f]",
					(int)PkgDataNo, PARAM_NAME.pkg[PkgDataNo].c_str(), NV.pkg[PkgDataNo], dMin, dMax);
			}
			OPR.isWrongPkg = (TRUE);
			i = PKG_MAX;
		}
	}

	if (OPR.isWrongPkg)
	{
		ER.Save(ER_PKG_DATA);
		return (TRUE);
	}
	//if (MotorPosCreate())
	//{
	//	//		ER.Save(ER_MOTOTR_POS_CREATE);
	//	//		return (TRUE);
	//}

	OPR.isCheckPkg = (FALSE);
	return (FALSE);
}

BOOL IsWrongSys(void)
{
	if (!OPR.isCheckSys)
		return (FALSE);

	for (int i = 0; i < maxSYS; i++)
	{
		double dMin = 0.;
		double dMax = 0.;
		BOOL isUse = FALSE;
		SysList SysDataNo = (SysList)i;

		switch (SysDataNo)
		{
		case machineSpeed:
		{
			dMin = 1.;
			dMax = 100.;
			isUse = TRUE;
		}
		break;
		}

		if (isUse && !Between(NV.sys[SysDataNo], dMin, dMax))
		{
			if (!OPR.isWrongSys)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"SYSTEM [%3d] : %s\nCur Value:[%.3f]\nMin Value:[%.3f]\nMax Value:[%.3f]",
					SysDataNo, PARAM_NAME.sys[SysDataNo].c_str(), NV.sys[SysDataNo], dMin, dMax);
			}
			OPR.isWrongSys = (TRUE);
		}
	}

	if (OPR.isWrongSys)
	{
		ER.Save(ER_SYS_DATA);
		return (TRUE);
	}

	OPR.isCheckSys = (FALSE);
	return (FALSE);
}

BOOL IsWrongRcp(void)
{
	if (!OPR.isCheckRcp)
		return (FALSE);

	for (int i = 0; i < maxRCP; i++)
	{
		double dMin = 0.;
		double dMax = 0.;
		BOOL isUse = FALSE;
		RcpList RcpDataNo = (RcpList)i;

		switch (RcpDataNo)
		{
			case trayMaxCount:
			{
				dMin = 1.0;
				dMax = 11.0;
				isUse = TRUE;
				break;
			}
			case sendElevLevelDnDelay:
			case recvElevLevelDnDelay:
			case partialElevLevelDnDelay:
			case sendElevLevelUpDelay:
			case recvElevLevelUpDelay:
			case partialElevLevelUpDelay:
			{
				dMin = 0.0;
				dMax = 10000.0;
				isUse = TRUE;
				break;
			}
			case foamPkLaserFoamBaseHeight:
			case foamPkLaserFoamWallHeight:
			case foamPkLaserPartialFoamBaseHeight:
			case foamPkLaserPartialFoamWallHeight:
			{
				dMin = 75.0;
				dMax = 130.0;
				isUse = TRUE;
				break;
			}
			default:
				break;
		}

		if (isUse && !Between(NV.rcp[RcpDataNo], dMin, dMax))
		{
			if (!OPR.isWrongRcp)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"RECIPE [%3d] : %s\nCur Value:[%.3f]\nMin Value:[%.3f]\nMax Value:[%.3f]",
					RcpDataNo, PARAM_NAME.rcp[RcpDataNo].c_str(), NV.rcp[RcpDataNo], dMin, dMax);
			}
			OPR.isWrongRcp = (TRUE);
		}
	}

	if (OPR.isWrongRcp)
	{
		ER.Save(ER_RCP_DATA);
		return true;
	}

	//if (MotorPosCreate())
	//{
	//	ER.Save(ER_MOTOTR_POS_CREATE);
	//	return (TRUE);
	//}

	OPR.isCheckRcp = (FALSE);
	return (FALSE);
}

void SetMesureTime(int no)
{
	if (!Between(no, 0, MAX_MESURETIME_CNT))
		return;

	if (MESURETIME[no].isStarted)
		return;

	MESURETIME[no].isStarted = 1;
	MESURETIME[no].setTime = (DWORD)MESURETIME[no].timer.Elapsed();
}

void GetMesureTime(int no)
{
	if (!Between(no, 0, MAX_MESURETIME_CNT))
		return;

	if (!MESURETIME[no].isStarted)
		return;

	MESURETIME[no].isStarted = 0;
	MESURETIME[no].getTime = (DWORD)(MESURETIME[no].timer.Elapsed() - MESURETIME[no].setTime);
}

BOOL Align2Point(ALIGN_POINT_DATA data, POINT2D coord, XYT& offset)
{
	// ------------(P2)-
	// |			   |
	// |			   |
	// -(P1)------------
	return kasiolib::OffsetFrom2Point(&data, &coord, &offset);
}

BOOL Align3Point(ALIGN_POINT_DATA data, POINT2D coord, XYT& offset)
{
	// -(P2)------------
	// |			   |
	// |			   |
	// -(P1)-------(P3)-
	return kasiolib::OffsetFrom3Point(&data, &coord, &offset);
}

BOOL Align4Point(ALIGN_POINT_DATA data, POINT2D coord, XYT& offset)
{
	// -(P2)-------(P4)-
	// |			   |
	// |			   |
	// -(P1)-------(P3)-
	return kasiolib::OffsetFrom4Point(&data, &coord, &offset);
}

int MtPosMove(AxisList mtno, int posIdx)
{
	if (!Between(mtno, 0, motorMAX - 1))
		return (MT_NOTHING);
	if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		return (MT_NOTHING);

	if (MT[mtno].IsRdy())
	{
		if (MT[mtno].InCmd(posIdx) && MT[mtno].InPos(posIdx))
			return (MT_INPOS);
		else if (!MT[mtno].InCmd(posIdx))
		{
			BOOL isCanMove = MT[mtno].CanMove(posIdx);
			double pos = MT[mtno].PosTable(posIdx);
			if (MT[mtno].m_state.cmdCnt < pos)
			{
			
			}
			else if (MT[mtno].m_state.cmdCnt > pos)
			{
				
			}

			if (isCanMove)
			{
				MT[mtno].PosMove(posIdx);
				return (MT_MOVE);
			}
		}
	}
	return (MT_NOTHING);
}

int MtPosMove(AxisList mtno, int posIdx, double pos, double vel, double acc)
{
	if (MT[mtno].IsRdy())
	{
		if (MT[mtno].InCmd(posIdx, pos) && MT[mtno].InPos(posIdx, pos, MT[mtno].m_inposBand))
			return (MT_INPOS);
		else if (!MT[mtno].InCmd(posIdx, pos))
		{
			BOOL isCanMove = MT[mtno].CanMove(posIdx);
			if (MT[mtno].m_state.cmdCnt < pos)
			{
			
			}
			else if (MT[mtno].m_state.cmdCnt > pos)
			{
				
			}

			if (isCanMove)
			{
				MT[mtno].PosMove(posIdx, pos, vel, acc);
				return (MT_MOVE);
			}
		}
	}
	return (MT_NOTHING);
}

void AntiCollisionJogFwd(AxisList nMtNo, double vel, double acc)
{
	//if (mtTRAY_PUSH_X == nMtNo)
	//{
	//	double anticollisionPos = MT[mtIN_PICK_X].PosTable(IN_PICK_X_INDEX_1) + 10.;
	//	if (MT[mtIN_PICK_X].m_state.isHome &&
	//		(MT[mtIN_PICK_X].m_state.realCnt <= anticollisionPos) &&
	//		(MT[mtIN_PICK_X].m_command.pos <= anticollisionPos))
	//	{
	//		MT[nMtNo].JogFwd(vel, acc);
	//	}
	//	else
	//	{
	//		double targetPos = MT[nMtNo].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
	//		if (MT[nMtNo].m_state.realCnt < targetPos)
	//			MT[nMtNo].PosMove(P_JOG, targetPos, fabs(vel), acc);
	//	}
	//}
	//else if (mtIN_PICK_X == nMtNo)
	//{
	//	double anticollisionPos = MT[mtTRAY_PUSH_X].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
	//	if (MT[mtTRAY_PUSH_X].m_state.isHome &&
	//		(MT[mtTRAY_PUSH_X].m_state.realCnt <= anticollisionPos) &&
	//		(MT[mtTRAY_PUSH_X].m_command.pos <= anticollisionPos))
	//	{
	//		MT[nMtNo].JogFwd(vel, acc);
	//	}
	//	else
	//	{
	//		double targetPos = MT[nMtNo].PosTable(IN_PICK_X_INDEX_1) + 10.;
	//		if (MT[nMtNo].m_state.realCnt < targetPos)
	//			MT[nMtNo].PosMove(P_JOG, targetPos, fabs(vel), acc);
	//	}
	//}
	//else if (mtOUT_PICK_X == nMtNo)
	//{
	//	double anticollisionPos = MT[mtSORT_PICK_Y].PosTable(SORT_PICK_Y_ANTICOLLISION) - 3.;
	//	if (MT[mtSORT_PICK_Y].m_state.isHome &&
	//		(MT[mtSORT_PICK_Y].m_state.realCnt >= anticollisionPos) &&
	//		(MT[mtSORT_PICK_Y].m_command.pos >= anticollisionPos))
	//	{
	//		MT[nMtNo].JogFwd(vel, acc);
	//	}
	//	else
	//	{
	//		double targetPos = MT[nMtNo].PosTable(OUT_PICK_X_INDEX_2) + 10.;
	//		if (MT[nMtNo].m_state.realCnt < targetPos)
	//			MT[nMtNo].PosMove(P_JOG, targetPos, fabs(vel), acc);
	//	}
	//}
	//else if (mtSORT_PICK_Y == nMtNo)
	//{
	//	ANTICOLLISION[acSORT_ULDTRAY].Axis1JogFwd(vel, acc);
	//}
	//else if (mtULD_TRAY_Y == nMtNo)
	//{
	//	ANTICOLLISION[acSORT_ULDTRAY].Axis2JogFwd(vel, acc);
	//}
	//else
	//{
	//	MT[nMtNo].JogFwd(vel, acc);
	//}
	MT[nMtNo].JogFwd(vel, acc);
}

void AntiCollisionJogBwd(AxisList nMtNo, double vel, double acc)
{
	//if (mtSORT_PICK_Y == nMtNo)
	//{
	//	double anticollisionPos = MT[mtOUT_PICK_X].PosTable(OUT_PICK_X_INDEX_2) + 10.;
	//	if (MT[mtOUT_PICK_X].m_state.isHome &&
	//		(MT[mtOUT_PICK_X].m_state.realCnt <= anticollisionPos) &&
	//		(MT[mtOUT_PICK_X].m_command.pos <= anticollisionPos))
	//	{
	//		MT[nMtNo].JogBwd(vel, acc);
	//	}
	//	else
	//	{
	//		double targetPos = MT[nMtNo].PosTable(SORT_PICK_Y_ANTICOLLISION);
	//		if (MT[nMtNo].m_state.realCnt > targetPos)
	//			MT[nMtNo].PosMove(P_JOG, targetPos, fabs(vel), acc);
	//	}
	//}
	//else
	{
		MT[nMtNo].JogBwd(vel, acc);
	}
}

void AntiCollisionRMove(AxisList nMtNo, double cmdDist, double pos, double vel, double acc)
{
	BOOL isCanMove = TRUE;
	if (MT[nMtNo].m_state.cmdCnt < pos)
	{
		//if (mtTRAY_PUSH_X == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtIN_PICK_X].PosTable(IN_PICK_X_INDEX_1) + 10.;
		//	if (!MT[mtIN_PICK_X].m_state.isHome ||
		//		MT[mtIN_PICK_X].m_state.realCnt > anticollisionPos1 ||
		//		MT[mtIN_PICK_X].m_command.pos > anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
		//		isCanMove &= (pos < anticollisionPos2);
		//	}
		//}
		//else if (mtIN_PICK_X == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtTRAY_PUSH_X].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
		//	if (!MT[mtTRAY_PUSH_X].m_state.isHome ||
		//		MT[mtTRAY_PUSH_X].m_state.realCnt > anticollisionPos1 ||
		//		MT[mtTRAY_PUSH_X].m_command.pos > anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(IN_PICK_X_INDEX_1) + 10.;
		//		isCanMove &= (pos < anticollisionPos2);
		//	}
		//}
		//else if (mtOUT_PICK_X == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtSORT_PICK_Y].PosTable(SORT_PICK_Y_ANTICOLLISION) - 3.;
		//	if (!MT[mtSORT_PICK_Y].m_state.isHome ||
		//		MT[mtSORT_PICK_Y].m_state.realCnt < anticollisionPos1 ||
		//		MT[mtSORT_PICK_Y].m_command.pos < anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(OUT_PICK_X_INDEX_2) + 10.;
		//		isCanMove &= (pos < anticollisionPos2);
		//	}
		//}
		//else if (mtSORT_PICK_Y == nMtNo)
		//{
		//	isCanMove &= ANTICOLLISION[acSORT_ULDTRAY].Axis1CanMove(pos);
		//}
		//else if (mtULD_TRAY_Y == nMtNo)
		//{
		//	isCanMove &= ANTICOLLISION[acSORT_ULDTRAY].Axis2CanMove(pos);
		//}
	}
	else if (MT[nMtNo].m_state.cmdCnt > pos)
	{
		//if (mtSORT_PICK_Y == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtOUT_PICK_X].PosTable(OUT_PICK_X_INDEX_2) + 10.;
		//	if (!MT[mtOUT_PICK_X].m_state.isHome ||
		//		MT[mtOUT_PICK_X].m_state.realCnt > anticollisionPos1 ||
		//		MT[mtOUT_PICK_X].m_command.pos > anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(SORT_PICK_Y_ANTICOLLISION);
		//		isCanMove &= (pos > anticollisionPos2);
		//	}
		//}
	}
	if (isCanMove)
	{
		MT[nMtNo].PosMove(P_JOG, pos, vel, acc);
	}
}

void AntiCollisionMove(AxisList nMtNo, int nIndex)
{
	BOOL isCanMove = TRUE;
	double pos = MT[nMtNo].PosTable(nIndex);
	if (MT[nMtNo].m_state.cmdCnt < pos)
	{
		//if (mtTRAY_PUSH_X == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtIN_PICK_X].PosTable(IN_PICK_X_INDEX_1) + 10.;
		//	if (!MT[mtIN_PICK_X].m_state.isHome ||
		//		MT[mtIN_PICK_X].m_state.realCnt > anticollisionPos1 ||
		//		MT[mtIN_PICK_X].m_command.pos > anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
		//		isCanMove &= (pos < anticollisionPos2);
		//	}
		//}
		//else if (mtIN_PICK_X == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtTRAY_PUSH_X].PosTable(LD_TRAY_Y_OUT_TRAY) + 3.;
		//	if (!MT[mtTRAY_PUSH_X].m_state.isHome ||
		//		MT[mtTRAY_PUSH_X].m_state.realCnt > anticollisionPos1 ||
		//		MT[mtTRAY_PUSH_X].m_command.pos > anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(IN_PICK_X_INDEX_1) + 10.;
		//		isCanMove &= (pos < anticollisionPos2);
		//	}
		//}
		//else if (mtOUT_PICK_X == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtSORT_PICK_Y].PosTable(SORT_PICK_Y_ANTICOLLISION) - 3.;
		//	if (!MT[mtSORT_PICK_Y].m_state.isHome ||
		//		MT[mtSORT_PICK_Y].m_state.realCnt < anticollisionPos1 ||
		//		MT[mtSORT_PICK_Y].m_command.pos < anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(OUT_PICK_X_INDEX_2) + 10.;
		//		isCanMove &= (pos < anticollisionPos2);
		//	}
		//}
		//else if (mtSORT_PICK_Y == nMtNo)
		//{
		//	isCanMove &= ANTICOLLISION[acSORT_ULDTRAY].Axis1CanMove(pos);
		//}
		//else if (mtULD_TRAY_Y == nMtNo)
		//{
		//	isCanMove &= ANTICOLLISION[acSORT_ULDTRAY].Axis2CanMove(pos);
		//}
	}
	else if (MT[nMtNo].m_state.cmdCnt > pos)
	{
		//if (mtSORT_PICK_Y == nMtNo)
		//{
		//	double anticollisionPos1 = MT[mtOUT_PICK_X].PosTable(OUT_PICK_X_INDEX_2) + 10.;
		//	if (!MT[mtOUT_PICK_X].m_state.isHome ||
		//		MT[mtOUT_PICK_X].m_state.realCnt > anticollisionPos1 ||
		//		MT[mtOUT_PICK_X].m_command.pos > anticollisionPos1)
		//	{
		//		double anticollisionPos2 = MT[nMtNo].PosTable(SORT_PICK_Y_ANTICOLLISION);
		//		isCanMove &= (pos > anticollisionPos2);
		//	}
		//}
	}
	if (isCanMove)
	{
		MT[nMtNo].PosMove(nIndex);
	}
}

void JustBeforePkgValueChanges(int nNo)
{

}

void JustBeforeNdmValueChanges(int nNo)
{

}

void JustBeforeDdmValueChanges(int nNo)
{

}

void JustBeforeUseValueChanges(int nNo)
{

}

void JustBeforeSysValueChanges(int nNo)
{

}

void JustBeforeRcpValueChanges(int nNo)
{

}