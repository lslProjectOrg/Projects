/*
Calculator Instument's Bar Info
*/
#ifndef __CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__
#define __CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MarketData.h"
#include "BarCalculator.h"
#include "InstrumentBarInfoRequest.h"

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;
class CInstrumentBarInfoStorager;

class CInstrumentBarInfoCalculator 
{
public:
	typedef std::map<int, Bar*>              MapIntervalBarInfoT;//seconds
	typedef std::map<int, Bar*>::iterator    MapIntervalBarInfoIterT;
	typedef std::map<int, Bar*>::value_type    MapIntervalBarInfoValueTypeT;
public:
	CInstrumentBarInfoCalculator(unsigned int nInstrumentID, const CInstrumentBarInfoRequest& instrumentBarInfoRequest);
	~CInstrumentBarInfoCalculator(void);

public:
	int updateMarketData(const MarketData& marketData);

private:
	void HandleNewBar(int interval, const Bar &bar);
	void HandleUpdateBar(int interval, const Bar &bar);

	int _ClearDataInMap(MapIntervalBarInfoT* pMapTimeBarInfo);
	int _InitBarCalculator();
private:
	BarCalculator*   m_pBarCalculator;
	MapIntervalBarInfoT*   m_pMapTimeBarInfo;
	CCFDServerUtilityFun*  m_pUtilityFun;
	unsigned int m_nInstrumentID;
	CInstrumentBarInfoStorager* m_pStorager;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;

};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__


