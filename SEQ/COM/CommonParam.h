#pragma once
#include "..\Defines.h"
#include "..\..\BASE\BaseAll.h"
#include "..\HW\Cylinder.h"
#include "..\HW\Motor.h"
#include "..\HW\Vacuum.h"
#include "..\HW\Laser.h"
#include "..\SRC\IPC_MMI.h"
#include "..\SRC\IPC_LASER.h"

namespace COM
{
	typedef struct tagNvMemory
	{
		double					ddm[1000];
		INT32					ndm[1000];
		INT32					use[1000];
		double					pkg[1000];
		double					sys[1000];
		double					rcp[1000];
		IPC_MMI::MtTable		motTable[200];
		IPC_MMI::VacTable		vacTable[200];
		IPC_MMI::CylTable		cylTable[200];
	}NvMemory;

	class ParamName
	{
	public:
		List<string>			pkg;
		List<string>			ddm;
		List<string>			ndm;
		List<string>			use;
		List<string>			sys;
		List<string>			rcp;

		ParamName()
		{
			for (int i = 0; i < 1000; i++)
			{
				pkg.Add("");
				ddm.Add("");
				ndm.Add("");
				use.Add("");
				sys.Add("");
				rcp.Add("");
			}

			BOOL CanWrite = TRUE;
			std::filesystem::path createFile;
			createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\PkgName.ini";
			CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
			createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\DParam.ini";
			CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
			createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\NParam.ini";
			CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
			createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\UseSkip.ini";
			CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
			createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\SysParam.ini";
			CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;
			createFile = "C:\\KOSES\\MMI\\Ini\\STANDARD\\RcpName.ini";
			CanWrite &= std::filesystem::exists(createFile) ? TRUE : FALSE;

			if (!CanWrite)
				return;

			CIni ini = { 0 };
			TCHAR strKeyCode[1024] = { 0 };
			TCHAR strStrName[1024] = { 0 };
			for (int i = 0; i < PKG_MAX; i++)
			{
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\PkgName.ini");
				swprintf_s(strKeyCode, 100, L"NAME%d", i);
				ini.GetString(L"PKGNAME", strKeyCode, strStrName, 512);
				pkg[i] = GetAscCode(strStrName);
			}
			for (int i = 0; i < DDM_MAX; i++)
			{
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\DParam.ini");
				swprintf_s(strKeyCode, 100, L"NAME%d", i);
				ini.GetString(L"DPARAM", strKeyCode, strStrName, 512);
				ddm[i] = GetAscCode(strStrName);
			}
			for (int i = 0; i < NDM_MAX; i++)
			{
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\NParam.ini");
				swprintf_s(strKeyCode, 100, L"NAME%d", i);
				ini.GetString(L"NPARAM", strKeyCode, strStrName, 512);
				ndm[i] = GetAscCode(strStrName);
			}
			for (int i = 0; i < maxUSE; i++)
			{
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\UseSkip.ini");
				swprintf_s(strKeyCode, 100, L"NAME%d", i);
				ini.GetString(L"USESKIPNAME", strKeyCode, strStrName, 512);
				use[i] = GetAscCode(strStrName);
			}
			for (int i = 0; i < maxSYS; i++)
			{
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\SysParam.ini");
				swprintf_s(strKeyCode, 100, L"NAME%d", i);
				ini.GetString(L"DPARAM", strKeyCode, strStrName, 512);
				sys[i] = GetAscCode(strStrName);
			}
			for (int i = 0; i < maxRCP; i++)
			{
				ZeroMemory(strKeyCode, sizeof(strKeyCode));
				ZeroMemory(strStrName, sizeof(strStrName));
				ini.SetPathName(L"C:\\KOSES\\MMI\\Ini\\STANDARD\\RcpName.ini");
				swprintf_s(strKeyCode, 100, L"NAME%d", i);
				ini.GetString(L"RCPNAME", strKeyCode, strStrName, 512);
				rcp[i] = GetAscCode(strStrName);
			}
		}
		~ParamName()
		{

		}
	};
}

extern COM::ParamName PARAM_NAME;
extern CNvm NV_SHARED_MEM;
extern CNvm NV_MEMORY;
extern IPC_LASER::SharedMem& SHARED_MEM;
extern COM::NvMemory& NV;
