#include "HelpClass.h"
#include "CommonData.h"
#include "Logger.h"
#include "TestFrame.h"



// extern boost::condition_variable g_conditionMainRun;

NS_BEGIN(TA_Base_Test) 


//////////////////////////////////////////////////////////////////////////

CHelpClass::CHelpClass(void)
{
	
}

CHelpClass::~CHelpClass(void)
{

}

void CHelpClass::log_SeeionINfo(const SessionInfo &stSessionInfo, const std::string& strLogInfo)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugTrace, "TestCaseCMSServer::_LOG_SeeionINfo");

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, localAddress=%s, peerAddress=%s, clientID=%d, sessionID=%d, sessionMasks=%lld",
		strLogInfo.c_str(),
		stSessionInfo.localAddress.c_str(),
		stSessionInfo.peerAddress.c_str(),
		stSessionInfo.clientID,
		stSessionInfo.sessionID,
		stSessionInfo.sessionMasks);

}






NS_END(TA_Base_Test) 



