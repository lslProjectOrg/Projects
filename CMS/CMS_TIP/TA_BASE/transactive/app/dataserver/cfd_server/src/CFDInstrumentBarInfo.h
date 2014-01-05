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
	void setCFDInstrumentID(int nCFDInstrumentID);
public:
	int  buidCFDBarInfo();
private:
	Bar m_nBarInfoFirst;
	Bar m_nBarInfoSecond;
	int m_nInterval;// = 5;//second
private:
	int m_nCFDInstrumentID;// = 3620*3521;
	Bar m_CFDBarInfo;
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CBuildCFDMarketData*     m_pBuildCFDMarketData;

};


NS_END(TA_Base_App)


#endif // __CLASS_CFD_INSTRUMENT_BAR_INFO_H__









