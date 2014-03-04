/*
Calculator Instument's Bar Info
*/
#ifndef __CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__
#define __CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__

#include "CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MarketData.h"
#include "BarCalculator.h"
#include "InstrumentBarInfoRequest.h"
#include "DBOperParam.h"

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;
class CDataArrayBuffer;

class CInstrumentBarInfoCalculator 
{
private:
	typedef std::map<int, Bar*>              MapIntervalBarInfoT;//seconds
	typedef std::map<int, Bar*>::iterator    MapIntervalBarInfoIterT;
	typedef std::map<int, Bar*>::value_type    MapIntervalBarInfoValueTypeT;
public:
	CInstrumentBarInfoCalculator(unsigned int nInstrumentID, const CInstrumentBarInfoRequest& instrumentBarInfoRequest);
	~CInstrumentBarInfoCalculator(void);

public:
	int onMarketDataUpdateForBar(const MarketData& marketData);
	int onMarketDataUpdateForTick(const MarketData& marketData);

	int storeMemoryDataToDB();
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
	CDataArrayBuffer* m_pStorager;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;
	CDBOperParam  m_DBOperParam;

};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_CALCULATOR__HH__


