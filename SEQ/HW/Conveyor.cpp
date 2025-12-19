#include "..\Includes.h"

namespace HW
{
	Conveyor::Conveyor()
	{
		isVirtual = (conveyorMAX > 0) ? 0 : 1;
		m_no = NOT_DEFINED;
		m_isDriving = FALSE;

		m_state.doA = NOT_DEFINED;
		m_state.doB = NOT_DEFINED;
		m_state.iDir = NOT_DEFINED;
		m_state.oDir = NOT_DEFINED;
		m_oCmd = NOT_DEFINED;
		m_nameA = "";
		m_nameB = "";
		m_nameC = "";

		CanMove = Conveyor::DefaultCanMove;
		m_sLogFncAddStart = m_sLogFncAddEnd = "";
	}

	Conveyor::~Conveyor()
	{

	}

	BOOL Conveyor::DefaultCanMove(int dir, int index)
	{
		if (OPR.isDoorOpen || OPR.isDoorUnlock)
		{
			MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Door Is Open.");
			return (FALSE);
		}
		return (TRUE);
	}

	void Conveyor::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\ConveyorName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < conveyorMAX; i++)
		{
			ConveyorList no = ConveyorList(i);

			CONV[no].m_no = i;
			CONV[no].CanMove = Conveyor::DefaultCanMove;
			CONV[no].SetParam("FWD", "BWD", "STOP");

			if (!CanWrite)
				continue;

			CIni ini = { 0 };
			TCHAR strSection[1024] = { 0 };
			TCHAR strKeyCode[1024] = { 0 };
			TCHAR strStrName[1024] = { 0 };
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\ConveyorName.ini");
			swprintf_s(strKeyCode, 100, L"CONVEYOR%d", i);
			ini.GetString(L"CONVEYORNAME", strKeyCode, strStrName, 512);
			CONV[no].m_name = GetAscCode(strStrName);
		}
	}

	void Conveyor::SLogFncAddMessage(string message)
	{
		m_sLogFncAddStart = message;
	}

	void Conveyor::SLogStartEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(ConveyorList(m_no)) && SLOG[no].conveyorLogStart[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].conveyorLogStart[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].conveyorLogStart[m_no].GetXFREventId() + "_" + m_name + "_" + (conveyorFWD == m_state.oDir ? m_nameA : conveyorBWD == m_state.oDir ? m_nameB : m_nameC) + m_sLogFncAddStart).c_str(), "START",
					SLOG[no].conveyorLogStart[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].conveyorLogStart[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'AC_MOTOR'",
					"'ACT_NAME'", ("'A" + ToString(m_no) + "'").c_str());
				SLOG[no].conveyorLogEnd[m_no] = SLOG[no].conveyorLogStart[m_no];
			}
		}
		m_sLogFncAddEnd = m_sLogFncAddStart;
		m_sLogFncAddStart = "";
	}

	void Conveyor::SLogEndEvent()
	{
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].conveyorLogEnd[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].conveyorLogEnd[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].conveyorLogEnd[m_no].GetXFREventId() + "_" + m_name + "_" + (conveyorFWD == m_state.oDir ? m_nameA : conveyorBWD == m_state.oDir ? m_nameB : m_nameC) + m_sLogFncAddEnd).c_str(), "END",
					SLOG[no].conveyorLogEnd[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].conveyorLogEnd[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'AC_MOTOR'",
					"'ACT_NAME'", ("'A" + ToString(m_no) + "'").c_str());
				SLOG[no].conveyorLogEnd[m_no].End(TRUE);
			}
		}
		m_sLogFncAddEnd = "";
	}

	BOOL Conveyor::SetIO(enDO doDir, enDO doRun)
	{
		m_state.doA = doDir;
		m_state.doB = doRun;
		return (TRUE);
	}

	BOOL Conveyor::SetParam(string nameA, string nameB, string nameC)
	{
		m_nameA = nameA;
		m_nameB = nameB;
		m_nameC = nameC;
		return (TRUE);
	}

	void Conveyor::Update()
	{
		if (OPR.isNoDevice)
		{
		}

		BOOL outA, outB;
		outA = outB = FALSE;

		// doA = doDir, doB = doRun
		if (NOT_DEFINED == m_state.doA && NOT_DEFINED != m_state.doB)
		{
			// Run
			outB = ::IsOn((enDO)m_state.doB);
		}
		else if (NOT_DEFINED != m_state.doA && NOT_DEFINED != m_state.doB)
		{
			// Dir Run
			outA = ::IsOn((enDO)m_state.doA);
			outB = ::IsOn((enDO)m_state.doB);
		}

		if (!outA && outB)
			m_state.oDir = conveyorFWD;
		else if (outA && outB)
			m_state.oDir = conveyorBWD;
		else if (!outA && !outB)
			m_state.oDir = conveyorSTOP;
		else 
			m_state.oDir = NOT_DEFINED;

		if (conveyorFWD == m_state.oDir || conveyorBWD == m_state.oDir || conveyorSTOP == m_state.oDir)
		{
			if (m_isDriving)
				SLogEndEvent();

			m_isDriving = FALSE;
		}

		if (m_oCmd != m_state.oDir)
		{
			if (m_isDriving)
			{
				SLogEndEvent();
				if (conveyorFWD == m_state.oDir || conveyorBWD == m_state.oDir || conveyorSTOP == m_state.oDir)
					SLogStartEvent();
			}
			else if (conveyorFWD == m_state.oDir || conveyorBWD == m_state.oDir || conveyorSTOP == m_state.oDir)
			{
				SLogEndEvent();
				SLogStartEvent();
				m_isDriving = TRUE;
			}
		}
		m_oCmd = m_state.oDir;
	}

	void Conveyor::Actuate(int cmdDir)
	{
		if (isVirtual)
			return;

		if (conveyorFWD == cmdDir)
			SetOut(FALSE, TRUE);
		else if (conveyorBWD == cmdDir)
			SetOut(TRUE, TRUE);
		else if (conveyorSTOP == cmdDir)
			SetOut(FALSE, FALSE);
	}

	void Conveyor::SetOut(BOOL isOnA, BOOL isOnB)
	{
		if (isVirtual)
			return;

		if (NOT_DEFINED != m_state.doA)
			::Set((enDO)m_state.doA, isOnA);
		if (NOT_DEFINED != m_state.doB)
			::Set((enDO)m_state.doB, isOnB);
	}

	void Conveyor::Fwd(INT32 index)
	{
		Actuate(conveyorFWD); 
	}

	void Conveyor::Bwd(INT32 index)
	{
		Actuate(conveyorBWD); 
	}

	void Conveyor::Stop(INT32 index)
	{ 
		Actuate(conveyorSTOP);
	}

	BOOL Conveyor::IsFwd(INT32 nDelay) { return (IsOn((enDO)m_state.doB, nDelay) && IsOff((enDO)m_state.doA)); }
	BOOL Conveyor::IsBwd(INT32 nDelay) { return (IsOn((enDO)m_state.doB, nDelay) && IsOn((enDO)m_state.doA, nDelay)); }
	BOOL Conveyor::IsStop(INT32 nDelay) { return (IsOff((enDO)m_state.doB, nDelay)); }

	void Conveyor::Simulation()
	{
		if (isVirtual)
			return;
	}
}

#ifndef CHECK_ACTUATOR_ARRAY
HW::Conveyor CONV[HW::Conveyor::maxDevice];
#else
CheckParser<HW::Conveyor, ConveyorList, ConveyorList(HW::Conveyor::maxDevice)> CONV;
#endif