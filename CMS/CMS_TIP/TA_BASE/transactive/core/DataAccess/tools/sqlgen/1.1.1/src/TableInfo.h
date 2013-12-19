/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/TableInfo.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin $
  * 
  * 
  */


#ifndef __DEF_CLASS_TABLEINFO_HH__
#define __DEF_CLASS_TABLEINFO_HH__

#include <iostream>
#include <string>
#include <list>
#include "CommonDef.h"
#include "CommonData.h"

class CSQLType;
  

class CTableInfo
{

public:
	CTableInfo();
	~CTableInfo();

public:
	/*
	Analyze all data in sqlcode.txt to 	class
	Analyze One Table Lines
	*/
	int analyzeData(VrtLinesConT& VtrOneTable);
	int checkSupportDBSqlNum();
	std::string getTableName();
	int analyzeDataToFiles();

private:	
	void _FreeData();
private:
	std::string m_strTableName;
	std::list<CSQLType*> m_LstDBInfo;  //[[common]][[oracle]][[mysql]][[sqlite]]

};

#endif // __DEF_CLASS_TABLEINFO_HH__






