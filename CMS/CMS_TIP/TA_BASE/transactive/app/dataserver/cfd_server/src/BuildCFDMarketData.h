#ifndef __CLASS_BUILD_CFD_MARKET_DATA_H__
#define __CLASS_BUILD_CFD_MARKET_DATA_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"
NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;

class CBuildCFDMarketData
{
public:
	CBuildCFDMarketData(void);	
	~CBuildCFDMarketData(void);
public:
	/*
	note: DataFirst.Time == DataSecond.Time
	CFDMarketData = DataFirst.Time
	*/
	int buildCFDMarketData(const Bar &DataFirst, const Bar &DataSecond, Bar &CFDMarketData, int interval);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
};


NS_END(TA_Base_App)


#endif // __CLASS_BUILD_CFD_MARKET_DATA_H__









