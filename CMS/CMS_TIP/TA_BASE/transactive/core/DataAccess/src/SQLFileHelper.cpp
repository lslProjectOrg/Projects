#include "SQLFileHelper.h"
#include "core/DataAccess/src/DataAccessException.h"
#include "core/DataAccess/src/ProjectSQLString.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Core)

SQLFileHelper::SQLFileHelper() 
{
	m_nArrSize = sizeof(s_SQLArray) / sizeof (*s_SQLArray);
	_InitSQLHashIndex();
}


SQLFileHelper::~SQLFileHelper()
{
	m_SQLHashTable.clear();
	m_hSQLHashIndex.clear();
}


void  SQLFileHelper::getSQLString(const std::string& strSQLKey, SQLStatement& rSqlStatements)
{
	int nIndex = -1;

	if (m_hSQLHashIndex.empty())
	{
		LOG_WARNING<<"The SQL hash table is empty when load strSQLKey="<<strSQLKey;
		_InitSQLHashIndex();
	}

	//nIndex = m_hSQLHashIndex[strSQLKey];
	strSQLIndexIterT iterFound = m_hSQLHashIndex.find(strSQLKey);
	if (iterFound == m_hSQLHashIndex.end())
	{
		LOG_ERROR<<"the SQLKey cannot match in Hash table. strSQLKey="<<strSQLKey;
		throw DataAccessException(ER_DB_ERR_SQLFileHelper, "the SQLKey cannot match in Hash table");		
	}		

	nIndex = iterFound->second;

	rSqlStatements.strSQLID  = strSQLKey;
	rSqlStatements.strCommonSQL = s_SQLArray[nIndex].pCommonSQL;
	rSqlStatements.strOracleSQL  = s_SQLArray[nIndex].pOracleSQL;
	rSqlStatements.strMySQLSQL  = s_SQLArray[nIndex].pMySQLSQL;
	rSqlStatements.strSQLiteSQL = s_SQLArray[nIndex].pSQLiteSQL;

}

std::string SQLFileHelper::getSQLString( long uSQLID )
{
	return m_SQLHashTable[uSQLID];
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