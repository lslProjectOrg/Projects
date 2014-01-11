#include "CFDRequest.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCFDRequest::CCFDRequest(void)
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentIDFirst = 3620;
	m_nCFDInstrumentIDSecond = 3620;
	m_nXValue = 1;
	m_nYValue = 1;
	m_nCFDInterval = TA_Base_Core::TIME_BASE_S_1MIN;


	
}

CCFDRequest::~CCFDRequest(void)
{
	BOOST_LOG_FUNCTION();
}


CCFDRequest& CCFDRequest::operator=( const CCFDRequest& cfdRequest )
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentIDFirst = cfdRequest.m_nCFDInstrumentIDFirst;
	m_nCFDInstrumentIDSecond = cfdRequest.m_nCFDInstrumentIDSecond;
	m_nXValue = cfdRequest.m_nXValue;
	m_nYValue = cfdRequest.m_nYValue;
	m_nCFDInterval = cfdRequest.m_nCFDInterval;
	m_strPathHistoryMarketDataInstrument = cfdRequest.m_strPathHistoryMarketDataInstrument;
	m_strCFDBarInfoTotal = cfdRequest.m_strCFDBarInfoTotal;

	return *this;
}




void CCFDRequest::logInfo()
{
	BOOST_LOG_FUNCTION();
	m_nCFDInstrumentIDFirst = 3620;
	m_nCFDInstrumentIDSecond = 3620;
	m_nXValue = 1;
	m_nYValue = 1;
	m_nCFDInterval = TA_Base_Core::TIME_BASE_S_1MIN;


	LOG_DEBUG<<"CCFDRequest: "
		<<" "<<"m_nCFDInstrumentIDFirst="<<m_nCFDInstrumentIDFirst
		<<" "<<"m_nCFDInstrumentIDSecond="<<m_nCFDInstrumentIDSecond
		<<" "<<"m_nXValue="<<m_nXValue
		<<" "<<"m_nYValue="<<m_nYValue
		<<" "<<"m_nCFDInterval="<<m_nCFDInterval;
	return;
}

NS_END(TA_Base_App)
















