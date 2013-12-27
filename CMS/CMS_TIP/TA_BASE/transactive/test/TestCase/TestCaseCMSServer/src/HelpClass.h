#ifndef __CLASS_HELPER__HH__
#define __CLASS_HELPER__HH__

#include "UtilityFun.h"

#include "MessageBroker.h"
#include "TestFrame.h"

NS_BEGIN(TA_Base_Test) 

using namespace cms;
class CHelpClass
{
public:
	CHelpClass(void);
	~CHelpClass(void);

public:
	void log_SeeionINfo(const SessionInfo &stSessionInfo, const std::string& strLogInfo);


};

NS_END(TA_Base_Test) 


#endif //__CLASS_HELPER__HH__


