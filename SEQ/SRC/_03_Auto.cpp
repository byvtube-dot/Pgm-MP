#include "..\Includes.h"

void AutoSequence()
{
	if (FSM[fsmSEQ].IsBusy())
	{
		OPR.isAuto = (FALSE);
		if (FSM[fsmSEQ].IsBusy(C_ERROR))
		{
			FSM[fsmSEQ].Set(C_IDLE);
			return;
		}

		LotEndCycle();
		AllHomeCycle();
		OperationCycle();
		return;
	}

	IN_PK.Auto();
	FOAM_RAIL.Auto();
	FOAM_ELEV[enFoamRecvElev].Auto();
	FOAM_ELEV[enFoamPartialElev].Auto();
	FOAM_ELEV[enFoamSendElev].Auto();
	FOAM_STACK_CONV.Auto();
	FOAM_PK.Auto();
	FOAM_CST[enFoamBoxLt].Auto();
	FOAM_CST[enFoamBoxRt].Auto();
	TRAY_SHUTTLE.Auto();
	MBB_SHUTTLE.Auto();
	MBB_QC.Auto();
	BTM_FOLD_CONV.Auto();
	FOLD_PK.Auto();
	LABEL_FEEDER[enlabelLt].Auto();
	LABEL_FEEDER[enlabelRt].Auto();
	LABEL_LASER.Auto();
	LABEL_SHUTTLE[enlabelLt].Auto();
	LABEL_SHUTTLE[enlabelRt].Auto();
	MBB_STACK[enMbbCstLt].Auto();
	MBB_STACK[enMbbCstRt].Auto();
	LT_COBOT.Auto();
	RT_COBOT.Auto();

	StartStop();

	IN_PK.Cycle();
	FOAM_RAIL.Cycle();
	FOAM_ELEV[enFoamRecvElev].Cycle();
	FOAM_ELEV[enFoamPartialElev].Cycle();
	FOAM_ELEV[enFoamSendElev].Cycle();
	FOAM_STACK_CONV.Cycle();
	FOAM_PK.Cycle();
	FOAM_CST[enFoamBoxLt].Cycle();
	FOAM_CST[enFoamBoxRt].Cycle();
	TRAY_SHUTTLE.Cycle();
	MBB_SHUTTLE.Cycle();
	MBB_QC.Cycle();
	BTM_FOLD_CONV.Cycle();
	FOLD_PK.Cycle();
	LABEL_FEEDER[enlabelLt].Cycle();
	LABEL_FEEDER[enlabelRt].Cycle();
	LABEL_LASER.Cycle();
	LABEL_SHUTTLE[enlabelLt].Cycle();
	LABEL_SHUTTLE[enlabelRt].Cycle();
	MBB_STACK[enMbbCstLt].Cycle();
	MBB_STACK[enMbbCstRt].Cycle();
	LT_COBOT.Cycle();
	RT_COBOT.Cycle();
}

void StartStop(void)
{
	if (OPR.isStop)
	{
		if (OPR.isAuto)
		{
			NV_MEMORY.Flush();
			NV_TRACEABILITY.Flush();
			NV_STAGE_DATA.Flush();
			NV_TRAY_PART_DATA.Flush();
			NV_COBOT_PART_DATA.Flush();
			NV_FOAM_PART_DATA.Flush();
			NV_LABEL_PART_DATA.Flush();
			NV_MBB_PART_DATA.Flush();
			NV_ROBOT.Flush();
			NV_FIRE_DETECTOR_DATA.Flush();
			NV_SEQ_VERSION.Flush();
		}
		OPR.isAuto = false;
	}
	else if (OPR.isAuto)
	{
		if (!OPR.isDoorUnlock)
		{
			IN_PK.m_isRun = TRUE;
			FOAM_RAIL.m_isRun = TRUE;
			FOAM_ELEV[enFoamRecvElev].m_isRun = TRUE;
			FOAM_ELEV[enFoamPartialElev].m_isRun = TRUE;
			FOAM_ELEV[enFoamSendElev].m_isRun = TRUE;
			FOAM_STACK_CONV.m_isRun = TRUE;
			FOAM_PK.m_isRun = TRUE;
			FOAM_CST[enFoamBoxLt].m_isRun = TRUE;
			FOAM_CST[enFoamBoxRt].m_isRun = TRUE;
			TRAY_SHUTTLE.m_isRun = TRUE;
			MBB_SHUTTLE.m_isRun = TRUE;
			MBB_QC.m_isRun = TRUE;
			BTM_FOLD_CONV.m_isRun = TRUE;
			FOLD_PK.m_isRun = TRUE;
			LABEL_FEEDER[enlabelLt].m_isRun = TRUE;
			LABEL_FEEDER[enlabelRt].m_isRun = TRUE;
			LABEL_LASER.m_isRun = TRUE;
			LABEL_SHUTTLE[enlabelLt].m_isRun = TRUE;
			LABEL_SHUTTLE[enlabelRt].m_isRun = TRUE;
			MBB_STACK[enMbbCstLt].m_isRun = TRUE;
			MBB_STACK[enMbbCstRt].m_isRun = TRUE;
			LT_COBOT.m_isRun = TRUE;
			RT_COBOT.m_isRun = TRUE;
		}
	}
}