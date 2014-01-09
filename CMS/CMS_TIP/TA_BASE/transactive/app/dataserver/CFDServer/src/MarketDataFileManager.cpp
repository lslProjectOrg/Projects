#include "MarketDataFileManager.h"
#include "MarketDataFileAnalier.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataFileManager::CMarketDataFileManager()
{
	m_strFileName.clear();
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

void CMarketDataFileManager::setFileName(const std::string& strFileName)
{
	BOOST_LOG_FUNCTION();
	m_strFileName = strFileName;
	m_pMarketDataFileAnalier->setFileName(strFileName);

}


int CMarketDataFileManager::analierFile()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	m_pMarketDataFileAnalier->analierFile();

	return nFunRes;
}




NS_END(TA_Base_App) 



