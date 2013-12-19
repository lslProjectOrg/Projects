/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/GenerateSQLFile.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin	$
  * 
  * to general sql.h sqlmacrodef.h by sqlcode.txt
  *
  */
  
 

#ifndef __DEF_CLASS_GENERATESQLFILE_HH__
#define __DEF_CLASS_GENERATESQLFILE_HH__
		   		 

#include "CommonDef.h"
#include "CommonData.h"

class CSQLCodeInfo;
			 
class CGenerateSQLFile
{
public:
	// sqlgen Constructors and destructor
	CGenerateSQLFile();
	~CGenerateSQLFile();	


private:
	CSQLCodeInfo*				m_pSQLCodeInfo;		
	VrtLinesConT				m_vrtDataInFile;


private:
	// The args input: inputfile outpath
	//eg.: "sqlgen.exe --inputfile=/u01/app/sqlcode.txt  --outpath=/u01/app/"
	std::string                 m_strParamProgName;
	std::string                 m_strParamInputFile;
	std::string                 m_strParamOutpath;

	//The directory: defSQLFileName_IN_SQLCODETXT  "sqlcode.txt" 
	std::string                 m_strDirSqlCodeTxt; 
	//The directory: defSQLFileName_Out_SQLH        "sql.h"			
	std::string                 m_strDirSqlH;  
	//The directory: defSQLFileName_Out_SQLMACRODEFH     "sqlmacrodef.h"	
	std::string                 m_strDirSqlMacrodefH;
	  
	//The directory: defSQLFilename_OUT_ERRORREPORT   "SqlGenErrorReport.log"
	//cd /u02/tabuild/3001_MYSQL/C955_Build/Base_3001/Base/code/transactive/core/data_access_interface
	//make DEBUG=1
	///export/home/tabuild/3001_TIP_NEW/Base_3001/Base/code/cots/sqlgen/1.1.0/bin/SOLARIS_5.10_i86pc/sqlgen --Inputfile=./src/sqlcode.txt --Outpath=./src
	//m_strDirErrorReport="../../../../SqlGenErrorReport.log" -- /u02/tabuild/3001_MYSQL/C955_Build/Base_3001/Base/SqlGenErrorReport.log
	std::string                 m_strDirErrorReport; 
	//m_strDirErrorReportReserve=./src/SqlGenErrorReport.log 
	std::string                 m_strDirErrorReportReserve;  

	//Ofstream of "sql.h"  
	std::ofstream				m_ofstreamSQLH;	 
	//Ofstream of "sqlmacrodef.h"
	std::ofstream				m_ofstreamSQLMACRODEFH; 
	std::string                 m_strFileTimeSqlcodeTXT;
	 
	
private:
	/*
	create and open sql.h sqlmacrodef.h, write all data to files
	*/
	int	_WriteDataToFiles();
	
	int	_OpenAllOutPutFiles(); 	
	int	_WriteBeginToFiles();		
	int	_WriteEndToFiles();
	int	_FormatToWriteFileTimeStr(const std::string& strSrc, std::string& strDest);
	int	_WriteTimeStrToFiles();
	int	_ClossAllOutPutFiles();

	int	_GeneralSQLFiles();
	
public:	
	/*
	//eg.: "sqlgen.exe --Inputfile=./src/sqlcode.txt --Outpath=./src"
	if not set "--inputfile="  or  "--outpath="
	use default value   "sqlgen.exe --inputfile=./sqlcode.txt  --outpath=./"
	*/
	int	setParam(int argc, char *argv[]);
	int	generalFiles();
		   	
//param
private:
	int	_ParseCmdLine(const VrtLinesConT& vecArgs);	
	bool _ExtractArg(const char* str, std::string& strParamName, std::string& strParamValue);
		  	
//check
private:	
	/*
	if  sql.h not exit 
	or sqlmacrodef.h not exit
	or sql.h update time is not the same as sqlcode.txt last modify time
	or sqlmacrodef.h update time is not the same as sqlcode.txt last modify time
	then
	remove old sql.h sqlmacrodef.h file 
	and create new sql.h sqlmacrodef.h file
	*/
	int	_CheckFileUpdateTime();
	std::string _GetFirstLineOfFile(const std::string& strFileName);
	std::string	_GetFileLastUpdateTime(const std ::string& strFilename);
	  
private:	
	/*
	check each line in sqlcode.txt
	line must have ';' or '+' or '=' or '[' or '[[' or 'AQ'
	*/
	int	_CheckLine(VrtLinesConT& vrtAllLines); 												
	int	_ReadFile(const std::string& strFileName, VrtLinesConT& vrtAllLines);
	int	_MutiLinesToSingleLine(VrtLinesConT& vrtAllLines);	
	int	_RemoveBlackLine(VrtLinesConT& vrtAllLines);	
	int	_TrimLines( VrtLinesConT& vrtAllLines );
	int _RemoveOldSQLFiles();
	
	/*
	* strFileName exist,but remove failed, return -1
	*/
	int _RemoveFile(const std::string& strFileName);
};


#endif  /// __DEF_CLASS_GENERATESQLFILE_HH__





