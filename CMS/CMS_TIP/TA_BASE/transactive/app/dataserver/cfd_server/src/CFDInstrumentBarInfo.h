#ifndef __CLASS_CFD_INSTRUMENT_BAR_INFO_H__
#define __CLASS_CFD_INSTRUMENT_BAR_INFO_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CBuildCFDMarketData;

class CCFDInstrumentBarInfo
{
public:
	CCFDInstrumentBarInfo(void);	
	~CCFDInstrumentBarInfo(void);
public:
	void setBarInfoFirst(const Bar&  nBarInfoFirst);
	void setBarInfoSecond(const Bar&  nBarInfoSecond);
	void setInterval(int nInterval);
public:
	/*
	note: DataFirst.Time == DataSecond.Time
	CFDMarketData = DataFirst.Time
	*/
	int  buidCFDBarInfo();
	void logInfo();
	void setCFDInstrumentIDFirst(unsigned int nCFDInstrumentIDFirst);
	void setCFDInstrumentIDSecond(unsigned int nCFDInstrumentIDSecond);
private:
	Bar m_nBarInfoFirst;
	Bar m_nBarInfoSecond;
	int m_nInterval;// = 5;//second
	unsigned int m_nCFDInstrumentIDFirst;// = 3620*3521;
	unsigned int m_nCFDInstrumentIDSecond;// = 3620*3521;
public:
	Bar m_CFDBarInfo;

};


NS_END(TA_Base_App)


#endif // __CLASS_CFD_INSTRUMENT_BAR_INFO_H__









