#include "CppSQLite3Buffer.h"
#include "core/CppSQLiteLib/src/CppSQLite3Execption.h"

NS_BEGIN(TA_Base_Core)


CppSQLite3Buffer::CppSQLite3Buffer()
{
	mpBuf = 0;
}


CppSQLite3Buffer::~CppSQLite3Buffer()
{
	clear();
}


void CppSQLite3Buffer::clear()
{
	if (mpBuf)
	{
		sqlite3_free(mpBuf);
		mpBuf = 0;
	}

}


const char* CppSQLite3Buffer::format(const char* szFormat, ...)
{
	clear();
	va_list va;
	va_start(va, szFormat);
	mpBuf = sqlite3_vmprintf(szFormat, va);
	va_end(va);
	return mpBuf;
}


NS_END(TA_Base_Core)



