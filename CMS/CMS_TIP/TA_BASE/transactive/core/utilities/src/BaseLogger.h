#ifndef __CLASS_BASE_LOGGER_HH__
#define __CLASS_BASE_LOGGER_HH__

#include "core/utilities/src/UtilitiesCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "BoostUtilityFun.h"

// Forward declaration

#define BaseLogSourceFLInfo			__FILE__,__LINE__

#define _BaseLog			    TA_Base_Core::CBaseLogger::getInstance().log


#define  defSQLFilename_OUT_ERRORREPORT   "BASE_LOG.log"



NS_BEGIN(TA_Base_Core)

//log
// Debug levels are sequentially indexed starting at 0.  It is possible to iterate 
// through all debug levels by iterating through the integral sequence [ 0, countDebugLevels() ).
// The current debug level determines the highest debugging level that will be logged.
enum EDebugLevel
{
	DebugFatal		= 0L,	// fatal error conditions
	DebugError,		    // error conditions
	DebugWarn,		        // warning conditions
	DebugNorm,		        // normal conditions
	DebugInfo,		        // informational messages
	DebugDebug,		    // debug-level messages
	DebugSQL,              // SQL query strings
	DebugCORBA,            // for CORBA debug messages (only if other flags are enabled - see CorbaUtil)
	DebugMsg,              // for Messaging library debug messages
	DebugMsgPub,           // for Messaging library message sending and receiving debug messages
	DebugPeriodic,         // messages logged periodically by keepalives, etc
	DebugDatabase,      	// data access (SimpleDB) messages (other than SQL query strings)
	DebugExternal,      	// 3rd party (non-transactive) messages
	DebugTrace,		    // Function Entry/Exit Messages
};


struct DebugLevelNameValue
{
	const char*   pLevelName;
	EDebugLevel   nLevelValue;
};

// Class Definition: Logger
class CBaseLogger
{
public:
	static CBaseLogger& getInstance();
	static void removeInstance();

private:
	static CBaseLogger* m_pInstance;
	static boost::mutex m_mutexInstance;
	static boost::mutex m_mutexStream;

private:
	CBaseLogger();
	~CBaseLogger();

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

	unsigned int _PthreadSelf();

private:
	std::string			m_strLogFileName;
	std::ofstream		m_streamLogFile;
	EDebugLevel			m_nCurrDebugLevel;

public:
	void CBaseLogger::testLog();
};



#endif // __CLASS_BASE_LOGGER_HH__

NS_END(TA_Base_Core)




