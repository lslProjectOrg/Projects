#include "SQLStatement.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_Core)

SQLStatement::SQLStatement()
{
	clear();
}

SQLStatement::~SQLStatement()
{
	clear();
}

void SQLStatement::clear()
{
	this->nSQLID = 0;
	this->strSQLID.clear();
	this->strCommonSQL.clear();
	this->strOracleSQL.clear();
	this->strMySQLSQL.clear();
	this->strSQLiteSQL.clear();
}


void  SQLStatement::logInfo()
{
	LOG_DEBUG<<"SQL Info:"<<" "<<"nSQLID="<<nSQLID<<" "<<"strSQLID="<<strSQLID<<std::endl
		<<" "<<"strCommonSQL="<<strCommonSQL<<std::endl
		<<" "<<"strOracleSQL="<<strOracleSQL<<std::endl
		<<" "<<"strMySQLSQL="<<strMySQLSQL<<std::endl
		<<" "<<"strSQLiteSQL="<<strSQLiteSQL<<std::endl;
	
}

NS_END(TA_Base_Core)



