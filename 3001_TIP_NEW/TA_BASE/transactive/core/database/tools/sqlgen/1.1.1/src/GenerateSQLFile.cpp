  /**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/GenerateSQLFile.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 
   
#include "GenerateSQLFile.h"

#ifndef WIN32
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <functional>
#include <sys/stat.h>
#include <windows.h>
#endif
		
#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"
#include "Logger.h"
#include "SQLDataProcessor.h"
#include "SQLCodeInfo.h"
#include <string>
				
CGenerateSQLFile::CGenerateSQLFile()
{	
	m_vrtDataInFile.clear();
	m_strParamProgName.clear();
	m_strParamInputFile.clear();
	m_strParamOutpath.clear();
	m_strDirSqlCodeTxt = defSQLFileName_IN_SQLCODETXT;
	m_strDirSqlH = defSQLFileName_Out_SQLH;
	m_strDirSqlMacrodefH = defSQLFileName_Out_SQLMACRODEFH;
	m_strDirErrorReport = defSQLFilename_OUT_ERRORREPORT;
	m_strDirErrorReportReserve = defSQLFilename_OUT_ERRORREPORT;
	m_strFileTimeSqlcodeTXT.clear();
	  	
 	m_pSQLCodeInfo = new CSQLCodeInfo();
 	CLogger::getInstance();
	CUtilityFun::getInstance();
	CSQLDataProcessor::getInstance();
}

CGenerateSQLFile::~CGenerateSQLFile()
{
	_ClossAllOutPutFiles();
	DEF_DELETE(m_pSQLCodeInfo);	
	m_vrtDataInFile.clear(); 	
	CLogger::removeInstance();
	CUtilityFun::removeInstance();
	CSQLDataProcessor::removeInstance();
}

int CGenerateSQLFile::setParam(int argc, char *argv[])
{
	int nFunRes = 0;
	VrtLinesConT vecArgs;
	std::string strLogError;

	if (1 == argc || argc > 3)
	{
		nFunRes = -1;
		return nFunRes;
	}

	for (int nIndex = 0; nIndex < argc; nIndex++)
	{
		vecArgs.push_back(argv[nIndex]);
	}

	nFunRes = _ParseCmdLine(vecArgs);
	vecArgs.clear();
	
	m_strDirSqlCodeTxt = defSQLFileName_IN_SQLCODETXT;
	m_strDirSqlH = defSQLFileName_Out_SQLH;
	m_strDirSqlMacrodefH = defSQLFileName_Out_SQLMACRODEFH;
	m_strDirErrorReport = defSQLFilename_OUT_ERRORREPORT;
	m_strDirErrorReportReserve = defSQLFilename_OUT_ERRORREPORT;
	if (false == m_strParamInputFile.empty())
	{
		m_strDirSqlCodeTxt = m_strParamInputFile;
	}

	if (false == m_strParamOutpath.empty())
	{
		m_strDirSqlH = m_strParamOutpath  + "/";
		m_strDirSqlH += defSQLFileName_Out_SQLH;

		m_strDirSqlMacrodefH = m_strParamOutpath  + "/";
		m_strDirSqlMacrodefH += defSQLFileName_Out_SQLMACRODEFH;

		//	/u02/tabuild/3001_MYSQL/C955_Build/Base_3001/Base/SqlGenErrorReport.log
		m_strDirErrorReport = "../../../../";
		m_strDirErrorReport += defSQLFilename_OUT_ERRORREPORT;
		
		// /u02/tabuild/3001_MYSQL/C955_Build/Base_3001/Base/code/transactive/core/data_access_interface/src/SqlGenErrorReport.log 
		m_strDirErrorReportReserve = m_strParamOutpath  + "/";
		m_strDirErrorReportReserve += defSQLFilename_OUT_ERRORREPORT; 
	} 	
	  
	return nFunRes;	
}



int CGenerateSQLFile::generalFiles()
{
	int nFunRes = 0;
	std::string strLogError;
	
	remove(m_strDirErrorReport.c_str());  //defSQLFilename_ErrorReport
	remove(m_strDirErrorReportReserve.c_str());  //defSQLFilename_ErrorReport

	nFunRes = CLogger::getInstance().setlogFile(m_strDirErrorReport);
	if (0 != nFunRes)
	{
		nFunRes = CLogger::getInstance().setlogFile(m_strDirErrorReportReserve);
		m_strDirErrorReport = m_strDirErrorReportReserve;	
	}																				
	CLogger::getInstance().setlogLevel(DebugError);

	nFunRes =  _CheckFileUpdateTime();
	if (0 == nFunRes)
	{
		strLogError = "sql.h sqlmacrodef.h files's update time is the same as sqlcode.txt, don't need GeneralSQLFiles()";
		CLogger::getInstance().deletelogFile();
		nFunRes = 0;
		return nFunRes;
	}

	//remove  *.o files
#ifndef WIN32
	system("make DEBUG=1 clean");
	printf("make DEBUG=1 clean \n"); 
#endif

	nFunRes = _GeneralSQLFiles();
	if (0 != nFunRes)
	{
		strLogError = "error! _GeneralSQLFiles()";
		nFunRes = -1;
		return nFunRes;
	}
	return nFunRes;
}

int CGenerateSQLFile::_FormatToWriteFileTimeStr(const std::string& strSrc, std::string& strDest)
{
	int nFunRes = 0;

	strDest.clear();

	strDest = "//[sqlcode.txt file time=";
	strDest += strSrc;
	strDest += "]";

	return nFunRes;
}

int CGenerateSQLFile::_RemoveFile(const std::string& strFileName)
{
	int nFunRes = 0;
	std::ifstream IfstreamSrcFile;

#ifdef WIN32
	BOOL bSetRes = FALSE; 	
	IfstreamSrcFile.open(strFileName.c_str());
	if (IfstreamSrcFile.is_open())
	{
		IfstreamSrcFile.close();
		bSetRes = ::SetFileAttributesA((LPCSTR)strFileName.c_str(), FILE_ATTRIBUTE_NORMAL);			
		if (!bSetRes)
		{
			DWORD dwErr = GetLastError();
			_SysLog(SourceFLInfo, DebugError, "error! SetFileAttributesA %s, error=%d", strFileName.c_str(), dwErr);
		}
		
		//remove old files
		//strFileName exist,but remove failed, return -1
		nFunRes = remove(strFileName.c_str());
	}
	else
	{
		IfstreamSrcFile.close();
	}
#endif

	//not need to check remove res here	
	remove(strFileName.c_str());  

	return nFunRes;
}


int CGenerateSQLFile::_RemoveOldSQLFiles()
{
	int nFunRes = 0;
	
	nFunRes = _RemoveFile(m_strDirSqlH);
	if (0 != nFunRes)
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	nFunRes = _RemoveFile(m_strDirSqlMacrodefH);
	if (0 != nFunRes)
	{
		nFunRes = -1;
		return nFunRes;
	}
	
	//remove(m_strDirErrorReport.c_str());  //defSQLFilename_ErrorReport
	
	return nFunRes;
}


int CGenerateSQLFile::_GeneralSQLFiles()
{
	int nFunRes = 0;

	nFunRes = _RemoveOldSQLFiles();
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! remove OldSQLFiles: %s %s",
				m_strDirSqlH.c_str(), m_strDirSqlMacrodefH.c_str());
		nFunRes = -1;
		return nFunRes;
	}

	//read file
	nFunRes =  _ReadFile(m_strDirSqlCodeTxt, m_vrtDataInFile);
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! _ReadAllLineFromFile()");
		nFunRes = -1;
		return nFunRes;
	}

	//check file data
	nFunRes =  _CheckLine(m_vrtDataInFile);
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! _CheckLine()");
		nFunRes = -1;
		return nFunRes;
	}
	
	//process data
	nFunRes =  _MutiLinesToSingleLine(m_vrtDataInFile);
	nFunRes =  _RemoveBlackLine(m_vrtDataInFile);
	
	//m_strDirSqlCodeTxt
	nFunRes = m_pSQLCodeInfo->setFileName(m_strDirSqlCodeTxt);	
	nFunRes =  m_pSQLCodeInfo->analyzeData(m_vrtDataInFile);
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! CSQLCodeInfo analyzeData()");
		nFunRes = -1;
		return nFunRes;
	}
	
	//check Oracle and Mysql sql Num
	nFunRes = m_pSQLCodeInfo->checkOracleAndMysqlSqlNum();
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! checkOracleAndMysqlSqlNum()");
		nFunRes = -1;
		return nFunRes;
	}

	nFunRes = m_pSQLCodeInfo->analyzeDataToFiles();
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! analyzeDataToFiles()");
		nFunRes = -1;
		return nFunRes;
	} 

	//write data to sql.h sqlmacrodef.h
	nFunRes = _WriteDataToFiles();

	//clear data
	if (NULL != m_pSQLCodeInfo)
	{
		delete m_pSQLCodeInfo;
		m_pSQLCodeInfo = NULL;
	}
	m_vrtDataInFile.clear();

	//remove file ErrorReport.log
	if (0 == nFunRes)
	{
		CLogger::getInstance().deletelogFile();
	}

	return nFunRes;	 
}

int CGenerateSQLFile::_ReadFile( const std::string& strFileName, VrtLinesConT& vrtAllLines )
{
	int nFunRet = 0;
	std::ifstream IfstreamSrcFile;
	char* pszInBuff = NULL;
	int nFilelines = 0;	 

	if (strFileName.empty())
	{
		nFunRet = -1;
		return nFunRet;
	}

	IfstreamSrcFile.open(strFileName.c_str());
	if (IfstreamSrcFile.fail())
	{
		_SysLog(SourceFLInfo, DebugError, "error! open file error. FileName=%s", strFileName.c_str());
		nFunRet = -1;
		return nFunRet;
	}

	pszInBuff = new char[DEF_INT_MAXLINELEN];
	memset(pszInBuff, 0, DEF_INT_MAXLINELEN);

	// Read in all lines and add them to a vector
	while ( IfstreamSrcFile.getline ( pszInBuff, DEF_INT_MAXLINELEN ) )
	{
		nFilelines++;
		if (LINE_COMMONT != pszInBuff[0])
		{
			std::string strLine = pszInBuff;
			
			CUtilityFun::getInstance().removeUnVisableStr(strLine);
			CUtilityFun::getInstance().trim(strLine);

			//check line is not empty or commont line
			if (false == strLine.empty() && ';' != strLine[0])
			{
				vrtAllLines.push_back( strLine );
			}	
		}
		memset(pszInBuff, 0, DEF_INT_MAXLINELEN);
	}//while

	// close file
	if (IfstreamSrcFile.is_open())
	{
		IfstreamSrcFile.close();
	}

	if (NULL != pszInBuff)
	{
		delete []pszInBuff;
		pszInBuff = NULL;
	}
	nFunRet = 0;
	return nFunRet;
}

int CGenerateSQLFile::_TrimLines( VrtLinesConT& vrtAllLines )
{
	int nFunRet = 0;
	VrtLinesConIterT iterAllLine;

	iterAllLine = vrtAllLines.begin();
	while ( iterAllLine != vrtAllLines.end() )
	{			
		CUtilityFun::getInstance().trim(*iterAllLine);
	
		iterAllLine++;
	}//while  
	return nFunRet;
}

int CGenerateSQLFile::_MutiLinesToSingleLine( VrtLinesConT& vrtAllLines )
{
	int nFunRet = 0; 
	std::string strGetLine;
	std::string strTmp;
	VrtLinesConIterT iterAllLine = vrtAllLines.begin();
	VrtLinesConIterT iterTmp = vrtAllLines.begin();	
	bool bFindJia = false;
				  
	strGetLine.clear();
	strTmp.clear();

	while ( iterAllLine != vrtAllLines.end() )
	{
		strGetLine.clear();
		strGetLine = (*iterAllLine);

		if (!strGetLine.empty())
		{ 		
			bFindJia = false;
			if (LINE_ADD == strGetLine[0])
			{
				bFindJia = true;
			} 	

			if (bFindJia)
			{
				strTmp += "\\";
				strTmp += "\n";
				strTmp +=strGetLine.substr(1);   //'+' -- '\'  '\r\n'
				(*iterAllLine) = "";
			}
			else
			{
				if (!strTmp.empty())
				{
					//strTmp += "\r\n";
					(*iterTmp) = strTmp;
					strTmp.clear();
				}
				iterTmp = iterAllLine;
				strTmp +=strGetLine;
			}
		}//if 

		iterAllLine++;
	}//while

	//last one line
	if ((iterAllLine == vrtAllLines.end()) && (iterTmp != vrtAllLines.end()))
	{
		if (!strTmp.empty())
		{
			//strTmp += "\r\n";
			(*iterTmp) = strTmp;
			strTmp.clear();
			strGetLine.clear();	 
		}
	}
	  
	nFunRet = 0;
	return nFunRet;
}

int CGenerateSQLFile::_RemoveBlackLine( VrtLinesConT& vrtAllLines )
{
	int nFunRet = 0; 
	VrtLinesConIterT vtrLineIter;
	
	_TrimLines(vrtAllLines);

	vtrLineIter = vrtAllLines.begin();
	while (vtrLineIter != vrtAllLines.end())
	{
		if ( (*vtrLineIter).empty())
		{
			vrtAllLines.erase(vtrLineIter);
			vtrLineIter = vrtAllLines.begin();
		}
		else
		{
			vtrLineIter++;	
		}  			
	}//while

	return nFunRet;
}

int CGenerateSQLFile::_CheckLine( VrtLinesConT& vrtAllLines )
{
	int							nFunRes = 0;
	VrtLinesConIterT			iterAllLine;
	std::string					strOneLineTmp;

	iterAllLine = vrtAllLines.begin();
	while ( iterAllLine != vrtAllLines.end() )
	{
		strOneLineTmp.clear();
		strOneLineTmp = *iterAllLine;

		if (!strOneLineTmp.empty()
			&&std::string::npos == strOneLineTmp.find(";")
			&& std::string::npos == strOneLineTmp.find("+")
			&& std::string::npos == strOneLineTmp.find("=")
			&& std::string::npos == strOneLineTmp.find("[")
			&& std::string::npos == strOneLineTmp.find("[[")
			&& std::string::npos == strOneLineTmp.find("AQ"))
		{
			_SysLog(SourceFLInfo, DebugError, "inVaild Line:%s", strOneLineTmp.c_str());
			nFunRes = -1;
			break;
		}//if 

		iterAllLine++;
	}//while

	return nFunRes;
}

int CGenerateSQLFile::_OpenAllOutPutFiles()
{
	int nFunRet = 0;

	m_ofstreamSQLH.open(m_strDirSqlH.c_str());
	if (m_ofstreamSQLH.fail())
	{
		_SysLog(SourceFLInfo, DebugError, "open outPut file error, fileName=%s", m_strDirSqlH.c_str());
		m_ofstreamSQLH.close();
		nFunRet = -1;
		return nFunRet;
	}

	m_ofstreamSQLMACRODEFH.open(m_strDirSqlMacrodefH.c_str());
	if (m_ofstreamSQLMACRODEFH.fail())
	{
		_SysLog(SourceFLInfo, DebugError, "open outPut file error, fileName=%s", m_strDirSqlMacrodefH.c_str());
		m_ofstreamSQLMACRODEFH.close();
		nFunRet = -1;
		return nFunRet;
	}

	return nFunRet;
}

int CGenerateSQLFile::_ClossAllOutPutFiles()
{
	int nFunRet = 0;

	if (m_ofstreamSQLH.is_open())
	{
		m_ofstreamSQLH.close();
	}
	if (m_ofstreamSQLMACRODEFH.is_open())
	{
		m_ofstreamSQLMACRODEFH.close();
	}

	return nFunRet;
}


int CGenerateSQLFile::_WriteTimeStrToFiles()
{
	int nFunRet = 0;
	std::string strFormatRes;

	if (m_strFileTimeSqlcodeTXT.empty() && !m_strDirSqlCodeTxt.empty())
	{
		m_strFileTimeSqlcodeTXT = _GetFileLastUpdateTime(m_strDirSqlCodeTxt);
	}

	_FormatToWriteFileTimeStr(m_strFileTimeSqlcodeTXT, strFormatRes);

	if (m_ofstreamSQLH.is_open())
	{
		m_ofstreamSQLH << strFormatRes << std::endl; 
	}
	if (m_ofstreamSQLMACRODEFH.is_open())
	{
		m_ofstreamSQLMACRODEFH << strFormatRes << std::endl;
	}
	   
	return nFunRet;	
}

int CGenerateSQLFile::_WriteBeginToFiles()
{
	int nFunRet = 0;

	if (m_ofstreamSQLH.is_open())
	{
		m_ofstreamSQLH << SQLH_SQLMACRODEFH_FILE_COMMENT_HEADER << std::endl;
		m_ofstreamSQLH << SQLH_FILE_BEGIN << std::endl;
	}
	
	if (m_ofstreamSQLMACRODEFH.is_open())
	{
		m_ofstreamSQLMACRODEFH << SQLH_SQLMACRODEFH_FILE_COMMENT_HEADER << std::endl;
		m_ofstreamSQLMACRODEFH << SQLMACRODEFH_FILE_BEGIN << std::endl;
	}

	return nFunRet;	
}

int CGenerateSQLFile::_WriteDataToFiles()
{
	int nFunRes = 0; 
	
	nFunRes = _OpenAllOutPutFiles();
	if (0 != nFunRes)
	{
		_SysLog(SourceFLInfo, DebugError, "error! _OpenAllOutPutFiles()");	
		nFunRes = -1;
		return nFunRes;
	}
	nFunRes = _WriteTimeStrToFiles(); 	
	nFunRes = _WriteBeginToFiles();	
	CSQLDataProcessor::getInstance().writeToFileSQLH(&m_ofstreamSQLH);
	CSQLDataProcessor::getInstance().writeToFileSQLMACRODEFH(&m_ofstreamSQLMACRODEFH);
	
	nFunRes = _WriteEndToFiles();
	nFunRes = _ClossAllOutPutFiles();

	return nFunRes;	
}

int CGenerateSQLFile::_WriteEndToFiles()
{
	int nFunRes = 0;

	if (m_ofstreamSQLH.is_open())
	{
		m_ofstreamSQLH << SQLH_FILE_END << std::endl; 
	}
	if (m_ofstreamSQLMACRODEFH.is_open())
	{
		m_ofstreamSQLMACRODEFH << SQLMACRODEFH_FILE_END << std::endl; 
	}

	return nFunRes;
}

int CGenerateSQLFile::_ParseCmdLine(const VrtLinesConT& vecArgs)
{
	int nFunRes = 0;
	unsigned int nIndex = 0;
	std::string strDefInputFile = def_Str_Argv_inputfile;
	std::string strDefOutPath = def_Str_Argv_outpath; 
	std::string strParamName;
	std::string strParamValue;
	const char* str = NULL;

	// First arg is USUALLY the program name
	if (vecArgs.size() > 0)
	{
		if (vecArgs[0].size() > 0)
		{
			if (vecArgs[0][0] != '-')
			{
				m_strParamProgName = vecArgs[0];
				nIndex = 1;
			}
		}
	}
	
	//defualt value
	m_strParamInputFile = defSQLFileName_IN_SQLCODETXT;
	m_strParamOutpath = "./";

	// The rest should be standard parameters	
	for (; nIndex < vecArgs.size(); nIndex++)
	{
		str = vecArgs[nIndex].c_str();
		if (!_ExtractArg(str, strParamName, strParamValue))
		{
			_SysLog(SourceFLInfo, DebugError, "Err: _ExtractArg() %s", str);
			nFunRes = -1;
			return nFunRes;
		}

		if (!strParamName.empty())
		{
			if (0 == CUtilityFun::getInstance().sysStricmp(strDefInputFile.c_str(), strParamName.c_str()))
			{
				m_strParamInputFile	= strParamValue;
			}
			else if (0 == CUtilityFun::getInstance().sysStricmp(strDefOutPath.c_str(), strParamName.c_str()))
			{
				m_strParamOutpath	= strParamValue;
			}

		}

	}//for

	return nFunRes;
}


bool CGenerateSQLFile::_ExtractArg(const char* str, std::string& strParamName, std::string& strParamValue)
{
	bool status = true;
	strParamName = "";
	strParamValue = "";

	// Skip leading whitespace
	while (isspace(*str))
	{
		str++;
	}

	// Expect the name to start with two hyphens followed by an alpha
	if (strncmp(str, "--", 2) == 0)
	{
		str += 2;
	}
	else
	{
		status = false;
	}

	if (!isalpha(*str))
	{
		status = false;
	}

	// Extract the name. Hyphens are skipped but cause the next character
	// to be upshifted

	bool upshift = true;
	while (status && *str != '\0' && *str != '=' && !isspace(*str))
	{
		if (*str == '-')
		{
			upshift = true;
		}
		else if (upshift)
		{
			strParamName += toupper(*str);
			upshift = false;
		}
		else
		{
			strParamName += *str;
		}
		str++;
	}

	// If we have an "=" then extract the value
	if (status && *str == '=')
	{
		str++;
		while (*str != '\0')
		{
			strParamValue += *str;
			str++;
		}
	}

	return status;
}

std::string CGenerateSQLFile::_GetFirstLineOfFile(const std::string& strFileName)
{
	std::ifstream IfstreamSrcFile;
	char* pszInBuff = NULL;
	char* pLineEndPos = NULL;
	std::string strFirstLine;

	strFirstLine.clear();
	//get the file time of sql.h
	IfstreamSrcFile.open(strFileName.c_str());
	if (IfstreamSrcFile.fail())
	{
		_SysLog(SourceFLInfo, DebugError, "file open error! file name=%s", strFileName.c_str());
		IfstreamSrcFile.close();
		return strFirstLine;
	}

	pszInBuff = new char[DEF_INT_MAXLINELEN];
	memset(pszInBuff, 0, DEF_INT_MAXLINELEN);

	IfstreamSrcFile.getline ( pszInBuff, DEF_INT_MAXLINELEN );
	strFirstLine = pszInBuff;

	if (NULL != pszInBuff)
	{
		delete []pszInBuff;
		pszInBuff = NULL;
	}


	if (IfstreamSrcFile.is_open())
	{
		IfstreamSrcFile.close();
	}
	
	return strFirstLine;
}

std::string CGenerateSQLFile::_GetFileLastUpdateTime(const std ::string& strFilename)
{
	std::string strFileLastUpdateTime;
	struct tm* pTm = NULL;
	time_t nFileMtTime;
	std::ifstream inFile;
	char* pszCurTime = NULL;
	int nStatFunRes = 0;
	struct stat statFileInfo ;

	strFileLastUpdateTime.clear();
	inFile.open(strFilename.c_str());
	if (inFile.fail())
	{	
		inFile.close();
		strFileLastUpdateTime.clear();
		return strFileLastUpdateTime;
	}

	nStatFunRes = stat( strFilename.c_str(), &statFileInfo );

	pszCurTime = new char[DEF_INT_BUFFSIZE];
	memset(pszCurTime, 0, DEF_INT_BUFFSIZE);
	//get file last update time
	nFileMtTime = statFileInfo.st_mtime;			
	pTm = localtime(&(nFileMtTime)); 
	if (NULL != pTm)
	{
		sprintf(pszCurTime, "%04d/%02d/%02d %02d:%02d:%02d",
			pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
			pTm->tm_hour, pTm->tm_min, pTm->tm_sec); 
	}		
	strFileLastUpdateTime = pszCurTime;

	if (NULL != pszCurTime)
	{
		delete[] pszCurTime;
		pszCurTime = NULL;
	}

	if (inFile.is_open())
	{
		inFile.close();
	}

	return strFileLastUpdateTime;
}


int CGenerateSQLFile::_CheckFileUpdateTime()
{
	int nFunRes = 0;
	std::string strFileTimeSqlH;
	std::string strFileTimeSqlmacrodefH;
		 
	//get the file time of sql.h
	strFileTimeSqlH = _GetFirstLineOfFile(m_strDirSqlH);
	if (strFileTimeSqlH.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	//get the file time of sqlmacrodef.h
	strFileTimeSqlmacrodefH = _GetFirstLineOfFile(m_strDirSqlMacrodefH);	
	if (strFileTimeSqlmacrodefH.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	m_strFileTimeSqlcodeTXT = _GetFileLastUpdateTime(m_strDirSqlCodeTxt);	
	if (m_strFileTimeSqlcodeTXT.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}

	//check time
	if ((std::string::npos != strFileTimeSqlH.find(m_strFileTimeSqlcodeTXT))
		&& (std::string::npos != strFileTimeSqlmacrodefH.find(m_strFileTimeSqlcodeTXT)))
	{
		nFunRes = 0;
	}
	else
	{
		nFunRes = -1;
	}	

	return nFunRes;
}



