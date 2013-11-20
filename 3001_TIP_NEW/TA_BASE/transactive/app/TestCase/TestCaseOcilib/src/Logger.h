/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/Logger.h $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */




#ifndef __DEF_CLASS_LOGGER_HH__
#define __DEF_CLASS_LOGGER_HH__

#include "CommonDef.h"
#include "CommonData.h"

// Forward declaration


// Class Definition: Logger
class CLogger
{

public:
	static CLogger& getInstance();
	static void removeInstance();

private:
	static CLogger* m_pInstance;
	
private:
	CLogger();
	~CLogger();

public:
	void setlogLevel(EDebugLevel nLoglevel);
	int  setlogFile(const std::string& strLogFileName);
	void deletelogFile();

	void log(const std::string& file, int line, EDebugLevel level, ...);

private:	
	std::string _ConstructMessage(int nStrLen, const std::string& file, int line, std::string& strLogLevel, va_list args);
														  
	std::string _ConstructMsgHeader(int nStrLen, const std::string& file, int line, std::string& strLogLevel);
	std::string _ConstructMsgBody(int nStrLen, va_list args);
	std::string _GetStringDebugLevel(EDebugLevel level);
private:
	std::string			m_strLogFileName;
	std::ofstream		m_streamLogFile;
	EDebugLevel			m_nCurrDebugLevel;

};



#endif // __DEF_CLASS_LOGGER_HH__


