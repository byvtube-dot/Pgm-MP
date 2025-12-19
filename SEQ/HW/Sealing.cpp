#include "..\Includes.h"
#include "Sealing.h"

void CSealing::Init(int nIndex, SEALDATA::ReadRegister* pInputArray, SEALDATA::WriteRegister* pOutputArray)
{
	m_no = nIndex;
	m_pReadData = pInputArray;
	m_pWriteData = pOutputArray;

}

void CSealing::Update(void)
{
	GetState();
	SetMMIDisplay();
}

void CSealing::GetState(void)
{
	for (int i = 0; i < enContorlFlagMax; i++)
	{
		if (!IsSetCtrlFlagOn(i))
			m_tmCtrlFlag[i].Reset();
	}

	if (m_pReadData->machineStatus == 4)	// Process Run
	{
		if (IsSetCtrlFlagOn(enOutStart))
			SetCtrlFlagOn(enOutStart, false);

		if (IsSetCtrlFlagOn(enOutStop))
			SetCtrlFlagOn(enOutStop, false);

		if (IsSetCtrlFlagOn(enOutAbort))
			SetCtrlFlagOn(enOutAbort, false);
	}

	if (m_pReadData->machineStatus == 2)	// Initalistation Run
	{
		if (IsSetCtrlFlagOn(enOutQuit))
			SetCtrlFlagOn(enOutQuit, false);

		if (IsSetCtrlFlagOn(enOutInit))
			SetCtrlFlagOn(enOutInit, false);
	}
}

int CSealing::GetResult(void)
{
	if (m_pReadData->ioStatus & BIT_MASK[enInGood] &&
		m_pReadData->ioStatus & BIT_MASK[enInBad])
		return 2;
	else if (m_pReadData->ioStatus & BIT_MASK[enInGood])
		return 0;
	else if (m_pReadData->ioStatus & BIT_MASK[enInBad])
		return 1;
	else
		return 3;
}

int CSealing::GetError(void)
{
	if (!Between(m_pReadData->machineMode, 0, 2))
	{
		if (m_pReadData->machineMode == 10)
			return ER_SEALING_INIT_REQ;
		else if (m_pReadData->machineMode == 11)
			return ER_SEALING_SCAN_REQ;
		else if (m_pReadData->machineMode == 12)
			return ER_SEALING_SELF_CHECK_REQ;
		else if (m_pReadData->machineMode == 20)
			return ER_SEALING_ENABLE_OFF;
		else
			return ER_SEALING_ABNORMAL_MODE;
	}
	
	if (m_pWriteData->controlFlags & BIT_MASK[enOutAbort])
		return ER_SEALING_MANUAL_ABORT;

	if ((m_pReadData->machineStatus == 0) &&
		(m_pReadData->ioStatus & BIT_MASK[enInBad]))
		return ER_SEALING_RESULT_ABORT;

	return 0;
}

void CSealing::SetMMIDisplay(void)
{
	NV.ndm[sealInReady] = m_pReadData->ioStatus & BIT_MASK[enInReady];
	NV.ndm[sealInError] = m_pReadData->ioStatus & BIT_MASK[enInError];
	NV.ndm[sealInGood] = m_pReadData->ioStatus & BIT_MASK[enInGood];
	NV.ndm[sealInBad] = m_pReadData->ioStatus & BIT_MASK[enInBad];
	NV.ndm[sealInEnable] = m_pReadData->ioStatus & BIT_MASK[enInEnable];
	NV.ndm[sealInStart] = m_pReadData->ioStatus & BIT_MASK[enInStart];
	NV.ndm[sealInReset] = m_pReadData->ioStatus & BIT_MASK[enInReset];

	NV.ndm[sealenOutAbort] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutAbort);
	NV.ndm[sealenOutStart] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutStart);
	NV.ndm[sealenOutCalib] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutCalib);
	NV.ndm[sealenOutShakeOff] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutShakeOff);
	NV.ndm[sealenOutInit] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutInit);
	NV.ndm[sealenOutStop] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutStop);
	NV.ndm[sealenOutQuit] = SEAL[sealMbb].IsSetCtrlFlagOn(enOutQuit);

}


void CSealing::SetCtrlFlagOn(int bit, bool isOn)
{
	if (!Between(bit, 0, enContorlFlagMax))
		return;

	if (isOn)
		m_pWriteData->controlFlags |= BIT_MASK[bit];  // 해당 비트 활성화
	else
		m_pWriteData->controlFlags &= ~BIT_MASK[bit];  // 해당 비트 비활성화
}

BOOL CSealing::IsSetCtrlFlagOn(int bit, bool isOn)
{
	if (!Between(bit, 0, enContorlFlagMax))
		return FALSE;

	if (isOn)
	{
		if (m_pWriteData->controlFlags & BIT_MASK[bit])
			return TRUE;
	}
	else
	{
		if (m_pWriteData->controlFlags & ~BIT_MASK[bit])
			return TRUE;
	}
	
	return FALSE;
}
void CSealing::ResetCtrlFlagOn(int bit)
{
	if (Between(bit, 0, enContorlFlagMax))
		m_pWriteData->controlFlags &= ~BIT_MASK[bit];  // 해당 비트 활성화
}

void CSealing::ResetProcess(void)
{
	if (m_pReadData->machineStatus == 6)
		SetInit();

	if (m_pReadData->machineError != 0)
		Quit();
}

void CSealing::Abort()
{
	SetCtrlFlagOn(enOutAbort);
}

BOOL CSealing::CanStart()
{
	if (BOn(iBSealcontactDetected))		// CC_100
		return FALSE;

	if (!(m_pReadData->ioStatus & BIT_MASK[enInReady]))
		return FALSE;

	if (m_pReadData->machineStatus != 3)
		return FALSE;

	return TRUE;
}

void CSealing::Start()
{
	if (CanStart())
		SetCtrlFlagOn(enOutStart);
}

void CSealing::Calib()
{
	SetCtrlFlagOn(enOutCalib);
}

void CSealing::Shakeoff()
{
	SetCtrlFlagOn(enOutShakeOff);
}

void CSealing::SetInit()
{
	SetCtrlFlagOn(enOutInit);
}

void CSealing::Stop()
{
	SetCtrlFlagOn(enOutStop);
}

BOOL CSealing::CanQuit()
{
	if (m_pReadData->ioStatus & BIT_MASK[enInError])
		return FALSE;

	return TRUE;
}

void CSealing::Quit()
{
	SetCtrlFlagOn(enOutQuit);
}

// Set Input
void CSealing::SetInReady(bool isOn)
{
	// Bit 0: Ready ( 0= system not ready for start, 1= system is ready for start) ; during welding process Ready=0 
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInReady];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInReady];
}

void CSealing::SetInError(bool isOn)
{
	// Bit 1: Error ( 0= Error, 1= no Error) 
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInError];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInError];
}

void CSealing::SetInGood(bool isOn)
{
	// Bit 2: Good (last limit check result), is set until the next weld cycle ; 1= Good 0= not Good 
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInGood];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInGood];
}

void CSealing::SetInBad(bool isOn)
{
	// Bit 3: Bad (last limit check result), is set until the next weld cycle ; 1= Bad 0= not Bad
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInBad];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInBad];
}

void CSealing::SetInEnable(bool isOn)
{
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInEnable];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInEnable];
}

void CSealing::SetInStart(bool isOn)
{
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInStart];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInStart];
}

void CSealing::SetInReset(bool isOn)
{
	if (isOn)
		m_pReadData->ioStatus |= BIT_MASK[enInReset];
	else
		m_pReadData->ioStatus &= ~BIT_MASK[enInReset];
}