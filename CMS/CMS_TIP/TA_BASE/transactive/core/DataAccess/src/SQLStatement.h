#ifndef _CLASS_SQL_STETEMENT_H__
#define _CLASS_SQL_STETEMENT_H__

#include <iostream>

class SQLStatement
{
public:
	SQLStatement();
	~SQLStatement();
public:
	int nSQLID;                                // the SQL ID map in the sqlcode.txt
	std::string strSQLID;                      // the SQL ID map in the sqlcode.txt
	std::string strCommonSQL;                     // the common SQL which can run at all supported db
	std::string strOracleSQL;                     // the oracle SQL statement
	std::string strMySQLSQL;                      // the mysql SQL statement
	std::string strSQLiteSQL;                     // the SQLite SQL statement

public:
	void clear();
	void logInfo();
};//class SQLStatement


#endif //_CLASS_SQL_STETEMENT_H__
