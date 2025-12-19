#include "..\..\Includes.h"

CInPk IN_PK;

CInPk::CInPk()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CInPk::~CInPk()
{

}

void CInPk::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	string MatId = static_cast<string> (TRAY_PART_DATA[nvInPk].labelInfo.sBoxId);
	if (0 == strcmp(TRAY_PART_DATA[nvInPk].labelInfo.sBoxId, ""))
		MatId = "$";

	// TODO: 25.10.20
	if ("BTM_FOAM_VI" == newValue)
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "IN_PICKER", "IN_PICKER");

		IN_PK.m_pSLog->RecordingOn(mtInPkX);
		IN_PK.m_pSLog->RecordingOn(mtInPkZ);
		IN_PK.m_pSLog->RecordingOn(cylInPkOc);
		IN_PK.m_pSLog->RecordingOn(VI_IN_PK);
	}
	else if ("PICKUP" == newValue)
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "BANDING_PART", "IN_PICKER");

		IN_PK.m_pSLog->RecordingOn(mtInPkX);
		IN_PK.m_pSLog->RecordingOn(mtInPkZ);
		IN_PK.m_pSLog->RecordingOn(cylInPkOc);
	}
	else if ("INSP" == newValue)
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "IN_PICKER", "IN_PICKER");

		IN_PK.m_pSLog->RecordingOn(mtInPkX);
		IN_PK.m_pSLog->RecordingOn(mtInPkZ);
		IN_PK.m_pSLog->RecordingOn(cylInPkOc);
		IN_PK.m_pSLog->RecordingOn(VI_IN_PK_BAND);
	}
	else  if ("PLACE" == newValue)
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "IN_PICKER", "BTM_FOAM_CONV");

		IN_PK.m_pSLog->RecordingOn(mtInPkX);
		IN_PK.m_pSLog->RecordingOn(mtInPkZ);
		IN_PK.m_pSLog->RecordingOn(cylInPkOc);
	}
	else  if ("BTM_FOAM_TRAY_VI" == newValue)
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "IN_PICKER", "IN_PICKER");

		IN_PK.m_pSLog->RecordingOn(mtInPkX);
		IN_PK.m_pSLog->RecordingOn(mtInPkZ);
		IN_PK.m_pSLog->RecordingOn(cylInPkOc);
		IN_PK.m_pSLog->RecordingOn(VI_IN_PK);
	}
	else  if ("LABEL_ATTACH_VI" == newValue)
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "IN_PICKER", "IN_PICKER");

		IN_PK.m_pSLog->RecordingOn(mtInPkX);
		IN_PK.m_pSLog->RecordingOn(mtInPkZ);
		IN_PK.m_pSLog->RecordingOn(cylInPkOc);
		IN_PK.m_pSLog->RecordingOn(VI_IN_PK);
	}
	else
	{
		IN_PK.m_pSLog->XFR(newValue, MatId, "", "");

		IN_PK.m_pSLog->RecordingOff(mtInPkX);
		IN_PK.m_pSLog->RecordingOff(mtInPkZ);
		IN_PK.m_pSLog->RecordingOff(cylInPkOc);
		IN_PK.m_pSLog->RecordingOff(VI_IN_PK);
		IN_PK.m_pSLog->RecordingOff(VI_IN_PK_BAND);
	}
}

void CInPk::Auto(void)
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

	if (Exist())
	{
		if (Exist() == enExistTray)
		{
			if (!IsCanCycleBandingVi())
				return;

			m_pFsm->Set(C_BANDING_VI_START);
			return;
		}
		else if (Exist() == enExistTrayQc)
		{
			if (FOAM_RAIL.Exist(_BTM_FOAM_) == enExistBtmFoamOpen)
			{
				if (!IsCanCycleBtmFoamVi())
					return;

				m_pFsm->Set(C_BTM_FOAM_VI_START);
				return;
			}
			else if (FOAM_RAIL.Exist(_BTM_FOAM_) == enExistBtmFoamQc)
			{
				if (!IsCanCyclePlace())
					return;

				m_pFsm->Set(C_TRAY_PLACE_START);
				return;
			}
		}
	}
	else
	{
		if (FOAM_RAIL.Exist(_TOP_FOAM_) == enExistTrayTopFoam)
		{
			if (!IsCanCycleTopFoamVi())
				return;

			m_pFsm->Set(C_LABEL_ATTACH_VI_START);
			return;
		}
		else if (FOAM_RAIL.Exist(_BTM_FOAM_) == enExistBtmFoamTray)
		{
			if (!IsCanCycleBtmFoamTrayVi())
				return;

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_START);
			return;
		}
		else
		{
			if (!IsCanCyclePickUp())
				return;

			m_pFsm->Set(C_TRAY_PICK_UP_START);
			return;
		}
	}
}

void CInPk::Cycle(void)
{
	if (!m_pFsm->IsBusy())
		return;

	if (OPR.isDoorOpen)
		m_pFsm->RstTimeLimit();

	if (IsHwErr() || C_ERROR == m_pFsm->Get())
	{
		if (OPR.isStop && !OPR.isAuto)
			m_pFsm->Set(C_IDLE);

		Off(oSmema01Recv);
		Off(oSmema02Comp);
		return;
	}

	CycleBtmFoamVi();
	CyclePickUp();
	CycleBandingVi();
	CyclePlace();
	CycleBtmFoamTrayVi();
	CycleTopFoamVi();
}

void CInPk::Init(int nIdx)
{
	strcpy(m_strName, "InPk");
	m_pFsm = &FSM[fsmInPick];

	m_pSLog = &SLOG[slogInPk];
	m_pSLog->SetXFRDeviceId("IN_PICKER");
	m_pSLog->SetXFRMaterialId("$");
	m_pSLog->SetXFRMaterialType("TRAY");
	m_pSLog->Add(mtInPkX);
	m_pSLog->Add(mtInPkZ);

	m_pSLog->Add(cylInPkOc);

	m_pSLog->Add(VI_IN_PK);
	m_pSLog->Add(VI_IN_PK_BAND);
}

BOOL CInPk::Common(void)
{
	if (!m_pFsm->IsBusy())
	{
		if (!Exist())
		{
		}
		else
		{
		}
	}

	// Smema On,Off Condition
	if (MT[mtInPkZ].InPos(P_IN_PK_Z_READY) || (MT[mtInPkX].m_state.isHome && !MT[mtInPkX].InPos(P_IN_PK_X_PICK)))
	{
		On(oSmema03Safety);
		if (!Exist())
			On(oSmema04Auto);
		else
			Off(oSmema04Auto);
	}
	else
	{
		Off(oSmema03Safety);
		Off(oSmema04Auto);
	}

	return TRUE;
}

BOOL CInPk::IsHwErr(void)
{
	if (!MT[mtInPkX].m_state.isHome)
		return TRUE;
	if (!MT[mtInPkZ].m_state.isHome)
		return TRUE;

	if (CYL[cylInPkOc].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CInPk::IsHwRdy(void)
{
	if (!MT[mtInPkX].IsRdy())
		return TRUE;
	if (!MT[mtInPkZ].IsRdy())
		return TRUE;

	return FALSE;
}

BOOL CInPk::CylInit(void)
{
	if (Exist() && !CYL[cylInPkOc].IsClose())
	{
		CYL[cylInPkOc].Close(IN_PK_CLOSE);
		return FALSE;
	}
	if (!Exist() && !CYL[cylInPkOc].IsOpen())
	{
		CYL[cylInPkOc].Open(IN_PK_OPEN);
		return FALSE;
	}

	return TRUE;
}

double CInPk::GetCurBtmFoamViZOffset(void)
{
	double offset = 0.0;
	// Tray Count 3 ~ 6  : Partial Foam
	// Tray Count 7 ~ 11 : Normal Foam
	int trayCnt = 11;
	if (TRAY_PART_DATA[nvBtmFoam].lotInfo.trayCnt > 0)
		trayCnt = TRAY_PART_DATA[nvBtmFoam].lotInfo.trayCnt;

	if (trayCnt < _TRAY_MIN_CNT)
		trayCnt = _TRAY_MIN_CNT;

	// Lower Z-axis by the difference from max quantity position
	int trayDiffCnt = _TRAY_MAX_CNT - trayCnt;
	double trayThick = 5.62;
	if (TRAY_PART_DATA[nvBtmFoam].lotInfo.trayThick > 0)
		trayThick = TRAY_PART_DATA[nvBtmFoam].lotInfo.trayThick;
	double foamDiff = 0.0;
	if (trayCnt < 7)		// Partial foam
		foamDiff = NV.rcp[partialFoamBtmGap] - NV.rcp[normalFoamBtmGap];

	offset = trayDiffCnt * trayThick - foamDiff;
	return offset;
}


double CInPk::GetCurTopFoamViZOffset(void)
{
	double offset = 0.0;
	// Tray Count 3 ~ 6  : Partial Foam
	// Tray Count 7 ~ 11 : Normal Foam
	int trayCnt = 11;
	if (TRAY_PART_DATA[nvTopFoam].lotInfo.trayCnt > 0)
		trayCnt = TRAY_PART_DATA[nvTopFoam].lotInfo.trayCnt;

	if (trayCnt < _TRAY_MIN_CNT)
		trayCnt = _TRAY_MIN_CNT;

	// Lower Z-axis by the difference from max quantity position
	int trayDiffCnt = _TRAY_MAX_CNT - trayCnt;
	double trayThick = 5.62;
	if (TRAY_PART_DATA[nvTopFoam].lotInfo.trayThick > 0)
		trayThick = TRAY_PART_DATA[nvTopFoam].lotInfo.trayThick;
	double foamDiff = 0.0;
	if (trayCnt < 7)		// Partial foam
		foamDiff = NV.rcp[partialFoamBtmGap] - NV.rcp[normalFoamBtmGap];

	offset = trayDiffCnt * trayThick - foamDiff;
	return offset;
}

int CInPk::GetCurFoamType(void)
{
	// Tray Count 3 ~ 6  : Partial Foam
	// Tray Count 7 ~ 11 : Normal Foam
	int trayCnt = TRAY_PART_DATA[nvInPk].lotInfo.trayCnt;

	if (trayCnt < _TRAY_MIN_CNT)
		trayCnt = _TRAY_MIN_CNT;

	if (trayCnt < 7)		// Partial foam
		return 1;
	else
		return 0;
}

BOOL CInPk::IsCanCyclePickUp(void)
{
	if (Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"InPk Memory Exist");
		return FALSE;
	}

	if (IsOn(oSmema01Recv))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"oSmema01Recv On");
		return FALSE;
	}
	if (IsOn(oSmema02Comp))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"oSmema02Comp On");
		return FALSE;
	}
	if (IsOff(oSmema03Safety))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"oSmema03Safety Off");
		return FALSE;
	}
	if (IsOff(oSmema04Auto))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"oSmema04Auto Off");
		return FALSE;
	}
	if (!NV.use[useStandalone] && AOff(iSmema01Request))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"iSmema01Request Off");
		return FALSE;
	}
	if (AOn(iSmema02Comp))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"iSmema02Comp On");
		return FALSE;
	}

	// TODO DELETE
	//if (OPR.isNoDevice && NV.ndm[simulationDebug0] > 100)
	//	return FALSE;

	return TRUE;
}

BOOL CInPk::IsCanCycleBandingVi(void)
{
	if (!NV.use[useInPkBandingInsp])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"useInPkBandingInsp Skip");
		return FALSE;
	}
	if (!VI[VI_IN_PK_BAND].IsAuto())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Vision Not Auto");
		return FALSE;
	}

	if (!Exist())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"InPk Memory Empty");
		return FALSE;
	}

	if (!NV.ndm[ltCobotInPkCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot Not Safety Pos or Cycle is Running ");
		return FALSE;
	}

	return TRUE;
}

BOOL CInPk::IsCanCycleBtmFoamVi(void)
{
	if (FOAM_RAIL.Exist(_BTM_FOAM_) != enExistBtmFoamOpen)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Memory Mismatch");
		return FALSE;
	}

	if (!NV.ndm[ltCobotInPkCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot Not Safety Pos or Cycle is Running ");
		return FALSE;
	}

	return TRUE;
}

BOOL CInPk::IsCanCycleBtmFoamTrayVi(void)
{
	if (FOAM_RAIL.Exist(_BTM_FOAM_) != enExistBtmFoamTray)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Memory Mismatch");
		return FALSE;
	}

	if (!NV.ndm[ltCobotInPkCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot Not Safety Pos or Cycle is Running ");
		return FALSE;
	}

	return TRUE;
}

BOOL CInPk::IsCanCycleTopFoamVi(void)
{
	if (FOAM_RAIL.Exist(_TOP_FOAM_) != enExistTrayTopFoam)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Foam Rail Memory Mismatch");
		return FALSE;
	}

	if (!NV.ndm[ltCobotInPkCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot Not Safety Pos or Cycle is Running ");
		return FALSE;
	}

	return TRUE;
}

BOOL CInPk::IsCanCyclePlace(void)
{
	if (FOAM_RAIL.Exist(_BTM_FOAM_) != enExistBtmFoamQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Btm Foam Rail Memory Mismatch");
		return FALSE;
	}

	if (Exist() != enExistTrayQc)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"InPk Memory Type Mismatch");
		return FALSE;
	}

	if (!NV.ndm[ltCobotInPkCanMove])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"Left Cobot Not Safety Pos or Cycle is Running ");
		return FALSE;
	}

	return TRUE;
}

BOOL CInPk::ExistCrossCheck(void)
{
	if (MT[mtInPkZ].InPos(P_IN_PK_Z_READY))
	{
		if (!Exist() && AOn(iInPkExist))
		{
			ER.Save(ER_IN_PK_SENSOR_STATUS);
			return TRUE;
		}
		else if (Exist() && AOff(iInPkExist))
		{
			ER.Save(ER_IN_PK_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CInPk::Exist(void)
{
	return TRAY_PART_DATA[nvInPk].exist;
}

BOOL CInPk::IsExist(void)
{
	if (!TRAY_PART_DATA[nvInPk].exist || AOff(iInPkExist))
	{
		ER.Save(ER_IN_PK_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CInPk::IsEmpty(void)
{
	if (TRAY_PART_DATA[nvInPk].exist || AOn(iInPkExist))
	{
		ER.Save(ER_IN_PK_EXIST);
		return FALSE;
	}

	return TRUE;
}

void CInPk::CycleBtmFoamVi(void)
{
	if (!m_pFsm->Between(C_BTM_FOAM_VI_START, C_BTM_FOAM_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_IN_PK_BTM_FOAM_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_BTM_FOAM_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "BTM_FOAM_VI";

				m_nViCnt = 0;
				m_nViCntTemp = 0;
				m_nViRetryCnt = 0;
				VI[VI_IN_PK].Clear();
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!VI[VI_IN_PK].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_BTM_FOAM_VI_MOVE);
			break;
		}
		case C_BTM_FOAM_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_BTM_FOAM_VI_1ST + m_nViCnt))
				break;

			m_pFsm->Set(C_BTM_FOAM_VI_DN);
			break;
		}
		case C_BTM_FOAM_VI_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_DN", m_strName);

			int nFoamType = _NORMAL;
			if (TRAY_PART_DATA[nvBtmFoam].flag.btmPartialFoamReq)
				nFoamType = _PARTIAL;

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_BTM_FOAM_VI_NORMAL + nFoamType))
				break;

			m_pFsm->Set(C_BTM_FOAM_VI_RST);
			break;
		}
		case C_BTM_FOAM_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_RST", m_strName);

			if (m_nViRetryCnt > 0)
			{
				if (!m_pFsm->TimeLimit(_1sec))
					break;
			}

			viMode = MODE_IN_PK_BTM_FOAM;
			if (!VI[VI_IN_PK].IsModeOn(viMode, 10))
			{
				VI[VI_IN_PK].SetMode(viMode);
				break;
			}

			VI[VI_IN_PK].Reset();
			m_pFsm->Set(C_BTM_FOAM_VI_TRIG);
			break;
		}
		case C_BTM_FOAM_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_BTM_FOAM_VI_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK].Start(viMode))
			{
				m_nViCntTemp = m_nViCnt;
				m_pFsm->Set(C_BTM_FOAM_VI_RESULT);
			}

			break;
		}
		case C_BTM_FOAM_VI_RESULT:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_RESULT", m_strName);
				m_nViCntTemp++;
			}

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_BTM_FOAM_VI_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK].IsBusy())
				break;

			if (m_nViCntTemp < 2)
			{
				if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_BTM_FOAM_VI_1ST + m_nViCntTemp))
					break;
			}

			if (!VI[VI_IN_PK].m_isRcvComp)
				break;

			if (!VI[VI_IN_PK].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[inPkViRetryCnt])
				{
					m_pFsm->Set(C_BTM_FOAM_VI_RST);
					break;
				}

				ER.Save(ER_VISION_IN_PK_BTM_FOAM_VI_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_nViRetryCnt = 0;
			m_nViCnt++;
			if (m_nViCnt < 2)
				m_pFsm->Set(C_BTM_FOAM_VI_MOVE);
			else
				m_pFsm->Set(C_BTM_FOAM_VI_UP);

			break;
		}
		case C_BTM_FOAM_VI_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_UP", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_READY))
				break;

			m_pFsm->Set(C_BTM_FOAM_VI_END);
			break;
		}
		case C_BTM_FOAM_VI_END:
		{
			if (IsHwRdy())
				return;

			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			TRAY_PART_DATA[nvBtmFoam].exist = enExistBtmFoamQc;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CInPk::CyclePickUp(void)
{
	if (!m_pFsm->Between(C_TRAY_PICK_UP_START, C_TRAY_PICK_UP_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_IN_PK_PICK_UP_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_TRAY_PICK_UP_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "PICKUP";
				NV.ndm[recvPreMcData] = COMM_IDLE;
				NV.ndm[commLotDisplay] = COMM_IDLE;
				NV.ndm[commLotCloseCheck] = COMM_IDLE;
				NV.ndm[commTrayTransferStart] = COMM_IDLE;
				ZeroMemory(&TRAY_PART_DATA[nvInPk], sizeof(IPC_MMI::TrayPartData));
			}

			if (!NV.use[useStandalone] && AOff(iSmema01Request))
			{
				ER.Save(ER_IN_PK_PRE_MC_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}
			else
			{
				if (!IsOn(oSmema01Recv))
					m_pSLog->LogFncIn("PICKUP_1ST_MC_COMP_RECV", "START", "$", "100", iSmema02Comp, "'ON'");

				On(oSmema01Recv);
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!IsEmpty())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!CylInit())
				break;

			m_pFsm->Set(C_TRAY_PICK_UP_DATA_RECV);
			break;
		}
		case C_TRAY_PICK_UP_DATA_RECV:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_DATA_RECV", m_strName);
				NV.ndm[recvPreMcData] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_50sec))
			{
				ER.Save(ER_IN_PK_PRE_MC_DATA_RECV_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if ((!NV.use[useStandalone] && !AOn(iSmema02Comp, 100)) || NV.ndm[recvPreMcData] != COMM_COMP)
				break;

			m_pSLog->LogFncIn("PICKUP_1ST_MC_COMP_RECV", "END", "$", "100", iSmema02Comp, "'ON'");

			if (0 == strcmp(TRAY_PART_DATA[nvInPk].labelInfo.lotId, ""))
			{
				ER.Save(ER_IN_PK_DATA_PACKING_DATA_EMPTY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			// Tray Count 3 ~ 6  : Partial Foam
			// Tray Count 7 ~ 11 : Normal Foam
			if (TRAY_PART_DATA[nvInPk].lotInfo.trayCnt > 6)		// Normal foam
				TRAY_PART_DATA[nvInPk].flag.btmPartialFoamReq = false;
			else					// Partial foam
				TRAY_PART_DATA[nvInPk].flag.btmPartialFoamReq = true;

			m_pFsm->Set(C_TRAY_PICK_UP_SEMEMA_CHECK);
			break;
		}
		case C_TRAY_PICK_UP_SEMEMA_CHECK:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_SEMEMA_CHECK", m_strName);
				On(oSmema02Comp);
			}

			if (AOff(iSmema02Comp))
				Off(oSmema02Comp);

			if (IsOff(oSmema02Comp))
				m_pFsm->Set(C_TRAY_PICK_UP_MOVE);
			break;
		}
		case C_TRAY_PICK_UP_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_PICK))
				break;

			m_pFsm->Set(C_TRAY_PICK_UP_Z_DN);
			break;
		}
		case C_TRAY_PICK_UP_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_Z_DN", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_PICK))
				break;

			m_pFsm->Set(C_TRAY_PICK_UP_Z_UP);
			break;
		}
		case C_TRAY_PICK_UP_Z_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_Z_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commTrayTransferStart] = COMM_REQ;
			}

			if (!CYL[cylInPkOc].IsClose())
			{
				CYL[cylInPkOc].Close(IN_PK_CLOSE);
				break;
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			Off(oSmema01Recv);
			if (NV.use[useSecsGem])
				m_pFsm->Set(C_TRAY_PICK_UP_LOT_DISPLAY);
			else
				m_pFsm->Set(C_TRAY_PICK_UP_END);

			break;
		}
		case C_TRAY_PICK_UP_LOT_DISPLAY:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_LOT_DISPLAY", m_strName);
				NV.ndm[commLotDisplay] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_IN_PK_LOT_DISPLAY_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[commLotDisplay] != COMM_COMP)
				break;

			m_pFsm->Set(C_TRAY_PICK_UP_LOT_CLOSE_CHECK);
			break;
		}
		case C_TRAY_PICK_UP_LOT_CLOSE_CHECK:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_LOT_CLOSE_CHECK", m_strName);
				NV.ndm[commLotCloseCheck] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_IN_PK_LOT_CLOSE_CHECK_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[commLotCloseCheck] != COMM_COMP)
				break;

			m_pFsm->Set(C_TRAY_PICK_UP_LABEL_INFO);
			break;
		}
		case C_TRAY_PICK_UP_LABEL_INFO:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_LABEL_INFO", m_strName);
				NV.ndm[commSboxLabelInfo] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_IN_PK_LABEL_INFO_REQ_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[commSboxLabelInfo] != COMM_COMP)
				break;

			if (0 == strcmp(TRAY_PART_DATA[nvInPk].lotInfo.lotId, ""))
			{
				ER.Save(ER_IN_PK_DATA_PACKING_DATA_EMPTY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (0 != strcmp(TRAY_PART_DATA[nvInPk].lotInfo.packingType, "TRAY"))
			{
				ER.Save(ER_IN_PK_DATA_PACKING_TYPE_NOT_TRAY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_TRAY_PICK_UP_END);
			break;
		}
		case C_TRAY_PICK_UP_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TRAY_PICK_UP_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			Off(oSmema01Recv);
			Exist() = enExistTray;

			if (!NV.use[useInPkBandingInsp])
				Exist() = enExistTrayQc;

			NV.ndm[recvPreMcData] = COMM_IDLE;
			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CInPk::CycleBandingVi(void)
{
	if (!m_pFsm->Between(C_BANDING_VI_START, C_BANDING_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_IN_PK_BANDING_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_BANDING_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BANDING_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "INSP";

				m_nViCnt = 0;
				m_nViCntTemp = 0;
				m_nViRetryCnt = 0;
				VI[VI_IN_PK_BAND].Clear();
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!VI[VI_IN_PK_BAND].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (!IsExist())
			{
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_BANDING_VI_DATA_SEND);
			break;
		}
		case C_BANDING_VI_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BANDING_VI_DATA_SEND", m_strName);
				NV.ndm[reqViDataSendTrayPk] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_IN_PK_VI_DATA_SEND_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[reqViDataSendTrayPk] != COMM_COMP)
				break;

			m_pFsm->Set(C_BANDING_VI_MOVE);
			break;
		}
		case C_BANDING_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BANDING_VI_MOVE", m_strName);

			if (m_nViCnt != 0 && m_nViCnt >= 3)
			{
				m_pFsm->Set(C_BANDING_VI_RESULT);
				break;
			}

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_BAND_VI_1ST + m_nViCnt))
				break;

			m_pFsm->Set(C_BANDING_VI_DN);
			break;
		}
		case C_BANDING_VI_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BANDING_VI_DN", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_BAND_VI))
				break;

			m_pFsm->Set(C_BANDING_VI_RST);
			break;
		}
		case C_BANDING_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BANDING_VI_RST", m_strName);

			if (m_nViCnt == 1)
				viMode = MODE_IN_PK_BANDING_CENTER;
			else
				viMode = MODE_IN_PK_BANDING_SIDE;

			if (!VI[VI_IN_PK_BAND].IsModeOn(viMode, 10))
			{
				VI[VI_IN_PK_BAND].SetMode(viMode);
				break;
			}

			VI[VI_IN_PK_BAND].Reset();
			m_pFsm->Set(C_BANDING_VI_TRIG);
			break;
		}
		case C_BANDING_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BANDING_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_BANDING_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK_BAND].Start(viMode))
			{
				m_nViCntTemp = m_nViCnt;
				m_pFsm->Set(C_BANDING_VI_RESULT);
			}

			break;
		}
		case C_BANDING_VI_RESULT:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BANDING_VI_RESULT", m_strName);
				m_nViCntTemp++;
			}

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_BANDING_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK_BAND].IsBusy())
				break;

			if (m_nViCntTemp < 3)
			{
				if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_BAND_VI_1ST + m_nViCntTemp))
					break;
			}

			if (!VI[VI_IN_PK_BAND].m_isRcvComp)
				break;

			if (!VI[VI_IN_PK_BAND].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < 3)
				{
					m_pFsm->Set(C_BANDING_VI_MOVE);
					break;
				}

				ER.Save(ER_VISION_IN_PK_BANDING_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_nViRetryCnt = 0;
			m_nViCnt++;
			if (m_nViCnt < 3)
				m_pFsm->Set(C_BANDING_VI_MOVE);
			else
				m_pFsm->Set(C_BANDING_VI_UP);
			break;
		}
		case C_BANDING_VI_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BANDING_VI_UP", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!IsCanCyclePlace())
			{
				if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_READY))
					break;
			}

			m_pFsm->Set(C_BANDING_VI_END);
			break;
		}
		case C_BANDING_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BANDING_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			TRAY_PART_DATA[nvInPk].exist = enExistTrayQc;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CInPk::CyclePlace(void)
{
	if (!m_pFsm->Between(C_TRAY_PLACE_START, C_TRAY_PLACE_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_IN_PK_PLACE_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_TRAY_PLACE_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PLACE_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "PLACE";
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!CylInit())
				break;

			// TODO: Error Check
			if (!IsExist() || !FOAM_RAIL.IsBtmFoamAlign() || FOAM_RAIL.Exist(_BTM_FOAM_) != enExistBtmFoamQc)
			{
				if (FOAM_RAIL.Exist(_BTM_FOAM_) != enExistBtmFoamQc)
					ER.Save(ER_BTM_FOAM_TRAY_DATA_MISMATCH);

				m_pFsm->Set(C_ERROR);
				break;
			}

			// TODO: Cobot Ready Position Check.

			m_pFsm->Set(C_TRAY_PLACE_MOVE);
			break;
		}
		case C_TRAY_PLACE_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TRAY_PLACE_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_PLACE))
				break;

			m_pFsm->Set(C_TRAY_PLACE_Z_DN);
			break;
		}
		case C_TRAY_PLACE_Z_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TRAY_PLACE_Z_DN", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_PLACE_NORMAL + GetCurFoamType()))
				break;

			m_pFsm->Set(C_TRAY_PLACE_Z_UP);
			break;
		}
		case C_TRAY_PLACE_Z_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_TRAY_PLACE_Z_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commTrayTransferEnd] = COMM_REQ;
			}

			if (!CYL[cylInPkOc].IsOpen())
			{
				CYL[cylInPkOc].Open(IN_PK_OPEN);
				break;
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_READY))
				break;

			m_pFsm->Set(C_TRAY_PLACE_END);
			break;
		}
		case C_TRAY_PLACE_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_TRAY_PLACE_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			memcpy(&TRAY_PART_DATA[nvBtmFoam], &TRAY_PART_DATA[nvInPk], sizeof(IPC_MMI::TrayPartData));
			ZeroMemory(&TRAY_PART_DATA[nvInPk], sizeof(IPC_MMI::TrayPartData));
			TRAY_PART_DATA[nvBtmFoam].exist = enExistBtmFoamTray;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CInPk::CycleBtmFoamTrayVi(void)
{
	if (!m_pFsm->Between(C_BTM_FOAM_TRAY_VI_START, C_BTM_FOAM_TRAY_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_IN_PK_BTM_FOAM_TRAY_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_BTM_FOAM_TRAY_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "BTM_FOAM_TRAY_VI";

				m_nViRetryCnt = 0;
				VI[VI_IN_PK].Clear();
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!VI[VI_IN_PK].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_DATA_SEND);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_DATA_SEND", m_strName);
				NV.ndm[reqViDataSendInPkTrayVi] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_IN_PK_BTM_FOAM_TRAY_VI_DATA_SEND_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[reqViDataSendInPkTrayVi] != COMM_COMP)
				break;

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_MOVE);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_TRAY_VI))
				break;

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_DN);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_DN", m_strName);

			// Subtract the current tray quantity from the maximum tray quantity.
			double Zoffset = GetCurBtmFoamViZOffset();
			double pos = MT[mtInPkZ].PosTable(P_IN_PK_Z_TRAY_VI) + Zoffset;

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_TRAY_VI, pos))
				break;

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_RST);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_RST", m_strName);

			if (m_nViRetryCnt > 0 && !m_pFsm->TimeLimit(_1sec))
				break;

			viMode = MODE_IN_PK_BTM_FOAM_TRAY;
			if (!VI[VI_IN_PK].IsModeOn(viMode, 10))
			{
				VI[VI_IN_PK].SetMode(viMode);
				break;
			}

			VI[VI_IN_PK].Reset();
			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_TRIG);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_TOP_FOAM_TRAY_VI_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK].Start(viMode))
				m_pFsm->Set(C_BTM_FOAM_TRAY_VI_RESULT);

			break;
		}
		case C_BTM_FOAM_TRAY_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_TOP_FOAM_TRAY_VI_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK].IsBusy() || !VI[VI_IN_PK].m_isRcvComp)
				break;

			if (!VI[VI_IN_PK].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[inPkViRetryCnt])
				{
					m_pFsm->Set(C_BTM_FOAM_TRAY_VI_RST);
					break;
				}

				ER.Save(ER_VISION_IN_PK_TOP_FOAM_TRAY_VI_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_UP);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_UP:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_UP", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_READY))
				break;

			m_pFsm->Set(C_BTM_FOAM_TRAY_VI_END);
			break;
		}
		case C_BTM_FOAM_TRAY_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_BTM_FOAM_TRAY_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			TRAY_PART_DATA[nvBtmFoam].exist = enExistBtmFoamTrayQc;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}

void CInPk::CycleTopFoamVi(void)
{
	if (!m_pFsm->Between(C_LABEL_ATTACH_VI_START, C_LABEL_ATTACH_VI_END))
		return;

	if (m_pFsm->TimeLimit(_40sec))
	{
		ER.Save(ER_IN_PK_LABEL_ATTACH_VI_CYCLE_TIME_OVER);
		m_pFsm->Set(C_ERROR);
		return;
	}

	if (IsHwRdy())
		return;

	switch (m_pFsm->Get())
	{
		case C_LABEL_ATTACH_VI_START:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_START", m_strName);
				m_tmCycleTime.Reset();
				m_state = "LABEL_ATTACH_VI";

				m_nViRetryCnt = 0;
				VI[VI_IN_PK].Clear();
				NV.ndm[commLabelAttachEnd] = COMM_IDLE;
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (!VI[VI_IN_PK].IsAuto())
			{
				ER.Save(ER_VISION_NOT_AUTO);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VI_DATA_SEND);
			break;
		}
		case C_LABEL_ATTACH_VI_DATA_SEND:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_DATA_SEND", m_strName);
				NV.ndm[reqViDataSendInPkLabel] = COMM_REQ;
			}

			if (m_pFsm->TimeLimit(_10sec))
			{
				ER.Save(ER_IN_PK_LABEL_ATTACH_VI_DATA_SEND_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (NV.ndm[reqViDataSendInPkLabel] != COMM_COMP)
				break;

			m_pFsm->Set(C_LABEL_ATTACH_VI_MOVE);
			break;
		}
		case C_LABEL_ATTACH_VI_MOVE:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_MOVE", m_strName);

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_LABEL_VI))
				break;

			m_pFsm->Set(C_LABEL_ATTACH_VI_DN);
			break;
		}
		case C_LABEL_ATTACH_VI_DN:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_RST", m_strName);

			// Subtract the current tray quantity from the maximum tray quantity.
			double Zoffset = GetCurTopFoamViZOffset();
			double pos = MT[mtInPkZ].PosTable(P_IN_PK_Z_LABEL_VI) + Zoffset;

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_LABEL_VI, pos))
				break;

			m_pFsm->Set(C_LABEL_ATTACH_VI_RST);
			break;
		}
		case C_LABEL_ATTACH_VI_RST:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_RST", m_strName);

			if (m_nViRetryCnt > 0 && !m_pFsm->TimeLimit(_1sec))
				break;

			viMode = MODE_IN_PK_FOAM_LABEL_ATTACHED;
			if (!VI[VI_IN_PK].IsModeOn(viMode, 10))
			{
				VI[VI_IN_PK].SetMode(viMode);
				break;
			}

			VI[VI_IN_PK].Reset();
			m_pFsm->Set(C_LABEL_ATTACH_VI_TRIG);
			break;
		}
		case C_LABEL_ATTACH_VI_TRIG:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_TRIG", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_FOAM_LABEL_ATTACHED_NOT_READY);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK].Start(viMode))
				m_pFsm->Set(C_LABEL_ATTACH_VI_RESULT);

			break;
		}
		case C_LABEL_ATTACH_VI_RESULT:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_RESULT", m_strName);

			if (m_pFsm->TimeLimit(_5sec))
			{
				ER.Save(ER_VISION_IN_PK_FOAM_LABEL_ATTACHED_RESULT_TIME_OVER);
				m_pFsm->Set(C_ERROR);
				break;
			}

			if (VI[VI_IN_PK].IsBusy() || !VI[VI_IN_PK].m_isRcvComp)
				break;

			if (!VI[VI_IN_PK].m_qcData.result[0])
			{
				m_nViRetryCnt++;
				if (m_nViRetryCnt < NV.rcp[inPkViRetryCnt])
				{
					m_pFsm->Set(C_LABEL_ATTACH_VI_RST);
					break;
				}

				ER.Save(ER_VISION_IN_PK_FOAM_LABEL_ATTACHED_RESULT_FAIL);
				m_pFsm->Set(C_ERROR);
				break;
			}

			m_pFsm->Set(C_LABEL_ATTACH_VI_UP);
			break;
		}
		case C_LABEL_ATTACH_VI_UP:
		{
			if (m_pFsm->Once())
			{
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_UP", m_strName);
				if (NV.use[useSecsGem])
					NV.ndm[commLabelAttachEnd] = COMM_REQ;
			}

			if (MT_INPOS != MtPosMove(mtInPkZ, P_IN_PK_Z_READY))
				break;

			if (MT_INPOS != MtPosMove(mtInPkX, P_IN_PK_X_READY))
				break;

			m_pFsm->Set(C_LABEL_ATTACH_VI_END);
			break;
		}
		case C_LABEL_ATTACH_VI_END:
		{
			if (m_pFsm->Once())
				LOG[logSEQ].Message("<%s> C_LABEL_ATTACH_VI_END [%.3f]", m_strName, m_tmCycleTime.ElapsedSec());

			TRAY_PART_DATA[nvTopFoam].exist = enExistTrayTopFoamQc;

			m_state = "";
			m_pFsm->Set(C_IDLE);
			break;
		}
	}
}
