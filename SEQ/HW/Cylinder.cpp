#include "..\Includes.h"

namespace HW
{
	Cylinder::Cylinder()
	{
		for (int i = 0; i < MAX_INDEX_CYLINDER; i++)
			m_indexName.Add("");

		isVirtual = (cylinderMAX > 0) ? 0 : 1;
		m_no = NOT_DEFINED;
		m_isDriving = FALSE;
		m_isError = FALSE;
		m_errorAct = NOT_DEFINED;
		m_errorUse = FALSE;

		m_state.diA = NOT_DEFINED;
		m_state.diB = NOT_DEFINED;
		m_state.doA = NOT_DEFINED;
		m_state.doB = NOT_DEFINED;
		m_state.iDir = NOT_DEFINED;
		m_state.oDir = NOT_DEFINED;
		m_oCmd = NOT_DEFINED;
		m_nameA = "";
		m_nameB = "";

		CanMove = Cylinder::DefaultCanMove;
		m_delayTime = 0;
		m_command.idx = 0;
		m_command.profile.delayTimeA = 0;
		m_command.profile.delayTimeB = 0;
		m_sLogFncAddStart = m_sLogFncAddEnd = "";
	}

	Cylinder::~Cylinder()
	{

	}

	BOOL Cylinder::DefaultCanMove(int dir, int index)
	{
		//if (OPR.isDoorOpen || OPR.isDoorUnlock)
		//{
		//	if (OPR.isCanMoveMsg)
		//	{
		//		MSGNOTICE.Set(MSG_NOTICE, "CLOSE", NULL, NULL, NULL, "Door Is Open.");
		//		return (FALSE);
		//	}
		//}
		return (TRUE);
	}

	void Cylinder::CreateInitParameter()
	{
		BOOL CanWrite = TRUE;
		std::filesystem::path createFile;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
		createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderIndexName.ini";
		CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

		CIni ini = { 0 };
		TCHAR strSection[1024] = { 0 };
		TCHAR strKeyCode[1024] = { 0 };
		TCHAR strStrName[1024] = { 0 };
		for (int i = 0; i < cylinderMAX; i++)
		{
			CylinderList no = CylinderList(i);
			CYL[no].m_no = i;
			CYL[no].m_command = CYL[no].CommandTable(0);
			CYL[no].CanMove = Cylinder::DefaultCanMove;

			if (!CanWrite)
				continue;

			CIni ini = { 0 };
			TCHAR strSection[1024] = { 0 };
			TCHAR strKeyCode[1024] = { 0 };
			TCHAR strStrName[1024] = { 0 };
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderName.ini");
			swprintf_s(strKeyCode, 100, L"CYLINDER%d", i);
			ini.GetString(L"CYLINDERNAME", strKeyCode, strStrName, 512);
			CYL[no].m_name = GetAscCode(strStrName);
			for (int indexNo = 0; indexNo < MAX_INDEX_CYLINDER; indexNo++)
			{
				ZeroMemory(strSection, sizeof(strSection));
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderIndexName.ini");
				swprintf_s(strSection, 100, L"CYLINDERINDEXNAME%d", i);
				swprintf_s(strKeyCode, 100, L"CYLINDERINDEXNAME%d", indexNo);
				ini.GetString(strSection, strKeyCode, strStrName, 512);
				CYL[no].m_indexName[indexNo] = GetAscCode(strStrName);
			}	
		}
	}

	void Cylinder::SLogFncAddMessage(string message)
	{
		m_sLogFncAddStart = message;
	}

	void Cylinder::SLogStartEvent()
	{
		m_delayTime = (cylinderFWD == m_state.oDir) ? m_command.profile.delayTimeA : m_command.profile.delayTimeB;
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].IsRecording(CylinderList(m_no)) && SLOG[no].cylinderLogStart[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].cylinderLogStart[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].cylinderLogStart[m_no].GetXFREventId() + "_" + m_indexName[m_command.idx] + m_sLogFncAddStart).c_str(), "START",
					SLOG[no].cylinderLogStart[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].cylinderLogStart[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'CYLINDER'",
					"'ACT_NAME'", ("'C" + ToString(m_no) + "'").c_str(),
					"'DELAYTIME'", ToString(m_delayTime).c_str());
				SLOG[no].cylinderLogEnd[m_no] = SLOG[no].cylinderLogStart[m_no];
			}
		}
		m_sLogFncAddEnd = m_sLogFncAddStart;
		m_sLogFncAddStart = "";
	}

	void Cylinder::SLogEndEvent()
	{
		m_delayTime = (cylinderFWD == m_state.oDir) ? m_command.profile.delayTimeA : m_command.profile.delayTimeB;
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (SLOG[no].cylinderLogEnd[m_no].m_uniqueXFRNumber)
			{
				SMESSAGE.Function(SLOG[no].cylinderLogEnd[m_no].GetXFRDeviceId().c_str(),
					(SLOG[no].cylinderLogEnd[m_no].GetXFREventId() + "_" + m_indexName[m_command.idx] + m_sLogFncAddEnd).c_str(), "END",
					SLOG[no].cylinderLogEnd[m_no].GetXFRMaterialId().c_str(),
					SLOG[no].cylinderLogEnd[m_no].GetXFRMaterialType().c_str(),
					"'TYPE'", "'CYLINDER'",
					"'ACT_NAME'", ("'C" + ToString(m_no) + "'").c_str(),
					"'DELAYTIME'", ToString(m_delayTime).c_str());
				SLOG[no].cylinderLogEnd[m_no].End(TRUE);
			}
		}
		m_sLogFncAddEnd = "";
	}

	CylCommand::SingleParamter Cylinder::CommandTable(int idx)
	{
		if (!Between(m_no , 0 , cylinderMAX - 1) || !Between(idx , 0 , MAX_INDEX_CYLINDER - 1))
		{
			throw CException(CException::OUT_OF_RANGE , __LINE__ , __FUNCTION__);
		}

		CylCommand::SingleParamter command;
		command.no = m_no;
		command.idx = idx;
		command.profile.delayTimeA = NV.cylTable[m_no].delayTimeA[idx];
		command.profile.delayTimeB = NV.cylTable[m_no].delayTimeB[idx];
		return command;
	}

	BOOL Cylinder::SetIO(enDI diA, enDI diB, enDO doA, enDO doB)
	{
		m_state.diA = diA;
		m_state.diB = diB;
		m_state.doA = doA;
		m_state.doB = doB;
		return (TRUE);
	}

	BOOL Cylinder::SetParam(string nameA, string nameB)
	{
		m_nameA = nameA;
		m_nameB = nameB;
		memcpy(&m_table, &NV.cylTable[m_no], sizeof(m_table));
		return (TRUE);
	}

	void Cylinder::SetErr(int timeLimit, ErrName errorCode, int errorAct, BOOL errorUse)
	{
		m_timeLimit = timeLimit;
		m_errorCode = errorCode;
		m_errorAct = errorAct;
		m_errorUse = errorUse;
	}

	void Cylinder::SetErr(int errorAct)
	{
		m_errorAct = errorAct;
	}

	void Cylinder::Reset()
	{
		m_tmError.Reset();
		m_isError = FALSE;
	}

	void Cylinder::Update()
	{
		if (OPR.isNoDevice)
		{
			if (NOT_DEFINED != m_state.doA && NOT_DEFINED != m_state.doB)
			{
				if (NOT_DEFINED != m_state.diA)
					::Set((enDI)m_state.diA, ::IsOn((enDO)m_state.doA, m_command.profile.delayTimeA));
				if (NOT_DEFINED != m_state.diB)
					::Set((enDI)m_state.diB, ::IsOn((enDO)m_state.doB, m_command.profile.delayTimeB));
			}
			else if (NOT_DEFINED == m_state.doA && NOT_DEFINED != m_state.doB)
			{
				if (NOT_DEFINED != m_state.diA)
					::Set((enDI)m_state.diA, ::IsOff((enDO)m_state.doB, m_command.profile.delayTimeA));
				if (NOT_DEFINED != m_state.diB)
					::Set((enDI)m_state.diB, ::IsOn((enDO)m_state.doB, m_command.profile.delayTimeB));
			}
			else if (NOT_DEFINED != m_state.doA && NOT_DEFINED == m_state.doB)
			{
				if (NOT_DEFINED != m_state.diA)
					::Set((enDI)m_state.diA, ::IsOn((enDO)m_state.doA, m_command.profile.delayTimeA));
				if (NOT_DEFINED != m_state.diB)
					::Set((enDI)m_state.diB, ::IsOff((enDO)m_state.doA, m_command.profile.delayTimeB));
			}
		}

		BOOL inA, inB, outA, outB;
		inA = inB = outA = outB = FALSE;

		if (NOT_DEFINED == m_state.doA && NOT_DEFINED != m_state.doB)
		{
			outB = ::IsOn((enDO)m_state.doB);
			outA = !outB;
		}
		else if (NOT_DEFINED != m_state.doA && NOT_DEFINED == m_state.doB)
		{
			outA = ::IsOn((enDO)m_state.doA);
			outB = !outA;
		}
		else if (NOT_DEFINED != m_state.doA && NOT_DEFINED != m_state.doB)
		{
			outA = ::IsOn((enDO)m_state.doA);
			outB = ::IsOn((enDO)m_state.doB);
		}

		if (NOT_DEFINED == m_state.diA && NOT_DEFINED != m_state.diB)
		{
			inB = ::AOn((enDI)m_state.diB);
			inA = !inB;
		}
		else if (NOT_DEFINED != m_state.diA && NOT_DEFINED == m_state.diB)
		{
			inA = ::AOn((enDI)m_state.diA);
			inB = !inA;
		}
		else if (NOT_DEFINED != m_state.diA && NOT_DEFINED != m_state.diB)
		{
			inA = ::AOn((enDI)m_state.diA);
			inB = ::AOn((enDI)m_state.diB);
		}
		else
		{
			inA = outA;
			inB = outB;
		}

		if (inA && !inB)
			m_state.iDir = cylinderFWD;
		else if (!inA && inB)
			m_state.iDir = cylinderBWD;
		else
			m_state.iDir = NOT_DEFINED;

		if ((outA && !outB) || ((outA == outB) && (inA && !inB)))
			m_state.oDir = cylinderFWD;
		else if ((!outA && outB) || ((outA == outB) && (!inA && inB)))
			m_state.oDir = cylinderBWD;
		else
			m_state.oDir = NOT_DEFINED;

		if (m_oCmd != m_state.oDir)
		{
			if (m_isDriving)
			{
				SLogEndEvent();
				if (cylinderFWD == m_state.oDir || cylinderBWD == m_state.oDir)
					SLogStartEvent();
			}
			else if (cylinderFWD == m_state.oDir || cylinderBWD == m_state.oDir)
			{
				SLogEndEvent();
				SLogStartEvent();
				m_isDriving = TRUE;
			}
			m_tmDelay.Reset();
			m_tmError.Reset();
		}
		m_oCmd = m_state.oDir;

		if (m_isDriving)
		{
			if (cylinderFWD == m_state.oDir)
				m_state.elapsedTimeA = m_tmDelay.ElapsedSec();
			else if (cylinderBWD == m_state.oDir)
				m_state.elapsedTimeB = m_tmDelay.ElapsedSec();
		}

		if (m_state.iDir == m_state.oDir &&
			(cylinderFWD == m_state.iDir || cylinderBWD == m_state.iDir) &&
			(cylinderFWD == m_state.oDir || cylinderBWD == m_state.oDir) &&
			m_tmDelay.TmOver(m_delayTime))
		{
			if (m_isDriving)
				SLogEndEvent();	
			
			m_isDriving = FALSE;
			m_tmError.Reset();
		}

		if (m_errorUse &&
			(NOT_DEFINED != m_state.diA || NOT_DEFINED != m_state.diB) &&
			m_tmError.TmOver(m_timeLimit))
		{
			if (m_isDriving)
				SLogEndEvent();
			
			m_isDriving = FALSE;
			if (!m_isError)
				Actuate(m_errorAct);
			
			//if (0 < m_errorCode)
			//	ER.Save(m_errorCode);

			m_isError = TRUE;
		}
	}

	int Cylinder::GetPos()
	{
		if (isVirtual)
			return (NOT_DEFINED);

		if (m_state.iDir == m_state.oDir &&
			(cylinderFWD == m_state.iDir || cylinderBWD == m_state.iDir) &&
			(cylinderFWD == m_state.oDir || cylinderBWD == m_state.oDir))
		{
			if (m_tmDelay.TmOver(m_delayTime))
			{
				return m_state.oDir;
			}
		}
		return (NOT_DEFINED);
	}

	int Cylinder::GetErr()
	{
		if (isVirtual)
			return (0);

		return m_isError;
	}

	void Cylinder::Actuate(int cmdDir)
	{
		if (isVirtual)
			return;

		if (cylinderFWD == cmdDir)
			SetOut(TRUE, FALSE);
		else if (cylinderBWD == cmdDir)
			SetOut(FALSE, TRUE);
		else if (cylinderFREE == cmdDir)
			SetOut(FALSE, FALSE);
	}

	void Cylinder::SetOut(BOOL isOnA, BOOL isOnB)
	{
		if (isVirtual)
			return;

		if (NOT_DEFINED != m_state.doA)
			::Set((enDO)m_state.doA, isOnA);
		if (NOT_DEFINED != m_state.doB)
			::Set((enDO)m_state.doB, isOnB);
	}

	void Cylinder::Fwd(INT32 index)
	{
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderFWD); 
	}

	void Cylinder::Bwd(INT32 index)
	{
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderBWD); 
	}

	void Cylinder::Open(INT32 index)
	{
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderOPEN);
	}

	void Cylinder::Close(INT32 index)
	{
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderCLOSE);
	}

	void Cylinder::Up(INT32 index)
	{ 
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderUP);
	}

	void Cylinder::Dn(INT32 index)
	{ 
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderDOWN);
	}

	void Cylinder::Cw(INT32 index)
	{ 
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderCW);
	}

	void Cylinder::CCw(INT32 index)
	{ 
		m_command = CommandTable(Between(index , 0 , MAX_INDEX_CYLINDER - 1) ? index : 0);
		Actuate(cylinderCCW);
	}

	BOOL Cylinder::IsFwd() { return (cylinderFWD == GetPos()); }
	BOOL Cylinder::IsBwd() { return (cylinderBWD == GetPos()); }
	BOOL Cylinder::IsOpen() { return (cylinderOPEN == GetPos()); }
	BOOL Cylinder::IsClose() { return (cylinderCLOSE == GetPos()); }
	BOOL Cylinder::IsUp() { return (cylinderUP == GetPos()); }
	BOOL Cylinder::IsDn() { return (cylinderDOWN == GetPos()); }
	BOOL Cylinder::IsCw() { return (cylinderCW == GetPos()); }
	BOOL Cylinder::IsCCw() { return (cylinderCCW == GetPos()); }

	void Cylinder::Simulation()
	{
		if (isVirtual)
			return;

		BOOL outA, outB;
		outA = outB = FALSE;

		if (NOT_DEFINED == m_state.doA && NOT_DEFINED != m_state.doB)
		{
			outB = ::IsOn((enDO)m_state.doB);
			outA = !outB;
		}
		else if (NOT_DEFINED != m_state.doA && NOT_DEFINED == m_state.doB)
		{
			outA = ::IsOn((enDO)m_state.doA);
			outB = !outA;
		}
		else if (NOT_DEFINED != m_state.doA && NOT_DEFINED != m_state.doB)
		{
			outA = ::IsOn((enDO)m_state.doA);
			outB = ::IsOn((enDO)m_state.doB);
		}

		if (outA && !outB)
		{// Fwd
			if (NOT_DEFINED != m_state.diA)
				::Set((enDI)m_state.diA, TRUE);

			if (NOT_DEFINED != m_state.diB)
				::Set((enDI)m_state.diB, FALSE);
		}
		else if (!outA && outB)
		{// Bwd
			if (NOT_DEFINED != m_state.diA)
				::Set((enDI)m_state.diA, FALSE);

			if (NOT_DEFINED != m_state.diB)
				::Set((enDI)m_state.diB, TRUE);
		}
	}
}

#ifndef CHECK_ACTUATOR_ARRAY
HW::Cylinder CYL[HW::Cylinder::maxDevice];
#else
CheckParser<HW::Cylinder, CylinderList, CylinderList(HW::Cylinder::maxDevice)> CYL;
#endif