#include "..\..\Includes.h"

void DustCollector::Auto(void)
{
	Common();
	LaserLamp();
	if (!m_isRun)
	{
		if (IsOn(oRunNo1, 10000))
			Off(oRunNo1);
		if (IsOn(oRunNo2, 10000))
			Off(oRunNo2);

		return;
	}

	m_isRun = FALSE;

	if (IsHwErr())
		return;

	On(oRunNo1);
	On(oRunNo2);
}

void DustCollector::Cycle(void)
{
	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		LOG[logSEQ].Message("<%s> C_ERROR", m_strName);
		m_pFsm->Set(C_IDLE);
		return;
	}
}

void DustCollector::Init(void)
{
	strcpy(m_strName, "DustCollector");
	m_pFsm = &FSM[fsmDust];
	//cDampIndex1 = cINDEX_DAMP_1;
	//cDampIndex2 = cINDEX_DAMP_2;
	//cDampTonado1 = cTONADO_DAMP_1;
	//cDampTonado2 = cTONADO_DAMP_2;
	//iRunNo1 = iDUST_RUN_1;
	//oRunNo1 = oDUST_RUN_1;
	//iRunNo2 = iDUST_RUN_2;
	//oRunNo2 = oDUST_RUN_2;
	m_pFsm->Set(C_IDLE);
}

void DustCollector::LaserLamp(void)
{
	BOOL LampNeedOn = FALSE;
	//LampNeedOn |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_LASER_START, Stage::C_LASER_END);
	//LampNeedOn |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_STAGE_CAL_START, Stage::C_STAGE_CAL_MARKING);
	//LampNeedOn |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_SCAN_CAL_START, Stage::C_SCAN_CAL_MARKING);
	//LampNeedOn |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_CROSS_HAIR_START, Stage::C_CROSS_HAIR_MARKING);
	//LampNeedOn |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_PWR_METER_START, Stage::C_PWR_METER_END);
	//LampNeedOn |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_LASER_START, Stage::C_LASER_END);
	//LampNeedOn |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_STAGE_CAL_START, Stage::C_STAGE_CAL_MARKING);
	//LampNeedOn |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_SCAN_CAL_START, Stage::C_SCAN_CAL_MARKING);
	//LampNeedOn |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_CROSS_HAIR_START, Stage::C_CROSS_HAIR_MARKING);
	//LampNeedOn |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_PWR_METER_START, Stage::C_PWR_METER_END);

	//if (LampNeedOn)
	//{
	//	if (IsOn(O_TOWER_LAMP_H, 1000))
	//		Off(O_TOWER_LAMP_H);
	//	else if (IsOff(O_TOWER_LAMP_H, 1000))
	//		On(O_TOWER_LAMP_H);
	//}
	//else
	//{
	//	Off(O_TOWER_LAMP_H);
	//}
}

void DustCollector::Common(void)
{
	m_isRun = FALSE;
	
	//m_isRun |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_LASER_START, Stage::C_LASER_END);
	//m_isRun |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_STAGE_CAL_START, Stage::C_STAGE_CAL_MARKING);
	//m_isRun |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_SCAN_CAL_START, Stage::C_SCAN_CAL_MARKING);
	//m_isRun |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_CROSS_HAIR_START, Stage::C_CROSS_HAIR_MARKING);
	//m_isRun |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_PWR_METER_START, Stage::C_PWR_METER_END);
	//m_isRun |= LASER_INDEX[Index1].m_pFsm->Between(Stage::C_INDEX_CLEAN_START, Stage::C_INDEX_CLEAN_END);
	//m_isRun |= NV.use[USE_DUST_TEST] && LASER_INDEX[Index1].IsExist() && VC[LASER_INDEX[Index1].vcIndex].AOn();

	//m_isRun |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_LASER_START, Stage::C_LASER_END);
	//m_isRun |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_STAGE_CAL_START, Stage::C_STAGE_CAL_MARKING);
	//m_isRun |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_SCAN_CAL_START, Stage::C_SCAN_CAL_MARKING);
	//m_isRun |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_CROSS_HAIR_START, Stage::C_CROSS_HAIR_MARKING);
	//m_isRun |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_PWR_METER_START, Stage::C_PWR_METER_END);
	//m_isRun |= LASER_INDEX[Index2].m_pFsm->Between(Stage::C_INDEX_CLEAN_START, Stage::C_INDEX_CLEAN_END);
	//m_isRun |= NV.use[USE_DUST_TEST] && LASER_INDEX[Index2].IsExist() && VC[LASER_INDEX[Index2].vcIndex].AOn();

	m_isRun &= NV.use[USE_DUST];

	if (IsError())
	{
		Off(oRunNo1);
		Off(oRunNo2);
		m_isRun = FALSE;
		if (m_pFsm->IsBusy())
			m_pFsm->Set(C_ERROR);
	}

	BOOL isOn1Index = FALSE;
	BOOL isOn1Tonado = FALSE;
	//if (LASER_INDEX[Index1].m_pFsm->Between(Stage::C_LASER_START, Stage::C_LASER_END) ||
	//	LASER_INDEX[Index1].m_pFsm->Between(Stage::C_STAGE_CAL_START, Stage::C_STAGE_CAL_MARKING) ||
	//	LASER_INDEX[Index1].m_pFsm->Between(Stage::C_SCAN_CAL_START, Stage::C_SCAN_CAL_MARKING) ||
	//	LASER_INDEX[Index1].m_pFsm->Between(Stage::C_CROSS_HAIR_START, Stage::C_CROSS_HAIR_MARKING) ||
	//	LASER_INDEX[Index1].m_pFsm->Between(Stage::C_PWR_METER_START, Stage::C_PWR_METER_END) ||
	//	/*LASER_INDEX[Index1].m_pFsm->Between(Stage::C_INDEX_CLEAN_START, Stage::C_INDEX_CLEAN_END) ||*/
	//	NV.use[USE_DUST_TEST] && LASER_INDEX[Index1].IsExist() && VC[LASER_INDEX[Index1].vcIndex].IsOn())
	//{
	//	isOn1Index = TRUE;
	//}
	//if (LASER_INDEX[Index1].m_pFsm->Between(Stage::C_INDEX_CLEAN_START, Stage::C_INDEX_CLEAN_END) ||
	//	NV.use[USE_DUST_TEST] && LASER_INDEX[Index1].IsExist() && VC[LASER_INDEX[Index1].vcIndex].IsOn())
	//{
	//	isOn1Tonado = TRUE;
	//}

	BOOL isOn2Index = FALSE;
	BOOL isOn2Tonado = FALSE;
	//if (LASER_INDEX[Index2].m_pFsm->Between(Stage::C_LASER_START, Stage::C_LASER_END) ||
	//	LASER_INDEX[Index2].m_pFsm->Between(Stage::C_STAGE_CAL_START, Stage::C_STAGE_CAL_MARKING) ||
	//	LASER_INDEX[Index2].m_pFsm->Between(Stage::C_SCAN_CAL_START, Stage::C_SCAN_CAL_MARKING) ||
	//	LASER_INDEX[Index2].m_pFsm->Between(Stage::C_CROSS_HAIR_START, Stage::C_CROSS_HAIR_MARKING) ||
	//	LASER_INDEX[Index2].m_pFsm->Between(Stage::C_PWR_METER_START, Stage::C_PWR_METER_END) ||
	//	/*LASER_INDEX[Index2].m_pFsm->Between(Stage::C_INDEX_CLEAN_START, Stage::C_INDEX_CLEAN_END) ||*/
	//	NV.use[USE_DUST_TEST] && LASER_INDEX[Index2].IsExist() && VC[LASER_INDEX[Index2].vcIndex].IsOn())
	//{
	//	isOn2Index = TRUE;
	//}
	//if (LASER_INDEX[Index2].m_pFsm->Between(Stage::C_INDEX_CLEAN_START, Stage::C_INDEX_CLEAN_END) ||
	//	NV.use[USE_DUST_TEST] && LASER_INDEX[Index2].IsExist() && VC[LASER_INDEX[Index2].vcIndex].IsOn())
	//{
	//	isOn2Tonado = TRUE;
	//}

	//if (NV.use[USE_DUST] && IsOn(oRunNo1, 3000))
	//{
	//	if (isOn1Index) CYL[cDampIndex1].Open();
	//	if (isOn1Tonado) CYL[cDampTonado1].Open();
	//	if (isOn2Index) CYL[cDampIndex2].Open();
	//	if (isOn2Tonado) CYL[cDampTonado2].Open();
	//}
	//else if (OPR.isCycleRun || OPR.isAuto)
	//{
	//	CYL[cDampIndex1].Close();
	//	CYL[cDampTonado1].Close();
	//	CYL[cDampIndex2].Close();
	//	CYL[cDampTonado2].Close();
	//}
}

BOOL DustCollector::IsHwErr(void)
{
	if (CYL[cDampTonado1].GetErr())
		return (TRUE);
	if (CYL[cDampTonado2].GetErr())
		return (TRUE);
	if (CYL[cDampIndex1].GetErr())
		return (TRUE);
	if (CYL[cDampIndex2].GetErr())
		return (TRUE);

	return (FALSE);
}

BOOL DustCollector::IsError(BOOL isNotice)
{
	BOOL isError = FALSE;

	if (NV.use[USE_DUST_COLLECTOR_ALARM])
	{
		if (IsOn(oRunNo1, 10000) && AOff(iRunNo1, 10000))
		{
			ER.Save(ER_DUSTCOLLECTOR1_RUNNING);
			Off(oRunNo1);
			isError = TRUE;
		}
		if (IsOn(oRunNo2, 10000) && AOff(iRunNo2, 10000))
		{
			ER.Save(ER_DUSTCOLLECTOR2_RUNNING);
			Off(oRunNo2);
			isError = TRUE;
		}
	}

	if (NV.use[USE_DUST])
	{
		WR.Del(WR_DUSTCOLLECTOR_SKIP);
	}
	else
	{
		m_isRun = FALSE;
		if (isNotice)
			WR.Save(WR_DUSTCOLLECTOR_SKIP);
	}

	return (isError);
}

DustCollector DUSTCOLLECTOR;