#include "..\Includes.h"

void DryRunProcess(void)
{
	if (NV.ndm[reqLabelDataSend] == 1)
	{
		NV.ndm[ltLabelPrintTest] = TRUE;
		NV.ndm[reqLabelDataSend] = FALSE;
		if (LABEL_PART_DATA[nvLabelLtFeeder].exist == enLabelExistPrs)
			LABEL_PART_DATA[nvLabelLtFeeder].exist = enLabelExist;
	}
	else if (NV.ndm[reqLabelDataSend] == 2)
	{
		NV.ndm[rtLabelPrintTest] = TRUE;
		NV.ndm[reqLabelDataSend] = FALSE;
		if (LABEL_PART_DATA[nvLabelRtFeeder].exist == enLabelExistPrs)
			LABEL_PART_DATA[nvLabelRtFeeder].exist = enLabelExist;
	}

	if (NV.ndm[ltLabelPrintTest])
	{
		if (LABEL_LASER.m_pFsm->IsBusy() || LABEL_SHUTTLE[enlabelLt].m_pFsm->IsBusy() || RT_COBOT.m_pFsm->IsBusy())
			return;

		TRAY_PART_DATA[nvTrayShuttle].exist = 9;
		TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPickup = true;
		TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPrinted = true;
		TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPickup = false;
		TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPrinted = true;

		if (COBOT_PART_DATA[nvRtCobot].existLabel)
			RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_REJECT_START);

		else if (RT_COBOT.IsCanCycleLabelPick() && LABEL_PART_DATA[nvLabelLtSuttle].exist == enLabelExistQc)
			RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_PICK_START, _LT);

		else if (LABEL_SHUTTLE[enlabelLt].IsCanCycleLabelQc())
			LABEL_SHUTTLE[enlabelLt].m_pFsm->Set(LABEL_SHUTTLE[enlabelLt].C_LABEL_QC_VI_START);

		else if (LABEL_SHUTTLE[enlabelLt].IsCanCycleLabelRecv())
			LABEL_SHUTTLE[enlabelLt].m_pFsm->Set(LABEL_SHUTTLE[enlabelLt].C_LABEL_RECV_START);

		else if (LABEL_LASER.IsCanCycleLaser())
			LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_START, _LT);

		else if (LABEL_PART_DATA[nvLabelLtFeeder].exist == enLabelExist && LABEL_LASER.IsCanCyclePrs())
			LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PRS_VI_START, _LT);
	}
	if (NV.ndm[rtLabelPrintTest])
	{
		if (LABEL_LASER.m_pFsm->IsBusy() || LABEL_SHUTTLE[enlabelRt].m_pFsm->IsBusy() || RT_COBOT.m_pFsm->IsBusy())
			return;

		TRAY_PART_DATA[nvTrayShuttle].exist = 9;
		TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPickup = true;
		TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPrinted = true;
		TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPickup = false;
		TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPrinted = true;

		if (COBOT_PART_DATA[nvRtCobot].existLabel)
			RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_REJECT_START);

		else if (RT_COBOT.IsCanCycleLabelPick() && LABEL_PART_DATA[nvLabelRtSuttle].exist == enLabelExistQc)
			RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_PICK_START, _RT);

		else if (LABEL_SHUTTLE[enlabelRt].IsCanCycleLabelQc())
			LABEL_SHUTTLE[enlabelRt].m_pFsm->Set(LABEL_SHUTTLE[enlabelRt].C_LABEL_QC_VI_START);

		else if (LABEL_SHUTTLE[enlabelRt].IsCanCycleLabelRecv())
			LABEL_SHUTTLE[enlabelRt].m_pFsm->Set(LABEL_SHUTTLE[enlabelRt].C_LABEL_RECV_START);

		else if (LABEL_LASER.IsCanCycleLaser())
			LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_START, _RT);

		else if (LABEL_PART_DATA[nvLabelRtFeeder].exist == enLabelExist && LABEL_LASER.IsCanCyclePrs())
			LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PRS_VI_START, _RT);
	}
	if (NV.ndm[simulationDebug2])
	{
		int nMtNo = mtInPkZ;
		if (MT[nMtNo].IsRdy() && MT[nMtNo].m_isRdy.TmOver(1000))
		{
			if (MT[nMtNo].InPos(49))
				MT[nMtNo].PosMove(48);
			else
				MT[nMtNo].PosMove(49);
		}
		else
		{
			if (!MT[nMtNo].m_state.isServoOn || !MT[nMtNo].m_state.isHome)
				NV.ndm[simulationDebug2] = FALSE;
		}
	}
	if (!OPR.isNoDevice && !OPR.isDryRun && !NV.use[useSecsGem] && NV.use[useStandalone])
	{
		if (AOff(iSmema01Request, _40sec) && !IN_PK.m_pFsm->Between(IN_PK.C_TRAY_PICK_UP_START, IN_PK.C_TRAY_PICK_UP_END))
			Set(iSmema01Request, true);
		else if (TRAY_PART_DATA[nvInPk].exist == enExistTray)
			Set(iSmema01Request, false);

		char temp[64];  // 임시 버퍼
		snprintf(temp, sizeof(temp), "%s%d", "LOTID", (int)NV.ndm[simulationDebug1]);
		strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotId, temp);

		if (IN_PK.m_pFsm->Get() == IN_PK.C_TRAY_PICK_UP_DATA_RECV &&
			IN_PK.m_pFsm->TimeLimit(_1sec))
		{
			if (!NV.ndm[recvPreMcData])
			{
				NV.ndm[simulationDebug1] = NV.ndm[simulationDebug0] / 4;
				NV.ndm[simulationDebug0]++;
				// LotInfo Data Set.
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotCloseChk, "N");
				//strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotId, temp);
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.partId, "K4AAG045WA-BCWEM00-HHE45L");
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.packingType, "TRAY");
				//if (NV.ndm[simulationDebug0] % 4 == 0)
				//	TRAY_PART_DATA[nvInPk].lotInfo.lotQty = 4 * (8 * 16);		// 4EA TRAY
				//else
					TRAY_PART_DATA[nvInPk].lotInfo.lotQty = 10 * (8 * 16);		// 10EA TRAY

				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotStep, "T170");
				TRAY_PART_DATA[nvInPk].lotInfo.trayArrayX = 8;
				TRAY_PART_DATA[nvInPk].lotInfo.trayArrayY = 16;
				TRAY_PART_DATA[nvInPk].lotInfo.trayThick = 5.62;
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.trayMarking, "BGA-7.5X11.0-8X16-0 C-034");
				TRAY_PART_DATA[nvInPk].lotInfo.pocketPitchX = 15.16;
				TRAY_PART_DATA[nvInPk].lotInfo.pocketPitchY = 19;
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.trayCode, "LA69-01444A");
				TRAY_PART_DATA[nvInPk].lotInfo.boxCnt = 4;
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.semiProd, "");
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.holdCode, "");
				strcpy(TRAY_PART_DATA[nvInPk].lotInfo.largeBox, "Y");

				// SBoxLabelInfo Data Set.
				char temp2[64];  // 임시 버퍼
				snprintf(temp2, sizeof(temp2), "%s_SBOX%d", TRAY_PART_DATA[nvInPk].lotInfo.lotId, (int)NV.ndm[simulationDebug0]);
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.sBoxId, temp2);
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.lotId, "lotId");
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.partNo, "partNo");
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.packingType, "packingType");
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.lotStep, "lotStep");
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.semiProd, "semiProd");
				TRAY_PART_DATA[nvInPk].labelInfo.boxCnt = 4;
				TRAY_PART_DATA[nvInPk].labelInfo.labelCnt = 4;
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.largeBox, "largeBox");
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.sBoxLabel, "sBoxLabel");
				strcpy(TRAY_PART_DATA[nvInPk].labelInfo.specialLabel, "partialLabel");
			}

			Set(iSmema02Comp, true);
			NV.ndm[recvPreMcData] = COMM_COMP;
		}

		if (IsOn(oSmema02Comp))
			Set(iSmema02Comp, false);
	}

	if (OPR.isDryRun)
	{
#pragma region Vision
		for (int nVi = 0; nVi < VI_MAX; nVi++)
		{
			Set(VI[nVi].m_iAuto, true);
			if (IsOn(VI[nVi].m_oMode1))		Set(VI[nVi].m_iMode1, true);
			else							Set(VI[nVi].m_iMode1, false);
			if (IsOn(VI[nVi].m_oMode2))		Set(VI[nVi].m_iMode2, true);
			else							Set(VI[nVi].m_iMode2, false);
			if (IsOn(VI[nVi].m_oMode3))		Set(VI[nVi].m_iMode3, true);
			else							Set(VI[nVi].m_iMode3, false);
			if (IsOn(VI[nVi].m_oMode4))		Set(VI[nVi].m_iMode4, true);
			else							Set(VI[nVi].m_iMode4, false);
			if (IsOn(VI[nVi].m_oMode5))		Set(VI[nVi].m_iMode5, true);
			else							Set(VI[nVi].m_iMode5, false);

			if (IsOn(VI[nVi].m_oStart, 50))
			{
				VI[nVi].m_prsData.data[0].dX = 0.0;
				VI[nVi].m_prsData.data[0].dY = 0.0;
				VI[nVi].m_prsData.data[0].dT = 0.0;
				VI[nVi].m_prsData.result[0] = 1;
				VI[nVi].m_qcData.result[0] = 1;
				VI[nVi].m_isRcvComp = TRUE;
				Set(VI[nVi].m_iBusy, true);
			}
			else if (IsOn(VI[nVi].m_oReset, 100))
				Set(VI[nVi].m_iBusy, true);
			else
				Set(VI[nVi].m_iBusy, false);
		}
#pragma endregion
#pragma region MMI_REQ
		for (int i = reqViDataSendTrayPk; i <= reqViDataSendMbbSealing; i++)
		{
			NDmList ndmNo = (NDmList)i;
			if (NV.ndm[ndmNo] == COMM_REQ || NV.ndm[ndmNo] == COMM_REQ + 1)
				NV.ndm[ndmNo] = COMM_COMP;
		}

		for (int i = commEqpFoamLtBoxRfidRead; i <= commEqpRtMbbStackRfidRead; i++)
		{
			NDmList ndmNo = (NDmList)i;
			if (NV.ndm[ndmNo] == COMM_REQ || NV.ndm[ndmNo] == COMM_REQ + 1)
				NV.ndm[ndmNo] = COMM_COMP;
		}
#pragma endregion

#pragma region _01_InPk
		if (IsOn(oCylInPkClose))
			Set(iCylInPkClose, true);
		else
			Set(iCylInPkClose, false);

		if (NV.use[useStandalone])
		{
			if (TRAY_PART_DATA[nvInPk].exist != enEmpty)
				IN_PK.m_tmDryRunInterval.Reset();

			if (IN_PK.m_tmDryRunInterval.TmOverSec(35) && TRAY_PART_DATA[nvInPk].exist == enEmpty)
			{
				if (NV.ndm[recvPreMcData] == COMM_REQ)
				{
					//TRAY_PART_DATA[nvInPk].exist = enExistTray;
					NV.ndm[simulationDebug1] = NV.ndm[simulationDebug0] / 4;
					NV.ndm[simulationDebug0]++;
					char temp[64];  // 임시 버퍼
					snprintf(temp, sizeof(temp), "%s%d", "LOTID", (int)NV.ndm[simulationDebug1]);
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotId, temp);
					// LotInfo Data Set.
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotCloseChk, "N");
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.partId, "K4AAG045WA-BCWEM00-HHE45L");
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.packingType, "TRAY");
					if (NV.ndm[simulationDebug0] % 4 == 0)
						TRAY_PART_DATA[nvInPk].lotInfo.lotQty = 4 * (8 * 16);		// 4EA TRAY
					else
						TRAY_PART_DATA[nvInPk].lotInfo.lotQty = 10 * (8 * 16);		// 10EA TRAY

					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.lotStep, "T170");
					TRAY_PART_DATA[nvInPk].lotInfo.trayArrayX = 8;
					TRAY_PART_DATA[nvInPk].lotInfo.trayArrayY = 16;
					TRAY_PART_DATA[nvInPk].lotInfo.trayThick = 5.62;
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.trayMarking, "BGA-7.5X11.0-8X16-0 C-034");
					TRAY_PART_DATA[nvInPk].lotInfo.pocketPitchX = 15.16;
					TRAY_PART_DATA[nvInPk].lotInfo.pocketPitchY = 19;
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.trayCode, "LA69-01444A");
					TRAY_PART_DATA[nvInPk].lotInfo.boxCnt = 4;
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.semiProd, "");
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.holdCode, "");
					strcpy(TRAY_PART_DATA[nvInPk].lotInfo.largeBox, "Y");

					// SBoxLabelInfo Data Set.
					char temp2[64];  // 임시 버퍼
					snprintf(temp2, sizeof(temp2), "%s_SBOX%d", TRAY_PART_DATA[nvInPk].lotInfo.lotId, (int)NV.ndm[simulationDebug0]);
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.sBoxId, temp2);
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.lotId, "lotId");
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.partNo, "partNo");
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.packingType, "packingType");
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.lotStep, "lotStep");
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.semiProd, "semiProd");
					TRAY_PART_DATA[nvInPk].labelInfo.boxCnt = 4;
					TRAY_PART_DATA[nvInPk].labelInfo.labelCnt = 4;
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.largeBox, "largeBox");
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.sBoxLabel, "sBoxLabel");
					strcpy(TRAY_PART_DATA[nvInPk].labelInfo.specialLabel, "partialLabel");
			
					NV.ndm[recvPreMcData] = COMM_COMP;
				}
			}
		}
		
		if (AOff(iSmema01Request, _40sec) &&
			!IN_PK.m_pFsm->Between(IN_PK.C_TRAY_PICK_UP_START, IN_PK.C_TRAY_PICK_UP_END))
			Set(iSmema01Request, true);
		else if (TRAY_PART_DATA[nvInPk].exist == enExistTray)
			Set(iSmema01Request, false);
#pragma endregion
		

		if (IsOn(oSmema02Comp))
			Set(iSmema02Comp, false);

		if (TRAY_PART_DATA[nvInPk].exist)
			Set(iInPkExist, true);
		else
			Set(iInPkExist, false);

		if (TRAY_PART_DATA[nvBtmFoam].exist)
		{
			Set(iBtmFoamExistIn, false);
			Set(iBtmFoamExistEnd, true);
		}
		else
		{
			Set(iBtmFoamExistIn, false);
			Set(iBtmFoamExistEnd, false);
		}

		if (NV.ndm[commLotDisplay] == COMM_REQ)
			NV.ndm[commLotDisplay] = COMM_COMP;

		if (NV.ndm[commLotCloseCheck] == COMM_REQ)
			NV.ndm[commLotCloseCheck] = COMM_COMP;

		if (NV.ndm[commSboxLabelInfo] == COMM_REQ)
			NV.ndm[commSboxLabelInfo] = COMM_COMP;
#pragma endregion

#pragma region _02_InRail
		//if (!FOAM_RAIL.m_pFsm->IsBusy())
		{
			if (TRAY_PART_DATA[nvTopFoam].exist)
			{
				Set(iTopFoamExistIn, false);
				Set(iTopFoamExistEnd, true);
			}
			else
			{
				Set(iTopFoamExistIn, false);
				Set(iTopFoamExistEnd, false);
			}
		}

		if (FOAM_RAIL.m_pFsm->Get() == FOAM_RAIL.C_BTM_FOAM_OPEN_START)
		{
			if (CONV[CV_FOAM_CONV].IsFwd(_1sec))
			{
				Set(iBtmFoamExistIn, false);
				Set(iBtmFoamExistEnd, true);
			}
			else
			{
				Set(iBtmFoamExistIn, true);
				Set(iBtmFoamExistEnd, false);
			}
		}

		if (FOAM_RAIL.m_pFsm->Get() == FOAM_RAIL.C_BTM_FOAM_SEND_CHECK)
		{
			if (CONV[CV_FOAM_CONV].IsFwd(_1sec))
			{
				Set(iBtmFoamExistIn, false);
				Set(iBtmFoamExistEnd, false);
			}
			if (CONV[CV_FOAM_CONV].IsFwd(_3sec))
			{
				Set(iTopFoamExistIn, false);
				Set(iTopFoamExistEnd, true);
			}
		}

		if (FOAM_RAIL.m_pFsm->Between(FOAM_RAIL.C_TOP_FOAM_SEND_CHECK, FOAM_RAIL.C_TOP_FOAM_SEND_END))
		{
			if (CONV[CV_FOAM_CONV].IsFwd(_3sec) && CONV[CV_TRAY_SHUTTLE].IsFwd(_3sec))
			{
				Set(iTrayShuttleExistIn, false);
				Set(iTrayShuttleExistEnd, true);
			}
		}
		else
		{
			if (!TRAY_PART_DATA[nvTrayShuttle].exist ||
				TRAY_SHUTTLE.m_pFsm->Between(TRAY_SHUTTLE.C_FOAM_PACK_SEND_1ST_POS_CHECK, TRAY_SHUTTLE.C_FOAM_PACK_SEND_END))
			{
				Set(iTrayShuttleExistIn, false);
				Set(iTrayShuttleExistEnd, false);
			}
			else if (TRAY_PART_DATA[nvTrayShuttle].exist)
			{
				Set(iTrayShuttleExistIn, false);
				Set(iTrayShuttleExistEnd, true);
			}
		}
#pragma endregion

#pragma region _03_FoamElev
		for (int nElev = 0; nElev < enFoamElevMax; nElev++)
		{
			if (FOAM_ELEV[nElev].m_pFsm->IsBusy())
			{
				if (FOAM_ELEV[nElev].m_pFsm->Get() == FOAM_ELEV[nElev].C_LEVEL_CHECK_Z_DN_CHECK)
				{
					if (FOAM_ELEV[nElev].m_pFsm->TimeLimit(200))
						Set(FOAM_ELEV[nElev].iElevTop, false);
				}
				else if (FOAM_ELEV[nElev].m_pFsm->Get() == FOAM_ELEV[nElev].C_LEVEL_CHECK_Z_UP_CHECK)
				{
					if (FOAM_ELEV[nElev].m_pFsm->TimeLimit(200))
					{
						if (nElev == enFoamSendElev)
						{
							if (FOAM_PART_DATA[FOAM_ELEV[nElev].m_nvFoamStack].cnt > 0 ||
								MT_INPOS == MtPosMove(FOAM_ELEV[nElev].MtZNum, P_ELEV_Z_TOP))
								Set(FOAM_ELEV[nElev].iElevTop, true);
						}
						else
						{
							if (FOAM_PART_DATA[FOAM_ELEV[nElev].m_nvFoamStack].exist ||
								MT_INPOS == MtPosMove(FOAM_ELEV[nElev].MtZNum, P_ELEV_Z_TOP))
								Set(FOAM_ELEV[nElev].iElevTop, true);
						}
					}
				}
			}
			else
			{
				if (MT[FOAM_ELEV[nElev].MtZNum].InPos(P_ELEV_Z_TOP))
					Set(FOAM_ELEV[nElev].iElevTop, true);
				else
					Set(FOAM_ELEV[nElev].iElevTop, false);
			}
		}

		if (FOAM_PART_DATA[nvFoamFlip].exist)
		{
			Set(iFoamFlipExistLt, true);
			Set(iFoamFlipExistRt, true);
		}
		else
		{
			Set(iFoamFlipExistLt, false);
			Set(iFoamFlipExistRt, false);
		}
#pragma endregion

#pragma region _04_FoamConv
		if (FOAM_STACK_CONV.C_FOAM_MOVE_BELT_RUN == FOAM_STACK_CONV.m_pFsm->Get())
		{
			if (CONV[CV_FOAM_STACK].IsFwd(500))
			{
				switch (FOAM_STACK_CONV.m_pFsm->GetMsg())
				{
					case FOAM_STACK_CONV.msgMzMoveLift1To2:
					{
						Set(iFoamRecvElevExistBtm, false);
						Set(iFoamRecvElevExistTop, false);
						Set(iFoamRrStackExistBtm, true);
						Set(iFoamRrStackExistTop, true);
						break;
					}
					case FOAM_STACK_CONV.msgMzMoveLift1To3:
					{
						Set(iFoamRecvElevExistBtm, false);
						Set(iFoamRecvElevExistTop, false);
						Set(iFoamRrStackExistBtm, false);
						Set(iFoamRrStackExistTop, false);
						Set(iFoamFtStackExistBtm, true);
						Set(iFoamFtStackExistTop, true);
						break;
					}
					case FOAM_STACK_CONV.msgMzMoveLift1To4:
					{
						Set(iFoamRecvElevExistBtm, false);
						Set(iFoamRecvElevExistTop, false);
						Set(iFoamRrStackExistBtm, false);
						Set(iFoamRrStackExistTop, false);
						Set(iFoamFtStackExistBtm, false);
						Set(iFoamFtStackExistTop, false);
						Set(iFoamSendElevExistBtm, true);
						Set(iFoamSendElevExistTop, true);
						break;
					}
					case FOAM_STACK_CONV.msgMzMoveLift2To3:
					{
						Set(iFoamRrStackExistBtm, false);
						Set(iFoamRrStackExistTop, false);
						Set(iFoamFtStackExistBtm, true);
						Set(iFoamFtStackExistTop, true);
						break;
					}
					case FOAM_STACK_CONV.msgMzMoveLift2To4:
					{
						Set(iFoamRrStackExistBtm, false);
						Set(iFoamRrStackExistTop, false);
						Set(iFoamFtStackExistBtm, false);
						Set(iFoamFtStackExistTop, false);
						Set(iFoamSendElevExistBtm, true);
						Set(iFoamSendElevExistTop, true);
						break;
					}
					case FOAM_STACK_CONV.msgMzMoveLift3To4:
					{
						Set(iFoamFtStackExistBtm, false);
						Set(iFoamFtStackExistTop, false);
						Set(iFoamSendElevExistBtm, true);
						Set(iFoamSendElevExistTop, true);
						break;
					}
				}
			}
		}
		else
		{
			if (FOAM_STACK_CONV.Exist2())
			{
				Set(iFoamRrStackExistBtm, true);
				Set(iFoamRrStackExistTop, true);
			}
			else
			{
				Set(iFoamRrStackExistBtm, false);
				Set(iFoamRrStackExistTop, false);
			}

			if (FOAM_STACK_CONV.Exist3())
			{
				Set(iFoamFtStackExistBtm, true);
				Set(iFoamFtStackExistTop, true);
			}
			else
			{
				Set(iFoamFtStackExistBtm, false);
				Set(iFoamFtStackExistTop, false);
			}
		}
#pragma endregion

#pragma region _05_FoamPk

#pragma endregion

#pragma region _06_FoamBox
		for (int nFoamBox = 0; nFoamBox < enFoamBoxMax; nFoamBox++)
		{
			if (FOAM_CST[nFoamBox].Exist())
			{
				Set(FOAM_CST[nFoamBox].iFoamBoxFoamExist, true);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectLtFt, true);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectRtFt, true);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectLtRr, true);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectRtRr, true);
			}
			else
			{
				Set(FOAM_CST[nFoamBox].iFoamBoxFoamExist, false);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectLtFt, false);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectRtFt, false);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectLtRr, false);
				Set(FOAM_CST[nFoamBox].iFoamBoxDetectRtRr, false);
			}

			// VALID: VALID가 ON되면 광통신이 시작된다. COMPLETE OFF되면 OFF됨.
			if ((FOAM_CST[nFoamBox].Exist() && FOAM_PART_DATA[nvFoamLtCst + nFoamBox].flag.cstMaterialEmpty && FOAM_PART_DATA[nvFoamLtCst + nFoamBox].flag.amrUnloadCallDid) ||
				(!FOAM_CST[nFoamBox].Exist() && FOAM_PART_DATA[nvFoamLtCst + nFoamBox].flag.amrLoadCallDid))
				Set(FOAM_CST[nFoamBox].iPioValId, true);
			else
			{
				if (AOff(FOAM_CST[nFoamBox].iPioComp))
					Set(FOAM_CST[nFoamBox].iPioValId, false);
			}

			// CS_0~3 : 작업할 설비의 PORT를 나타낸다. 전부 OFF되거나 두 개 이상 ON되면 에러처리
			// VALID가 ON되면 유효하다. COMPLETE OFF되면 OFF됨.
			if (AOn(FOAM_CST[nFoamBox].iPioValId))
			{
				Set(FOAM_CST[nFoamBox].iPioCs0, true);
				Set(FOAM_CST[nFoamBox].iPioCs1, false);
				Set(FOAM_CST[nFoamBox].iPioCs2, false);
				Set(FOAM_CST[nFoamBox].iPioCs3, false);
			}
			else
			{
				if (AOff(FOAM_CST[nFoamBox].iPioComp))
				{
					Set(FOAM_CST[nFoamBox].iPioCs0, false);
					Set(FOAM_CST[nFoamBox].iPioCs1, false);
					Set(FOAM_CST[nFoamBox].iPioCs2, false);
					Set(FOAM_CST[nFoamBox].iPioCs3, false);
				}
			}

			// TR_REQ : 설비에서 L_REQ, U_REQ가 ON되면 ON된다. BUSY가 OFF된 후 OFF된다.
			if (IsOn(FOAM_CST[nFoamBox].oPioLdReq) || IsOn(FOAM_CST[nFoamBox].oPioUldReq))
				Set(FOAM_CST[nFoamBox].iPioTrReq, true);
			else
			{
				if (AOff(FOAM_CST[nFoamBox].iPioBusy))
					Set(FOAM_CST[nFoamBox].iPioTrReq, false);
			}

			// BUSY : 설비의 READY가 ON되면 ON된다. 실제 작업을 하는 동안 ON이다. ROBOT 동작이 완료된 후 OFF된다.
			if (IsOn(FOAM_CST[nFoamBox].oPioRdy) && (IsOn(FOAM_CST[nFoamBox].oPioLdReq) || IsOn(FOAM_CST[nFoamBox].oPioUldReq)))
				Set(FOAM_CST[nFoamBox].iPioBusy, true);
			else
			{
				if (IsOff(FOAM_CST[nFoamBox].oPioUldReq, 1000))
					Set(FOAM_CST[nFoamBox].iPioBusy, false);
			}

			// Busy작업 후 5초후 Sensor On or Off를 위함
			if (AOn(FOAM_CST[nFoamBox].iPioBusy, 5000))
			{
				if (IsOn(FOAM_CST[nFoamBox].oPioUldReq))
					FOAM_CST[nFoamBox].Exist() = FALSE;
				if (IsOn(FOAM_CST[nFoamBox].oPioLdReq))
					FOAM_CST[nFoamBox].Exist() = TRUE;
			}

			// COMPLETE : BUSY가 OFF된후 ON된다. 설비의 READY가 OFF된 후 OFF된다.
			if (AOff(FOAM_CST[nFoamBox].iPioBusy) && IsOn(FOAM_CST[nFoamBox].oPioRdy))
				Set(FOAM_CST[nFoamBox].iPioComp, true);
			else
				Set(FOAM_CST[nFoamBox].iPioComp, false);
		}


#pragma endregion

#pragma region _07_TrayShuttle
		
#pragma endregion


#pragma region _08_MBBShuttle
		Set(iBSealcontactDetected, true);

		//if (!MBB_SHUTTLE.m_pFsm->IsBusy())
		{
			if (MBB_SHUTTLE.Exist())
				Set(iMbbShuttleConvExist, true);
			else
				Set(iMbbShuttleConvExist, false);
		}

		if (MBB_SHUTTLE.m_pFsm->Get() == MBB_SHUTTLE.C_MBB_SEALING_WORK)
			SEAL[sealMbb].m_pReadData->machineStatus = 4;
		else if (MBB_SHUTTLE.m_pFsm->Get() == MBB_SHUTTLE.C_MBB_SEALING_RESULT)
		{
			SEAL[sealMbb].m_pReadData->machineStatus = 3;
			SEAL[sealMbb].m_pReadData->ioStatus |= SEAL[sealMbb].BIT_MASK[enInBad];
		}

		if (MBB_SHUTTLE.m_pFsm->Between(MBB_SHUTTLE.C_MBB_OPEN_MBB_ALIGN_CHECK, MBB_SHUTTLE.C_MBB_OPEN_CLAMP))
		{
			if (MBB_SHUTTLE.m_pFsm->Get() == MBB_SHUTTLE.C_MBB_OPEN_MBB_ALIGN_CHECK)
			{
				if ((!OPR.isNoDevice || MT[mtMbbShuttleConv].m_state.isDriving) &&
					MBB_SHUTTLE.m_pFsm->TimeLimit(_1sec))
				{
					Set(iMbbOpenBtmBlockExistFt, true);
					Set(iMbbOpenBtmBlockExistRr, true);
				}
			}
		}
		else
		{
			Set(iMbbOpenBtmBlockExistFt, false);
			Set(iMbbOpenBtmBlockExistRr, false);
		}
		
		if (MBB_SHUTTLE.m_pFsm->Between(MBB_SHUTTLE.C_MBB_PACK_SEND_CHECK, MBB_SHUTTLE.C_MBB_PACK_SEND_END))
		{
			if (MBB_SHUTTLE.m_pFsm->TimeLimit(_3sec))
			{
				Set(iQcConvExistIn, false);
				Set(iQcConvExistEnd, true);
			}
		}
		else
		{
			if (MBB_QC.m_pFsm->Between(MBB_QC.C_MBB_QC_SEND_CHECK, MBB_QC.C_MBB_QC_SEND_END))
			{
				if (CONV[CV_QC].IsFwd(_1sec) && CONV[CV_MBB_FOLD].IsFwd(_1sec))
				{
					Set(iQcConvExistIn, false);
					Set(iQcConvExistEnd, false);
					Set(iMbbFoldConvExist, false);
				}
				if (CONV[CV_QC].IsFwd(_3sec) && CONV[CV_MBB_FOLD].IsFwd(_3sec))
				{
					Set(iMbbFoldConvExist, true);
				}
			}
			else
			{
				if (FOLD_PK.m_pFsm->Between(FOLD_PK.C_PICK_UP_Z_SLOW_UP, FOLD_PK.C_PICK_UP_END))
				{
					Set(iMbbFoldConvExist, false);
				}
				else
				{
					if (TRAY_PART_DATA[nvMbbFold].exist)
						Set(iMbbFoldConvExist, true);
					else
						Set(iMbbFoldConvExist, false);
				}
				
				//if (!MBB_QC.m_pFsm->IsBusy() && !FOLD_PK.m_pFsm->IsBusy())
				{
					if (MBB_QC.Exist())
					{
						Set(iQcConvExistIn, false);
						Set(iQcConvExistEnd, true);
					}
					else
					{
						Set(iQcConvExistIn, false);
						Set(iQcConvExistEnd, false);
					}

				}
			}
		}

#pragma endregion

#pragma region _10_BtmFoldConv

#pragma endregion

#pragma region _11_FoldPk
		if (FOLD_PK.Exist())
		{
			Set(iMbbFoldPkLtExist, true);
			Set(iMbbFoldPkRtExist, true);
		}
		else
		{
			Set(iMbbFoldPkLtExist, false);
			Set(iMbbFoldPkRtExist, false);
		}

		if (FOLD_PK.m_pFsm->Get() == FOLD_PK.C_PLACE_DATA_SEND)
		{
			if (FOLD_PK.m_pFsm->TimeLimit(_1sec))
				NV.ndm[reqNextMcDataSend] = COMM_COMP;
		}
		
		if (IsOn(oSmema11Req))
			Set(iSmema11Receive, true);
		else
			Set(iSmema11Receive, false);

		if (IsOn(oSmema12Comp))
			Set(iSmema12Comp, true);
		else
			Set(iSmema12Comp, false);

		Set(iSmema13Safety, true);
		Set(iSmema14Auto, true);

#pragma endregion

#pragma region _12_LabelFeeder
		for (int nLabel = 0; nLabel < enlabelMax; nLabel++)
		{
			//LABEL_FEEDER[nLabel].ExistCst() = true;
			if (LABEL_PART_DATA[LABEL_FEEDER[nLabel].nvLabelFeeder].flag.existCst)
			{
				Set(LABEL_FEEDER[nLabel].iLabelCstLabelExist, true);
				Set(LABEL_FEEDER[nLabel].iLabelCstDetectFt, true);
				Set(LABEL_FEEDER[nLabel].iLabelCstDetectRr, true);
			}
			else
			{
				Set(LABEL_FEEDER[nLabel].iLabelCstLabelExist, false);
				Set(LABEL_FEEDER[nLabel].iLabelCstDetectFt, false);
				Set(LABEL_FEEDER[nLabel].iLabelCstDetectRr, false);
			}

			// VALID: VALID가 ON되면 광통신이 시작된다. COMPLETE OFF되면 OFF됨.
			if ((LABEL_FEEDER[nLabel].ExistCst() && LABEL_PART_DATA[nvLabelLtFeeder + nLabel].flag.cstMaterialEmpty && LABEL_PART_DATA[nvLabelLtFeeder + nLabel].flag.amrUnloadCallDid) ||
				(!LABEL_FEEDER[nLabel].ExistCst() && LABEL_PART_DATA[nvLabelLtFeeder + nLabel].flag.amrLoadCallDid))
				Set(LABEL_FEEDER[nLabel].iPioValId, true);
			else
			{
				if (AOff(LABEL_FEEDER[nLabel].iPioComp))
					Set(LABEL_FEEDER[nLabel].iPioValId, false);
			}

			// CS_0~3 : 작업할 설비의 PORT를 나타낸다. 전부 OFF되거나 두 개 이상 ON되면 에러처리
			// VALID가 ON되면 유효하다. COMPLETE OFF되면 OFF됨.
			if (AOn(LABEL_FEEDER[nLabel].iPioValId))
			{
				Set(LABEL_FEEDER[nLabel].iPioCs0, true);
				Set(LABEL_FEEDER[nLabel].iPioCs1, false);
				Set(LABEL_FEEDER[nLabel].iPioCs2, false);
				Set(LABEL_FEEDER[nLabel].iPioCs3, false);
			}
			else
			{
				if (AOff(LABEL_FEEDER[nLabel].iPioComp))
				{
					Set(LABEL_FEEDER[nLabel].iPioCs0, false);
					Set(LABEL_FEEDER[nLabel].iPioCs1, false);
					Set(LABEL_FEEDER[nLabel].iPioCs2, false);
					Set(LABEL_FEEDER[nLabel].iPioCs3, false);
				}
			}

			// TR_REQ : 설비에서 L_REQ, U_REQ가 ON되면 ON된다. BUSY가 OFF된 후 OFF된다.
			if (IsOn(LABEL_FEEDER[nLabel].oPioLdReq) || IsOn(LABEL_FEEDER[nLabel].oPioUldReq))
				Set(LABEL_FEEDER[nLabel].iPioTrReq, true);
			else
			{
				if (AOff(LABEL_FEEDER[nLabel].iPioBusy))
					Set(LABEL_FEEDER[nLabel].iPioTrReq, false);
			}

			// BUSY : 설비의 READY가 ON되면 ON된다. 실제 작업을 하는 동안 ON이다. ROBOT 동작이 완료된 후 OFF된다.
			if (IsOn(LABEL_FEEDER[nLabel].oPioRdy) && (IsOn(LABEL_FEEDER[nLabel].oPioLdReq) || IsOn(LABEL_FEEDER[nLabel].oPioUldReq)))
				Set(LABEL_FEEDER[nLabel].iPioBusy, true);
			else
			{
				if (IsOff(LABEL_FEEDER[nLabel].oPioUldReq, 1000))
					Set(LABEL_FEEDER[nLabel].iPioBusy, false);
			}

			// Busy작업 후 5초후 Sensor On or Off를 위함
			if (AOn(LABEL_FEEDER[nLabel].iPioBusy, 5000))
			{
				if (IsOn(LABEL_FEEDER[nLabel].oPioUldReq))
					LABEL_FEEDER[nLabel].ExistCst() = false;
				if (IsOn(LABEL_FEEDER[nLabel].oPioLdReq))
					LABEL_FEEDER[nLabel].ExistCst() = true;
			}

			// COMPLETE : BUSY가 OFF된후 ON된다. 설비의 READY가 OFF된 후 OFF된다.
			if (AOff(LABEL_FEEDER[nLabel].iPioBusy) && IsOn(LABEL_FEEDER[nLabel].oPioRdy))
				Set(LABEL_FEEDER[nLabel].iPioComp, true);
			else
				Set(LABEL_FEEDER[nLabel].iPioComp, false);
		}

		if (LABEL_FEEDER[enlabelLt].m_pFsm->Get() == LABEL_FEEDER[enlabelLt].C_LABEL_FEED_SENSOR_CHECK ||
			LABEL_FEEDER[enlabelRt].m_pFsm->Get() == LABEL_FEEDER[enlabelRt].C_LABEL_FEED_SENSOR_CHECK ||
			LABEL_SHUTTLE[enlabelLt].m_pFsm->Get() == LABEL_SHUTTLE[enlabelLt].C_LABEL_RECV_SENSOR_CHECK ||
			LABEL_SHUTTLE[enlabelRt].m_pFsm->Get() == LABEL_SHUTTLE[enlabelRt].C_LABEL_RECV_SENSOR_CHECK)
		{
			if (LABEL_FEEDER[enlabelLt].m_pFsm->TimeLimit(_3sec) ||
				LABEL_FEEDER[enlabelRt].m_pFsm->TimeLimit(_3sec) ||
				LABEL_SHUTTLE[enlabelLt].m_pFsm->TimeLimit(_3sec) ||
				LABEL_SHUTTLE[enlabelRt].m_pFsm->TimeLimit(_3sec))
				Set(iLaserLabelPos, false);
		}
		else
			Set(iLaserLabelPos, true);


#pragma endregion

#pragma region _13_LabelLaser
		LAS[LASER].m_isReadData = true;
#pragma endregion

#pragma region _14_LabelShuttle
		for (int nLabel = 0; nLabel < enlabelMax; nLabel++)
		{
			if (LABEL_PART_DATA[nvLabelLtSuttle + nLabel].exist ||
				LABEL_SHUTTLE[nLabel].m_pFsm->Between(CLabelShuttle::C_LABEL_RECV_UP, CLabelShuttle::C_LABEL_RECV_END))
				Set(LABEL_SHUTTLE[nLabel].iLabelRecvExist, true);
			else
				Set(LABEL_SHUTTLE[nLabel].iLabelRecvExist, false);
		}

#pragma endregion

#pragma region _15_MBBStack
		for (int nMbbBox = 0; nMbbBox < enMbbCstMax; nMbbBox++)
		{
			if (MBB_PART_DATA[nMbbBox].cnt >= NV.rcp[mbbMaxCount])
				MBB_PART_DATA[nMbbBox].flag.cstMaterialEmpty = true;

			if (MBB_STACK[nMbbBox].ExistCst())
			{
				Set(MBB_STACK[nMbbBox].iMbbCstExist, true);
				Set(MBB_STACK[nMbbBox].iMbbCstDetectLt, true);
				Set(MBB_STACK[nMbbBox].iMbbCstDetectRt, true);
			}
			else
			{
				Set(MBB_STACK[nMbbBox].iMbbCstExist, false);
				Set(MBB_STACK[nMbbBox].iMbbCstDetectLt, false);
				Set(MBB_STACK[nMbbBox].iMbbCstDetectRt, false);
			}

			if (MBB_STACK[nMbbBox].m_pFsm->IsBusy())
			{
				if (MBB_STACK[nMbbBox].m_pFsm->Get() == MBB_STACK[nMbbBox].C_LEVEL_CHECK_Z_DN_CHECK)
				{
					if (MBB_STACK[nMbbBox].m_pFsm->TimeLimit(200))
					{
						Set(MBB_STACK[nMbbBox].iMbbCstMidLevel, false);
						Set(MBB_STACK[nMbbBox].iMbbCstRrLevel, false);
					}
				}
				else if (MBB_STACK[nMbbBox].m_pFsm->Get() == MBB_STACK[nMbbBox].C_LEVEL_CHECK_Z_UP_CHECK)
				{
					if (MBB_STACK[nMbbBox].m_pFsm->TimeLimit(200))
					{
						Set(MBB_STACK[nMbbBox].iMbbCstMidLevel, true);
						Set(MBB_STACK[nMbbBox].iMbbCstRrLevel, true);
					}
				}
			}
			else
			{
				if (MT[MBB_STACK[nMbbBox].MtZNum].InPos(P_ELEV_Z_TOP))
				{
					Set(MBB_STACK[nMbbBox].iMbbCstMidLevel, true);
					Set(MBB_STACK[nMbbBox].iMbbCstRrLevel, true);
				}
				else
				{
					Set(MBB_STACK[nMbbBox].iMbbCstMidLevel, false);
					Set(MBB_STACK[nMbbBox].iMbbCstRrLevel, false);
				}
			}


			// VALID: VALID가 ON되면 광통신이 시작된다. COMPLETE OFF되면 OFF됨.
			if ((MBB_STACK[nMbbBox].ExistCst() && MBB_PART_DATA[nvLtMbbStack + nMbbBox].flag.cstMaterialEmpty && MBB_PART_DATA[nvLtMbbStack + nMbbBox].flag.amrUnloadCallDid) ||
				(!MBB_STACK[nMbbBox].ExistCst() && MBB_PART_DATA[nvLtMbbStack + nMbbBox].flag.amrLoadCallDid))
				Set(MBB_STACK[nMbbBox].iPioValId, true);
			else
			{
				if (AOff(MBB_STACK[nMbbBox].iPioComp))
					Set(MBB_STACK[nMbbBox].iPioValId, false);
			}

			// CS_0~3 : 작업할 설비의 PORT를 나타낸다. 전부 OFF되거나 두 개 이상 ON되면 에러처리
			// VALID가 ON되면 유효하다. COMPLETE OFF되면 OFF됨.
			if (AOn(MBB_STACK[nMbbBox].iPioValId))
			{
				Set(MBB_STACK[nMbbBox].iPioCs0, true);
				Set(MBB_STACK[nMbbBox].iPioCs1, false);
				Set(MBB_STACK[nMbbBox].iPioCs2, false);
				Set(MBB_STACK[nMbbBox].iPioCs3, false);
			}
			else
			{
				if (AOff(MBB_STACK[nMbbBox].iPioComp))
				{
					Set(MBB_STACK[nMbbBox].iPioCs0, false);
					Set(MBB_STACK[nMbbBox].iPioCs1, false);
					Set(MBB_STACK[nMbbBox].iPioCs2, false);
					Set(MBB_STACK[nMbbBox].iPioCs3, false);
				}
			}

			// TR_REQ : 설비에서 L_REQ, U_REQ가 ON되면 ON된다. BUSY가 OFF된 후 OFF된다.
			if (IsOn(MBB_STACK[nMbbBox].oPioLdReq) || IsOn(MBB_STACK[nMbbBox].oPioUldReq))
				Set(MBB_STACK[nMbbBox].iPioTrReq, true);
			else
			{
				if (AOff(MBB_STACK[nMbbBox].iPioBusy))
					Set(MBB_STACK[nMbbBox].iPioTrReq, false);
			}

			// BUSY : 설비의 READY가 ON되면 ON된다. 실제 작업을 하는 동안 ON이다. ROBOT 동작이 완료된 후 OFF된다.
			if (IsOn(MBB_STACK[nMbbBox].oPioRdy) && (IsOn(MBB_STACK[nMbbBox].oPioLdReq) || IsOn(MBB_STACK[nMbbBox].oPioUldReq)))
				Set(MBB_STACK[nMbbBox].iPioBusy, true);
			else
			{
				if (IsOff(MBB_STACK[nMbbBox].oPioUldReq, 1000))
					Set(MBB_STACK[nMbbBox].iPioBusy, false);
			}

			// Busy작업 후 5초후 Sensor On or Off를 위함
			if (AOn(MBB_STACK[nMbbBox].iPioBusy, 5000))
			{
				if (IsOn(MBB_STACK[nMbbBox].oPioUldReq))
					MBB_STACK[nMbbBox].ExistCst() = false;
				if (IsOn(MBB_STACK[nMbbBox].oPioLdReq))
					MBB_STACK[nMbbBox].ExistCst() = true;
			}

			// COMPLETE : BUSY가 OFF된후 ON된다. 설비의 READY가 OFF된 후 OFF된다.
			if (AOff(MBB_STACK[nMbbBox].iPioBusy) && IsOn(MBB_STACK[nMbbBox].oPioRdy))
				Set(MBB_STACK[nMbbBox].iPioComp, true);
			else
				Set(MBB_STACK[nMbbBox].iPioComp, false);
		}
#pragma endregion

#pragma region _16_LtCoBot

#pragma endregion

#pragma region _17_RtCoBot

#pragma endregion
	}

	if (OPR.isNoDevice)
	{
		for (int cyno = 0; cyno < cylinderMAX; cyno++)
			CYL[CylinderList(cyno)].Simulation();

		Set(iBEmo01, true);
		Set(iBEmo02, true);
		Set(iBEmo03, true);
		Set(iBEmo04, true);
		Set(iBEmo05, true);
		Set(iBEmo06, true);
		Set(iBEmo07, true);
		Set(iBEmo08, true);
		Set(iBEmo09, true);

		Set(iBFoamSafetyLightCurtain, true);
		Set(iBMbbSafetyLightCurtain, true);

		for (int nDoorNo = iDoorCloseBtm01; nDoorNo <= iDoorCloseBtm03; nDoorNo++)
			Set((enDI)nDoorNo, true);
		for (int nDoorNo = iDoorCloseBtm04; nDoorNo <= iDoorCloseBtm10; nDoorNo++)
			Set((enDI)nDoorNo, true);

		for (int nFireNo = 0; nFireNo < nvFireDetectorMax; nFireNo++)
			FIRE_DETECTOR_DATA[nFireNo].isRun = true;

		// Cobot Part
		ROBOT[robotLt].m_robotState.activated = TRUE;
		ROBOT[robotLt].m_robotState.realMode = TRUE;
		ROBOT[robotLt].m_robotState.programRun = TRUE;
		ROBOT[robotLt].m_isHome = TRUE;

		ROBOT[robotRt].m_robotState.activated = TRUE;
		ROBOT[robotRt].m_robotState.realMode = TRUE;
		ROBOT[robotRt].m_robotState.programRun = TRUE;
		ROBOT[robotRt].m_isHome = TRUE;

		// Sealing Part
		for (int nSealNo = 0; nSealNo < SEAL_MAX; nSealNo++)
		{
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutAbort))
			{
				SEAL[nSealNo].SetInReady(false);
				SEAL[nSealNo].SetInError(false);
				SEAL[nSealNo].SetInBad(true);
			}
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutStart))
			{
				if (SEAL[nSealNo].m_tmCtrlFlag[enOutStart].TmOver(3000))
				{
					SEAL[nSealNo].SetInReady(true);
					SEAL[nSealNo].m_pReadData->machineStatus = 4;
				}
				else
				{
					SEAL[nSealNo].SetInReady(false);
					SEAL[nSealNo].SetInError(true);
					SEAL[nSealNo].SetInGood(true);
				}
			}
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutCalib))
			{
				SEAL[nSealNo].SetInReady(false);
				SEAL[nSealNo].SetInError(true);
			}
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutShakeOff))
			{
				SEAL[nSealNo].SetInReady(false);
				SEAL[nSealNo].SetInError(true);
			}
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutInit))
			{
				SEAL[nSealNo].SetInReady(false);
				SEAL[nSealNo].SetInError(true);
				SEAL[nSealNo].SetInGood(true);
			}
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutStop))
			{
				SEAL[nSealNo].SetInReady(true);
				SEAL[nSealNo].SetInError(true);
			}
			if (SEAL[nSealNo].IsSetCtrlFlagOn(enOutQuit))
			{
				SEAL[nSealNo].SetInReady(true);
				SEAL[nSealNo].SetInError(false);
				SEAL[nSealNo].SetInBad(false);
				SEAL[nSealNo].m_pReadData->machineStatus = 3;
			}
		}

	}
}
