#ifndef __CLASS_HISTORY_DATA_SERVER_UTILITY_FUN_H__
#define __CLASS_HISTORY_DATA_SERVER_UTILITY_FUN_H__ 


#include "HistoryDataServerCommonData.h"


NS_BEGIN(TA_Base_App)


class CHistoryDataServerUtilityFun
{
public:
	CHistoryDataServerUtilityFun(void);	
	~CHistoryDataServerUtilityFun(void);
public:
	//"%d-%d-%d %d:%d:%d"
	time_t strToDateTime(const std::string& strTimeValue);

	//"%04d-%02d-%02d %02d:%02d:%02d"
	std::string dataTimeToStr(time_t nTimeValue);



};


NS_END(TA_Base_App)


#endif // __CLASS_HISTORY_DATA_SERVER_UTILITY_FUN_H__









