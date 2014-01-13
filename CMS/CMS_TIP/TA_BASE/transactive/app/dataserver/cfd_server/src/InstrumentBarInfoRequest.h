#ifndef __CLASS_INSTRUMENT_BAR_INFO_REQUEST_H__
#define __CLASS_INSTRUMENT_BAR_INFO_REQUEST_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App)


class CInstrumentBarInfoRequest
{
public:
	CInstrumentBarInfoRequest(void);	
	~CInstrumentBarInfoRequest(void);

public:
	CInstrumentBarInfoRequest& operator=(const CInstrumentBarInfoRequest& cfdRequest);

public:
	void logInfo();
	void getLstBarTime(std::list<int>& lstBarTime);
private:
	void _AddBarTimeNormal();
	void _AddBarTime(int nSeconds);

public:
	//c://TestData//HistoryMarketDataTotal   20131220.csv 
	std::string m_strHistoryMarketDataTotal;

	//C://TestData//InstrumetBarInfoTotal   SQLiteDB_3320.db
	std::string m_strInstrumetBarInfoTotal;

public:
	//c://TestData//HistoryMarketDataTotal//20131220.csv 
	std::string m_strCurrentAnalierFileName;	
	
private:
	std::list<int>   m_lstBarTime;
};


NS_END(TA_Base_App)


#endif // __CLASS_INSTRUMENT_BAR_INFO_REQUEST_H__









