#ifndef SQLCODE_H
#define SQLCODE_H

#include "core/DataAccess/src/ProjectSQLDef.h"
#include "core/DataAccess/src/ProjectSQLMacroDef.h"
#include "core/DataAccess/src/DataAccessCommonData.h"
#include "core/DataAccess/src/SQLVarParms.h"
#include "core/DataAccess/src/SQLStatement.h"
#include "core/DataAccess/src/SQLTypeAdapter.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

NS_BEGIN(TA_Base_Core)


class SQLFileHelper;

class SQLCode
{
public:
	// the destructor of the class
	~SQLCode();

	// get the only instance of the class
	static SQLCode& getInstance();	
	static void removeInstance();

public:
	//prepareSQLStatement(rSqlStatement, SQLID, SQLParam1, SQLParam2, SQLParam3, ...)
	void prepareSQLStatement(SQLStatement& rSqlStatement,const SQLTypeAdapter& arg0);
	void prepareSQLStatement(SQLStatement& rSqlStatement, SQLVarParms& varParms);

	// Declare the remaining overloads of PrepareSQLStatement()
	DEF_SQL_PARMS(void, prepareSQLStatement);
	//	void prepareSQLStatement(SQLStatement& rSqlStatment, const SQLTypeAdapter& arg0, const SQLTypeAdapter& arg1) 
	//  void prepareSQLStatement(SQLStatement& rSqlStatment, const SQLTypeAdapter& arg0, const SQLTypeAdapter& arg1, const SQLTypeAdapter& arg2)

public:
	std::string escapeInsertString(const std::string& theString);

private:
	void  _BuildSQLStatement(SQLVarParms& varParms, SQLStatement& rSqlStatement);

	void    _GetDbTypeAndSQLKey(const SQLVarParms& varParms, std::string& strSQLKey);
	void    _GetSQLFormat(const std::string& strSQLKey, SQLStatement& strSQLFormats);
	void    _GetSQLID(const std::string& strSQLKey, SQLStatement& rSqlStatement);
	void    _BuildNormalSQL(const SQLVarParms& varParms, SQLStatement& rSQLFormats, SQLStatement& rSqlStatement);
	size_t  _GetSQLSize(const SQLVarParms& varParms, const std::string& strFormat, int nSQLType = enumBeginDbType);
	void    _BuildSQL(const SQLVarParms& varParms, const std::string& strFormat, std::string& strSQL, int nSQLType = enumBeginDbType);



private:
	SQLCode();
	SQLCode(const SQLCode&);

private:
	static boost::mutex m_mutexInstance;
	static SQLCode* m_pInstance;	           // the only one instance of the class


	boost::mutex  m_mutexSQLFileHelper;
	SQLFileHelper*  m_pSqlFileHelper;          // the SQL file helper

};

NS_END(TA_Base_Core)

#endif   /// SQLCODE_H



