/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/SQLItem.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin $
  * 
  * 
  */




#ifndef __DEF_CLASS_SQLITEM_HH__
#define __DEF_CLASS_SQLITEM_HH__

   
#include <iostream>
#include <string>
#include <list>
#include "CommonDef.h"
#include "CommonData.h"
 
   

class CSQLItem
{
public:
	CSQLItem();
	~CSQLItem();

private:
	void _FreeData();

public:
	/*
	Analyze all data in sqlcode.txt to 	class
	Analyze One sql Line
	*/ 
	int analyzeData(const std::string& strLine);
	int checkDuplicateSqlID();

	int getDataForFileTypeCommon(std::string& strOUTDefineID, std::string& strOUTSQLID, std::string& strOUTCommonSQL, std::string& strOUTOracleSQL, std::string& strOUTMySqlSQL, std::string& strOUTSQLiteSQL);
	int getDataForFileTypeOracleMySqlSQLite( const std::string& strINMySqlLine, const std::string& strINSQLiteLine, std::string& strOUTDefineID, std::string& strOUTSQLID, std::string& strOUTCommonSQL, std::string& strOUTOracleSQL, std::string& strOUTMySqlSQL, std::string& strOUTSQLiteSQL );


	std::string getSqlID();
	std::string getSqlLine();
private:
	//check SqlID, SqlID String will not contain ' ', must contain '0123456789'
	int _CheckSqlID();

private:
	std::string m_strSqlId;
	std::string m_strSqlLine;

	
};



#endif // __DEF_CLASS_SQLITEM_HH__










