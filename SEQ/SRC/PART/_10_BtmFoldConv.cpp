#include "..\..\Includes.h"

CBtmFoldConv BTM_FOLD_CONV;

CBtmFoldConv::CBtmFoldConv()
{
	m_state.Init(nullptr, nullptr, StateUpdate, &m_no);
}
CBtmFoldConv::~CBtmFoldConv()
{

}

void CBtmFoldConv::StateUpdate(string oldValue, string newValue, LPVOID lpParam)
{
	if (oldValue == newValue)
		return;

	if ("FOAM_PACK_SEND" == newValue)
		BTM_FOLD_CONV.m_pSLog->XFR(newValue, "$", "BTM_FOLD_CONV", "MBB_SHUTTLE");
	else
		BTM_FOLD_CONV.m_pSLog->XFR(newValue, "$", "", "");
}

void CBtmFoldConv::Auto(void)
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

	//if (Exist() == enExistTrayTopFoamQc && TRAY_PART_DATA[nvMbbFold].exist == enExistMbb)
	//{
	//	if (!IsCanCycle())
	//		return;

	//	m_pFsm->Set(C_FOAM_PACK_SEND_START);
	//	return;
	//}
}

void CBtmFoldConv::Cycle(void)
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
		}
		return;
	}

}

void CBtmFoldConv::Init(int nIdx)
{
	m_no = nIdx;
	strcpy(m_strName, "BtmFoldConv");
	m_pFsm = &FSM[fsmBtmFoldConv];

	m_pSLog = &SLOG[slogBtmFoldConv];
	m_pSLog->SetXFRDeviceId("BTM_FOLD_CONV");
	m_pSLog->SetXFRMaterialType("SEALED_MBB");

	m_pSLog->Add(cylMbbFoldAssistUd, TRUE);
	m_pSLog->Add(cylMbbFoldAlignFtFb, TRUE);
	m_pSLog->Add(cylMbbFoldAlignRrFb, TRUE);
	m_pSLog->Add(cylMbbFoldRtFb, TRUE);
	m_pSLog->Add(cylMbbFoldRtFtOc, TRUE);
	m_pSLog->Add(cylMbbFoldRtRrOc, TRUE);

	m_pSLog->Add(CV_MBB_FOLD, TRUE);
}

BOOL CBtmFoldConv::Common(void)
{

	return TRUE;
}

BOOL CBtmFoldConv::IsHwErr(void)
{
	if (CYL[cylMbbFoldAssistUd].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldAlignFtFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldAlignRrFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldRtFb].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldRtFtOc].GetErr())
		return TRUE;
	if (CYL[cylMbbFoldRtRrOc].GetErr())
		return TRUE;

	return FALSE;
}

BOOL CBtmFoldConv::IsHwRdy(void)
{

	return FALSE;
}

BOOL CBtmFoldConv::CylInit(void)
{
	BOOL isCylInitDone = TRUE;

	if (!CYL[cylMbbFoldAssistUd].IsDn())
	{
		CYL[cylMbbFoldAssistUd].Dn(MBB_FOLD_ASSIST_DN);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylMbbFoldAlignFtFb].IsBwd() || !CYL[cylMbbFoldAlignRrFb].IsBwd())
	{
		CYL[cylMbbFoldAlignFtFb].Bwd(MBB_FOLD_ALIGN_FT_BWD);
		CYL[cylMbbFoldAlignRrFb].Bwd(MBB_FOLD_ALIGN_RR_BWD);
		isCylInitDone = FALSE;
	}

	if (!CYL[cylMbbFoldRtFb].IsBwd())
	{
		CYL[cylMbbFoldRtFb].Bwd(MBB_FOLD_RT_BWD);
		isCylInitDone = FALSE;
	}
	if (!CYL[cylMbbFoldRtFtOc].IsOpen() || !CYL[cylMbbFoldRtRrOc].IsOpen())
	{
		CYL[cylMbbFoldRtFtOc].Open(MBB_FOLD_RT_FT_OPEN);
		CYL[cylMbbFoldRtRrOc].Open(MBB_FOLD_RT_RR_OPEN);
		isCylInitDone = FALSE;
	}

	return isCylInitDone;
}

BOOL CBtmFoldConv::IsCanCycle(void)
{

	return TRUE;
}

BOOL CBtmFoldConv::ExistCrossCheck(void)
{
	if (!MBB_QC.m_pFsm->Between(MBB_QC.C_MBB_QC_SEND_START, MBB_QC.C_MBB_QC_SEND_END) &&
		!FOLD_PK.m_pFsm->Between(FOLD_PK.C_PICK_UP_START, FOLD_PK.C_PICK_UP_END))
	{
		if (!Exist() && AOn(iMbbFoldConvExist))
		{
			ER.Save(ER_MBB_FOLD_SENSOR_STATUS);
			return TRUE;
		}
		if (Exist() && AOff(iMbbFoldConvExist))
		{
			ER.Save(ER_MBB_FOLD_MEMORY_STATUS);
			return TRUE;
		}
	}

	return FALSE;
}

int& CBtmFoldConv::Exist(void)
{
	return TRAY_PART_DATA[nvMbbFold].exist;
}

BOOL CBtmFoldConv::IsExist(void)
{
	if (!TRAY_PART_DATA[nvMbbFold].exist || AOff(iMbbFoldConvExist))
	{
		ER.Save(ER_MBB_FOLD_EMPTY);
		return FALSE;
	}

	return TRUE;
}

BOOL CBtmFoldConv::IsEmpty(void)
{
	if (TRAY_PART_DATA[nvMbbFold].exist || AOn(iMbbFoldConvExist))
	{
		ER.Save(ER_MBB_FOLD_EXIST);
		return FALSE;
	}

	return TRUE;
}

