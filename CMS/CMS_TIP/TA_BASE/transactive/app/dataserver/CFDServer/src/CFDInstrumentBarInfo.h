#ifndef __CLASS_CFD_INSTRUMENT_BAR_INFO_H__
#define __CLASS_CFD_INSTRUMENT_BAR_INFO_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"
#include "CFDRequest.h"


NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CBuildCFDMarketData;

class CCFDInstrumentBarInfo
{
public:
	CCFDInstrumentBarInfo(const CCFDRequest& cfdRequest);	
	~CCFDInstrumentBarInfo(void);
public:
	void setBarInfoFirst(const MarketData&  nBarInfoFirst);
	void setBarInfoSecond(const MarketData&  nBarInfoSecond);

public:
	/*
	note: DataFirst.Time == DataSecond.Time
	CFDMarketData = DataFirst.Time
	*/
	int  buidCFDBarInfo();
	void logInfo();
	void setCFDRequest(const CCFDRequest& cfdRequest);
private:
	MarketData m_nBarInfoFirst;
	MarketData m_nBarInfoSecond;
	CCFDRequest m_CFDRequest;;
public:
	Bar m_CFDBarInfo;

};


NS_END(TA_Base_App)


#endif // __CLASS_CFD_INSTRUMENT_BAR_INFO_H__









