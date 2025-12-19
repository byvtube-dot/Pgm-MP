#include "..\Includes.h"

void AllHomeCycle(void)
{
	if (!FSM[fsmSEQ].Between(C_ALLHOME_START, C_ALLHOME_END))
		return;

	int nSlave;
	for (int gtNo = 0; gtNo < gtMAX; gtNo++)
	{
		nSlave = GT[gtNo].m_slave;
		MT[nSlave].m_state.isHome = TRUE;
	}

	// TODO: Check
	if (0 < ER.GetNo())
	{
		for (int i = 0; i < MAX_ERROR_ARRAY; i++)
		{
			if (0 == ER.GetNo(i))
				continue;

			if (!Between(ER.GetNo(i), ER_MT_ALL_HOME, ER_MT_ALL_HOME + 129))
			{
				for (int mtno = 0; mtno < motorMAX; mtno++)
					MT[AxisList(mtno)].CancelHomeSearch();

				ER.Save(ER_ALLHOME_CYCLE_CANCEL);
				FSM[fsmSEQ].Set(C_ERROR);
				return;
			}
		}
	}

	if (FSM[fsmSEQ].TimeLimit(_5min))
	{
		for (int mtno = 0; mtno < motorMAX; mtno++)
			MT[AxisList(mtno)].CancelHomeSearch();

		ER.Save(ER_ALLHOME_CYCLE_TM_OVER);
		FSM[fsmSEQ].Set(C_ERROR);
		return;
	}

	switch (FSM[fsmSEQ].Get())
	{
		case C_ALLHOME_START:
		{
			if (FSM[fsmSEQ].Once())
			{
				LOG[logSEQ].Message("C_ALL_HOME_START");
			}

			//CYL[cNG_TRAY_ALIGN_X].Bwd();
			//CYL[cNG_TRAY_ALIGN_Y].Bwd();
			//CYL[cGOOD_TRAY_ALIGN_X].Bwd();
			//CYL[cGOOD_TRAY_ALIGN_Y].Bwd();

			//if (!CYL[cNG_TRAY_ALIGN_X].IsBwd() || !CYL[cNG_TRAY_ALIGN_Y].IsBwd())
			//	break;
			//if (!CYL[cGOOD_TRAY_ALIGN_X].IsBwd() || !CYL[cGOOD_TRAY_ALIGN_Y].IsBwd())
			//	break;

			FSM[fsmSEQ].Set(C_ALLHOME_END);
		}
		break;
		case C_ALLHOME_END:
		{
			if (FSM[fsmSEQ].Once())
			{
				LOG[logSEQ].Message("C_ALL_HOME_END");
			}

			BOOL isComp = TRUE;

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

				if (MT[mtno].m_state.isHome || MT[mtno].m_fsmDrv.IsBusy())
					continue;

				//isComp = FALSE;
				MT[mtno].StartHomeCommand();
			}

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

				if (MT[mtno].IsRdy(MT[mtno].m_homePosIdx))
					continue;

				isComp = FALSE;
			}

			if (isComp)
			{
				LOG[logSEQ].Message("Home function complete!!!");
				FSM[fsmSEQ].Set(C_IDLE);

				OPR.isCheckErr = (FALSE);
				ER.Clear();
				WR.Clear();
			}
		}
		break;
	}
}

void MtHome(AxisList mtno)
{
	OPR.isCanMoveMsg = (TRUE);
	if (!MT[mtno].CanMove(HOME_CANMOVE) || MT[mtno].isVirtualSW || MT[mtno].isVirtualHW)
		return;
	MT[mtno].CancelHomeSearch();
	MT[mtno].StartHomeCommand();
	OPR.isCanMoveMsg = (FALSE);
}