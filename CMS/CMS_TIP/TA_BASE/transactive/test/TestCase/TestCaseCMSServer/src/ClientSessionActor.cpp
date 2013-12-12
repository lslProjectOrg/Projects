#include "ClientSessionActor.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 


CClientSessionActor::CClientSessionActor(void)
{
	BOOST_LOG_FUNCTION();

}

CClientSessionActor::~CClientSessionActor(void)
{
	BOOST_LOG_FUNCTION();

}

void CClientSessionActor::setSessionInfo( const cms::SessionInfo &stSessionInfo )
{
	m_SessionInfo.peerAddress = stSessionInfo.peerAddress;
	m_SessionInfo.sessionID = stSessionInfo.sessionID;

	m_SessionInfo.clientID = stSessionInfo.clientID;
	m_SessionInfo.sessionMasks = stSessionInfo.sessionMasks;
	m_SessionInfo.localAddress = stSessionInfo.localAddress;
}

cms::SessionID CClientSessionActor::getSessionID()
{
	return m_SessionInfo.sessionID;
}



NS_END(TA_Base_Test) 



