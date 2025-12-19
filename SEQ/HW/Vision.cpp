#include "..\Includes.h"

namespace HW
{
	Vision::Vision()
	{
		//for (int i = 0; i < MAX_INDEX_VISION; i++)
		//	m_indexName.Add("");	
		
		isVirtual = (VI_MAX > 0) ? 0 : 1;
		m_no = NOT_DEFINED;
		m_isWaitData.Init(nullptr, nullptr, SLogEvent, &m_no);
		m_isRcvComp.Init(nullptr, nullptr, CompEvent, &m_no);
	}

	Vision::~Vision()
	{

	}

	void Vision::CompEvent(bool oldValue, bool newValue, LPVOID lpParam)
	{
		int& no = *reinterpret_cast<int*>(lpParam);
		VisionList vino = (VisionList)no;
		if (oldValue && !newValue)
		{

		}
		else if (!oldValue && newValue)
		{
			VI[vino].m_isWaitData = false;
		}
	}

	void Vision::SLogEvent(bool oldValue, bool newValue, LPVOID lpParam)
	{
		int& no = *reinterpret_cast<int*>(lpParam);
		VisionList vino = (VisionList)no;
		if (oldValue && !newValue)
		{
			VI[vino].SLogEndEvent();
		}
		else if (!oldValue && newValue)
		{
			VI[vino].SLogEndEvent();
			VI[vino].SLogStartEvent();

		}
	}

	void Vision::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\VisionName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < VI_MAX; i++)
		{
			VisionList no = VisionList(i);
			VI[no].m_no = i;

			if (!CanWrite)
				continue;

			ZeroMemory(strSection, sizeof(strSection));
			ZeroMemory(strKeyCode, sizeof(strKeyCode));
			ZeroMemory(strStrName, sizeof(strStrName));
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VisionName.ini");
			swprintf_s(strKeyCode, 100, L"VISION%d", i);
			ini.GetString(L"VISIONNAME", strKeyCode, strStrName, 512);
			VI[no].m_name = GetAscCode(strStrName);
		}
	}

	void Vision::SLogStartEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(VisionList(m_no)) && SLOG[no].visionLogStart[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].visionLogStart[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].visionLogStart[m_no].GetXFREventId() + "_" + m_name + "_INSPETION").c_str(), "START",
					SLOG[no].visionLogStart[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].visionLogStart[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'VISION'", "'ACT_NAME'", ("'VI" + ToString(m_no) + "'").c_str());
				SLOG[no].visionLogEnd[m_no] = SLOG[no].visionLogStart[m_no];
			}
		}
	}

	void Vision::SLogEndEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].visionLogEnd[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].visionLogEnd[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].visionLogEnd[m_no].GetXFREventId() + "_" + m_name + "_INSPETION").c_str(), "END",
					SLOG[no].visionLogEnd[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].visionLogEnd[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'VISION'", "'ACT_NAME'", ("'VI" + ToString(m_no) + "'").c_str());
				SLOG[no].visionLogEnd[m_no].End(TRUE);
			}
		}
	}

	BOOL Vision::SetIO(enDI iAuto, enDI iBusy,
		enDI iMode1, enDI iMode2, enDI iMode3, enDI iMode4, enDI iMode5,
		enDO oStart, enDO oReset, enDO oLive,
		enDO oMode1, enDO oMode2, enDO oMode3, enDO oMode4, enDO oMode5)
	{
		m_iAuto = iAuto;
		m_iBusy = iBusy;
		m_iPass = (enDI)NOT_DEFINED;
		m_iFail = (enDI)NOT_DEFINED;
		m_iStop = (enDI)NOT_DEFINED;
		m_iMode1 = iMode1;
		m_iMode2 = iMode2;
		m_iMode3 = iMode3;
		m_iMode4 = iMode4;
		m_iMode5 = iMode5;

		m_oStart = oStart;
		m_oReset = oReset;
		m_oLive = oLive;
		m_oMode1 = oMode1;
		m_oMode2 = oMode2;
		m_oMode3 = oMode3;
		m_oMode4 = oMode4;
		m_oMode5 = oMode5;

		return (TRUE);
	}

	BOOL Vision::SetIO(enDI iAuto, enDI iBusy, enDI iPass, enDI iFail, enDI iStop,
		enDI iMode1, enDI iMode2, enDI iMode3, enDI iMode4, enDI iMode5,
		enDO oStart, enDO oReset, enDO oLive,
		enDO oMode1, enDO oMode2, enDO oMode3, enDO oMode4, enDO oMode5)
	{
		m_iAuto = iAuto;
		m_iBusy = iBusy;
		m_iPass = iPass;
		m_iFail = iFail;
		m_iStop = iStop;
		m_iMode1 = iMode1;
		m_iMode2 = iMode2;
		m_iMode3 = iMode3;
		m_iMode4 = iMode4;
		m_iMode5 = iMode5;

		m_oStart = oStart;
		m_oReset = oReset;
		m_oLive = oLive;
		m_oMode1 = oMode1;
		m_oMode2 = oMode2;
		m_oMode3 = oMode3;
		m_oMode4 = oMode4;
		m_oMode5 = oMode5;

		return (TRUE);
	}

	void Vision::Update(void)
	{
		if (isVirtual || OPR.isDebugMode || NOT_DEFINED == m_no)
			return;

		if (OPR.isNoDevice)
		{
			Set(m_iBusy, IsOn(m_oStart, 100));
			if (NOT_DEFINED != m_iMode1 && NOT_DEFINED != m_oMode1)
				Set(m_iMode1, IsOn(m_oMode1));
			if (NOT_DEFINED != m_iMode2 && NOT_DEFINED != m_oMode2)
				Set(m_iMode2, IsOn(m_oMode2));
			if (NOT_DEFINED != m_iMode3 && NOT_DEFINED != m_oMode3)
				Set(m_iMode3, IsOn(m_oMode3));
			if (NOT_DEFINED != m_iMode4 && NOT_DEFINED != m_oMode4)
				Set(m_iMode4, IsOn(m_oMode4));
			if (NOT_DEFINED != m_iMode5 && NOT_DEFINED != m_oMode5)
				Set(m_iMode5, IsOn(m_oMode5));
		}

		m_tmReservedReset.Reset(!m_isReservedReset);
		if (m_isReservedReset && m_tmReservedReset.TmOver(100) || IsOn(m_oReset))
		{
			On(m_oReset);
			m_isReservedReset = 0;
		}
		if (m_isWaitData && IsOff(m_oStart) && AOff(m_iBusy))
			m_isWaitData = false;		

		if (AOn(m_iBusy) || IsOn(m_oReset, 100))
			Off(m_oReset);

		if (AOn(m_iBusy) || IsOn(m_oStart, 5000))
			Off(m_oStart);
	}

	BOOL Vision::IsAuto()
	{
		return AOn(m_iAuto);
	}

	BOOL Vision::IsStart()
	{
		return IsOn(m_oStart);
	}

	BOOL Vision::Start(int mode)
	{
		if (isVirtual || NOT_DEFINED != mode && !IsMode(mode))
			return FALSE;

		if (NOT_DEFINED != m_iBusy && AOn(m_iBusy))
			return FALSE;

		if (NOT_DEFINED != m_oLive && IsOn(m_oLive))
			return FALSE;

		if (!m_isWaitData && IsOff(m_oStart) && IsOff(m_oReset) && !m_isReservedReset)
		{
			On(m_oStart);

			m_isRcvComp = false;
			m_isWaitData = true;
			return TRUE;
		}
		return FALSE;
	}

	BOOL Vision::IsBusy()
	{
		return (AOn(m_iBusy) || IsOn(m_oStart));
	}

	void Vision::Reset(BOOL isReservedReset)
	{
		Clear();
		if (isReservedReset)
			m_isReservedReset = 1;
		else
			On(m_oReset);
	}

	void Vision::Clear(void)
	{
		m_isWaitData = false;
		ZeroMemory(&m_prsData , sizeof(m_prsData));
		ZeroMemory(&m_qcData , sizeof(m_qcData));
		ZeroMemory(&m_unitData , sizeof(m_unitData));
		Off(m_oStart);
	}

	void Vision::SetMode(UINT mode)
	{
		if (isVirtual)
			return;

		if (NOT_DEFINED != m_oMode1)
		{
			if (g_bitMask16[0] & mode)
				On(m_oMode1);
			else
				Off(m_oMode1);
		}

		if (NOT_DEFINED != m_oMode2)
		{
			if (g_bitMask16[1] & mode)
				On(m_oMode2);
			else
				Off(m_oMode2);
		}

		if (NOT_DEFINED != m_oMode3)
		{
			if (g_bitMask16[2] & mode)
				On(m_oMode3);
			else
				Off(m_oMode3);
		}

		if (NOT_DEFINED != m_oMode4)
		{
			if (g_bitMask16[3] & mode)
				On(m_oMode4);
			else
				Off(m_oMode4);
		}

		if (NOT_DEFINED != m_oMode5)
		{
			if (g_bitMask16[4] & mode)
				On(m_oMode5);
			else
				Off(m_oMode5);
		}
	}

	BOOL Vision::IsModeOn(UINT mode, DWORD dDelay)
	{
		if (isVirtual)
			return (FALSE);

		if (g_bitMask16[0] & mode)
		{
			if (NOT_DEFINED != m_oMode1 && IsOff(m_oMode1, dDelay))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_oMode1 && IsOn(m_oMode1, dDelay))
				return (FALSE);
		}

		if (g_bitMask16[1] & mode)
		{
			if (NOT_DEFINED != m_oMode2 && IsOff(m_oMode2, dDelay))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_oMode2 && IsOn(m_oMode2, dDelay))
				return (FALSE);
		}

		if (g_bitMask16[2] & mode)
		{
			if (NOT_DEFINED != m_oMode3 && IsOff(m_oMode3, dDelay))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_oMode3 && IsOn(m_oMode3, dDelay))
				return (FALSE);
		}

		if (g_bitMask16[3] & mode)
		{
			if (NOT_DEFINED != m_oMode4 && IsOff(m_oMode4, dDelay))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_oMode4 && IsOn(m_oMode4, dDelay))
				return (FALSE);
		}

		if (g_bitMask16[4] & mode)
		{
			if (NOT_DEFINED != m_oMode5 && IsOff(m_oMode5, dDelay))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_oMode5 && IsOn(m_oMode5, dDelay))
				return (FALSE);
		}

		return (TRUE);
	}

	BOOL Vision::IsMode(UINT mode)
	{
		if (isVirtual)
			return (FALSE);

		if (g_bitMask16[0] & mode)
		{
			if (NOT_DEFINED != m_iMode1 && AOff(m_iMode1))
				return (FALSE);
			if (NOT_DEFINED != m_oMode1 && IsOff(m_oMode1))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_iMode1 && AOn(m_iMode1))
				return (FALSE);
			if (NOT_DEFINED != m_oMode1 && IsOn(m_oMode1))
				return (FALSE);
		}

		if (g_bitMask16[1] & mode)
		{
			if (NOT_DEFINED != m_iMode2 && AOff(m_iMode2))
				return (FALSE);
			if (NOT_DEFINED != m_oMode2 && IsOff(m_oMode2))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_iMode2 && AOn(m_iMode2))
				return (FALSE);
			if (NOT_DEFINED != m_oMode2 && IsOn(m_oMode2))
				return (FALSE);
		}

		if (g_bitMask16[2] & mode)
		{
			if (NOT_DEFINED != m_iMode3 && AOff(m_iMode3))
				return (FALSE);
			if (NOT_DEFINED != m_oMode3 && IsOff(m_oMode3))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_iMode3 && AOn(m_iMode3))
				return (FALSE);
			if (NOT_DEFINED != m_oMode3 && IsOn(m_oMode3))
				return (FALSE);
		}

		if (g_bitMask16[3] & mode)
		{
			if (NOT_DEFINED != m_iMode4 && AOff(m_iMode4))
				return (FALSE);
			if (NOT_DEFINED != m_oMode4 && IsOff(m_oMode4))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_iMode4 && AOn(m_iMode4))
				return (FALSE);
			if (NOT_DEFINED != m_oMode4 && IsOn(m_oMode4))
				return (FALSE);
		}

		if (g_bitMask16[4] & mode)
		{
			if (NOT_DEFINED != m_iMode5 && AOff(m_iMode5))
				return (FALSE);
			if (NOT_DEFINED != m_oMode5 && IsOff(m_oMode5))
				return (FALSE);
		}
		else
		{
			if (NOT_DEFINED != m_iMode5 && AOn(m_iMode5))
				return (FALSE);
			if (NOT_DEFINED != m_oMode5 && IsOn(m_oMode5))
				return (FALSE);
		}

		return (TRUE);
	}

	BOOL Vision::IsLive(void)
	{
		if (isVirtual)
			return (FALSE);

		if (NOT_DEFINED != m_oLive && IsOn(m_oLive))
			return (TRUE);

		return (FALSE);
	}

	bool Vision::LiveOn(int mode)
	{
		if (isVirtual || NOT_DEFINED != mode && !IsMode(mode))
			return false;

		if (NOT_DEFINED != m_iBusy && AOn(m_iBusy))
			return false;

		if (IsOn(m_oStart) || IsOn(m_oReset) || m_isReservedReset)
			return false;

		if (NOT_DEFINED != m_oLive)
		{
			On(m_oLive);
			return true;
		}
		return false;
	}

	void Vision::LiveOff(void)
	{
		if (isVirtual)
			return;

		if (NOT_DEFINED != m_oLive)
			Off(m_oLive);
	}
}

#ifndef CHECK_ACTUATOR_ARRAY
HW::Vision VI[HW::Vision::maxDevice];
#else
CheckParser<HW::Vision, VisionList, VisionList(HW::Vision::maxDevice)> VI;
#endif