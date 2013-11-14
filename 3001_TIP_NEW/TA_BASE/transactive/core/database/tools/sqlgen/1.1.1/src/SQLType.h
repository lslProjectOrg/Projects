/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/SQLType.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin $
  * 
  * 
  */


#ifndef __DEF_CLASS_SQLTYPE_HH__
#define __DEF_CLASS_SQLTYPE_HH__



#include <iostream>
#include <string>
#include <list>

#include "CommonDef.h"
#include "CommonData.h"

class CSQLItem; 

class CSQLType
{

public:
	CSQLType();	
	~CSQLType();
public:
	/*
	Analyze all data in sqlcode.txt to 	class
	Analyze One DB Lines
	*/ 
	int analyzeData(VrtLinesConT& VtrOneDb);
	std::string getDBName();
	int getDataNum();
	
	int analyzeDataTypeCommon(const std::string& strIndexTableName);
	int analyzeDataTypeOracleMysql(const std::string& strIndexTableName, CSQLType* pDBInfoMysql);
	int analyzeDataTypeCommonOracleMysql(const std::string& strIndexTableName, CSQLType* pDBInfoOracle, CSQLType* pDBInfoMysql);
	
	/*
		check Oracle and Mysql SqlId match
	*/
	int checkSqlIDMatch(const std::string& strSqlID, std::string& strOUTMysqlSqlLine);
private:
	void _FreeData();
private:
	std::string m_strDbName;
	std::list<CSQLItem*> m_LstSQLInfo;
	

};

#endif // __DEF_CLASS_SQLTYPE_HH__







