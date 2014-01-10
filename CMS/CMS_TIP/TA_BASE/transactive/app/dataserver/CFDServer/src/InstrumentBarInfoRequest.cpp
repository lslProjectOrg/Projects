#include "InstrumentBarInfoRequest.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CInstrumentBarInfoRequest::CInstrumentBarInfoRequest(void)
{
	BOOST_LOG_FUNCTION();

	//c://TestData//HistoryMarketDataTotal   20131220.csv 
	//C://TestData//HistoryMarketDataInstrument   3320.csv
	//C://TestData//CFDBarInfoTotal   SQLiteDB_CFD_3320_3321.db
	//C://TestData//InstrumetBarInfoTotal   SQLiteDB_3320.db

	m_strHistoryMarketDataTotal = "c://TestData//HistoryMarketDataTotal";
	m_strInstrumetBarInfoTotal = "C://TestData//InstrumetBarInfoTotal";
	m_strCurrentAnalierFileName = m_strHistoryMarketDataTotal + "//" + "20131220.csv";
	
}

CInstrumentBarInfoRequest::~CInstrumentBarInfoRequest(void)
{
	BOOST_LOG_FUNCTION();
}


CInstrumentBarInfoRequest& CInstrumentBarInfoRequest::operator=( const CInstrumentBarInfoRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	m_strCurrentAnalierFileName = instrumentBarInfoRequest.m_strCurrentAnalierFileName;
	m_strHistoryMarketDataTotal = instrumentBarInfoRequest.m_strHistoryMarketDataTotal;
	m_strInstrumetBarInfoTotal = instrumentBarInfoRequest.m_strInstrumetBarInfoTotal;

	instrumentBarInfoRequest.getLstBarTime(m_lstBarTime);


	return *this;
}




void CInstrumentBarInfoRequest::logInfo()
{
	BOOST_LOG_FUNCTION();

	return;
}

void CInstrumentBarInfoRequest::addBarTimeNormal()
{
	BOOST_LOG_FUNCTION();
	m_lstBarTime.push_back(TA_Base_Core::TIME_BASE_S_5S);//seconds
	m_lstBarTime.push_back(TA_Base_Core::TIME_BASE_S_1MIN);//seconds
	m_lstBarTime.push_back(TA_Base_Core::TIME_BASE_S_5MIN);//seconds
	m_lstBarTime.push_back(TA_Base_Core::TIME_BASE_S_30MIN);//seconds
	m_lstBarTime.push_back(TA_Base_Core::TIME_BASE_S_1HOUR);//seconds
	m_lstBarTime.push_back(TA_Base_Core::TIME_BASE_S_1DAY);//seconds
	return;
}

void CInstrumentBarInfoRequest::addBarTime(int nSeconds)
{
	m_lstBarTime.push_back(nSeconds);//seconds

}



void CInstrumentBarInfoRequest::getLstBarTime( std::list<int>& lstBarTime )
{
	std::list<int>::iterator iterList;
	iterList = m_lstBarTime.begin();
	while (iterList != m_lstBarTime.end())
	{
		int nSeconds = *iterList;
		lstBarTime.push_back(nSeconds);
		iterList++;
	}
}

NS_END(TA_Base_App)
















