#ifndef __CLASS_INSTRUMENT_MARKET_DATA_REQUEST_H__
#define __CLASS_INSTRUMENT_MARKET_DATA_REQUEST_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App)


class CInstrumentMarketDataRequest
{
public:
	CInstrumentMarketDataRequest(void);	
	~CInstrumentMarketDataRequest(void);

public:
	CInstrumentMarketDataRequest& operator=(const CInstrumentMarketDataRequest& cfdRequest);

public:
	void logInfo();
public:
	//c://TestData//HistoryMarketDataTotal   20131220.csv 
	//C://TestData//HistoryMarketDataInstrument   3320.csv
	//C://TestData//CFDBarInfoTotal   SQLiteDB_CFD_3320_3321.db
	//C://TestData//InstrumetBarInfoTotal   SQLiteDB_3320.db

	std::string m_strHistoryMarketDataTotal;
	std::string m_strHistoryMarketDataInstrument;

public:
	std::string m_strCurrentAnalierFileName;	//c://TestData//HistoryMarketDataTotal//20131220.csv 
	
};


NS_END(TA_Base_App)


#endif // __CLASS_INSTRUMENT_MARKET_DATA_REQUEST_H__









