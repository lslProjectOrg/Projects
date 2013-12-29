#include "MarketDataFileAnalier.h"

#include "MarketDataDispatcher.h"
#include "CFDServerUtilityFun.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataFileAnalier::CMarketDataFileAnalier()
{
	BOOST_LOG_FUNCTION();

	m_strFileName.clear();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pMarketDataDispatcher = new CMarketDataDispatcher();

}

CMarketDataFileAnalier::~CMarketDataFileAnalier(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMarketDataDispatcher)
	{
		delete m_pMarketDataDispatcher;
		m_pMarketDataDispatcher = NULL;
	}

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

}

void CMarketDataFileAnalier::setFileName(const std::string& strFileName)
{
	BOOST_LOG_FUNCTION();
	m_strFileName = strFileName;
}

int CMarketDataFileAnalier::analierFile()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	
	//std::string filename = "C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample\\sample.csv";
	std::string filename = m_strFileName;
	std::ifstream priceFile(filename.c_str());
	std::string strLogInfo = "get data from file";

	LOG_DEBUG<<"AnalierFile: "<<m_strFileName;

	while (!priceFile.eof())
	{
		MarketData marketDataTmp(0);
		priceFile >> marketDataTmp;
		//process price ....

		m_pUtilityFun->logMarketDataInfo(strLogInfo, marketDataTmp);
	
		m_pMarketDataDispatcher->dispatcherMarketData(marketDataTmp);
	}

	priceFile.close();

	return nFunRes;
}



NS_END(TA_Base_App) 



