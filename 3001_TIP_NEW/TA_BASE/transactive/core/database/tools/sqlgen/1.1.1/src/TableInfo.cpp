/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/TableInfo.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 





#include "TableInfo.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"
#include "Logger.h"
#include "SQLDataProcessor.h"
#include "SQLType.h"

CTableInfo::CTableInfo()
{
   m_strTableName.clear();
   m_LstDBInfo.clear();
}

CTableInfo::~CTableInfo()
{
   _FreeData();
}




int CTableInfo::analyzeData(VrtLinesConT& VtrOneTable)
{
	int					nFunRes = 0;
	VrtLinesConIterT	VtrIterTable;
	VrtLinesConT		VtrOneDb;
						   
	if (VtrOneTable.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}


	VtrIterTable = VtrOneTable.begin();
	while (VtrIterTable != VtrOneTable.end())
	{
		std::string	strLineTmp = (*VtrIterTable);
		
		//check this line is [TABLE]
		if (std::string::npos != strLineTmp.find("[") && std::string::npos == strLineTmp.find("[["))
		{//[]
			m_strTableName = strLineTmp;	
			VtrIterTable++;
			continue;		  		
		}

		//check this line is [[DB]]
		if (std::string::npos != strLineTmp.find("[") && std::string::npos != strLineTmp.find("[["))
		{  //[[]]

			if (VtrOneDb.size() > 0)
			{
				CSQLType* pNewSQLType = new CSQLType();
				nFunRes = pNewSQLType->analyzeData(VtrOneDb);			
				VtrOneDb.clear(); 
				m_LstDBInfo.push_back(pNewSQLType);
				pNewSQLType = NULL;
			}		
		}	  
		VtrOneDb.push_back(strLineTmp);	

		VtrIterTable++;	 

		if (VtrOneTable.end() == VtrIterTable)
		{		
			CSQLType* pNewSQLType = new CSQLType();
			nFunRes = pNewSQLType->analyzeData(VtrOneDb);			
			VtrOneDb.clear(); 
			m_LstDBInfo.push_back(pNewSQLType);
			pNewSQLType = NULL;	 
		}
			
		if (0 != nFunRes)
		{
			nFunRes = -1;
			return nFunRes;
		}

	}//while


	return nFunRes;
}

void CTableInfo::_FreeData()
{		
	std::list<CSQLType*>::iterator itemLst;
	CSQLType* pGet = NULL;

	itemLst = m_LstDBInfo.begin();
	while (itemLst != m_LstDBInfo.end())
	{
		pGet = (*itemLst);
		DEF_DELETE(pGet);
		itemLst++;
	}
	m_strTableName.clear();
	m_LstDBInfo.clear();
} 

int CTableInfo::checkSupportDBSqlNum()
{
	int				nFunRes = 0;
	int				nListCount = 0;	
	int				nOralceSqlCount = 0;
	int				nMysqlSqlCount = 0;
	int				nSQLiteSqlCount = 0;

	std::string     strDbOracle;
	std::string     strDbMysql;
	std::string     strDbSQLite;
	std::list<CSQLType*>::iterator itemLst;
	CSQLType*        pDBInfo = NULL;
	std::string     strDBName;
	
	//contain [[oracle]] [[mysql]]
	nListCount = m_LstDBInfo.size();
	if (nListCount < 2)
	{
		nFunRes = 0;
		return nFunRes;
	}
	
	strDbOracle = DB_ORACLE;
	strDbMysql = DB_MYSQL;
	strDbSQLite = DB_SQLite;

	nOralceSqlCount = 0;
	nMysqlSqlCount = 0;
	nSQLiteSqlCount = 0;
	
	//get data
	itemLst = m_LstDBInfo.begin();
	while (itemLst != m_LstDBInfo.end())
	{
		pDBInfo = (*itemLst);
		strDBName = pDBInfo->getDBName();

		if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDbOracle.c_str()))
		{
			nOralceSqlCount = pDBInfo->getDataNum();
		}
		else if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDbMysql.c_str()))
		{
			nMysqlSqlCount =  pDBInfo->getDataNum();
		}
		else if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDbSQLite.c_str()))
		{
			nSQLiteSqlCount =  pDBInfo->getDataNum();
		}
	
		itemLst++;
	}//while
	
	//check
	if (nOralceSqlCount != nMysqlSqlCount)
	{
		nFunRes = -1;
		_SysLog(SourceFLInfo, DebugError, "error! nOralceSqlCount != nMysqlSqlCount TableName=%s", m_strTableName.c_str());
		return nFunRes;
	} 
	if (nOralceSqlCount != nSQLiteSqlCount)
	{
		nFunRes = -1;
		_SysLog(SourceFLInfo, DebugError, "error! nOralceSqlCount != nSQLiteSqlCount TableName=%s", m_strTableName.c_str());
		return nFunRes;
	} 
	if (nMysqlSqlCount != nSQLiteSqlCount)
	{
		nFunRes = -1;
		_SysLog(SourceFLInfo, DebugError, "error! nMysqlSqlCount != nSQLiteSqlCount TableName=%s", m_strTableName.c_str());
		return nFunRes;
	} 



	return nFunRes;
}

std::string CTableInfo::getTableName()
{
   return m_strTableName;
}




int CTableInfo::analyzeDataToFiles()
{
	int nFunRes = 0;
	std::list<CSQLType*>::iterator itemLst;
	CSQLType* pDBInfoCommon = NULL;
	CSQLType* pDBInfoOracle = NULL;
	CSQLType* pDBInfoMysql = NULL;
	CSQLType* pDBInfoSQLite = NULL;

	CSQLType* pGet = NULL;
	std::string strDBNameCommon = DB_COMMON;
	std::string strDBNameOracle = DB_ORACLE;	
	std::string strDBNameMySql = DB_MYSQL;	
	std::string strDBNameSQLite = DB_SQLite;	

	std::string strDBName;
	std::string strIndexTableName;

	//just have one DB [[common]]
	itemLst = m_LstDBInfo.begin();
	while (itemLst != m_LstDBInfo.end())
	{
		pGet = (*itemLst);	

		strDBName = pGet->getDBName();
		if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDBNameCommon.c_str()))//DB_COMMON
		{
			pDBInfoCommon = pGet;
		}
		else if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDBNameOracle.c_str()))//DB_ORACLE
		{
			pDBInfoOracle = pGet;
		}
		else if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDBNameMySql.c_str()))//DB_MYSQL
		{
			pDBInfoMysql = pGet;
		}
		else if (0 == CUtilityFun::getInstance().sysStricmp(strDBName.c_str(), strDBNameSQLite.c_str()))//DB_SQLite
		{
			pDBInfoSQLite = pGet;
		}

		itemLst++;
	}//while
	
	//[0][AlarmState]
	strIndexTableName = CSQLDataProcessor::getInstance().getIndexTableName(m_strTableName);

	if (NULL != pDBInfoCommon && NULL != pDBInfoOracle && NULL != pDBInfoMysql && NULL != pDBInfoSQLite)
	{
		//have [[common]] [[oracle]] [[mysql]] [[sqlite]]
		nFunRes = pDBInfoCommon->analyzeDataTypeCommonOracleMysqlSqlite(strIndexTableName, pDBInfoOracle, pDBInfoMysql, pDBInfoSQLite);
	}
	else if (NULL != pDBInfoOracle && NULL != pDBInfoMysql && NULL != pDBInfoSQLite)
	{
		//just have [[oracle]] [[mysql]] [[sqlite]]
		nFunRes = pDBInfoOracle->analyzeDataTypeOracleMysqlSqlite(strIndexTableName, pDBInfoMysql, pDBInfoSQLite);
	} 
	else if (NULL != pDBInfoCommon)
	{
		//just have [[common]]
		nFunRes = pDBInfoCommon->analyzeDataTypeCommon(strIndexTableName);
	} 

	return nFunRes;
}

