/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/SQLDataProcessor.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 
#include "SQLDataProcessor.h"
 
#include <sstream>
#include <algorithm>
#include <cctype>
#include <functional> 

#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"
#include "Logger.h"


CSQLDataProcessor* CSQLDataProcessor::m_pInstance = 0;

CSQLDataProcessor& CSQLDataProcessor::getInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CSQLDataProcessor();
	} 
	return (*m_pInstance);
}

void CSQLDataProcessor::removeInstance()
{
	DEF_DELETE(m_pInstance);
}


CSQLDataProcessor::CSQLDataProcessor()
{
   m_nIndexTable = 0;
   m_nDefineIDIndex = 0;
   m_vrtSQLH.clear();
   m_vrtSQLMAPDEFH.clear();
   m_mapSqlIDDuplicateCheck.clear();

}

CSQLDataProcessor::~CSQLDataProcessor()
{
	m_vrtSQLH.clear();
	m_vrtSQLMAPDEFH.clear();
	m_mapSqlIDDuplicateCheck.clear();
}



int CSQLDataProcessor::checkDuplicateSqlID(const std::string& strSqlIDCheck)
{
	int nFunRes = 0;

	if (strSqlIDCheck.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	if (m_mapSqlIDDuplicateCheck.end() == m_mapSqlIDDuplicateCheck.find(strSqlIDCheck))
	{
		std::string strNewSqlID = strSqlIDCheck;
		std::string strNewSqlIDValue = "#define    " + strSqlIDCheck;
		m_mapSqlIDDuplicateCheck.insert(mapSqlIdLineConValueT(strNewSqlID, strNewSqlIDValue));
		nFunRes = 0;		
	}
	else
	{
		//find same sqlID
		nFunRes = -1;
		return nFunRes;
	}

	return nFunRes;
}

std::string CSQLDataProcessor::getIndexTableName(const std::string& strTableName)
{
	std::string strIndexTableName;
	char* pszTemp = NULL;
	pszTemp = new char[DEF_INT_BUFFSIZE];
	
	memset(pszTemp, 0, DEF_INT_BUFFSIZE);
	sprintf(pszTemp, "\n//[%d]%s\n", m_nIndexTable, strTableName.c_str()); //[0][ALARMRULE]
	m_nIndexTable++;

	strIndexTableName = pszTemp;
	DEF_DELETE_ARRAY(pszTemp);
	
	return strIndexTableName;
}



int CSQLDataProcessor::addDataToFileSQLH(const std::string& strLine)
{
	int nFunRes = 0;
	m_vrtSQLH.push_back(strLine);
	return nFunRes;
}



int CSQLDataProcessor::addDataToFileSQLMACRODEFH(const std::string& strLine)
{
	int nFunRes = 0;
	m_vrtSQLMAPDEFH.push_back(strLine);
	return nFunRes;
}




int CSQLDataProcessor::getDataForFileTypeCommon(
const std::string& strINSqlID, 
const std::string& strINSqlLine, 
std::string& strOUTDefineID, 
std::string& strOUTSQLID, 
std::string& strOUTCommonSQL, 
std::string& strOUTOracleSQL, 
std::string& strOUTMySQLSQL)
{
	int         nFunRes = 0;
	std::string strIndex;
	std::string strBeginParentheses	= "\t{\n";
	std::string strEndParentheses	= "\n\t},\n";

	std::string strTmpSQLID = "\t\t/*SQLID*/";
	std::string strTmpCommonSQL = "\t\t/*CommonSQL*/";
	std::string strTmpOracleSQL = "\t\t/*OracleSQL*/";
	std::string strTmpMySQLSQL = "\t\t/*MySQLSQL*/";
	std::string strQuotationMarks = "\"";
	std::string strBeginQuotationMarks = "(\"";
	std::string strEndQuotationMarks = "\"),\n";
	std::string strEndQuotationMarksLast = "\")\t";
	
	
	/*sqlmadef.h*/
	strIndex = CUtilityFun::getInstance().int2str(m_nDefineIDIndex);
	//[0][AlarmState]
	//[[common]]
	//#define 	ALARMSTATE_SELECT_71001 	 "ALARMSTATE_SELECT_71001"
	strOUTDefineID = "#define \t" + strINSqlID + " \t " + strQuotationMarks + strINSqlID + strQuotationMarks + "\n";	 	

	/*sql.h*/
	//[0][AlarmState]
	//--{	/*SQLID*/("ALARMSTATE_SELECT_71001"), 
	//--	/*CommonSQL*/("SELECT Pkey,Name, DisplayName, TypeKey FROM AlarmState"), 
	//--	/*OracleSQL*/(""), 
	//--	/*MySQLSQL*/(""), 

	//			  \t{\t					/*SQLID*/	  ("					   ALARMSTATE_SELECT_71001	   "),\n
	strOUTSQLID = strBeginParentheses + strTmpSQLID + strBeginQuotationMarks + strINSqlID  + strEndQuotationMarks;

	//					/*CommonSQL*/	  ("					   SELECT Pkey,Name, DisplayName, TypeKey FROM AlarmState               "),\n
	strOUTCommonSQL =   strTmpCommonSQL + strBeginQuotationMarks + strINSqlLine + strEndQuotationMarks;

	//					/*OracleSQL*/	  ("					   "),\n
	strOUTOracleSQL =   strTmpOracleSQL + strBeginQuotationMarks + strEndQuotationMarks;

	//					/*MySQLSQL*/	  ("					 ")                       \t},
	strOUTMySQLSQL =   strTmpMySQLSQL + strBeginQuotationMarks + strEndQuotationMarksLast + strEndParentheses;

	m_nDefineIDIndex += DB_NUM;	 

	return nFunRes;	 
}

int CSQLDataProcessor::getDataForFileTypeOracleMySql(
const std::string& strINSqlID, 
const std::string& strINOracleSQLLine, 
const std::string& strINMySqlSQLLine,
std::string& strOUTDefineID, 
std::string& strOUTSQLID, 
std::string& strOUTCommonSQL, 
std::string& strOUTOracleSQL, 
std::string& strOUTMySQLSQL)
{
	int         nFunRes = 0;
	std::string strIndex;
	std::string strBeginParentheses	= "\t{\n";
	std::string strEndParentheses	= "\n\t},\n";

	std::string strTmpSQLID = "\t\t/*SQLID*/";
	std::string strTmpCommonSQL = "\t\t/*CommonSQL*/";
	std::string strTmpOracleSQL = "\t\t/*OracleSQL*/";
	std::string strTmpMySQLSQL = "\t\t/*MySQLSQL*/";
	std::string strQuotationMarks = "\"";
	std::string strBeginQuotationMarks = "(\"";
	std::string strEndQuotationMarks = "\"),\n";
	std::string strEndQuotationMarksLast = "\")\t";



	/*sqlmadef.h*/
	strIndex = CUtilityFun::getInstance().int2str(m_nDefineIDIndex);

	//#define 	ALARMSTATE_SELECT_71001 	 "ALARMSTATE_SELECT_71001"
	strOUTDefineID = "#define \t" + strINSqlID + " \t " + strQuotationMarks + strINSqlID + strQuotationMarks + "\n";	 	
			 
	/*sql.h*/
	//--{	/*SQLID*/("ALARMSTATE_SELECT_71001"), 
	//--	/*CommonSQL*/(""), 
	//--	/*OracleSQL*/("SELECT Pkey,Name, DisplayName, TypeKey FROM AlarmState"), 
	//--	/*MySQLSQL*/("SELECT Pkey,Name, DisplayName, TypeKey FROM AlarmState")	},
	
	//			  \t{\t					/*SQLID*/	  ("					   ALARMSTATE_SELECT_71001	   "),\n
	strOUTSQLID = strBeginParentheses + strTmpSQLID + strBeginQuotationMarks + strINSqlID  + strEndQuotationMarks;
	
	//					/*CommonSQL*/	  ("					   "),\n
	strOUTCommonSQL =   strTmpCommonSQL + strBeginQuotationMarks + strEndQuotationMarks;
	
	//					/*OracleSQL*/	  ("					   SELECT Pkey,Name, DisplayName, TypeKey FROM AlarmState     "),\n
	strOUTOracleSQL =   strTmpOracleSQL + strBeginQuotationMarks + strINOracleSQLLine + strEndQuotationMarks;

	//					/*MySQLSQL*/	  ("					   SELECT Pkey,Name, DisplayName, TypeKey FROM AlarmState     ") \t},
	strOUTMySQLSQL =   strTmpMySQLSQL + strBeginQuotationMarks + strINMySqlSQLLine + strEndQuotationMarksLast + strEndParentheses;
		
	m_nDefineIDIndex += DB_NUM;	 
	
	return nFunRes;
}




int CSQLDataProcessor::_WriteToFile(VrtLinesConT& vrtLines, std::ofstream* pofstreamFile)
{
	int nFunRes = 0;
	VrtLinesConIterT iterVrt;	

	if (NULL == pofstreamFile || false == pofstreamFile->is_open())
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	iterVrt = vrtLines.begin();
	while (iterVrt != vrtLines.end())
	{ 
		if (pofstreamFile->is_open())
		{
			*pofstreamFile << *iterVrt ;//<< std::endl; 
		}
		iterVrt++;
	} 
	
	return nFunRes;
}

int CSQLDataProcessor::writeToFileSQLH(std::ofstream* pofstreamSQLH)
{
	int nFunRes = 0;
	
	nFunRes = _WriteToFile(m_vrtSQLH, pofstreamSQLH);
				  	
	return nFunRes;
}


int CSQLDataProcessor::writeToFileSQLMACRODEFH(std::ofstream* pofstreamSQLMACRODEFH)
{
	int nFunRes = 0;

	nFunRes = _WriteToFile(m_vrtSQLMAPDEFH, pofstreamSQLMACRODEFH);

	return nFunRes;	
}













