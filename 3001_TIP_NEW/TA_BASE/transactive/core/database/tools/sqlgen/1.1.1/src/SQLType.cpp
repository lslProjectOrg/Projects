/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/SQLType.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 

#include "SQLType.h" 
#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"
#include "Logger.h"
#include "SQLDataProcessor.h"
#include "SQLItem.h"

CSQLType::CSQLType()
{

}

CSQLType::~CSQLType()
{
   _FreeData();
}



int CSQLType::analyzeData(VrtLinesConT& VtrOneDb)
{
	int					nFunRes = 0;
	VrtLinesConIterT	VtrIterDb;
								   
	if (VtrOneDb.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}							   

	VtrIterDb = VtrOneDb.begin();
	while (VtrIterDb != VtrOneDb.end())
	{
		std::string	strLineTmp = (*VtrIterDb);
		
		//check this line is [[DB]]
		if (std::string::npos != strLineTmp.find("[[") )
		{//[[]]			
			m_strDbName = strLineTmp;
			VtrIterDb++;
			continue;		  		
		}

		CSQLItem* pNewSQLItem = new CSQLItem();
		nFunRes = pNewSQLItem->analyzeData(strLineTmp);
		if (0 != nFunRes)
		{
			_SysLog(SourceFLInfo, DebugError, "SQLItem analyzeData error: [line=%s]", strLineTmp.c_str());
			DEF_DELETE(pNewSQLItem);
			nFunRes = -1;
			return nFunRes;
		}
		
		m_LstSQLInfo.push_back(pNewSQLItem);
		pNewSQLItem = NULL;
		VtrIterDb++;			 

	}//while

	return nFunRes;
}

void CSQLType::_FreeData()
{		
	std::list<CSQLItem*>::iterator itemLst;
	CSQLItem* pGet = NULL;
	
	itemLst = m_LstSQLInfo.begin();
	while (itemLst != m_LstSQLInfo.end())
	{
		pGet = (*itemLst);
		DEF_DELETE(pGet);
		itemLst++;
	}
	m_strDbName.clear();
	m_LstSQLInfo.clear();
}

std::string CSQLType::getDBName()
{
	return m_strDbName;
}

int CSQLType::getDataNum()
{
	return m_LstSQLInfo.size();
}

int CSQLType::checkSqlIDMatch(const std::string& strSqlID, std::string& strOUTMysqlSqlLine)
{
	int nFunRes = 0;
	std::list<CSQLItem*>::iterator itemLst;	
	CSQLItem* pSQLItemGet = NULL;
	std::string strSqlIDGet;
	
	//set not finded	
	nFunRes = -1;
	strOUTMysqlSqlLine.clear();
	
	itemLst = m_LstSQLInfo.begin();
	while (itemLst != m_LstSQLInfo.end())
	{
		pSQLItemGet = (*itemLst);
		strSqlIDGet = pSQLItemGet->getSqlID();
		
		//if (0 == CUtilityFun::getInstance().sysStricmp(strSqlIDGet.c_str(), strSqlID.c_str()))
		if (0 == strSqlIDGet.compare(strSqlID))
		{
			//find same sqlID
			strOUTMysqlSqlLine =  pSQLItemGet->getSqlLine();
			nFunRes = 0;
			break;
		}
	
		itemLst++;
	}//while
	
	
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "checkSqlIDMatch error! SqlID=%s", strSqlID.c_str());
	}
	
	return nFunRes;	
}

int CSQLType::analyzeDataTypeCommon(const std::string& strIndexTableName)
{  		
	int				nFunRes = 0;
	std::string		strTableTypeCommon;
	std::list<CSQLItem*>::iterator itemLst;
	CSQLItem*       pSQLInfoCommon = NULL;
	
	//add to file 
	CSQLDataProcessor::getInstance().addDataToFileSQLH(strIndexTableName);
	CSQLDataProcessor::getInstance().addDataToFileSQLMACRODEFH(strIndexTableName);

	if (m_LstSQLInfo.empty())
	{
		nFunRes = 0;
		return nFunRes;
	} 

	
	strTableTypeCommon = "//[[common]]\n";	 //[[common]]
	CSQLDataProcessor::getInstance().addDataToFileSQLMACRODEFH(strTableTypeCommon);

	
	itemLst = m_LstSQLInfo.begin();
	while (itemLst != m_LstSQLInfo.end())
	{ 	
		pSQLInfoCommon = (*itemLst);

		/*
		add check if there have same sqlID in sqlcode.txt
		in sqlmacrodef.h will not define Duplicate
		*/
		nFunRes = pSQLInfoCommon->checkDuplicateSqlID();
		if (0 != nFunRes)
		{
			nFunRes = -1;
			return nFunRes;
		}

		std::string strDefineID;
		std::string strSQLID;
		std::string strCommonSQL;  
		std::string strOracleSQL; 
		std::string strMySqlSQL;   
		/*sqlmadef.h*/
		strDefineID.clear();
		strSQLID.clear();
		strCommonSQL.clear();
		strOracleSQL.clear();
		strMySqlSQL.clear();
		
		pSQLInfoCommon->getDataForFileTypeCommon(strDefineID, strSQLID, strCommonSQL, strOracleSQL, strMySqlSQL);
		
		CSQLDataProcessor::getInstance().addDataToFileSQLMACRODEFH(strDefineID);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strSQLID); 
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strCommonSQL);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strOracleSQL);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strMySqlSQL);
		
		itemLst++;
		
	}//while

	return	nFunRes; 
}



int CSQLType::analyzeDataTypeOracleMysql(const std::string& strIndexTableName, CSQLType* pDBInfoMysql)
{  		
	int				nFunRes = 0;
	std::string		strTableTypeOracleMySql;
	std::list<CSQLItem*>::iterator itemLst;
	CSQLItem*       pSQLInfoOracle = NULL;
	std::string     strSqlID;
	std::string     strMysqlSqlLine;

	//add to file 
	CSQLDataProcessor::getInstance().addDataToFileSQLH(strIndexTableName);
	CSQLDataProcessor::getInstance().addDataToFileSQLMACRODEFH(strIndexTableName);
		  	
	strTableTypeOracleMySql = "//[[oracle]][[mysql]]\n";	
	CSQLDataProcessor::getInstance().addDataToFileSQLMACRODEFH(strTableTypeOracleMySql);

			 
	itemLst = m_LstSQLInfo.begin();
	while (itemLst != m_LstSQLInfo.end())
	{ 	
		pSQLInfoOracle = (*itemLst);

		/*
		add check if there have same sqlID in sqlcode.txt
		in sqlmacrodef.h will not define Duplicate
		*/
		nFunRes = pSQLInfoOracle->checkDuplicateSqlID();
		if (0 != nFunRes)
		{
			nFunRes = -1;
			return nFunRes;
		}
		
		/*
		check Oracle and Mysql SqlId match
		*/
		strMysqlSqlLine.clear();
		strSqlID.clear();
		strSqlID = pSQLInfoOracle->getSqlID();
		nFunRes = pDBInfoMysql->checkSqlIDMatch(strSqlID, strMysqlSqlLine);
		if (0 != nFunRes)
		{
			nFunRes = -1;
			return nFunRes;
		}

		std::string strDefineID;
		std::string strSQLID;
		std::string strCommonSQL;  
		std::string strOracleSQL; 
		std::string strMySqlSQL;   
		/*sqlmadef.h*/
		strDefineID.clear();
		strSQLID.clear();
		strCommonSQL.clear();
		strOracleSQL.clear();
		strMySqlSQL.clear();
		
		pSQLInfoOracle->getDataForFileTypeOracleMySql(strMysqlSqlLine, strDefineID, strSQLID, strCommonSQL, strOracleSQL, strMySqlSQL);
		
		CSQLDataProcessor::getInstance().addDataToFileSQLMACRODEFH(strDefineID);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strSQLID);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strCommonSQL);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strOracleSQL);
		CSQLDataProcessor::getInstance().addDataToFileSQLH(strMySqlSQL);
		
		itemLst++;
		
	}//while

	return	nFunRes; 
}

int CSQLType::analyzeDataTypeCommonOracleMysql(const std::string& strIndexTableName, CSQLType* pDBInfoOracle, CSQLType* pDBInfoMysql)
{
	int		nFunRes = 0;
	
	nFunRes = analyzeDataTypeCommon(strIndexTableName);
	if (0 != nFunRes)
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	nFunRes = pDBInfoOracle->analyzeDataTypeOracleMysql(strIndexTableName, pDBInfoMysql);
	if (0 != nFunRes)
	{
		nFunRes = -1;
		return nFunRes;
	}
	return	nFunRes; 
}

