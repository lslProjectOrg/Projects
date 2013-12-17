#include "CppSQLite3Query.h"
#include "core/CppSQLiteLib/src/CppSQLite3Execption.h"

NS_BEGIN(TA_Base_Core)

////////////////////////////////////////////////////////////////////////////////

CppSQLite3Query::CppSQLite3Query()
{
	mpVM = 0;
	mnCols = 0;
	mbOwnVM = false;
	mbEof = true;
}


CppSQLite3Query::CppSQLite3Query(const CppSQLite3Query& rQuery)
{
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
}


CppSQLite3Query::CppSQLite3Query(sqlite3* pDB,
								 sqlite3_stmt* pVM,
								 bool bEof,
								 bool bOwnVM/*=true*/)
{
	mpDB = pDB;
	mpVM = pVM;
	mbEof = bEof;
	mnCols = sqlite3_column_count(mpVM);
	mbOwnVM = bOwnVM;
}


CppSQLite3Query::~CppSQLite3Query()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}


CppSQLite3Query& CppSQLite3Query::operator=(const CppSQLite3Query& rQuery)
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
	mpVM = rQuery.mpVM;
	// Only one object can own the VM
	const_cast<CppSQLite3Query&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
	return *this;
}


int CppSQLite3Query::numFields()
{
	checkVM();
	return mnCols;
}


const char* CppSQLite3Query::fieldValue(int nField)
{
	checkVM();

	if (nField < 0 || nField > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return (const char*)sqlite3_column_text(mpVM, nField);
}


const char* CppSQLite3Query::fieldValue(const char* szField)
{
	int nField = fieldIndex(szField);
	return (const char*)sqlite3_column_text(mpVM, nField);
}


int CppSQLite3Query::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int(mpVM, nField);
	}
}


int CppSQLite3Query::getIntField(const char* szField, int nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}


sqlite_int64 CppSQLite3Query::getInt64Field(int nField, sqlite_int64 nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int64(mpVM, nField);
	}
}


sqlite_int64 CppSQLite3Query::getInt64Field(const char* szField, sqlite_int64 nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, nNullValue);
}


double CppSQLite3Query::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return sqlite3_column_double(mpVM, nField);
	}
}


double CppSQLite3Query::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	int nField = fieldIndex(szField);
	return getFloatField(nField, fNullValue);
}


const char* CppSQLite3Query::getStringField(int nField, const char* szNullValue/*=""*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (const char*)sqlite3_column_text(mpVM, nField);
	}
}


const char* CppSQLite3Query::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}


const unsigned char* CppSQLite3Query::getBlobField(int nField, int& nLen)
{
	checkVM();

	if (nField < 0 || nField > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	nLen = sqlite3_column_bytes(mpVM, nField);
	return (const unsigned char*)sqlite3_column_blob(mpVM, nField);
}


const unsigned char* CppSQLite3Query::getBlobField(const char* szField, int& nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}


bool CppSQLite3Query::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}


bool CppSQLite3Query::fieldIsNull(const char* szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}


int CppSQLite3Query::fieldIndex(const char* szField)
{
	checkVM();

	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			const char* szTemp = sqlite3_column_name(mpVM, nField);

			if (strcmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
		"Invalid field name requested",
		DONT_DELETE_MSG);
}


const char* CppSQLite3Query::fieldName(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_name(mpVM, nCol);
}


const char* CppSQLite3Query::fieldDeclType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_decltype(mpVM, nCol);
}


int CppSQLite3Query::fieldDataType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_type(mpVM, nCol);
}


bool CppSQLite3Query::eof()
{
	checkVM();
	return mbEof;
}


void CppSQLite3Query::nextRow()
{
	checkVM();

	int nRet = sqlite3_step(mpVM);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		mbEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = sqlite3_finalize(mpVM);
		mpVM = 0;
		const char* szError = sqlite3_errmsg(mpDB);
		throw CppSQLite3Exception(nRet,
			(char*)szError,
			DONT_DELETE_MSG);
	}
}


void CppSQLite3Query::finalize()
{
	if (mpVM && mbOwnVM)
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


void CppSQLite3Query::checkVM()
{
	if (mpVM == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Null Virtual Machine pointer",
			DONT_DELETE_MSG);
	}
}



NS_END(TA_Base_Core)



