#include "CppSQLite3Statement.h"
#include "core/CppSQLiteLib/src/CppSQLite3Execption.h"

NS_BEGIN(TA_Base_Core)

CppSQLite3Statement::CppSQLite3Statement()
{
	mpDB = 0;
	mpVM = 0;
}


CppSQLite3Statement::CppSQLite3Statement(const CppSQLite3Statement& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
}


CppSQLite3Statement::CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM)
{
	mpDB = pDB;
	mpVM = pVM;
}


CppSQLite3Statement::~CppSQLite3Statement()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}


CppSQLite3Statement& CppSQLite3Statement::operator=(const CppSQLite3Statement& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3Statement&>(rStatement).mpVM = 0;
	return *this;
}


int CppSQLite3Statement::execDML()
{
	checkDB();
	checkVM();

	const char* szError=0;

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = sqlite3_changes(mpDB);

		nRet = sqlite3_reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}

		return nRowsChanged;
	}
	else
	{
		nRet = sqlite3_reset(mpVM);
		szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}


CppSQLite3Query CppSQLite3Statement::execQuery()
{
	checkDB();
	checkVM();

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(mpDB, mpVM, true/*eof*/, false);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(mpDB, mpVM, false/*eof*/, false);
	}
	else
	{
		nRet = sqlite3_reset(mpVM);
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::bind(int nParam, const char* szValue)
{
	checkVM();
	int nRes = sqlite3_bind_text(mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
			"Error binding string param",
			DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::bind(int nParam, const int nValue)
{
	checkVM();
	int nRes = sqlite3_bind_int(mpVM, nParam, nValue);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
			"Error binding int param",
			DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::bind(int nParam, const double dValue)
{
	checkVM();
	int nRes = sqlite3_bind_double(mpVM, nParam, dValue);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
			"Error binding double param",
			DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::bind(int nParam, const unsigned char* blobValue, int nLen)
{
	checkVM();
	int nRes = sqlite3_bind_blob(mpVM, nParam,
		(const void*)blobValue, nLen, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
			"Error binding blob param",
			DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::bindNull(int nParam)
{
	checkVM();
	int nRes = sqlite3_bind_null(mpVM, nParam);

	if (nRes != SQLITE_OK)
	{
		throw CppSQLite3Exception(nRes,
			"Error binding NULL param",
			DONT_DELETE_MSG);
	}
}


int CppSQLite3Statement::bindParameterIndex(const char* szParam)
{
	checkVM();

	int nParam = sqlite3_bind_parameter_index(mpVM, szParam);

	int nn = sqlite3_bind_parameter_count(mpVM);
	const char* sz1 = sqlite3_bind_parameter_name(mpVM, 1);
	const char* sz2 = sqlite3_bind_parameter_name(mpVM, 2);

	if (!nParam)
	{
		char buf[128];
		sprintf(buf, "Parameter '%s' is not valid for this statement", szParam);
		throw CppSQLite3Exception(CPPSQLITE_ERROR, buf, DONT_DELETE_MSG);
	}

	return nParam;
}


void CppSQLite3Statement::bind(const char* szParam, const char* szValue)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, szValue);
}


void CppSQLite3Statement::bind(const char* szParam, const int nValue)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, nValue);
}

void CppSQLite3Statement::bind(const char* szParam, const double dwValue)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, dwValue);
}

void CppSQLite3Statement::bind(const char* szParam, const unsigned char* blobValue, int nLen)
{
	int nParam = bindParameterIndex(szParam);
	bind(nParam, blobValue, nLen);
}


void CppSQLite3Statement::bindNull(const char* szParam)
{
	int nParam = bindParameterIndex(szParam);
	bindNull(nParam);
}


void CppSQLite3Statement::reset()
{
	if (mpVM)
	{
		int nRet = sqlite3_reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Statement::finalize()
{
	if (mpVM)
	{
		int nRet = sqlite3_finalize(mpVM);
		mpVM = 0;

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(mpDB);
			throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}


void CppSQLite3Statement::checkDB()
{
	if (mpDB == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Database not open",
			DONT_DELETE_MSG);
	}
}


void CppSQLite3Statement::checkVM()
{
	if (mpVM == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Null Virtual Machine pointer",
			DONT_DELETE_MSG);
	}
}



NS_END(TA_Base_Core)



