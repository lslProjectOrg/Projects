#ifndef __CLASS_CFD_REQUEST_H__
#define __CLASS_CFD_REQUEST_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App)


class CCFDRequest
{
public:
	CCFDRequest(void);	
	~CCFDRequest(void);

public:
	CCFDRequest& operator=(const CCFDRequest& cfdRequest);

public:
	void logInfo();


public:
	unsigned int m_nCFDInstrumentIDFirst;// = 3620
	unsigned int m_nCFDInstrumentIDSecond;// = 3521;
	int m_nXValue;
	int m_nYValue;
	int m_nCFDInterval;//CFD bar info
public:
	//c://TestData//HistoryMarketDataTotal   20131220.csv 
	//C://TestData//HistoryMarketDataInstrument   3320.csv
	//C://TestData//CFDBarInfoTotal   SQLiteDB_CFD_3320_3321.db

	std::string m_strPathHistoryMarketDataInstrument;
	std::string m_strCFDBarInfoTotal;

};


NS_END(TA_Base_App)


#endif // __CLASS_CFD_REQUEST_H__









