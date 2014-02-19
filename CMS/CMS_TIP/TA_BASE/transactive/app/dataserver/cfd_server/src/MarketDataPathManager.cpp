#include "MarketDataPathManager.h"


#include "MarketDataFileManager.h"
#include "FileSystemItem.h"
#include "FileSystemManager.h"


#include "BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataPathManager::CMarketDataPathManager(void)
{
	m_pMarketDataFileManager = new CMarketDataFileManager(CMarketDataFileManager::AnalierType_Begin);
	m_pFileSystemManager = new CFileSystemManager();

}

CMarketDataPathManager::~CMarketDataPathManager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMarketDataFileManager)
	{
		delete m_pMarketDataFileManager;
		m_pMarketDataFileManager = NULL;
	}

	if (NULL != m_pFileSystemManager)
	{
		delete m_pFileSystemManager;
		m_pFileSystemManager = NULL;
	}

}
int CMarketDataPathManager::_SetAnalieType(CMarketDataFileManager::enAnalierType nAnalierType)
{	
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	m_nAnalierType = nAnalierType;
	m_pMarketDataFileManager->setAnalieType(m_nAnalierType);

	return nFunRes;
}

int CMarketDataPathManager::analieAllFiles()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	switch (m_nAnalierType)
	{
	case CMarketDataFileManager::AnalierType_Dispatch_MarkketData:
		_AnalieAllFilesTypeMarketData();
		break;
	}
	return nFunRes;
}



int CMarketDataPathManager::_AnalieAllFilesTypeMarketData()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CFileSystemManager::MapTimeFileSystemItemT mapTimeFileSystemItemTmp;
	CFileSystemManager::MapTimeFileSystemItemIterT iterMap;
	CFileSystemItem* pFileSystemItem = NULL;

	if (CMarketDataFileManager::AnalierType_Dispatch_MarkketData != m_nAnalierType)
	{
		nFunRes = -1;
		return nFunRes;
	}

	LOG_INFO<<"check and create Directory ="<<m_InstrumentBarInfoRequest.m_strInstrumetBarInfoTotal;
	m_pFileSystemManager->createDirectory(m_InstrumentBarInfoRequest.m_strInstrumetBarInfoTotal);

	LOG_INFO<<"analies Directory ="<<m_InstrumentBarInfoRequest.m_strHistoryMarketDataTotal;
	m_pFileSystemManager->getAllFileSystemItemInPath(m_InstrumentBarInfoRequest.m_strHistoryMarketDataTotal, mapTimeFileSystemItemTmp);

	iterMap = mapTimeFileSystemItemTmp.begin();
	while (iterMap != mapTimeFileSystemItemTmp.end())
	{
		pFileSystemItem = iterMap->second;

		m_InstrumentBarInfoRequest.m_strCurrentAnalierFileName = pFileSystemItem->getFileFullPath();

		m_pMarketDataFileManager->setInstrumentBarInfoRequest(m_InstrumentBarInfoRequest);
		nFunRes = m_pMarketDataFileManager->analierFile();

		iterMap++;
	}


	m_pFileSystemManager->freeData(mapTimeFileSystemItemTmp);
	mapTimeFileSystemItemTmp.clear();

	return nFunRes;
}

int CMarketDataPathManager::setInstrumentBarInfoRequest(const CInstrumentBarInfoRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;
	_SetAnalieType(CMarketDataFileManager::AnalierType_Dispatch_MarkketData);
	return nFunRes;
}


NS_END(TA_Base_App) 



