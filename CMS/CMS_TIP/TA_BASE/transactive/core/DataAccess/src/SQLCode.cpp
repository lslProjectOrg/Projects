#include "SQLCode.h"
#include "SQLFileHelper.h"

#include "core/DataAccess/src/DataAccessException.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_Core)


boost::mutex SQLCode::m_mutexInstance;
SQLCode* SQLCode::m_pInstance = 0;

SQLCode& SQLCode::getInstance()
{
	BOOST_LOG_FUNCTION();
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	if (NULL == m_pInstance)
	{
		m_pInstance = new SQLCode();
	}
	return (*m_pInstance);
}

void SQLCode::removeInstance()
{
	BOOST_LOG_FUNCTION();
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	if ( NULL != m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


SQLCode::SQLCode()
{
	BOOST_LOG_FUNCTION();
	LOG_DEBUG<<"Begin build the Hash table...";
	{
		boost::mutex::scoped_lock lock(m_mutexSQLFileHelper);	
		m_pSqlFileHelper = new SQLFileHelper();
	}

	LOG_DEBUG<<"End build the Hash table...";
}

SQLCode::~SQLCode()
{
	BOOST_LOG_FUNCTION();

	{
		boost::mutex::scoped_lock lock(m_mutexSQLFileHelper);	
		if (NULL != m_pSqlFileHelper)
		{
			delete m_pSqlFileHelper;
			m_pSqlFileHelper = NULL;
		}
	}

}

void SQLCode::_BuildSQLStatement(SQLVarParms& varParms, SQLStatement& rSqlStatement)
{
	BOOST_LOG_FUNCTION();
	std::string strSQLKey;
	SQLStatement SQLFormats;
	int nDbType = 0;
	size_t uiTotalSQLSize = 0;	

	try
	{
		size_t uVarCount = varParms.size();
		if (uVarCount < 1)
		{
			throw DataAccessException(ER_DB_ERR_SQLCode, "the PrepareStatement parameter count error!");
		}

		_GetDbTypeAndSQLKey(varParms, strSQLKey);//strSQLID=strSQLKey=varParms[0]
		_GetSQLFormat(strSQLKey, SQLFormats);

		_BuildNormalSQL(varParms, SQLFormats, rSqlStatement);
		rSqlStatement.logInfo();
	}
	catch (DataAccessException& e)
	{
		throw e;
	}
	catch (...)
	{
		throw DataAccessException(ER_DB_ERR_UNKNOWN, "buildSQLStatement error!");
	}

}


void  SQLCode::_GetDbTypeAndSQLKey(const SQLVarParms& varParms, std::string& strSQLKey)
{
	BOOST_LOG_FUNCTION();

	strSQLKey = varParms[0].data(enumBeginDbType);
}

size_t  SQLCode::_GetSQLSize(const SQLVarParms& varParms, const std::string& strFormat, int nSQLType)
{
	BOOST_LOG_FUNCTION();

	size_t uiTotalSize = strFormat.size();
	size_t uVarCount   = varParms.size();
	size_t nIndex = 1;	
	for (; nIndex < uVarCount; nIndex++)
	{
		uiTotalSize += varParms[nIndex].length(nSQLType);
	}

	return uiTotalSize;
}

void  SQLCode::_BuildSQL(const SQLVarParms& varParms, const std::string& strSQLFormat, std::string& strSQL, int nSQLType)
{	
	BOOST_LOG_FUNCTION();

	size_t uVarCount = varParms.size();

	{
		int nSQLLen = DEF_INT_MAXLINELEN;
		char* szSQL = new char[nSQLLen];

		memset(szSQL, 0, nSQLLen);

		DEF_CONSTRUCT_SQLCODE(DEF_INT_MAXLINELEN, nSQLType);		

		strSQL = szSQL;
		delete[] szSQL;
		szSQL = NULL;
	}
}

void  SQLCode::_GetSQLFormat(const std::string& strSQLKey, SQLStatement& strSQLFormats)
{
	BOOST_LOG_FUNCTION();

	boost::mutex::scoped_lock lock(m_mutexSQLFileHelper);	

	m_pSqlFileHelper->getSQLString(strSQLKey, strSQLFormats);

	if (strSQLFormats.strCommonSQL.empty() 
		&& strSQLFormats.strOracleSQL.empty() 
		&& strSQLFormats.strMySQLSQL.empty() 
		&& strSQLFormats.strSQLiteSQL.empty())
	{
		throw DataAccessException(ER_DB_ERR_SQLCode, "Cannot find the SQL statement in the hash-table");
	}
}


void  SQLCode::_GetSQLID(const std::string& strSQLKey, SQLStatement& rSqlStatement)
{
	BOOST_LOG_FUNCTION();

	rSqlStatement.nSQLID = atoi(strSQLKey.c_str());
}





void  SQLCode::_BuildNormalSQL(const SQLVarParms& varParms, SQLStatement& rSQLFormats, SQLStatement& rSqlStatement)
{
	BOOST_LOG_FUNCTION();

	if (!rSQLFormats.strCommonSQL.empty())
	{
		_BuildSQL(varParms, rSQLFormats.strCommonSQL, rSqlStatement.strCommonSQL);
	}
	else
	{
		if (rSQLFormats.strOracleSQL.empty() 
			|| rSQLFormats.strMySQLSQL.empty() 
			|| rSQLFormats.strSQLiteSQL.empty())
		{
			throw DataAccessException(ER_DB_ERR_SQLCode, "The MySQL or Oracle SQL or SQLite Format is empty");
		}
		
		// build Oracle SQL Statement
		_BuildSQL(varParms, rSQLFormats.strOracleSQL, rSqlStatement.strOracleSQL, enumOracleDb);

		// build MySQL SQL statement
		_BuildSQL(varParms, rSQLFormats.strMySQLSQL, rSqlStatement.strMySQLSQL, enumMysqlDb);

		// build SqliteDb SQL Statement
		_BuildSQL(varParms, rSQLFormats.strSQLiteSQL, rSqlStatement.strSQLiteSQL, enumSqliteDb);
	}	

}



void SQLCode::prepareSQLStatement(SQLStatement& rSqlStatement, const SQLTypeAdapter& arg0)
{
	BOOST_LOG_FUNCTION();

	return prepareSQLStatement(rSqlStatement, SQLVarParms() << arg0);
}

void SQLCode::prepareSQLStatement(SQLStatement& rSqlStatement, SQLVarParms& varParms)
{
	BOOST_LOG_FUNCTION();

	try
	{
		LOG_DEBUG<< "Begin prepare SQL statement...";
		rSqlStatement.clear();
		_BuildSQLStatement(varParms, rSqlStatement);
		LOG_DEBUG<< "End prepare SQL statement...";
	}
	catch (DataAccessException& e)
	{
		e.logInfo();
		throw e;
	}
	catch (...)
	{			
		throw DataAccessException(ER_DB_ERR_UNKNOWN, "unknow buildSQLStatement erro!");
	}
}	

std::string SQLCode::escapeInsertString(const std::string& theString)
{
	BOOST_LOG_FUNCTION();

	// create a new string to hold the parsed string
	std::string escapedString = theString;
	int pos = escapedString.find_first_of("'");
	while (-1 != pos)
	{
		char charToInsert = escapedString.at(pos);
		escapedString.insert(pos,&charToInsert, 1);
		pos = escapedString.find_first_of("'", pos + 2);
	}
	return escapedString;
}


NS_END(TA_Base_Core)
