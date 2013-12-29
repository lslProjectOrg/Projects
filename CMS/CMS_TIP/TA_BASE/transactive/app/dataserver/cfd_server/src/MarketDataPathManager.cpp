#include "MarketDataPathManager.h"

#include "MarketDataFileManager.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataPathManager::CMarketDataPathManager(void)
{
	m_strFilePathName.clear();
	m_pMarketDataFileManager = new CMarketDataFileManager();

}

CMarketDataPathManager::~CMarketDataPathManager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMarketDataFileManager)
	{
		delete m_pMarketDataFileManager;
		m_pMarketDataFileManager = NULL;
	}

}
int CMarketDataPathManager::setPathName(const std::string& strFilePathName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_strFilePathName = strFilePathName;
	return nFunRes;
}

int CMarketDataPathManager::analieAllFiles()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LOG_DEBUG<<"analie AllFiles m_strFilePathName="<<m_strFilePathName;

	//TODO.
	std::string strOneFileName = "G:\\LSL\\LSL_Code\\Svn_Work\\PUBLIC\\MarketData\\sample\\sample.csv";

	m_pMarketDataFileManager->setFileName(strOneFileName);
	nFunRes = m_pMarketDataFileManager->analierFile();

	return nFunRes;
}



NS_END(TA_Base_App) 



