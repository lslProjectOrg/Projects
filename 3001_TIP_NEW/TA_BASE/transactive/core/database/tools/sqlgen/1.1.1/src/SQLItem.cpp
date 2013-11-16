/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/SQLItem.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 
#include "SQLItem.h"
#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"
#include "Logger.h"
#include "SQLDataProcessor.h"
 
CSQLItem::CSQLItem()
{

}

CSQLItem::~CSQLItem()
{
   _FreeData();
}

void CSQLItem::_FreeData()
{
	m_strSqlId.clear();
	m_strSqlLine.clear();
}


int CSQLItem::analyzeData(const std::string& strLine)
{
	int					nFunRes = 0;
	std::string::size_type nFindTmp = std::string::npos;

	if (strLine.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}				   

	nFindTmp = strLine.find_first_of("=");
	if (std::string::npos == nFindTmp)
	{
		nFunRes = -1;
		return nFunRes;
	}
	m_strSqlId = strLine.substr(0, nFindTmp);
	m_strSqlLine = strLine.substr(nFindTmp + 1);
			
	CUtilityFun::getInstance().trim(m_strSqlId);
				 
	return nFunRes;
}

int CSQLItem::checkDuplicateSqlID()
{
    int nFunRes = 0;
    
    //check SqlID, SqlID String will not contain ' ', must contain '0123456789'
	nFunRes = _CheckSqlID();
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError,  "check SqlID Error, SqlID String will not contain ' ', must contain '0123456789', SqlId=%s", m_strSqlId.c_str());
		return nFunRes;
	} 
    
    
	nFunRes = CSQLDataProcessor::getInstance().checkDuplicateSqlID(m_strSqlId);
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError,  "checkDuplicateSqlID Error: SqlId=%s", m_strSqlId.c_str());
	}  

    return nFunRes;
}


int CSQLItem::_CheckSqlID()
{
	int nFunRes = 0;
	std::string::size_type nFindPos = std::string::npos;

	//check SqlID, SqlID String will not contain ' ', must contain '0123456789'
	nFindPos = m_strSqlId.find(0x20);  //" " 0x20 character space
	if (std::string::npos != nFindPos)
	{
		nFunRes = -1;
		return nFunRes;
	}

	// finds the location of the first -numerical character 
	nFindPos = m_strSqlId.find_first_of("0123456789");
	if (std::string::npos == nFindPos)
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	return nFunRes;
}

std::string CSQLItem::getSqlID()
{
	return m_strSqlId;
}

std::string CSQLItem::getSqlLine()
{
	return m_strSqlLine;
}

int CSQLItem::getDataForFileTypeCommon(std::string& strOUTDefineID, std::string& strOUTSQLID, std::string& strOUTCommonSQL, std::string& strOUTOracleSQL, std::string& strOUTMySqlSQL, std::string& strOUTSQLiteSQL)
{
	int nFunRes = 0;
	CSQLDataProcessor::getInstance().getDataForFileTypeCommon(m_strSqlId, m_strSqlLine, strOUTDefineID, strOUTSQLID, strOUTCommonSQL,  strOUTOracleSQL, strOUTMySqlSQL, strOUTSQLiteSQL);
	return nFunRes;
}


int CSQLItem::getDataForFileTypeOracleMySqlSQLite( const std::string& strINMySqlLine, const std::string& strINSQLiteLine, std::string& strOUTDefineID, std::string& strOUTSQLID, std::string& strOUTCommonSQL, std::string& strOUTOracleSQL, std::string& strOUTMySqlSQL, std::string& strOUTSQLiteSQL )
{
	int nFunRes = 0;
	CSQLDataProcessor::getInstance().getDataForFileTypeOracleMySqlSQLite(m_strSqlId, m_strSqlLine, strINMySqlLine, strINSQLiteLine, strOUTDefineID, strOUTSQLID, strOUTCommonSQL,  strOUTOracleSQL, strOUTMySqlSQL, strOUTSQLiteSQL);
	return nFunRes;
}











