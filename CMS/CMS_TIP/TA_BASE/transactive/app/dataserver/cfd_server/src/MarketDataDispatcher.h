/*
Dispatcher MarketData class by SecurityID(InstumentID)
*/
#ifndef __CLASS_MARKET_DATA_DISPATCHER__HH__
#define __CLASS_MARKET_DATA_DISPATCHER__HH__

#include "CFDServerCommonData.h"

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
	void setInstrumentBarInfoRequest( const CInstrumentBarInfoRequest& instrumentBarInfoRequest );
private:
	int _StoreMemoryDataToDB();
	int _ClearDataInMap(MapInstrumentIDBarInfoCalcT* pMapInstrumentIDBarInfoCalc);

private:
	MapInstrumentIDBarInfoCalcT*   m_mapInstrumentIDBarInfoCalc;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;

private:
	unsigned int       m_nStoreAllDataMaxSize;
	unsigned int       m_nStoreAllDataMaxIndex;

};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_DISPATCHER__HH__


