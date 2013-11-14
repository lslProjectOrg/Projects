//////////////////////////////////////////////////////////////////////
///		@file		SQLCode.h
///		@author		zhilin,ouyang
///		@date		2010-11-29 14:44:41
///
///		@brief	    to hold all of the SQL statements used in our 
///                 system in one location. The SQLCode class is based 
///                 on the Singleton design pattern.
//////////////////////////////////////////////////////////////////////
#ifndef SQLCODE_H
#define SQLCODE_H

#include "sqldef.h"
#include "SQLVarParms.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/database/src/CommonType.h"


#include "sqlmacrodef.h"

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

	/**
	* buildSQLStatement
	*
	* @brief: to build the SQL statement.
	* @param: varParms contains the SQL ID, variables in sequence.
	*
	* Returns the SQL statement if successfully otherwise empty.
	*/
	void  buildSQLStatement(SQLVarParms& varParms, SQLStatement& rSqlStatement);

private:
		
	enum enumSQLType
	{
		enumCommon_SQL         = 0,
		enumOracle_SQL,
		enumMySQL_SQL,
		enumEnd_SQL
	};

private:		

	inline  
	void    _GetDbTypeAndSQLKey(const SQLVarParms& varParms, std::string& strSQLKey);
	void    _GetSQLFormat(const std::string& strSQLKey, SQLStatement& strSQLFormats);
	inline
	void    _GetSQLID(const std::string& strSQLKey, SQLStatement& rSqlStatement);
	void    _PrintSQL(const std::string& strSQLKey, SQLStatement& strSQLFormats);
	void    _BuildNormalSQL(const SQLVarParms& varParms, SQLStatement& rSQLFormats, SQLStatement& rSqlStatement);
	size_t  _GetSQLSize(const SQLVarParms& varParms, const std::string& strFormat, int nSQLType = enumCommon_SQL);
	void    _BuildSQL(const SQLVarParms& varParms, const std::string& strFormat, std::string& strSQL, int nSQLType = enumCommon_SQL);
	void    _BuildLargeSQL(const SQLVarParms& varParms, size_t uSQLSize, const std::string& strFormat, std::string& strSQL, int nSQLType = enumCommon_SQL);



private:
	SQLCode();
	SQLCode(const SQLCode&);

	static SQLCode* m_pInstance;	           // the only one instance of the class
	SQLFileHelper*  m_pSqlFileHelper;          // the SQL file helper

	static ReEntrantThreadLockable m_instanceLock;
	ReEntrantThreadLockable  m_threadLock;

};

NS_END(TA_Base_Core)

#endif   /// SQLCODE_H



