#pragma once
#include "..\..\SEQ\Defines.h"
#include "..\..\BASE\BaseAll.h"
#include "..\..\BASE\Ini.h"

template <typename T, std::size_t LastValue>
class NameParser
{
private:
	std::string name;
	std::vector<std::pair<T, std::string>> list;

public:
	NameParser() : list(std::move(ToStrings<T, LastValue>())) {}
	~NameParser() = default;
	std::string& operator[](size_t index)
	{
		auto it = std::find_if(list.begin(), list.end(),
			[index](std::pair<T, std::string>& element) {
			return element.first == static_cast<T>(index);
		});

		name = "";
		if (it != list.end())
		{
			name = it->second;
			//std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
			//	return std::toupper(c);
			//});
			// djs Upper Delete
		}
		return name;
	}

	size_t size()
	{
		return list.size();
	}
};

template <typename T, std::size_t maxIndex>
void SetIndexName(AxisList mtno)
{
	NameParser<T, maxIndex>	names;
	for (int i = 0; i < maxIndex; i++)
	{
		CIni ini = { 0 };
		TCHAR strSection[100] = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\MotorIndexName.ini");
		swprintf_s(strSection, 100, L"INDEXNAME%d", mtno);
		swprintf_s(strKeyCode, 100, L"INDEXNAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(names[i].c_str()));
		ini.WriteString(strSection, strKeyCode, strStrName);
	}
}

template <typename T, std::size_t maxIndex>
void SetIndexName(RainbowRobotics::RobotList mtno)
{
	NameParser<T, maxIndex>	names;
	for (int i = 0; i < maxIndex; i++)
	{
		CIni ini = { 0 };
		TCHAR strSection[100] = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\RobotIndexName.ini");
		swprintf_s(strSection, 100, L"ROBOTINDEXNAME%d", mtno);
		swprintf_s(strKeyCode, 100, L"ROBOTINDEXNAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(names[i].c_str()));
		ini.WriteString(strSection, strKeyCode, strStrName);
	}
}

template <typename T, std::size_t maxIndex>
void SetIndexName(CylinderList cyno)
{
	NameParser<T, maxIndex>	names;
	for (int i = 0; i < maxIndex; i++)
	{
		CIni ini = { 0 };
		TCHAR strSection[100] = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\CylinderIndexName.ini");
		swprintf_s(strSection, 100, L"CYLINDERINDEXNAME%d", cyno);
		swprintf_s(strKeyCode, 100, L"CYLINDERINDEXNAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(names[i].c_str()));
		ini.WriteString(strSection, strKeyCode, strStrName);
	}
}

template <typename T, std::size_t maxIndex>
void SetIndexName(VacuumList vcno)
{
	NameParser<T, maxIndex>	names;
	for (int i = 0; i < maxIndex; i++)
	{
		CIni ini = { 0 };
		TCHAR strSection[100] = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VacuumIndexName.ini");
		swprintf_s(strSection, 100, L"VACUUMINDEXNAME%d", vcno);
		swprintf_s(strKeyCode, 100, L"VACUUMINDEXNAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(names[i].c_str()));
		ini.WriteString(strSection, strKeyCode, strStrName);
	}
}

template <typename T, std::size_t maxIndex>
void SetIndexName(VisionList vino)
{
	NameParser<T, maxIndex>	names;
	for (int i = 0; i < maxIndex; i++)
	{
		CIni ini = { 0 };
		TCHAR strSection[100] = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\VisionIndexName.ini");
		swprintf_s(strSection, 100, L"VISIONINDEXNAME%d", vino);
		swprintf_s(strKeyCode, 100, L"VISIONINDEXNAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(names[i].c_str()));
		ini.WriteString(strSection, strKeyCode, strStrName);
	}
}

template <typename T, std::size_t maxIndex>
void SetIndexName(LaserList lsno)
{
	NameParser<T, maxIndex>	names;
	for (int i = 0; i < maxIndex; i++)
	{
		CIni ini = { 0 };
		TCHAR strSection[100] = { 0 };
		TCHAR strKeyCode[100] = { 0 };
		TCHAR strStrName[100] = { 0 };
		ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\LaserIndexName.ini");
		swprintf_s(strSection, 100, L"LASERINDEXNAME%d", lsno);
		swprintf_s(strKeyCode, 100, L"LASERINDEXNAME%d", i);
		swprintf_s(strStrName, 100, L"%s", GetUniCode(names[i].c_str()));
		ini.WriteString(strSection, strKeyCode, strStrName);
	}
}

void InitIndexNameAxis();
void InitIndexNameRobot();
void InitIndexNameCylinder();
void InitIndexNameVacuum();
void RefactoringErrorCode(std::vector<std::pair<ErrName, std::string>> newErList,
	std::vector<std::pair<WrName, std::string>> newWrList);