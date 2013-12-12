#ifndef __CLASS_CLIENT_SESSION_ACTOR__HH__
#define __CLASS_CLIENT_SESSION_ACTOR__HH__

#include "UtilityFun.h"
#include "Message.h"
#include "MessageBroker.h"

NS_BEGIN(TA_Base_Test) 


class CClientSessionActor
{
public:
	CClientSessionActor(void);
	~CClientSessionActor(void);
public:
	void setSessionInfo(const cms::SessionInfo &stSessionInfo);
	cms::SessionID getSessionID();
public:
	cms::SessionInfo m_SessionInfo;


};

NS_END(TA_Base_Test) 


#endif //__CLASS_CLIENT_SESSION_ACTOR__HH__


