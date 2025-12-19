#include "..\Includes.h"

namespace IPC_MMI
{
	BOOL CIPC::Init(void)
	{
		if (MAX_IO_CHANNEL != MAX_CH)
			return (FALSE);

		int nSize = sizeof(IpcBuffer);
		if (FALSE == m_kamelas.Run(L"MMI2SEQ", nSize, TRUE))
			return (FALSE);

		return (TRUE);
	}

	void CIPC::Comm(void)
	{
		UINT32 nSize = 0;
		static IpcBuffer mmiToSeq;
		if (FALSE == m_kamelas.Recv((PBYTE)&mmiToSeq, 0, nSize))
			return;

		switch (mmiToSeq.command)
		{
			case CMD_RD_IO:
			{
				nSize = sizeof(ReadIO) + 4;
				for (int i = 0; i < MAX_CH; i++)
					memcpy(&mmiToSeq.buffer.rdIO.inCH[i], WMX.m_di[i].dataAddress, sizeof(BYTE) * 2);

				for (int i = 0; i < MAX_CH; i++)
					memcpy(&mmiToSeq.buffer.rdIO.outCH[i], WMX.m_do[i].dataAddress, sizeof(BYTE) * 2);
			}
			break;
			case CMD_WR_IO:
			{
				nSize = 0;
				if (!WMX.isConnected)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_IO],[WMX Communication is Not Stable!]");
					break;
				}

				if (!OPR.isDebugMode)
				{
					if (!OPR.isStop)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO] Machine is Not Stop!");
						break;
					}
					if (OPR.isPaused)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO],[Machine is Paused Stop!]");
						break;
					}
					if (OPR.isCycleRun)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO],[Machine is Cycle Running!]");
						break;
					}
				}

				enDO oNo = (enDO)mmiToSeq.buffer.wrIO.nIONo;
				if (Between(oNo, 0, ((doLAST / 100) * 100)) + 15)
				{
					Set(oNo, mmiToSeq.buffer.wrIO.bOn);

					int chIdx = (oNo / 100);
					int bit16 = ((oNo % 100) % MAX_CONTACT);

					if (mmiToSeq.buffer.wrIO.bOn)
						LOG[logMMI].Message("[CMD_WR_IO],[output],[%.4d],[on]", oNo);
					else
						LOG[logMMI].Message("[CMD_WR_IO],[output],[%.4d],[off]", oNo);
				}
			}
			break;
			case CMD_RD_PKG:
			{
				nSize = sizeof(DoubleData) + 4;
				double* pOrigin = &NV.pkg[(PkgList)0];
				double* pTarget = &mmiToSeq.buffer.pkg.dVal[0];
				memcpy(pTarget, pOrigin, sizeof(double) * 1000);
			}
			break;
			case CMD_WR_PKG:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_PKG] Machine is Not Stop!");
					break;
				}
				if (OPR.isPaused)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_PKG] Machine is Paused Stop!");
					break;
				}

				int nStart = mmiToSeq.buffer.pkg.nStart;
				int nEnd = mmiToSeq.buffer.pkg.nEnd;

				if (999 < nEnd)
					nEnd = 999;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (999 < nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_PKG],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					if ((round(NV.pkg[(PkgList)i] * 1000.) / 1000.) != (round(mmiToSeq.buffer.pkg.dVal[i] * 1000.) / 1000.))
					{
						JustBeforePkgValueChanges(i);
						LOG[logMMI].Message("[CMD_WR_PKG],[PkgList],[%3d],[Old Value],[%.3f],[New Value],[%.3f]",
							i, round(NV.pkg[(PkgList)i] * 1000.) / 1000., round(mmiToSeq.buffer.pkg.dVal[i] * 1000.) / 1000.);
					}
					NV.pkg[(PkgList)i] = mmiToSeq.buffer.pkg.dVal[i];
				}
				OPR.isCheckPkg = (TRUE);
				NV_MEMORY.Flush();
			}
			break;
			case CMD_RD_NDM:
			{
				nSize = sizeof(IntData) + 4;
				int* pOrigin = &NV.ndm[(NDmList)0];
				int* pTarget = &mmiToSeq.buffer.ndm.nVal[0];
				memcpy(pTarget, pOrigin, sizeof(int) * 1000);
			}
			break;
			case CMD_WR_NDM:
			{
				nSize = 0;
				int nStart = mmiToSeq.buffer.ndm.nStart;
				int nEnd = mmiToSeq.buffer.ndm.nEnd;

				if (999 < nEnd)
					nEnd = 999;

				BOOL bErr = (nStart < 0) || (nEnd < nStart) || (999 < nEnd);
				if (bErr)
				{
					LOG[logMMI].Message("[CMD_WR_NDM],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					if (NV.ndm[(NDmList)i] != mmiToSeq.buffer.ndm.nVal[i])
					{
						JustBeforeNdmValueChanges(i);
						LOG[logMMI].Message("[CMD_WR_NDM],[NDmList],[%3d],[Old Value],[%3d],[New Value],[%3d]",
							i, NV.ndm[(NDmList)i], mmiToSeq.buffer.ndm.nVal[i]);
					}
					NV.ndm[(NDmList)i] = mmiToSeq.buffer.ndm.nVal[i];
				}
			}
			break;
			case CMD_RD_DDM:
			{
				nSize = sizeof(DoubleData) + 4;
				double* pOrigin = &NV.ddm[(DDmList)0];
				double* pTarget = &mmiToSeq.buffer.ddm.dVal[0];
				memcpy(pTarget, pOrigin, sizeof(double) * 1000);
			}
			break;
			case CMD_WR_DDM:
			{
				nSize = 0;
				int nStart = mmiToSeq.buffer.ddm.nStart;
				int nEnd = mmiToSeq.buffer.ddm.nEnd;

				if (999 < nEnd)
					nEnd = 999;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (999 < nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_DDM],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					if ((round(NV.ddm[(DDmList)i] * 1000.) / 1000.) != (round(mmiToSeq.buffer.ddm.dVal[i] * 1000.) / 1000.))
					{
						JustBeforeDdmValueChanges(i);
						LOG[logMMI].Message("[CMD_WR_DDM],[DDmList],[%3d],[Old Value],[%.3f],[New Value],[%.3f]",
							i, round(NV.ddm[(DDmList)i] * 1000.) / 1000., round(mmiToSeq.buffer.ddm.dVal[i] * 1000.) / 1000.);
					}
					NV.ddm[(DDmList)i] = mmiToSeq.buffer.ddm.dVal[i];
				}
			}
			break;
			case CMD_RD_USE:
			{
				nSize = sizeof(IntData) + 4;
				int* pOrigin = &NV.use[(UseList)0];
				int* pTarget = &mmiToSeq.buffer.use.nVal[0];
				memcpy(pTarget, pOrigin, sizeof(int) * 1000);
			}
			break;
			case CMD_WR_USE:
			{
				nSize = 0;
				int nStart = mmiToSeq.buffer.use.nStart;
				int nEnd = mmiToSeq.buffer.use.nEnd;

				if (499 < nEnd)
					nEnd = 499;

				BOOL bErr = (nStart < 0) || (nEnd < nStart) || (999 < nEnd);
				if (bErr)
				{
					LOG[logMMI].Message("[CMD_WR_USE],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					if (NV.use[(UseList)i] != mmiToSeq.buffer.use.nVal[i])
					{
						JustBeforeUseValueChanges(i);
						LOG[logMMI].Message("[CMD_WR_USE],[UseList],[%3d],[Old Value],[%3d],[New Value],[%3d]",
							i, NV.use[(UseList)i], mmiToSeq.buffer.use.nVal[i]);
					}
					NV.use[(UseList)i] = mmiToSeq.buffer.use.nVal[i];
				}
				NV_MEMORY.Flush();
			}
			break;
			case CMD_RD_MT_STATUS:
			{
				nSize = sizeof(MtStatus) + 4;
				AxisList nMtNo = (AxisList)mmiToSeq.buffer.mtStatus.nMtNo;

				if (!Between(nMtNo, 0, (motorMAX - 1)))
					break;

				mmiToSeq.buffer.mtStatus.bHoming = MT[nMtNo].m_fsmDrv.Between(C_HOME_START, C_HOME_END);
				mmiToSeq.buffer.mtStatus.bServoOn = MT[nMtNo].m_state.isServoOn;
				mmiToSeq.buffer.mtStatus.bBrakeOn = MT[nMtNo].m_state.isBrakeOn;
				mmiToSeq.buffer.mtStatus.bAlarm = MT[nMtNo].m_state.isAlarm;
				mmiToSeq.buffer.mtStatus.bDriving = MT[nMtNo].m_state.isDriving;
				mmiToSeq.buffer.mtStatus.bPaused = MT[nMtNo].m_state.isPaused;
				mmiToSeq.buffer.mtStatus.bHome = MT[nMtNo].m_state.isHome;
				mmiToSeq.buffer.mtStatus.bCw = MT[nMtNo].m_state.isHwCwSen;
				mmiToSeq.buffer.mtStatus.bCCw = MT[nMtNo].m_state.isHwCCwSen;
				mmiToSeq.buffer.mtStatus.bOrg = MT[nMtNo].m_state.isHomeSen;
				mmiToSeq.buffer.mtStatus.nCurIndex = MT[nMtNo].m_curIdx;
				mmiToSeq.buffer.mtStatus.dRealCnt = round(MT[nMtNo].m_state.realCnt * 1000.) / 1000.;
			}
			break;
			case CMD_RD_MT_TABLE:
			{
				nSize = sizeof(MtTable) + 4;
				AxisList nMtNo = (AxisList)mmiToSeq.buffer.mtTable.nMtNo;

				if (!Between(nMtNo, 0, (motorMAX - 1)))
					break;

				for (int nIndex = 0; nIndex < MAX_INDEX_MOTOR; nIndex++)
				{
					mmiToSeq.buffer.mtTable.dPos[nIndex] = round(NV.motTable[nMtNo].dPos[nIndex] * 1000.) / 1000.;
					mmiToSeq.buffer.mtTable.dVel[nIndex] = round(NV.motTable[nMtNo].dVel[nIndex] * 1000.) / 1000.;
					mmiToSeq.buffer.mtTable.dAcc[nIndex] = round(NV.motTable[nMtNo].dAcc[nIndex] * 1000.) / 1000.;
				}
			}
			break;
			case CMD_WR_MT_TABLE:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_MT_TABLE] Machine is Not Stop!");
					break;
				}
				if (OPR.isPaused)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_MT_TABLE] Machine is Paused Stop!");
					break;
				}

				AxisList nMtNo = (AxisList)mmiToSeq.buffer.mtTable.nMtNo;

				if (!Between(nMtNo, 0, (motorMAX - 1)))
					break;

				double dev = 0;
				double oldVal = 0, newVal = 0;

				for (int nIndex = 0; nIndex < MAX_INDEX_MOTOR; nIndex++)
				{
					if (round(NV.motTable[nMtNo].dPos[nIndex] * 1000.) != round(mmiToSeq.buffer.mtTable.dPos[nIndex] * 1000.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Axis:%2d, Index:%2d, Old Pos:%.3f(mm) -> New Pos:%.3f(mm)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], nMtNo, nIndex,
							NV.motTable[nMtNo].dPos[nIndex], mmiToSeq.buffer.mtTable.dPos[nIndex]);
					}
					if (round(NV.motTable[nMtNo].dVel[nIndex] * 1000.) != round(mmiToSeq.buffer.mtTable.dVel[nIndex] * 1000.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Axis:%2d, Index:%2d, Old Vel:%.3f(mm/s) -> New Vel:%.3f(mm/s)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], nMtNo, nIndex,
							NV.motTable[nMtNo].dVel[nIndex], mmiToSeq.buffer.mtTable.dVel[nIndex]);
					}
					if (round(NV.motTable[nMtNo].dAcc[nIndex] * 1000.) != round(mmiToSeq.buffer.mtTable.dAcc[nIndex] * 1000.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Axis:%2d, Index:%2d, Old Vel:%.3f(mm/s^2) -> New Vel:%.3f(mm/s^2)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], nMtNo, nIndex,
							NV.motTable[nMtNo].dAcc[nIndex], mmiToSeq.buffer.mtTable.dAcc[nIndex]);
					}

					NV.motTable[nMtNo].dPos[nIndex] = round(mmiToSeq.buffer.mtTable.dPos[nIndex] * 1000.) / 1000.;
					NV.motTable[nMtNo].dVel[nIndex] = round(mmiToSeq.buffer.mtTable.dVel[nIndex] * 1000.) / 1000.;
					NV.motTable[nMtNo].dAcc[nIndex] = round(mmiToSeq.buffer.mtTable.dAcc[nIndex] * 1000.) / 1000.;
				}
				NV_MEMORY.Flush();
			}
			break;
			case CMD_WR_MT_CMD:
			{
				nSize = 0;
				if (MTCMD_STOP != mmiToSeq.buffer.mtControl.nCmd &&
					MTCMD_JOG_STOP != mmiToSeq.buffer.mtControl.nCmd &&
					MTCMD_SERVO_OFF != mmiToSeq.buffer.mtControl.nCmd)
				{
					if (!OPR.isStop)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_MT_CMD] Machine is Not Stop!");
						break;
					}
					if (OPR.isEmg)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_MT_CMD] Machine is Emergency status!");
						break;
					}
					if (OPR.isWrongParam)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_MT_CMD] Machine have Wrong Parameters!");
						break;
					}
					if (OPR.isCycleRun)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_MT_CMD] Machine is Cycle Running!");
						break;
					}
					if (OPR.isDoorUnlock)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_MT_CMD] Machine Door is Unlocked!");
						break;
					}
					if (OPR.isDoorOpen)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_MT_CMD] Machine Door is Opened!");
						break;
					}
				}

				MtControl(mmiToSeq.buffer.mtControl);
				LOG[logMMI].Message("[CMD_WR_MT_CMD]");
			}
			break;
			case CMD_RD_ERR_WR:
			{
				nSize = sizeof(ErrWr) + 4;
				for (int i = 0; i < 10; i++)
				{
					mmiToSeq.buffer.errWr.err[i] = ER.m_err[i];
					mmiToSeq.buffer.errWr.wr[i] = WR.m_err[i];
				}
			}
			break;
			case CMD_RD_VAC_STATE:
			{
				nSize = sizeof(mmiToSeq.buffer.vacState) + 4;
				for (int i = 0; (i < vacuumMAX) && (i < 100); i++)
				{
					VacuumList vcno = VacuumList(i);
					memcpy(&mmiToSeq.buffer.vacState[vcno] , &VC[vcno].m_state , sizeof(mmiToSeq.buffer.vacState[vcno]));
				}
			}
			break;
			case CMD_RD_VAC_TABLE:
			{
				nSize = sizeof(VacTable) + 4;
				VacuumList nVcNo = (VacuumList)mmiToSeq.buffer.vacTable.actuatorNo;
				if (!Between(nVcNo , 0 , (vacuumMAX - 1)))
					break;

				memcpy(&mmiToSeq.buffer.vacTable , &NV.vacTable[nVcNo] , sizeof(mmiToSeq.buffer.vacTable));
			}
			break;
			case CMD_WR_VAC_TABLE:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_TABLE] Machine is Not Stop!");
					break;
				}
				if (OPR.isPaused)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_TABLE] Machine is Paused Stop!");
					break;
				}

				VacuumList nVcNo = (VacuumList)mmiToSeq.buffer.vacTable.actuatorNo;
				if (!Between(nVcNo , 0 , (vacuumMAX - 1)))
					break;

				memcpy(&NV.vacTable[nVcNo] , &mmiToSeq.buffer.vacTable , sizeof(NV.vacTable[nVcNo]));
				memcpy(&VC[nVcNo].m_table , &mmiToSeq.buffer.vacTable , sizeof(VC[nVcNo].m_table));
				VC[nVcNo].m_command = VC[nVcNo].CommandTable(0);
				LOG[logMMI].Message("[CMD_WR_VAC_TABLE]");
			}
			break;
			case CMD_WR_VAC_CONTROL:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_CONTROL] Machine is Not Stop!");
					break;
				}
				if (OPR.isEmg)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_CONTROL] Machine is Emergency status!");
					break;
				}
				if (OPR.isWrongParam)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_CONTROL] Machine have Wrong Parameters!");
					break;
				}
				if (OPR.isCycleRun)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_CONTROL] Machine is Cycle Running!");
					break;
				}
				if (OPR.isDoorUnlock)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_CONTROL] Machine Door is Unlocked!");
					break;
				}
				if (OPR.isDoorOpen)
				{
					MSGNOTICE.Set(MSG_NOTICE ,
						"CLOSE" , NULL , NULL , NULL ,
						"[CMD_WR_VAC_CONTROL] Machine Door is Opened!");
					break;
				}

				VacuumList nVcNo = (VacuumList)mmiToSeq.buffer.vacControl.actuatorNo;
				if (!Between(nVcNo , 0 , (vacuumMAX - 1)))
					break;

				if (mmiToSeq.buffer.vacControl.setOn)
				{
					VC[nVcNo].On(mmiToSeq.buffer.vacControl.indexNo);
				}
				else if (mmiToSeq.buffer.vacControl.setOff)
				{
					VC[nVcNo].Off(mmiToSeq.buffer.vacControl.indexNo);
				}
				else if (mmiToSeq.buffer.vacControl.setBlow)
				{
					if (VC[nVcNo].IsBlowOn())
						VC[nVcNo].BlowOff();
					else
						VC[nVcNo].BlowOn(mmiToSeq.buffer.vacControl.indexNo);
				}
				LOG[logMMI].Message("[CMD_WR_VAC_CONTROL]");
			}
			break;
			case CMD_RD_CYL_STATE:
			{
				nSize = sizeof(mmiToSeq.buffer.cylState) + 4;
				for (int i = 0; (i < cylinderMAX) && (i < 100); i++)
				{
					CylinderList cyno = CylinderList(i);
					memcpy(&mmiToSeq.buffer.cylState[cyno], &CYL[cyno].m_state, sizeof(mmiToSeq.buffer.cylState[cyno]));
				}
			}
			break;
			case CMD_RD_CYL_TABLE:
			{
				nSize = sizeof(CylTable) + 4;
				CylinderList nCyNo = (CylinderList)mmiToSeq.buffer.cylTable.actuatorNo;
				if (!Between(nCyNo, 0, (cylinderMAX - 1)))
					break;

				memcpy(&mmiToSeq.buffer.cylTable, &NV.cylTable[nCyNo], sizeof(mmiToSeq.buffer.cylTable));
			}
			break;
			case CMD_WR_CYL_TABLE:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_MT_TABLE] Machine is Not Stop!");
					break;
				}
				if (OPR.isPaused)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_MT_TABLE] Machine is Paused Stop!");
					break;
				}

				CylinderList nCyNo = (CylinderList)mmiToSeq.buffer.cylTable.actuatorNo;
				if (!Between(nCyNo, 0, (cylinderMAX - 1)))
					break;

				memcpy(&NV.cylTable[nCyNo], &mmiToSeq.buffer.cylTable, sizeof(NV.cylTable[nCyNo]));
				memcpy(&CYL[nCyNo].m_table, &mmiToSeq.buffer.cylTable, sizeof(CYL[nCyNo].m_table));
				LOG[logMMI].Message("[CMD_WR_CYL_TABLE]");
			}
			break;
			case CMD_WR_CYL_CONTROL:
			{
				nSize = 0;		
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_CYL_CONTROL] Machine is Not Stop!");
					break;
				}
				if (OPR.isEmg)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_CYL_CONTROL] Machine is Emergency status!");
					break;
				}
				if (OPR.isWrongParam)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_CYL_CONTROL] Machine have Wrong Parameters!");
					break;
				}
				if (OPR.isCycleRun)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_CYL_CONTROL] Machine is Cycle Running!");
					break;
				}
				if (OPR.isDoorUnlock)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_CYL_CONTROL] Machine Door is Unlocked!");
					break;
				}
				if (OPR.isDoorOpen)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_CYL_CONTROL] Machine Door is Opened!");
					break;
				}		
				CylinderList nCyNo = (CylinderList)mmiToSeq.buffer.cylControl.actuatorNo;
				if (!Between(nCyNo, 0, (cylinderMAX - 1)))
					break;

				if (mmiToSeq.buffer.cylControl.setA)
				{
					if (!NV.use[useMasterKeyMode])
					{
						OPR.isCanMoveMsg = 1;
						if (!CYL[nCyNo].CanMove(Cylinder::cylinderFWD, mmiToSeq.buffer.cylControl.indexNo))
						{
							OPR.isCanMoveMsg = 0;
							break;
						}
						OPR.isCanMoveMsg = 0;
					}
					CYL[nCyNo].Fwd(mmiToSeq.buffer.cylControl.indexNo);
				}
				else if (mmiToSeq.buffer.cylControl.setB)
				{
					if (!NV.use[useMasterKeyMode])
					{
						OPR.isCanMoveMsg = 1;
						if (!CYL[nCyNo].CanMove(Cylinder::cylinderBWD, mmiToSeq.buffer.cylControl.indexNo))
						{
							OPR.isCanMoveMsg = 0;
							break;
						}
						OPR.isCanMoveMsg = 0;
					}
					CYL[nCyNo].Bwd(mmiToSeq.buffer.cylControl.indexNo);
				}
				LOG[logMMI].Message("[CMD_WR_CYL_CONTROL]");
			}
			break;

			case CMD_RD_SEQ_VERSION:
			{
				memcpy(&mmiToSeq.buffer.seqVersion, &SEQ_VERSION.data, sizeof(IPC_MMI::SeqVersion));
				break;
			}
			// Fire Detector
			case CMD_WR_FIRE_DETECT_STATE:
			{
				int nStart = mmiToSeq.buffer.fireDetectorInfo.nStart;
				int nEnd = mmiToSeq.buffer.fireDetectorInfo.nEnd;

				if (nvFireDetectorMax <= nEnd)
					nEnd = nvFireDetectorMax - 1;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (nvFireDetectorMax <= nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_FIRE_DETECTOR_INFO],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					memcpy(&FIRE_DETECTOR_DATA[i], &mmiToSeq.buffer.fireDetectorInfo.data[i], sizeof(IPC_MMI::FireDetector));
					LOG[logMMI].Message("[CMD_WR_FIRE_DETECTOR_INFO%d]", i);
				}
				NV_FIRE_DETECTOR_DATA.Flush();
				break;
			}
			/** FDC VACUUM DATA **/
			case CMD_RD_VAC_FDC:
			{
				nSize = sizeof(mmiToSeq.buffer.vacFdcInfo) + 4;
				for (int i = 0; (i < vacuumMAX) && (i < 100); i++)
				{
					VacuumList nVcNo = VacuumList(i);
					memcpy(&mmiToSeq.buffer.vacFdcInfo.data[nVcNo], &VC[nVcNo].m_fdcData, sizeof(mmiToSeq.buffer.vacFdcInfo.data[nVcNo]));
				}
				break;
			}
			case CMD_WR_VAC_FDC:
			{
				VacuumList nVcNo = (VacuumList)mmiToSeq.buffer.vacFdcInfo.actuatorNo;
				if (!Between(nVcNo, 0, (vacuumMAX - 1)))
					break;

				memcpy(&VC[nVcNo].m_fdcData.dataSend, &mmiToSeq.buffer.vacFdcInfo.data[nVcNo].dataSend, sizeof(VC[nVcNo].m_fdcData.dataSend));
				break;
			}
			/** FDC VACUUM DATA **/

			case CMD_RD_MSGNOTICE:
			{
				nSize = sizeof(MsgNotice) + 4;
				memcpy(&mmiToSeq.buffer.msgNoticeTitle.strMsgTtile, &MSGNOTICE.m_strMsgTitle, sizeof(mmiToSeq.buffer.msgNoticeTitle.strMsgTtile));
				memcpy(&mmiToSeq.buffer.msgNoticeTitle.strBtn, &MSGNOTICE.m_strMsgBtn, sizeof(mmiToSeq.buffer.msgNoticeTitle.strBtn));
			}
			break;
			case CMD_RD_SYS:
			{
				nSize = sizeof(DoubleData) + 4;
				double* pOrigin = &NV.sys[(SysList)0];
				double* pTarget = &mmiToSeq.buffer.sys.dVal[0];
				memcpy(pTarget, pOrigin, sizeof(double) * 1000);
			}
			break;
			case CMD_WR_SYS:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_SYS] Machine is Not Stop!");
					break;
				}
				if (OPR.isPaused)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_SYS] Machine is Paused Stop!");
					break;
				}

				int nStart = mmiToSeq.buffer.sys.nStart;
				int nEnd = mmiToSeq.buffer.sys.nEnd;

				if (999 < nEnd)
				{
					nEnd = 999;
				}
				BOOL bErr = (nStart < 0) || (nEnd < nStart) || (999 < nEnd);
				if (bErr)
				{
					LOG[logMMI].Message("[CMD_WR_SYS],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}
				for (int i = nStart; i <= nEnd; i++)
				{
					if ((round(NV.sys[(SysList)i] * 1000.) / 1000.) != (round(mmiToSeq.buffer.sys.dVal[i] * 1000.) / 1000.))
					{
						JustBeforeSysValueChanges(i);
						LOG[logMMI].Message("[CMD_WR_SYS],[SysList],[%3d],[Old Value],[%.3f],[New Value],[%.3f]",
							i, round(NV.sys[(SysList)i] * 1000.) / 1000., round(mmiToSeq.buffer.sys.dVal[i] * 1000.) / 1000.);
					}
					NV.sys[(SysList)i] = mmiToSeq.buffer.sys.dVal[i];
				}
				OPR.isCheckSys = (TRUE);
				NV_MEMORY.Flush();
			}
			break;
			case CMD_RD_RCP:
			{
				nSize = sizeof(DoubleData) + 4;
				double* pOrigin = &NV.rcp[(RcpList)0];
				double* pTarget = &mmiToSeq.buffer.rcp.dVal[0];
				memcpy(pTarget, pOrigin, sizeof(double) * 1000);
			}
			break;
			case CMD_WR_RCP:
			{
				nSize = 0;
				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_RCP] Machine is Not Stop!");
					break;
				}
				if (OPR.isPaused)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_RCP] Machine is Paused Stop!");
					break;
				}

				int nStart = mmiToSeq.buffer.rcp.nStart;
				int nEnd = mmiToSeq.buffer.rcp.nEnd;

				if (999 < nEnd)
					nEnd = 999;

				BOOL bErr = (nStart < 0) || (nEnd < nStart) || (999 < nEnd);
				if (bErr)
				{
					LOG[logMMI].Message("[CMD_WR_RCP],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					if ((round(NV.rcp[(RcpList)i] * 1000.) / 1000.) != (round(mmiToSeq.buffer.rcp.dVal[i] * 1000.) / 1000.))
					{
						JustBeforeRcpValueChanges(i);
						LOG[logMMI].Message("[CMD_WR_RCP],[RcpList],[%3d],[Old Value],[%.3f],[New Value],[%.3f]",
							i, round(NV.rcp[(RcpList)i] * 1000.) / 1000., round(mmiToSeq.buffer.rcp.dVal[i] * 1000.) / 1000.);
					}
					NV.rcp[(RcpList)i] = mmiToSeq.buffer.rcp.dVal[i];
				}
				OPR.isCheckRcp = (TRUE);
				NV_MEMORY.Flush();
			}
			break;
			case CMD_RD_MT_UPDATE_MMI:
			{
				nSize = sizeof(MtUpdateMmi) + 4;
				memcpy(&mmiToSeq.buffer.mtupdatemmi, &UPDATEMMI.m_pMtUpdate, sizeof(mmiToSeq.buffer.mtupdatemmi));
			}
			break;
			case CMD_RD_PKG_UPDATE_MMI:
			{
				nSize = sizeof(PkgUpdateMmi) + 4;
				memcpy(&mmiToSeq.buffer.pkgupdatemmi, &UPDATEMMI.m_pPkgUpdate, sizeof(mmiToSeq.buffer.pkgupdatemmi));
			}
			break;
			case CMD_RD_RCP_UPDATE_MMI:
			{
				nSize = sizeof(PkgUpdateMmi) + 4;
				memcpy(&mmiToSeq.buffer.pkgupdatemmi, &UPDATEMMI.m_pRcpUpdate, sizeof(mmiToSeq.buffer.pkgupdatemmi));
			}
			break;
			
			case CMD_WR_PRS_RESULT:
			{
				nSize = 0;
				VisionList viNo = VisionList(mmiToSeq.buffer.prsViData.viNo);
				if (!Between(viNo, 0, VI_MAX - 1))
				{
					LOG[logMMI].Message("[CMD_WR_PRS_RESULT] VI%d No - Abnormal", viNo);
					break;
				}
				if (!OPR.isDryRun)
				{
					memcpy(&VI[viNo].m_prsData, &mmiToSeq.buffer.prsViData, sizeof(VI[viNo].m_prsData));
					VI[viNo].m_isRcvComp = true;
				}
				LOG[logMMI].Message("[CMD_WR_PRS%d_RESULT]", viNo);
			}
			break;
			case CMD_WR_QC_RESULT:
			{
				// (기준) ----->
				// ------------>
				// ------------>
				nSize = 0;
				VisionList viNo = VisionList(mmiToSeq.buffer.prsViData.viNo);
				if (!Between(viNo, 0, VI_MAX - 1))
				{
					LOG[logMMI].Message("[CMD_WR_QC_RESULT] VI%d No - Abnormal", viNo);
					break;
				}

				if (!OPR.isDryRun)
				{
					memcpy(&VI[viNo].m_qcData, &mmiToSeq.buffer.qcViData, sizeof(VI[viNo].m_qcData));
					VI[viNo].m_isRcvComp = true;
				}
				LOG[logMMI].Message("[CMD_WR_QC%d_RESULT]", viNo);
			}
			break;
			case CMD_WR_VISION_BARCODE:
			{
				nSize = 0;
				VisionList viNo = VisionList(mmiToSeq.buffer.bcdViData.viNo);
				if (!Between(viNo, 0, VI_MAX - 1))
				{
					LOG[logMMI].Message("[CMD_WR_VISION_BARCODE] VI%d No - Abnormal", viNo);
					break;
				}
				memcpy(&VI[viNo].m_unitData, &mmiToSeq.buffer.bcdViData, sizeof(VI[viNo].m_unitData));
				VI[viNo].m_isRcvComp = true;
				LOG[logMMI].Message("[CMD_WR_VISION_BARCODE]");
				break;
			}
			case CMD_WR_BCD_RESULT:
			{
				int nBcd = mmiToSeq.buffer.barcodeResult.barCodeNo;
				if (!Between(nBcd, bcdMin, bcdMax - 1))
				{
					LOG[logMMI].Message("IPC[CMD_WR_BCD_RESULT] : Invalid Bcd Index");
					return;
				}

				memcpy(&TRAY_PART_DATA[nvTrayShuttle].lotInfo.labelComparison, &mmiToSeq.buffer.barcodeResult.str[nBcd], (sizeof(char) * 128));
				break;
			}


			case CMD_WR_PRE_MC_DATA_RECV:
			{
				int nStart = mmiToSeq.buffer.trayPartInfo.nStart;
				int nEnd = mmiToSeq.buffer.trayPartInfo.nEnd;

				if (nvInPk != nStart || nvInPk != nEnd)
				{
					LOG[logMMI].Message("[CMD_WR_PRE_MC_DATA_RECV],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				memcpy(&TRAY_PART_DATA[nvInPk], &mmiToSeq.buffer.trayPartInfo.data, sizeof(IPC_MMI::TrayPartData));
				LOG[logMMI].Message("[CMD_WR_PRE_MC_DATA_RECV%d]", nvInPk);	// Comment out after testing 
				NV.ndm[recvPreMcData] = COMM_COMP; // MMI Change ?
				NV_TRAY_PART_DATA.Flush();
				break;
			}
			case CMD_RD_TRAY_PART_INFO:
			{
				memcpy(&mmiToSeq.buffer.trayPartInfo.data, &TRAY_PART_DATA, sizeof(IPC_MMI::TrayPartData) * nvTrayPartMax);
				break;
			}
			case CMD_WR_TRAY_PART_INFO:
			{
				int nStart = mmiToSeq.buffer.trayPartInfo.nStart;
				int nEnd = mmiToSeq.buffer.trayPartInfo.nEnd;

				if (nvTrayPartMax <= nEnd)
					nEnd = nvTrayPartMax - 1;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (nvTrayPartMax <= nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_TRAY_PART_INFO],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					memcpy(&TRAY_PART_DATA[i], &mmiToSeq.buffer.trayPartInfo.data[i], sizeof(IPC_MMI::TrayPartData));
					LOG[logMMI].Message("[CMD_WR_TRAY_PART_INFO%d]", i);
				}
				NV_TRAY_PART_DATA.Flush();
				break;
			}
			case CMD_RD_COBOT_PART_INFO:
			{
				memcpy(&mmiToSeq.buffer.cobotPartInfo.data, &COBOT_PART_DATA, sizeof(CobotPartData) * nvCobotPartMax);
				break;
			}
			case CMD_WR_COBOT_PART_INFO:
			{
				int nStart = mmiToSeq.buffer.cobotPartInfo.nStart;
				int nEnd = mmiToSeq.buffer.cobotPartInfo.nEnd;

				if (nvCobotPartMax <= nEnd)
					nEnd = nvCobotPartMax - 1;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (nvCobotPartMax <= nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_COBOT_PART_INFO],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					memcpy(&COBOT_PART_DATA[i], &mmiToSeq.buffer.cobotPartInfo.data[i], sizeof(IPC_MMI::CobotPartData));
					LOG[logMMI].Message("[CMD_WR_COBOT_PART_INFO%d]", i);
				}
				NV_COBOT_PART_DATA.Flush();
				break;
			}
			case CMD_RD_FOAM_PART_INFO:
			{
				memcpy(&mmiToSeq.buffer.foamPartInfo.data, &FOAM_PART_DATA, sizeof(FoamPartData) * nvFoamPartMax);
				break;
			}
			case CMD_WR_FOAM_PART_INFO:
			{
				int nStart = mmiToSeq.buffer.foamPartInfo.nStart;
				int nEnd = mmiToSeq.buffer.foamPartInfo.nEnd;

				if (nvFoamPartMax <= nEnd)
					nEnd = nvFoamPartMax - 1;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (nvFoamPartMax <= nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_FOAM_PART_INFO],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					memcpy(&FOAM_PART_DATA[i], &mmiToSeq.buffer.foamPartInfo.data[i], sizeof(IPC_MMI::FoamPartData));
					LOG[logMMI].Message("[CMD_WR_COBOT_PART_INFO%d]", i);
				}
				NV_FOAM_PART_DATA.Flush();
				break;
			}
			case CMD_RD_LABEL_PART_INFO:
			{
				memcpy(&mmiToSeq.buffer.labelPartInfo.data, &LABEL_PART_DATA, sizeof(LabelPartData) * nvLabelPartMax);
				break;
			}
			case CMD_WR_LABEL_PART_INFO:
			{
				int nStart = mmiToSeq.buffer.labelPartInfo.nStart;
				int nEnd = mmiToSeq.buffer.labelPartInfo.nEnd;

				if (nvLabelPartMax <= nEnd)
					nEnd = nvLabelPartMax - 1;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (nvLabelPartMax <= nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_LABEL_PART_INFO],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					memcpy(&LABEL_PART_DATA[i], &mmiToSeq.buffer.labelPartInfo.data[i], sizeof(IPC_MMI::LabelPartData));
					LOG[logMMI].Message("[CMD_WR_LABEL_PART_INFO%d]", i);
				}
				NV_LABEL_PART_DATA.Flush();
				break;
			}
			case CMD_RD_MBB_PART_INFO:
			{
				memcpy(&mmiToSeq.buffer.mbbPartInfo.data, &MBB_PART_DATA, sizeof(MbbPartData) * nvMbbPartMax);
				break;
			}
			case CMD_WR_MBB_PART_INFO:
			{
				int nStart = mmiToSeq.buffer.mbbPartInfo.nStart;
				int nEnd = mmiToSeq.buffer.mbbPartInfo.nEnd;

				if (nvMbbPartMax <= nEnd)
					nEnd = nvMbbPartMax - 1;

				BOOL isErr = (nStart < 0) || (nEnd < nStart) || (nvMbbPartMax <= nEnd);
				if (isErr)
				{
					LOG[logMMI].Message("[CMD_WR_MBB_PART_INFO],Fail,begin[%d],end[%d]", nStart, nEnd);
					break;
				}

				for (int i = nStart; i <= nEnd; i++)
				{
					memcpy(&MBB_PART_DATA[i], &mmiToSeq.buffer.mbbPartInfo.data[i], sizeof(IPC_MMI::MbbPartData));
					LOG[logMMI].Message("[CMD_WR_MBB_PART_INFO%d]", i);
				}
				NV_MBB_PART_DATA.Flush();
				break;
			}

			// Cobot
			case CMD_RD_ROBOT_IO:
			{
				RobotList RobotNo = RobotList(mmiToSeq.buffer.rdRobotIO.robotNo);

				memcpy(&mmiToSeq.buffer.rdRobotIO.inCH, &ROBOT[RobotNo].m_robotIo.robotDi, sizeof(uint16_t));
				memcpy(&mmiToSeq.buffer.rdRobotIO.outCH, &ROBOT[RobotNo].m_robotIo.robotDo, sizeof(uint16_t));
				break;
			}
			case CMD_WR_ROBOT_IO:
			{
				RobotList RobotNo = RobotList(mmiToSeq.buffer.rdRobotIO.robotNo);
				if (!OPR.isDebugMode)
				{
					if (!OPR.isStop)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO] Machine is Not Stop!");
						break;
					}
					if (OPR.isPaused)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO],[Machine is Paused Stop!]");
						break;
					}
					if (OPR.isCycleRun)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO],[Machine is Cycle Running!]");
						break;
					}
				}

				RobotDo oNo = (RobotDo)mmiToSeq.buffer.wtRobotIO.nIONo;
				if (Between(oNo, 0, robotDoMax))
				{
					ROBOT[RobotNo].Set(oNo, mmiToSeq.buffer.wtRobotIO.bOn);

					if (mmiToSeq.buffer.wrIO.bOn)
						LOG[logMMI].Message("[CMD_WR_IO],[Robot output],[%.4d],[on]", oNo);
					else
						LOG[logMMI].Message("[CMD_WR_IO],[Robot output],[%.4d],[off]", oNo);
				}
				break;
			}
			case CMD_RD_ROBOT_STATUS:
			{
				RobotList RobotNo = RobotList(mmiToSeq.buffer.rdRobotIO.robotNo);

				mmiToSeq.buffer.robotState.curTcp = ROBOT[RobotNo].m_robotCurPos.tcp;
				mmiToSeq.buffer.robotState.curJoint = ROBOT[RobotNo].m_robotCurPos.joint;

				mmiToSeq.buffer.robotState.activated = ROBOT[RobotNo].m_robotState.activated;
				mmiToSeq.buffer.robotState.realMode = ROBOT[RobotNo].m_robotState.realMode;
				mmiToSeq.buffer.robotState.collisionDetected = ROBOT[RobotNo].m_robotState.collisionDetected;
				mmiToSeq.buffer.robotState.armPowerEngaged = ROBOT[RobotNo].m_robotState.armPowerEngaged;
				mmiToSeq.buffer.robotState.directTeachingMode = ROBOT[RobotNo].m_robotState.directTeachingMode;
				mmiToSeq.buffer.robotState.moving = ROBOT[RobotNo].m_robotState.moving;
				mmiToSeq.buffer.robotState.pauseState = ROBOT[RobotNo].m_robotState.pauseState;
				mmiToSeq.buffer.robotState.teachingPendantConnecred = ROBOT[RobotNo].m_robotState.teachingPendantConnecred;
				mmiToSeq.buffer.robotState.programRun = ROBOT[RobotNo].m_robotState.programRun;
				mmiToSeq.buffer.robotState.noArcModeOn = ROBOT[RobotNo].m_robotState.noArcModeOn;
				mmiToSeq.buffer.robotState.emgButtonReleased = ROBOT[RobotNo].m_robotState.emgButtonReleased;
				mmiToSeq.buffer.robotState.firstProgramRun = ROBOT[RobotNo].m_robotState.firstProgramRun;
				mmiToSeq.buffer.robotState.lastProgramLoadDone = ROBOT[RobotNo].m_robotState.lastProgramLoadDone;
				mmiToSeq.buffer.robotState.underActivation = ROBOT[RobotNo].m_robotState.underActivation;
				mmiToSeq.buffer.robotState.collisionState = ROBOT[RobotNo].m_robotState.collisionState;
				mmiToSeq.buffer.robotState.systemSOS = ROBOT[RobotNo].m_robotState.systemSOS;
				mmiToSeq.buffer.robotState.homeIsBegin = ROBOT[RobotNo].m_robotState.homeIsBegin;
				mmiToSeq.buffer.robotState.controlBoxHeartBeat = ROBOT[RobotNo].m_robotState.controlBoxHeartBeat;

				mmiToSeq.buffer.robotState.controlBoxAmpere = ROBOT[RobotNo].m_robotState.controlBoxAmpere;
				mmiToSeq.buffer.robotState.tcpMass = ROBOT[RobotNo].m_robotState.tcpMass;

				mmiToSeq.buffer.robotState.collisionThreshold = ROBOT[RobotNo].m_robotState.collisionThreshold;
				mmiToSeq.buffer.robotState.speedBarValue = ROBOT[RobotNo].m_robotState.speedBarValue;

				mmiToSeq.buffer.robotState.curIdx = ROBOT[RobotNo].m_curIdx;
				mmiToSeq.buffer.robotState.isHome = ROBOT[RobotNo].m_isHome;
				mmiToSeq.buffer.robotState.isHoming = false;

				break;
			}
			case CMD_RD_ROBOT_POS_TABLE:
			{
				RobotList RobotNo = RobotList(mmiToSeq.buffer.rdRobotIO.robotNo);

				if (!Between(RobotNo, 0, (robotMAX - 1)))
					break;

				for (int nIndex = 0; nIndex < MAX_INDEX_ROBOT; nIndex++)
				{
					mmiToSeq.buffer.robotTable.nVel[nIndex] = ROBOT_TABLE[RobotNo].nVel[nIndex];

					mmiToSeq.buffer.robotTable.joint[nIndex].joint_0 = round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_0 * 10.) / 10.;
					mmiToSeq.buffer.robotTable.joint[nIndex].joint_1 = round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_1 * 10.) / 10.;
					mmiToSeq.buffer.robotTable.joint[nIndex].joint_2 = round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_2 * 10.) / 10.;
					mmiToSeq.buffer.robotTable.joint[nIndex].joint_3 = round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_3 * 10.) / 10.;
					mmiToSeq.buffer.robotTable.joint[nIndex].joint_4 = round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_4 * 10.) / 10.;
					mmiToSeq.buffer.robotTable.joint[nIndex].joint_5 = round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_5 * 10.) / 10.;

					mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPx = round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPx * 10.) / 10.;
					mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPy = round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPy * 10.) / 10.;
					mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPz = round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPz * 10.) / 10.;
					mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRx = round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRx * 10.) / 10.;
					mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRy = round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRy * 10.) / 10.;
					mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRz = round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRz * 10.) / 10.;
				}
				break;
			}
			case CMD_WR_ROBOT_POS_TABLE:
			{
				RobotList RobotNo = RobotList(mmiToSeq.buffer.rdRobotIO.robotNo);

				if (!OPR.isStop)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_MT_TABLE] Machine is Not Stop!");
					break;
				}
				if (ROBOT[RobotNo].m_robotState.pauseState)
				{
					MSGNOTICE.Set(MSG_NOTICE,
						"CLOSE", NULL, NULL, NULL,
						"[CMD_WR_MT_TABLE] Robot is Paused Stop!");
					break;
				}

				if (!Between(RobotNo, 0, (robotMAX - 1)))
					break;

				for (int nIndex = 0; nIndex < MAX_INDEX_ROBOT; nIndex++)
				{
					if (round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_0 * 10.) != round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_0 * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Joint_0:%.3f(deg) -> New Joint:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].joint[nIndex].joint_0, mmiToSeq.buffer.robotTable.joint[nIndex].joint_0);
					}
					if (round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_1 * 10.) != round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_1 * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Joint_1:%.3f(deg) -> New Joint_1:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].joint[nIndex].joint_1, mmiToSeq.buffer.robotTable.joint[nIndex].joint_1);
					}
					if (round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_2 * 10.) != round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_2 * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Joint_2:%.3f(deg) -> New Joint_2:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].joint[nIndex].joint_2, mmiToSeq.buffer.robotTable.joint[nIndex].joint_2);
					}
					if (round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_3 * 10.) != round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_3 * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Joint_3:%.3f(deg) -> New Joint_3:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].joint[nIndex].joint_3, mmiToSeq.buffer.robotTable.joint[nIndex].joint_3);
					}
					if (round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_4 * 10.) != round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_4 * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Joint_4:%.3f(deg) -> New Joint_4:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].joint[nIndex].joint_4, mmiToSeq.buffer.robotTable.joint[nIndex].joint_4);
					}
					if (round(ROBOT_TABLE[RobotNo].joint[nIndex].joint_5 * 10.) != round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_5 * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Joint_5:%.3f(deg) -> New Joint_5:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].joint[nIndex].joint_5, mmiToSeq.buffer.robotTable.joint[nIndex].joint_5);
					}

					if (round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPx * 10.) != round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPx * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old tcpPx:%.3f(mm) -> New tcpPx:%.3f(mm)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPx, mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPx);
					}
					if (round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPy * 10.) != round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPy * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old tcpPy:%.3f(mm) -> New tcpPy:%.3f(mm)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPy, mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPy);
					}
					if (round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPz * 10.) != round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPz * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old tcpPz:%.3f(mm) -> New tcpPz:%.3f(mm)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPz, mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPz);
					}
					if (round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRx * 10.) != round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRx * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old tcpRx:%.3f(deg) -> New tcpRx:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRx, mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRx);
					}
					if (round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRy * 10.) != round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRy * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old tcpRy:%.3f(deg) -> New tcpRy:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRy, mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRy);
					}
					if (round(ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRz * 10.) != round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRz * 10.))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old tcpRz:%.3f(deg) -> New tcpRz:%.3f(deg)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRz, mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRz);
					}

					if (round(ROBOT_TABLE[RobotNo].nVel[nIndex]) != round(mmiToSeq.buffer.robotTable.nVel[nIndex]))
					{
						LOG[logMMI].Message("[CMD_WR_MT_TABLE], grp:%d, job:%d, Robot:%2d, Index:%2d, Old Speed:%d(%) -> New Speed:%d(%)",
							NV.ndm[deviceCurGrpNo], NV.ndm[deviceCurJobNo], RobotNo, nIndex,
							ROBOT_TABLE[RobotNo].nVel[nIndex], mmiToSeq.buffer.robotTable.nVel[nIndex]);
					}

					ROBOT_TABLE[RobotNo].joint[nIndex].joint_0 = round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_0 * 10.) / 10.;
					ROBOT_TABLE[RobotNo].joint[nIndex].joint_1 = round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_1 * 10.) / 10.;
					ROBOT_TABLE[RobotNo].joint[nIndex].joint_2 = round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_2 * 10.) / 10.;
					ROBOT_TABLE[RobotNo].joint[nIndex].joint_3 = round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_3 * 10.) / 10.;
					ROBOT_TABLE[RobotNo].joint[nIndex].joint_4 = round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_4 * 10.) / 10.;
					ROBOT_TABLE[RobotNo].joint[nIndex].joint_5 = round(mmiToSeq.buffer.robotTable.joint[nIndex].joint_5 * 10.) / 10.;

					ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPx = round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPx * 10.) / 10.;
					ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPy = round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPy * 10.) / 10.;
					ROBOT_TABLE[RobotNo].tcp[nIndex].tcpPz = round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpPz * 10.) / 10.;
					ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRx = round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRx * 10.) / 10.;
					ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRy = round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRy * 10.) / 10.;
					ROBOT_TABLE[RobotNo].tcp[nIndex].tcpRz = round(mmiToSeq.buffer.robotTable.tcp[nIndex].tcpRz * 10.) / 10.;

					ROBOT_TABLE[RobotNo].nVel[nIndex] = mmiToSeq.buffer.robotTable.nVel[nIndex];
				}

				NV_ROBOT.Flush();
				break;
			}
			case CMD_WR_ROBOT_CMD:
			{
				if (!OPR.isDebugMode && !OPR.isNoDevice)
				{
					if (!OPR.isStop)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO] Machine is Not Stop!");
						break;
					}
					else if (OPR.isCycleRun)
					{
						MSGNOTICE.Set(MSG_NOTICE,
							"CLOSE", NULL, NULL, NULL,
							"[CMD_WR_IO],[Machine is Cycle Running!]");
						break;
					}
				}

				RobotControl(mmiToSeq.buffer.robotCmd);
				LOG[logMMI].Message("[CMD_WR_ROBOT_CMD]");
			}
		}

		if (m_kamelas.Send((PBYTE)&mmiToSeq, 0, nSize))
			tmWatchDog.Reset();
		else
			LOG[logMMI].Message("Failed to send data");
	}

	void CIPC::MtControl(MtCmd motCommand)
	{
		AxisList nMtNo = AxisList(motCommand.nMtNo);

		for (int gtNo = 0; gtNo < gtMAX; gtNo++)
		{
			if (nMtNo == GT[gtNo].m_slave) {
				nMtNo = (AxisList) GT[gtNo].m_master;
				break;
			}
		}

		if (!Between(nMtNo, 0, (motorMAX - 1)))
			return;

		if (1 > NV.ndm[jogSpeed])
			NV.ndm[jogSpeed] = 1;	//	mm/s
		if ((int)MT[nMtNo].m_maxVel < NV.ndm[jogSpeed])
			NV.ndm[jogSpeed] = (int)MT[nMtNo].m_maxVel;

		if (MTCMD_SERVO_ON == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_SERVO_ON],[Axis:%.2d]", nMtNo);
			MT[nMtNo].ServoOn();
		}
		else if (MTCMD_SERVO_OFF == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_SERVO_OFF],[Axis:%.2d]", nMtNo);

			if (MT[nMtNo].m_state.isDriving)
				MT[nMtNo].CancelHomeSearch();

			MT[nMtNo].m_nxtIdx = MT[nMtNo].m_curIdx = P_JOG;
			MT[nMtNo].ServoOff();
		}
		else if (MTCMD_INDEX_MOVE == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_INDEX_MOVE],[Axis:%.2d]", nMtNo);

			if (OPR.isDoorOpen)
				return;

			OPR.isCanMoveMsg = 1;
			if (MT[nMtNo].IsRdy())
			{
				if (NV.use[useMasterKeyMode] || MT[nMtNo].CanMove(motCommand.nCmdIndexNo))
					AntiCollisionMove(nMtNo, motCommand.nCmdIndexNo);
			}
			else
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Motor is Not Ready Status", MT[nMtNo].m_name.c_str());
			}
			OPR.isCanMoveMsg = 0;
		}
		else if (MTCMD_JOG_MOVE == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_JOG_MOVE],[Axis:%.2d]", nMtNo);

			if (OPR.isDoorOpen)
				return;

			if (MT[nMtNo].m_state.isDriving || MT[nMtNo].m_state.isHome && !MT[nMtNo].m_state.isDelayedPosSet && !OPR.isNoDevice)
				return;

			double vel = (double)NV.ndm[jogSpeed];
			double acc = MT[nMtNo].AccTable(MT[nMtNo].m_homePosIdx);
			if (motCommand.nDir)
			{
				if (NV.use[useMasterKeyMode])
				{
					MT[nMtNo].JogFwd(vel, acc);
				}
				else
				{
					OPR.isCanMoveMsg = 1;
					if (nullptr == AntiCollisionJogFwd || !MT[nMtNo].CanMove(P_JOG))
					{
						OPR.isCanMoveMsg = 0;
						return;
					}
					OPR.isCanMoveMsg = 0;
					AntiCollisionJogFwd(nMtNo, vel, acc);
				}
			}
			else
			{
				if (NV.use[useMasterKeyMode])
				{
					MT[nMtNo].JogBwd(vel, acc);
				}
				else
				{
					OPR.isCanMoveMsg = 1;
					if (nullptr == AntiCollisionJogBwd || !MT[nMtNo].CanMove(P_JOG))
					{
						OPR.isCanMoveMsg = 0;
						return;
					}
					OPR.isCanMoveMsg = 0;
					AntiCollisionJogBwd(nMtNo, vel, acc);
				}
			}
		}
		else if (MTCMD_JOG_STOP == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_JOG_STOP],[Axis:%.2d]", nMtNo);
			MT[nMtNo].Stop();
		}
		else if (MTCMD_STOP == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_STOP Axis],[Axis:%.2d]", nMtNo);
			MT[nMtNo].Stop();
		}
		else if (MTCMD_R_MOVE == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_R_MOVE],[Axis:%.2d]", nMtNo);

			if (OPR.isDoorOpen)
				return;

			int nDir = NOT_DEFINED;
			if (motCommand.nDir)
				nDir = 1;

			if (MT[nMtNo].m_state.isDriving || MT[nMtNo].m_state.isHome && !MT[nMtNo].m_state.isDelayedPosSet && !OPR.isNoDevice)
				return;

			double dCmdDist = nDir * motCommand.dPulse;
			double pos = MT[nMtNo].m_state.cmdCnt + dCmdDist;
			double vel = NV.ndm[jogSpeed];
			double acc = MT[nMtNo].AccTable(MT[nMtNo].m_homePosIdx);

			if (NV.use[useMasterKeyMode])
			{
				MT[nMtNo].PosMove(P_JOG, pos, vel, acc);
			}
			else
			{
				OPR.isCanMoveMsg = 1;
				if (!MT[nMtNo].CanMove(P_JOG))
				{
					OPR.isCanMoveMsg = 0;
					return;
				}
				OPR.isCanMoveMsg = 0;
				AntiCollisionRMove(nMtNo, dCmdDist, pos, vel, acc);
			}
		}
		else if (MTCMD_ALL_HOME == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_ALL_HOME],[Axis:%.2d]", nMtNo);
		}
		else if (MTCMD_HOME == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_HOME],[Axis:%.2d]", nMtNo);

			if (OPR.isDoorOpen)
				return;

			OPR.isCanMoveMsg = 1;
			if (!MT[nMtNo].CanMove(HOME_CANMOVE))
			{
				OPR.isCanMoveMsg = 0;
				return;
			}
			OPR.isCanMoveMsg = 0;
			MT[nMtNo].CancelHomeSearch();
			MT[nMtNo].StartHomeCommand();
		}
		else if (MTCMD_RESET == motCommand.nCmd)
		{
			LOG[logMMI].Message("[MTCMD_RESET],[Axis:%.2d]", nMtNo);
			MT[nMtNo].AlarmClear();
		}
		else if (MTCMD_BRAKE_ON == motCommand.nCmd)
		{
			if (!MT[nMtNo].m_state.isServoOn && !MT[nMtNo].m_state.isAlarm)
			{
				MT[nMtNo].BrakeOn();
				LOG[logMMI].Message("[MTCMD_BRAKE_ON],[Axis:%.2d]", nMtNo);
			}
		}
		else if (MTCMD_BRAKE_OFF == motCommand.nCmd)
		{
			MT[nMtNo].BrakeOff();
			LOG[logMMI].Message("[MTCMD_BRAKE_OFF],[Axis:%.2d]", nMtNo);
		}
	}

	void CIPC::RobotControl(RobotCmd robotCommand)
	{
		RobotList RobotNo = RobotList(robotCommand.nRobotNo);
		if (!Between(RobotNo, 0, (robotMAX - 1)))
			return;

		if (RBCMD_ARM_ACTIVATION == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_ARM_ACTIVATION],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].Activation();
		}
		else if (RBCMD_ARM_POWER_OFF == robotCommand.nCmd)
		{
			if (ROBOT[RobotNo].m_robotState.programRun)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Program Runnig", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			if (ROBOT[RobotNo].m_robotState.moving)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Moving", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_ARM_POWER_OFF],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].Deactivation();
		}
		else if (RBCMD_CHANGE_REAL_MODE == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.activated)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Activated", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_CHANGE_REAL_MODE],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ChangeRealMode();
		}
		else if (RBCMD_START_PROGRAM_ONCE == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.activated)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Activated", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.realMode)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Real Mode", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			LOG[logMMI].Message("[RBCMD_START_PROGRAM_ONCE],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ProgramOnceStart();
		}
		else if (RBCMD_START_PROGRAM_REPEAT == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.activated)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Activated", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.realMode)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Real Mode", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			LOG[logMMI].Message("[RBCMD_START_PROGRAM_REPEAT],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ProgramRepeatStart();
		}
		else if (RBCMD_STOP_PROGRAM == robotCommand.nCmd)
		{
			if (ROBOT[RobotNo].m_robotState.moving)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Moving", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_STOP_PROGRAM],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ProgramStop();
		}
		else if (RBCMD_PAUSE_PROGRAM == robotCommand.nCmd)
		{
			LOG[logMMI].Message("[RBCMD_PAUSE_PROGRAM],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].Pause();
		}
		else if (RBCMD_RESUME_PAUSE == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.pauseState)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Pause Robot", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_RESUME_PAUSE],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ResumePause();
		}
		else if (RBCMD_RESUME_COLLISION == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.collisionDetected)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Pause Robot", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_RESUME_COLLISION],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ResumeCollision();
		}
		else if (RBCMD_LOAD_DEFAULT_PROGRAM == robotCommand.nCmd)
		{
			if (ROBOT[RobotNo].m_robotState.moving)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Moving", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (ROBOT[RobotNo].m_robotState.programRun)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Program Run", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_LOAD_DEFAULT_PROGRAM],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].LoadDefaultProgram();
		}
		else if (RBCMD_LOAD_PROGRAM_TABLE == robotCommand.nCmd)
		{
			if (ROBOT[RobotNo].m_robotState.moving)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Moving", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (ROBOT[RobotNo].m_robotState.programRun)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Program Run", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			LOG[logMMI].Message("[RBCMD_LOAD_DEFAULT_PROGRAM],[%s : Load Program No. %d]", ROBOT[RobotNo].m_name.c_str(), robotCommand.nValue);
			ROBOT[RobotNo].LoadProgram(robotCommand.nValue);
		}
		else if (RBCMD_RESET_SOS_FLAG == robotCommand.nCmd)
		{
			LOG[logMMI].Message("[RBCMD_RESET_SOS_FLAG],[%s]", ROBOT[RobotNo].m_name.c_str());
			ROBOT[RobotNo].ClearSystemSos();
		}
		else if (RBCMD_SPEED_BAR_CHANGE == robotCommand.nCmd)
		{
			LOG[logMMI].Message("[RBCMD_SPEED_BAR_CHANGE],[%s : Speed %d]", ROBOT[RobotNo].m_name.c_str(), robotCommand.nValue);
			ROBOT[RobotNo].SetSpeed(robotCommand.nValue);
		}
		else if (RBCMD_INDEX_JMOVE == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.activated)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Activated", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (ROBOT[RobotNo].m_robotState.programRun)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Program Runnig", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.pauseState)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Pause Robot", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (ROBOT[RobotNo].m_robotState.moving)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Moving", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			//if (ROBOT[RobotNo].CanMove(NOT_DEFINED))
			//{
			//	LOG[logMMI].Message("[RBCMD_INDEX_JMOVE],[%s : idx %d]", ROBOT[RobotNo].m_name.c_str(), robotCommand.nValue);
			//	ROBOT[RobotNo].Jmove(robotCommand.nValue);
			//}
		}
		else if (RBCMD_INDEX_LMOVE == robotCommand.nCmd)
		{
			if (OPR.isDoorOpen)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Door Open", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.activated)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Not Robot Activated", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (ROBOT[RobotNo].m_robotState.programRun)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Program Runnig", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (!ROBOT[RobotNo].m_robotState.pauseState)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Pause Robot", ROBOT[RobotNo].m_name.c_str());
				return;
			}
			else if (ROBOT[RobotNo].m_robotState.moving)
			{
				MSGNOTICE.Set(MSG_NOTICE,
					"CLOSE", NULL, NULL, NULL,
					"[%s]Is Robot Moving", ROBOT[RobotNo].m_name.c_str());
				return;
			}

			if (ROBOT[RobotNo].CanMove(NOT_DEFINED))
			{
				LOG[logMMI].Message("[RBCMD_INDEX_JMOVE],[%s : idx %d]", ROBOT[RobotNo].m_name.c_str(), robotCommand.nValue);
				ROBOT[RobotNo].Move(robotCommand.nValue);
			}
		}
	}
};