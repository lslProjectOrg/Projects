//////////////////////////////////////////////////////////////////////
///		@file		SQLFileHelper.cpp
///		@author		zhilin,ouyang
///		@date		2010-12-9 14:44:41
///
///		@brief	    to load the sql.dat file and store the SQL statements
///                 into a hash-table for later retrieve.
//////////////////////////////////////////////////////////////////////
#include "SQLFileHelper.h"
#include "core/exceptions/src/SQLException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/uuid/src/TAuuid.h"
#include "SQLUtilityFun.h"
#include <fstream>
#include <time.h>


#include "sql.h"


#define  defSQLFileName        "sql.dat"
#define  defSQLTempFilePre     "sql_"
#define  defSQLTempFileEnd     ".txt"
#define  defSQLCommonSection   "[[common]]"
#define  defSQLOracleSection   "[[oracle]]"
#define  defSQLMySQLSection    "[[mysql]]"
#define  defSQLSqliteSection   "[[sqlite]]"

#define  defSectionWordLen      20

NS_BEGIN(TA_Base_Core)

SQLFileHelper::SQLFileHelper() 
{
	m_strSQLFile = defSQLFileName;

	m_nArrSize = sizeof(s_SQLArray) / sizeof (*s_SQLArray);

	_InitSQLHashIndex();
}

SQLFileHelper::SQLFileHelper(const char* pszSQLFile)
{
	m_strSQLFile = pszSQLFile;
	m_nArrSize = sizeof(s_SQLArray) / sizeof (*s_SQLArray);

	_InitSQLHashIndex();
}

SQLFileHelper::~SQLFileHelper()
{

	m_SQLHashTable.clear();
	m_hSQLHashIndex.clear();
}

std::string SQLFileHelper::getSQLString(u_long uSQLID)
{
	FUNCTION_ENTRY("SQLFileHelper::GetSQLString(u_long uSQLID)");		

	FUNCTION_EXIT;
	return m_SQLHashTable[uSQLID];
}

void  SQLFileHelper::getSQLString(const std::string& strSQLKey, SQLStatement& rSqlStatements)
{
	FUNCTION_ENTRY("SQLFileHelper::getSQLStringEx()");	

	if (m_hSQLHashIndex.empty())
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn,
			"SQLFileHelper::GetSQLString() The SQL hash table is empty when load strSQLKey: %s", strSQLKey.c_str());
		_InitSQLHashIndex();
	}

	int nIndex = -1;
	//nIndex = m_hSQLHashIndex[strSQLKey];
	strSQLIndexIterT iterFound = m_hSQLHashIndex.find(strSQLKey);
	if (iterFound == m_hSQLHashIndex.end())
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError,
			"SQLFileHelper::GetSQLString() the SQLKey cannot match in Hash table. strSQLKey: %s", strSQLKey.c_str());
		TA_THROW(BadParamCount("the SQLKey cannot match in Hash table"));
	}		

	nIndex = iterFound->second;

	rSqlStatements.strSQLID  = strSQLKey;
	rSqlStatements.strCommonSQL = s_SQLArray[nIndex].pCommonSQL;
	rSqlStatements.strOracleSQL  = s_SQLArray[nIndex].pOracleSQL;
	rSqlStatements.strMySQLSQL  = s_SQLArray[nIndex].pMySQLSQL;
	rSqlStatements.strSQLiteSQL = s_SQLArray[nIndex].pSQLiteSQL;

	FUNCTION_EXIT;
}

void   SQLFileHelper::_InitSQLHashIndex()
{
	std::string  strSQLKey;

	for (int nIndex = 0; nIndex < m_nArrSize; nIndex++)
	{
		strSQLKey = s_SQLArray[nIndex].pSQLID;
		m_hSQLHashIndex.insert(strSQLHashIndexValT(strSQLKey, nIndex));
	}
}


NS_END(TA_Base_Core)