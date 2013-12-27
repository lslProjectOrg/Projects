#ifndef __CLASS_CFDSERVER_UTILITY_FUN_H__
#define __CLASS_CFDSERVER_UTILITY_FUN_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include "MessageBroker.h"

NS_BEGIN(TA_Base_App)


class CCFDServerUtilityFun
{
public:
	CCFDServerUtilityFun(void);	
	~CCFDServerUtilityFun(void);
public:
	std::string getTimeString(unsigned int nTimeValue);
	void logBarInfo(const std::string& strInfo, const Bar &bar);
};


NS_END(TA_Base_App)


#endif // __CLASS_CFDSERVER_UTILITY_FUN_H__









