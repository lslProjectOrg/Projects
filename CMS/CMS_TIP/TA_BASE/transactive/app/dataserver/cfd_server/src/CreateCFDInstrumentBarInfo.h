#ifndef __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__
#define __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CCFDInstrumentBarInfo;
class CSyncMarketDataForCFD;
class CCFDInstrumentBarInfoCalculator;

class CCreateCFDInstrumentBarInfo
{
public:
	CCreateCFDInstrumentBarInfo(void);	
	~CCreateCFDInstrumentBarInfo(void);
public:
	void doOneTest();
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CSyncMarketDataForCFD*   m_CSyncMarketDataForCFD;
	CCFDInstrumentBarInfoCalculator*   m_pCFDBarInfoCalculator;
};


NS_END(TA_Base_App)


#endif // __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__









