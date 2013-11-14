/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/SQLDataProcessor.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin	$
  * 
  * to declare some useful methods used in sqlgen
  *
  */


#ifndef __DEF_CLASS_SQLDATAPROCESSOR_HH__
#define __DEF_CLASS_SQLDATAPROCESSOR_HH__

#include <iostream>
#include <string>
#include "CommonDef.h"
#include "CommonData.h"


class CSQLDataProcessor
{
public:
	static CSQLDataProcessor& getInstance();
	static void removeInstance();

private:
	static CSQLDataProcessor* m_pInstance;
	
private:
	CSQLDataProcessor();
	~CSQLDataProcessor();	

public:	
	std::string getIndexTableName(const std::string& strTableName);
	int addDataToFileSQLH(const std::string& strLine);
	int addDataToFileSQLMACRODEFH(const std::string& strLine);
	
	int writeToFileSQLH(std::ofstream* pofstreamSQLH);
	int writeToFileSQLMACRODEFH(std::ofstream* pofstreamSQLMACRODEFH);

	
	int checkDuplicateSqlID(const std::string& strSqlIDCheck);
	
	int getDataForFileTypeCommon(
		const std::string& strINSqlID, 
		const std::string& strINSqlLine, 
		std::string& strOUTDefineID, 
		std::string& strOUTSQLID, 
		std::string& strOUTCommonSQL, 
		std::string& strOUTOracleSQL, 
		std::string& strOUTMySQLSQL);
		
	int getDataForFileTypeOracleMySql(
		const std::string& strINSqlID, 
		const std::string& strINOracleSQLLine, 
		const std::string& strINMySqlSQLLine,
		std::string& strOUTDefineID, 
		std::string& strOUTSQLID, 
		std::string& strOUTCommonSQL, 
		std::string& strOUTOracleSQL, 
		std::string& strOUTMySQLSQL);

private:	
	int _WriteToFile(VrtLinesConT& vrtLines, std::ofstream* pofstreamFile);

private:
	int m_nIndexTable;
	int m_nDefineIDIndex;
private:
	VrtLinesConT				m_vrtSQLH;
	VrtLinesConT				m_vrtSQLMAPDEFH;
	
	/*
	for sqlid Duplicate check 
	if there have same sqlID in sqlcode.txt
	in sqlmacrodef.h will not define Duplicate
	*/
	mapSqlIdLineConT			m_mapSqlIDDuplicateCheck; 

};




#endif // __DEF_CLASS_SQLDATAPROCESSOR_HH__















