#include "..\Includes.h"

namespace HW
{
	Vacuum::Vacuum()
	{
		for (int i = 0; i < MAX_INDEX_VACUUM; i++)
			m_indexName.Add("");

		isVirtual = (vacuumMAX > 0) ? 0 : 1;
		ZeroMemory(&m_state, sizeof(m_state));
		ZeroMemory(&m_table, sizeof(m_table));
		ZeroMemory(&m_fdcData, sizeof(m_fdcData));

		// default setting
		m_state.iHighNo = m_state.iLowNo = NOT_DEFINED;
		m_state.oOnNo = m_state.oOffNo = m_state.oBlowNo = NOT_DEFINED;
		m_state.pressureType = IPC_MMI::PRESSURE_NEGATIVE;
		m_fdcData.use = true;

		m_no = NOT_DEFINED;
		m_errorAct = NOT_DEFINED;
		m_errorUse = FALSE;

		m_isDriving = FALSE;
		m_isError = FALSE;

		m_iDir = NOT_DEFINED;
		m_oDir = NOT_DEFINED;
		m_oCmd = NOT_DEFINED;
		m_nameA = "ON";
		m_nameB = "OFF";
		m_delayTime = 0;
		m_command.idx = 0;
		m_sLogFncAddStart = m_sLogFncAddEnd = "";
	};

	Vacuum::~Vacuum()
	{

	}

	void Vacuum::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumIndexName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < vacuumMAX; i++)
		{
			VacuumList no = VacuumList(i);
			VC[no].m_no = i;
			VC[no].m_command = VC[no].CommandTable(0);

			if (!CanWrite)
				continue;

			ZeroMemory(strSection, sizeof(strSection));
			ZeroMemory(strKeyCode, sizeof(strKeyCode));
			ZeroMemory(strStrName, sizeof(strStrName));
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumName.ini");
			swprintf_s(strKeyCode, 100, L"VACUUM%d", i);
			ini.GetString(L"VACUUMNAME", strKeyCode, strStrName, 512);
			VC[no].m_name = GetAscCode(strStrName);
			for (int indexNo = 0; indexNo < MAX_INDEX_VACUUM; indexNo++)
			{
				ZeroMemory(strSection, sizeof(strSection));
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumIndexName.ini");
				swprintf_s(strSection, 100, L"VACUUMINDEXNAME%d", i);
				swprintf_s(strKeyCode, 100, L"VACUUMINDEXNAME%d", indexNo);
				ini.GetString(strSection, strKeyCode, strStrName, 512);
				VC[no].m_indexName[indexNo] = GetAscCode(strStrName);
			}		
		}
	}

	void Vacuum::SLogFncAddMessage(string message)
	{
		m_sLogFncAddStart = message;
	}

	void Vacuum::SLogStartEvent()
	{
		m_delayTime = vacuumON == m_iDir ? m_command.profile.timeOn : m_command.profile.timeOff;
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(VacuumList(m_no)) && SLOG[no].vacuumLogStart[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].vacuumLogStart[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].vacuumLogStart[m_no].GetXFREventId() + "_" + m_indexName[m_command.idx] + m_sLogFncAddStart).c_str(), "START",
					SLOG[no].vacuumLogStart[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].vacuumLogStart[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'VACUUM'",
					"'ACT_NAME'", ("'V" + ToString(m_no) + "'").c_str(),
					m_bAnalogueInput ? "'PRESSURE'" : NULL,
					m_bAnalogueInput ? ToString(m_state.value, 3).c_str() : NULL);
				SLOG[no].vacuumLogEnd[m_no] = SLOG[no].vacuumLogStart[m_no];
			}
		}
		m_sLogFncAddEnd = m_sLogFncAddStart;
		m_sLogFncAddStart = "";
	}

	void Vacuum::SLogEndEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].vacuumLogEnd[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].vacuumLogEnd[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].vacuumLogEnd[m_no].GetXFREventId() + "_" + m_indexName[m_command.idx] + m_sLogFncAddEnd).c_str(), "END",
					SLOG[no].vacuumLogEnd[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].vacuumLogEnd[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'VACUUM'",
					"'ACT_NAME'", ("'V" + ToString(m_no) + "'").c_str(),
					m_bAnalogueInput ? "'PRESSURE'" : NULL,
					m_bAnalogueInput ? ToString(m_state.value, 3).c_str() : NULL);
				SLOG[no].vacuumLogEnd[m_no].End(TRUE);
			}
		}
		m_sLogFncAddEnd = "";
	}

	VcCommand::SingleParamter Vacuum::CommandTable(int idx)
	{
		if (!Between(m_no, 0, vacuumMAX - 1) || !Between(idx, 0, MAX_INDEX_VACUUM - 1))
		{
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);
		}

		VcCommand::SingleParamter command;
		command.no = m_no;
		command.idx = idx;
		command.profile.highOn = NV.vacTable[m_no].valueHighOn[idx];
		command.profile.highOff = NV.vacTable[m_no].valueHighOff[idx];
		command.profile.lowOn = NV.vacTable[m_no].valueLowOn[idx];
		command.profile.lowOff = NV.vacTable[m_no].valueLowOff[idx];
		command.profile.blowOn = NV.vacTable[m_no].valueBlowOn[idx];
		command.profile.timeOn = NV.vacTable[m_no].delayTimeOn[idx];
		command.profile.timeOff = NV.vacTable[m_no].delayTimeOff[idx];
		command.profile.timeBlow = NV.vacTable[m_no].delayTimeBlow[idx];
		return command;
	}

	BOOL Vacuum::SetIO(enAI ioNo, enDO oOnNo, enDO oOffNo, enDO oBlowNo)
	{
		m_aiNo = ioNo;
		m_bAnalogueInput = true;

		m_state.oOnNo = oOnNo;
		m_state.oOffNo = oOffNo;
		m_state.oBlowNo = oBlowNo;

		{
			// 25.08.27 88-24-0042-sec-2 Only
			if (m_aiNo == aiAirSealNozzleN2)
				m_bMfcSensor = true;
			else
				m_bMfcSensor = false;

			m_tmRefreshMfcSensor.Reset();
			m_LastUpdateTime = m_tmRefreshMfcSensor.ElapsedSec();
		}

		return (TRUE);
	}

	BOOL Vacuum::SetIO(enAI ioNo, enDI iHighNo, enDI iLowNo, enDO oOnNo, enDO oOffNo, enDO oBlowNo)
	{
		m_aiNo = ioNo;
		if (NOT_DEFINED != m_aiNo)
			m_bAnalogueInput = true;
		else
		{
			m_state.iHighNo = iHighNo;
			m_state.iLowNo = iLowNo;

			m_bAnalogueInput = false;
		}

		m_state.oOnNo = oOnNo;
		m_state.oOffNo = oOffNo;
		m_state.oBlowNo = oBlowNo;
		return (TRUE);
	}

	void Vacuum::SetParam(double dLevelH, double dLevelL, double dSensorH, double dSensorL, double dValueH, double dValueL, double dResolution)
	{
		m_dDeviceLevelH = dLevelH;
		m_dDeviceLevelL = dLevelL;
		m_dSensorLevelH = dSensorH;
		m_dSensorLevelL = dSensorL;
		m_dDisplayValueH = dValueH;
		m_dDisplayValueL = dValueL;
		m_dResolution = dResolution;

		memcpy(&m_table, &NV.vacTable[m_no], sizeof(m_table));
	}

	void Vacuum::SetErr(LONG timeLimit, ErrName errorCode, int errorAct, BOOL errorUse)
	{
		m_timeLimit = timeLimit;
		m_errorCode = errorCode;
		m_errorAct = errorAct;
		m_errorUse = errorUse;
	}

	void Vacuum::SetErr(int errorAct)
	{
		m_errorAct = errorAct;
	}

	void Vacuum::Reset()
	{
		m_tmError.Reset();
		m_isError = FALSE;
	}

	void Vacuum::Update(void)
	{
		if (isVirtual || OPR.isDebugMode)
			return;

		if (NOT_DEFINED == m_no)
			return;

		if (m_bAnalogueInput)
		{
			double resolution = (m_dResolution * (m_dSensorLevelH / m_dDeviceLevelH));
			double value = 0.;

			if (OPR.isNoDevice || OPR.isDryRun)
			{
				if (this->IsOn(1))
					value = m_dDisplayValueH * (((rand() % 10) * 0.001) + 0.85);
				else
				{
					value = m_dDisplayValueH * ((rand() % 5) * 0.01);
					if (rand() % 2)
						value = (-value);
				}
			}

			if (!OPR.isNoDevice && !OPR.isDryRun)
			{
				double ratio = (m_dDeviceLevelH / m_dSensorLevelH);
				double offset = (m_dSensorLevelL * ratio) - m_dDeviceLevelL;
				double compensation = offset * (resolution / m_dDeviceLevelH);
				double rate;
				// Overflow occurs when the input voltage falls below 1V,
				// given a configured range of 1V to 5V.
				if (::Get(m_aiNo) < m_dResolution)
					rate = (::Get(m_aiNo) - compensation) / (resolution - compensation);
				else
					rate = 0.0;

				value = (rate * (m_dDisplayValueH - m_dDisplayValueL)) + m_dDisplayValueL;

				// Flow Accumulation
				if (m_bMfcSensor)
				{
					double currentTime = m_tmRefreshMfcSensor.ElapsedSec();
					double deltaTime = currentTime - m_LastUpdateTime;

					if (Between(deltaTime, 0.0, 1.0) && value > 0.05)
					{
						// * 1000 = Liter ->Milli Liter Conversion,  deltaTime = s, value = L/min, 1min=60s deltaFlow
						double deltaMfcValuemilliLiter = value * 1000 / 60 * deltaTime;
						if (deltaMfcValuemilliLiter > 0)
							NV.ddm[mfcValuemilliLiter] += deltaMfcValuemilliLiter;
					}

					m_LastUpdateTime = currentTime;
				}
			}

			m_state.value = value;

			/** FDC VACUUM DATA **/
			if (m_fdcData.use)
			{
				if (this->IsOn(1) && !m_fdcData.collectStart && OPR.isAuto)
				{
					m_fdcData.collectStart = TRUE;
				}
				if (this->IsOff() && m_fdcData.collectStart && OPR.isAuto)
				{
					m_fdcData.collectStart = FALSE;
					m_fdcData.dataSend = COMM_REQ;
				}
			}

			if (m_fdcData.collectStart)
			{
				double currentTime = m_tmFdcRefresh.Elapsed_mSec();
				double deltaTime = currentTime - m_FdcLastUpdateTime;

				if (Between(deltaTime, 0.0, 10000.0))
				{
					if (deltaTime > 100.0)
					{
						if (fabs(m_state.value) > m_command.profile.highOn)
						{
							if (m_FdcAverageDataCnt == 0)
							{
								m_fdcData.min = m_state.value;
								m_fdcData.max = m_state.value;
							}

							if (m_fdcData.min > m_state.value)
								m_fdcData.min = m_state.value;
							if (m_fdcData.max < m_state.value)
								m_fdcData.max = m_state.value;

							m_FdcAverageData += m_state.value;
							m_FdcAverageDataCnt++;
							if (m_FdcAverageDataCnt > INT_MAX - 1)	// Overflow Protection
							{
								m_FdcAverageData = m_FdcAverageData / m_FdcAverageDataCnt;
								m_FdcAverageDataCnt = 1;
							}
							m_fdcData.avg = m_FdcAverageData / m_FdcAverageDataCnt;
						}
						m_FdcLastUpdateTime = currentTime;
					}
				}
				else
					m_FdcLastUpdateTime = currentTime;
			}

			if (m_fdcData.dataSend == COMM_COMP)
			{
				m_FdcAverageData = 0.0;
				m_FdcAverageDataCnt = 0.0;
				m_fdcData.dataSend = COMM_IDLE;
			}
			/** FDC VACUUM DATA **/
			
			if (IPC_MMI::PRESSURE_NEGATIVE == m_state.pressureType)
			{
				if (m_state.value <= -m_command.profile.highOn)
					m_state.highOn = true;
				else if (m_state.value >= -m_command.profile.highOff)
					m_state.highOn = false;

				if (m_state.value <= -m_command.profile.lowOn)
					m_state.lowOn = true;
				else if (m_state.value >= -m_command.profile.lowOff)
					m_state.lowOn = false;

				if (m_state.value <= -m_command.profile.blowOn)
					m_state.blowOn = true;
				else
					m_state.blowOn = false;
			}
			else
			{
				if (m_state.value >= m_command.profile.highOn)
					m_state.highOn = true;
				else if (m_state.value <= m_command.profile.highOff)
					m_state.highOn = false;

				if (m_state.value >= m_command.profile.lowOn)
					m_state.lowOn = true;
				else if (m_state.value <= m_command.profile.lowOff)
					m_state.lowOn = false;

				if (m_state.value >= m_command.profile.blowOn)
					m_state.blowOn = true;
				else
					m_state.blowOn = false;
			}
		}
		else
		{
			if (NOT_DEFINED != m_state.iHighNo && ::AOn((enDI)m_state.iHighNo))
				m_state.highOn = true;
			else
				m_state.highOn = false;

			if (NOT_DEFINED != m_state.iLowNo && ::AOn((enDI)m_state.iLowNo) ||
				NOT_DEFINED == m_state.iLowNo && NOT_DEFINED != m_state.iHighNo && ::AOn((enDI)m_state.iHighNo))
				m_state.lowOn = true;
			else
				m_state.lowOn = false;

			if (NOT_DEFINED != m_state.oBlowNo && m_state.lowOn)
				m_state.blowOn = true;
			else
				m_state.blowOn = false;
		}

		m_tmHighOn.Reset(!m_state.highOn);
		m_tmLowOff.Reset(m_state.lowOn);
		m_tmBlowOff.Reset(m_state.blowOn);
		
		if (NOT_DEFINED != m_state.oBlowNo && !m_state.blowOn && m_tmBlowOff.TmOver((DWORD)m_command.profile.timeBlow))
		{
			::Off((enDO)m_state.oBlowNo);
		}

		if (this->AOn() && !this->AOff())
			m_iDir = vacuumON;
		else if (!this->AOn() && this->AOff())
			m_iDir = vacuumOFF;
		else
			m_iDir = NOT_DEFINED;

		if (this->IsOn() && !this->IsOff())
			m_oDir = vacuumON;
		else if (!this->IsOn() && this->IsOff())
			m_oDir = vacuumOFF;
		else
			m_oDir = NOT_DEFINED;

		if (NOT_DEFINED == m_state.oOnNo && NOT_DEFINED == m_state.oOffNo)
		{
			if (vacuumON == m_iDir)
			{
				m_tmDelay.Reset();
				m_tmError.Reset();
			}
			m_oCmd = m_oDir = vacuumON;//
		}

		if (m_oCmd != m_oDir)
		{
			if (m_isDriving)
			{
				SLogEndEvent();
				if (vacuumON == m_oDir || vacuumOFF == m_oDir)
					SLogStartEvent();
			}
			else if (vacuumON == m_oDir || vacuumOFF == m_oDir)
			{
				SLogEndEvent();
				SLogStartEvent();
				m_isDriving = TRUE;
			}
			m_tmDelay.Reset();
			m_tmError.Reset();
		}
		m_oCmd = m_oDir;

		if (m_isDriving)
		{
			if (vacuumON == m_oDir)
				m_state.elapsedTimeOn = m_tmDelay.ElapsedSec();
			else if (vacuumOFF == m_oDir)
				m_state.elapsedTimeOff = m_tmDelay.ElapsedSec();
		}
		
		if (m_iDir == m_oDir &&
			(vacuumON == m_iDir || vacuumOFF == m_iDir) &&
			(vacuumON == m_oDir || vacuumOFF == m_oDir) &&
			m_tmDelay.TmOver(m_delayTime))
		{
			if (m_isDriving)
				SLogEndEvent();
			
			m_isDriving = FALSE;
			m_tmError.Reset();
		}

		if (m_errorUse && m_tmError.TmOver(m_timeLimit))
		{
			if (m_isDriving)
				SLogEndEvent();
			
			m_isDriving = FALSE;
			if (!m_isError)
			{
				if (vacuumON == m_errorAct)
				{
					if (Between(m_command.idx , 0 , MAX_INDEX_VACUUM - 1))
						this->On(m_command.idx);
				}
				else if (vacuumOFF == m_errorAct)
				{
					if (Between(m_command.idx , 0 , MAX_INDEX_VACUUM - 1))
						this->Off(m_command.idx);
				}
				else if (vacuumFREE == m_errorAct)
					this->Clear();
			}

			if (0 < m_errorCode)
				ER.Save(m_errorCode);

			m_isError = TRUE;
		}
	}

	void Vacuum::Clear(void)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		if (NOT_DEFINED != m_state.oOnNo)
			::Off((enDO)m_state.oOnNo);
		if (NOT_DEFINED != m_state.oOffNo)
			::Off((enDO)m_state.oOffNo);
		if (NOT_DEFINED != m_state.oBlowNo)
			::Off((enDO)m_state.oBlowNo);
	}

	void Vacuum::On(int index)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		m_command = CommandTable(Between(index , 0 , MAX_INDEX_VACUUM - 1) ? index : 0);

		if (NOT_DEFINED != m_state.oOnNo)
			::On((enDO)m_state.oOnNo);
		if (NOT_DEFINED != m_state.oOffNo)
			::Off((enDO)m_state.oOffNo);
		if (NOT_DEFINED != m_state.oBlowNo)
			::Off((enDO)m_state.oBlowNo);
	}

	void Vacuum::Off(int index , bool bBlowUse)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		m_command = CommandTable(Between(index , 0 , MAX_INDEX_VACUUM - 1) ? index : 0);

		if (NOT_DEFINED != m_state.oOnNo)
			::Off((enDO)m_state.oOnNo);
		if (NOT_DEFINED != m_state.oOffNo)
			::On((enDO)m_state.oOffNo);
		if (NOT_DEFINED != m_state.oBlowNo && bBlowUse && m_state.blowOn)
			::On((enDO)m_state.oBlowNo);
	}

	void Vacuum::BlowOn(int index)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		m_command = CommandTable(Between(index , 0 , MAX_INDEX_VACUUM - 1) ? index : 0);

		if (NOT_DEFINED != m_state.oOnNo && NOT_DEFINED != m_state.oOffNo)
		{
			::Off((enDO)m_state.oOnNo);
			::Off((enDO)m_state.oOffNo);
		}
		else
		{
			if (NOT_DEFINED != m_state.oOnNo)
				::Off((enDO)m_state.oOnNo);
			if (NOT_DEFINED != m_state.oOffNo)
				::On((enDO)m_state.oOffNo);
		}

		if (NOT_DEFINED != m_state.oBlowNo)
		{
			::On((enDO)m_state.oBlowNo);
			m_tmBlowOff.Reset();
		}
	}

	void Vacuum::BlowOff(void)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return;

		if (NOT_DEFINED != m_state.oBlowNo)
			::Off((enDO)m_state.oBlowNo);
	}

	bool Vacuum::AOn()
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;
		 
		if (m_state.highOn && m_tmHighOn.TmOver((DWORD)(m_command.profile.timeOn)))
		{
			return true;
		}
		return false;
	}

	bool Vacuum::AOff()
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;

		if (!m_state.lowOn && m_tmLowOff.TmOver((DWORD)(m_command.profile.timeOff)))
		{
			return true;
		}
		return false;
	}

	bool Vacuum::IsOn(LONG lDelayTime)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;

		if ((NOT_DEFINED == m_state.oOnNo || ::IsOn((enDO)m_state.oOnNo, lDelayTime)) &&
			(NOT_DEFINED == m_state.oOffNo || ::IsOff((enDO)m_state.oOffNo, lDelayTime)))
		{
			return true;
		}

		return false;
	}

	bool Vacuum::IsOff(LONG lDelayTime)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;

		if ((NOT_DEFINED == m_state.oOnNo || ::IsOff((enDO)m_state.oOnNo, lDelayTime)) &&
			(NOT_DEFINED == m_state.oOffNo || ::IsOn((enDO)m_state.oOffNo, lDelayTime)))
		{
			return true;
		}

		return false;
	}

	bool Vacuum::IsBlowOn(LONG lDelayTime)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;

		if (NOT_DEFINED != m_state.oBlowNo && ::IsOn((enDO)m_state.oBlowNo, lDelayTime))
		{
			return true;
		}
		return false;
	}

	bool Vacuum::IsBlowOff(LONG lDelayTime)
	{
		if (isVirtual || NOT_DEFINED == m_no)
			return false;

		if (NOT_DEFINED != m_state.oBlowNo && ::IsOff((enDO)m_state.oBlowNo, lDelayTime))
		{
			return true;
		}
		return false;
	}
}

#ifndef CHECK_ACTUATOR_ARRAY
HW::Vacuum VC[HW::Vacuum::maxDevice];
#else
CheckParser<HW::Vacuum, VacuumList, VacuumList(HW::Vacuum::maxDevice)> VC;
#endif