#ifndef __CLASS_PROJECT_UTILITY_FUN_H__
#define __CLASS_PROJECT_UTILITY_FUN_H__ 


#include "ProjectCommonData.h"

#include "TradeClient.h"


NS_BEGIN(TA_App_App)

class CProjectUtilityFun
{
public:
	CProjectUtilityFun(void);	
	~CProjectUtilityFun(void);
public:
	//"%d-%d-%d %d:%d:%d"
	time_t strToDateTime(const std::string& strTimeValue);

	//"%04d-%02d-%02d %02d:%02d:%02d"
	std::string dataTimeToStr(time_t nTimeValue);

public:
	void log_Instrument_info(const std::string& strLogInfo, const Instrument &instrument );
};


NS_END(TA_App_App)


#endif // __CLASS_PROJECT_UTILITY_FUN_H__









