#include "..\Includes.h"

namespace HW
{
	void Set(enDI ioNo, BOOL isOn)
	{
		if (NOT_DEFINED == ioNo)
			return;

		WMX.Set(ioNo, isOn);
	}

	BOOL AOn(enDI ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return WMX.AOn(ioNo, delaymS);
	}

	BOOL AOff(enDI ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return WMX.AOff(ioNo, delaymS);
	}

	BOOL BOn(enDI ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return WMX.AOff(ioNo, delaymS);
	}

	BOOL BOff(enDI ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return WMX.AOn(ioNo, delaymS);
	}

	void Set(enDO ioNo, BOOL isOn)
	{
		if (NOT_DEFINED == ioNo)
			return;

		WMX.Set(ioNo, isOn);
	}

	void On(enDO ioNo)
	{
		if (NOT_DEFINED == ioNo)
			return;

		WMX.Set(ioNo, TRUE);
	}

	void Off(enDO ioNo)
	{
		if (NOT_DEFINED == ioNo)
			return;

		WMX.Set(ioNo, FALSE);
	}

	BOOL IsOn(enDO ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return WMX.IsOn(ioNo, delaymS);
	}

	BOOL IsOff(enDO ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return WMX.IsOff(ioNo, delaymS);
	}

	double Get(enAI ioNo)
	{
		if (NOT_DEFINED == ioNo)
			return (0.);

		UINT16 chIdx = (UINT16)(ioNo / 100);
		UINT16 bit04 = (UINT16)((ioNo % 100) % 4);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit04, 0, 3))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		INT16 dataSize = 2;
		INT32 value = 0;
		BYTE Buffer[8] = { 0, };
		memcpy(&Buffer[0], WMX.m_ai[chIdx].dataAddress, sizeof(BYTE) * 8);
		memcpy(&value, &Buffer[0] + (bit04 * dataSize), dataSize);
		return (double)value;
	}

	void Set(enAO ioNo, INT32 value)
	{
		if (NOT_DEFINED == ioNo)
			return;

		UINT16 chIdx = (UINT16)(ioNo / 100);
		UINT16 bit04 = (UINT16)((ioNo % 100) % 4);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit04, 0, 3))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		INT16 dataSize = 2;
		BYTE Buffer[8] = { 0, };
		memcpy(&Buffer[0], WMX.m_ao[chIdx].dataAddress, sizeof(BYTE) * 8);
		memcpy(&Buffer[0] + bit04 * dataSize, &value, dataSize);
		memcpy(WMX.m_ao[chIdx].dataAddress, &Buffer[0], sizeof(BYTE) * 8);
	}

	void Set(enUSERMEM userMemNo, BOOL isOn)
	{
		if (NOT_DEFINED == userMemNo)
			return;

		WMX.UserMemorySet(userMemNo, isOn);
	}

	void On(enUSERMEM userMemNo)
	{
		if (NOT_DEFINED == userMemNo)
			return;

		WMX.UserMemorySet(userMemNo, TRUE);
	}

	void Off(enUSERMEM userMemNo)
	{
		if (NOT_DEFINED == userMemNo)
			return;

		WMX.UserMemorySet(userMemNo, FALSE);
	}

	BOOL IsOn(enUSERMEM userMemNo, DWORD delaymS)
	{
		if (NOT_DEFINED == userMemNo)
			return FALSE;

		return WMX.IsUserMemoryOn(userMemNo, delaymS);
	}

	BOOL IsOff(enUSERMEM userMemNo, DWORD delaymS)
	{
		if (NOT_DEFINED == userMemNo)
			return FALSE;

		return WMX.IsUserMemoryOff(userMemNo, delaymS);
	}

	void Set(seqInput ioNo, BOOL isOn)
	{
		if (NOT_DEFINED == ioNo)
			return;

		LASERIO.Set(ioNo, isOn);
	}

	BOOL AOn(seqInput ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return LASERIO.AOn(ioNo, delaymS);
	}

	BOOL AOff(seqInput ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return LASERIO.AOff(ioNo, delaymS);
	}

	BOOL BOn(seqInput ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return LASERIO.AOff(ioNo, delaymS);
	}

	BOOL BOff(seqInput ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return LASERIO.AOn(ioNo, delaymS);
	}

	void Set(seqOutput ioNo, BOOL isOn)
	{
		if (NOT_DEFINED == ioNo)
			return;

		LASERIO.Set(ioNo, isOn);
	}

	void On(seqOutput ioNo)
	{
		if (NOT_DEFINED == ioNo)
			return;

		LASERIO.Set(ioNo, TRUE);
	}

	void Off(seqOutput ioNo)
	{
		if (NOT_DEFINED == ioNo)
			return;

		LASERIO.Set(ioNo, FALSE);
	}

	BOOL IsOn(seqOutput ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return LASERIO.IsOn(ioNo, delaymS);
	}

	BOOL IsOff(seqOutput ioNo, DWORD delaymS)
	{
		if (NOT_DEFINED == ioNo)
			return FALSE;

		return LASERIO.IsOff(ioNo, delaymS);
	}
}