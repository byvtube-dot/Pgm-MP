#include "SRC\Parser.h"

static const BOOL refactoringErrorCode = FALSE;

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hOpenEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"KOSES_SEQUENCE");
	if (nullptr != hOpenEvent)
		return 1;

	HANDLE hCreateEvent = CreateEvent(NULL, FALSE, FALSE, L"KOSES_PARSER");
	if (nullptr == hCreateEvent)
		return 2;
	
	CreateDirectory(L"C:\\KOSES", NULL);
	CreateDirectory(L"C:\\KOSES\\SEQ", NULL);
	CreateDirectory(L"C:\\KOSES\\SEQ\\DAT", NULL);
	CreateDirectory(L"C:\\KOSES\\MMI", NULL);
	CreateDirectory(L"C:\\KOSES\\MMI\\Ini", NULL);
	CreateDirectory(L"C:\\KOSES\\MMI\\Ini\\STANDARD", NULL);

	std::ofstream file;
	std::filesystem::path createFile;
	std::filesystem::path deleteFile;

	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 4;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 4;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\ConveyorName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 4;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 4;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 4;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\SensorName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 5;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\PkgName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 6;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\DParam.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 7;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\NParam.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 8;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\UseSkip.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 9;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\SysParam.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 10;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\RcpName.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 11;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 12;
	}
	file.close();
	createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\Warning.ini";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return 13;
	}
	file.close();

	NameParser<AxisList, motorMAX>						motName;
	NameParser<ConveyorList, conveyorMAX>				convName;
	NameParser<RainbowRobotics::RobotList, RainbowRobotics::robotMAX> robotName;
	NameParser<CylinderList, cylinderMAX>				cylName;
	NameParser<VacuumList, vacuumMAX>					vacName;
	NameParser<VisionList, VI_MAX>						visName;
	NameParser<LaserList, laserMAX>						lasName;
	NameParser<enDI, diLAST>							diName;
	NameParser<enDO, doLAST>							doName;
	NameParser<enAI, aiLAST>							aiName;
	NameParser<enAO, aoLAST>							aoName;
	NameParser<PkgList, PKG_MAX>						pkgName;
	NameParser<DDmList, DDM_MAX>						ddmName;
	NameParser<NDmList, NDM_MAX>						ndmName;
	NameParser<UseList, maxUSE>							useName;
	NameParser<SysList, maxSYS>							sysName;
	NameParser<RcpList, maxRCP>							rcpName;

	for (int i = 0; i < motorMAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorName.ini");
		swprintf_s(strKeyCode, 100, L"AXIS%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(motName[i].c_str()));
		ini.WriteString(L"AXISNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < conveyorMAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\ConveyorName.ini");
		swprintf_s(strKeyCode, 100, L"CONVEYOR%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(convName[i].c_str()));
		ini.WriteString(L"CONVEYORNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < RainbowRobotics::robotMAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotName.ini");
		swprintf_s(strKeyCode, 100, L"ROBOT%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(robotName[i].c_str()));
		ini.WriteString(L"ROBOTNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < cylinderMAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderName.ini");
		swprintf_s(strKeyCode, 100, L"CYLINDER%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(cylName[i].c_str()));
		ini.WriteString(L"CYLINDERNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < vacuumMAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumName.ini");
		swprintf_s(strKeyCode, 100, L"VACUUM%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(vacName[i].c_str()));
		ini.WriteString(L"VACUUMNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < VI_MAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VisionName.ini");
		swprintf_s(strKeyCode, 100, L"VISION%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(visName[i].c_str()));
		ini.WriteString(L"VISIONNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < laserMAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\LaserName.ini");
		swprintf_s(strKeyCode, 100, L"LASER%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(lasName[i].c_str()));
		ini.WriteString(L"LASERNAME", strKeyCode, strStrName);
	}
	for (int ch = 0; ch < ((enDI::diLAST != 0) ? ((enDI::diLAST / 100) + 1) : 0); ch++)
	{
		for (int no = 0; no < MAX_CONTACT; no++)
		{
			int ioNo = (ch * 100) + no;
			CIni ini = { 0 };
			TCHAR strSection[100] = { 0 };
			TCHAR strKeyCode[100] = { 0 };
			TCHAR strStrName[100] = { 0 };
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\SensorName.ini");
			swprintf_s(strSection, 100, L"INPUT_CHANNEL_%d", ch);
			swprintf_s(strKeyCode, 100, L"NO_%d", no);
			swprintf_s(strStrName, 100, L"%s", GetUniCode(diName[ioNo].c_str()));
			ini.WriteString(strSection, strKeyCode, strStrName);
		}
	}
	for (int ch = 0; ch < ((enDO::doLAST != 0) ? ((enDO::doLAST / 100) + 1) : 0); ch++)
	{
		for (int no = 0; no < MAX_CONTACT; no++)
		{
			int ioNo = (ch * 100) + no;
			CIni ini = { 0 };
			TCHAR strSection[100] = { 0 };
			TCHAR strKeyCode[100] = { 0 };
			TCHAR strStrName[100] = { 0 };
			ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\SensorName.ini");
			swprintf_s(strSection, 100, L"OUTPUT_CHANNEL_%d", ch);
			swprintf_s(strKeyCode, 100, L"NO_%d", no);
			swprintf_s(strStrName, 100, L"%s", GetUniCode(doName[ioNo].c_str()));
			ini.WriteString(strSection, strKeyCode, strStrName);
		}
	}
	for (int i = 0; i < PKG_MAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\PkgName.ini");
		swprintf_s(strKeyCode, 100, L"NAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(pkgName[i].c_str()));
		ini.WriteString(L"PKGNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < DDM_MAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\DParam.ini");
		swprintf_s(strKeyCode, 100, L"NAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(ddmName[i].c_str()));
		ini.WriteString(L"DPARAM", strKeyCode, strStrName);
	}
	for (int i = 0; i < NDM_MAX; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\NParam.ini");
		swprintf_s(strKeyCode, 100, L"NAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(ndmName[i].c_str()));
		ini.WriteString(L"NPARAM", strKeyCode, strStrName);
	}
	for (int i = 0; i < maxUSE; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\UseSkip.ini");
		swprintf_s(strKeyCode, 100, L"NAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(useName[i].c_str()));
		ini.WriteString(L"USESKIPNAME", strKeyCode, strStrName);
	}
	for (int i = 0; i < maxSYS; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\SysParam.ini");
		swprintf_s(strKeyCode, 100, L"NAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(sysName[i].c_str()));
		ini.WriteString(L"DPARAM", strKeyCode, strStrName);
	}
	for (int i = 0; i < maxRCP; i++)
	{
		CIni ini = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\RcpName.ini");
		swprintf_s(strKeyCode, 100, L"NAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(rcpName[i].c_str()));
		ini.WriteString(L"RCPNAME", strKeyCode, strStrName);
	}

	std::vector<std::pair<ErrName, std::string>>		orgErList = ToStrings<ErrName, ER_MAX + 1>();
	std::vector<std::pair<ErrName, std::string>>		newErList;
	for (int i = 0; i < orgErList.size(); i++)
	{
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		int nNo = orgErList[i].first;
		CIni ini = { 0 };

		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini");
		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		ZeroMemory(strStrName, sizeof(strStrName));
		newErList.emplace_back(orgErList[i].first, orgErList[i].second);
		swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second).c_str()));
		ini.WriteString(L"NAME", strKeyCode, strStrName);

		//switch (nNo)
		//{
		//case ER_MT_ALARM:
		//case ER_MT_SERVO_OFF:
		//case ER_MT_HW_LIMIT:
		//case ER_MT_SW_LIMIT:
		//case ER_MT_RESUME:
		//case ER_MT_POS_CMD:
		//case ER_MT_VEL_CMD:
		//case ER_MT_ACC_CMD:
		//case ER_MT_MOTION:
		//case ER_MT_MODE:
		//case ER_MT_HOME_TMOVER:
		//case ER_MT_CMD_READY:
		//{
		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini");
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	newErList.emplace_back(orgErList[i].first, orgErList[i].second);
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	for (int no = 0; no < motorMAX; no++)
		//	{
		//		if ("" == motName[no])
		//			continue;

		//		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//		ZeroMemory(strStrName, sizeof(strStrName));
		//		newErList.emplace_back(ErrName(nNo + no + 1), std::string(orgErList[i].second + "_" + motName[no]));
		//		swprintf_s(strKeyCode, 100, L"NAME%d", nNo + no + 1);
		//		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second + "_" + motName[no]).c_str()));
		//		ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	}
		//}
		//break;
		//case ER_MT_ALL_HOME:
		//{
		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini");
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	newErList.emplace_back(orgErList[i].first, orgErList[i].second);
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	for (int no = 0; no < motorMAX; no++)
		//	{
		//		if ("" == motName[no])
		//			continue;

		//		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//		ZeroMemory(strStrName, sizeof(strStrName));
		//		newErList.emplace_back(ErrName(nNo + no + 1), std::string(orgErList[i].second + "_" + motName[no]));
		//		swprintf_s(strKeyCode, 100, L"NAME%d", nNo + no + 1);
		//		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second + "_" + motName[no]).c_str()));
		//		ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	}
		//}
		//break;
		//case ER_CYL_ALARM:
		//{
		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini");
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	newErList.emplace_back(orgErList[i].first, orgErList[i].second);
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	for (int no = 0; no < cylinderMAX; no++)
		//	{
		//		if ("" == cylName[no])
		//			continue;

		//		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//		ZeroMemory(strStrName, sizeof(strStrName));
		//		newErList.emplace_back(ErrName(nNo + no + 1), std::string(orgErList[i].second + "_" + cylName[no]));
		//		swprintf_s(strKeyCode, 100, L"NAME%d", nNo + no + 1);
		//		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second + "_" + cylName[no]).c_str()));
		//		ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	}
		//}
		//break;
		//case ER_VAC_ALARM:
		//{
		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini");
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	newErList.emplace_back(orgErList[i].first, orgErList[i].second);
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	for (int no = 0; no < vacuumMAX; no++)
		//	{
		//		if ("" == vacName[no])
		//			continue;

		//		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//		ZeroMemory(strStrName, sizeof(strStrName));
		//		newErList.emplace_back(ErrName(nNo + no + 1), std::string(orgErList[i].second + "_" + vacName[no]));
		//		swprintf_s(strKeyCode, 100, L"NAME%d", nNo + no + 1);
		//		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second + "_" + vacName[no]).c_str()));
		//		ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	}
		//}
		//break;
		//default:
		//{
		//	if (ER_MT_ALARM < nNo && (ER_VAC_ALARM + 200) > nNo)
		//		continue;

		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Alarm.ini");
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	newErList.emplace_back(orgErList[i].first, orgErList[i].second);
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgErList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//}
		//break;
		//}
	}

	std::vector<std::pair<WrName, std::string>>		orgWrList = ToStrings<WrName, WR_MAX + 1>();
	std::vector<std::pair<WrName, std::string>>		newWrList;
	for (int i = 0; i < orgWrList.size(); i++)
	{
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		int nNo = orgWrList[i].first;
		CIni ini = { 0 };

		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Warning.ini");
		newWrList.emplace_back(orgWrList[i].first, orgWrList[i].second);
		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		ZeroMemory(strStrName, sizeof(strStrName));
		swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgWrList[i].second).c_str()));
		ini.WriteString(L"NAME", strKeyCode, strStrName);

		//switch (nNo)
		//{
		//case WR_MT_PAUSE:
		//{
		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Warning.ini");
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	newWrList.emplace_back(orgWrList[i].first, orgWrList[i].second);
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgWrList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	for (int no = 0; no < motorMAX; no++)
		//	{
		//		if ("" == motName[no])
		//			continue;

		//		ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//		ZeroMemory(strStrName, sizeof(strStrName));
		//		newWrList.emplace_back(WrName(nNo + no + 1), std::string(orgWrList[i].second + "_" + motName[no]));
		//		swprintf_s(strKeyCode, 100, L"NAME%d", nNo + no + 1);
		//		swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgWrList[i].second + "_" + motName[no]).c_str()));
		//		ini.WriteString(L"NAME", strKeyCode, strStrName);
		//	}
		//}
		//break;
		//default:
		//{
		//	if (WR_MT_PAUSE < nNo && (WR_MT_PAUSE + 200) > nNo)
		//		continue;

		//	ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\Warning.ini");
		//	newWrList.emplace_back(orgWrList[i].first, orgWrList[i].second);
		//	ZeroMemory(strKeyCode, sizeof(strKeyCode));
		//	ZeroMemory(strStrName, sizeof(strStrName));
		//	swprintf_s(strKeyCode, 100, L"NAME%d", nNo);
		//	swprintf_s(strStrName, 100, L"%s", GetUniCode(std::string(orgWrList[i].second).c_str()));
		//	ini.WriteString(L"NAME", strKeyCode, strStrName);
		//}
		//break;
		//}
	}

	InitIndexNameAxis();
	InitIndexNameRobot();
	InitIndexNameCylinder();
	InitIndexNameVacuum();
	if (refactoringErrorCode)
		RefactoringErrorCode(newErList, newWrList);
	
	std::filesystem::path createPath = std::filesystem::current_path();
	deleteFile = createPath.parent_path() / "LIB" / "DELETE.exe";
	if (!std::filesystem::exists(deleteFile) || !ExecuteFile(deleteFile.wstring()))
	{
		std::wcout << L"파일이 존재하지 않습니다: " << deleteFile.c_str() << std::endl;
		return 101;
	}
	return 0;
}