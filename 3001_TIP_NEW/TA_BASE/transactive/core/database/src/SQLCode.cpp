//////////////////////////////////////////////////////////////////////
///		@file		SQLCode.cpp
///		@author		zhilin,ouyang
///		@date		2010-11-29 14:44:41
///
///		@brief	    to hold all of the SQL statements used in our 
///                 system in one location. The SQLCode class is based 
///                 on the Singleton design pattern.
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "SQLCode.h"
#include "SQLFileHelper.h"
#include "core/exceptions/src/SQLException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/RunParams.h"

NS_BEGIN(TA_Base_Core)



SQLCode* SQLCode::m_pInstance = 0;
NonReEntrantThreadLockable SQLCode::m_instanceLock;

SQLCode& SQLCode::getInstance()
{
	FUNCTION_ENTRY("SQLCode::GetInstance()");
	 ThreadGuard guard( m_instanceLock );
	if (NULL == m_pInstance)
	{
		m_pInstance = new SQLCode();
	}

	FUNCTION_EXIT;
	return (*m_pInstance);
}

void SQLCode::removeInstance()
{
	if ( NULL != m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


SQLCode::SQLCode()
{
	LOG0(SourceInfo, DebugUtil::DebugSQL,"Begin build the Hash table...");

	m_pSqlFileHelper = new SQLFileHelper();

	LOG0(SourceInfo, DebugUtil::DebugSQL,"End build the Hash table...");
}

SQLCode::~SQLCode()
{
	if (NULL != m_pSqlFileHelper)
	{
		delete m_pSqlFileHelper;
		m_pSqlFileHelper = NULL;
	}
}

void SQLCode::buildSQLStatement(SQLVarParms& varParms, SQLStatement& rSqlStatement)
{
	FUNCTION_ENTRY("SQLCode::_ConstructSQLStatement()");
	std::string strSQLKey;
	SQLStatement SQLFormats;
	int nDbType = 0;
	size_t uiTotalSQLSize = 0;	

	try
	{
		size_t uVarCount = varParms.size();
		if (uVarCount < defMINPARAMSIZE)
			TA_THROW(BadParamCount("the PrepareStatement parameter count error"));

		_GetDbTypeAndSQLKey(varParms, strSQLKey);//strSQLID=strSQLKey=varParms[0]
		_GetSQLFormat(strSQLKey, SQLFormats);
		//_GetSQLID(strSQLKey, rSqlStatement);

		_BuildNormalSQL(varParms, SQLFormats, rSqlStatement);
		_PrintSQL(strSQLKey, rSqlStatement);			
	}
	catch (BadParamCount* e)
	{
		SQLCodeException SQLException(e->what());
		throw SQLException;
	}
	catch (BadIndex* e)
	{
		SQLCodeException SQLException(e->what());
		throw SQLException;
	}
	catch(DbTypeNotSupported* e)
	{
		SQLCodeException SQLException(e->what());
		throw SQLException;
	}
	catch (...)
	{
		SQLCodeException SQLException("Unknown SQLCode exception");
		throw SQLException;
	}

	FUNCTION_EXIT;
}

inline
void  SQLCode::_GetDbTypeAndSQLKey(const SQLVarParms& varParms, std::string& strSQLKey)
{
	strSQLKey = varParms[0].data(enumBeginDbType);
}

size_t  SQLCode::_GetSQLSize(const SQLVarParms& varParms, const std::string& strFormat, int nSQLType)
{
	size_t uiTotalSize = strFormat.size();
	size_t uVarCount   = varParms.size();
	if ( defMINPARAMSIZE == uVarCount )
		return uiTotalSize;

	size_t i = defMINPARAMSIZE;	
	for (; i < uVarCount; i++)
	{
		uiTotalSize += varParms[i].length(nSQLType);
	}

	return uiTotalSize;
}

void  SQLCode::_BuildSQL(const SQLVarParms& varParms, const std::string& strSQLFormat, std::string& strSQL, int nSQLType)
{	
	size_t uVarCount = varParms.size();
	if (defMINPARAMSIZE == uVarCount)	
	{
		strSQL = strSQLFormat;	
	}
	else
	{
		/*
		uVarCount=2  varParms[1]=param 1
		uVarCount=3 varParms[2]=param 2
		*/
		int nSQLLen = MAX_SQLSTRING_LEN+1;
		char* szSQL = new char[nSQLLen];
		memset(szSQL, 0, nSQLLen);

		DEF_CONSTRUCT_SQLCODE(MAX_SQLSTRING_LEN, nSQLType);		

		strSQL = szSQL;
		delete[] szSQL;
		szSQL = NULL;
	}
}

void  SQLCode::_BuildLargeSQL(const SQLVarParms& varParms, size_t uSQLSize, const std::string& strSQLFormat, std::string& strSQL, int nSQLType)
{
	size_t uVarCount = varParms.size();
	if (defMINPARAMSIZE == uVarCount)	
	{
		strSQL = strSQLFormat;	
	}
	else
	{
		/*
		uVarCount=2  varParms[1]=param 1
		uVarCount=3 varParms[2]=param 2
		*/
		char *szSQL = new char[uSQLSize + 1];		
		memset(szSQL, 0, uSQLSize + 1);

		DEF_CONSTRUCT_SQLCODE(uSQLSize, nSQLType);		

		strSQL = szSQL;

		delete[] szSQL;
		szSQL = 0;
	}
	
}

void  SQLCode::_GetSQLFormat(const std::string& strSQLKey, SQLStatement& strSQLFormats)
{
	ThreadGuard guard(m_threadLock); // avoid threads racing
	TA_ASSERT(NULL != m_pSqlFileHelper, "sql file helper handler is null.");
	m_pSqlFileHelper->getSQLString(strSQLKey, strSQLFormats);

	if (strSQLFormats.strCommonSQL.empty() && strSQLFormats.strOracleSQL.empty() && strSQLFormats.strMySQLSQL.empty() && strSQLFormats.strSQLiteSQL.empty())
	{
		TA_THROW(BadParamCount("Cannot find the SQL statement in the hash-table"));	
	}
}

inline
void  SQLCode::_GetSQLID(const std::string& strSQLKey, SQLStatement& rSqlStatement)
{
	rSqlStatement.nSQLID = atoi(strSQLKey.c_str());
}


void  SQLCode::_PrintSQL(const std::string& strSQLKey, SQLStatement& rSqlStatement)
{
	if (!rSqlStatement.strCommonSQL.empty())
	{
		if ( rSqlStatement.strCommonSQL.size() > MAXLOGMESGSIZE)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,"SQLID: %s, LargeSQL: ", strSQLKey.c_str());
			LOGLARGESTRING(SourceInfo, DebugUtil::DebugSQL, rSqlStatement.strCommonSQL.c_str());
		}
		else
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,
				"SQLID: %s, SQL: %s", strSQLKey.c_str(), rSqlStatement.strCommonSQL.c_str());
		}
	}
	if (!rSqlStatement.strMySQLSQL.empty())
	{
		if ( rSqlStatement.strMySQLSQL.size() > MAXLOGMESGSIZE)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,"SQLID: %s, MySQL LargeSQL: ", strSQLKey.c_str());
			LOGLARGESTRING(SourceInfo, DebugUtil::DebugSQL, rSqlStatement.strMySQLSQL.c_str());
		}
		else
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,
				"SQLID: %s, MySQL SQL: %s", strSQLKey.c_str(), rSqlStatement.strMySQLSQL.c_str());
		}
	}
	if (!rSqlStatement.strSQLiteSQL.empty())
	{
		if ( rSqlStatement.strSQLiteSQL.size() > MAXLOGMESGSIZE)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,"SQLID: %s, Oracle LargeSQL: ", strSQLKey.c_str());
			LOGLARGESTRING(SourceInfo, DebugUtil::DebugSQL, rSqlStatement.strSQLiteSQL.c_str());
		}
		else
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,
				"SQLID: %s, Oracle SQL: %s", strSQLKey.c_str(), rSqlStatement.strSQLiteSQL.c_str());
		}
	}
}



void  SQLCode::_BuildNormalSQL(const SQLVarParms& varParms, SQLStatement& rSQLFormats, SQLStatement& rSqlStatement)
{
	if (!rSQLFormats.strCommonSQL.empty())
	{
		size_t uiTotalSQLSize = _GetSQLSize(varParms, rSQLFormats.strCommonSQL);
		if ( uiTotalSQLSize > MAX_SQLSTRING_LEN )
		{
			_BuildLargeSQL(varParms, uiTotalSQLSize, rSQLFormats.strCommonSQL, rSqlStatement.strCommonSQL);
		}
		else
		{
			_BuildSQL(varParms, rSQLFormats.strCommonSQL, rSqlStatement.strCommonSQL);
		}
	}
	else
	{
		if (rSQLFormats.strOracleSQL.empty() || rSQLFormats.strMySQLSQL.empty() || rSQLFormats.strSQLiteSQL.empty())
			TA_THROW(BadParamCount("The MySQL or Oracle SQL or SQLite Format is empty"));

		// build MySQL SQL statement
		size_t uiTotalSQLSize = _GetSQLSize(varParms, rSQLFormats.strMySQLSQL, enumMySQL_SQL);
		if ( uiTotalSQLSize > MAX_SQLSTRING_LEN )
		{
			_BuildLargeSQL(varParms, uiTotalSQLSize, rSQLFormats.strMySQLSQL, rSqlStatement.strMySQLSQL, enumMySQL_SQL);
		}
		else
		{
			_BuildSQL(varParms, rSQLFormats.strMySQLSQL, rSqlStatement.strMySQLSQL, enumMySQL_SQL);
		}
		
		// build Oracle SQL Statement
		uiTotalSQLSize = _GetSQLSize(varParms, rSQLFormats.strOracleSQL, enumOracle_SQL);
		if ( uiTotalSQLSize > MAX_SQLSTRING_LEN )
		{
			_BuildLargeSQL(varParms, uiTotalSQLSize, rSQLFormats.strOracleSQL, rSqlStatement.strOracleSQL, enumOracle_SQL);
		}
		else
		{
			_BuildSQL(varParms, rSQLFormats.strOracleSQL, rSqlStatement.strOracleSQL, enumOracle_SQL);
		}

		// build Oracle SQL Statement
		uiTotalSQLSize = _GetSQLSize(varParms, rSQLFormats.strSQLiteSQL, enumSQLLite_SQL);
		if ( uiTotalSQLSize > MAX_SQLSTRING_LEN )
		{
			_BuildLargeSQL(varParms, uiTotalSQLSize, rSQLFormats.strSQLiteSQL, rSqlStatement.strSQLiteSQL, enumSQLLite_SQL);
		}
		else
		{
			_BuildSQL(varParms, rSQLFormats.strSQLiteSQL, rSqlStatement.strSQLiteSQL, enumSQLLite_SQL);
		}

	}	

}




NS_END(TA_Base_Core)
