#include "MarketDataFileManager.h"
#include "MarketDataFileAnalier.h"

#include "BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataFileManager::CMarketDataFileManager(enAnalierType nAnalierType)
{
	m_nAnalierType = nAnalierType;
	m_pMarketDataFileAnalier = new CMarketDataFileAnalier();
}

CMarketDataFileManager::~CMarketDataFileManager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMarketDataFileAnalier)
	{
		delete m_pMarketDataFileAnalier;
		m_pMarketDataFileAnalier = NULL;
	}

}

int CMarketDataFileManager::setAnalieType(enAnalierType nAnalierType)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	m_nAnalierType = nAnalierType;

	return nFunRes;
}


void CMarketDataFileManager::setInstrumentBarInfoRequest( const CInstrumentBarInfoRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;
	setAnalieType(AnalierType_Dispatch_MarkketData);

}

int CMarketDataFileManager::analierFile()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	switch (m_nAnalierType)
	{
	case AnalierType_Dispatch_MarkketData:
		m_pMarketDataFileAnalier->setInstrumentBarInfoRequest(m_InstrumentBarInfoRequest);
		nFunRes = m_pMarketDataFileAnalier->analierFile();
		break;
	default:
		nFunRes = -1;
		break;
	}

	return nFunRes;
}






NS_END(TA_Base_App) 



