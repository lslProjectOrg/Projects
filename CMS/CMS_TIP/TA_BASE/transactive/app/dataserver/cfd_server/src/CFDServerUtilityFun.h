#ifndef __CLASS_CFDSERVER_UTILITY_FUN_H__
#define __CLASS_CFDSERVER_UTILITY_FUN_H__ 


#include "CFDServerCommonData.h"

#include "MarketData.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App)

class CAWorkTime;

class CCFDServerUtilityFun
{
public:
	CCFDServerUtilityFun(void);	
	~CCFDServerUtilityFun(void);
public:
	//"%d-%d-%d %d:%d:%d"
	time_t strToDateTime(const std::string& strTimeValue);

	//"%04d-%02d-%02d %02d:%02d:%02d"
	std::string dataTimeToStr(time_t nTimeValue);
public:
	void logMarketDataInfo(const std::string& strInfo, const MarketData &marketData);
	void logBarInfo(const std::string& strInfo, int interval, Bar* pBarInfo);

	CAWorkTime*  m_pWorkTime;


};


NS_END(TA_Base_App)


#endif // __CLASS_CFDSERVER_UTILITY_FUN_H__









