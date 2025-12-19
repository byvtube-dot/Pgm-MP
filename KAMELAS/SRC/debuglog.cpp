#include "debuglog.h"
#include <tchar.h>

namespace kamelaslib
{

void DebugWrite(INT32 , const TCHAR* lpMessage)
{
	::OutputDebugString(lpMessage);
#ifdef _MSC_VER
	::OutputDebugString( _TEXT("\n"));
#endif
}

void DebugLog(INT32 code, TCHAR* lpMessage, ...)
{
#ifdef _DEBUG
	static TCHAR buffer[1027*4];
	va_list ap;
	va_start(ap, lpMessage);
	_vstprintf(buffer, lpMessage, ap);
	va_end(ap);

	DebugWrite(code, buffer);
#endif
}

}
