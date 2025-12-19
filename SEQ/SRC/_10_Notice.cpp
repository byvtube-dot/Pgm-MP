#include "..\Includes.h"

BOOL NoticeProcess(void)
{
	switch (NV.ndm[msgSeq2Mmi])
	{
	case MSG_NOTICE:
	{
	}
	break;

	case MSG_HOME_FAIL:
	{

	}
	break;

	case MSG_OP_BREAK:
	{
		if (RECEIVE_CMD_BTN1 == NV.ndm[msgMmi2Seq])
		{
			AllCycleOff();
		}
	}
	break;

	case MSG_INLET_VI_ORG_FAIL:
	{
		//if (RECEIVE_CMD_BTN1 == NV.ndm[MSG_MMI2SEQ])
		//{
		//	if (!INPICK.m_pFsm->IsBusy())
		//	{
		//		INPICK.m_pFsm->Set(InPick::C_ORG_ERR);
		//	}
		//}
	}
	break;

	case MSG_INLET_VI_ALIGN_FAIL:
	{
		//if (RECEIVE_CMD_BTN1 == NV.ndm[MSG_MMI2SEQ])
		//{
		//	if (!INPICK.m_pFsm->IsBusy())
		//	{
		//		INPICK.m_pFsm->Set(InPick::C_ALIGN_ERR);
		//	}
		//}
	}
	break;

	case MSG_OP_UNITWAIT:
	{
		//if (RECEIVE_CMD_BTN1 == NV.ndm[MSG_MMI2SEQ])
		//{
		//	g_tmUnitWait.Reset();
		//}
		//else if (RECEIVE_CMD_BTN2 == NV.ndm[MSG_MMI2SEQ])
		//{
		//	if (NV.waferInfo[nvWaferFd1].exist != enWaferEmpty)
		//	{
		//		NV.ndm[MemPreAttach1] = TRUE;
		//		NV.ndm[MemMainAttach1] = TRUE;
		//	}

		//	if (NV.waferInfo[nvWaferFd2].exist != enWaferEmpty)
		//	{
		//		NV.ndm[MemPreAttach2] = TRUE;
		//		NV.ndm[MemMainAttach2] = TRUE;
		//	}
		//	g_tmUnitWait.Reset();
		//	g_bWaitDryRun = TRUE;
		//}
	}
	break;

	case MSG_INPK_PICKUP_FAIL:
	{
		if (RECEIVE_CMD_BTN1 == NV.ndm[msgMmi2Seq])
		{
			//if (AOn(iIN_TRAY_LEVEL, 10))
			//	NV.ndm[EXIST_IN_TRAY] = 2;
		}
	}
	break;
	}

	return (FALSE);
}