/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/SQLCodeInfo.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 
  
#include "SQLCodeInfo.h"

#include "CommonDef.h"
#include "CommonData.h"		
#include "UtilityFun.h"
#include "Logger.h"
#include "SQLDataProcessor.h"
#include "TableInfo.h"

CSQLCodeInfo::CSQLCodeInfo()
{
	m_strFileName.clear();
	m_LstTableInfo.clear();
}

CSQLCodeInfo::~CSQLCodeInfo()
{
	_FreeData();
}

void CSQLCodeInfo::_FreeData()
{  	
	CTableInfo* pGet = NULL;
	std::list<CTableInfo*>::iterator itemLst;
	itemLst = m_LstTableInfo.begin();
	while (itemLst != m_LstTableInfo.end())
	{
		pGet = (*itemLst);
		DEF_DELETE(pGet);
		itemLst++;
	}
	m_strFileName.clear();
	m_LstTableInfo.clear();

}


int CSQLCodeInfo::analyzeData(VrtLinesConT& vrtAllLines)
{
	int								nFunRes = 0;
	int								nLinesCount = -1;		 
	VrtLinesConIterT				vtrLineIter;
	VrtLinesConT					VtrProcessOneTable;
	

	nLinesCount = vrtAllLines.size();
	vtrLineIter = vrtAllLines.begin();
	VtrProcessOneTable.clear();
	while ( vtrLineIter != vrtAllLines.end() )
	{
		std::string strOneLine; 
		strOneLine = (*vtrLineIter); 
		
		if (strOneLine.empty())
		{
			vtrLineIter++;
			continue;
		}	

		//check line is "[TABLE]"
		if (std::string::npos != strOneLine.find("[") 
			&& std::string::npos == strOneLine.find("[["))
		{
			if (VtrProcessOneTable.size() > 0)
			{
				CTableInfo* pNewTableInfo = new CTableInfo();
				nFunRes = pNewTableInfo->analyzeData(VtrProcessOneTable);//one table
				VtrProcessOneTable.clear();
				m_LstTableInfo.push_back(pNewTableInfo);	
			}
		}

		VtrProcessOneTable.push_back(strOneLine);

		vtrLineIter++;

		if (vrtAllLines.end() == vtrLineIter)
		{
			CTableInfo* pNewTableInfo = new CTableInfo();
			nFunRes = pNewTableInfo->analyzeData(VtrProcessOneTable);//one table 
			VtrProcessOneTable.clear();
			m_LstTableInfo.push_back(pNewTableInfo);								 
		}

		if (0 != nFunRes)
		{
			nFunRes = -1;
			return nFunRes;
		}		

	}//while


	return nFunRes;	
}






int CSQLCodeInfo::checkSupportDBSqlNum()
{
	int			nFunRes = 0;
	int			nTableAllClassListCount = 0;
	CTableInfo* pTableInfo = NULL;
	std::list<CTableInfo*>::iterator  itemLst; 
	int			nListCount = 0;

	if (m_strFileName.empty() || m_LstTableInfo.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}
	nListCount = m_LstTableInfo.size();
	itemLst = m_LstTableInfo.begin();
	while(itemLst != m_LstTableInfo.end())
	{
		pTableInfo = (*itemLst);
		
		nFunRes = pTableInfo->checkSupportDBSqlNum();
		if (0 != nFunRes)
		{	   		
			_SysLog(SourceFLInfo, DebugError, "error! checkSupportDBSqlNum() FileName=%s", m_strFileName.c_str());
			nFunRes = -1;
			return nFunRes; 				
		}
		
		itemLst++; 
	}  //for

	return nFunRes;
}






int CSQLCodeInfo::analyzeDataToFiles()
{
	int          nFunRes = 0;
	CTableInfo*  pTableInfo = NULL;
	std::list<CTableInfo*>::iterator itemLst;
	std::string  strTableName;
		
	if (m_strFileName.empty() || m_LstTableInfo.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	itemLst = m_LstTableInfo.begin();
	while (itemLst != m_LstTableInfo.end())
	{
		pTableInfo = NULL;				
		pTableInfo = (*itemLst);
		
		strTableName = pTableInfo->getTableName();
		nFunRes = pTableInfo->analyzeDataToFiles();
		if (0 != nFunRes)
		{
			_SysLog(SourceFLInfo, DebugError, 
				"analyzeDataToFiles error, table name = %s", strTableName.c_str());
			nFunRes = -1;
			return nFunRes;
		} 
		itemLst++;
	}//while

	return nFunRes;
}

int CSQLCodeInfo::setFileName( const std::string& strFileName )
{
   int nFunRes = 0;	    
   m_strFileName = strFileName;
   return nFunRes;
}


