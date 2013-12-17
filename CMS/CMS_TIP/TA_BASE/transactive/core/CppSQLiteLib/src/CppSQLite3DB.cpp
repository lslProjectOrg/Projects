#include "CppSQLite3DB.h"
#include "core/CppSQLiteLib/src/CppSQLite3Execption.h"

NS_BEGIN(TA_Base_Core)


CppSQLite3DB::CppSQLite3DB()
{
	m_pDB = 0;
	m_nBusyTimeoutMs = 60000; // 60 seconds
}


CppSQLite3DB::CppSQLite3DB(const CppSQLite3DB& db)
{
	m_pDB = db.m_pDB;
	m_nBusyTimeoutMs = 60000; // 60 seconds
}


CppSQLite3DB::~CppSQLite3DB()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
}


CppSQLite3DB& CppSQLite3DB::operator=(const CppSQLite3DB& db)
{
	m_pDB = db.m_pDB;
	m_nBusyTimeoutMs = 60000; // 60 seconds
	return *this;
}


void CppSQLite3DB::open(const char* szFile)
{
	int nRet = sqlite3_open(szFile, &m_pDB);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}

	setBusyTimeout(m_nBusyTimeoutMs);
}


void CppSQLite3DB::close()
{
	if (m_pDB)
	{
		if (sqlite3_close(m_pDB) == SQLITE_OK)
		{
			m_pDB = 0;
		}
		else
		{
			throw CppSQLite3Exception(CPPSQLITE_ERROR,
				"Unable to close database",
				DONT_DELETE_MSG);
		}
	}
}


CppSQLite3Statement CppSQLite3DB::compileStatement(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);
	return CppSQLite3Statement(m_pDB, pVM);
}


bool CppSQLite3DB::tableExists(const char* szTable)
{
	char szSQL[DEF_INT_BUFFSIZE];
	memset(szSQL, 0, DEF_INT_BUFFSIZE);
	sprintf(szSQL,
		"select count(*) from sqlite_master where type='table' and name='%s'",
		szTable);
	int nRet = execScalar(szSQL);
	return (nRet > 0);
}


int CppSQLite3DB::execDML(const char* szSQL)
{
	checkDB();

	char* szError=0;

	int nRet = sqlite3_exec(m_pDB, szSQL, 0, 0, &szError);

	if (nRet == SQLITE_OK)
	{
		return sqlite3_changes(m_pDB);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError);
	}
}


CppSQLite3Query CppSQLite3DB::execQuery(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);

	int nRet = sqlite3_step(pVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CppSQLite3Query(m_pDB, pVM, true/*eof*/);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CppSQLite3Query(m_pDB, pVM, false/*eof*/);
	}
	else
	{
		nRet = sqlite3_finalize(pVM);
		const char* szError= sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}



CppSQLite3Query* CppSQLite3DB::execQueryEx(const char* szSQL)
{
	CppSQLite3Query* pQuery = NULL;
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);

	int nRet = sqlite3_step(pVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		pQuery = new CppSQLite3Query(m_pDB, pVM, true/*eof*/);
		return pQuery;
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		pQuery = new CppSQLite3Query(m_pDB, pVM, false/*eof*/);
		return pQuery;
	}
	else
	{
		pQuery = NULL;
		nRet = sqlite3_finalize(pVM);
		const char* szError= sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}


int CppSQLite3DB::execScalar(const char* szSQL, int nNullValue/*=0*/)
{
	CppSQLite3Query q = execQuery(szSQL);

	if (q.eof() || q.numFields() < 1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid scalar query",
			DONT_DELETE_MSG);
	}

	return q.getIntField(0, nNullValue);
}


CppSQLite3Table CppSQLite3DB::getTable(const char* szSQL)
{
	checkDB();

	char* szError=0;
	char** paszResults=0;
	int nRet;
	int nRows(0);
	int nCols(0);

	nRet = sqlite3_get_table(m_pDB, szSQL, &paszResults, &nRows, &nCols, &szError);

	if (nRet == SQLITE_OK)
	{
		return CppSQLite3Table(paszResults, nRows, nCols);
	}
	else
	{
		throw CppSQLite3Exception(nRet, szError);
	}
}


sqlite_int64 CppSQLite3DB::lastRowId()
{
	return sqlite3_last_insert_rowid(m_pDB);
}


void CppSQLite3DB::setBusyTimeout(int nMillisecs)
{
	m_nBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(m_pDB, m_nBusyTimeoutMs);
}


void CppSQLite3DB::checkDB()
{
	if (!m_pDB)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Database not open",
			DONT_DELETE_MSG);
	}
}


sqlite3_stmt* CppSQLite3DB::compile(const char* szSQL)
{
	checkDB();

	const char* szTail=0;
	sqlite3_stmt* pVM;

	int nRet = sqlite3_prepare_v2(m_pDB, szSQL, -1, &pVM, &szTail);

	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CppSQLite3Exception(nRet,
			(char*)szError,
			DONT_DELETE_MSG);
	}

	return pVM;
}

bool CppSQLite3DB::IsAutoCommitOn()
{
	checkDB();
	return sqlite3_get_autocommit(m_pDB) ? true : false;
}


NS_END(TA_Base_Core)



