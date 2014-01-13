#include "InstrumentMarketDataRequest.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CInstrumentMarketDataRequest::CInstrumentMarketDataRequest(void)
{
	BOOST_LOG_FUNCTION();


	//c://TestData//HistoryMarketDataTotal   20131220.csv 
	//C://TestData//HistoryMarketDataInstrument   3320.csv
	//C://TestData//CFDBarInfoTotal   SQLiteDB_CFD_3320_3321.db
	//C://TestData//InstrumetBarInfoTotal   SQLiteDB_3320.db

	m_strHistoryMarketDataTotal = "c://TestData//HistoryMarketDataTotal";
	m_strHistoryMarketDataInstrument = "C://TestData//HistoryMarketDataInstrument";
	m_strCurrentAnalierFileName = m_strHistoryMarketDataTotal + "//" + "20131220.csv";
	
}

CInstrumentMarketDataRequest::~CInstrumentMarketDataRequest(void)
{
	BOOST_LOG_FUNCTION();
}


CInstrumentMarketDataRequest& CInstrumentMarketDataRequest::operator=( const CInstrumentMarketDataRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	m_strCurrentAnalierFileName = instrumentBarInfoRequest.m_strCurrentAnalierFileName;
	m_strHistoryMarketDataTotal = instrumentBarInfoRequest.m_strHistoryMarketDataTotal;
	m_strHistoryMarketDataInstrument = instrumentBarInfoRequest.m_strHistoryMarketDataInstrument;

	return *this;
}




void CInstrumentMarketDataRequest::logInfo()
{
	BOOST_LOG_FUNCTION();

	return;
}


NS_END(TA_Base_App)
















