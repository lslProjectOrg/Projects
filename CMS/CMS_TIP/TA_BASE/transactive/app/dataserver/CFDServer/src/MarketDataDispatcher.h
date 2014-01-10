/*
Dispatcher MarketData class by SecurityID(InstumentID)
*/
#ifndef __CLASS_MARKET_DATA_DISPATCHER__HH__
#define __CLASS_MARKET_DATA_DISPATCHER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "MarketData.h"
#include "InstrumentBarInfoRequest.h"


NS_BEGIN(TA_Base_App) 

class CInstrumentBarInfoCalculator;

class CMarketDataDispatcher 
{
public:
	typedef std::map<unsigned int, CInstrumentBarInfoCalculator*>				MapInstrumentIDBarInfoCalcT;
	typedef std::map<unsigned int, CInstrumentBarInfoCalculator*>::iterator		MapInstrumentIDBarInfoCalcIterT;
	typedef std::map<unsigned int, CInstrumentBarInfoCalculator*>::value_type   MapInstrumentIDBarInfoCalcValueTypeT;

public:
	CMarketDataDispatcher(void);
	~CMarketDataDispatcher(void);

public:
	int dispatcherMarketData(const MarketData& marketData);
	int _ClearDataInMap(MapInstrumentIDBarInfoCalcT* pMapInstrumentIDBarInfoCalc);
	void setInstrumentBarInfoRequest( const CInstrumentBarInfoRequest& instrumentBarInfoRequest );
private:
	MapInstrumentIDBarInfoCalcT*   m_mapInstrumentIDBarInfoCalc;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;

};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_DISPATCHER__HH__


