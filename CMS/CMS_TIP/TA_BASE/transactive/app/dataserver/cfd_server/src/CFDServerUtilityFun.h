#ifndef __CLASS_CFDSERVER_UTILITY_FUN_H__
#define __CLASS_CFDSERVER_UTILITY_FUN_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include "MarketData.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App)


class CCFDServerUtilityFun
{
public:
	CCFDServerUtilityFun(void);	
	~CCFDServerUtilityFun(void);
public:
	std::string getTimeStringForlog(unsigned int nTimeValue);
	std::string getTimeStringForSQL(unsigned int nTimeValue);
public:
	void logMarketDataInfo(const std::string& strInfo, const MarketData &marketData);
	void logBarInfoEx(const std::string& strInfo, int interval, Bar* pBarInfo);

	time_t stringToDatetime(const std::string& strTimeValue);

};


NS_END(TA_Base_App)


#endif // __CLASS_CFDSERVER_UTILITY_FUN_H__









