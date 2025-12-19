#include "..\Includes.h"

namespace HW
{
	Laser::Laser()
	{
		//for (int i = 0; i < MAX_INDEX_LASER; i++)
		//	m_indexName.Add("");	
		
		isVirtual = (laserMAX > 0) ? 0 : 1;
		m_no = NOT_DEFINED;
		m_iMode = NOT_DEFINED;

		m_iAuto = (seqInput)NOT_DEFINED;
		m_iReady = (seqInput)NOT_DEFINED;
		m_iBusy = (seqInput)NOT_DEFINED;
		m_iError = (seqInput)NOT_DEFINED;
		m_iRecvCoordData = (seqInput)NOT_DEFINED;

		m_oTrig = (seqOutput)NOT_DEFINED;
		m_oReset = (seqOutput)NOT_DEFINED;
		m_oSendCoordData = (seqOutput)NOT_DEFINED;

		m_codeErrorAuto = (ErrName)NOT_DEFINED;
		m_codeErrorReady = (ErrName)NOT_DEFINED;
		m_codeErrorBusy = (ErrName)NOT_DEFINED;
		m_codeErrorDataWait = (ErrName)NOT_DEFINED;
		m_codeErrorCoordDataWait = (ErrName)NOT_DEFINED;
		m_codeErrorStop = (ErrName)NOT_DEFINED;
		m_codeErrorMode = (ErrName)NOT_DEFINED;
		m_isWaitData.Init(nullptr, nullptr, SLogEvent, &m_no);
	};

	Laser::~Laser()
	{

	};

	void Laser::SLogEvent(bool oldValue, bool newValue, LPVOID lpParam)
	{
		int& no = *reinterpret_cast<int*>(lpParam);
		LaserList lano = (LaserList)no;
		if (oldValue && !newValue)
		{
			LAS[lano].SLogEndEvent();
		}
		else if (!oldValue && newValue)
		{
			LAS[lano].SLogEndEvent();
			LAS[lano].SLogStartEvent();
		}
	}

	void Laser::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\LaserName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < laserMAX; i++)
		{
			LaserList no = LaserList(i);
			LAS[no].m_no = i;

			if (!CanWrite)
				continue;

			ZeroMemory(strSection, sizeof(strSection));
			ZeroMemory(strKeyCode, sizeof(strKeyCode));
			ZeroMemory(strStrName, sizeof(strStrName));
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\LaserName.ini");
			swprintf_s(strKeyCode, 100, L"LASER%d", i);
			ini.GetString(L"LASERNAME", strKeyCode, strStrName, 512);
			LAS[no].m_name = GetAscCode(strStrName);
		}
	}

	void Laser::SLogStartEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(LaserList(m_no)) && SLOG[no].laserLogStart[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].laserLogStart[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].laserLogStart[m_no].GetXFREventId() + "_" + m_name + "_FIRE").c_str(), "START",
					SLOG[no].laserLogStart[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].laserLogStart[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'LASER'",
					"'ACT_NAME'", ("'L" + ToString(m_no) + "'").c_str()
					/*, "DELAYTIME", ToString(m_delayTime).c_str(),*/
					/*"SEN_X", ToString(m_command.profile.vel).c_str(),*/
				/*"SEN_X", ToString(m_command.profile.acc).c_str()*/);
				SLOG[no].laserLogEnd[m_no] = SLOG[no].laserLogStart[m_no];
			}
		}
	}

	void Laser::SLogEndEvent() 
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].laserLogEnd[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].laserLogEnd[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].laserLogEnd[m_no].GetXFREventId() + "_" + m_name + "_FIRE").c_str(), "END",
					SLOG[no].laserLogEnd[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].laserLogEnd[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'LASER'",
					"'ACT_NAME'", ("'L" + ToString(m_no) + "'").c_str()
					/*, "DELAYTIME", ToString(m_delayTime).c_str(),*/
					/*"VEL_T", ToString(m_command.profile.vel).c_str(),*/
				/*"ACC_T", ToString(m_command.profile.acc).c_str()*/);
				SLOG[no].laserLogEnd[m_no].End(TRUE);
			}
		}
	}

	void Laser::SetIO(seqInput iAuto, seqInput iReady, seqInput iBusy, seqInput iError, seqInput iRecvCoordData,
		seqOutput oTrig, seqOutput oReset, seqOutput oSendCoordData)
	{
		m_iAuto = iAuto;
		m_iReady = iReady;
		m_iBusy = iBusy;
		m_iError = iError;
		m_iRecvCoordData = iRecvCoordData;

		m_oTrig = oTrig;
		m_oReset = oReset;
		m_oSendCoordData = oSendCoordData;
	}

	void Laser::SetErr(ErrName codeErrorAuto, ErrName codeErrorReady, ErrName codeErrorBusy, ErrName codeErrorDataWait,
		ErrName codeErrorCoordDataWait, ErrName codeErrorStop, ErrName codeErrorMode)
	{
		m_codeErrorAuto = codeErrorAuto;
		m_codeErrorReady = codeErrorReady;
		m_codeErrorBusy = codeErrorBusy;
		m_codeErrorDataWait = codeErrorDataWait;
		m_codeErrorCoordDataWait = codeErrorCoordDataWait;
		m_codeErrorStop = codeErrorStop;
		m_codeErrorMode = codeErrorMode;
	}

	void Laser::Update(void)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		if (OPR.isNoDevice)
		{
			Set(m_iReady, TRUE);
			if (IsOn(m_oTrig, 100))
				Set(m_iBusy, TRUE);
			else if (AOn(m_iBusy, 100))
				Set(m_iBusy, FALSE);
		}

		if (!m_isWaitCoordData1 && !m_isWaitCoordData2)
			m_tmWaitCoordData.Reset();

		if (m_tmWaitCoordData.TmOver(10000))
			m_isErrorCoordDataWait = true;

		m_isError = false;

		if (m_isErrorReady)
		{
			if (NOT_DEFINED != m_codeErrorReady)
				ER.Save(m_codeErrorReady);

			m_isError = true;
		}

		if (m_isErrorMode)
		{
			if (NOT_DEFINED != m_codeErrorMode)
				ER.Save(m_codeErrorMode);

			m_isErrorMode = true;
		}

		if (m_isErrorBusy)
		{
			if (NOT_DEFINED != m_codeErrorBusy)
				ER.Save(m_codeErrorBusy);

			m_isError = true;
		}

		if (m_isErrorCoordDataWait)
		{
			if (NOT_DEFINED != m_codeErrorCoordDataWait)
				ER.Save(m_codeErrorCoordDataWait);

			m_isWaitCoordData1 = false;
			m_isWaitCoordData2 = false;
			m_isError = true;
		}

		if (m_isErrorStop)
		{
			if (NOT_DEFINED != m_codeErrorStop)
				ER.Save(m_codeErrorStop);

			m_isError = true;
		}

		if (IsOff(m_oTrig, 10) && AOff(m_iBusy, 10))
			m_isWaitData = false;

		if (IsOn(m_oReset, 100))
			Off(m_oReset);

		if (IsOn(m_oSendCoordData))
		{
			if (IsOn(m_oSendCoordData, 3000))
			{
				m_isErrorCoordDataWait = true;
				Off(m_oSendCoordData);
			}
			if (AOn(m_iRecvCoordData) || AOn(m_iError))
			{
				Off(m_oSendCoordData);
			}
		}
		else if (IsOff(m_oSendCoordData, 10))
		{
			m_isWaitCoordData1 = false;
		}

		if (IsOn(m_oTrig))
		{
			if (IsOn(m_oTrig, 3000))
			{
				m_isErrorBusy = true;
				Off(m_oTrig);
			}
			else if (AOn(m_iBusy) || AOn(m_iError))
			{
				Off(m_oTrig);
			}
		}
	}

	int Laser::IsMode()
	{
		return m_iMode;
	}

	bool Laser::IsWaitCoordData()
	{
		if (m_isWaitCoordData1 || m_isWaitCoordData2)
			return true;

		return false;
	}

	bool Laser::SendCoordData()
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;

		On(m_oSendCoordData);
		m_isWaitCoordData1 = true;
		m_isWaitCoordData2 = true;
		return true;
	}

	BOOL Laser::IsError()
	{
		return AOn(m_iError);
	}

	BOOL Laser::Start()
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return FALSE;

		if (IsMode() == NOT_DEFINED)
		{
			m_isErrorMode = true;
			return FALSE;
		}

		if (!m_isWaitData && !m_isError &&
			IsOff(m_oTrig) && AOn(m_iReady, 100) && AOff(m_iError, 100))
		{
			if (!OPR.isDryRun)
				On(m_oTrig);

			m_isWaitData = true;
			return TRUE;
		}
		//else if (AOn(m_iReady))
		//{
		//	m_isErrorReady = true;
		//}
		//else if (AOn(m_iBusy, 3000))
		//{
		//	m_isErrorBusy = true;
		//}

		return FALSE;
	};

	BOOL Laser::IsBusy()
	{
		return (AOn(m_iBusy) || IsOn(m_oTrig));
	}

	void Laser::Reset()
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		On(m_oReset);
	}

	void Laser::Clear()
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		Off(m_oTrig);
		Off(m_oSendCoordData);
		ZeroMemory(&m_scanData, sizeof(IPC_LASER::ScannerData));
		m_isReadData = false;
		m_isWaitData = false;
		m_isWaitCoordData1 = false;
		m_isWaitCoordData2 = false;
		m_iMode = NOT_DEFINED;
	}

	void Laser::SetMode(enLaserMode iMode)
	{
		m_iMode = iMode;
	}

	//-------------------------------------------------------------------
	// 스캐너 캘리브레이션 데이터를 스캐너 설치 방향에 맞게 회전 변환함..
	IPC_LASER::ScanCal Laser::RotateCalData(IPC_LASER::ScanCal * pOrg, int angle, int markCnt)
	{
		// 회전 반시계+
		IPC_LASER::ScanCal newCal = *pOrg;
		int xOrg = 0, yOrg = 0;
		if (90 == angle || -270 == angle)
		{
			for (int xCnt = 0; xCnt < markCnt; xCnt++)
			{
				for (int yCnt = 0; yCnt < markCnt; yCnt++)
				{
					xOrg = yCnt;
					yOrg = (markCnt - 1) - xCnt;
					newCal.val[xCnt][yCnt].dX = pOrg->val[xOrg][yOrg].dY;
					newCal.val[xCnt][yCnt].dY = pOrg->val[xOrg][yOrg].dX * (-1.0);
				}
			}
		}
		else if (-90 == angle || 270 == angle)
		{
			for (int xCnt = 0; xCnt < markCnt; xCnt++)
			{
				for (int yCnt = 0; yCnt < markCnt; yCnt++)
				{
					xOrg = (markCnt - 1) - yCnt;
					yOrg = xCnt;
					newCal.val[xCnt][yCnt].dX = pOrg->val[xOrg][yOrg].dY * (-1.0);
					newCal.val[xCnt][yCnt].dY = pOrg->val[xOrg][yOrg].dX;
				}
			}
		}
		else if (-180 == angle || 180 == angle)
		{
			for (int xCnt = 0; xCnt < markCnt; xCnt++)
			{
				for (int yCnt = 0; yCnt < markCnt; yCnt++)
				{
					xOrg = (markCnt - 1) - xCnt;
					yOrg = (markCnt - 1) - yCnt;
					newCal.val[xCnt][yCnt].dX = pOrg->val[xOrg][yOrg].dX * (-1.0);
					newCal.val[xCnt][yCnt].dY = pOrg->val[xOrg][yOrg].dY * (-1.0);
				}
			}
		}

		return (newCal);
	}

	// 스캐너 가공 데이터를 스캐너 설치 방향에 맞게 회전 변환함..
	IPC_LASER::ScannerCoord Laser::RotateCoordData(IPC_LASER::ScannerCoord* pOrg, int scanAngle, int drawAngle)
	{
		IPC_LASER::ScannerCoord newCoord = *pOrg;

		int markCnt = newCoord.unitCnt;

		int xOrg = 0, yOrg = 0;
		if (90 == scanAngle || -270 == scanAngle)
		{
			for (int cnt = 0; cnt < markCnt; cnt++)
			{
				newCoord.x[cnt] = pOrg->y[cnt];
				newCoord.y[cnt] = pOrg->x[cnt] * (-1.0);
				newCoord.t[cnt] = pOrg->t[cnt] + drawAngle;
			}
		}
		else if (-90 == scanAngle || 270 == scanAngle)
		{
			for (int cnt = 0; cnt < markCnt; cnt++)
			{
				newCoord.x[cnt] = pOrg->y[cnt] * (-1.0);
				newCoord.y[cnt] = pOrg->x[cnt];
				newCoord.t[cnt] = pOrg->t[cnt] + drawAngle;
			}
		}
		else if (-180 == scanAngle || 180 == scanAngle)
		{
			for (int cnt = 0; cnt < markCnt; cnt++)
			{
				newCoord.x[cnt] = pOrg->x[cnt] * (-1.0);
				newCoord.y[cnt] = pOrg->y[cnt] * (-1.0);
				newCoord.t[cnt] = pOrg->t[cnt] + drawAngle;
			}
		}
		else
		{
			for (int cnt = 0; cnt < markCnt; cnt++)
			{
				newCoord.t[cnt] = pOrg->t[cnt] + drawAngle;
			}
		}


		return (newCoord);
	}
}

#ifndef CHECK_ACTUATOR_ARRAY
HW::Laser LAS[HW::Laser::maxDevice];
#else
CheckParser<HW::Laser, LaserList, LaserList(HW::Laser::maxDevice)> LAS;
#endif