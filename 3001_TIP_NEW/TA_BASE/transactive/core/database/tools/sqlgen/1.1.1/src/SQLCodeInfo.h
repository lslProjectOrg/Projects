/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/SQLCodeInfo.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin $
  * 
  * 
  */


#ifndef __DEF_CLASS_SQLCODEINFO_HH__
#define __DEF_CLASS_SQLCODEINFO_HH__


#include <iostream>
#include <string>
#include <list>
#include "CommonDef.h"
#include "CommonData.h"

class CTableInfo;

class CSQLCodeInfo
{ 
public:
	CSQLCodeInfo();
	~CSQLCodeInfo();

public:
	/*
	Analyze all data in sqlcode.txt to 	class
	Analyze all Lines
	*/ 
	int analyzeData(VrtLinesConT& vrtAllLines);
	
	/*
	check eachTable's Oracle sql num is the same as Mysql sql num
	*/
	int checkOracleAndMysqlSqlNum();
	int analyzeDataToFiles();
	int setFileName(const std::string& strFileName);

private:	
	void _FreeData();
private:
	std::string m_strFileName;
	std::list<CTableInfo*> m_LstTableInfo;
	
};



#endif // __DEF_CLASS_SQLCODEINFO_HH__


















