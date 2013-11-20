/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/Logger.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */

#include "Logger.h"

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <stdio.h>

#include "CommonDef.h"
#include "CommonData.h"


CLogger* CLogger::m_pInstance = 0;
						
CLogger& CLogger::getInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CLogger();
	}
	return (*m_pInstance);
}

void CLogger::removeInstance()
{
	DEF_DELETE(m_pInstance);
}


CLogger::CLogger()
{
	m_strLogFileName = defSQLFilename_OUT_ERRORREPORT;
	m_nCurrDebugLevel = DebugError;
}


CLogger::~CLogger()
{
    if (m_streamLogFile.is_open())
    {
		m_streamLogFile.close();
    }
}




static DebugLevelNameValue nameValueMapping[] =
{
	{ "DebugFatal",		DebugFatal		},
	{ "DebugError",		DebugError		},
	{ "DebugWarn",		DebugWarn		},
	{ "DebugNorm",		DebugNorm		},
	{ "DebugInfo",		DebugInfo		},
	{ "DebugDebug",		DebugDebug		},
	{ "DebugSQL",        DebugSQL         },
	{ "DebugCORBA",      DebugCORBA       },
	{ "DebugMsg",	    DebugMsg	        },
	{ "DebugMsgPub",	    DebugMsgPub	    },
	{ "DebugPeriodic",   DebugPeriodic    },
	{ "DebugDatabase",	DebugDatabase	},
	{ "DebugExternal",	DebugExternal	},
	{ "DebugTrace",		DebugTrace		},
	{ NULL,			DebugInfo		}
};


std::string CLogger::_GetStringDebugLevel(EDebugLevel level)
{
	std::string strLevel;
	DebugLevelNameValue* pItem = nameValueMapping;

	while (pItem->pLevelName != NULL)
	{
		if (level == pItem->nLevelValue)
		{
			strLevel = pItem->pLevelName;
			break;
		}
		pItem++;
	}

	return strLevel;
}

void CLogger::deletelogFile()
{
	if (m_streamLogFile.is_open())
	{
		m_streamLogFile.close();
	}
	remove(m_strLogFileName.c_str());
	m_strLogFileName.clear();

}

int CLogger::setlogFile(const std::string& strLogFileName)
{
	int nFunRes = 0;
	m_strLogFileName = strLogFileName;
	if (m_streamLogFile.is_open())
	{
		m_strLogFileName.clear();
		m_streamLogFile.close();
	}
	// Open the file for appending (retain existing entries) 
	m_streamLogFile.open(m_strLogFileName.c_str(), std::ios::out | std::ios::app);
	if (m_streamLogFile.is_open())
	{
		m_streamLogFile << m_strLogFileName.c_str() << std::endl;
		m_streamLogFile << "****************************************************************************************" << std::endl;
		m_streamLogFile.flush();
		nFunRes = 0;
	}
	else
	{
		m_streamLogFile.close();
		nFunRes = -1;
	}

	return nFunRes;
}

void CLogger::setlogLevel(EDebugLevel nLoglevel)
{
	m_nCurrDebugLevel = nLoglevel;
}

void CLogger::log(const std::string& file, int line, EDebugLevel level, ...)
{
	int nGetlevel = 0;
	std::string strLogLevel;  
	int nStrLen = DEF_INT_MAXLINELEN;
	std::string logMsg;

	if ( level > m_nCurrDebugLevel )
	{
		return;
	}

	// Get the variable args
	va_list argPtr;

	//argPtr piont to level's pos
	va_start ( argPtr, level );
		
	// Construct the log message
	strLogLevel = _GetStringDebugLevel( ( EDebugLevel )level );	
	logMsg = _ConstructMessage (nStrLen, file, line, strLogLevel, argPtr);
	
	// Reset the arg pointer
	va_end ( argPtr );

	// Print the log message
	if (m_streamLogFile.is_open())
	{	 
		m_streamLogFile << logMsg.c_str();
		m_streamLogFile.flush();
	}
	else
	{
		printf("log: %s \r\n", logMsg.c_str());
	}

	return;
}




std::string CLogger::_ConstructMsgBody(int nStrLen, va_list args)
{
	char* format = "";
	std::string testFormat;
	std::string strMsgBody;
	char* buff = NULL;
	int ret = -1;

	format = va_arg ( args, char* );
	testFormat = format;

	// Construct message from format string and args
	strMsgBody.clear();


	buff = new char[nStrLen];
	memset(buff, 0, nStrLen);

	ret = vsnprintf ( buff, nStrLen, format, args ); 
	strMsgBody = buff;

	//message = message.insert ( 0, "    "); // Preprend indentation
	//message += "\n";

	if (NULL != buff)
	{
		delete[] buff;
		buff = NULL;
	}
	return strMsgBody;
}


std::string CLogger::_ConstructMsgHeader(int nStrLen, const std::string& file, int line, std::string& strLogLevel)
{  
	std::string strLogMsgHeader;
	struct timeb timebuffer;
	struct tm newtime;
	std::string strLogMsgHeaderDateTime;///t[11/01/2013 14:44:07:906]
	std::string strLogMsgHeaderLogLevel;//[INFO]
	std::string strLogMsgHeaderThreadId;//[Thrd: 6248000]
	std::string strLogMsgHeaderFileName;//d:\googledrive1987.cpp:
	std::string strLogMsgHeaderLineNumber;   //234:

	char* pszBuffLogHeader = NULL;
	pszBuffLogHeader = new char[nStrLen];
	memset(pszBuffLogHeader, 0, nStrLen);

	//construct log header

	// Retrieve Time and Date
	ftime ( &timebuffer ); 

#if defined ( SOLARIS ) || defined( LINUX )
	newtime = *localtime_r ( &timebuffer.time, &newtime );
#else
	newtime = *localtime ( &timebuffer.time );   
#endif

	strLogMsgHeaderDateTime.clear();
	if ( ( &newtime ) != NULL )
	{
		sprintf ( pszBuffLogHeader, " [%02d/%02d/%02d %02d:%02d:%02d:%03d]", 
			newtime.tm_mday, newtime.tm_mon + 1, newtime.tm_year + 1900, 
			newtime.tm_hour, newtime.tm_min, newtime.tm_sec, timebuffer.millitm );
		//strLogMsgHeaderDateTime += "\t";
		strLogMsgHeaderDateTime = pszBuffLogHeader;
	}

	strLogMsgHeaderLogLevel.clear();
	strLogMsgHeaderLogLevel += " [";
	strLogMsgHeaderLogLevel += strLogLevel;
	strLogMsgHeaderLogLevel += "]";

	// Retrieve Thread Id
	memset(pszBuffLogHeader, 0, nStrLen);
	strLogMsgHeaderThreadId.clear();

	sprintf ( pszBuffLogHeader, " [Thrd: %d]", 1);
	strLogMsgHeaderThreadId = pszBuffLogHeader;

	strLogMsgHeaderFileName.clear();
	strLogMsgHeaderFileName = " " + file;
	strLogMsgHeaderFileName += ":";

	// Convert line to string

	memset(pszBuffLogHeader, 0, nStrLen);
	sprintf ( pszBuffLogHeader, "%d: ", line );
	strLogMsgHeaderLineNumber = pszBuffLogHeader;

	// Construct the log message

	strLogMsgHeader.clear();

	// Indent so that the first line of the log message is obvious when word wrapping is enabled
	//strLogMsgHeader += "\t";
	strLogMsgHeader += strLogMsgHeaderDateTime;
	strLogMsgHeader	+= strLogMsgHeaderLogLevel;
	strLogMsgHeader	+= strLogMsgHeaderThreadId;
	strLogMsgHeader	+= strLogMsgHeaderFileName;
	strLogMsgHeader	+= strLogMsgHeaderLineNumber;

	if (NULL != pszBuffLogHeader)
	{
		delete[] pszBuffLogHeader;
		pszBuffLogHeader = NULL;
	}

	return strLogMsgHeader;
}

std::string CLogger::_ConstructMessage(int nStrLen, const std::string& file, int line, std::string& strLogLevel, va_list args)
{  
	std::string strLogMsgHeader;
	std::string strLogMsgBody;
	std::string strLogMsg; 	

	//construct log header
	strLogMsgHeader = _ConstructMsgHeader(DEF_INT_BUFFSIZE, file, line, strLogLevel);
	//construct log body 
	strLogMsgBody = _ConstructMsgBody(nStrLen, args);

	strLogMsg = strLogMsgHeader + strLogMsgBody;
	strLogMsg += "\n";

	return strLogMsg;
}





