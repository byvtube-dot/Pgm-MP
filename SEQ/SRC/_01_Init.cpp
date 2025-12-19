#include "..\Includes.h"

void InitSequence(void)
{
	BOOL isInit = (TRUE); //## 각 초기화 단계가 성공했는지 확인
	if (isInit)	isInit &= InitLOG(); //## 로그 시스템 초기화
	if (isInit)	isInit &= InitINI(); //## 수정 버전 정보를 확인
	if (isInit)	isInit &= InitIO(); //## WMX(장비 제어 라이브러리)를 오픈, EtherCAT 통신 기반 IO 모듈들을 등록(DI,DO,AI,AO)
	if (isInit)	isInit &= InitMOTOR();
	if (isInit)	isInit &= InitSEAL();
	if (isInit)	isInit &= InitINTERPOLATION();
	if (isInit)	isInit &= InitVISION();
	if (isInit)	isInit &= InitVACUUM();
	if (isInit)	isInit &= InitCYLINDER();
	if (isInit)	isInit &= InitCONVEYOR();
	if (isInit)	isInit &= InitLASER();
	if (isInit)	isInit &= InitROBOT();
	if (isInit)	isInit &= InitETC();
	if (isInit)
	{
		LOG[logSYS].Message("Initialized PGM");
		NV.ndm[startSeq] = 0;
	}
	if (!isInit)
		throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
}

BOOL InitLOG(void)
{
	if (!SMESSAGE.Open(&CSysTm::GetTm()))
		return (FALSE);

	COM::SLog::UniqueNumber() = 1;
	for (int i = 0; i < slogMAX; i++)
		SLOG[SLogList(i)].m_no = i;

	LOG[logMT].Init(L"LOG000", L"logMT", L"LOG", TRUE, FALSE, 1000);
	LOG[logIO].Init(L"LOG001", L"logIO", L"LOG", TRUE, FALSE, 1000);
	LOG[logDM].Init(L"LOG002", L"logDM", L"LOG", TRUE, FALSE, 1000);
	LOG[logMC].Init(L"LOG003", L"logMC", L"LOG", TRUE, FALSE, 30);
	LOG[logSYS].Init(L"LOG004", L"logSYS", L"LOG", TRUE, TRUE, 30);
	LOG[logSEQ].Init(L"LOG005", L"logSEQ", L"LOG", TRUE, TRUE, 1000);
	LOG[logGEM].Init(L"LOG006", L"logGEM", L"LOG", TRUE, FALSE, 1000);
	LOG[logMMI].Init(L"LOG007", L"logMMI", L"LOG", TRUE, FALSE, 1000);
	LOG[logLAS].Init(L"LOG008", L"logLAS", L"LOG", TRUE, FALSE, 1000);
	LOG[logLASERIO].Init(L"LOG009", L"logLASERIO", L"LOG", TRUE, FALSE, 100);
	LOG[logROBOT].Init(L"LOG010", L"logROBOT", L"LOG", TRUE, FALSE, 100);
	return (TRUE);
}

BOOL InitINI(void)
{
	WCHAR strPath[MAX_PATH] = { 0 };
	GetCurDirectory(strPath);
	wcscat(strPath, L"\\Revision.ini");
	FILE* fp = _wfopen(strPath, L"r");
	if (NULL != fp)
	{
		fclose(fp);
		WCHAR strLogCount[1024] = { 0 };
		WCHAR strCommitDate[1024] = { 0 };
		WCHAR strBuildDate[1024] = { 0 };
		WCHAR strModified[1024] = { 0 };
		CIni iniRevision;
		iniRevision.SetPathName(strPath);
		iniRevision.GetString(L"REVISION", L"LOG_COUNT", strLogCount, 512);
		iniRevision.GetString(L"REVISION", L"COMMIT_DATE", strCommitDate, 512);
		iniRevision.GetString(L"REVISION", L"BUILD_DATE", strBuildDate, 512);
		iniRevision.GetString(L"REVISION", L"MODIFIED", strModified, 512);
		WCHAR* pEnd = NULL;
		LOG[logSYS].Message("Log Count:%s", GetAscCode(strLogCount));
		LOG[logSYS].Message("Commit Date:%s", GetAscCode(strCommitDate));
		LOG[logSYS].Message("Build Date:%s", GetAscCode(strBuildDate));
		if (!!wcstod(strModified, &pEnd))
			LOG[logSYS].Message("Modified Program");
		else
			LOG[logSYS].Message("Original Program");
	}

	Sleep(100);
	return (TRUE);
}

BOOL InitIO(void)
{
	if (!WMX.Open())
		return (FALSE);

	WMX.InitPdoDevice(UR20_HUB);
	WMX.InitPdoDevice(UR20_FBC_EC);
	WMX.InitPdoDevice(UR20_16DI_N, 0);			//	INPUT : 0000 ~ 0015
	WMX.InitPdoDevice(UR20_16DI_N, 1);			//	INPUT : 0100 ~ 0115
	WMX.InitPdoDevice(UR20_16DI_N, 2);			//	INPUT : 0200 ~ 0215
	WMX.InitPdoDevice(UR20_16DI_N, 3);			//	INPUT : 0300 ~ 0315
	WMX.InitPdoDevice(UR20_16DI_N, 4);			//	INPUT : 0400 ~ 0415
	WMX.InitPdoDevice(UR20_16DI_N, 5);			//	INPUT : 0500 ~ 0515
	WMX.InitPdoDevice(UR20_16DI_N, 6);			//	INPUT : 0600 ~ 0615
	WMX.InitPdoDevice(UR20_16DI_N, 7);			//	INPUT : 0700 ~ 0715
	WMX.InitPdoDevice(UR20_16DI_N, 8);			//	INPUT : 0800 ~ 0815
	WMX.InitPdoDevice(UR20_16DI_N, 9);			//	INPUT : 0900 ~ 0915
	WMX.InitPdoDevice(UR20_16DI_N, 10);			//	INPUT : 1000 ~ 1015
	WMX.InitPdoDevice(UR20_16DI_N, 11);			//	INPUT : 1100 ~ 1115
	WMX.InitPdoDevice(UR20_16DI_N, 12);			//	INPUT : 1200 ~ 1215
	WMX.InitPdoDevice(UR20_16DI_N, 13);			//	INPUT : 1300 ~ 1315
	WMX.InitPdoDevice(UR20_16DI_N, 14);			//	INPUT : 1400 ~ 1415
	WMX.InitPdoDevice(UR20_4AI_UI_16, 0);		//	AI : 000 ~ 003
	WMX.InitPdoDevice(UR20_4AI_UI_16, 1);		//	AI : 100 ~ 103
	WMX.InitPdoDevice(UR20_4AI_UI_16, 2);		//	AI : 200 ~ 203
	WMX.InitPdoDevice(UR20_4AI_UI_16, 3);		//	AI : 300 ~ 303
	WMX.InitPdoDevice(UR20_4AI_UI_16, 4);		//	AI : 400 ~ 403

	WMX.InitPdoDevice(UR20_FBC_EC);
	WMX.InitPdoDevice(UR20_16DO_N, 0);			//	OUTPUT : 0000 ~ 0015
	WMX.InitPdoDevice(UR20_16DO_N, 1);			//	OUTPUT : 0100 ~ 0115
	WMX.InitPdoDevice(UR20_16DO_N, 2);			//	OUTPUT : 0200 ~ 0215
	WMX.InitPdoDevice(UR20_16DO_N, 3);			//	OUTPUT : 0300 ~ 0315
	WMX.InitPdoDevice(UR20_16DO_N, 4);			//	OUTPUT : 0400 ~ 0415
	WMX.InitPdoDevice(UR20_16DO_N, 5);			//	OUTPUT : 0500 ~ 0515
	WMX.InitPdoDevice(UR20_16DO_N, 6);			//	OUTPUT : 0600 ~ 0615
	WMX.InitPdoDevice(UR20_16DO_N, 7);			//	OUTPUT : 0700 ~ 0715
	WMX.InitPdoDevice(UR20_16DO_N, 8);			//	OUTPUT : 0800 ~ 0815
	WMX.InitPdoDevice(UR20_16DO_N, 9);			//	OUTPUT : 0900 ~ 0915
	WMX.InitPdoDevice(UR20_16DO_N, 10);			//	OUTPUT : 1000 ~ 1015
	WMX.InitPdoDevice(UR20_16DO_N, 11);			//	OUTPUT : 1100 ~ 1115
	WMX.InitPdoDevice(UR20_4AO_UI_16, 0);		//	AO : 000 ~ 003

	WMX.InitPdoDevice(PANASONIC_SERVO, mtInPkX);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtInPkZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtTrayShuttleY);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtTrayPusherX);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbOpenZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamSendElevZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamFlipT);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamRecvElevZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamPartialElevZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamPkX);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamPkY);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamPkY2);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtFoamPkZ);

	WMX.InitPdoDevice(SEALING, 0);				// SetAddress

	// Right
	WMX.InitPdoDevice(UR20_FBC_EC);
	WMX.InitPdoDevice(UR20_16DI_N, 15);			//	INPUT : 1500 ~ 1515
	WMX.InitPdoDevice(UR20_16DI_N, 16);			//	INPUT : 1600 ~ 1615
	WMX.InitPdoDevice(UR20_16DI_N, 17);			//	INPUT : 1700 ~ 1715
	WMX.InitPdoDevice(UR20_16DI_N, 18);			//	INPUT : 1800 ~ 1815
	WMX.InitPdoDevice(UR20_16DI_N, 19);			//	INPUT : 1900 ~ 1915
	WMX.InitPdoDevice(UR20_16DI_N, 20);			//	INPUT : 2000 ~ 2015
	WMX.InitPdoDevice(UR20_16DI_N, 21);			//	INPUT : 2100 ~ 2115
	WMX.InitPdoDevice(UR20_16DI_N, 22);			//	INPUT : 2200 ~ 2215
	WMX.InitPdoDevice(UR20_16DI_N, 23);			//	INPUT : 2300 ~ 2315
	WMX.InitPdoDevice(UR20_16DI_N, 24);			//	INPUT : 2400 ~ 2415
	WMX.InitPdoDevice(UR20_16DI_N, 25);			//	INPUT : 2500 ~ 2515
	WMX.InitPdoDevice(UR20_16DI_N, 26);			//	INPUT : 2600 ~ 2615
	WMX.InitPdoDevice(UR20_16DI_N, 27);			//	INPUT : 2700 ~ 2715
	WMX.InitPdoDevice(UR20_16DI_N, 28);			//	INPUT : 2800 ~ 2815
	WMX.InitPdoDevice(UR20_16DI_N, 29);			//	INPUT : 2800 ~ 2815

	WMX.InitPdoDevice(UR20_FBC_EC);
	WMX.InitPdoDevice(UR20_16DI_N, 30);			//	INPUT : 2800 ~ 2815
	WMX.InitPdoDevice(UR20_16DI_N, 31);			//	INPUT : 2800 ~ 2815
	WMX.InitPdoDevice(UR20_16DI_N, 32);			//	INPUT : 2800 ~ 2815
	WMX.InitPdoDevice(UR20_16DI_N, 33);			//	INPUT : 2800 ~ 2815
	WMX.InitPdoDevice(UR20_4AI_UI_16, 5);		//	AI : 500 ~ 503
	WMX.InitPdoDevice(UR20_4AI_UI_16, 6);		//	AI : 600 ~ 603
	WMX.InitPdoDevice(UR20_4AI_UI_16, 7);		//	AI : 700 ~ 703

	WMX.InitPdoDevice(UR20_FBC_EC);
	WMX.InitPdoDevice(UR20_16DO_N, 12);			//	OUTPUT : 1200 ~ 1215
	WMX.InitPdoDevice(UR20_16DO_N, 13);			//	OUTPUT : 1100 ~ 1115
	WMX.InitPdoDevice(UR20_16DO_N, 14);			//	OUTPUT : 1100 ~ 1115
	WMX.InitPdoDevice(UR20_16DO_N, 15);			//	OUTPUT : 1500 ~ 1515
	WMX.InitPdoDevice(UR20_16DO_N, 16);			//	OUTPUT : 1600 ~ 1615
	WMX.InitPdoDevice(UR20_16DO_N, 17);			//	OUTPUT : 1700 ~ 1715
	WMX.InitPdoDevice(UR20_16DO_N, 18);			//	OUTPUT : 1800 ~ 1815
	WMX.InitPdoDevice(UR20_16DO_N, 19);			//	OUTPUT : 1900 ~ 1915
	WMX.InitPdoDevice(UR20_16DO_N, 20);			//	OUTPUT : 2000 ~ 2015
	WMX.InitPdoDevice(UR20_16DO_N, 21);			//	OUTPUT : 2100 ~ 2115
	WMX.InitPdoDevice(UR20_16DO_N, 22);			//	OUTPUT : 2200 ~ 2215
	WMX.InitPdoDevice(UR20_16DO_N, 23);			//	OUTPUT : 2300 ~ 2315
	WMX.InitPdoDevice(UR20_16DO_N, 24);			//	OUTPUT : 2400 ~ 2415
	WMX.InitPdoDevice(UR20_16DO_N, 25);			//	OUTPUT : 2500 ~ 2515
	WMX.InitPdoDevice(UR20_16DO_N, 26);			//	OUTPUT : 2600 ~ 2615

	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbShuttleY);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbShuttleConv);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbQcViY);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbFoldPkX);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbFoldPkZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtLabelLtShuttleY);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtLabelRtShuttleY);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtLabelLaserX);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtLabelLtFeed);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtLabelRtFeed);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbLtElevZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbRtElevZ);
	WMX.InitPdoDevice(PANASONIC_SERVO, mtMbbQcX);

	//UINT16 MAX_DI_CH = (enDI::diLAST != 0) ? ((enDI::diLAST / 100) + 1) : 0;
	//for (int chIdx = 0; chIdx < MAX_DI_CH; chIdx++)
	//{
	//	for (int bit16 = 0; bit16 < 16; bit16++)
	//	{
	//		int byteAddress = WMX.m_di[chIdx].byteAddress;
	//		int bitAddress = bit16;
	//		if (8 <= bitAddress)
	//		{
	//			byteAddress++;
	//			bitAddress = bit16 - 8;
	//		}
	//	}
	//}

	return (TRUE);
}

BOOL InitMOTOR(void)
{
	Motor::CreateInitParameter();

	// 0 Axis
	MT[mtInPkX].CanMove = InPkXCanMove;
	MT[mtInPkX].m_maxVel = _20MM_PITCH_;
	MT[mtInPkX].m_maxAcc = _0_5G_;
	MT[mtInPkX].m_maxJerkAcc = _5G_;
	MT[mtInPkX].m_inposBand = 0.005;
	MT[mtInPkX].m_sLogDir = "X";

	// 1 Axis
	MT[mtInPkZ].CanMove = InPkZCanMove;
	MT[mtInPkZ].m_maxVel = _10MM_PITCH_;
	MT[mtInPkZ].m_maxAcc = _0_5G_;
	MT[mtInPkZ].m_maxJerkAcc = _5G_;
	MT[mtInPkZ].m_inposBand = 0.005;
	MT[mtInPkZ].m_sLogDir = "Z";

	// 2 Axis
	MT[mtTrayShuttleY].CanMove = TrayShuttleYCanMove;
	MT[mtTrayShuttleY].m_maxVel = _20MM_PITCH_;
	MT[mtTrayShuttleY].m_maxAcc = _0_5G_;
	MT[mtTrayShuttleY].m_maxJerkAcc = _5G_;
	MT[mtTrayShuttleY].m_inposBand = 0.005;
	MT[mtTrayShuttleY].m_sLogDir = "Y";

	// 3 Axis
	MT[mtTrayPusherX].CanMove = TrayPushXCanMove;
	MT[mtTrayPusherX].m_maxVel = _20MM_PITCH_;	// 30002 * 50;
	MT[mtTrayPusherX].m_maxAcc = _0_5G_;
	MT[mtTrayPusherX].m_maxJerkAcc = _5G_;
	MT[mtTrayPusherX].m_inposBand = 0.005;
	MT[mtTrayPusherX].m_sLogDir = "X";

	// 4 Axis
	MT[mtMbbOpenZ].CanMove = MbbOpenZCanMove;
	MT[mtMbbOpenZ].m_maxVel = _5MM_PITCH_;
	MT[mtMbbOpenZ].m_maxAcc = _0_5G_;
	MT[mtMbbOpenZ].m_maxJerkAcc = _5G_;
	MT[mtMbbOpenZ].m_inposBand = 0.005;
	MT[mtMbbOpenZ].m_sLogDir = "Z";

	// 5 Axis
	MT[mtFoamSendElevZ].CanMove = FoamSendElevZCanMove;
	MT[mtFoamSendElevZ].m_maxVel = _10MM_PITCH_;
	MT[mtFoamSendElevZ].m_maxAcc = _0_5G_;
	MT[mtFoamSendElevZ].m_maxJerkAcc = _5G_;
	MT[mtFoamSendElevZ].m_inposBand = 0.005;
	MT[mtFoamSendElevZ].m_sLogDir = "Z";

	// 6 Axis
	MT[mtFoamFlipT].CanMove = FoamTurnTCanMove;
	MT[mtFoamFlipT].m_maxVel = _20MM_PITCH_;	//12999 * 50;
	MT[mtFoamFlipT].m_maxAcc = _0_5G_;
	MT[mtFoamFlipT].m_maxJerkAcc = _5G_;
	MT[mtFoamFlipT].m_inposBand = 0.005;
	MT[mtFoamFlipT].m_sLogDir = "T";

	// 7 Axis
	MT[mtFoamRecvElevZ].CanMove = FoamRecvElevZCanMove;
	MT[mtFoamRecvElevZ].m_maxVel = _10MM_PITCH_;
	MT[mtFoamRecvElevZ].m_maxAcc = _0_5G_;
	MT[mtFoamRecvElevZ].m_maxJerkAcc = _5G_;
	MT[mtFoamRecvElevZ].m_inposBand = 0.005;
	MT[mtFoamRecvElevZ].m_sLogDir = "Z";

	// 8 Axis
	MT[mtFoamPartialElevZ].CanMove = FoamPartialElevCanMove;
	MT[mtFoamPartialElevZ].m_maxVel = _10MM_PITCH_;
	MT[mtFoamPartialElevZ].m_maxAcc = _0_5G_;
	MT[mtFoamPartialElevZ].m_maxJerkAcc = _5G_;
	MT[mtFoamPartialElevZ].m_inposBand = 0.005;
	MT[mtFoamPartialElevZ].m_sLogDir = "Z";

	// 9 Axis
	MT[mtFoamPkX].CanMove = FoamPkXCanMove;
	MT[mtFoamPkX].m_maxVel = _20MM_PITCH_;	// 104992 * 50;
	MT[mtFoamPkX].m_maxAcc = _0_5G_;
	MT[mtFoamPkX].m_maxJerkAcc = _5G_;
	MT[mtFoamPkX].m_inposBand = 0.005;
	MT[mtFoamPkX].m_sLogDir = "X";

	// 10 Axis
	MT[mtFoamPkY].CanMove = FoamPkYCanMove;
	MT[mtFoamPkY].m_maxVel = _20MM_PITCH_;
	MT[mtFoamPkY].m_maxAcc = _0_5G_;
	MT[mtFoamPkY].m_maxJerkAcc = _5G_;
	MT[mtFoamPkY].m_inposBand = 0.005;
	MT[mtFoamPkY].m_sLogDir = "Y";

	// 11 Axis
	MT[mtFoamPkY2].CanMove = FoamPkYCanMove;
	MT[mtFoamPkY2].m_maxVel = _20MM_PITCH_;
	MT[mtFoamPkY2].m_maxAcc = _0_5G_;
	MT[mtFoamPkY2].m_maxJerkAcc = _5G_;
	MT[mtFoamPkY2].m_inposBand = 0.005;
	MT[mtFoamPkY2].m_sLogDir = "Y";

	// 12 Axis
	MT[mtFoamPkZ].CanMove = FoamPkZCanMove;
	MT[mtFoamPkZ].m_maxVel = _20MM_PITCH_;
	MT[mtFoamPkZ].m_maxAcc = _0_5G_;
	MT[mtFoamPkZ].m_maxJerkAcc = _5G_;
	MT[mtFoamPkZ].m_inposBand = 0.005;
	MT[mtFoamPkZ].m_sLogDir = "Z";

	// RIGHT PART
	// 13 Axis
	MT[mtMbbShuttleY].CanMove = MbbShuttleYCanMove;
	MT[mtMbbShuttleY].m_maxVel = _20MM_PITCH_;
	MT[mtMbbShuttleY].m_maxAcc = _0_5G_;
	MT[mtMbbShuttleY].m_maxJerkAcc = _5G_;
	MT[mtMbbShuttleY].m_inposBand = 0.005;
	MT[mtMbbShuttleY].m_sLogDir = "Y";

	// 14 Axis
	MT[mtMbbShuttleConv].CanMove = MbbShuttleConvCanMove;
	MT[mtMbbShuttleConv].m_maxVel = _20MM_PITCH_;	//14451 * 50;;
	MT[mtMbbShuttleConv].m_maxAcc = _0_5G_;
	MT[mtMbbShuttleConv].m_maxJerkAcc = _5G_;
	MT[mtMbbShuttleConv].m_inposBand = 0.005;
	MT[mtMbbShuttleConv].m_sLogDir = "T";

	// 15 Axis
	MT[mtMbbQcViY].CanMove = MbbQcYCanMove;
	MT[mtMbbQcViY].m_maxVel = _20MM_PITCH_;
	MT[mtMbbQcViY].m_maxAcc = _0_5G_;
	MT[mtMbbQcViY].m_maxJerkAcc = _5G_;
	MT[mtMbbQcViY].m_inposBand = 0.005;
	MT[mtMbbQcViY].m_sLogDir = "Y";

	// 16 Axis
	MT[mtMbbFoldPkX].CanMove = MbbFoldXCanMove;
	MT[mtMbbFoldPkX].m_maxVel = _20MM_PITCH_;
	MT[mtMbbFoldPkX].m_maxAcc = _0_5G_;
	MT[mtMbbFoldPkX].m_maxJerkAcc = _5G_;
	MT[mtMbbFoldPkX].m_inposBand = 0.005;
	MT[mtMbbFoldPkX].m_sLogDir = "X";

	// 17 Axis
	MT[mtMbbFoldPkZ].CanMove = MbbFoldZCanMove;
	MT[mtMbbFoldPkZ].m_maxVel = _10MM_PITCH_;
	MT[mtMbbFoldPkZ].m_maxAcc = _0_5G_;
	MT[mtMbbFoldPkZ].m_maxJerkAcc = _5G_;
	MT[mtMbbFoldPkZ].m_inposBand = 0.005;
	MT[mtMbbFoldPkZ].m_sLogDir = "Z";

	// 18 Axis
	MT[mtLabelLtShuttleY].CanMove = LabelLtShuttleYCanMove;
	MT[mtLabelLtShuttleY].m_maxVel = _20MM_PITCH_;
	MT[mtLabelLtShuttleY].m_maxAcc = _0_5G_;
	MT[mtLabelLtShuttleY].m_maxJerkAcc = _5G_;
	MT[mtLabelLtShuttleY].m_inposBand = 0.005;
	MT[mtLabelLtShuttleY].m_sLogDir = "Y";

	// 19 Axis
	MT[mtLabelRtShuttleY].CanMove = LabelRtShuttleYCanMove;
	MT[mtLabelRtShuttleY].m_maxVel = _20MM_PITCH_;
	MT[mtLabelRtShuttleY].m_maxAcc = _0_5G_;
	MT[mtLabelRtShuttleY].m_maxJerkAcc = _5G_;
	MT[mtLabelRtShuttleY].m_inposBand = 0.005;
	MT[mtLabelRtShuttleY].m_sLogDir = "Y";

	// 20 Axis
	MT[mtLabelLaserX].CanMove = LabelLaserXCanMove;
	MT[mtLabelLaserX].m_maxVel = _20MM_PITCH_;
	MT[mtLabelLaserX].m_maxAcc = _0_5G_;
	MT[mtLabelLaserX].m_maxJerkAcc = _5G_;
	MT[mtLabelLaserX].m_inposBand = 0.005;
	MT[mtLabelLaserX].m_sLogDir = "X";

	// 21 Axis
	MT[mtLabelLtFeed].CanMove = LabelLtFeedCanMove;
	MT[mtLabelLtFeed].m_maxVel = _20MM_PITCH_;	// 94247 * 50;
	MT[mtLabelLtFeed].m_maxAcc = _0_5G_;
	MT[mtLabelLtFeed].m_maxJerkAcc = _5G_;
	MT[mtLabelLtFeed].m_inposBand = 0.005;
	MT[mtLabelLtFeed].m_sLogDir = "T";

	// 22 Axis
	MT[mtLabelRtFeed].CanMove = LabelRtFeedCanMove;
	MT[mtLabelRtFeed].m_maxVel = _20MM_PITCH_;	// 94247 * 50;
	MT[mtLabelRtFeed].m_maxAcc = _0_5G_;
	MT[mtLabelRtFeed].m_maxJerkAcc = _5G_;
	MT[mtLabelRtFeed].m_inposBand = 0.005;
	MT[mtLabelRtFeed].m_sLogDir = "T";

	// 23 Axis
	MT[mtMbbLtElevZ].CanMove = MbbLtElevZCanMove;
	MT[mtMbbLtElevZ].m_maxVel = _5MM_PITCH_;
	MT[mtMbbLtElevZ].m_maxAcc = _0_5G_;
	MT[mtMbbLtElevZ].m_maxJerkAcc = _5G_;
	MT[mtMbbLtElevZ].m_inposBand = 0.005;
	MT[mtMbbLtElevZ].m_sLogDir = "Z";

	// 24 Axis
	MT[mtMbbRtElevZ].CanMove = MbbRtElevZCanMove;
	MT[mtMbbRtElevZ].m_maxVel = _5MM_PITCH_;
	MT[mtMbbRtElevZ].m_maxAcc = _0_5G_;
	MT[mtMbbRtElevZ].m_maxJerkAcc = _5G_;
	MT[mtMbbRtElevZ].m_inposBand = 0.005;
	MT[mtMbbRtElevZ].m_sLogDir = "Z";

	// 25 Axis
	MT[mtMbbQcX].CanMove = MbbMbbQcXCanMove;
	MT[mtMbbQcX].m_maxVel = _5MM_PITCH_;
	MT[mtMbbQcX].m_maxAcc = _0_5G_;
	MT[mtMbbQcX].m_maxJerkAcc = _5G_;
	MT[mtMbbQcX].m_inposBand = 0.005;
	MT[mtMbbQcX].m_sLogDir = "X";


	GT[gtFoamPkY].m_master = mtFoamPkY;
	GT[gtFoamPkY].m_slave = mtFoamPkY2;
	GT[gtFoamPkY].m_groupId = gtFoamPkY;
	GT[gtFoamPkY].SetGroup();
	GT[gtFoamPkY].Set();

	return (TRUE);
}

BOOL InitSEAL(void)
{
	SEALLIB.Init();
	return (TRUE);
}

BOOL InitINTERPOLATION(void)
{
	InterpolationMotor::CreateInitParameter();

	return (TRUE);
}

BOOL InitVISION(void)
{
	Vision::CreateInitParameter();

	VI[VI_IN_PK_BAND].SetIO(iViAuto, iViInPkBusy, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED,
		iViInPkMode1, iViInPkMode2, iViInPkMode3, iViInPkMode4, (enDI)NOT_DEFINED,
		oViInPkTrig, oViInPkReset, (enDO)NOT_DEFINED,
		oViInPkMode1, oViInPkMode2, oViInPkMode3, oViInPkMode4, (enDO)NOT_DEFINED);

	VI[VI_IN_PK].SetIO(iViAuto, iViLtCobotBusy, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED,
		iViLtCobotMode1, iViLtCobotMode2, iViLtCobotMode3, iViLtCobotMode4, (enDI)NOT_DEFINED,
		oViLtCobotTrig, oViLtCobotReset, (enDO)NOT_DEFINED,
		oViLtCobotMode1, oViLtCobotMode2, oViLtCobotMode3, oViLtCobotMode4, (enDO)NOT_DEFINED);

	VI[VI_RT_COBOT].SetIO(iViAuto, iViRtCobotBusy, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED,
		iViRtCobotMode1, iViRtCobotMode2, iViRtCobotMode3, iViRtCobotMode4, (enDI)NOT_DEFINED,
		oViRtCobotTrig, oViRtCobotReset, (enDO)NOT_DEFINED,
		oViRtCobotMode1, oViRtCobotMode2, oViRtCobotMode3, oViRtCobotMode4, (enDO)NOT_DEFINED);

	VI[VI_LABEL_PRS].SetIO(iViAuto, iViLabelFeederPrsBusy, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED,
		iViLabelFeederMode1, iViLabelFeederMode2, iViLabelFeederMode3, iViLabelFeederMode4, (enDI)NOT_DEFINED,
		oViLabelFeederTrig, oViLabelFeederReset, (enDO)NOT_DEFINED,
		oViLabelFeederMode1, oViLabelFeederMode2, oViLabelFeederMode3, oViLabelFeederMode4, (enDO)NOT_DEFINED);

	VI[VI_LABEL_QC].SetIO(iViAuto, iViLabelQcBusy, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED,
		iViLabelQcMode1, iViLabelQcMode2, iViLabelQcMode3, iViLabelQcMode4, (enDI)NOT_DEFINED,
		oViLabelQcTrig, oViLabelQcReset, (enDO)NOT_DEFINED,
		oViLabelQcMode1, oViLabelQcMode2, oViLabelQcMode3, oViLabelQcMode4, (enDO)NOT_DEFINED);

	VI[VI_MBB_QC].SetIO(iViAuto, iViMbbQcBusy, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED, (enDI)NOT_DEFINED,
		iViMbbQcMode1, iViMbbQcMode2, iViMbbQcMode3, iViMbbQcMode4, (enDI)NOT_DEFINED,
		oViMbbQcTrig, oViMbbQcReset, (enDO)NOT_DEFINED,
		oViMbbQcMode1, oViMbbQcMode2, oViMbbQcMode3, oViMbbQcMode4, (enDO)NOT_DEFINED);

	return (TRUE);
}

BOOL InitVACUUM(void)
{
	Vacuum::CreateInitParameter();

	VC[vcMainAir1].SetIO(aiMainAir1, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcMainAir1].SetParam(MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAX_PRESSURE, MPS_P35R_NCA_MIN_PRESSURE, WEIDMULLER_AD_S7);
	VC[vcMainAir1].SetErr(3000, ER_VAC_ALARM_MAIN_AIR1, HW::Vacuum::vacuumFREE, TRUE);
	VC[vcMainAir1].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcMainAir1].m_fdcData.use = false;

	VC[vcMainAir2].SetIO(aiMainAir2, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcMainAir2].SetParam(MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAX_PRESSURE, MPS_P35R_NCA_MIN_PRESSURE, WEIDMULLER_AD_S7);
	VC[vcMainAir2].SetErr(3000, ER_VAC_ALARM_MAIN_AIR2, HW::Vacuum::vacuumFREE, TRUE);
	VC[vcMainAir2].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcMainAir2].m_fdcData.use = false;

	VC[vcMainAir3].SetIO(aiMainAir3, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcMainAir3].SetParam(MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAX_PRESSURE, MPS_P35R_NCA_MIN_PRESSURE, WEIDMULLER_AD_S7);
	VC[vcMainAir3].SetErr(3000, ER_VAC_ALARM_MAIN_AIR3, HW::Vacuum::vacuumFREE, TRUE);
	VC[vcMainAir3].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcMainAir3].m_fdcData.use = false;

	VC[vcMainAir4].SetIO(aiMainAir4, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcMainAir4].SetParam(MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAX_PRESSURE, MPS_P35R_NCA_MIN_PRESSURE, WEIDMULLER_AD_S7);
	VC[vcMainAir4].SetErr(3000, ER_VAC_ALARM_MAIN_AIR4, HW::Vacuum::vacuumFREE, TRUE);
	VC[vcMainAir4].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcMainAir4].m_fdcData.use = false;

	VC[vcMbbOpenBtm].SetIO(aiVacMbbOpenBtm, (enDO)NOT_DEFINED, oVacMbbOpenBtmOff, (enDO)NOT_DEFINED);
	VC[vcMbbOpenBtm].SetParam(ZSE40A_MAXVOLT, ZSE40A_MINVOLT, ZSE40A_MAXVOLT, ZSE40A_MINVOLT, ZSE40A_MAX_PRESSURE, ZSE40A_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcMbbOpenTop].SetIO(aiVacMbbOpenTop, oVacMbbOpenTopOn, (enDO)NOT_DEFINED, oVacMbbOpenTopBlow);
	VC[vcMbbOpenTop].SetParam(PSE541_MAXVOLT, MPS_V35R_NCA_MINVOLT, PSE541_MAXVOLT, MPS_V35R_NCA_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcMbbOpenTop2].SetIO(aiVacMbbOpenTop2, oVacMbbOpenTopOn, (enDO)NOT_DEFINED, oVacMbbOpenTopBlow);
	VC[vcMbbOpenTop2].SetParam(PSE541_MAXVOLT, MPS_V35R_NCA_MINVOLT, PSE541_MAXVOLT, MPS_V35R_NCA_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcSealNozzle].SetIO(aiVacSealNozzle, (enDO)NOT_DEFINED, oVacSealNozzleOff, (enDO)NOT_DEFINED);
	VC[vcSealNozzle].SetParam(ZSE40A_MAXVOLT, ZSE40A_MINVOLT, ZSE40A_MAXVOLT, ZSE40A_MINVOLT, ZSE40A_MAX_PRESSURE, ZSE40A_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcSealNozzleN2].SetIO(aiAirSealNozzleN2, oVacSealNozzleN2On, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcSealNozzleN2].SetParam(MFC_MAXVOLT, MFC_MINVOLT, MFC_MAXVOLT, MFC_MINVOLT, MFC_MAX_ML, MFC_MIN_ML, WEIDMULLER_AD_S7);
	VC[vcSealNozzleN2].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;

	VC[vcLoadCellFt].SetIO(aiLoadCellSealFt, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcLoadCellFt].SetParam(BAUMER_LOADCELL_MAXVOLT, BAUMER_LOADCELL_MINVOLT, BAUMER_LOADCELL_MAXVOLT, BAUMER_LOADCELL_MINVOLT, BAUMER_LOADCELL_MAX_NEWTON, BAUMER_LOADCELL_MIN_NEWTON, WEIDMULLER_AD_S7);
	VC[vcLoadCellFt].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcLoadCellFt].m_fdcData.use = false;

	VC[vcLoadCellRr].SetIO(aiLoadCellSealRr, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcLoadCellRr].SetParam(BAUMER_LOADCELL_MAXVOLT, BAUMER_LOADCELL_MINVOLT, BAUMER_LOADCELL_MAXVOLT, BAUMER_LOADCELL_MINVOLT, BAUMER_LOADCELL_MAX_NEWTON, BAUMER_LOADCELL_MIN_NEWTON, WEIDMULLER_AD_S7);
	VC[vcLoadCellRr].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcLoadCellRr].m_fdcData.use = false;

	VC[vcLtCobotLabel].SetIO(aiVacLtCobotLabel, oVacLtCobotLabelOn, (enDO)NOT_DEFINED, oVacLtCobotLabelBlow);
	VC[vcLtCobotLabel].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcLtCobotFoam].SetIO(aiVacLtCobotFoam, oVacLtCobotFoamOn, (enDO)NOT_DEFINED, oVacLtCobotFoamBlow);
	VC[vcLtCobotFoam].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcLtCobotFoam2].SetIO(aiVacLtCobotFoam2, oVacLtCobotFoamOn, (enDO)NOT_DEFINED, oVacLtCobotFoamBlow);
	VC[vcLtCobotFoam2].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcFoamPkLaser].SetIO(aiFoamPkLaser, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcFoamPkLaser].SetParam(IL100_MAXVOLT, IL100_MINVOLT, IL100_MAXVOLT, IL100_MINVOLT, IL100_MAX_MM, IL100_MIN_MM, WEIDMULLER_AD_S7);
	VC[vcFoamPkLaser].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcFoamPkLaser].m_fdcData.use = false;

	VC[vcFoamFlip].SetIO(aiVacFoamFlip, oVacFoamFlipOn, (enDO)NOT_DEFINED, oVacFoamFlipBlow);
	VC[vcFoamFlip].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcFoamFlip2].SetIO(aiVacFoamFlip2, oVacFoamFlipOn, (enDO)NOT_DEFINED, oVacFoamFlipBlow);
	VC[vcFoamFlip2].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcFoamPk].SetIO(aiVacFoamPk, oVacFoamPkOn, (enDO)NOT_DEFINED, oVacFoamPkBlow);
	VC[vcFoamPk].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcFoamPk2].SetIO(aiVacFoamPk2, oVacFoamPkOn, (enDO)NOT_DEFINED, oVacFoamPkBlow);
	VC[vcFoamPk2].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcFlowLabelExhaust].SetIO(aiFlowLabelExhaust, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED, (enDO)NOT_DEFINED);
	VC[vcFlowLabelExhaust].SetParam(MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAXVOLT, MPS_P35R_NCA_MINVOLT, MPS_P35R_NCA_MAX_PRESSURE, MPS_P35R_NCA_MIN_PRESSURE, WEIDMULLER_AD_S7);
	VC[vcFlowLabelExhaust].SetErr(3000, ER_VAC_ALARM_FLOW_LABEL_EXHAUST, HW::Vacuum::vacuumFREE, TRUE);
	VC[vcFlowLabelExhaust].m_state.pressureType = IPC_MMI::PRESSURE_POSITIVE;
	VC[vcFlowLabelExhaust].m_fdcData.use = false;

	VC[vcQcSealedHead].SetIO(aiVacQcSealedHead, (enDO)NOT_DEFINED, oVacQcSealedHeadOff, (enDO)NOT_DEFINED);
	VC[vcQcSealedHead].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcLtLabelRecv].SetIO(aiVacLtLabelRecv, oVacLtLabelRecvOn, (enDO)NOT_DEFINED, oVacLtLabelRecvBlow);
	VC[vcLtLabelRecv].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcRtLabelRecv].SetIO(aiVacRtLabelRecv, oVacRtLabelRecvOn, (enDO)NOT_DEFINED, oVacRtLabelRecvBlow);
	VC[vcRtLabelRecv].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcRtCobotLabel].SetIO(aiVacRtCobotLabel, oVacRtCobotLabelOn, (enDO)NOT_DEFINED, oVacRtCobotLabelBlow);
	VC[vcRtCobotLabel].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcRtCobotMbb].SetIO(aiVacRtCobotMbb, oVacRtCobotMbbOn, (enDO)NOT_DEFINED, oVacRtCobotMbbBlow);
	VC[vcRtCobotMbb].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	VC[vcRtCobotMbb2].SetIO(aiVacRtCobotMbb2, oVacRtCobotMbbOn, (enDO)NOT_DEFINED, oVacRtCobotMbbBlow);
	VC[vcRtCobotMbb2].SetParam(PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAXVOLT, PSE541_MINVOLT, PSE541_MAX_PRESSURE, PSE541_MIN_PRESSURE, WEIDMULLER_AD_S7);

	return (TRUE);
}

BOOL InitCYLINDER(void)
{
	Cylinder::CreateInitParameter();

	CYL[cylInPkOc].CanMove = cylInPkOcCanMove;
	CYL[cylInPkOc].SetIO(iCylInPkOpen, iCylInPkClose, oCylInPkOpen, oCylInPkClose);
	CYL[cylInPkOc].SetParam("OPEN", "CLOSE");
	CYL[cylInPkOc].SetErr(10000, ER_CYL_ALARM_IN_PK_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylBtmFoamTableUd].SetIO(iCylBtmFoamTableUp, iCylBtmFoamTableDn, oCylBtmFoamTableUp, oCylBtmFoamTableDn);
	CYL[cylBtmFoamTableUd].SetParam("UP", "DOWN");
	CYL[cylBtmFoamTableUd].SetErr(10000, ER_CYL_ALARM_BTM_FOAM_TABLE_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylBtmFoamStopUd].SetIO(iCylBtmFoamStopUp, iCylBtmFoamStopDn, oCylBtmFoamStopUp, oCylBtmFoamStopDn);
	CYL[cylBtmFoamStopUd].SetParam("UP", "DOWN");
	CYL[cylBtmFoamStopUd].SetErr(10000, ER_CYL_ALARM_BTM_FOAM_STOP_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylBtmFoamFtClampOc].SetIO(iCylBtmFoamClampFtOp, iCylBtmFoamClampFtCl, oCylBtmFoamClampOp, oCylBtmFoamClampCl);
	CYL[cylBtmFoamFtClampOc].SetParam("OPEN", "CLOSE");
	CYL[cylBtmFoamFtClampOc].SetErr(10000, ER_CYL_ALARM_BTM_FOAM_FRONT_CLAMP_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylBtmFoamRrClampOc].SetIO(iCylBtmFoamClampRrOp, iCylBtmFoamClampRrCl, oCylBtmFoamClampOp, oCylBtmFoamClampCl);
	CYL[cylBtmFoamRrClampOc].SetParam("OPEN", "CLOSE");
	CYL[cylBtmFoamRrClampOc].SetErr(10000, ER_CYL_ALARM_BTM_FOAM_REAR_CLAMP_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylTopFoamStopUd].SetIO(iCylTopFoamStopUp, iCylTopFoamStopDn, oCylTopFoamStopUp, oCylTopFoamStopDn);
	CYL[cylTopFoamStopUd].SetParam("UP", "DOWN");
	CYL[cylTopFoamStopUd].SetErr(10000, ER_CYL_ALARM_TOP_FOAM_STOP_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylTrayShuttleNozzleUd].SetIO(iCylTrayShuttleAirBlowUp, iCylTrayShuttleAirBlowDn, oCylTrayShuttleAirBlowUp, oCylTrayShuttleAirBlowDn);
	CYL[cylTrayShuttleNozzleUd].SetParam("UP", "DOWN");
	CYL[cylTrayShuttleNozzleUd].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_NOZZLE_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylTrayShuttleBridgeUd].SetIO(iCylTrayShuttleBridgeUp, iCylTrayShuttleBridgeDn, oCylTrayShuttleBridgeUp, oCylTrayShuttleBridgeDn);
	CYL[cylTrayShuttleBridgeUd].SetParam("UP", "DOWN");
	CYL[cylTrayShuttleBridgeUd].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_BRIDGE_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylTrayShuttleBridgeFb].SetIO(iCylTrayShuttleBridgeFwd, iCylTrayShuttleBridgeBwd, oCylTrayShuttleBridgeFwd, oCylTrayShuttleBridgeBwd);
	CYL[cylTrayShuttleBridgeFb].SetParam("FWD", "BWD");
	CYL[cylTrayShuttleBridgeFb].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_BRIDGE_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylTrayShuttleFtStopFb].SetIO(iCylTrayShuttleStopFtFwd, iCylTrayShuttleStopFtBwd, oCylTrayShuttleStopFwd, oCylTrayShuttleStopBwd);
	CYL[cylTrayShuttleFtStopFb].SetParam("FWD", "BWD");
	CYL[cylTrayShuttleFtStopFb].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_FRONT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylTrayShuttleRrStopFb].SetIO(iCylTrayShuttleStopRrFwd, iCylTrayShuttleStopRrBwd, oCylTrayShuttleStopFwd, oCylTrayShuttleStopBwd);
	CYL[cylTrayShuttleRrStopFb].SetParam("FWD", "BWD");
	CYL[cylTrayShuttleRrStopFb].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_REAR_STOP_FB, Cylinder::cylinderBWD, TRUE);


	CYL[cylTrayShuttleFtMaskUd].SetIO(iCylTrayShuttleMaskFtUp, iCylTrayShuttleMaskDn, oCylTrayShuttleMaskUp, oCylTrayShuttleMaskDn);
	CYL[cylTrayShuttleFtMaskUd].SetParam("UP", "DOWN");
	CYL[cylTrayShuttleFtMaskUd].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_FRONT_MASK_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylTrayShuttleRrMaskUd].SetIO(iCylTrayShuttleMaskRrUp, iCylTrayShuttleMaskDn, oCylTrayShuttleMaskUp, oCylTrayShuttleMaskDn);
	CYL[cylTrayShuttleRrMaskUd].SetParam("UP", "DOWN");
	CYL[cylTrayShuttleRrMaskUd].SetErr(10000, ER_CYL_ALARM_TRAY_SHUTTLE_REAR_MASK_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylMbbOpenTopGuideFb].SetIO(iCylMbbOpenTopGuideFwd, iCylMbbOpenTopGuideBwd, oCylMbbOpenTopGuideFwd, oCylMbbOpenTopGuideBwd);
	CYL[cylMbbOpenTopGuideFb].SetParam("FWD", "BWD");
	CYL[cylMbbOpenTopGuideFb].SetErr(10000, ER_CYL_ALARM_MBB_OPEN_TOP_GUIDE_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbOpenTopVacUd].SetIO(iCylMbbOpenTopVacUp, iCylMbbOpenTopVacDn, oCylMbbOpenTopVacUp, oCylMbbOpenTopVacDn);
	CYL[cylMbbOpenTopVacUd].SetParam("UP", "DOWN");
	CYL[cylMbbOpenTopVacUd].SetErr(10000, ER_CYL_ALARM_MBB_OPEN_TOP_VAC_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylMbbOpenTopClampUd].SetIO(iCylMbbOpenTopClampUp, iCylMbbOpenTopClampDn, oCylMbbOpenTopClampUp, oCylMbbOpenTopClampDn);
	CYL[cylMbbOpenTopClampUd].SetParam("UP", "DOWN");
	CYL[cylMbbOpenTopClampUd].SetErr(10000, ER_CYL_ALARM_MBB_OPEN_TOP_CLAMP_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylSealNozzleFb].SetIO(iCylSealNozzleFwd, iCylSealNozzleBwd, oCylSealNozzleFwd, oCylSealNozzleBwd);
	CYL[cylSealNozzleFb].SetParam("FWD", "BWD");
	CYL[cylSealNozzleFb].SetErr(10000, ER_CYL_ALARM_SEAL_NOZZLE_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylSealMbbOpenGuideFtOc].SetIO(iCylSealMbbOpenGuideFtOp, iCylSealMbbOpenGuideFtCl, oCylSealMbbOpenGuideOp, oCylSealMbbOpenGuideCl);
	CYL[cylSealMbbOpenGuideFtOc].SetParam("OPEN", "CLOSE");
	CYL[cylSealMbbOpenGuideFtOc].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_OPEN_GUIDE_FRONT_OC, Cylinder::cylinderCLOSE, TRUE);

	CYL[cylSealMbbOpenGuideRrOc].SetIO(iCylSealMbbOpenGuideRrOp, iCylSealMbbOpenGuideRrCl, oCylSealMbbOpenGuideOp, oCylSealMbbOpenGuideCl);
	CYL[cylSealMbbOpenGuideRrOc].SetParam("OPEN", "CLOSE");
	CYL[cylSealMbbOpenGuideRrOc].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_OPEN_GUIDE_REAR_OC, Cylinder::cylinderCLOSE, TRUE);

	CYL[cylSealNozzleMfcN2Oc].SetIO((enDI)NOT_DEFINED, (enDI)NOT_DEFINED, oCylSealNozzleMfcN2Op, (enDO)NOT_DEFINED);
	CYL[cylSealNozzleMfcN2Oc].SetParam("OPEN", "CLOSE");
	CYL[cylSealNozzleMfcN2Oc].SetErr(10000, ER_CYL_ALARM_SEAL_NOZZLE_MFC_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylSealBtmGuideUd].SetIO(iCylSealBtmGuideUp, iCylSealBtmGuideDn, oCylSealBtmGuideUp, oCylSealBtmGuideDn);
	CYL[cylSealBtmGuideUd].SetParam("UP", "DOWN");
	CYL[cylSealBtmGuideUd].SetErr(10000, ER_CYL_ALARM_SEAL_BTM_GUIDE_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylSealerBtmUd].SetIO(iCylSealerBtmUp, iCylSealerBtmDn, oCylSealerBtmUp, oCylSealerBtmDn);
	CYL[cylSealerBtmUd].SetParam("UP", "DOWN");
	CYL[cylSealerBtmUd].SetErr(10000, ER_CYL_ALARM_SEALER_BTM_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylSealerTopFtUd].SetIO(iCylSealerTopFtUp, iCylSealerTopFtDn, oCylSealerTopUp, oCylSealerTopDn);
	CYL[cylSealerTopFtUd].SetParam("UP", "DOWN");
	CYL[cylSealerTopFtUd].SetErr(10000, ER_CYL_ALARM_SEALER_TOP_FRONT_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylSealerTopRrUd].SetIO(iCylSealerTopRrUp, iCylSealerTopRrDn, oCylSealerTopUp, oCylSealerTopDn);
	CYL[cylSealerTopRrUd].SetParam("UP", "DOWN");
	CYL[cylSealerTopRrUd].SetErr(10000, ER_CYL_ALARM_SEALER_TOP_REAR_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylSealTopGuideUd].SetIO(iCylSealTopGuideUp, iCylSealTopGuideDn, oCylSealTopGuideUp, oCylSealTopGuideDn);
	CYL[cylSealTopGuideUd].SetParam("UP", "DOWN");
	CYL[cylSealTopGuideUd].SetErr(10000, ER_CYL_ALARM_SEAL_TOP_GUIDE_UD, Cylinder::cylinderUP, TRUE);


	CYL[cylLtCobotLabelPkUd].SetIO(iCylLtCobotLabelPkUp, iCylLtCobotLabelPkDn, oCylLtCobotLabelPkUp, oCylLtCobotLabelPkDn);
	CYL[cylLtCobotLabelPkUd].SetParam("UP", "DOWN");
	CYL[cylLtCobotLabelPkUd].SetErr(10000, ER_CYL_ALARM_LEFT_COBOT_LABEL_PK_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylLtCobotLabelPkTurnFb].SetIO(iCylLtCobotLabelPkTurnFwd, iCylLtCobotLabelPkTurnBwd, oCylLtCobotLabelPkTurnFwd, oCylLtCobotLabelPkTurnBwd);
	CYL[cylLtCobotLabelPkTurnFb].SetParam("FWD", "BWD");
	CYL[cylLtCobotLabelPkTurnFb].SetErr(10000, ER_CYL_ALARM_LEFT_COBOT_LABEL_PK_TURN_FB, Cylinder::cylinderDOWN, TRUE);

	CYL[cylFoamRecvElevLtAlignFb].SetIO(iCylFoamRecvElevLtAlignFwd, iCylFoamRecvElevLtAlignBwd, oCylFoamRecvElevLtAlignFwd, oCylFoamRecvElevLtAlignBwd);
	CYL[cylFoamRecvElevLtAlignFb].SetParam("FWD", "BWD");
	CYL[cylFoamRecvElevLtAlignFb].SetErr(10000, ER_CYL_ALARM_FOAM_RECV_ELEV_LEFT_ALIGN_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamRecvElevRrAlignFb].SetIO(iCylFoamRecvElevRrAlignFwd, iCylFoamRecvElevRrAlignBwd, oCylFoamRecvElevRrAlignFwd, oCylFoamRecvElevRrAlignBwd);
	CYL[cylFoamRecvElevRrAlignFb].SetParam("FWD", "BWD");
	CYL[cylFoamRecvElevRrAlignFb].SetErr(10000, ER_CYL_ALARM_FOAM_RECV_ELEV_REAR_ALIGN_FB, Cylinder::cylinderBWD, TRUE);
	 
	CYL[cylFoamRecvElevLtStopFb].SetIO(iCylFoamRecvElevLtStopFwd, iCylFoamRecvElevLtStopBwd, oCylFoamRecvElevStopFwd, oCylFoamRecvElevStopBwd);
	CYL[cylFoamRecvElevLtStopFb].SetParam("FWD", "BWD");
	CYL[cylFoamRecvElevLtStopFb].SetErr(10000, ER_CYL_ALARM_FOAM_RECV_ELEV_LT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamRecvElevRtStopFb].SetIO(iCylFoamRecvElevRtStopFwd, iCylFoamRecvElevRtStopBwd, oCylFoamRecvElevStopFwd, oCylFoamRecvElevStopBwd);
	CYL[cylFoamRecvElevRtStopFb].SetParam("FWD", "BWD");
	CYL[cylFoamRecvElevRtStopFb].SetErr(10000, ER_CYL_ALARM_FOAM_RECV_ELEV_RT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamRrStackLiftUd].SetIO(iCylFoamRrStackLiftUp, iCylFoamRrStackLiftDn, oCylFoamRrStackLiftUp, oCylFoamRrStackLiftDn);
	CYL[cylFoamRrStackLiftUd].SetParam("UP", "DOWN");
	CYL[cylFoamRrStackLiftUd].SetErr(10000, ER_CYL_ALARM_FOAM_REAR_STACK_LIFT_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylFoamRrStackLtStopFb].SetIO(iCylFoamRrStackLtStopFwd, iCylFoamRrStackLtStopBwd, oCylFoamRrStackStopFwd, oCylFoamRrStackStopBwd);
	CYL[cylFoamRrStackLtStopFb].SetParam("FWD", "BWD");
	CYL[cylFoamRrStackLtStopFb].SetErr(10000, ER_CYL_ALARM_FOAM_REAR_STACK_LT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamRrStackRtStopFb].SetIO(iCylFoamRrStackRtStopFwd, iCylFoamRrStackRtStopBwd, oCylFoamRrStackStopFwd, oCylFoamRrStackStopBwd);
	CYL[cylFoamRrStackRtStopFb].SetParam("FWD", "BWD");
	CYL[cylFoamRrStackRtStopFb].SetErr(10000, ER_CYL_ALARM_FOAM_REAR_STACK_RT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamFtStackLiftUd].SetIO(iCylFoamFtStackLiftUp, iCylFoamFtStackLiftDn, oCylFoamFtStackLiftUp, oCylFoamFtStackLiftDn);
	CYL[cylFoamFtStackLiftUd].SetParam("UP", "DOWN");
	CYL[cylFoamFtStackLiftUd].SetErr(10000, ER_CYL_ALARM_FOAM_FRONT_STACK_LIFT_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylFoamFtStackLtStopFb].SetIO(iCylFoamFtStackLtStopFwd, iCylFoamFtStackLtStopBwd, oCylFoamFtStackStopFwd, oCylFoamFtStackStopBwd);
	CYL[cylFoamFtStackLtStopFb].SetParam("FWD", "BWD");
	CYL[cylFoamFtStackLtStopFb].SetErr(10000, ER_CYL_ALARM_FOAM_FRONT_STACK_LT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamFtStackRtStopFb].SetIO(iCylFoamFtStackRtStopFwd, iCylFoamFtStackRtStopBwd, oCylFoamFtStackStopFwd, oCylFoamFtStackStopBwd);
	CYL[cylFoamFtStackRtStopFb].SetParam("FWD", "BWD");
	CYL[cylFoamFtStackRtStopFb].SetErr(10000, ER_CYL_ALARM_FOAM_FRONT_STACK_RT_STOP_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamSendElevLtAlignFb].SetIO(iCylFoamSendElevLtAlignFwd, iCylFoamSendElevLtAlignBwd, oCylFoamSendElevLtAlignFwd, oCylFoamSendElevLtAlignBwd);
	CYL[cylFoamSendElevLtAlignFb].SetParam("FWD", "BWD");
	CYL[cylFoamSendElevLtAlignFb].SetErr(10000, ER_CYL_ALARM_FOAM_SEND_ELEV_LEFT_ALIGN_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamSendElevRtAlignFb].SetIO(iCylFoamSendElevRtAlignFwd, iCylFoamSendElevRtAlignBwd, oCylFoamSendElevRtAlignFwd, oCylFoamSendElevRtAlignBwd);
	CYL[cylFoamSendElevRtAlignFb].SetParam("FWD", "BWD");
	CYL[cylFoamSendElevRtAlignFb].SetErr(10000, ER_CYL_ALARM_FOAM_SEND_ELEV_RIGHT_ALIGN_FB, Cylinder::cylinderBWD, TRUE);


	CYL[cylFoamPartialElevLtAlignFb].SetIO(iCylFoamPartialElevLtAlignFwd, iCylFoamPartialElevLtAlignBwd, oCylFoamPartialElevLtAlignFwd, oCylFoamPartialElevLtAlignBwd);
	CYL[cylFoamPartialElevLtAlignFb].SetParam("FWD", "BWD");
	CYL[cylFoamPartialElevLtAlignFb].SetErr(10000, ER_CYL_ALARM_FOAM_PARTIAL_ELEV_LEFT_ALIGN_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylFoamPartialElevRrAlignFb].SetIO(iCylFoamPartialElevRrAlignFwd, iCylFoamPartialElevRrAlignBwd, oCylFoamPartialElevRrAlignFwd, oCylFoamPartialElevRrAlignBwd);
	CYL[cylFoamPartialElevRrAlignFb].SetParam("FWD", "BWD");
	CYL[cylFoamPartialElevRrAlignFb].SetErr(10000, ER_CYL_ALARM_FOAM_PARTIAL_ELEV_FRONT_ALIGN_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbShuttleAlignFtFb].SetIO(iCylMbbShuttleAlignFtFwd, iCylMbbShuttleAlignFtBwd, oCylMbbShuttleAlignFwd, oCylMbbShuttleAlignBwd);
	CYL[cylMbbShuttleAlignFtFb].SetParam("FWD", "BWD");
	CYL[cylMbbShuttleAlignFtFb].SetErr(10000, ER_CYL_ALARM_MBB_SHUTTLE_ALIGN_FRONT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbShuttleAlignRrFb].SetIO(iCylMbbShuttleAlignRrFwd, iCylMbbShuttleAlignRrBwd, oCylMbbShuttleAlignFwd, oCylMbbShuttleAlignBwd);
	CYL[cylMbbShuttleAlignRrFb].SetParam("FWD", "BWD");
	CYL[cylMbbShuttleAlignRrFb].SetErr(10000, ER_CYL_ALARM_MBB_SHUTTLE_ALIGN_REAR_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbShuttleClampFtFb].SetIO(iCylMbbShuttleClampFtFwd, iCylMbbShuttleClampFtBwd, oCylMbbShuttleClampFwd, oCylMbbShuttleClampBwd);
	CYL[cylMbbShuttleClampFtFb].SetParam("FWD", "BWD");
	CYL[cylMbbShuttleClampFtFb].SetErr(10000, ER_CYL_ALARM_MBB_SHUTTLE_CLAMP_FRONT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbShuttleClampRrFb].SetIO(iCylMbbShuttleClampRrFwd, iCylMbbShuttleClampRrBwd, oCylMbbShuttleClampFwd, oCylMbbShuttleClampBwd);
	CYL[cylMbbShuttleClampRrFb].SetParam("FWD", "BWD");
	CYL[cylMbbShuttleClampRrFb].SetErr(10000, ER_CYL_ALARM_MBB_SHUTTLE_CLAMP_REAR_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylQcSealedHeadBtmFtVacUd].SetIO(iCylQcSealedHeadBtmVacFtUp, iCylQcSealedHeadBtmVacFtDn, oCylQcSealedHeadBtmVacUp, oCylQcSealedHeadBtmVacDn);
	CYL[cylQcSealedHeadBtmFtVacUd].SetParam("UP", "DOWN");
	CYL[cylQcSealedHeadBtmFtVacUd].SetErr(10000, ER_CYL_ALARM_QC_SEALED_HEAD_FT_VAC_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylQcSealedHeadBtmRrVacUd].SetIO(iCylQcSealedHeadBtmVacRrUp, iCylQcSealedHeadBtmVacRrDn, oCylQcSealedHeadBtmVacUp, oCylQcSealedHeadBtmVacDn);
	CYL[cylQcSealedHeadBtmRrVacUd].SetParam("UP", "DOWN");
	CYL[cylQcSealedHeadBtmRrVacUd].SetErr(10000, ER_CYL_ALARM_QC_SEALED_HEAD_RR_VAC_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylQcSealedHeadTopFtClampUd].SetIO(iCylQcSealedHeadTopClampFtUp, iCylQcSealedHeadTopClampFtDn, oCylQcSealedHeadTopClampUp, oCylQcSealedHeadTopClampDn);
	CYL[cylQcSealedHeadTopFtClampUd].SetParam("UP", "DOWN");
	CYL[cylQcSealedHeadTopFtClampUd].SetErr(10000, ER_CYL_ALARM_QC_SEALED_HEAD_VAC_FT_GUIDE_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylQcSealedHeadTopRrClampUd].SetIO(iCylQcSealedHeadTopClampRrUp, iCylQcSealedHeadTopClampRrDn, oCylQcSealedHeadTopClampUp, oCylQcSealedHeadTopClampDn);
	CYL[cylQcSealedHeadTopRrClampUd].SetParam("UP", "DOWN");
	CYL[cylQcSealedHeadTopRrClampUd].SetErr(10000, ER_CYL_ALARM_QC_SEALED_HEAD_VAC_RR_GUIDE_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylQcSealedTrayStopUd].SetIO(iCylQcSealedTrayStopUp, iCylQcSealedTrayStopDn, oCylQcSealedTrayStopUp, oCylQcSealedTrayStopDn);
	CYL[cylQcSealedTrayStopUd].SetParam("UP", "DOWN");
	CYL[cylQcSealedTrayStopUd].SetErr(10000, ER_CYL_ALARM_QC_SEALED_TRAY_STOP_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylQcSealedTrayAlignFtFb].SetIO(iCylQcSealedTrayAlignFtFwd, iCylQcSealedTrayAlignFtBwd, oCylQcSealedTrayAlignFwd, oCylQcSealedTrayAlignBwd);
	CYL[cylQcSealedTrayAlignFtFb].SetParam("FWD", "BWD");
	CYL[cylQcSealedTrayAlignFtFb].SetErr(10000, ER_CYL_ALARM_QC_SEALED_TRAY_ALIGN_FRONT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylQcSealedTrayAlignRrFb].SetIO(iCylQcSealedTrayAlignRrFwd, iCylQcSealedTrayAlignRrBwd, oCylQcSealedTrayAlignFwd, oCylQcSealedTrayAlignBwd);
	CYL[cylQcSealedTrayAlignRrFb].SetParam("FWD", "BWD");
	CYL[cylQcSealedTrayAlignRrFb].SetErr(10000, ER_CYL_ALARM_QC_SEALED_TRAY_ALIGN_REAR_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldAssistUd].SetIO(iCylMbbFoldAssistUp, iCylMbbFoldAssistDn, oCylMbbFoldAssistUp, oCylMbbFoldAssistDn);
	CYL[cylMbbFoldAssistUd].SetParam("UP", "DOWN");
	CYL[cylMbbFoldAssistUd].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_ASSIST_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylMbbFoldAlignFtFb].SetIO(iCylMbbFoldAlignFtFwd, iCylMbbFoldAlignFtBwd, oCylMbbFoldAlignFwd, oCylMbbFoldAlignBwd);
	CYL[cylMbbFoldAlignFtFb].SetParam("FWD", "BWD");
	CYL[cylMbbFoldAlignFtFb].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_FRONT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldAlignRrFb].SetIO(iCylMbbFoldAlignRrFwd, iCylMbbFoldAlignRrBwd, oCylMbbFoldAlignFwd, oCylMbbFoldAlignBwd);
	CYL[cylMbbFoldAlignRrFb].SetParam("FWD", "BWD");
	CYL[cylMbbFoldAlignRrFb].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_REAR_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldRtFb].SetIO(iCylMbbFoldRtFwd, iCylMbbFoldRtBwd, oCylMbbFoldRtFwd, oCylMbbFoldRtBwd);
	CYL[cylMbbFoldRtFb].SetParam("FWD", "BWD");
	CYL[cylMbbFoldRtFb].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_RIGHT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldRtFtOc].SetIO(iCylMbbFoldRtFtOp, iCylMbbFoldRtFtCl, oCylMbbFoldRtOp, oCylMbbFoldRtCl);
	CYL[cylMbbFoldRtFtOc].SetParam("OPEN", "CLOSE");
	CYL[cylMbbFoldRtFtOc].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_RIGHT_FRONT_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylMbbFoldRtRrOc].SetIO(iCylMbbFoldRtRrOp, iCylMbbFoldRtRrCl, oCylMbbFoldRtOp, oCylMbbFoldRtCl);
	CYL[cylMbbFoldRtRrOc].SetParam("OPEN", "CLOSE");
	CYL[cylMbbFoldRtRrOc].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_RIGHT_REAR_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylMbbFoldPkLtBlockTurnFb].SetIO(iCylMbbFoldPkLtTurnFwd, iCylMbbFoldPkLtTurnBwd, oCylMbbFoldPkLtTurnFwd, oCylMbbFoldPkLtTurnBwd);
	CYL[cylMbbFoldPkLtBlockTurnFb].SetParam("FWD", "BWD");
	CYL[cylMbbFoldPkLtBlockTurnFb].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_PK_LEFT_BLOCK_TURN_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldPkLtBlockFtOc].SetIO(iCylMbbFoldPkLtFtOp, iCylMbbFoldPkLtFtCl, oCylMbbFoldPkLtOp, oCylMbbFoldPkLtCl);
	CYL[cylMbbFoldPkLtBlockFtOc].SetParam("OPEN", "CLOSE");
	CYL[cylMbbFoldPkLtBlockFtOc].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_PK_LEFT_BLOCK_FRONT_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylMbbFoldPkLtBlockRrOc].SetIO(iCylMbbFoldPkLtRrOp, iCylMbbFoldPkLtRrCl, oCylMbbFoldPkLtOp, oCylMbbFoldPkLtCl);
	CYL[cylMbbFoldPkLtBlockRrOc].SetParam("OPEN", "CLOSE");
	CYL[cylMbbFoldPkLtBlockRrOc].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_PK_LEFT_BLOCK_REAR_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylMbbFoldPkClampOc].SetIO(iCylMbbFoldPkClampOp, iCylMbbFoldPkClampCl, oCylMbbFoldPkClampOp, oCylMbbFoldPkClampCl);
	CYL[cylMbbFoldPkClampOc].SetParam("OPEN", "CLOSE");
	CYL[cylMbbFoldPkClampOc].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_PK_CLAMP_OC, Cylinder::cylinderOPEN, TRUE);

	CYL[cylLtLabelFeederFb].SetIO(iCylLtLabelFeederFwd, iCylLtLabelFeederBwd, oCylLtLabelFeederFwd, oCylLtLabelFeederBwd);
	CYL[cylLtLabelFeederFb].SetParam("FWD", "BWD");
	CYL[cylLtLabelFeederFb].SetErr(20000, ER_CYL_ALARM_LEFT_LABEL_FEEDER_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylLtLabelFeederMountFb].SetIO(iCylLtLabelFeederMountFwd, iCylLtLabelFeederMountBwd, oCylLtLabelFeederMountFwd, oCylLtLabelFeederMountBwd);
	CYL[cylLtLabelFeederMountFb].SetParam("FWD", "BWD");
	CYL[cylLtLabelFeederMountFb].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_FEEDER_MOUNT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylLtLabelFeederLockFtFb].SetIO(iCylLtLabelFeederLockFtFwd, iCylLtLabelFeederLockFtBwd, oCylLtLabelFeederLockFwd, oCylLtLabelFeederLockBwd);
	CYL[cylLtLabelFeederLockFtFb].SetParam("FWD", "BWD");
	CYL[cylLtLabelFeederLockFtFb].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_FEEDER_LOCK_FRONT_FB, Cylinder::cylinderFWD, TRUE);

	CYL[cylLtLabelFeederLockRrFb].SetIO(iCylLtLabelFeederLockRrFwd, iCylLtLabelFeederLockRrBwd, oCylLtLabelFeederLockFwd, oCylLtLabelFeederLockBwd);
	CYL[cylLtLabelFeederLockRrFb].SetParam("FWD", "BWD");
	CYL[cylLtLabelFeederLockRrFb].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_FEEDER_LOCK_REAR_FB, Cylinder::cylinderFWD, TRUE);

	CYL[cylLtLabelRecvUd].SetIO(iCylLtLabelRecvUp, iCylLtLabelRecvDn, oCylLtLabelRecvUp, oCylLtLabelRecvDn);
	CYL[cylLtLabelRecvUd].SetParam("UP", "DOWN");
	CYL[cylLtLabelRecvUd].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_RECV_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylLtLabelRecvFb].SetIO(iCylLtLabelRecvFwd, iCylLtLabelRecvBwd, oCylLtLabelRecvFwd, oCylLtLabelRecvBwd);
	CYL[cylLtLabelRecvFb].SetParam("FWD", "BWD");
	CYL[cylLtLabelRecvFb].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_RECV_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylLtLabelRecvEjectPinLtUd].SetIO(iCylLtLabelRecvEjectPinLtUp, iCylLtLabelRecvEjectPinLtDn, oCylLtLabelRecvEjectPinUp, oCylLtLabelRecvEjectPinDn);
	CYL[cylLtLabelRecvEjectPinLtUd].SetParam("FWD", "BWD");
	CYL[cylLtLabelRecvEjectPinLtUd].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_RECV_EJECT_PIN_LT_UD, Cylinder::cylinderBWD, TRUE);

	CYL[cylLtLabelRecvEjectPinRtUd].SetIO(iCylLtLabelRecvEjectPinRtUp, iCylLtLabelRecvEjectPinRtDn, oCylLtLabelRecvEjectPinUp, oCylLtLabelRecvEjectPinDn);
	CYL[cylLtLabelRecvEjectPinRtUd].SetParam("FWD", "BWD");
	CYL[cylLtLabelRecvEjectPinRtUd].SetErr(10000, ER_CYL_ALARM_LEFT_LABEL_RECV_EJECT_PIN_RT_UD, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtLabelRecvUd].SetIO(iCylRtLabelRecvUp, iCylRtLabelRecvDn, oCylRtLabelRecvUp, oCylRtLabelRecvDn);
	CYL[cylRtLabelRecvUd].SetParam("UP", "DOWN");
	CYL[cylRtLabelRecvUd].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_RECV_UD, Cylinder::cylinderDOWN, TRUE);

	CYL[cylRtLabelRecvFb].SetIO(iCylRtLabelRecvFwd, iCylRtLabelRecvBwd, oCylRtLabelRecvFwd, oCylRtLabelRecvBwd);
	CYL[cylRtLabelRecvFb].SetParam("FWD", "BWD");
	CYL[cylRtLabelRecvFb].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_RECV_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtLabelRecvEjectPinLtUd].SetIO(iCylRtLabelRecvEjectPinLtUp, iCylRtLabelRecvEjectPinLtDn, oCylRtLabelRecvEjectPinUp, oCylRtLabelRecvEjectPinDn);
	CYL[cylRtLabelRecvEjectPinLtUd].SetParam("FWD", "BWD");
	CYL[cylRtLabelRecvEjectPinLtUd].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_RECV_EJECT_PIN_LT_UD, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtLabelRecvEjectPinRtUd].SetIO(iCylRtLabelRecvEjectPinRtUp, iCylRtLabelRecvEjectPinRtDn, oCylRtLabelRecvEjectPinUp, oCylRtLabelRecvEjectPinDn);
	CYL[cylRtLabelRecvEjectPinRtUd].SetParam("FWD", "BWD");
	CYL[cylRtLabelRecvEjectPinRtUd].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_RECV_EJECT_PIN_RT_UD, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtLabelFeederFb].SetIO(iCylRtLabelFeederFwd, iCylRtLabelFeederBwd, oCylRtLabelFeederFwd, oCylRtLabelFeederBwd);
	CYL[cylRtLabelFeederFb].SetParam("FWD", "BWD");
	CYL[cylRtLabelFeederFb].SetErr(20000, ER_CYL_ALARM_RIGHT_LABEL_FEEDER_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtLabelFeederMountFb].SetIO(iCylRtLabelFeederMountFwd, iCylRtLabelFeederMountBwd, oCylRtLabelFeederMountFwd, oCylRtLabelFeederMountBwd);
	CYL[cylRtLabelFeederMountFb].SetParam("FWD", "BWD");
	CYL[cylRtLabelFeederMountFb].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_FEEDER_MOUNT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtLabelFeederLockFtFb].SetIO(iCylRtLabelFeederLockFtFwd, iCylRtLabelFeederLockFtBwd, oCylRtLabelFeederLockFwd, oCylRtLabelFeederLockBwd);
	CYL[cylRtLabelFeederLockFtFb].SetParam("FWD", "BWD");
	CYL[cylRtLabelFeederLockFtFb].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_FEEDER_LOCK_FRONT_FB, Cylinder::cylinderFWD, TRUE);

	CYL[cylRtLabelFeederLockRrFb].SetIO(iCylRtLabelFeederLockRrFwd, iCylRtLabelFeederLockRrBwd, oCylRtLabelFeederLockFwd, oCylRtLabelFeederLockBwd);
	CYL[cylRtLabelFeederLockRrFb].SetParam("FWD", "BWD");
	CYL[cylRtLabelFeederLockRrFb].SetErr(10000, ER_CYL_ALARM_RIGHT_LABEL_FEEDER_LOCK_REAR_FB, Cylinder::cylinderFWD, TRUE);

	CYL[cylPowerMeterOc].SetIO(iCylPowerMeterOp, iCylPowerMeterCl, oCylPowerMeterOp, oCylPowerMeterCl);
	CYL[cylPowerMeterOc].SetParam("OPEN", "CLOSE");
	CYL[cylPowerMeterOc].SetErr(10000, ER_CYL_ALARM_POWER_METER_OC, Cylinder::cylinderCLOSE, TRUE);

	CYL[cylLabelQcViFb].SetIO(iCylLabelQcViLt, iCylLabelQcViRt, oCylLabelQcViLt, oCylLabelQcViRt);
	CYL[cylLabelQcViFb].SetParam("FWD", "BWD");
	CYL[cylLabelQcViFb].SetErr(10000, ER_CYL_ALARM_LABEL_QC_VI_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtCobotLabelPkUd].SetIO(iCylRtCobotLabelPkUp, iCylRtCobotLabelPkDn, oCylRtCobotLabelPkUp, oCylRtCobotLabelPkDn);
	CYL[cylRtCobotLabelPkUd].SetParam("UP", "DOWN");
	CYL[cylRtCobotLabelPkUd].SetErr(10000, ER_CYL_ALARM_RIGHT_COBOT_LABEL_PK_UD, Cylinder::cylinderUP, TRUE);

	CYL[cylRtCobotLabelPkTurnFb].SetIO(iCylRtCobotLabelPkTurnFwd, iCylRtCobotLabelPkTurnBwd, oCylRtCobotLabelPkTurnFwd, oCylRtCobotLabelPkTurnBwd);
	CYL[cylRtCobotLabelPkTurnFb].SetParam("FWD", "BWD");
	CYL[cylRtCobotLabelPkTurnFb].SetErr(10000, ER_CYL_ALARM_RIGHT_COBOT_LABEL_PK_TURN_FB, Cylinder::cylinderDOWN, TRUE);

	CYL[cylLtMbbCstFb].SetIO(iCylLtMbbCstFwd, iCylLtMbbCstBwd, oCylLtMbbCstFwd, oCylLtMbbCstBwd);
	CYL[cylLtMbbCstFb].SetParam("FWD", "BWD");
	CYL[cylLtMbbCstFb].SetErr(20000, ER_CYL_ALARM_LEFT_MBB_CST_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylRtMbbCstFb].SetIO(iCylRtMbbCstFwd, iCylRtMbbCstBwd, oCylRtMbbCstFwd, oCylRtMbbCstBwd);
	CYL[cylRtMbbCstFb].SetParam("FWD", "BWD");
	CYL[cylRtMbbCstFb].SetErr(20000, ER_CYL_ALARM_RIGHT_MBB_CST_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldPkLtFtAssist].SetIO(iCylMbbFoldPkLtFtAssistFwd, iCylMbbFoldPkLtFtAssistBwd, oCylMbbFoldPkLtAssistFwd, oCylMbbFoldPkLtAssistBwd);
	CYL[cylMbbFoldPkLtFtAssist].SetParam("FWD", "BWD");
	CYL[cylMbbFoldPkLtFtAssist].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_PK_LT_FT_ASSIST_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbFoldPkLtRrAssist].SetIO(iCylMbbFoldPkLtRrAssistFwd, iCylMbbFoldPkLtRrAssistBwd, oCylMbbFoldPkLtAssistFwd, oCylMbbFoldPkLtAssistBwd);
	CYL[cylMbbFoldPkLtRrAssist].SetParam("FWD", "BWD");
	CYL[cylMbbFoldPkLtRrAssist].SetErr(10000, ER_CYL_ALARM_MBB_FOLD_PK_LT_RR_ASSIST_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylSealMbbClampUd].SetIO(iCylSealMBBClampUp, iCylSealMBBClampDn, oCylSealMBBClampUp, oCylSealMBBClampDn);
	CYL[cylSealMbbClampUd].SetParam("UP", "DOWN");
	CYL[cylSealMbbClampUd].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_CLAMP_UD, Cylinder::cylinderBWD, TRUE);

	CYL[cylSealMbbClampFtFb].SetIO(iCylSealMBBClampFtFwd, iCylSealMBBClampFtBwd, oCylSealMBBClampFwd, oCylSealMBBClampBwd);
	CYL[cylSealMbbClampFtFb].SetParam("FWD", "BWD");
	CYL[cylSealMbbClampFtFb].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_CLAMP_FT_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylSealMbbClampRrFb].SetIO(iCylSealMBBClampRrFwd, iCylSealMBBClampRrBwd, oCylSealMBBClampFwd, oCylSealMBBClampBwd);
	CYL[cylSealMbbClampRrFb].SetParam("FWD", "BWD");
	CYL[cylSealMbbClampRrFb].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_CLAMP_RR_FB, Cylinder::cylinderBWD, TRUE);

	CYL[cylSealMbbClampFtOc].SetIO((enDI)NOT_DEFINED, iCylSealMBBClampFtClose, oCylSealMBBClampOpen, oCylSealMBBClampClose);
	CYL[cylSealMbbClampFtOc].SetParam("OPEN", "CLOSE");
	CYL[cylSealMbbClampFtOc].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_CLAMP_FT_OC, Cylinder::cylinderBWD, TRUE);

	CYL[cylSealMbbClampRrOc].SetIO((enDI)NOT_DEFINED, iCylSealMBBClampRrClose, oCylSealMBBClampOpen, oCylSealMBBClampClose);
	CYL[cylSealMbbClampRrOc].SetParam("OPEN", "CLOSE");
	CYL[cylSealMbbClampRrOc].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_CLAMP_RR_OC, Cylinder::cylinderBWD, TRUE);

	CYL[cylMbbShuttleClampUd].SetIO(iCylMbbShuttleClampUp, iCylMbbShuttleClampDn, oCylMbbShuttleClampUp, oCylMbbShuttleClampDn);
	CYL[cylMbbShuttleClampUd].SetParam("OPEN", "CLOSE");
	CYL[cylMbbShuttleClampUd].SetErr(10000, ER_CYL_ALARM_SEAL_MBB_CLAMP_RR_OC, Cylinder::cylinderBWD, TRUE);
	
	return (TRUE);
}

BOOL InitCONVEYOR(void)
{
	Conveyor::CreateInitParameter();

	CONV[CV_FOAM_CONV].SetIO(oAcFoamConvDir, oAcFoamConvRun);
	CONV[CV_FOAM_CONV].SetParam("FWD", "BWD", "STOP");

	CONV[CV_TRAY_SHUTTLE].SetIO(oAcTrayShuttleConvDir, oAcTrayShuttleConvRun);
	CONV[CV_TRAY_SHUTTLE].SetParam("FWD", "BWD", "STOP");

	CONV[CV_FOAM_STACK].SetIO(oAcFoamStackConvDir, oAcFoamStackConvRun);
	CONV[CV_FOAM_STACK].SetParam("FWD", "BWD", "STOP");

	CONV[CV_QC].SetIO(oAcQcConvDir, oAcQcConvRun);
	CONV[CV_QC].SetParam("FWD", "BWD", "STOP");

	CONV[CV_MBB_FOLD].SetIO(oAcMbbFoldConvDir, oAcMbbFoldConvRun);
	CONV[CV_MBB_FOLD].SetParam("FWD", "BWD", "STOP");

	return TRUE;
}

BOOL InitLASER(void)
{
	Laser::CreateInitParameter();

	LAS[LASER].SetIO(iLaserAuto, iLaserReady, iLaserBusy, iLaserError, iLaserRecvData,
		oLaserTrig, oLaserReset, oLaserSendData);
	LAS[LASER].SetErr(ER_LASER_NOT_AUTO, ER_SCAN_READY, ER_SCAN_BUSY, ER_SCAN_DATA_TIMEOVER,
		ER_SCAN_COORD_DATA_TIMEOVER, ER_SCAN_ERROR, ER_SCAN_MODE);

	return (TRUE);
}

BOOL InitROBOT(void)
{
	Robot::CRobot::CreateInitParameter();

	ROBOT[robotLt].Init(L"cobot_Lt", L"192.168.1.10", RobotPort);
	ROBOT[robotRt].Init(L"cobot_Rt", L"192.168.2.10", RobotPort);
	ROBOT[robotLt].CanMove = LtCobotCanMove;
	ROBOT[robotRt].CanMove = RtCobotCanMove;

	return (TRUE);
}

BOOL InitETC(void)
{
	//if (IPC_MMI::MAX_SEND_SCAN_COORD != IPC_LASER::MAX_SEND_SCAN_COORD)
	//	return (FALSE);
	//if (IPC_MMI::MAX_UNIT != IPC_LASER::MAX_UNIT)
	//	return (FALSE);
	//if (IPC_MMI::MAX_FIDUCIAL != IPC_LASER::MAX_FIDUCIAL)
	//	return (FALSE);
	//if (IPC_MMI::MAX_STEP != IPC_LASER::MAX_STEP)
	//	return (FALSE);

	// SeqVersion Update
	char buffer[128];
	sprintf(buffer, SEQ_VERSION_DATA);
	memcpy(&SEQ_VERSION.data, &buffer, sizeof(buffer));
	LOG[logSYS].Message("S/W Version : %s", SEQ_VERSION.data);

	OPR.isStop = (TRUE);
	OPR.isAuto = (FALSE);
	OPR.isAllHome = (FALSE);
	OPR.isDryRun = (FALSE);
	OPR.isPaused = (FALSE);
	NV.ndm[jobEnable] = 0;
	NV.ndm[scrAllHomeComp] = 0;

	OPR.isCheckPkg = (TRUE);
	OPR.isCheckSys = (TRUE);
	OPR.isCheckRcp = (TRUE);

	if (1 > NV.ndm[deviceCurGrpNo])
		NV.ndm[deviceCurGrpNo] = 1;
	if (1 > NV.ndm[deviceCurJobNo])
		NV.ndm[deviceCurJobNo] = 1;

	//FSM[fsmIndex1].Init(nullptr, nullptr, Index1FsmEvent);
	//FSM[fsmIndex2].Init(nullptr, nullptr, Index2FsmEvent);

	LAMPBUZZER.Init((LampConfig*)&NV.sys[twrLampR1],
		oTowerLampGreen, oTowerLampYellow, oTowerLampRed, oBuzzer01, oBuzzer02, oBuzzer03, oBuzzer04);
	TENKEYOPR.Init((PUSHORT)WMX.m_di[0].dataAddress, (PUSHORT)WMX.m_do[0].dataAddress);
	UPDATEMMI.Init(&NV.ndm[updateMtSeq2Mmi], &NV.ndm[updatePkgSeq2Mmi], &NV.ndm[updateRepSeq2Mmi],
		ER_MOTOR_UPDATE_TO_MMI_TIMEOVER, ER_MOTOR_UPDATE_TO_MMI_FAIL,
		ER_PKG_UPDATE_TO_MMI_TIMEOVER, ER_PKG_UPDATE_TO_MMI_FAIL,
		ER_RCP_UPDATE_TO_MMI_TIMEOVER, ER_RCP_UPDATE_TO_MMI_FAIL);

//	INDEX_LOCK.Init(nullptr, nullptr, IndexLockEvent);
	INDEX_LOCK = NOT_DEFINED;

	for (int i = 0; i < MAX_MESURETIME_CNT; i++)
	{
		MESURETIME[i].isStarted = 0;
		MESURETIME[i].setTime = 0;
		MESURETIME[i].getTime = 0;
		MESURETIME[i].timer.Reset();
	}

	IN_PK.Init();
	FOAM_RAIL.Init();
	FOAM_ELEV[enFoamRecvElev].Init(enFoamRecvElev);
	FOAM_ELEV[enFoamPartialElev].Init(enFoamPartialElev);
	FOAM_ELEV[enFoamSendElev].Init(enFoamSendElev);
	FOAM_STACK_CONV.Init();
	FOAM_PK.Init();
	FOAM_CST[enFoamBoxLt].Init(enFoamBoxLt);
	FOAM_CST[enFoamBoxRt].Init(enFoamBoxRt);
	TRAY_SHUTTLE.Init();
	MBB_SHUTTLE.Init();
	MBB_QC.Init();
	BTM_FOLD_CONV.Init();
	FOLD_PK.Init();
	LABEL_FEEDER[enlabelLt].Init(enlabelLt);
	LABEL_FEEDER[enlabelRt].Init(enlabelRt);
	LABEL_LASER.Init();
	LABEL_SHUTTLE[enlabelLt].Init(enlabelLt);
	LABEL_SHUTTLE[enlabelRt].Init(enlabelRt);
	MBB_STACK[enMbbCstLt].Init(enMbbCstLt);
	MBB_STACK[enMbbCstRt].Init(enMbbCstRt);
	LT_COBOT.Init();
	RT_COBOT.Init();

	//LASER_INDEX[Index1].Init(Index1);
	

	//double hwDist = 380.;//??
	//ANTICOLLISION[acSORT_ULDTRAY].Init(AntiCollision::DIR_RL, 380, OPR.isNoDevice,
	//	mtSORT_PICK_Y, &MT[mtSORT_PICK_Y].PosTable(SORT_PICK_Y_ANTICOLLISION),
	//	mtULD_TRAY_Y, &MT[mtULD_TRAY_Y].PosTable(ULD_TRAY_Y_GOOD_TRAY));

	//for (int i = 0; i < vacuumMAX; i++)
	//{
	//	NV.vacuumTable[i].actuatorNo = i;
	//	for (int j = 0; j < MAX_INDEX_VACUUM; j++)
	//	{
	//		NV.vacuumTable[i].valueHighOn[j] = 60;
	//		NV.vacuumTable[i].valueHighOff[j] = 60;
	//		NV.vacuumTable[i].valueLowOn[j] = 10;
	//		NV.vacuumTable[i].valueLowOff[j] = 10;
	//		NV.vacuumTable[i].valueBlowOn[j] = 15;
	//		NV.vacuumTable[i].delayTimeOn[j] = 100;
	//		NV.vacuumTable[i].delayTimeOff[j] = 100;
	//		NV.vacuumTable[i].delayTimeBlow[j] = 100;
	//	}
	//}
	//for (int i = 0; i < cylinderMAX; i++)
	//{
	//	NV.cylinderTable[i].actuatorNo = i;
	//	for (int j = 0; j < MAX_INDEX_VACUUM; j++)
	//	{
	//		NV.cylinderTable[i].delayTimeA[j] = 100;
	//		NV.cylinderTable[i].delayTimeB[j] = 100;
	//	}
	//}
	//ANTICOLLISION[acSORT_ULDTRAY].m_pMtX1 = &MT[mtSORT_PICK_Y];
	//ANTICOLLISION[acSORT_ULDTRAY].m_pMtX2 = &MT[mtULD_TRAY_Y];
	//ANTICOLLISION[acSORT_ULDTRAY].m_iBumpNo = iBUMP_SORT_PK;
	//ANTICOLLISION[acSORT_ULDTRAY].m_dMt1Pos = MT[mtSORT_PICK_Y].PosTable(SORT_PICK_Y_ANTICOLLISION);
	//ANTICOLLISION[acSORT_ULDTRAY].m_dMt2Pos = MT[mtULD_TRAY_Y].PosTable(ULD_TRAY_Y_GOOD_TRAY);
	//ANTICOLLISION[acSORT_ULDTRAY].m_dHwDist = 380.;
	//ANTICOLLISION[acSORT_ULDTRAY].m_baseDir = CAntiCollision::DIR_RL;
	//ANTICOLLISION[acSORT_ULDTRAY].skipAntiCollision = OPR.isNoDevice;
	return (TRUE);
}