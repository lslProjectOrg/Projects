#include "HelpClass.h"
#include "CommonData.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

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
	BOOST_LOG_FUNCTION();

	LOG_DEBUG<<"strLogInfo="<<strLogInfo
		<<" "<<"localAddress="<<stSessionInfo.localAddress
		<<" "<<"peerAddress="<<stSessionInfo.peerAddress
		<<" "<<"clientID="<<stSessionInfo.clientID
		<<" "<<"sessionID="<<stSessionInfo.sessionID
		<<" "<<"sessionMasks="<<stSessionInfo.sessionMasks;
}






NS_END(TA_Base_Test) 



