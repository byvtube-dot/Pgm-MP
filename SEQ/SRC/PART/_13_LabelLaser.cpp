#include "..\..\Includes.h"

CLabelLaser LABEL_LASER;

CLabelLaser::CLabelLaser()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CLabelLaser::~CLabelLaser()
{

}

void CLabelLaser::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	int nDir = 0;
	if (LABEL_PART_DATA[nvLabelLtFeeder].exist)
		nDir = nvLabelLtFeeder;
	else if (LABEL_PART_DATA[nvLabelRtFeeder].exist)
		nDir = nvLabelRtFeeder;

	string MatId = static_cast<string> (LABEL_PART_DATA[nvLabelLtFeeder + nDir].labelInfo.sBoxId);
	if (0 == strcmp(LABEL_PART_DATA[nvLabelLtFeeder + nDir].labelInfo.sBoxId, ""))
		MatId = "$";

	if ("PRS_VI" == newValue)
		LABEL_LASER.m_pSLog->XFR(newValue, MatId, "LABEL_LASER", "LABEL_LASER");
	else if ("LASER" == newValue)
		LABEL_LASER.m_pSLog->XFR(newValue, MatId, "LABEL_LASER", "LABEL_LASER");
	else if ("POWER_METER" == newValue)
		LABEL_LASER.m_pSLog->XFR(newValue, MatId, "LABEL_LASER", "LABEL_LASER");
	else
		LABEL_LASER.m_pSLog->XFR(newValue, MatId, "", "");
}

void CLabelLaser::Auto(void)
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

	if (IsHwErr() || IsHwRdy())
		return;

	if (m_pFsm->IsBusy())
		return;

	if (!m_PowerMeterDid && NV.use[useCheckLaserPower] && NV.use[useLaserLabelPrint])
	{
		if (IsCanCyclePowerMeter())
			m_pFsm->Set(C_PWR_METER_START, MODE_PWR_CHECK);

		return;
	}

	int nDir;
	if (LABEL_PART_DATA[nvLabelLtFeeder].exist && LABEL_PART_DATA[nvLabelLtFeeder].flag.inUseCst)
		nDir = _LT;
	else if (LABEL_PART_DATA[nvLabelRtFeeder].exist && LABEL_PART_DATA[nvLabelRtFeeder].flag.inUseCst)
		nDir = _RT;
	else
		return;

	if (LABEL_PART_DATA[nDir].exist == enLabelExist)
	{
		if (!IsCanCyclePrs())
			return;

		m_pFsm->Set(C_PRS_VI_START, nDir);
		return;
	}

	if (LABEL_PART_DATA[nDir].exist == enLabelExistPrs)
	{
		if (!IsCanCycleLaser())
			return;

		if (SetLabelPrintReq(nDir))
			m_pFsm->Set(C_LASER_START, nDir);
		return;
	}
}

void CLabelLaser::Cycle(void)
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

	CyclePrs();
	CycleLaser();
	CyclePowerMeter();
	Cycle1PointCal();
}

void CLabelLaser::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "LabelLaser");
	m_pFsm = &FSM[fsmLabelLaser];

	m_pSLog = &SLOG[slogLabelLaser];
	m_pSLog->SetXFRDeviceId("LABEL_LASER");
	m_pSLog->SetXFRMaterialType("LASER");

	m_pSLog->Add(mtLabelLaserX, TRUE);

	m_pSLog->Add(cylPowerMeterOc, TRUE);
	m_pSLog->Add(LASER, TRUE);

	m_pSLog->Add(VI_LABEL_PRS, TRUE);
}

BOOL CLabelLaser::Common(void)
{
	if (NV.use[useLaserLabelPrint])
	{
		On(oSolLaserExpenderAirBlow);
		On(oSolLaserScannerAirBlow);
	}
	else
	{
		Off(oSolLaserExpenderAirBlow);
		Off(oSolLaserScannerAirBlow);
	}
	return TRUE;
}

BOOL CLabelLaser::IsHwErr(void)
{
	if (!MT[mtLabelLaserX].m_state.isHome)
		return TRUE;

	if (CYL[cylPowerMeterOc].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CLabelLaser::IsHwRdy(void)
{
	if (!MT[mtLabelLaserX].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CLabelLaser::CylInit(void)
{
	if (!CYL[cylPowerMeterOc].IsClose())
	{
		CYL[cylPowerMeterOc].Close(POWER_METER_CLOSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelLaser::IsCanCyclePrs(void)
{
	if (LABEL_PART_DATA[nvLabelLtFeeder].exist != enLabelExist && LABEL_PART_DATA[nvLabelRtFeeder].exist != enLabelExist)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Empty");
		return FALSE;
	}

	if (LABEL_FEEDER[enlabelLt].m_pFsm->IsBusy() || LABEL_FEEDER[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Cycle is Busy");
		return FALSE;
	}

	if (LABEL_SHUTTLE[enlabelLt].m_pFsm->IsBusy() || LABEL_SHUTTLE[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelLaser::IsCanCycleLaser(void)
{
	if (LABEL_PART_DATA[nvLabelLtFeeder].exist != enLabelExistPrs && LABEL_PART_DATA[nvLabelRtFeeder].exist != enLabelExistPrs)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Prs task Not Complete");
		return FALSE;
	}

	if (LABEL_FEEDER[enlabelLt].m_pFsm->IsBusy() || LABEL_FEEDER[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Feed Cycle is Busy");
		return FALSE;
	}

	return TRUE;
}

BOOL CLabelLaser::IsCanCyclePowerMeter(void)
{
	if (m_PowerMeterDid)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Power Meter flag task completed");
		return FALSE;
	}

	if (LABEL_FEEDER[enlabelLt].m_pFsm->IsBusy() || LABEL_FEEDER[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Feeder Cycle is Busy");
		return FALSE;
	}

	if (LABEL_SHUTTLE[enlabelLt].m_pFsm->IsBusy() || LABEL_SHUTTLE[enlabelRt].m_pFsm->IsBusy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Label Shuttle Cycle is Busy");
		return FALSE;
	}


	return TRUE;
}

BOOL CLabelLaser::SetLabelPrintReq(int nDir)
{
	for (int nvNo = nvTrayShuttle; nvNo > -1; nvNo--)
	{
		if (TRAY_PART_DATA[nvNo].exist && !Between(TRAY_PART_DATA[nvNo].exist, enExistBtmFoam, enExistBtmFoamQc))
		{
			if (!TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted || !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted)
			{
				memcpy(&LABEL_PART_DATA[nvLabelLtFeeder + nDir].labelInfo, &TRAY_PART_DATA[nvNo].labelInfo, sizeof(IPC_MMI::SBoxLabelInfo));
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CLabelLaser::SetLabelPrinted(void)
{
	for (int nvNo = nvTrayShuttle; nvNo > -1; nvNo--)
	{
		if (Between(TRAY_PART_DATA[nvNo].exist, enExistBtmFoam, enExistBtmFoamQc))
			continue;

		if (TRAY_PART_DATA[nvNo].exist &&
			!TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted)
		{
			TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted = true;
			return;
		}
		else if (TRAY_PART_DATA[nvNo].exist &&
			TRAY_PART_DATA[nvNo].flag.ltCobotLabelPrinted && !TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted)
		{
			TRAY_PART_DATA[nvNo].flag.rtCobotLabelPrinted = true;
			return;
		}
	}
	return;
}

void CLabelLaser::CyclePrs(void)
{
	if (!m_pFsm->Between(C_PRS_VI_START, C_PRS_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_LABEL_PRS_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch (m_pFsm->Get())
	{
		case C_PRS_VI_START:
		{
			if (m_pFsm->Once())
			{
				if (nMsgDir)
					LOG[logSEQ].Message("<%s> C_PRS_VI_START [RIGHT_FEEDER]", m_strName);
				else
					LOG[logSEQ].Message("<%s> C_PRS_VI_START [LEFT_FEEDER]", m_strName);

				m_tmCycleTime.Reset();
				m_state = "PRS_VI";

				m_nViCnt = 0;
				m_nViRetryCnt = 0;
				VI[VI_LABEL_PRS].Clear();
			}

			if (!VI[VI_LABEL_PRS].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CylInit())
				break;

			m_pFsm->Set(C_PRS_VI_MOVE);
			break;
		}
		case C_PRS_VI_MOVE:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PRS_VI_MOVE", m_strName);
				viMode = (ViMode)(MODE_LABEL_PRS_LABEL_EDGE_LT_TOP + m_nViCnt);
			}

			if (!MtPosMove(mtLabelLaserX, P_LASER_X_LT_PRS_LT + (nMsgDir * 2) + m_nViCnt))
				break;

			m_pFsm->Set(C_PRS_VI_RST);
			break;
		}
		case C_PRS_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PRS_VI_RST", m_strName);

			if (!VI[VI_LABEL_PRS].IsModeOn(viMode, 10))
			{
				VI[VI_LABEL_PRS].SetMode(viMode);
				break;
			}

			VI[VI_LABEL_PRS].Reset();
			m_pFsm->Set(C_PRS_VI_TRIG);
			break;
		}
		case C_PRS_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PRS_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_NOT_READY + (nMsgDir * 3)));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_LABEL_PRS].Start(viMode))
			{
				m_pFsm->Set(C_PRS_VI_RESULT);
			}
			break;
		}
		case C_PRS_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PRS_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_RESULT_TIME_OVER + (nMsgDir * 3)));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_LABEL_PRS].IsBusy())
				break;

			if (!VI[VI_LABEL_PRS].m_isRcvComp)
				break;

			bool bResultFail = false;
			bool bResultLimit = false;
			bResultFail |= (VI[VI_LABEL_PRS].m_prsData.result[0] != 1);
			bResultFail |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dX) == 999.999;
			bResultFail |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dY) == 999.999;
			bResultLimit |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dX) >= NV.ddm[prsDataToleranceX];
			bResultLimit |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dY) >= NV.ddm[prsDataToleranceY];

			if (bResultFail || bResultLimit)
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[labelPrsRetryCnt])
				{
					m_pFsm->Set(C_PRS_VI_MOVE);
					break;
				}

				if (bResultFail)
					ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_RESULT_FAIL + (nMsgDir * 3)));
				if (bResultLimit)
					ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_DATA_LIMIT + (nMsgDir * 3)));

				m_pFsm->Set(C_ERROR);
				break;
			}

			LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[m_nViCnt] = VI[VI_LABEL_PRS].m_prsData.data[0];
			m_nViCnt++;
			if (m_nViCnt < 2)		// 1: 1shot or 2: 2 Shot
				m_pFsm->Set(C_PRS_VI_MOVE);
			else
				m_pFsm->Set(C_PRS_VI_END);
			break;
		}
		case C_PRS_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PRS_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].exist = enLabelExistPrs;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLabelLaser::CycleLaser(void)
{
	if (!m_pFsm->Between(C_LASER_START, C_LASER_END))
		return;

	if (m_pFsm->TimeLimit(_10min))
	{
		ER.Save(ER_LABEL_LASER_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch(m_pFsm->Get())
	{
		case C_LASER_START:			
		{
			if (!NV.use[useLaserLabelPrint])
			{
				LOG[logSEQ].Message("<%s> Skip IndexLaser", m_strName);
				MtPosMove(mtLabelLaserX, P_LASER_X_LT_LASER + nMsgDir);
				m_pFsm->Set(C_LASER_END);
				break;
			}

			if (m_pFsm->Once())
			{
				if (nMsgDir)
					LOG[logSEQ].Message("<%s> C_LASER_START [RIGHT_FEEDER]", m_strName);
				else
					LOG[logSEQ].Message("<%s> C_LASER_START [LEFT_FEEDER]", m_strName);

				uphTable.MachineCnt = 0;
				m_tmCycleTime.Reset();
				m_state = "LASER";

				LAS[LASER].Clear();
				LAS[LASER].Reset();
				LAS[LASER].m_scanData.iStageNo = nMsgDir;

				double dLtX = LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dX;
				double dLtY = LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dY;
				double dLabelXPitch = MT[mtLabelLaserX].PosTable(P_LASER_X_LT_PRS_RT + (nMsgDir * 2)) - MT[mtLabelLaserX].PosTable(P_LASER_X_LT_PRS_LT + (nMsgDir * 2));
				double dRtX = dLabelXPitch + LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[1].dX;
				double dRtY = LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[1].dY;
				double dX = dRtX - dLtX;
				double dY = dRtY - dLtY;
				double dT = GetTheta(dX, dY);

				POINT2D ptLocal = { NV.rcp[labelEdgeCoordLtX], NV.rcp[labelEdgeCoordLtY] };
				POINT2D rot = { 0, 0 };
				POINT2D sCenter = { 0, 0 };
				XYT targetpos = { 0, 0, 0 };
				rot = Rotate(ptLocal, sCenter, dT);
				targetpos.dX = rot.dX + LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dX;
				targetpos.dY = rot.dY + LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dY;
				targetpos.dT = dT;

				LOG[logSEQ].Message("<%s> C_LASER_START LASER SEND DATA X:[%.3f] Y:[%.3f] T:[%.3f]", m_strName, targetpos.dX, targetpos.dY, targetpos.dT);

				ZeroMemory(LAS[LASER].m_scanData.laserLabelData.boxId, sizeof(LAS[LASER].m_scanData.laserLabelData.boxId));
				ZeroMemory(LAS[LASER].m_scanData.laserLabelData.readZpl, sizeof(LAS[LASER].m_scanData.laserLabelData.readZpl));
				memcpy(&LAS[LASER].m_scanData.laserLabelData.prsData, &targetpos, sizeof(XYT));
				memcpy(&LAS[LASER].m_scanData.laserLabelData.boxId, &LABEL_PART_DATA[nMsgDir].labelInfo.sBoxId, sizeof(LAS[LASER].m_scanData.laserLabelData.boxId));
				memcpy(&LAS[LASER].m_scanData.laserLabelData.readZpl, &LABEL_PART_DATA[nMsgDir].labelInfo.sBoxLabel, sizeof(LAS[LASER].m_scanData.laserLabelData.readZpl));
				LAS[LASER].SetMode(MODE_MARKING_START);
			}
			else
			{
				if (!OPR.isDryRun && !LAS[LASER].SendCoordData())
				{
					ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
					m_pFsm->Set(C_ERROR);
					break;
				}
				else
					m_pFsm->Set(C_LASER_SEND_DEVICE);
			}
			break;
		}

		case C_LASER_SEND_DEVICE:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LASER_SEND_DEVICE", m_strName);

				if (OPR.isNoDevice || OPR.isDryRun)
				{
					LAS[LASER].m_isWaitCoordData1 = false;
					LAS[LASER].m_isWaitCoordData2 = false;
				}
			}

			if (LAS[LASER].m_isError && !OPR.isDryRun)
			{
				OPR.isStop = TRUE;
				ER.Save(ER_SCANNER_ERROR);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!LAS[LASER].m_isReadData && !OPR.isNoDevice && !OPR.isDryRun)
				break;

			if (!LAS[LASER].m_isWaitCoordData1 && !LAS[LASER].m_isWaitCoordData2)
			{
				LOG[logSEQ].Message("<%s> DATA_RECV READLABELID: [%s]", m_strName, LAS[LASER].m_scanData.laserLabelData.readLabelId);
				m_pFsm->Set(C_LASER_MOVE);
			}
			else if (LAS[LASER].m_tmWaitCoordData.TmOver(5000))
			{
				ER.Save(ER_SEND_SCAN_COORD_TIMEOVER);
				m_pFsm->Set(C_ERROR);
			}
			break;
		}
		case C_LASER_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_MOVE", m_strName);
			
			if (MT_INPOS != MtPosMove(mtLabelLaserX, P_LASER_X_LT_LASER + nMsgDir))
				break;

			m_pFsm->Set(C_LASER_TRIG);
			break;
		}
		case C_LASER_TRIG:	
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_TRIG", m_strName);

			if (LAS[LASER].m_isError && !OPR.isDryRun)
			{
				OPR.isStop = TRUE;
				ER.Save(ER_SCANNER_ERROR);
				m_pFsm->Set(C_ERROR);
				break;
			}

			DWORD dwDelay_ms = (DWORD)NV.rcp[laserDelay];
			if (!MT[mtLabelLaserX].m_isRdy.TmOver(dwDelay_ms))
				return;

			if (LAS[LASER].Start())
				m_pFsm->Set(C_LASER_BUSY);

			break;
		}

		case C_LASER_BUSY:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_BUSY", m_strName);

			if (LAS[LASER].m_isError)
			{
				OPR.isStop = TRUE;
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (LAS[LASER].m_isWaitData || LAS[LASER].IsBusy())
				break;

			m_pFsm->Set(C_LASER_GEM);
			break;
		}

		case C_LASER_GEM:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_GEM", m_strName);

			m_pFsm->Set(C_LASER_END);
			break;
		}

		case C_LASER_END:
		{
			if (OPR.isNoDevice || OPR.isDryRun)
			{
				if (!m_pFsm->TimeLimit(_10sec))
					break;
			}

			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			LABEL_PART_DATA[nMsgDir].exist = enLabelExistPrinted;
			SetLabelPrinted();
			ZeroMemory(&LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0], sizeof(XYT));
			
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLabelLaser::CyclePowerMeter(void)
{
	if (!m_pFsm->Between(C_PWR_METER_START, C_PWR_METER_END))
		return;
	
	if (m_pFsm->TimeLimit(_10min))  // 시간이 오래 걸림..
	{
		ER.Save(ER_LABEL_LASER_POWER_METER_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_PWR_METER_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PWR_METER_START", m_strName);
				LAS[LASER].Clear();
				LAS[LASER].Reset();
				LAS[LASER].m_scanData.iStageNo = m_no;
				m_tmCycleTime.Reset();
				m_state = "POWER_METER";
			}

			m_pFsm->Set(C_PWR_METER_SEND_COORD);	
			break;
		}

		case C_PWR_METER_SEND_COORD:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_PWR_METER_SEND_COORD", m_strName);

				enLaserMode scanMode = (enLaserMode)m_pFsm->GetMsg();
				LAS[LASER].SetMode(scanMode);
				if (!OPR.isDryRun && !LAS[LASER].SendCoordData())
				{
					ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
					m_pFsm->Set(C_ERROR);
					break;
				}

				if (OPR.isNoDevice || OPR.isDryRun)
				{
					LAS[LASER].m_isWaitCoordData1 = false;
					LAS[LASER].m_isWaitCoordData2 = false;
				}
			}

			if (!LAS[LASER].m_isWaitCoordData1 && !LAS[LASER].m_isWaitCoordData2)
			{
				m_pFsm->Set(C_PWR_METER_MOVE);
			}
			else if (LAS[LASER].m_tmWaitCoordData.TmOver(5000))
			{
				ER.Save(ER_SEND_SCAN_COORD_TIMEOVER);
				m_pFsm->Set(C_ERROR);
			}
			break;
		}

		case C_PWR_METER_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PWR_METER_MOVE", m_strName);

			CYL[cylPowerMeterOc].Open(POWER_METER_OPEN);

			if (MT_INPOS != MtPosMove(mtLabelLaserX, P_LASER_X_POWER_METER))
				break;

			m_pFsm->Set(C_PWR_METER_TRIG);
			break;
		}

		case C_PWR_METER_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PWR_METER_TRIG", m_strName);

			if (LAS[LASER].m_isError)
			{
				OPR.isStop = TRUE;
				m_pFsm->Set(C_ERROR);
				break;
			}
		
			DWORD dwDelay_ms = (DWORD)NV.rcp[laserDelay];

			if (!MT[mtLabelLaserX].m_isRdy.TmOver(dwDelay_ms))
				break;

			if (!CYL[cylPowerMeterOc].IsOpen())
			{
				CYL[cylPowerMeterOc].Open(POWER_METER_OPEN);
				break;
			}

			if (OPR.isDryRun || LAS[LASER].Start())
				m_pFsm->Set(C_PWR_METER_BUSY);	
			break;
		}

		case C_PWR_METER_BUSY:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_PWR_METER_BUSY", m_strName);

			if (LAS[LASER].m_isError)
			{
				OPR.isStop = TRUE;
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (LAS[LASER].m_isWaitData || LAS[LASER].IsBusy())
				break;

			m_pFsm->Set(C_PWR_METER_END);
			break;
		}

		case C_PWR_METER_END:
		{
			if (!CYL[cylPowerMeterOc].IsClose())
			{
				CYL[cylPowerMeterOc].Close(POWER_METER_CLOSE);
				break;
			}

			LOG[logSEQ].Message("<%s> C_PWR_METER_END[%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			if (MODE_PWR_CHECK == m_pFsm->GetMsg())
			{
				m_PowerMeterDid = true;
				//IsPowerCheckComp() = TRUE;
			}
			else
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"<%s> Power Mapping Finish..!", m_strName);
			}
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CLabelLaser::Cycle1PointCal(void)
{
	if (!m_pFsm->Between(C_LASER_1POINT_CAL_START, C_LASER_1POINT_CAL_END))
		return;

	if(m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_LABEL_LASER_1POINT_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	int nMsgDir = m_pFsm->GetMsg();
	switch(m_pFsm->Get())
	{
		case C_LASER_1POINT_CAL_START:
		{
			if (m_pFsm->Once())
			{
				if (nMsgDir)
					LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_VI_START [RIGHT_FEEDER]", m_strName);
				else
					LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_VI_START [LEFT_FEEDER]", m_strName);

				m_state = "LASER_1POINT_CAL";
				m_tmCycleTime.Reset();

				LAS[LASER].Clear();
				LAS[LASER].Reset();
				LAS[LASER].m_scanData.iStageNo = nMsgDir;
				ZeroMemory(&LAS[LASER].m_scanData.laserLabelData.prsData, sizeof(XYT));
				LAS[LASER].SetMode(MODE_CROSSHAIR);

				m_nViCnt = 0;
				VI[VI_LABEL_PRS].Clear();

				if (NV.rcp[labelEdgeCoordLtX] == 0)		NV.rcp[labelEdgeCoordLtX] = -60.0;
				if (NV.rcp[labelEdgeCoordLtY] == 0)		NV.rcp[labelEdgeCoordLtY] =  25.0;
				if (NV.rcp[labelEdgeCoordRtX] == 0)		NV.rcp[labelEdgeCoordRtX] =  60.0;
				if (NV.rcp[labelEdgeCoordRtY] == 0)		NV.rcp[labelEdgeCoordRtY] =  25.0;
				if (NV.rcp[calCrossHairCoordLtX] == 0)	NV.rcp[calCrossHairCoordLtX] = -58.0;
				if (NV.rcp[calCrossHairCoordLtY] == 0)	NV.rcp[calCrossHairCoordLtY] =  23.0;
				if (NV.rcp[calCrossHairCoordRtX] == 0)	NV.rcp[calCrossHairCoordRtX] =  58.0;
				if (NV.rcp[calCrossHairCoordRtY] == 0)	NV.rcp[calCrossHairCoordRtY] =  23.0;
			}

			if (!OPR.isDryRun && !LAS[LASER].SendCoordData())
			{
				ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
				m_pFsm->Set(C_ERROR);
				break;
			}
			else
				m_pFsm->Set(C_LASER_1POINT_CAL_SEND_DEVICE);

			break;
		}
		case C_LASER_1POINT_CAL_SEND_DEVICE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_SEND_DEVICE", m_strName);

			if (!LAS[LASER].m_isWaitCoordData1 && !LAS[LASER].m_isWaitCoordData2)
			{
				LOG[logSEQ].Message("<%s> DATA_RECV READLABELID: [%s]", m_strName, LAS[LASER].m_scanData.laserLabelData.readLabelId);
				m_pFsm->Set(C_LASER_1POINT_CAL_MOVE);
			}
			else if (LAS[LASER].m_tmWaitCoordData.TmOver(5000))
			{
				ER.Save(ER_SEND_SCAN_COORD_TIMEOVER);
				m_pFsm->Set(C_ERROR);
			}
			break;
		}
		case C_LASER_1POINT_CAL_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtLabelLaserX, P_LASER_X_LT_LASER + nMsgDir))
				break;

			m_pFsm->Set(C_LASER_1POINT_CAL_TRIG);
			break;
		}
		case C_LASER_1POINT_CAL_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_TRIG", m_strName);

			if (LAS[LASER].m_isError && !OPR.isDryRun)
			{
				OPR.isStop = TRUE;
				ER.Save(ER_SCANNER_ERROR);
				m_pFsm->Set(C_ERROR);
				break;
			}

			DWORD dwDelay_ms = (DWORD)NV.rcp[laserDelay];
			if (!MT[mtLabelLaserX].m_isRdy.TmOver(dwDelay_ms))
				return;

			if (LAS[LASER].Start())
				m_pFsm->Set(C_LASER_1POINT_CAL_BUSY);

			break;
		}
		case C_LASER_1POINT_CAL_BUSY:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_BUSY", m_strName);

			if (LAS[LASER].m_isError)
			{
				OPR.isStop = TRUE;
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (LAS[LASER].m_isWaitData || LAS[LASER].IsBusy())
				break;

			m_pFsm->Set(C_LASER_1POINT_CAL_VI_MOVE);
			break;
		}
		case C_LASER_1POINT_CAL_VI_MOVE:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_VI_MOVE", m_strName);
			}

			double dPos = 0.0;
			double dPitch = 0.0;
			if (m_nViCnt == 0)
			{
				dPitch = NV.rcp[calCrossHairCoordLtX] - NV.rcp[labelEdgeCoordLtX];
				dPos = MT[mtLabelLaserX].PosTable(P_LASER_X_LT_PRS_LT + (nMsgDir * 2)) + dPitch;
			}
			else
			{
				dPitch = NV.rcp[calCrossHairCoordRtX] - NV.rcp[labelEdgeCoordRtX];
				dPos = MT[mtLabelLaserX].PosTable(P_LASER_X_LT_PRS_RT + (nMsgDir * 2)) + dPitch;
			}

			if (!MtPosMove(mtLabelLaserX, P_LASER_X_LT_PRS_LT + (nMsgDir * 2) + m_nViCnt, dPos))
				break;

			m_pFsm->Set(C_LASER_1POINT_CAL_VI_RST);
			break;
		}
		case C_LASER_1POINT_CAL_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_VI_RST", m_strName);

			viMode = MODE_LABEL_PRS_1POINT;
			if (!VI[VI_LABEL_PRS].IsModeOn(viMode, 10))
			{
				VI[VI_LABEL_PRS].SetMode(viMode);
				break;
			}

			VI[VI_LABEL_PRS].Reset();
			m_pFsm->Set(C_LASER_1POINT_CAL_VI_TRIG);
			break;
		}
		case C_LASER_1POINT_CAL_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_NOT_READY + (nMsgDir * 3)));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_LABEL_PRS].Start(viMode))
				m_pFsm->Set(C_LASER_1POINT_CAL_VI_RESULT);

			break;
		}
		case C_LASER_1POINT_CAL_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_RESULT_TIME_OVER + (nMsgDir * 3)));
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_LABEL_PRS].IsBusy() || !VI[VI_LABEL_PRS].m_isRcvComp)
				break;

			bool bResultFail = false;
			bool bResultLimit = false;
			bResultFail |= (VI[VI_LABEL_PRS].m_prsData.result[0] != 1);
			bResultFail |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dX) == 999.999;
			bResultFail |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dY) == 999.999;
			bResultLimit |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dX) >= 10.0;
			bResultLimit |= fabs(VI[VI_LABEL_PRS].m_prsData.data[0].dY) >= 10.0;

			if (bResultFail || bResultLimit)
			{
				if (bResultFail)
					ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_RESULT_FAIL + (nMsgDir * 3)));
				if (bResultLimit)
					ER.Save((ErrName)(ER_VISION_LABEL_PRS_LABEL_EDGE_LT_TOP_DATA_LIMIT + (nMsgDir * 3)));

				m_pFsm->Set(C_ERROR);
				break;
			}

			LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[m_nViCnt] = VI[VI_LABEL_PRS].m_prsData.data[0];
			m_nViCnt++;
			if (m_nViCnt < 2)
				m_pFsm->Set(C_LASER_1POINT_CAL_VI_MOVE);
			else
				m_pFsm->Set(C_LASER_1POINT_CAL_RESULT_SEND);
			break;
		}
		case C_LASER_1POINT_CAL_RESULT_SEND:
		{
			if (m_pFsm->Once())
			{
				LAS[LASER].m_scanData.iStageNo = nMsgDir;

				double dLtX = LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dX;
				double dLtY = LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dY;
				double dRtX = MT[mtLabelLaserX].PosTable(P_LASER_X_LT_PRS_RT + (nMsgDir * 2)) - MT[mtLabelLaserX].PosTable(P_LASER_X_LT_PRS_LT + (nMsgDir * 2)) - 4
					+ LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[1].dX;
				double dRtY = LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[1].dY;
				double dX = dRtX - dLtX;
				double dY = dRtY - dLtY;
				double dT = GetTheta(dX, dY);

				POINT2D ptLocal = { NV.rcp[calCrossHairCoordLtX], NV.rcp[calCrossHairCoordLtY] };
				POINT2D rot = { 0, 0 };
				POINT2D sCenter = { 0, 0 };
				rot = Rotate(ptLocal, sCenter, dT);

				XYT targetpos = { 0, 0, 0 };
				targetpos.dX = rot.dX - LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dX;
				targetpos.dY = rot.dY - LABEL_PART_DATA[nvLabelLtFeeder + nMsgDir].PrsResult[0].dY - 2;	// Check Point!!
				targetpos.dT = -dT;

				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_RESULT_SEND X:[%.3f] Y:[%.3f] T:[%.3f]", m_strName, targetpos.dX, targetpos.dY, targetpos.dT);

				memcpy(&LAS[LASER].m_scanData.laserLabelData.prsData, &targetpos, sizeof(XYT));
				LAS[LASER].SetMode(MODE_1POINT_DONE);
			}

			if (!OPR.isDryRun && !LAS[LASER].SendCoordData())
			{
				ER.Save(ER_NOT_DEFINE_SCANNER_NUMBER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LASER_1POINT_CAL_END);
			break;
		}
		case C_LASER_1POINT_CAL_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LASER_1POINT_CAL_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}