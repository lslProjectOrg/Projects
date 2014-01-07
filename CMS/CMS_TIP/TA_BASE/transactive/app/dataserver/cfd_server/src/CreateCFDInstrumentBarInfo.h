#ifndef __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__
#define __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"
#include "SyncMarketDataForCFD.h"

NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CCFDInstrumentBarInfo;
class CCFDInstrumentBarInfoCalculator;

class CCreateCFDInstrumentBarInfo
{
public:
	CCreateCFDInstrumentBarInfo(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond);	
	~CCreateCFDInstrumentBarInfo(void);
public:
	void doOneTest();
	int UpdateCFDMarketData(CSyncMarketDataForCFD::LstCFDBarInfoT& lstCFDBarInfo);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CSyncMarketDataForCFD*   m_CSyncMarketDataForCFD;
	CCFDInstrumentBarInfoCalculator*   m_pCFDBarInfoCalculator;
	unsigned int m_nInstrumentIDFirest;
	unsigned int m_nInstrumentIDSecond;
};


NS_END(TA_Base_App)


#endif // __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__









