#include "..\Includes.h"

namespace COM
{
	void SaveMotorRecipe(void)
	{
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].isVirtualHW || MT[mtno].isVirtualSW)
				continue;

			if (!MT[mtno].m_state.isHome)
				return;
		}

		CreateDirectory(L"C:\\KOSES", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ\\Ini", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ\\Ini\\Standard", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ\\Ini\\Individual", NULL);

		CIni ini;
		WCHAR strDirectory[200] = { 0 };
		WCHAR strKeyCode[200] = { 0 };
		WCHAR strStrName[200] = { 0 };

		//for (int mtNo = 0; mtNo < mMAX; mtNo++)
		//{
		//	ZeroMemory(&strDirectory, sizeof(strDirectory));
		//	swprintf_s(strDirectory, 100, L"C:\\KOSES\\SEQ\\Ini\\STANDARD\\MOTOR%d.ini", mtNo);
		//	FILE* fp = _wfopen(strDirectory, L"r");
		//	if (NULL == fp)
		//		return;
		//	
		//	fclose(fp);
		//}

		wmx3Api::Config::HomeParam homeParam;
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].isVirtualHW || MT[mtno].isVirtualSW)
				continue;

			ZeroMemory(&homeParam, sizeof(homeParam));
			ZeroMemory(&strDirectory, sizeof(strDirectory));

			WMX.coremotion.config->GetHomeParam(mtno, &homeParam);
			swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\STANDARD\\MOTOR%d.ini", mtno);
			ini.SetPathName(strDirectory);
			for (int idxNo = 0; idxNo < MAX_INDEX_MOTOR; idxNo++)
			{
				ZeroMemory(&strKeyCode, sizeof(strKeyCode));
				ZeroMemory(&strStrName, sizeof(strStrName));

				swprintf_s(strKeyCode, L"POSITION%d", idxNo);

				if (0. < MT[mtno].m_moveStroke)
				{
					if (wmx3Api::Config::HomeDirection::Positive == MT[mtno].m_homeDirection)
						swprintf_s(strStrName, L"%.3f", MT[mtno].PosTable(idxNo) - MT[mtno].m_distLStoIndexPulse - WMX.PulseToMM(homeParam.homePosition, MT[mtno].m_pulseRate));
					else
						swprintf_s(strStrName, L"%.3f", MT[mtno].PosTable(idxNo) + MT[mtno].m_distLStoIndexPulse - WMX.PulseToMM(homeParam.homePosition, MT[mtno].m_pulseRate));
				}
				else
				{
					swprintf_s(strStrName, L"%.3f", MT[mtno].PosTable(idxNo));
				}

				ini.WriteString(L"INDEXPOS", strKeyCode, strStrName);
			}

			for (int idxNo = 0; idxNo < MAX_INDEX_MOTOR; idxNo++)
			{
				ZeroMemory(&strKeyCode, sizeof(strKeyCode));
				ZeroMemory(&strStrName, sizeof(strStrName));

				swprintf_s(strKeyCode, L"SPEED%d", idxNo);
				swprintf_s(strStrName, L"%.3f", MT[mtno].VelTable(idxNo));
				ini.WriteString(L"INDEXSPD", strKeyCode, strStrName);
			}

			for (int idxNo = 0; idxNo < MAX_INDEX_MOTOR; idxNo++)
			{
				ZeroMemory(&strKeyCode, sizeof(strKeyCode));
				ZeroMemory(&strStrName, sizeof(strStrName));

				swprintf_s(strKeyCode, L"ACC%d", idxNo);
				swprintf_s(strStrName, L"%.3f", MT[mtno].AccTable(idxNo));
				ini.WriteString(L"INDEXACC", strKeyCode, strStrName);
			}
		}
	}

	void LoadMotorRecipe(void)
	{
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].isVirtualHW || MT[mtno].isVirtualSW)
				continue;

			if (!MT[mtno].m_state.isHome)
				return;
		}

		CreateDirectory(L"C:\\KOSES", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ\\Ini", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ\\Ini\\Standard", NULL);
		CreateDirectory(L"C:\\KOSES\\SEQ\\Ini\\Individual", NULL);

		CIni ini;
		WCHAR strDirectory[200] = { 0, };
		WCHAR strKeyCode[200] = { 0, };
		WCHAR strStrName[200] = { 0, };

		for (int mtNo = 0; mtNo < motorMAX; mtNo++)
		{
			ZeroMemory(&strDirectory, sizeof(strDirectory));
			swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Standard\\MOTOR%d.ini", mtNo);
			FILE* fp = _wfopen(strDirectory, L"r");
			if (NULL == fp)
				return;

			fclose(fp);
		}

		wmx3Api::Config::HomeParam homeParam;
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (MT[mtno].isVirtualHW || MT[mtno].isVirtualSW)
				continue;

			ZeroMemory(&homeParam, sizeof(homeParam));
			ZeroMemory(&strDirectory, sizeof(strDirectory));

			WMX.coremotion.config->GetHomeParam(mtno, &homeParam);
			for (int idxNo = 0; idxNo < MAX_INDEX_MOTOR; idxNo++)
			{
				swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Standard\\MOTOR%d.ini", mtno);
				ini.SetPathName(strDirectory);

				ZeroMemory(&strKeyCode, sizeof(strKeyCode));
				ZeroMemory(&strStrName, sizeof(strStrName));

				swprintf_s(strKeyCode, L"POSITION%d", idxNo);

				WCHAR strGetValue[1024] = { 0, };
				ini.GetString(L"INDEXPOS", strKeyCode, strGetValue, 512);
				WCHAR* pEnd = NULL;

				double pos = (double)round(wcstod(strGetValue, &pEnd) * 1000.) / 1000.;

				if (0. < MT[mtno].m_moveStroke)
				{
					if (wmx3Api::Config::HomeDirection::Positive == MT[mtno].m_homeDirection)
						swprintf_s(strStrName, L"%.3f", pos + MT[mtno].m_distLStoIndexPulse + WMX.PulseToMM(homeParam.homePosition, MT[mtno].m_pulseRate));
					else
						swprintf_s(strStrName, L"%.3f", pos - MT[mtno].m_distLStoIndexPulse + WMX.PulseToMM(homeParam.homePosition, MT[mtno].m_pulseRate));
				}
				else
				{
					swprintf_s(strStrName, L"%.3f", pos);
				}

				swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Individual\\MOTOR%d.ini", mtno);
				ini.SetPathName(strDirectory);
				ini.WriteString(L"INDEXPOS", strKeyCode, strStrName);
			}

			for (int idxNo = 0; idxNo < MAX_INDEX_MOTOR; idxNo++)
			{
				swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Standard\\MOTOR%d.ini", mtno);
				ini.SetPathName(strDirectory);

				ZeroMemory(&strKeyCode, sizeof(strKeyCode));
				ZeroMemory(&strStrName, sizeof(strStrName));

				swprintf_s(strKeyCode, 100, L"SPEED%d", idxNo);

				WCHAR strGetValue[1024] = { 0, };
				ini.GetString(L"INDEXSPD", strKeyCode, strGetValue, 512);

				swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Individual\\MOTOR%d.ini", mtno);
				ini.SetPathName(strDirectory);
				ini.WriteString(L"INDEXSPD", strKeyCode, strGetValue);
			}

			for (int idxNo = 0; idxNo < MAX_INDEX_MOTOR; idxNo++)
			{
				swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Standard\\MOTOR%d.ini", mtno);
				ini.SetPathName(strDirectory);

				ZeroMemory(&strKeyCode, sizeof(strKeyCode));
				ZeroMemory(&strStrName, sizeof(strStrName));

				swprintf_s(strKeyCode, 100, L"ACC%d", idxNo);

				WCHAR strGetValue[1024] = { 0, };
				ini.GetString(L"INDEXACC", strKeyCode, strGetValue, 512);

				swprintf_s(strDirectory, L"C:\\KOSES\\SEQ\\Ini\\Individual\\MOTOR%d.ini", mtno);
				ini.SetPathName(strDirectory);
				ini.WriteString(L"INDEXACC", strKeyCode, strGetValue);
			}
		}
	}
}