#include "..\Includes.h"
#include "SealingLib.h"

void CSealingLib::Init(void)
{
	memset(m_ReadData, 0, sizeof(m_ReadData));
	memset(m_WriteData, 0, sizeof(m_WriteData));

	for (int nSealNo = 0; nSealNo < SEAL_MAX; ++nSealNo)
		SEAL[nSealNo].Init(nSealNo, &m_ReadData[nSealNo], &m_WriteData[nSealNo]);
}

void CSealingLib::ReadInput(void)
{
	if (OPR.isNoDevice)
		return;

	for (int nSealNo = 0; nSealNo < SEAL_MAX; ++nSealNo)
		memcpy(&m_ReadData[nSealNo], WMX.m_si[nSealNo].dataAddress, sizeof(SEALDATA::ReadRegister));

}

void CSealingLib::WriteOutput(void)
{
	for (int nSealNo = 0; nSealNo < SEAL_MAX; ++nSealNo)
		memcpy(WMX.m_so[nSealNo].dataAddress, &m_WriteData[0], sizeof(SEALDATA::WriteRegister));
}

void CSealingLib::Update(void)
{
	if (!WMX.isConnected)
		return;

	GetState();
	GetErr();

	WriteOutput();
	ReadInput();

	for (int nSealNo = 0; nSealNo < SEAL_MAX; ++nSealNo)
		SEAL[nSealNo].Update();
}

void CSealingLib::GetState(void)
{
}

void CSealingLib::GetErr(void)
{
	
}
