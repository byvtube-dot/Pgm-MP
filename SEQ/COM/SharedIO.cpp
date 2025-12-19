#include "..\Includes.h"

namespace COM
{
	SharedIO::SharedIO()
	{
		ZeroMemory(iBitMem, sizeof(iBitMem));
		ZeroMemory(oBitMem, sizeof(oBitMem));
	}

	SharedIO::~SharedIO()
	{

	}

	void SharedIO::Update(void)
	{
		Set(oLaserEmo, !IsEMO());
		for (int ch = 0; ch < MAX_LASER_DI_CH; ch++)
		{
			for (int no = 0; no < MAX_CONTACT; no++)
			{
				seqInput iNo = (seqInput)((ch * 100) + no);
				if (AOn(iNo))
				{
					if (!(iBitMem[ch] & g_bitMask16[no]))
					{
						LOG[logLASERIO].Message("input,%.4d,on", (ch * 100) + no);
						iBitMem[ch] |= g_bitMask16[no];
					}
					tiOff[ch][no].Reset();
				}
				else
				{
					if (iBitMem[ch] & g_bitMask16[no])
					{
						LOG[logLASERIO].Message("input,%.4d,off", (ch * 100) + no);
						iBitMem[ch] &= ~g_bitMask16[no];
					}
					tiOn[ch][no].Reset();
				}
			}
		}

		for (int ch = 0; ch < MAX_LASER_DO_CH; ch++)
		{
			for (int no = 0; no < MAX_CONTACT; no++)
			{
				seqOutput oNo = (seqOutput)((ch * 100) + no);
				if (IsOn(oNo))
				{
					if (!(oBitMem[ch] & g_bitMask16[no]))
					{
						LOG[logLASERIO].Message("output,%.4d,on", (ch * 100) + no);
						oBitMem[ch] |= g_bitMask16[no];
					}
					toOff[ch][no].Reset();
				}
				else
				{
					if (oBitMem[ch] & g_bitMask16[no])
					{
						LOG[logLASERIO].Message("output,%.4d,off", (ch * 100) + no);
						oBitMem[ch] &= ~g_bitMask16[no];
					}
					toOn[ch][no].Reset();
				}
			}
		}
	}

	BOOL SharedIO::AOn(seqInput ioNo, DWORD dwDelayTime)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % 16);

		if (SHARED_MEM.inCh[chIdx] & g_bitMask16[bit16])
		{
			if (!dwDelayTime || tiOn[chIdx][bit16].TmOver(dwDelayTime))
				return (TRUE);
		}
		return (FALSE);
	}

	BOOL SharedIO::AOff(seqInput ioNo, DWORD dwDelayTime)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % 16);

		if (!AOn(ioNo))
		{
			if (!dwDelayTime || tiOff[chIdx][bit16].TmOver(dwDelayTime))
				return (TRUE);
		}
		return (FALSE);
	}

	void SharedIO::Set(seqInput ioNo, BOOL isOn)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % 16);

		if (isOn)
			SHARED_MEM.inCh[chIdx] |= g_bitMask16[bit16];
		else
			SHARED_MEM.inCh[chIdx] &= ~g_bitMask16[bit16];
	}

	BOOL SharedIO::IsOn(seqOutput ioNo, DWORD dwDelayTime)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % 16);

		if (SHARED_MEM.outCh[chIdx] & g_bitMask16[bit16])
		{
			if (!dwDelayTime || toOn[chIdx][bit16].TmOver(dwDelayTime))
				return (TRUE);
		}
		return (FALSE);
	}

	BOOL SharedIO::IsOff(seqOutput ioNo, DWORD dwDelayTime)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % 16);

		if (!IsOn(ioNo))
		{
			if (!dwDelayTime || toOff[chIdx][bit16].TmOver(dwDelayTime))
				return (TRUE);
		}
		return (FALSE);
	}

	void SharedIO::Set(seqOutput ioNo, BOOL isOn)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % 16);

		if (isOn)
			SHARED_MEM.outCh[chIdx] |= g_bitMask16[bit16];
		else
			SHARED_MEM.outCh[chIdx] &= ~g_bitMask16[bit16];
	}

	void SharedIO::On(seqOutput ioNo)
	{
		Set(ioNo, TRUE);
	}

	void SharedIO::Off(seqOutput ioNo)
	{
		Set(ioNo, FALSE);
	}
}