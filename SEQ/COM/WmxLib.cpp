#include "..\Includes.h"

namespace COM
{
	static const UINT16 MAX_DI_CH = (enDI::diLAST != 0) ? ((enDI::diLAST / 100) + 1) : 0;
	static const UINT16 MAX_DO_CH = (enDO::doLAST != 0) ? ((enDO::doLAST / 100) + 1) : 0;
	static const UINT16 MAX_AI_CH = (enAI::aiLAST != 0) ? ((enAI::aiLAST / 100) + 1) : 0;
	static const UINT16 MAX_AO_CH = (enAO::aoLAST != 0) ? ((enAO::aoLAST / 100) + 1) : 0;
	static const UINT16 MAX_USERMEM_BUFFER_CH = (MAX_USERMEM_BUFFER_COUNT / MAX_USERMEM_CONTACT) % 2 ? (MAX_USERMEM_BUFFER_COUNT / MAX_USERMEM_CONTACT) + 1 : (MAX_USERMEM_BUFFER_COUNT / MAX_USERMEM_CONTACT);
	static const UINT16 MAX_USERMEM_CH = (enUSERMEM::userMemLAST != 0) ? ((enUSERMEM::userMemLAST / 100) + 1 + MAX_USERMEM_BUFFER_CH) : 0;

	CWmxLib::CWmxLib()
	{
		ZeroMemory(m_iReadWriteData, sizeof(m_iReadWriteData));
		ZeroMemory(m_oReadWriteData, sizeof(m_oReadWriteData));
		ZeroMemory(m_oReadOnlyData, sizeof(m_oReadOnlyData));
		ZeroMemory(nSlaveType, sizeof(nSlaveType));
		ZeroMemory(m_userMemReadWriteData, sizeof(m_userMemReadWriteData));
		ZeroMemory(m_userMemReadOnlyData, sizeof(m_userMemReadOnlyData));

		m_iMaxDataSize = 0;
		m_oMaxDataSize = 0;

		ZeroMemory(&m_iPdoSorting, sizeof(m_iPdoSorting));
		ZeroMemory(&m_oPdoSorting, sizeof(m_oPdoSorting));

		ZeroMemory(iBitMem, sizeof(iBitMem));
		ZeroMemory(oBitMem, sizeof(oBitMem));

		panasonicChannelCount = 0;
	}

	CWmxLib::~CWmxLib()
	{
	}

	void CWmxLib::Update(void)
	{
		WriteIO();
		Connection();
		ReadIO();
		coremotion.GetStatus(&status);
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			MT[mtno].GetState();
		}
		for (int i = 0; i < apibufferMAX; i++)
		{
			ApiBufferList abno = ApiBufferList(i);
			APIBUFFER[abno].CommandCycle();
		}
		for (int i = 0; i < interpolationMAX; i++)
		{
			InterpolationMotorList lnno = InterpolationMotorList(i);
			LN[lnno].PosCommandCycle();
		}
		for (int i = 0; i < motorMAX; i++)
		{
			AxisList mtno = AxisList(i);
			if (NULL != MT[mtno].HomeExternalCycle)
				MT[mtno].HomeExternalCycle();
			else	
				MT[mtno].HomeInternalCycle();

			MT[mtno].HomeSearchCycle();
			MT[mtno].JogCommandCycle();
			MT[mtno].PosCommandCycle();
			MT[mtno].VelCommandCycle();
			MT[mtno].TrqCommandCycle();
		}
	}

	void CWmxLib::WriteIO(void)
	{
		if (!isIoConnected)
		{
			for (int ch = 0; ch < MAX_DO_CH; ch++)
			{
				for (int no = 0; no < MAX_CONTACT; no++)
				{
					toOff[ch][no].Reset();
					toOn[ch][no].Reset();
				}
			}
			return;
		}

		for (int ch = 0; ch < MAX_DO_CH; ch++)
		{
			for (int no = 0; no < MAX_CONTACT; no++)
			{
				enDO oNo = (enDO)((ch * 100) + no);
				if (IsReadOnly(oNo))
				{
					UINT16 ch16 = 0;
					io.GetOutBytes(m_do[ch].byteAddress, sizeof(BYTE) * 2, (UINT8*)&ch16);
					Set(oNo, !!(ch16 & g_bitMask16[no]));
				}

				if (IsOn(oNo))
				{
					if (!(oBitMem[ch] & g_bitMask16[no]))
					{
						LOG[logIO].Message("output,%.4d,on", (ch * 100) + no);
						oBitMem[ch] |= g_bitMask16[no];
					}
					toOff[ch][no].Reset();
				}
				else
				{
					if (oBitMem[ch] & g_bitMask16[no])
					{
						LOG[logIO].Message("output,%.4d,off", (ch * 100) + no);
						oBitMem[ch] &= ~g_bitMask16[no];
					}
					toOn[ch][no].Reset();
				}
			}
		}

		for (int ch = 0; ch < MAX_USERMEM_CH; ch++)
		{
			for (int no = 0; no < MAX_USERMEM_CONTACT; no++)
			{
				enUSERMEM oNo = (enUSERMEM)((ch * 100) + no);
				if (IsUserMemoryReadOnly(oNo))
				{
					UINT8 ch08 = 0;
					usermemory.GetMBytes(ch, 1, (unsigned char*)&ch08);
					UserMemorySet(oNo, !!(ch08 & g_bitMask08[no]));
				}
				if (MAX_CH <= ch)
					continue;

				if (IsUserMemoryOn(oNo))
				{
					if (!(userBitMem[ch] & g_bitMask08[no]))
					{
						LOG[logIO].Message("usermem,%.4d,on", (ch * 100) + no);
						userBitMem[ch] |= g_bitMask08[no];
					}
					tusermemOff[ch][no].Reset();
				}
				else
				{
					if (userBitMem[ch] & g_bitMask08[no])
					{
						LOG[logIO].Message("usermem,%.4d,off", (ch * 100) + no);
						userBitMem[ch] &= ~g_bitMask08[no];
					}
					tusermemOn[ch][no].Reset();
				}
			}
		}
		io.SetOutBytes(0, m_oMaxDataSize, &m_oReadWriteData[0]);
		usermemory.SetMBytes(0, MAX_USERMEM_CH, (unsigned char*)m_userMemReadWriteData);
	}

	void CWmxLib::ReadIO(void)
	{
		if (!isIoConnected)
		{
			for (int ch = 0; ch < MAX_DI_CH; ch++)
			{
				for (int no = 0; no < MAX_CONTACT; no++)
				{
					tiOff[ch][no].Reset();
					tiOn[ch][no].Reset();
				}
			}
			return;
		}

		if (!OPR.isNoDevice)
		{
			io.GetInBytes(0, m_iMaxDataSize, &m_iReadWriteData[0]);
		}

		for (int ch = 0; ch < MAX_DI_CH; ch++)
		{
			for (int no = 0; no < MAX_CONTACT; no++)
			{
				enDI iNo = (enDI)((ch * 100) + no);
				if (AOn(iNo))
				{
					if (!(iBitMem[ch] & g_bitMask16[no]))
					{
						LOG[logIO].Message("input,%.4d,on", (ch * 100) + no);
						iBitMem[ch] |= g_bitMask16[no];
					}
					tiOff[ch][no].Reset();
				}
				else
				{
					if (iBitMem[ch] & g_bitMask16[no])
					{
						LOG[logIO].Message("input,%.4d,off", (ch * 100) + no);
						iBitMem[ch] &= ~g_bitMask16[no];
					}
					tiOn[ch][no].Reset();
				}
			}
		}
	}

	void CWmxLib::Connection(void)
	{
		if (OPR.isNoDevice)
		{
			isConnected = (TRUE);
			isIoConnected = (TRUE);
			return;
		}

		ecat.GetMasterInfo(&masterInfo);

		int axesCnt = 0;
		isIoConnected = (TRUE);
		isMtConnected = ((nMtslaveCnt + nIoslaveCnt) == masterInfo.numOfSlaves);
		for (int i = 0; i < (nMtslaveCnt + nIoslaveCnt); i++)
		{
			ecApi::EcStateMachine::T isOpState = ecApi::EcStateMachine::Op;
			if (SLAVE_MT_TYPE == nSlaveType[i])
			{
				isMtConnected &= (isOpState == masterInfo.slaves[i].state);
				isMtConnected &= (0 < masterInfo.slaves[i].numOfAxes);
				axesCnt += masterInfo.slaves[i].numOfAxes;
			}
			else
			{
				isIoConnected &= (isOpState == masterInfo.slaves[i].state);
			}
		}

		axesCnt += nMtDummyCnt;

		if (motorMAX != axesCnt)
			isMtConnected = (FALSE);

		if (isMtConnected && isIoConnected)
		{
			isConnected = (TRUE);
			// 25.08.08 EMO -> Reset -> Does Not Turn On.
			if (!IsMotorPwr())
				MotorPwr(TRUE, nHotConnectCnt);
			else
			{
				if (!isPastConnect)
				{
					for (int mtno = 0; mtno < motorMAX; mtno++)
					{
						if (!OPR.isDebugMode)
							coremotion.axisControl->SetServoOn(mtno, 1);
					}
					LOG[logSYS].Message("All Node is Connected!");
				}
			}

			tmConnectLoop.Reset();
		}
		else
		{
			isConnected = (FALSE);
			if (isPastConnect)
			{
				for (int mtno = 0; mtno < motorMAX; mtno++)
				{
					if (MT[AxisList(mtno)].isVirtualHW) continue;
					MT[AxisList(mtno)].CancelHomeSearch();
				}

				ZeroMemory(m_oReadWriteData, sizeof(m_oReadWriteData));

				api.StopCommunication();
				nStartCommCnt = 0;
				nHotConnectCnt = 0;

				OPR.isCheckPkg = (TRUE);
				OPR.isCheckSys = (TRUE);
				OPR.isCheckRcp = (TRUE);

				for (int i = 0; i < fsmMAX; i++)
					FSM[FsmList(i)].Set(C_IDLE);

				LOG[logSYS].Message("Disconnect EtherCat Communication!");
				tmConnectLoop.Reset();
			}

			OPR.isStop = (TRUE);
			OPR.isAuto = (FALSE);
		}

		isPastConnect = (isMtConnected && isIoConnected);
		if (!tmConnectLoop.TmOver(3000))
			return;

		tmConnectLoop.Reset();

		if (isIoConnected)
		{
			if (nHotConnectCnt > 50)
			{
				api.StopCommunication();
				nHotConnectCnt = 0;
			}
			else if (IsMotorPwr())
			{
				LOG[logSYS].Message("Try to StartHotConnect!");
				ecat.StartHotconnect();
				nHotConnectCnt++;
			}
			else
			{
				MotorPwr(TRUE, nHotConnectCnt);
				nHotConnectCnt++;
			}
		}
		else
		{
			if (nStartCommCnt > 10)
			{
				api.StopCommunication();
				nStartCommCnt = 0;
			}
			else
			{
				api.StartCommunication();
				nStartCommCnt++;
			}
		}
	}

	BOOL CWmxLib::Open(void)
	{
		int errCode = 0;
		errCode = api.CreateDevice((wchar_t*)L"C:\\Program Files\\SoftServo\\WMX3\\", DeviceType::DeviceTypeNormal);
		if (errCode)
			return (FALSE);

		errCode = api.SetWatchdog(0xFFFFFFFF);
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}

		DevicesInfoW devices;
		errCode = api.GetAllDevices(&devices);
		if (errCode)
		{
			if (ErrorCode::DeviceIsNull == errCode)
			{
				errCode = 0;
			}
			else
			{
				api.CloseDevice();
				return (FALSE);
			}
		}
		else
		{
			for (int i = 0; i < (int)devices.count; i++)
			{
				if (0 == wcscmp(devices.devices[i].name, L"EtherCat Lib"))
					api.SetWatchdog(i, 1000);
			}
		}

		errCode = api.SetDeviceName((wchar_t*)L"EtherCat Lib");
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}

		errCode = api.GetAllDevices(&devices);
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}
		else
		{
			for (int i = 0; i < (int)devices.count; i++)
			{
				if (0 == wcscmp(devices.devices[i].name, L""))
					api.SetWatchdog(i, 1000);
			}
		}

		errCode = api.StopCommunication();
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}

		WCHAR strFolderPath[MAX_PATH] = {0,};
		if (!GetCurDirectory(strFolderPath))
		{
			api.CloseDevice();
			return (FALSE);
		}
		wcscat(strFolderPath, L"\\wmx_parameters.xml");
		errCode = coremotion.config->ImportAndSetAll(strFolderPath);
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}

		errCode = api.StartCommunication();
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}

		isConnected = (FALSE);

		for (int i = 0; i < MAX_API_BUFFER_CHANNEL; i++)
		{
			apibuffer.Halt(i);
			Sleep(100);
			apibuffer.Clear(i);
			apibuffer.FreeApiBuffer(i);
			Sleep(100);
			if (apibuffer.CreateApiBuffer(i, 1024 * 1024 * 3))
			{
				api.CloseDevice();
				return (FALSE);
			}
		}

		ModulesInfoW modules;
		errCode = api.GetModulesInfo(&modules);
		if (errCode)
		{
			api.CloseDevice();
			return (FALSE);
		}

		int isEnableEtherCAT = 0;
		int isEnableSimulation = 0;
		for (int i = 0; i < modules.numOfModule; i++)
		{
			if (!wcscmp(L"EtherCAT", modules.modules[i].moduleName))
				isEnableEtherCAT++;

			if (!wcscmp(L"Simulation", modules.modules[i].moduleName))
				isEnableSimulation++;
		}

		if (1 == isEnableEtherCAT && 0 == isEnableSimulation)
		{
			OPR.isNoDevice = (FALSE);
		}
		else if (0 == isEnableEtherCAT && 1 == isEnableSimulation)
		{
			OPR.isNoDevice = (TRUE);
			isConnected = (TRUE);
			isIoConnected = (TRUE);
		}
		else
		{
			api.CloseDevice();
			return (FALSE);
		}

		return (TRUE);
	}

	void CWmxLib::InitPdoSorting(void)
	{
		if (0 < m_iPdoSorting.dataCount)
		{
			for (UINT i = 0; i < m_iPdoSorting.dataCount; i++)
			{
				UINT channel = m_iPdoSorting.channel[i];
				switch (m_iPdoSorting.dataType[i])
				{
				case PdoSorting::DIGITAL:
				{
					m_di[channel].byteAddress = m_iPdoSorting.rvaByte[i] + m_iMaxDataSize;
					m_di[channel].dataAddress = &m_iReadWriteData[0] + m_di[channel].byteAddress;
					if (!m_iPdoSorting.isWago[i])
					{
						m_di[channel].byteAddress += m_iPdoSorting.rvaWagoMaxSize;
						m_di[channel].dataAddress += m_iPdoSorting.rvaWagoMaxSize;
					}
				}
				break;
				case PdoSorting::ANALOGUE:
				{
					m_ai[channel].byteAddress = m_iPdoSorting.rvaByte[i] + m_iMaxDataSize;
					m_ai[channel].dataAddress = &m_iReadWriteData[0] + m_ai[channel].byteAddress;
					if (!m_iPdoSorting.isWago[i])
					{
						m_ai[channel].byteAddress += m_iPdoSorting.rvaWagoMaxSize;
						m_ai[channel].dataAddress += m_iPdoSorting.rvaWagoMaxSize;
					}
				}
				break;
				}
			}
			m_iMaxDataSize += (m_iPdoSorting.rvaWagoMaxSize + m_iPdoSorting.rvaMaxSize);
			ZeroMemory(&m_iPdoSorting, sizeof(m_iPdoSorting));
		}
	
		if (0 < m_oPdoSorting.dataCount)
		{
			for (UINT i = 0; i < m_oPdoSorting.dataCount; i++)
			{
				UINT channel = m_oPdoSorting.channel[i];
				switch (m_oPdoSorting.dataType[i])
				{
				case PdoSorting::DIGITAL:
				{
					m_do[channel].byteAddress = m_oPdoSorting.rvaByte[i] + m_oMaxDataSize;
					m_do[channel].dataAddress = &m_oReadWriteData[0] + m_do[channel].byteAddress;
					if (!m_oPdoSorting.isWago[i])
					{
						m_do[channel].byteAddress += m_oPdoSorting.rvaWagoMaxSize;
						m_do[channel].dataAddress += m_oPdoSorting.rvaWagoMaxSize;
					}
				}
				break;
				case PdoSorting::ANALOGUE:
				{
					m_ao[channel].byteAddress = m_oPdoSorting.rvaByte[i] + m_oMaxDataSize;
					m_ao[channel].dataAddress = &m_oReadWriteData[0] + m_ao[channel].byteAddress;
					if (!m_oPdoSorting.isWago[i])
					{
						m_ao[channel].byteAddress += m_oPdoSorting.rvaWagoMaxSize;
						m_ao[channel].dataAddress += m_oPdoSorting.rvaWagoMaxSize;
					}
				}
				break;
				}
			}
			m_oMaxDataSize += (m_oPdoSorting.rvaWagoMaxSize + m_oPdoSorting.rvaMaxSize);
			ZeroMemory(&m_oPdoSorting, sizeof(m_oPdoSorting));
		}
	}

	void CWmxLib::InitPdoDevice(SLAVE_MODULE_TYPE module)
	{
		switch (module)
		{
		case UR20_HUB:
		{
			InitPdoSorting();
			nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_IO_TYPE;//io type
			nIoslaveCnt++;
		}
		break;
		case UR20_FBC_EC:
		{
			InitPdoSorting();
			nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_IO_TYPE;//io type
			nIoslaveCnt++;
		}
		break;
		case WAGO_HUB:
		{
			InitPdoSorting();
			nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_IO_TYPE;//io type
			nIoslaveCnt++;
		}
		break;
		case WAGO_750_354_COUPLER:
		{
			InitPdoSorting();
			nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_IO_TYPE;//io type
			nIoslaveCnt++;
			m_iMaxDataSize += 4;
			m_oMaxDataSize += 4;
		}
		break;
		};
	}

	void CWmxLib::InitPdoDevice(DIGITAL_INPUT_MODULE_TYPE module, int channelNo)
	{
		switch (module)
		{
		case UR20_16DI_N:
		{
			m_iPdoSorting.dataType[m_iPdoSorting.dataCount] = PdoSorting::DIGITAL;
			m_iPdoSorting.channel[m_iPdoSorting.dataCount] = channelNo;
			m_iPdoSorting.rvaByte[m_iPdoSorting.dataCount] = m_iPdoSorting.rvaMaxSize;
			m_iPdoSorting.rvaMaxSize += 2;
			m_iPdoSorting.dataCount++;
		}
		break;
		case WAGO_750_1407_DI_16CH:
		{
			m_iPdoSorting.dataType[m_iPdoSorting.dataCount] = PdoSorting::DIGITAL;
			m_iPdoSorting.channel[m_iPdoSorting.dataCount] = channelNo;
			m_iPdoSorting.rvaByte[m_iPdoSorting.dataCount] = m_iPdoSorting.rvaMaxSize;
			m_iPdoSorting.rvaMaxSize += 2;
			m_iPdoSorting.dataCount++;
		}
		break;
		};
	}

	void CWmxLib::InitPdoDevice(DIGITAL_OUTPUT_MODULE_TYPE module, int channelNo)
	{
		switch (module)
		{
		case UR20_16DO_N:
		{
			m_oPdoSorting.dataType[m_oPdoSorting.dataCount] = PdoSorting::DIGITAL;
			m_oPdoSorting.channel[m_oPdoSorting.dataCount] = channelNo;
			m_oPdoSorting.rvaByte[m_oPdoSorting.dataCount] = m_oPdoSorting.rvaMaxSize;
			m_oPdoSorting.rvaMaxSize += 2;
			m_oPdoSorting.dataCount++;
		}
		break;
		case WAGO_750_1505_DO_16CH:
		{
			m_oPdoSorting.dataType[m_oPdoSorting.dataCount] = PdoSorting::DIGITAL;
			m_oPdoSorting.channel[m_oPdoSorting.dataCount] = channelNo;
			m_oPdoSorting.rvaByte[m_oPdoSorting.dataCount] = m_oPdoSorting.rvaMaxSize;
			m_oPdoSorting.rvaMaxSize += 2;
			m_oPdoSorting.dataCount++;
		}
		break;
		};
	}

	void CWmxLib::InitPdoDevice(ANALOG_INPUT_MODULE_TYPE module, int channelNo)
	{
		switch (module)
		{
		case UR20_4AI_UI_16:
		{
			m_iPdoSorting.dataType[m_iPdoSorting.dataCount] = PdoSorting::ANALOGUE;
			m_iPdoSorting.channel[m_iPdoSorting.dataCount] = channelNo;
			m_iPdoSorting.rvaByte[m_iPdoSorting.dataCount] = m_iPdoSorting.rvaMaxSize;
			m_iPdoSorting.rvaMaxSize += 8;
			m_iPdoSorting.dataCount++;
		}
		break;
		case WAGO_750_459_AI_4CH:
		case WAGO_750_471_AI_4CH:
		{
			m_iPdoSorting.isWago[m_iPdoSorting.dataCount] = TRUE;

			m_iPdoSorting.dataType[m_iPdoSorting.dataCount] = PdoSorting::ANALOGUE;
			m_iPdoSorting.channel[m_iPdoSorting.dataCount] = channelNo;
			m_iPdoSorting.rvaByte[m_iPdoSorting.dataCount] = m_iPdoSorting.rvaWagoMaxSize;
			m_iPdoSorting.rvaWagoMaxSize += 8;
			m_iPdoSorting.dataCount++;
		}
		break;
		};
	}

	void CWmxLib::InitPdoDevice(ANALOG_OUTPUT_MODULE_TYPE module, int channelNo)
	{
		switch (module)
		{
		case UR20_4AO_UI_16:
		{
			m_oPdoSorting.dataType[m_oPdoSorting.dataCount] = PdoSorting::ANALOGUE;
			m_oPdoSorting.channel[m_oPdoSorting.dataCount] = channelNo;
			m_oPdoSorting.rvaByte[m_oPdoSorting.dataCount] = m_oPdoSorting.rvaMaxSize;
			m_oPdoSorting.rvaMaxSize += 8;
			m_oPdoSorting.dataCount++;

			//m_iPdoSorting.rvaMaxSize += 1;
		}
		break;
		case WAGO_750_564_AO_4CH:
		{
			m_oPdoSorting.isWago[m_oPdoSorting.dataCount] = TRUE;

			m_oPdoSorting.dataType[m_oPdoSorting.dataCount] = PdoSorting::ANALOGUE;
			m_oPdoSorting.channel[m_oPdoSorting.dataCount] = channelNo;
			m_oPdoSorting.rvaByte[m_oPdoSorting.dataCount] = m_oPdoSorting.rvaWagoMaxSize;
			m_oPdoSorting.rvaWagoMaxSize += 8;
			m_oPdoSorting.dataCount++;
		}
		break;
		};
	}

	void CWmxLib::InitPdoDevice(KOSES_MODULE_TYPE module, int inputChannelNo1, int inputChannelNo2, int outputChannelNo)
	{
		switch (module)
		{
		case KOSES_8CH_VACUUM:
		{
			InitPdoSorting();
			nIoslaveCnt++;

			m_iPdoSorting.dataType[m_iPdoSorting.dataCount] = PdoSorting::ANALOGUE;
			m_iPdoSorting.channel[m_iPdoSorting.dataCount] = inputChannelNo1;
			m_iPdoSorting.rvaByte[m_iPdoSorting.dataCount] = m_iPdoSorting.rvaMaxSize;
			m_iPdoSorting.rvaMaxSize += 8;
			m_iPdoSorting.dataCount++;

			m_iPdoSorting.dataType[m_iPdoSorting.dataCount] = PdoSorting::ANALOGUE;
			m_iPdoSorting.channel[m_iPdoSorting.dataCount] = inputChannelNo2;
			m_iPdoSorting.rvaByte[m_iPdoSorting.dataCount] = m_iPdoSorting.rvaMaxSize;
			m_iPdoSorting.rvaMaxSize += 8;
			m_iPdoSorting.dataCount++;

			m_oPdoSorting.dataType[m_oPdoSorting.dataCount] = PdoSorting::DIGITAL;
			m_oPdoSorting.channel[m_oPdoSorting.dataCount] = outputChannelNo;
			m_oPdoSorting.rvaByte[m_oPdoSorting.dataCount] = m_oPdoSorting.rvaMaxSize;
			m_oPdoSorting.rvaMaxSize += 2;
			m_oPdoSorting.dataCount++;
		}
		break;
		};
	}

	void CWmxLib::InitPdoDevice(IO_MOTOR module, AxisList axisNo)
	{
		switch (module)
		{
			case DUMMY_MOTOR:
			{
				nMtDummyCnt++;
				break;
			}
			case PANASONIC_SERVO:
			{
				InitPdoSorting();
				nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_MT_TYPE;//mt type
				nMtslaveCnt++;
				m_iMaxDataSize += 4;

				MT[axisNo].m_brakeNo = panasonicChannelCount;
				m_mt[panasonicChannelCount].byteAddress = m_oMaxDataSize;
				m_mt[panasonicChannelCount].dataAddress = &m_oReadWriteData[0] + m_oMaxDataSize;
				panasonicChannelCount++;
				m_oMaxDataSize += 4;
				break;
			}
			case EZI_SERVO_02:
			{
				InitPdoSorting();
				nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_MT_TYPE;//mt type
				nMtslaveCnt++;
				m_iMaxDataSize += 4;
				m_oMaxDataSize += 4;
				break;
			}
			case LS_MECAPION:
			{
				InitPdoSorting();
				nSlaveType[nIoslaveCnt + nMtslaveCnt] = SLAVE_MT_TYPE;//mt type
				nMtslaveCnt++;
				m_iMaxDataSize += 4;
				break;
			}
		}
	}

	void CWmxLib::InitPdoDevice(IO_SEALING module, int channelNo)
	{
		switch (module)
		{
			case SEALING:
			{
				InitPdoSorting();
				nIoslaveCnt++;
				m_si[channelNo].byteAddress = m_iMaxDataSize;
				m_si[channelNo].dataAddress = &m_iReadWriteData[0] + m_iMaxDataSize;
				m_so[channelNo].byteAddress = m_oMaxDataSize;
				m_so[channelNo].dataAddress = &m_oReadWriteData[0] + m_oMaxDataSize;
				SEALLIB.SetAddress(m_iMaxDataSize, m_oMaxDataSize);
				m_iMaxDataSize += SIZE_INPUT;
				m_oMaxDataSize += SIZE_OUTPUT; 
				break;
			}
		}
	}

	void CWmxLib::OutDataClear()
	{
		ZeroMemory(m_oReadWriteData, sizeof(m_oReadWriteData));
	}

	double CWmxLib::MMToPulse(double umValue, double pulseRate)
	{
		double pulseValue = round((round(umValue * 1000.) / pulseRate) * 1000.) / 1000.;
		return pulseValue;
	}

	double CWmxLib::PulseToMM(double pulseValue, double pulseRate)
	{
		double umValue = round(pulseValue * pulseRate) / 1000.;
		return umValue;
	}

	int CWmxLib::CommandJog(MtCommand::SingleParamter command, BOOL isModeChange)
	{
		if (!status.axesStatus[command.no].commandReady)
			return CMD_ERR_READY;

		if (isModeChange)
		{
			AxisCommandMode::T	commandMode;
			coremotion.axisControl->GetAxisCommandMode(command.no, &commandMode);
			if (AxisCommandMode::Position != commandMode)
			{
				if (coremotion.axisControl->SetAxisCommandMode(command.no, AxisCommandMode::Position))
					return CMD_ERR_MODE;
				else
					return CMD_MODE_CHANGE;
			}
		}

		if (0 == command.profile.vel)
			return CMD_ERR_VEL;

		if (0 >= command.profile.acc)
			return CMD_ERR_ACC;

		if (0 >= command.pulseRate)
			return CMD_ERR_PULSERATE;

		Motion::JogCommand jogCommand = Motion::JogCommand();
		jogCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		jogCommand.axis = command.no;
		jogCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		jogCommand.profile.startingVelocity = 0;
		jogCommand.profile.endVelocity = 0;
		jogCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		jogCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		jogCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		jogCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		jogCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		jogCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		if (coremotion.motion->StartJog(&jogCommand))
			return CMD_ERR_MOTION_FAIL;

		return CMD_DONE;
	}

	int CWmxLib::CommandPos(MtCommand::SingleParamter command, BOOL isModeChange)
	{
		if (!status.axesStatus[command.no].commandReady)
			return CMD_ERR_READY;

		if (isModeChange)
		{
			AxisCommandMode::T	commandMode;
			coremotion.axisControl->GetAxisCommandMode(command.no, &commandMode);
			if (AxisCommandMode::Position != commandMode)
			{
				if (coremotion.axisControl->SetAxisCommandMode(command.no, AxisCommandMode::Position))
					return CMD_ERR_MODE;
				else
					return CMD_MODE_CHANGE;
			}
		}

		if (round(command.limitN) != round(command.limitP))
		{
			if (!Between(command.pos, command.limitN, command.limitP))
				return CMD_ERR_SOFTLIMIT;
		}

		if (0 >= command.profile.vel)
			return CMD_ERR_VEL;

		if (0 >= command.profile.acc)
			return CMD_ERR_ACC;

		if (0 >= command.pulseRate)
			return CMD_ERR_PULSERATE;

		Motion::PosCommand posCommand = Motion::PosCommand();
		posCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		posCommand.axis = command.no;
		posCommand.target = MMToPulse(command.pos, command.pulseRate);
		posCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		posCommand.profile.startingVelocity = MMToPulse(command.profile.startingVelocity, command.pulseRate);
		posCommand.profile.endVelocity = MMToPulse(command.profile.endVelocity, command.pulseRate);
		posCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		posCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		posCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		posCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		posCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		posCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		if (coremotion.motion->StartPos(&posCommand))
			return CMD_ERR_MOTION_FAIL;

		return CMD_DONE;
	}

	int CWmxLib::CommandVel(MtCommand::SingleParamter command, BOOL isModeChange)
	{
		if (!status.axesStatus[command.no].commandReady)
			return CMD_ERR_READY;

		if (isModeChange)
		{
			AxisCommandMode::T	commandMode;
			coremotion.axisControl->GetAxisCommandMode(command.no, &commandMode);
			if (AxisCommandMode::Velocity != commandMode)
			{
				if (coremotion.axisControl->SetAxisCommandMode(command.no, AxisCommandMode::Velocity))
					return CMD_ERR_MODE;
				else
					return CMD_MODE_CHANGE;
			}
		}

		if (0 == command.profile.vel)
			return CMD_ERR_VEL;

		if (0 >= command.profile.acc)
			return CMD_ERR_ACC;

		if (0 >= command.pulseRate)
			return CMD_ERR_PULSERATE;

		Velocity::VelCommand velCommand = Velocity::VelCommand();
		velCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		velCommand.axis = command.no;
		velCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		velCommand.profile.startingVelocity = MMToPulse(command.profile.startingVelocity, command.pulseRate);
		velCommand.profile.endVelocity = MMToPulse(command.profile.endVelocity, command.pulseRate);
		velCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		velCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		velCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		velCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		velCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		velCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		if (coremotion.velocity->StartVel(&velCommand))
			return CMD_ERR_MOTION_FAIL;

		return CMD_DONE;
	}

	int CWmxLib::CommandTrq(MtCommand::SingleParamter command, BOOL isModeChange)
	{
		if (!status.axesStatus[command.no].commandReady)
			return CMD_ERR_READY;

		if (isModeChange)
		{
			AxisCommandMode::T	commandMode;
			coremotion.axisControl->GetAxisCommandMode(command.no, &commandMode);
			if (AxisCommandMode::Torque != commandMode)
			{
				if (coremotion.axisControl->SetAxisCommandMode(command.no, AxisCommandMode::Torque))
					return CMD_ERR_MODE;
				else
					return CMD_MODE_CHANGE;
			}
		}

		Torque::TrqCommand trqCommand = Torque::TrqCommand();
		trqCommand.axis = command.no;
		trqCommand.torque = command.trq;

		if (!command.maxVel)
		{
			if (coremotion.torque->StartTrq(&trqCommand))
				return CMD_ERR_MOTION_FAIL;
		}
		else
		{
			if (coremotion.torque->StartTrq(&trqCommand, command.profile.vel))
				return CMD_ERR_MOTION_FAIL;

		}

		return CMD_DONE;
	}

	int CWmxLib::CommandPos(MtCommand::InterpolationParamter command, BOOL isModeChange)
	{
		for (int i = 0; i < command.axisCount; i++)
		{
			AxisList mtno = (AxisList)command.parameter[i].no;
			if (!status.axesStatus[mtno].commandReady)
				return CMD_ERR_READY;
		}

		if (isModeChange)
		{
			BOOL isModeError = FALSE;
			BOOL isModeChanging = FALSE;
			AxisCommandMode::T	commandMode;
			for (int i = 0; i < command.axisCount; i++)
			{
				AxisList mtno = (AxisList)command.parameter[i].no;
				coremotion.axisControl->GetAxisCommandMode(mtno, &commandMode);
				if (AxisCommandMode::Position != commandMode)
				{
					if (coremotion.axisControl->SetAxisCommandMode(mtno, AxisCommandMode::Position))
					{
						ER.Save((ErrName)(ER_MT_MODE + mtno + 1));
						LOG[logMT].Message("[C_POS_LINEAR],[%.2d],[SetAxisCommandMode]", mtno);
						isModeError = TRUE;
					}

					isModeChanging = TRUE;
				}
			}

			if (isModeError)
				return CMD_ERR_MODE;

			if (isModeChanging)
				return CMD_MODE_CHANGE;
		}

		for (int i = 0; i < command.axisCount; i++)
		{
			AxisList mtno = (AxisList)command.parameter[i].no;
			if (round(command.parameter[i].limitN) == round(command.parameter[i].limitP))
				continue;

			if (Between(command.parameter[i].pos, command.parameter[i].limitN, command.parameter[i].limitP))
				continue;

			return CMD_ERR_SOFTLIMIT;
		}

		if (0 >= command.profile.vel)
			return CMD_ERR_VEL;

		if (0 >= command.profile.acc)
			return CMD_ERR_ACC;

		Motion::LinearIntplCommand linearIntplCommand = Motion::LinearIntplCommand();

		linearIntplCommand.axisCount = command.axisCount;
		linearIntplCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		linearIntplCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		linearIntplCommand.profile.startingVelocity = 0;
		linearIntplCommand.profile.endVelocity = 0;
		linearIntplCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		linearIntplCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		linearIntplCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		linearIntplCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		linearIntplCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		linearIntplCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		for (int i = 0; i < command.axisCount; i++)
		{
			AxisList mtno = (AxisList)command.parameter[i].no;

			linearIntplCommand.axis[i] = mtno;
			linearIntplCommand.target[i] = MMToPulse(command.parameter[i].pos, command.parameter[i].pulseRate);
			linearIntplCommand.maxVelocity[i] = MMToPulse(command.parameter[i].maxVel, command.parameter[i].pulseRate);
			linearIntplCommand.maxAcc[i] = MMToPulse(command.parameter[i].maxAcc, command.parameter[i].pulseRate);
			linearIntplCommand.maxDec[i] = MMToPulse(command.parameter[i].maxAcc, command.parameter[i].pulseRate);
			linearIntplCommand.maxJerkAcc[i] = MMToPulse(command.parameter[i].maxJerkAcc, command.parameter[i].pulseRate);
			linearIntplCommand.maxJerkDec[i] = MMToPulse(command.parameter[i].maxJerkAcc, command.parameter[i].pulseRate);
		}

		if (coremotion.motion->StartLinearIntplPos(&linearIntplCommand))
			return CMD_ERR_MOTION_FAIL;

		return CMD_DONE;
	}

	int CWmxLib::CommandSimulateTime(MtCommand::SingleParamter command, double specificPos, double& calculatedMoveTime, double& calculatedRemainTime)
	{
		Motion::SimulatePosCommand simulate = Motion::SimulatePosCommand();
		simulate.posCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		simulate.posCommand.axis = command.no;
		simulate.posCommand.target = MMToPulse(command.pos, command.pulseRate);
		simulate.posCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		simulate.posCommand.profile.startingVelocity = MMToPulse(command.profile.startingVelocity, command.pulseRate);
		simulate.posCommand.profile.endVelocity = MMToPulse(command.profile.endVelocity, command.pulseRate);
		simulate.posCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		simulate.posCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		simulate.posCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		simulate.posCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		simulate.posCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		simulate.posCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		simulate.setStartPos = 1;
		simulate.startPos = MMToPulse(command.startPos, command.pulseRate);

		if (coremotion.motion->SimulateTimeAtPos(&simulate, specificPos, &calculatedMoveTime, &calculatedRemainTime))
			return CMD_ERR_MOTION_FAIL;

		return CMD_DONE;
	}

	int CWmxLib::CommandSimulatePosAndVel(MtCommand::SingleParamter command, ReturnTimeType returnType, double timeMilliseconds, double& calculatedPos, double& calculatedVel)
	{
		Motion::SimulatePosCommand simulate = Motion::SimulatePosCommand();
		simulate.posCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		simulate.posCommand.axis = command.no;
		simulate.posCommand.target = MMToPulse(command.pos, command.pulseRate);
		simulate.posCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		simulate.posCommand.profile.startingVelocity = MMToPulse(command.profile.startingVelocity, command.pulseRate);
		simulate.posCommand.profile.endVelocity = MMToPulse(command.profile.endVelocity, command.pulseRate);
		simulate.posCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		simulate.posCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		simulate.posCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		simulate.posCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		simulate.posCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		simulate.posCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		simulate.setStartPos = 1;
		simulate.startPos = MMToPulse(command.startPos, command.pulseRate);

		if (coremotion.motion->SimulatePosAtTime(&simulate, returnType, timeMilliseconds, &calculatedPos, &calculatedVel))
			return CMD_ERR_MOTION_FAIL;

		calculatedPos = PulseToMM(calculatedPos, command.pulseRate);
		calculatedVel = PulseToMM(calculatedVel, command.pulseRate);
		return CMD_DONE;
	}

	int CWmxLib::CommandSimulateTime(MtCommand::SingleParamter command, double& peakVelocity,
		double& totalTime, double& accelerationTime,
		double& cruiseTime, double& decelerationTime)
	{
		Motion::SimulatePosCommand simulate = Motion::SimulatePosCommand();
		simulate.posCommand.profile.type = ProfileType::JerkLimitedFixedVelocityS;
		simulate.posCommand.axis = command.no;
		simulate.posCommand.target = MMToPulse(command.pos, command.pulseRate);
		simulate.posCommand.profile.velocity = MMToPulse(command.profile.vel, command.pulseRate);
		simulate.posCommand.profile.startingVelocity = MMToPulse(command.profile.startingVelocity, command.pulseRate);
		simulate.posCommand.profile.endVelocity = MMToPulse(command.profile.endVelocity, command.pulseRate);
		simulate.posCommand.profile.acc = MMToPulse(command.profile.acc, command.pulseRate);
		simulate.posCommand.profile.dec = MMToPulse(command.profile.acc, command.pulseRate);
		simulate.posCommand.profile.jerkAcc = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		simulate.posCommand.profile.jerkDec = MMToPulse(command.profile.jerkAcc, command.pulseRate);
		simulate.posCommand.profile.jerkAccRatio = command.profile.jerkRatio;
		simulate.posCommand.profile.jerkDecRatio = command.profile.jerkRatio;

		simulate.setStartPos = 1;
		simulate.startPos = MMToPulse(command.startPos, command.pulseRate);

		if (coremotion.motion->SimulatePos(&simulate, &peakVelocity, &totalTime, &accelerationTime, &cruiseTime, &decelerationTime))
			return CMD_ERR_MOTION_FAIL;

		peakVelocity = PulseToMM(peakVelocity, command.pulseRate);
		return CMD_DONE;
	}

	BOOL CWmxLib::AOn(enDI ioNo, DWORD dwDelayTime)
	{
		BOOL isOn = (TRUE);
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (*(UINT16*)m_di[chIdx].dataAddress & g_bitMask16[bit16])
		{
			if (isOn)
			{
				if (!dwDelayTime || tiOn[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		else
		{
			if (!isOn)
			{
				if (!dwDelayTime || tiOff[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		return (FALSE);
	}

	BOOL CWmxLib::AOff(enDI ioNo, DWORD dwDelayTime)
	{
		BOOL isOn = (FALSE);
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (*(UINT16*)m_di[chIdx].dataAddress & g_bitMask16[bit16])
		{
			if (isOn)
			{
				if (!dwDelayTime || tiOn[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		else
		{
			if (!isOn)
			{
				if (!dwDelayTime || tiOff[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		return (FALSE);
	}

	void CWmxLib::Set(enDI ioNo, BOOL isOn)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (isOn)
			*(UINT16*)m_di[chIdx].dataAddress |= g_bitMask16[bit16];
		else
			*(UINT16*)m_di[chIdx].dataAddress &= ~g_bitMask16[bit16];
	}

	BOOL CWmxLib::IsOn(enDO ioNo, DWORD dwDelayTime)
	{
		BOOL isOn = (TRUE);
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (*(UINT16*)m_do[chIdx].dataAddress & g_bitMask16[bit16])
		{
			if (isOn)
			{
				if (!dwDelayTime || toOn[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		else
		{
			if (!isOn)
			{
				if (!dwDelayTime || toOff[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		return (FALSE);
	}

	BOOL CWmxLib::IsOff(enDO ioNo, DWORD dwDelayTime)
	{
		BOOL isOn = (FALSE);
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (*(UINT16*)m_do[chIdx].dataAddress & g_bitMask16[bit16])
		{
			if (isOn)
			{
				if (!dwDelayTime || toOn[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		else
		{
			if (!isOn)
			{
				if (!dwDelayTime || toOff[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		return (FALSE);
	}

	BOOL CWmxLib::IsReadOnly(enDO ioNo)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		UINT16& ch16 = *(UINT16*)(&m_oReadOnlyData[0] + m_do[chIdx].byteAddress);
		if (ch16 & g_bitMask16[bit16])
			return (TRUE);

		return (FALSE);
	}

	BOOL CWmxLib::IsBrakeOn(int no)
	{
		if (NOT_DEFINED == no)
			return (FALSE);
	
		if (*(UINT32*)m_mt[no].dataAddress & (0x00010000))
		{
			return (TRUE);
		}
		return (FALSE);
	}

	void CWmxLib::Set(enDO ioNo, BOOL isOn)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (isOn)
			*(UINT16*)m_do[chIdx].dataAddress |= g_bitMask16[bit16];
		else
			*(UINT16*)m_do[chIdx].dataAddress &= ~g_bitMask16[bit16];
	}

	void CWmxLib::SetReadOnly(enDO ioNo, BOOL isOn)
	{
		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_CH - 1) || !Between(bit16, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		UINT16& ch16 = *(UINT16*)(&m_oReadOnlyData[0] + m_do[chIdx].byteAddress);
		if (isOn)
			ch16 |= g_bitMask16[bit16];
		else
			ch16 &= ~g_bitMask16[bit16];
	}

	void CWmxLib::SetBrake(int no, BOOL isOn)
	{
		if (NOT_DEFINED == no)
			return;

		if (isOn)
			*(UINT32*)m_mt[no].dataAddress |= (0x00010000);
		else
			*(UINT32*)m_mt[no].dataAddress &= ~(0x00010000);
	}

	BOOL CWmxLib::IsUserMemoryOn(enUSERMEM userMemNo, DWORD dwDelayTime)
	{
		BOOL isOn = (TRUE);
		int chIdx = (userMemNo / 100);
		int bit16 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);
		if (!Between(chIdx, 0, MAX_USERMEM_CH - 1) || !Between(bit16, 0, MAX_USERMEM_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);
	
		if (m_userMemReadWriteData[chIdx] & g_bitMask08[bit16])
		{
			if (isOn)
			{
				if (!dwDelayTime || tusermemOn[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		else
		{
			if (!isOn)
			{
				if (!dwDelayTime || tusermemOff[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		return (FALSE);
	}

	BOOL CWmxLib::IsUserMemoryOff(enUSERMEM userMemNo, DWORD dwDelayTime)
	{
		BOOL isOn = (FALSE);
		int chIdx = (userMemNo / 100);
		int bit16 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);
		if (!Between(chIdx, 0, MAX_USERMEM_CH - 1) || !Between(bit16, 0, MAX_USERMEM_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (m_userMemReadWriteData[chIdx] & g_bitMask08[bit16])
		{
			if (isOn)
			{
				if (!dwDelayTime || tusermemOn[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		else
		{
			if (!isOn)
			{
				if (!dwDelayTime || tusermemOff[chIdx][bit16].TmOver(dwDelayTime))
					return (TRUE);
			}
		}
		return (FALSE);
	}

	void CWmxLib::UserMemorySet(enUSERMEM userMemNo, BOOL isOn)
	{
		int chIdx = (userMemNo / 100);
		int bit16 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);
		if (!Between(chIdx, 0, MAX_USERMEM_CH - 1) || !Between(bit16, 0, MAX_USERMEM_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (isOn)
			m_userMemReadWriteData[chIdx] |= g_bitMask08[bit16];
		else
			m_userMemReadWriteData[chIdx] &= ~g_bitMask08[bit16];
	}

	BOOL CWmxLib::IsUserMemoryReadOnly(enUSERMEM userMemNo)
	{
		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_USERMEM_CH - 1) || !Between(bit08, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (m_userMemReadOnlyData[chIdx] & g_bitMask08[bit08])
			return (TRUE);

		return (FALSE);
	}

	void CWmxLib::SetUserMemoryReadOnly(enUSERMEM userMemNo, BOOL isOn)
	{
		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_CONTACT);
		if (!Between(chIdx, 0, MAX_USERMEM_CH - 1) || !Between(bit08, 0, MAX_CONTACT - 1))
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);

		if (isOn)
			m_userMemReadOnlyData[chIdx] |= g_bitMask08[bit08];
		else
			m_userMemReadOnlyData[chIdx] &= ~g_bitMask08[bit08];
	}
}