/*
Calculator CFDInstrument's Bar Info
*/
#ifndef __CLASS_CFD_INSTUMENT_BAR_INFO_CALCULATOR__HH__
#define __CLASS_CFD_INSTUMENT_BAR_INFO_CALCULATOR__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MarketData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;
class CInstrumentBarInfoStorager;

class CCFDInstrumentBarInfoCalculator 
{
public:
	typedef std::map<int, Bar*>              MapIntervalBarInfoT;//seconds
	typedef std::map<int, Bar*>::iterator    MapIntervalBarInfoIterT;
	typedef std::map<int, Bar*>::value_type    MapIntervalBarInfoValueTypeT;
public:
	CCFDInstrumentBarInfoCalculator(unsigned int nInstrumentID);
	~CCFDInstrumentBarInfoCalculator(void);

public:
	int updateMarketData(const MarketData& marketData);
private:
	int _ClearDataInMap(MapIntervalBarInfoT* pMapTimeBarInfo);
	void HandleNewBar(int interval, const Bar &bar);
	void HandleUpdateBar(int interval, const Bar &bar);
private:
	BarCalculator*   m_pBarCalculator;
	MapIntervalBarInfoT*   m_pMapTimeBarInfo;
	CCFDServerUtilityFun*  m_pUtilityFun;
	unsigned int m_nInstrumentID;
	CInstrumentBarInfoStorager* m_pStorager;

};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__


